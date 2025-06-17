#include <iostream>

#include <locale>

#include "ConsoleHelper.h"

int main() {
    setlocale(LC_ALL, "");
    ConsoleHelper::launch();

    // std::wifstream wifs("../korea.txt");
    // wifs.imbue(std::locale(wifs.getloc(), new std::codecvt_utf8<wchar_t>));
    // std::wstring ws;
    // wifs >> ws;
    // std::wcout << ws;
    return 0;
}
