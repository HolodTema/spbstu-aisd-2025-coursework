#ifndef CODE_HELPER_H
#define CODE_HELPER_H

#include <algorithm>
#include <map>
#include <string>
#include <vector>


class CodeHelper {
public:

    class CharInfo {
    public:
        CharInfo(const wchar_t& ch, const int& frequency):
            ch_(ch),
            frequency_(frequency)
        { }

        wchar_t getChar() const {
            return ch_;
        }

        int getFrequency() const {
            return frequency_;
        }

        std::wstring getCode() const {
            return code_;
        }

        void addZeroToCode() {
            code_ += L'0';
        }

        void addOneToCode() {
            code_ += L'1';
        }

    private:
        wchar_t ch_;
        int frequency_;
        std::wstring code_;
    };

    //returns frequencyVector with amount of all frequencies
    std::pair<std::vector<CharInfo>, int> createFrequencyVector(const std::wstring& str) {
        std::map<wchar_t, int> frequencyMap;
        for (wchar_t ch : str) {
            frequencyMap[ch] ++;
        }

        std::vector<CharInfo> result;
        for (std::pair<wchar_t, int> pair : frequencyMap) {
            result.emplace_back(pair.first, pair.second);
        }
        auto comparator = [](const CharInfo& lhs, const CharInfo& rhs) {return lhs.getFrequency() > rhs.getFrequency(); };
        std::sort(result.begin(), result.end(), comparator);

        return std::make_pair(result, str.size());
    }

    void generateCode(std::vector<CharInfo>::iterator beginIter, std::vector<CharInfo>::iterator endIter, int frequencyAmount) {
        if (beginIter + 1 == endIter) {
            return;
        }

        int freqSum = 0;
        int higherSum = 0;
        std::vector<CharInfo>::iterator splitIter = beginIter;
        for (std::vector<CharInfo>::iterator it = beginIter; it != endIter; ++it) {
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

    std::pair<std::wstring, std::map<std::wstring, wchar_t>> encodeString(const std::wstring& str) {
        std::pair<std::vector<CharInfo>, int> pair = createFrequencyVector(str);
        std::vector<CharInfo> frequencyVector = pair.first;
        int frequencyAmount = pair.second;

        generateCode(frequencyVector.begin(), frequencyVector.end(), frequencyAmount);

        std::map<wchar_t, std::wstring> codesMap;
        for (CharInfo charInfo : frequencyVector) {
            codesMap[charInfo.getChar()] = charInfo.getCode();
        }

        std::wstring result;
        for (wchar_t ch : str) {
            result += codesMap[ch];
        }

        std::map<std::wstring, wchar_t> mapCodesInversed = inverseMap(codesMap);

        return std::make_pair(result, mapCodesInversed);
    }

    std::wstring decodeString(const std::wstring& encodedStr, const std::map<std::wstring, wchar_t>& mapCodes) {
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

    template<typename K, typename V>
    std::map<V, K> inverseMap(const std::map<K, V>& map) {
        std::map<V, K> result;
        for (typename std::map<K, V>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
            result[it->second] = it->first;
        }
        return result;
    }
};

#endif //CODE_HELPER_H
