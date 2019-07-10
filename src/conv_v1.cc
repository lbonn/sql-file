#include <string>
#include "conv.h"

#include "sql_utils.h"

namespace conv {

std::string descr(const std::string& conv_name) {
    (void)conv_name;
    // hardcoded!
    return "euros / dollars";
}

double apply(const std::string& conv_name, const std::string& date, double val) {
    (void)conv_name;

    sql::Guard db("conv.db", false);

    std::string b_date, a_date;
    double b_conv, a_conv;

    {
        auto statement = db.prepareStatement<std::string>("SELECT spot_date, val FROM eurusd "
                "WHERE spot_date <= ? "
                "ORDER BY spot_date DESC "
                "LIMIT 1",
                date);
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
        auto statement = db.prepareStatement<std::string>("SELECT spot_date, val FROM eurusd "
                "WHERE spot_date >= ? "
                "ORDER BY spot_date ASC "
                "LIMIT 1",
                date);
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
