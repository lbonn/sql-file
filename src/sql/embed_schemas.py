#!/usr/bin/env python3

import sys
import os
import re

warning_text = '''
//This is autogenerated file, don't modify it manually
//Latest schema timestamp: %d
#include <string>
#include <vector>
#include <map>
'''

def escape_string(sql):
    return sql.translate(str.maketrans({'"':  r'\"', '\n':  '\\n'}))


def append_migration(migration_path, source):
    migration_file = open(migration_path, 'r')
    source.write("\"")
    migration_content = migration_file.read()
    source.write(escape_string(migration_content))


if __name__ == '__main__':
    if len(sys.argv) != 6:
        print("\nIncorrect arguments")
        print("Usage:\n {} {} {} {} {} {}\n".format(sys.argv[0], "schemas_dir", "schemav", "schema",
                                                 "output_source", "name_prefix"))
        sys.exit(-1)

    sql_dir = sys.argv[1]
    schemav = int(sys.argv[2])
    schema = sys.argv[3]
    schemas_output = sys.argv[4]
    prefix = sys.argv[5]

    migration_dir = os.path.join(sql_dir, 'migration')
    rollback_dir = os.path.join(sql_dir, 'rollback')
    migration_list = sorted(os.listdir(migration_dir))[:schemav+1]
    rollback_migrations_list = sorted(os.listdir(rollback_dir))[:schemav]

    file_depends_list = [os.path.join(sql_dir, schema)]
    file_depends_list += [os.path.join(migration_dir, p) for p in migration_list]
    file_depends_list += [os.path.join(rollback_dir, p) for p in rollback_migrations_list]
    file_depends_list.append(__file__)  # set dependency on itself (this script)
    max_file_stamp = max(os.path.getmtime(p) for p in file_depends_list)

    pattern = re.compile(r"//Latest schema timestamp: (\d+)")
    if os.path.exists(schemas_output):
        for i, line in enumerate(open(schemas_output)):
            for match in re.finditer(pattern, line):
                if int(match.groups()[0]) == int(max_file_stamp):
                    # header up to date, exiting
                    sys.exit(0)

    with open(schemas_output, 'w') as source:
        source.write(warning_text % max_file_stamp)
        source.write("extern const std::vector<std::string> {}_schema_migrations = {{".format(prefix))
        for migration in migration_list[:-1]:
            append_migration(os.path.join(migration_dir, migration), source)
            source.write("\",\n")
        append_migration(os.path.join(migration_dir, migration_list[-1]), source)
        source.write("\"\n};\n")

        source.write("extern const std::vector<std::string> {}_schema_rollback_migrations = {{".format(prefix))
        if len(rollback_migrations_list) > 0:
            ver = int(rollback_migrations_list[0].split(".")[1])
            for i in range(ver):
                source.write("\"\",\n")
            for migration in rollback_migrations_list[:-1]:
                append_migration(os.path.join(rollback_dir, migration), source)
                source.write("\",\n")
            version = int(rollback_migrations_list[-1].split(".")[1])
            append_migration(os.path.join(rollback_dir, rollback_migrations_list[-1]), source)
            source.write("\"")
        source.write("\n};\n")

        current_schema = open(os.path.join(sql_dir, schema), 'r').read()
        current_schema_escaped = escape_string(current_schema)
        source.write('extern const std::string %s_current_schema = "%s";' % (prefix, current_schema_escaped));
        source.write('extern const int %s_current_schema_version = %u;' % (prefix ,(len(migration_list)-1)));
