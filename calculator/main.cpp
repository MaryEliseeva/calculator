#include "calculator.h"
#include "pluginsLoader.h"
#include "ExpressionParser.h"
#include <iostream>
#include <filesystem>

void loadAllPlugins(Calculator& calc, const std::string& pluginDir) {
    bool anyLoaded = false;

    if (std::filesystem::exists(pluginDir)) {
        for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
            if (entry.path().extension() == ".dll") {
                try {
                    loadPlugin(calc, entry.path().string().c_str());
                    anyLoaded = true;
                }
                catch (const std::exception& e) {
                    std::cerr << "Ошибка при загрузке "
                        << entry.path().filename().string()
                        << ": " << e.what() << "\n";
                }
            }
        }
    }
    else {
        std::cerr << "Папка '" << pluginDir << "' не найдена\n";
    }

    if (!anyLoaded) {
        std::cerr << "Не удалось загрузить ни одного плагина.\n";
    }
}

void runInteractiveMode(Calculator& calc) {
    ExpressionParser parser(calc);
    std::string expr;

    std::cout << "\nВведите выражение (0 для выхода):\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, expr);

        if (expr == "0")
            break;

        try {
            double result = parser.evaluate(expr);
            std::cout << "Результат: " << result << "\n";
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка вычисления: " << e.what() << "\n";
        }
    }
}


int main() {
    setlocale(LC_CTYPE, "Russian");

    Calculator calc;

    loadAllPlugins(calc, "plugins");

    calc.listFunctions();

    runInteractiveMode(calc);

    std::cout << "Конец\n";
    return 0;
}


