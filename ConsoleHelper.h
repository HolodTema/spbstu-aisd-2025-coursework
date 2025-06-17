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
        EncodingInfo<MapCodesEnglish> encodingInfo;
    };

    struct DecodeDataNonEnglish {
        std::string encodedText;
        EncodingInfo<MapCodesNonEnglish> encodingInfo;
    };

    static int onOptionEncodeChosen(const bool& isEnglish, const bool& isBitMode) {
        if (isEnglish) {
            std::pair<std::string, std::string> pairEncodeData = getEncodeDataEnglish();
            if (pairEncodeData.first.size() != 0) {
                std::wcout << std::wstring(pairEncodeData.first.begin(), pairEncodeData.first.end());
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

            std::wcout << L"\nDone!\n";
            std::wcout << L"Encoded text:\n";
            std::wcout << std::wstring(result.begin(), result.end()) << '\n';
            std::wcout << L"Codes of encoding:\n";
            std::wcout << std::wstring(encodingCodes.begin(), encodingCodes.end());
            if (isBitMode) {
                std::wcout << L"Amount of zero-fillers: " << zeroFillers << "\n";
            }
            std::wcout << L"\nAlso these results are saved in files:\n";
            std::wcout << L"encoding_results.txt\nencoding.codes.txt\n";
            return 0;
        }
        else {
            std::pair<std::string, std::wstring> pairEncodeData = getEncodeDataNonEnglish();
            if (pairEncodeData.first.size() != 0) {
                std::wcout << std::wstring(pairEncodeData.first.begin(), pairEncodeData.first.end()) << L'\n';
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

            std::wcout << "\nDone!\n";
            std::wcout << "Encoded text:\n";
            std::wcout << std::wstring(result.begin(), result.end()) << '\n';
            std::wcout << "Codes of encoding:\n";
            std::wcout << encodingCodes;
            std::wcout << "\nAlso these results are saved in files:\n";
            std::wcout << "encoding_results.txt\nencoding.codes.txt\n";
            return 0;
        }
    }

    static int onOptionDecodeChosen(bool isEnglish, const bool& isBitMode) {
        if (isEnglish) {
            std::pair<std::string, DecodeDataEnglish> pairDecodeData = getDecodeDataEnglish();
            if (!pairDecodeData.first.empty()) {
                std::wcout << std::wstring(pairDecodeData.first.begin(), pairDecodeData.first.end());
                return 1;
            }

            std::string encodedText = pairDecodeData.second.encodedText;
            CodeHelper helper;

            std::string decodedText;
            if (isBitMode) {
                decodedText = helper.decodeStringBits(encodedText, pairDecodeData.second.encodingInfo);
            }
            else {
                decodedText = helper.decodeString(encodedText, pairDecodeData.second.encodingInfo.mapCodes);
            }

            bool isSuccessful = saveDecodingResults(decodedText);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }

            std::wcout << L"\nDone!\n";
            std::wcout << L"Decoded text:\n";
            std::wcout << std::wstring(decodedText.begin(), decodedText.end()) << '\n';
            std::wcout << L"\nAlso this result is saved in file:\n";
            std::wcout << L"decoding_results.txt\n";
        }
        else {
            std::pair<std::string, DecodeDataNonEnglish> pairDecodeData = getDecodeDataNonEnglish();
            if (!pairDecodeData.first.empty()) {
                std::wcout << std::wstring(pairDecodeData.first.begin(), pairDecodeData.first.end());
                return 1;
            }

            std::string encodedText = pairDecodeData.second.encodedText;
            CodeHelper helper;

            std::wstring decodedText;
            if (isBitMode) {
                decodedText = helper.decodeStringBits(encodedText, pairDecodeData.second.encodingInfo);
            }
            else {
                decodedText = helper.decodeString(encodedText, pairDecodeData.second.encodingInfo.mapCodes);
            }

            bool isSuccessful = saveDecodingResults(decodedText);
            if (!isSuccessful) {
                return onErrorUnableToSaveResults();
            }

            std::wcout << L"\nDone!\n";
            std::wcout << L"Decoded text:\n";
            std::wcout << decodedText << '\n';
            std::wcout << L"\nAlso this result is saved in file:\n";
            std::wcout << L"decoding_results.txt\n";
        }
        return 0;
    }

    static std::pair<std::string, std::string> getEncodeDataEnglish() {
        std::wcout << "\nEnter option:\n1. enter string to encode\n2. choose text file to encode\n";
        int optionFromStringFile = 0;
        std::wcin >> optionFromStringFile;
        if (!std::wcin) {
            return std::make_pair("Error: invalid option.\n", std::string());
        }
        if (optionFromStringFile == 1) {
            std::wcout << "Enter text to encode:\n";
            std::wstring text;

            std::getline(std::wcin, text);
            std::getline(std::wcin, text);

            if (std::wcin.fail()) {
                return std::make_pair("Error: unable to encode this text.\n", std::string());
            }
            if (text.size() == 0) {
                return std::make_pair("Error: entered text is empty.\n", std::string());
            }
            return std::make_pair(std::string(), convertWstringToString(text));
        }
        else if (optionFromStringFile == 2) {
            //from file
            std::wcout << "Enter relative path to file:\n";
            std::wstring filePath;
            std::wcin >> filePath;
            if (!std::wcin) {
                return std::make_pair("Error: invalid file path.\n", std::string());
            }

            std::ifstream fis;
            fis.open(convertWstringToString(filePath));
            if (!fis.is_open()) {
                return std::make_pair("Error: invalid file path.\n", std::string());
            }

            std::string text;
            std::string fileLine;
            while (std::getline(fis, fileLine, '\n')) {
                text += fileLine + '\n';
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
            std::wcout << "Enter text to encode:\n";
            std::wstring text;
            std::getline(std::wcin, text);
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
            std::wcout << "Enter relative path to file:\n";
            std::wstring filePath;
            std::wcin >> filePath;
            if (!std::wcin) {
                return std::make_pair("Error: invalid file path.\n", std::wstring());
            }
            std::string strFilePath = convertWstringToString(filePath);

            std::wifstream wfis(strFilePath);
            if (!wfis.is_open()) {
                return std::make_pair("Error: invalid file path.\n", std::wstring());
            }
            wfis.imbue(std::locale(wfis.getloc(), new std::codecvt_utf8<wchar_t>));
            std::wstring textLine;
            std::wstring text;
            while (std::getline(wfis, textLine, L'\n')) {
                text += textLine + L'\n';
            }
            wfis.close();

            if (text.size() == 0) {
                return std::make_pair("Error: entered text is empty.\n", std::wstring());
            }
            return std::make_pair(std::string(), std::wstring(text.begin(), text.end()));
        }
        else {
            return std::make_pair("Error: invalid option.\n", std::wstring());
        }
    }

    static std::pair<std::string, DecodeDataEnglish> getDecodeDataEnglish() {
        std::wcout << "\nEnter path to the file with encoded text:\n";
        std::wstring filePathEncodedText;
        std::wcin >> filePathEncodedText;
        if (!std::wcin) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataEnglish());
        }

        std::ifstream fis(convertWstringToString(filePathEncodedText));
        if (!fis.is_open()) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataEnglish());
        }

        std::string encodedText;
        std::string encodedTextLine;
        while (std::getline(fis, encodedTextLine, '\n')) {
            encodedText += encodedTextLine + '\n';
        }
        if (encodedText.size() == 0) {
            return std::make_pair("Error: this file is empty. Nothing to decode.\n", DecodeDataEnglish());
        }
        fis.close();

        std::wcout << "Enter path to the file with encoding keys:\n";
        std::wstring filePathEncodingKeys;
        std::wcin >> filePathEncodingKeys;
        if (!std::wcin) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataEnglish());
        }
        std::string encodingKeys;
        std::ifstream fisEncodingKeys(convertWstringToString(filePathEncodingKeys));
        if (!fisEncodingKeys.is_open()) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataEnglish());
        }
        std::pair<EncodingInfo<MapCodesEnglish>, bool> pair = parseMapCodesFromFile(fisEncodingKeys);
        if (!pair.second) {
            return std::make_pair("Error: file with encoding codes is invalid.\n", DecodeDataEnglish());
        }

        DecodeDataEnglish decodeData;
        decodeData.encodedText = encodedText;
        decodeData.encodingInfo = pair.first;
        return std::make_pair(std::string(), decodeData);
    }

    static std::pair<std::string, DecodeDataNonEnglish> getDecodeDataNonEnglish() {
        std::wcout << "\nEnter path to the file with encoded text:\n";
        std::wstring filePathEncodedText;
        std::wcin >> filePathEncodedText;
        if (!std::wcin) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataNonEnglish());
        }
        std::string encodedText;

        std::ifstream fis(convertWstringToString(filePathEncodedText));
        if (!fis.is_open()) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataNonEnglish());
        }
        std::string encodedTextLine;
        while (std::getline(fis, encodedTextLine, '\n')) {
            encodedText += encodedTextLine + '\n';
        }
        if (encodedText.size() == 0) {
            return std::make_pair("Error: this file is empty. Nothing to decode.\n", DecodeDataNonEnglish());
        }
        fis.close();

        std::wcout << "Enter path to the file with encoding keys:\n";
        std::wstring filePathEncodingKeys;
        std::wcin >> filePathEncodingKeys;
        if (!std::wcin) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataNonEnglish());
        }
        std::wstring encodingKeys;
        std::wifstream wfisEncodingKeys(convertWstringToString(filePathEncodingKeys));
        wfisEncodingKeys.imbue(std::locale(wfisEncodingKeys.getloc(), new std::codecvt_utf8<wchar_t>));
        if (!wfisEncodingKeys.is_open()) {
            return std::make_pair("Error: invalid file path.\n", DecodeDataNonEnglish());
        }
        std::pair<EncodingInfo<MapCodesNonEnglish>, bool> pair = parseMapCodesFromFile(wfisEncodingKeys);
        if (!pair.second) {
            return std::make_pair("Error: file with encoding codes is invalid.\n", DecodeDataNonEnglish());
        }

        DecodeDataNonEnglish decodeData;
        decodeData.encodedText = encodedText;
        decodeData.encodingInfo = pair.first;
        return std::make_pair(std::string(), decodeData);
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
        std::ofstream outEncodedText;
        outEncodedText.open("encoding_results.txt");
        if (!outEncodedText.is_open()) {
            return false;
        }
        outEncodedText << encodedText;
        outEncodedText.close();

        std::wofstream outMapCodes;
        outMapCodes.imbue(std::locale(outMapCodes.getloc(), new std::codecvt_utf8<wchar_t>));
        outMapCodes.open("encoding_codes.txt");
        if (!outMapCodes.is_open()) {
            return false;
        }

        if (isBitMode) {
            outMapCodes << "Non-English text. Encoded in bit-mode. Amount of zero-fillers: " << zeroFillers << "\n";
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
            outMapCodes << "English text. Encoded in bit-mode. Amount of zero-fillers: " << zeroFillers << "\n";
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
        out.imbue(std::locale(out.getloc(), new std::codecvt_utf8<wchar_t>));
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

    static std::pair<EncodingInfo<MapCodesNonEnglish>, bool> parseMapCodesFromFile(std::wifstream& in) {
        EncodingInfo<MapCodesNonEnglish> encodingInfo;

        std::wstring fileLine;
        std::wstring code;
        wchar_t character;
        std::wstring header;
        std::getline(in, header);
        encodingInfo.fillZeroes = convertCharToDigit(header[header.size() - 1]);
        std::wcout << "zero fillers = " << encodingInfo.fillZeroes << "\n";
        while (std::getline(in, fileLine)) {
            if (fileLine.size() >= 4) {
                character = fileLine[0];
                code = fileLine.substr(4, fileLine.size());
                encodingInfo.mapCodes[convertWstringToString(code)] = character;
            }
        }
        in.close();
        return std::make_pair(encodingInfo, true);
    }

    static std::pair<EncodingInfo<MapCodesEnglish>, bool> parseMapCodesFromFile(std::ifstream& in) {
        EncodingInfo<MapCodesEnglish> encodingInfo;

        std::string fileLine;
        std::string code;
        char character;
        std::string header;
        std::getline(in, header);
        encodingInfo.fillZeroes = convertCharToDigit(header[header.size() - 1]);
        std::wcout << "zero fillers = " << encodingInfo.fillZeroes << "\n";
        while (std::getline(in, fileLine)) {
            if (fileLine.size() >= 4) {
                character = fileLine[0];
                code = fileLine.substr(4, fileLine.size());
                encodingInfo.mapCodes[code] = character;
            }
        }
        in.close();
        return std::make_pair(encodingInfo, true);
    }

    static unsigned int convertCharToDigit(char ch) {
        if (ch == '0') {
            return 0;
        }
        if (ch == '1') {
            return 1;
        }
        if (ch == '2') {
            return 2;
        }
        if (ch == '3') {
            return 3;
        }
        if (ch == '4') {
            return 4;
        }
        if (ch == '5') {
            return 5;
        }
        if (ch == '6') {
            return 6;
        }
        if (ch == '7') {
            return 7;
        }
        return 0;
    }

    static unsigned int convertCharToDigit(wchar_t ch) {
        if (ch == L'0') {
            return 0;
        }
        if (ch == L'1') {
            return 1;
        }
        if (ch == L'2') {
            return 2;
        }
        if (ch == L'3') {
            return 3;
        }
        if (ch == L'4') {
            return 4;
        }
        if (ch == L'5') {
            return 5;
        }
        if (ch == L'6') {
            return 6;
        }
        if (ch == L'7') {
            return 7;
        }
        return 0;
    }

    static std::string convertWstringToString(const std::wstring& wstr)  {
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;
        return converter.to_bytes( wstr );
    }

};

#endif //CONSOLE_HELPER_H
