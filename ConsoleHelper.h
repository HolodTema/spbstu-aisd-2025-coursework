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

        bool isBitMode = (optionEncodeDecode == 2 || optionEncodeDecode == 4);
        if (optionEncodeDecode == 1 || optionEncodeDecode == 2) {
            return onOptionEncodeChosen(isEnglish, isBitMode);
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
    using DecodingDataEnglish = std::pair<std::string, MapCodesEnglish>;
    using DecodingDataNonEnglish = std::pair<std::string, MapCodesNonEnglish>;

    struct DecodeDataEnglish {
        std::string encodedText;
        MapCodesEnglish mapCodes;
    };

    struct DecodeDataNonEnglish {
        std::string encodedText;
        MapCodesNonEnglish mapCodes;
    };


    static int onOptionEncodeChosen(const bool& isEnglish, const bool& isBitMode) {
        if (isEnglish) {
            std::pair<std::string, std::string> pairEncodeData = getEncodeDataEnglish();
            if (pairEncodeData.first.size() != 0) {
                std::cout << pairEncodeData.first;
                return 1;
            }

            std::string text = pairEncodeData.second;
            CodeHelper helper;

            std::string result;
            MapCodesEnglish mapCodes;
            unsigned int zeroFillers = 0;
            if (isBitMode) {
                std::pair<std::string, EncodingInfo<MapCodesEnglish>> pair = helper.encodeStringBits(text);
                result = pair.first;
                mapCodes = pair.second.mapCodes;
                zeroFillers = pair.second.fillZeroes;
            }
            else {
                std::pair<std::string, MapCodesEnglish> pair = helper.encodeString(text);
                result = pair.first;
                mapCodes = pair.second;
            }



            std::string encodingCodes = convertMapCodesToString(mapCodes);
            bool isSuccessful = saveEncodingResults(result, encodingCodes, isBitMode, zeroFillers);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }

            std::cout << "\nDone!\n";
            std::cout << "Encoded text:\n";
            std::cout << result << '\n';
            std::cout << "Codes of encoding:\n";
            std::cout << encodingCodes;
            if (isBitMode) {
                std::cout << "Amount of zero-fillers: " << zeroFillers << "\n";
            }
            std::cout << "\nAlso these results are saved in files:\n";
            std::cout << "encoding_results.txt\nencoding.codes.txt\n";
            return 0;
        }
        else {
            std::pair<std::string, std::wstring> pairEncodeData = getEncodeDataNonEnglish();
            if (pairEncodeData.first.size() != 0) {
                std::cout << pairEncodeData.first;
                return 1;
            }

            std::wstring text = pairEncodeData.second;
            CodeHelper helper;

            std::string result;
            MapCodesNonEnglish mapCodes;
            unsigned int zeroFillers = 0;
            if (isBitMode) {
                std::pair<std::string, EncodingInfo<MapCodesNonEnglish>> pair = helper.encodeStringBits(text);
                result = pair.first;
                mapCodes = pair.second.mapCodes;
                zeroFillers = pair.second.fillZeroes;
            }
            else {
                std::pair<std::string, MapCodesNonEnglish> pair = helper.encodeString(text);
                result = pair.first;
                mapCodes = pair.second;
            }

            std::wstring encodingCodes = convertMapCodesToString(mapCodes);
            bool isSuccessful = saveEncodingResults(result, encodingCodes, isBitMode, zeroFillers);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }

            std::cout << "\nDone!\n";
            std::cout << "Encoded text:\n";
            std::cout << result << '\n';
            std::cout << "Codes of encoding:\n";
            std::wcout << encodingCodes;
            std::cout << "\nAlso these results are saved in files:\n";
            std::cout << "encoding_results.txt\nencoding.codes.txt\n";
            return 0;
        }
    }

    static int onOptionDecodeChosen(bool isEnglish, const bool& isBitMode) {

    }


    static std::pair<std::string, std::string> getEncodeDataEnglish() {
        std::wcout << "\nEnter option:\n1. enter string to encode\n2. choose text file to encode\n";
        int optionFromStringFile = 0;
        std::wcin >> optionFromStringFile;
        if (!std::wcin) {
            return std::make_pair("Error: invalid option.\n", std::string());
        }
        if (optionFromStringFile == 1) {
            std::cout << "Enter text to encode:\n";
            std::string text;
            std::getline(std::cin, text);
            if (std::cin.fail()) {
                return std::make_pair("Error: unable to encode this text.\n", std::string());
            }
            if (text.size() == 0) {
                return std::make_pair("Error: entered text is empty.\n", std::string());
            }
            return std::make_pair(std::string(), text);
        }
        else if (optionFromStringFile == 2) {
            //from file
            std::cout << "Enter relative path to file:\n";
            std::string filePath;
            std::cin >> filePath;
            if (!std::cin) {
                return std::make_pair("Error: invalid file path.\n", std::string());
            }

            std::ifstream fis;
            fis.open(filePath);
            if (!fis.is_open()) {
                return std::make_pair("Error: invalid file path.\n", std::string());
            }

            std::string text;
            std::string fileLine;
            while (std::getline(fis, fileLine)) {
                text += fileLine;
            }
            fis.close();

            return std::make_pair(std::string(), text);
        }
        else {
            return std::make_pair("Error: invalid option.\n", std::string());
        }
    }

    static std::pair<std::string, std::wstring> getEncodeDataNonEnglish() {
        std::wcout << "\nEnter option:\n1. enter string to encode\n2. choose text file to encode\n";
        int optionFromStringFile = 0;
        std::wcin >> optionFromStringFile;
        if (!std::wcin) {
            return std::make_pair("Error: invalid option.\n", std::wstring());
        }
        if (optionFromStringFile == 1) {
            std::cout << "Enter text to encode:\n";
            std::wstring text;
            std::getline(std::wcin, text);
            if (std::wcin.fail()) {
                return std::make_pair("Error: unable to encode this text.\n", std::wstring());
            }
            if (text.size() == 0) {
                return std::make_pair("Error: entered text is empty.\n", std::wstring());
            }
            return std::make_pair(std::string(), text);
        }
        else if (optionFromStringFile == 2) {
            //from file
            std::cout << "Enter relative path to file:\n";
            std::string filePath;
            std::cin >> filePath;
            if (!std::cin) {
                return std::make_pair("Error: invalid file path.\n", std::wstring());
            }

            std::wifstream wfis;
            wfis.open(filePath);
            if (!wfis.is_open()) {
                return std::make_pair("Error: invalid file path.\n", std::wstring());
            }

            std::wstring text;
            std::wstring fileLine;
            while (std::getline(wfis, fileLine)) {
                text += fileLine;
            }
            wfis.close();

            return std::make_pair(std::string(), text);
        }
        else {
            return std::make_pair("Error: invalid option.\n", std::wstring());
        }
    }


    static std::pair<std::string, DecodeDataEnglish> getDecodeDataEnglish() {
        std::cout << "\nEnter option:\n1. enter string to decode\n2. choose text file to decode\n";
        int optionFromStringFile = 0;
        std::cin >> optionFromStringFile;
        if (!std::cin) {
            return std::make_pair("Error: invalid option.\n", DecodeDataEnglish());
        }
        if (optionFromStringFile == 1) {
            //from string
            std::cout << "Enter text to decode:\n";
            std::string text;
            std::getline(std::cin, text);
            if (std::cin.fail()) {
                return std::make_pair("Error: entered text is invalid.\n", DecodeDataEnglish());
            }
            if (text.size() == 0) {
                return std::make_pair("Error: the text to decode is empty.\n", DecodeDataEnglish());
            }

            std::cout << "Enter file path of key codes:\n";
            std::string filePath;
            std::cin >> filePath;

            std::ifstream fis;
            fis.open(filePath);
            if (!fis.is_open()) {
                return std::make_pair("Error: unable to open this file.\n", DecodeDataEnglish());
            }
            std::pair<MapCodesEnglish, bool> pair = parseMapCodesFromFile(fis);
            if (!pair.second) {
                return std::make_pair("Error: file with encoding codes is invalid.\n", DecodeDataEnglish());
            }

            DecodeDataEnglish decodeData;
            decodeData.encodedText = text;
            decodeData.mapCodes = pair.first;
            return std::make_pair(std::string(), decodeData);
        }
        else if (optionFromStringFile == 2) {
            //from file
            return std::make_pair("Not implemented.\n", DecodeDataEnglish());

        }
        else {
            return std::make_pair("Error: invalid option.\n", DecodeDataEnglish());
        }
    }

    static std::pair<std::string, DecodeDataNonEnglish> getDecodeDataNonEnglish() {

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

    static bool saveEncodingResults(const std::string& encodedText, const std::wstring& encodingCodes, const bool& isBitMode, const unsigned int& zeroFillers) {
        std::wofstream outEncodedText;
        outEncodedText.open("encoding_results.txt");
        if (!outEncodedText.is_open()) {
            return false;
        }

        outEncodedText << std::wstring(encodedText.begin(), encodedText.end());
        outEncodedText.close();

        std::wofstream outMapCodes;
        outMapCodes.open("encoding_codes.txt");
        if (!outMapCodes.is_open()) {
            return false;
        }

        if (isBitMode) {
            outMapCodes << "Non-English text. Encoded in bit-mode\n";
            outMapCodes << "Amount of zero-fillers: " << zeroFillers << "\n";
        }
        else {
            outMapCodes << "Non-English text. Encoded without bit-mode\n";
        }
        outMapCodes << encodingCodes;
        outMapCodes.close();
        return true;
    }

    static bool saveEncodingResults(const std::string& encodedText, const std::string& encodingCodes, const bool& isBitMode, const unsigned int& zeroFillers) {
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

        if (isBitMode) {
            outMapCodes << "English text. Encoded in bit-mode\n";
            outMapCodes << "Amount of zero-fillers: " << zeroFillers << "\n";
        }
        else {
            outMapCodes << "English text. Encoded without bit-mode\n";
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







    static std::wstring convertMapCodesToString(const MapCodesNonEnglish& mapCodes) {
        std::wstring result;
        for (auto it = mapCodes.begin(); it != mapCodes.end(); it++) {
            result += it->second;
            result += L" - ";
            result += std::wstring(it->first.begin(), it->first.end());
            result += L"\n";
        }
        return result;
    }

    static std::string convertMapCodesToString(const MapCodesEnglish& mapCodes) {
        std::string result;
        for (auto it = mapCodes.begin(); it != mapCodes.end(); it++) {
            result += it->second;
            result += " - ";
            result += it->first;
            result += "\n";
        }
        return result;
    }

    static std::pair<MapCodesNonEnglish, bool> parseMapCodesFromFile(std::wifstream& in) {
        MapCodesNonEnglish mapCodes;

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
            mapCodes[convertWstringToString(code)] = character;
        }
        in.close();
        return std::make_pair(mapCodes, true);
    }

    static std::pair<MapCodesEnglish, bool> parseMapCodesFromFile(std::ifstream& in) {
        MapCodesEnglish mapCodes;

        std::string fileLine;
        std::string code;
        char character;
        char dash;
        while (std::getline(in, fileLine)) {
            std::istringstream stream(fileLine);
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
