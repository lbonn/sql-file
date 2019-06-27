cmake_minimum_required (VERSION 3.12)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SQLITE REQUIRED sqlite3)

add_compile_options("-Wall" "-Wextra" "-Werror" "$<$<CONFIG:RELEASE>:-O3>" "$<$<CONFIG:DEBUG>:-Og>")

add_executable(currency_convert src/currency_convert.cc)
target_link_libraries(currency_convert ${SQLITE})
set_target_properties(currency_convert PROPERTIES
    CXX_STANDARD 17
    )
