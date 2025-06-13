#ifndef CONSOLE_HELPER_H
#define CONSOLE_HELPER_H

#include <codecvt>
#include <fstream>
#include <locale>
#include <thread>
#include <sstream>

#include "CodeHelper.h"

class ConsoleHelper {
public:

    static int launch() {
        std::wcout << "Hello, this program allows you to encode or decode text using Shannon-Fano coding.\n\n";

        std::wcout << "Enter language of text to encode/decode:\n";
        std::wcout << "1. English (ASCII characters)\n2. other languages (non-ASCII characters)\n";
        int optionLang = 0;
        std::wcin >> optionLang;
        if (!std::wcin) {
            return onErrorInvalidOption();
        }
        if (optionLang != 1 && optionLang != 2) {
            return onErrorInvalidOption();
        }
        bool isEnglish = optionLang == 1;

        std::wcout << "Enter option:\n";
        std::wcout << "1. encode text in simple mode (every digit 0 or 1 will be full char in text)\n";
        std::wcout << "2. encode text in bit mode (every digit 0 or 1 is inside integer bits)\n";
        std::wcout << "3. decode text in simple mode\n";
        std::wcout << "4. decode text in bit mode\n";
        int optionEncodeDecode = 0;
        std::wcin >> optionEncodeDecode;
        if (!std::wcin) {
            return onErrorInvalidOption();
        }

        bool isBitMode = optionEncodeDecode == 2 || optionEncodeDecode == 4;
        if (optionEncodeDecode == 1 || optionEncodeDecode == 2) {
            onOptionEncodeChosen(isEnglish, isBitMode);
        }
        else if (optionEncodeDecode == 3 || optionEncodeDecode == 4) {
            onOptionDecodeChosen(isEnglish, isBitMode);
        }
        else {
            return onErrorInvalidOption();
        }
        return 0;
    }

private:
    static int onOptionEncodeChosen(bool isEnglish, bool isBitMode) {
        std::wcout << "\nEnter option:\n1. enter string to encode\n2. choose text file to encode\n";
        int optionFromStringFile = 0;
        std::wcin >> optionFromStringFile;
        if (!std::wcin) {
            return onErrorInvalidOption();
        }
        if (optionFromStringFile == 1) {
            return onOptionEncodeFromStringChosen(isEnglish, isBitMode);
        }
        else if (optionFromStringFile == 2) {
            return onOptionEncodeFromFileChosen(isEnglish, isBitMode);
        }
        else {
            return onErrorInvalidOption();
        }
        return 0;
    }

    static int onOptionEncodeFromStringChosen(bool isEnglish,  bool isBitMode) {
        if (isEnglish) {


        }
        else {
            std::wcout << "Enter text to encode:\n";
            std::wstring text;
            std::getline(std::wcin, text);
            std::getline(std::wcin, text);
            if (std::wcin.fail()) {
                return onErrorInvalidEnteredTextToEncode();
            }
            if (text.size() == 0) {
                return onErrorEmptyText();
            }

            return onEncodeNonEnglish(text, isBitMode);
        }
        return 0;
    }

    static int onOptionEncodeFromFileChosen(bool isEnglish,  bool isBitMode) {
        if (isEnglish) {

        }
        else {
            std::wcout << "Enter relative file path:\n";
            std::wstring filePath;
            std::wcin >> filePath;

            std::wifstream in;
            in.open(convertWstringToString(filePath));
            if (!in.is_open()) {
                return onErrorUnableToOpenFileToRead();
            }

            std::wstring text = readFile(in);

            if (text.size() == 0) {
                return onErrorEmptyText();
            }

            return onEncodeNonEnglish(text, isBitMode);
        }
        return 0;
    }

