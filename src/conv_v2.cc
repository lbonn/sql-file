#include <string>
#include "conv.h"

namespace conv {

std::string descr(const std::string& conv_name) {
    (void)conv_name;
    return "euros to dollars";
}

double apply(const std::string& conv_name, const std::string& date, double val) {
    (void)conv_name;
    (void)date;
    (void)val;
    return 0;
}

}  // namespace
