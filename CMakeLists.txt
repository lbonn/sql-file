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

add_custom_command(OUTPUT sql_schemas_v1.cc
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/sql/embed_schemas.py ${PROJECT_SOURCE_DIR}/src/sql/ schema.1.sql ${CMAKE_CURRENT_BINARY_DIR}/sql_schemas_v1.cc currconv
)

add_executable(currency_convert_v1 $<TARGET_OBJECTS:currency_convert> src/conv_v1.cc sql_schemas_v1.cc)
target_link_libraries(currency_convert_v1 ${SQLITE_LIBRARIES})
set_target_properties(currency_convert_v1 PROPERTIES
    CXX_STANDARD 17
    )

add_custom_command(OUTPUT sql_schemas_v2.cc
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/src/sql/embed_schemas.py ${PROJECT_SOURCE_DIR}/src/sql/ schema.2.sql ${CMAKE_CURRENT_BINARY_DIR}/sql_schemas_v2.cc currconv
)

add_executable(currency_convert_v2 $<TARGET_OBJECTS:currency_convert> src/conv_v2.cc sql_schemas_v2.cc)
target_link_libraries(currency_convert_v2 ${SQLITE_LIBRARIES})
set_target_properties(currency_convert_v2 PROPERTIES
    CXX_STANDARD 17
    )