    static int onEncodeNonEnglish(const std::wstring& text, bool isBitMode) {
        CodeHelper helper;

        if (isBitMode) {

        }
        else {
            std::pair<std::string, std::unordered_map<std::wstring, wchar_t>> pair = helper.encodeString(text);
            std::wstring result = pair.first;
            std::unordered_map<std::wstring, wchar_t> mapCodes = pair.second;

            std::wstring encodingCodes = convertMapCodesToString(mapCodes);
            bool isSuccessful = saveEncodingResults(result, encodingCodes);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }
            std::wcout << "\nDone!\n";
            std::wcout << "Encoded text:\n";
            std::wcout << result << L'\n';
            std::wcout << "Codes of encoding:\n";
            std::wcout << encodingCodes;
            std::wcout << "\nAlso these results are saved in files:\n";
            std::wcout << "encoding_results.txt\nencoding.codes.txt\n";
        }
        return 0;
    }

    static int onOptionDecodeChosen(bool isEnglish,  bool isBitMode) {
        std::wcout << "\nEnter option:\n1. enter string to decode\n2. choose text file to decode\n";
        int optionFromStringFile = 0;
        std::wcin >> optionFromStringFile;
        if (!std::wcin) {
            return onErrorInvalidOption();
        }
        if (optionFromStringFile == 1) {
            return onOptionDecodeFromStringChosen(isEnglish, isBitMode);
        }
        else if (optionFromStringFile == 2) {
            return onOptionDecodeFromFileChosen(isEnglish, isBitMode);
        }
        else {
            return onErrorInvalidOption();
        }
        return 0;
    }

    static int onOptionDecodeFromStringChosen(bool isEnglish, bool isBitMode) {
        if (isEnglish) {

        }
        else {
            std::wcout << "Enter text to decode:\n";
            std::wstring text;
            std::getline(std::wcin, text);
            if (std::wcin.fail()) {
                return onErrorInvalidEnteredTextToEncode();
            }
            if (text.size() == 0) {
                return onErrorEmptyText();
            }

            std::cout << "Enter file path of key codes:\n";
            std::wstring filePath;
            std::wcin >> filePath;

            std::wifstream in;
            in.open(convertWstringToString(filePath));
            if (!in.is_open()) {
                return onErrorUnableToOpenFileToRead();
            }
            std::pair<std::unordered_map<std::wstring, wchar_t>, bool> pair = parseMapCodesFromFile(in);
            if (!pair.second) {
                return onErrorInvalidEncodingCodesFile();
            }
            return onDecodeNonEnglish(text, pair.first, isBitMode);;
        }
        return 0;
    }

    static int onDecodeNonEnglish(const std::wstring& text, std::unordered_map<std::wstring, wchar_t> mapCodes, bool& isBitMode) {
        CodeHelper helper;

        if (isBitMode) {

        }
        else {
            std::wstring result = helper.decodeString(text, mapCodes);

            bool isSuccessful = saveDecodingResults(result);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }
            std::wcout << "\nDone!\n";
            std::wcout << "Decoded text:\n";
            std::wcout << result << L'\n';
            std::wcout << "Also decoded text was saved in file decoding_results.txt,\n";
            return 0;
        }
    }

    static int onDecodeEnglish(const std::string text, std::unordered_map<std::string, char> mapCodes, bool& isBitMode) {
        CodeHelper helper;
        if (isBitMode) {

        }
        else {
            std::string result = helper.decodeString(text, mapCodes);

            bool isSuccessful = saveDecodingResults(result);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }
            std::wcout << "\nDone!\n";
            std::wcout << "Decoded text:\n";
            std::cout << result << '\n';
            std::wcout << "Also decoded text was saved in file decoding_results.txt,\n";
        }
        return 0;
    }

    static int onOptionDecodeFromFileChosen(bool isEnglish, bool isBitMode) {

    }

    static std::wstring readFile(std::wifstream& in) {
        std::wstring fileLine;
        std::wstring text;
        while (std::getline(in, fileLine)) {
            text += fileLine;
        }
        in.close();
        return text;
    }

    static int onErrorInvalidOption() {
        std::wcout << "Error: invalid option.\n";
        return 1;
    }

    static int onErrorEmptyText() {
        std::wcout << "Error: there is no text to handle.\n";
        return 1;
    }

    static int onErrorInvalidEnteredTextToEncode() {
        std::wcout << "Error: invalid entered text to encode. Try using non-English language option to encode this text.\n";
        return 1;
    }

    static int onErrorUnableToSaveResults() {
        std::wcout << "Error: unable to save results of operation.\n";
        return 1;
    }

    static int onErrorUnableToOpenFileToRead() {
        std::wcout << "Error: unable to open file to read. Ensure that file exists.\n";
        return 1;
    }

    static int onErrorInvalidEncodingCodesFile() {
        std::wcout << "Error: file with encoding codes is invalid.\n";
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

    static bool saveEncodingResults(const std::string& encodedText, const std::string& encodingCodes) {
        std::ofstream outEncodedText;
        outEncodedText.open("encoding_results.txt");
        if (!outEncodedText.is_open()) {
            return false;
        }

        outEncodedText << encodedText;
        outEncodedText.close();

        std::ofstream outMapCodes;
        outMapCodes.open("encoding_codes.txt");
        if (!outMapCodes.is_open()) {
            return false;
        }

        outMapCodes << encodingCodes;
        outMapCodes.close();
        return true;
    }

    static bool saveDecodingResults(const std::wstring& decodedText) {
        std::wofstream out;
        out.open("decoding_results.txt");
        if (!out.is_open()) {
            return false;
        }

        out << decodedText;
        out.close();
        return true;
    }

    static bool saveDecodingResults(const std::string& decodedText) {
        std::ofstream out;
        out.open("decoding_results.txt");
        if (!out.is_open()) {
            return false;
        }

        out << decodedText;
        out.close();
        return true;
    }

    static std::wstring convertMapCodesToString(const std::unordered_map<std::wstring, wchar_t>& mapCodes) {
        std::wstring result;
        for (auto it = mapCodes.begin(); it != mapCodes.end(); it++) {
            result += it->second;
            result += L" - ";
            result += it->first;
            result += L"\n";
        }
        return result;
    }

    static std::pair<std::unordered_map<std::wstring, wchar_t>, bool> parseMapCodesFromFile(std::wifstream& in) {
        std::unordered_map<std::wstring, wchar_t> mapCodes;

        std::wstring fileLine;
        std::wstring code;
        wchar_t character;
        wchar_t dash;
        while (std::getline(in, fileLine)) {
            std::wistringstream stream(fileLine);
            stream >> character >> dash >> code;
            if (!stream) {
                return std::make_pair(mapCodes, false);
            }
            mapCodes[code] = character;
        }
        in.close();
        return std::make_pair(mapCodes, true);
    }

    static std::string convertWstringToString(const std::wstring& wstr)  {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes( wstr );
    }

};

#endif //CONSOLE_HELPER_H
