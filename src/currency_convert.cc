#include <algorithm>
#include <iostream>

#include "conv.h"

int main(int argc, const char *argv[]) {
    if (argc != 5) {
        std::cerr << "usage: " << argv[0] << " db conv date value\n";
        return 1;
    }

    const char *db = argv[1];
    std::string conv_name = argv[2];
    std::transform(conv_name.begin(), conv_name.end(),conv_name.begin(), ::toupper);
    const std::string date = argv[3];
    const double val = std::stod(argv[4], nullptr);

    try {
        conv::Converter converter(db, conv_name);
        const std::string conv_descr = converter.descr();

        std::cout << conv_descr << ": " << val << " / " << converter.apply(date, val) << "\n";
    } catch (std::runtime_error &e) {
        std::cerr << "Conversion error: " << e.what() << "\n";
    }
}
