cmake_minimum_required (VERSION 3.12)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE REQUIRED sqlite3)

add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>" "$<$<CONFIG:DEBUG>:-Og>")


add_library(currency_convert OBJECT src/currency_convert.cc)
set_target_properties(currency_convert PROPERTIES
    CXX_STANDARD 17
    )

add_custom_command(OUTPUT sql_schemas_v0.cc
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/sql/embed_schemas.py ${PROJECT_SOURCE_DIR}/src/sql/ 0 schema.0.sql ${CMAKE_CURRENT_BINARY_DIR}/sql_schemas_v0.cc currconv
)

add_executable(currency_convert_v0 $<TARGET_OBJECTS:currency_convert> src/conv_v0.cc sql_schemas_v0.cc)
target_link_libraries(currency_convert_v0 ${SQLITE_LIBRARIES} stdc++fs)
set_target_properties(currency_convert_v0 PROPERTIES
    CXX_STANDARD 17
    )

add_custom_command(OUTPUT sql_schemas_v1.cc
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/sql/embed_schemas.py ${PROJECT_SOURCE_DIR}/src/sql/ 1 schema.1.sql ${CMAKE_CURRENT_BINARY_DIR}/sql_schemas_v1.cc currconv
)

add_executable(currency_convert_v1 $<TARGET_OBJECTS:currency_convert> src/conv_v1.cc sql_schemas_v1.cc)
target_link_libraries(currency_convert_v1 ${SQLITE_LIBRARIES} stdc++fs)
set_target_properties(currency_convert_v1 PROPERTIES
    CXX_STANDARD 17
    )

# test

add_test(NAME convs COMMAND ${PROJECT_SOURCE_DIR}/test/test.sh ${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/data)
enable_testing(ON)
