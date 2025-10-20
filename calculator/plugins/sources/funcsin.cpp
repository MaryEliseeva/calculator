#include <cmath>
#include <vector>
#include <stdexcept>

#define M_PI 3.14159265358979323846

extern "C" __declspec(dllexport)
double compute(const std::vector<double>& args) {
    if (args.size() != 1)
        throw std::runtime_error("sin() ожидает 1 аргумент");
    return std::sin(args[0] * M_PI / 180.0);
}

extern "C" __declspec(dllexport)
const char* getName() {
    return "sin";
}
