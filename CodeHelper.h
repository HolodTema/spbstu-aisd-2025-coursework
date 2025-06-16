#ifndef CODE_HELPER_H
#define CODE_HELPER_H

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "SmartBoolArray.h"

using MapCodesEnglish = std::unordered_map<std::string, char>;
using MapCodesNonEnglish = std::unordered_map<std::string, wchar_t>;

class CodeHelper {
public:

    //non-english encode
    std::pair<std::string, MapCodesNonEnglish> encodeString(const std::wstring& str) {
        VectorFrequencyNonEnglish vectorFrequency = createFrequencyVector(str);

        for (auto charInfo : vectorFrequency) {
            std::wcout << charInfo.getChar() << L" - " << charInfo.getFrequency() << L'\n';
        }
        generateCode(vectorFrequency.begin(), vectorFrequency.end(), str.size());

        std::unordered_map<wchar_t, std::string> codesMap;
        for (CharInfo<wchar_t> charInfo : vectorFrequency) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        for (wchar_t ch : str) {
            result += codesMap[ch];
        }

        MapCodesNonEnglish mapCodesInverse = inverseMap(codesMap);

        return std::make_pair(result, mapCodesInverse);
    }

    //english encode
    std::pair<std::string, MapCodesEnglish> encodeString(const std::string& str) {
        VectorFrequencyEnglish vectorFrequency = createFrequencyVector(str);

        generateCode(vectorFrequency.begin(), vectorFrequency.end(), str.size());

        std::unordered_map<char, std::string> codesMap;
        for (const CharInfo<char>& charInfo : vectorFrequency) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        for (wchar_t ch : str) {
            result += codesMap[ch];
        }

        MapCodesEnglish mapCodesInverse = inverseMap(codesMap);

        return std::make_pair(result, mapCodesInverse);
    }

    //english encode bits-mode
    std::pair<std::string, MapCodesEnglish> encodeStringBits(const std::string& str) {
        VectorFrequencyEnglish vectorFrequency = createFrequencyVector(str);

        generateCode(vectorFrequency.begin(), vectorFrequency.end(), str.size());

        std::unordered_map<char, std::string> codesMap;
        for (const CharInfo<char>& charInfo : vectorFrequency) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        std::string singleChar;
        std::string code;
        for (char messageChar : str) {
            code = codesMap[messageChar];
            for (char codeChar : code) {
                if (singleChar.size() < 8) {
                    singleChar += codeChar;
                }
                else {
                    result += convertBinCodeToChar(singleChar);
                    singleChar.clear();
                }
            }
        }

        MapCodesEnglish mapCodesInverse = inverseMap(codesMap);
        return std::make_pair(result, mapCodesInverse);
    }

    //non-english encode bits-mode
    std::pair<std::string, MapCodesNonEnglish> encodeStringBits(const std::wstring& str) {
        VectorFrequencyNonEnglish vectorFrequency = createFrequencyVector(str);

        generateCode(vectorFrequency.begin(), vectorFrequency.end(), str.size());

        std::unordered_map<wchar_t, std::string> codesMap;
        for (const CharInfo<wchar_t>& charInfo : vectorFrequency) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        std::string singleChar;
        std::string code;
        for (char messageChar : str) {
            code = codesMap[messageChar];
            for (char codeChar : code) {
                if (singleChar.size() < 8) {
                    singleChar += codeChar;
                }
                else {
                    result += convertBinCodeToChar(singleChar);
                    singleChar.clear();
                }
            }
        }

        MapCodesNonEnglish mapCodesInverse = inverseMap(codesMap);
        return std::make_pair(result, mapCodesInverse);
    }




    //non-english decode
    std::wstring decodeString(const std::string& encodedStr, const MapCodesNonEnglish& mapCodes) {
        std::wstring result;
        std::string code;

        int i = 0;
        while (i < encodedStr.size()) {
            while (!mapCodes.contains(code)) {
                code += encodedStr[i];
                ++i;
            }
            result += mapCodes.at(code);
            code.clear();
        }
        return result;
    }

    //english decode
    std::string decodeString(const std::string& encodedStr, const MapCodesEnglish& mapCodes) {
        std::string result;
        std::string code;

        int i = 0;
        while (i < encodedStr.size()) {
            while (!mapCodes.contains(code)) {
                code += encodedStr[i];
                ++i;
            }
            result += mapCodes.at(code);
            code.clear();
        }
        return result;
    }

    //english decode bits-mode
    std::string decodeStringBits(const std::string& encodedStr, const MapCodesEnglish& mapCodes) {
        std::string result;
        std::string encodedStrBits;

        for (const char& encodedChar: encodedStr) {
            encodedStrBits += convertCharToBinCode(encodedChar);
        }

        return decodeString(encodedStrBits, mapCodes);
    }

