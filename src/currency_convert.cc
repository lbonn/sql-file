#include <algorithm>
#include <iostream>

#include "conv.h"

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " conv date value\n";
        return 1;
    }

    std::string conv_name = argv[1];
    std::transform(conv_name.begin(), conv_name.end(),conv_name.begin(), ::toupper);
    const std::string date = argv[2];
    const double val = std::stod(argv[3], nullptr);


    conv::Converter converter("conv.db", conv_name);
    const std::string conv_descr = converter.descr();

    try {
        std::cout << conv_descr << ": " << val << " / " << converter.apply(date, val) << "\n";
    } catch (std::runtime_error &e) {
        std::cerr << "Conversion error: " << e.what() << "\n";
    }
}
