#include <iostream>


#include "ConsoleHelper.h"

int main() {
    setlocale(LC_ALL, "");
    // ConsoleHelper::launch();

    // CodeHelper helper;
    // std::string strToEncode = "Hello world!";
    // std::pair<std::string, MapCodesEnglish> pair = helper.encodeString(strToEncode);
    // std::string result = pair.first;
    // std::cout << result << '\n';
    //
    // for (auto pairCodeChar : pair.second) {
    //     std::cout << pairCodeChar.second << " - " << pairCodeChar.first << '\n';
    // }
    // std::cout << '\n';
    //
    // std::string strDecoded = helper.decodeString(result, pair.second);
    // std::cout << strDecoded << '\n';


    // CodeHelper helper;
    // std::wstring strToEncode = L"Könnt ihr mich fühlen? — Wir fühlen dich";
    // std::pair<std::string, MapCodesNonEnglish> pair = helper.encodeString(strToEncode);
    // std::string result = pair.first;
    // std::wcout << L"---------\n";
    // std::wcout << std::wstring(result.begin(), result.end()) << '\n';
    //
    // for (auto pairCodeChar : pair.second) {
    //     std::wcout << pairCodeChar.second << L" - " << std::wstring(pairCodeChar.first.begin(), pairCodeChar.first.end()) << L'\n';
    //     // std::wcout << pairCodeChar.second;
    //     // std::cout << '\n' << pairCodeChar.first << '\n';
    // }
    // std::wcout << '\n';
    //
    // std::wstring strDecoded = helper.decodeString(result, pair.second);
    // std::wcout << strDecoded << '\n';





    CodeHelper helper;
    std::string strToEncode = "Hello world!";
    std::pair<std::string, MapCodesEnglish> pair = helper.encodeStringBits(strToEncode);
    std::string result = pair.first;
    std::cout << result << '\n';

    for (auto pairCodeChar : pair.second) {
        std::cout << pairCodeChar.second << " - " << pairCodeChar.first << '\n';
    }
    std::cout << '\n';

    std::string strDecoded = helper.decodeStringBits(result, pair.second);
    std::cout << strDecoded << '\n';

    std::cout << "end.\n";

    return 0;
}
