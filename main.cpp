#include <iostream>

#include <locale>

#include "ConsoleHelper.h"

int main() {
    setlocale(LC_ALL, "");
    // ConsoleHelper::launch();


    // const std::locale utf8_locale = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>());
    // std::wifstream inFile("../esenin_black_man.txt", std::ios::in);
    // inFile.imbue(utf8_locale);
    // if (!inFile.is_open()) {
    //     std::cerr << "Error: cannot open file!" << "\n";
    //     return 1;
    // }
    // std::wstring fileContents;
    // inFile.seekg(0, std::ios::end);
    // fileContents.resize(inFile.tellg());
    // inFile.seekg(0, std::ios::beg);
    // inFile.read(fileContents.data(), fileContents.size());
    // inFile.close();
    // std::wcout << fileContents << std::endl;



    std::ifstream fis("../korea.txt");
    fis.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));

    std::string textLine;
    std::string text;
    while (std::getline(fis, textLine)) {
        text += textLine + '\n';
    }

    std::cout << text;

    return 0;
}
