#include <cmath>
#include <vector>
#include <stdexcept>

extern "C" __declspec(dllexport)
double compute(const std::vector<double>& args) {
    if (args.size() != 2)
        throw std::runtime_error("sum() ожидает 2 аргумента");
    return args[0] + args[1];
}

extern "C" __declspec(dllexport)
const char* getName() {
    return "sum";
}
