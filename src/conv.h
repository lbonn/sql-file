#ifndef CONV_H
#define CONV_H

#include <cstdlib>
#include <unistd.h>
#include <filesystem>

#include "sql_utils.h"

// from auto-generated schema source file
extern const std::vector<std::string> schema_migrations;
extern const std::vector<std::string> schema_rollback_migrations;
extern const std::string current_schema;
extern const int current_schema_version;

namespace conv {

class Converter {
 public:

  Converter(const char *db_path, const std::string& conv_name) :
      db_path_(db_path), conv_name_(conv_name) {
          if (!std::filesystem::is_regular_file(db_path)) {
              throw std::runtime_error(std::string("could not find ") + db_path);
          }

          int dbVersion = getVersion();
          if (dbVersion != current_schema_version) {
              std::cerr << "db version mismatch, attempting migration...\n";

              // make a copy to preserve the original
              int fd = mkstemp(tmpfn_);
              if (fd < 0) {
                  throw std::runtime_error("could not get temporary file");
              }
              close(fd);
              std::filesystem::copy_file(db_path, tmpfn_, std::filesystem::copy_options::overwrite_existing);
              db_path_ = tmpfn_;

              if (dbVersion > current_schema_version) {
                  dbMigrateBackward(dbVersion);
              } else {
                  dbMigrateForward(dbVersion);
              }
          }
      }

  sql::Guard guard() {
      return sql::Guard(db_path_, false);
  }

  int getVersion() {
      auto db = guard();
      auto statement = db.prepareStatement("SELECT version from version LIMIT 1;");
      int result = statement.step();
      if (result != SQLITE_ROW) {
          throw std::runtime_error("Could not find database version");
      }
      return statement.get_result_col_int(0);
  }

  void dbMigrateForward(int version_from) {
      int version_to = current_schema_version;

      auto db = guard();

      if (!db.beginTransaction()) {
          throw std::runtime_error("migration failed");
      }

      for (int32_t k = version_from + 1; k <= version_to; k++) {
          auto result_code = db.exec(schema_migrations.at(static_cast<size_t>(k)), nullptr, nullptr);
          if (result_code != SQLITE_OK) {
              std::cerr << "Can't migrate DB from version " << (k - 1) << " to version " << k << ": " << db.errmsg();
              throw std::runtime_error("migration failed");
          }
      }

      insertBackMigrations(db, version_to);

      db.commitTransaction();
  }

  void dbMigrateBackward(int version_from) {
      int version_to = current_schema_version;

      auto db = guard();

      if (!db.beginTransaction()) {
          throw std::runtime_error("migration failed");
      }

      for (int ver = version_from; ver > version_to; --ver) {
          std::string migration;
          {
              // make sure the statement is destroyed before the next database operation
              auto statement = db.prepareStatement("SELECT migration FROM rollback_migrations WHERE version_from=?;", ver);
              if (statement.step() != SQLITE_ROW) {
                  throw std::runtime_error("migration failed");
              }
              migration = *(statement.get_result_col_str(0));
          }

          if (db.exec(migration, nullptr, nullptr) != SQLITE_OK) {
              std::cerr << "Can't migrate db from version " << (ver) << " to version " << ver - 1 << ": " << db.errmsg();
              throw std::runtime_error("migration failed");
          }
          if (db.exec(std::string("DELETE FROM rollback_migrations WHERE version_from=") + std::to_string(ver) + ";", nullptr,
                      nullptr) != SQLITE_OK) {
              std::cerr << "Can't clear old migration script: " << db.errmsg();
              throw std::runtime_error("migration failed");
          }
      }

      db.commitTransaction();
  }

  void insertBackMigrations(sql::Guard& db, int version_latest) {
      if (schema_rollback_migrations.empty()) {
          return;
      }

      if (schema_rollback_migrations.size() < static_cast<size_t>(version_latest) + 1) {
          std::cerr << "Backward migrations from " << schema_rollback_migrations.size() << " to " << version_latest
              << " are missing";
          throw std::runtime_error("inserting backward migrations failed");
      }

      for (int k = 1; k <= version_latest; k++) {
          if (schema_rollback_migrations.at(static_cast<size_t>(k)).empty()) {
              continue;
          }
          auto statement = db.prepareStatement("INSERT OR REPLACE INTO rollback_migrations VALUES (?,?);", k,
                  schema_rollback_migrations.at(static_cast<uint32_t>(k)));
          if (statement.step() != SQLITE_DONE) {
              throw std::runtime_error("inserting backward migrations failed");
          }
      }
  }

  std::string descr();
  double apply(const std::string& date, double val);

 private:
  const char *db_path_;
  const std::string conv_name_;
  char tmpfn_[15]{"/tmp/cc_XXXXXX"};
};

}  // namespace

#endif
