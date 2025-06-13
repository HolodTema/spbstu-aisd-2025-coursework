#include <iostream>


#include "ConsoleHelper.h"

int main() {

    // CodeHelper helper;
    // std::wstring str = L"Hello world!";
    // std::pair<std::vector<CodeHelper::CharInfo>, int> pair = helper.createFrequencyVector(str);
    // std::vector<CodeHelper::CharInfo> vec = pair.first;
    // int frequencyAmount = pair.second;
    //
    // for (const CodeHelper::CharInfo& charInfo : vec) {
    //     std::wcout << charInfo.getChar() << L' ' << charInfo.getFrequency() << L'\n';
    // }
    // std::wcout << L'\n' << frequencyAmount << L'\n';

    // CodeHelper helper;
    // std::wstring str = L"Hello world!";
    // std::pair<std::wstring, std::map<std::wstring, wchar_t>> pair = helper.encodeString(str);
    // std::wstring result = pair.first;
    // std::map<std::wstring, wchar_t> mapCodes = pair.second;
    // std::wcout << result << '\n';
    //
    // std::wstring strDecoded = helper.decodeString(result, mapCodes);
    // std::wcout << strDecoded << L'\n';

    ConsoleHelper::launch();

    return 0;
}
