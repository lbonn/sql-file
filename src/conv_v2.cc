#include <string>
#include "conv.h"

#include "sql_utils.h"

namespace conv {

std::string descr(const std::string& conv_name) {
    sql::Guard db("conv.db", false);

    auto statement = db.prepareStatement<std::string>("SELECT conv_description FROM convs "
            "WHERE conv_name = ? LIMIT 1", conv_name);

    int result = statement.step();
    if (result == SQLITE_DONE) {
        throw std::runtime_error("conv description not found");
    } else if (result != SQLITE_ROW) {
        throw std::runtime_error(db.errmsg());
    }

    return statement.get_result_col_str(0).value();
}

double apply(const std::string& conv_name, const std::string& date, double val) {
    sql::Guard db("conv.db", false);

    std::string b_date, a_date;
    double b_conv, a_conv;

    {
        auto statement = db.prepareStatement<std::string>("SELECT spot_date, val FROM spots INNER JOIN convs "
                "WHERE spots.conv_id = convs.id AND convs.conv_name = ? AND spots.spot_date <= ?"
                "ORDER BY spots.spot_date DESC "
                "LIMIT 1",
                conv_name, date);
        int result = statement.step();
        if (result == SQLITE_DONE) {
            throw std::runtime_error("out of range");
        } else if (result != SQLITE_ROW) {
            throw std::runtime_error(db.errmsg());
        }

        b_date = statement.get_result_col_str(0).value();
        b_conv = statement.get_result_col_double(1);
    }

    {
        auto statement = db.prepareStatement<std::string>("SELECT spot_date, val FROM spots INNER JOIN convs "
                "WHERE spots.conv_id = convs.id AND convs.conv_name = ? AND spots.spot_date >= ?"
                "ORDER BY spot_date ASC "
                "LIMIT 1",
                conv_name, date);
        int result = statement.step();
        if (result == SQLITE_DONE) {
            throw std::runtime_error("out of range");
        } else if (result != SQLITE_ROW) {
            throw std::runtime_error(db.errmsg());
        }

        a_date = statement.get_result_col_str(0).value();
        a_conv = statement.get_result_col_double(1);
    }

    auto conv = (b_conv + a_conv) / 2;

    return val * conv;
}

}  // namespace
