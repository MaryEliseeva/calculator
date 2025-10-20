#include <cmath>
#include <vector>
#include <stdexcept>

extern "C" __declspec(dllexport)
double compute(const std::vector<double>& args) {
    if (args.size() != 1)
        throw std::runtime_error("ln() ������� 1 ��������");
    if (args[0] <= 0)
        throw std::runtime_error("ln() �������� ������ ��� ������������� �����");
    return std::log(args[0]);
}

extern "C" __declspec(dllexport)
const char* getName() {
    return "ln";
}
