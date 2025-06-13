#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

#include <fstream>
#include <thread>
#include <bits/chrono.h>

#include "CodeHelper.h"

class ConsoleHelper {
public:

    static int launch() {
        std::cout << "Hello, this program allows you to encode or decode text using Shannon-Fano coding.\n\n";

        std::cout << "Enter language of text to encode/decode:\n";
        std::cout << "1. English\n2. other languages (non ASCII characters)\n";
        int optionLang = 0;
        std::cin >> optionLang;
        if (!std::cin) {
            return onErrorInvalidOption();
        }
        if (optionLang != 1 && optionLang != 2) {
            return onErrorInvalidOption();
        }
        bool isEnglish = optionLang == 1;

        std::cout << "Enter option:\n1. encode text\n2. decode text\n";
        int optionEncodeDecode = 0;
        std::cin >> optionEncodeDecode;
        if (!std::cin) {
            return onErrorInvalidOption();
        }
        if (optionEncodeDecode == 1) {
            onOptionEncodeChosen(isEnglish);
        }
        else if (optionEncodeDecode == 2) {
            onOptionDecodeChosen(isEnglish);
        }
        else {
            return onErrorInvalidOption();
        }
        return 0;
    }

private:
    static int onOptionEncodeChosen(bool isEnglish) {
        std::cout << "\nEnter option:\n1. enter string to encode\n2. choose text file to encode\n";
        int optionFromStringFile = 0;
        std::cin >> optionFromStringFile;
        if (!std::cin) {
            return onErrorInvalidOption();
        }
        if (optionFromStringFile == 1) {
            onOptionEncodeFromStringChosen(isEnglish);
        }
        else if (optionFromStringFile == 2) {

        }
        else {
            return onErrorInvalidOption();
        }
        return 0;
    }

    static int onOptionEncodeFromStringChosen(bool isEnglish) {
        if (isEnglish) {
            // std::cout << "Enter text to encode:\n";
            // std::string text;
            // std::cin >> text;
            // if (!std::cin) {
            //     return onErrorInvalidEnteredTextToEncode();
            // }
            // if (text.size() == 0) {
            //     return onErrorEmptyText();
            // }
            //
            // CodeHelper helper;

        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            std::cout << "Enter text to encode:\n";
            std::wstring text;
            std::wcin >> text;
            if (!std::wcin) {
                return onErrorInvalidEnteredTextToEncode();
            }
            if (text.size() == 0) {
                return onErrorEmptyText();
            }

            CodeHelper helper;
            std::pair<std::wstring, std::map<std::wstring, wchar_t>> pair = helper.encodeString(text);
            std::wstring result = pair.first;
            std::map<std::wstring, wchar_t> mapCodes = pair.second;

            std::wstring encodingCodes = convertMapCodesToString(mapCodes);
            bool isSuccessful = saveEncodingResults(result, encodingCodes);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }
            std::cout << "\nDone!\n";
            std::cout << "Encoded text:\n";
            std::wcout << result << L'\n';
            std::cout << "Codes of encoding:\n";
            std::wcout << encodingCodes;
            std::cout << "\nAlso these results are saved in files:\n";
            std::cout << "encoding_results.txt\nencoding.codes.txt\n";
        }
        return 0;
    }

    static int onOptionDecodeChosen(bool isEnglish) {
        return 0;
    }

    static int onErrorInvalidOption() {
        std::cout << "Error: invalid option.\n";
        return 1;
    }

    static int onErrorEmptyText() {
        std::cout << "Error: there is no text to handle.\n";
        return 1;
    }

    static int onErrorInvalidEnteredTextToEncode() {
        std::cout << "Error: invalid entered text to encode. Try using non-English language option to encode this text.\n";
        return 1;
    }

    static int onErrorUnableToSaveResults() {
        std::cout << "Error: unable to save results of operation.\n";
        return 1;
    }

    static bool saveEncodingResults(const std::wstring& encodedText, const std::wstring& encodingCodes) {
        std::wofstream outEncodedText;
        outEncodedText.open("encoding_results.txt");
        if (!outEncodedText.is_open()) {
            return false;
        }

        outEncodedText << encodedText;
        outEncodedText.close();

        std::wofstream outMapCodes;
        outMapCodes.open("encoding_codes.txt");
        if (!outMapCodes.is_open()) {
            return false;
        }

        outMapCodes << encodingCodes;
        outMapCodes.close();
        return true;
    }

    static std::wstring convertMapCodesToString(const std::map<std::wstring, wchar_t>& mapCodes) {
        std::wstring result;
        for (auto it = mapCodes.begin(); it != mapCodes.end(); it++) {
            result += it->second;
            result += L" - ";
            result += it->first;
            result += L"\n";
        }
        return result;
    }
};

#endif //CONSOLE_HELPER_H
