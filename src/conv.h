#ifndef CONV_H
#define CONV_H

#include <filesystem>

#include "sql_utils.h"

namespace conv {

class Converter {
 public:

  Converter(const char *db_path, const std::string& conv_name) :
      db_path_(db_path), conv_name_(conv_name) {
      if (!std::filesystem::is_regular_file(db_path)) {
          throw std::runtime_error(std::string("could not find ") + db_path);
      }
  }

  sql::Guard guard() {
      return sql::Guard(db_path_, false);
  }

  std::string descr();
  double apply(const std::string& date, double val);

 private:
  const char *db_path_;
  const std::string conv_name_;
};

}  // namespace

#endif
