#ifndef CONV_H
#define CONV_H

namespace conv {

std::string descr(const std::string& conv_name);
double apply(const std::string& conv_name, const std::string& date, double val);

}  // namespace

#endif
