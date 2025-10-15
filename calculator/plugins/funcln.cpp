#include <cmath>
#include <vector>
#include <stdexcept>

extern "C" __declspec(dllexport)
double compute(const std::vector<double>& args) {
    if (args.size() != 1)
        throw std::runtime_error("ln() ожидает 1 аргумент");
    if (args[0] <= 0)
        throw std::runtime_error("ln() определён только для положительных чисел");
    return std::log(args[0]);
}

extern "C" __declspec(dllexport)
const char* getName() {
    return "ln";
}