    //non-english decode bits-mode
    std::wstring decodeStringBits(const std::string& encodedStr, const MapCodesNonEnglish& mapCodes) {
        std::string result;
        std::string encodedStrBits;

        for (const char& encodedChar: encodedStr) {
            encodedStrBits += convertCharToBinCode(encodedChar);
        }

        return decodeString(encodedStrBits, mapCodes);
    }


private:
    template <typename T>
    class CharInfo {
    public:
        CharInfo(const T& ch, const int& frequency):
            ch_(ch),
            frequency_(frequency)
        { }

        T getChar() const {
            return ch_;
        }

        int getFrequency() const {
            return frequency_;
        }

        std::string getCode() const {
            return code_;
        }

        void addZeroToCode() {
            code_ += L'0';
        }

        void addOneToCode() {
            code_ += L'1';
        }

    private:
        T ch_;
        int frequency_;
        std::string code_;
    };

    using VectorFrequencyEnglish = std::vector<CharInfo<char>>;
    using VectorFrequencyNonEnglish = std::vector<CharInfo<wchar_t>>;

    VectorFrequencyNonEnglish createFrequencyVector(const std::wstring& str) {
        std::map<wchar_t, int> frequencyMap;
        for (wchar_t ch : str) {
            frequencyMap[ch] ++;
        }

        VectorFrequencyNonEnglish result;
        for (auto pair : frequencyMap) {
            result.emplace_back(pair.first, pair.second);
        }
        auto comparator = [](const CharInfo<wchar_t>& lhs, const CharInfo<wchar_t>& rhs) {return lhs.getFrequency() > rhs.getFrequency(); };
        std::sort(result.begin(), result.end(), comparator);

        return result;
    }

    VectorFrequencyEnglish createFrequencyVector(const std::string& str) {
        std::map<char, int> frequencyMap;
        for (char ch : str) {
            frequencyMap[ch] ++;
        }

        VectorFrequencyEnglish result;
        for (std::pair<char, int> pair : frequencyMap) {
            result.emplace_back(pair.first, pair.second);
        }
        auto comparator = [](const CharInfo<char>& lhs, const CharInfo<char>& rhs) {return lhs.getFrequency() > rhs.getFrequency(); };
        std::sort(result.begin(), result.end(), comparator);

        return result;
    }

    void generateCode(std::vector<CharInfo<wchar_t>>::iterator beginIter, std::vector<CharInfo<wchar_t>>::iterator endIter, int frequencyAmount) {
        if (beginIter + 1 == endIter) {
            return;
        }

        int freqSum = 0;
        int higherSum = 0;
        std::vector<CharInfo<wchar_t>>::iterator splitIter = beginIter;
        for (std::vector<CharInfo<wchar_t>>::iterator it = beginIter; it != endIter; ++it) {
            if (freqSum < frequencyAmount - freqSum) {
                (*it).addZeroToCode();
                ++splitIter;
                higherSum += (*it).getFrequency();
            }
            else {
                (*it).addOneToCode();
            }
            freqSum += (*it).getFrequency();
        }
        generateCode(beginIter, splitIter, higherSum);
        generateCode(splitIter, endIter, frequencyAmount-higherSum);
    }

    void generateCode(std::vector<CharInfo<char>>::iterator beginIter, std::vector<CharInfo<char>>::iterator endIter, int frequencyAmount) {
        if (beginIter + 1 == endIter) {
            return;
        }

        int freqSum = 0;
        int higherSum = 0;
        std::vector<CharInfo<char>>::iterator splitIter = beginIter;
        for (std::vector<CharInfo<char>>::iterator it = beginIter; it != endIter; ++it) {
            if (freqSum < frequencyAmount - freqSum) {
                (*it).addZeroToCode();
                ++splitIter;
                higherSum += (*it).getFrequency();
            }
            else {
                (*it).addOneToCode();
            }
            freqSum += (*it).getFrequency();
        }
        generateCode(beginIter, splitIter, higherSum);
        generateCode(splitIter, endIter, frequencyAmount-higherSum);
    }

    template<typename K, typename V>
    std::unordered_map<V, K> inverseMap(const std::unordered_map<K, V>& map) {
        std::unordered_map<V, K> result;
        for (typename std::unordered_map<K, V>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
            result[it->second] = it->first;
        }
        return result;
    }

    char convertBinCodeToChar(const std::string& str) {
        char result = 0;
        char bit = 0;
        for (int i = 0; i < str.size(); ++i) {
            bit = str[str.size()-1-i] - '0';
            result += bit*std::pow(2, i);
        }
        return result;
    }

    std::string convertCharToBinCode(char ch) {
        std::string result;

        // if (ch < 0) {
        //     result += '1';
        //     ch *= -1;
        // }
        while (ch > 1) {
            result += (ch % 2);
            ch /= 2;
        }
        result += ch;
        return result;
    }
};

#endif //CODE_HELPER_H
