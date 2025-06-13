#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

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
            std::cout << "Enter text to encode:\n";
            std::string text;
            std::cin >> text;
            if (!std::cin) {
                return onErrorInvalidOption();
            }
            if (text.size() == 0) {
                return onErrorEmptyText();
            }

            CodeHelper helper;
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
};

#endif //CONSOLE_HELPER_H
