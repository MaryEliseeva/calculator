#include "pluginsLoader.h"
#include <windows.h>
#include <iostream>

void loadPlugin(Calculator& calc, const char* dllPath) {

    HMODULE lib = LoadLibraryA(dllPath);
    if (!lib) {
      
        std::cerr << "Не удалось загрузить DLL: " << dllPath << "\n";
        return;
    }

    auto compute = reinterpret_cast<double(*)(const std::vector<double>&)>(
        GetProcAddress(lib, "compute")
        );
    auto getName = reinterpret_cast<const char* (*)()>(
        GetProcAddress(lib, "getName")
        );

    if (!compute || !getName) {
        std::cerr << "Некорректная DLL: " << dllPath << "\n";
        FreeLibrary(lib);
        return;
    }

    try {
        calc.addFunction(getName(), compute);
        std::cout << "Функция загружена: " << getName() << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при добавлении функции: " << e.what() << "\n";
        FreeLibrary(lib);
    }
}
