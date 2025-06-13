#ifndef CODE_HELPER_H
#define CODE_HELPER_H

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "SmartBoolArray.h"


class CodeHelper {
public:

    std::pair<std::string, std::unordered_map<std::string, wchar_t>> encodeString(const std::wstring& str) {
        std::pair<std::vector<CharInfo<wchar_t>>, int> pair = createFrequencyVector(str);
        std::vector<CharInfo<wchar_t>> frequencyVector = pair.first;
        int frequencyAmount = pair.second;

        generateCode(frequencyVector.begin(), frequencyVector.end(), frequencyAmount);

        std::unordered_map<wchar_t, std::string> codesMap;
        for (CharInfo<wchar_t> charInfo : frequencyVector) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        for (wchar_t ch : str) {
            result += codesMap[ch];
        }

        std::unordered_map<std::string, wchar_t> mapCodesInversed = inverseMap(codesMap);

        return std::make_pair(result, mapCodesInversed);
    }

    //for english
    std::pair<std::string, std::unordered_map<std::string, char>> encodeStringBits(const std::string& str) {
        std::pair<std::vector<CharInfo<char>>, int> pair = createFrequencyVector(str);
        std::vector<CharInfo<char>> frequencyVector = pair.first;
        int frequencyAmount = pair.second;

        generateCode(frequencyVector.begin(), frequencyVector.end(), frequencyAmount);

        std::unordered_map<char, std::string> codesMap;
        for (const CharInfo<char>& charInfo : frequencyVector) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::string result;
        std::string singleChar;
        for (char ch : str) {
            while (singleChar.size() < 8) {
                singleChar += codesMap[ch];
            }
            result += codesMap[ch];
        }

        std::unordered_map<std::string, char> mapCodesInversed = inverseMap(codesMap);

        return std::make_pair(result, mapCodesInversed);
    }


    std::wstring decodeString(const std::wstring& encodedStr, const std::unordered_map<std::wstring, wchar_t>& mapCodes) {
        std::wstring result;
        std::wstring code;

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

    //non-english decode
    std::wstring decodeStringBits(const std::string& encodedStr, const std::unordered_map<std::wstring, wchar_t>& mapCodes) {
        std::wstring result;
        std::wstring code;

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

    std::string decodeString(const std::string& encodedStr, const std::unordered_map<std::string, char>& mapCodes) {
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

    std::pair<std::vector<CharInfo<wchar_t>>, int> createFrequencyVector(const std::wstring& str) {
        std::map<wchar_t, int> frequencyMap;
        for (wchar_t ch : str) {
            frequencyMap[ch] ++;
        }

        std::vector<CharInfo<wchar_t>> result;
        for (std::pair<wchar_t, int> pair : frequencyMap) {
            result.emplace_back(pair.first, pair.second);
        }
        auto comparator = [](const CharInfo<wchar_t>& lhs, const CharInfo<wchar_t>& rhs) {return lhs.getFrequency() > rhs.getFrequency(); };
        std::sort(result.begin(), result.end(), comparator);

        return std::make_pair(result, str.size());
    }

    std::pair<std::vector<CharInfo<char>>, int> createFrequencyVector(const std::string& str) {
        std::map<char, int> frequencyMap;
        for (char ch : str) {
            frequencyMap[ch] ++;
        }

        std::vector<CharInfo<char>> result;
        for (std::pair<char, int> pair : frequencyMap) {
            result.emplace_back(pair.first, pair.second);
        }
        auto comparator = [](const CharInfo<char>& lhs, const CharInfo<char>& rhs) {return lhs.getFrequency() > rhs.getFrequency(); };
        std::sort(result.begin(), result.end(), comparator);

        return std::make_pair(result, str.size());
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
};

#endif //CODE_HELPER_H
