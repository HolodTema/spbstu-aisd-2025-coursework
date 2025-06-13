#ifndef ALPHABET_HELPER_H
#define ALPHABET_HELPER_H

#include <algorithm>
#include <map>
#include <string>
#include <fstream>
#include <vector>

#include "StreamGuard.h"

class AlphabetHelper {
public:

    std::vector<std::pair<wchar_t, int>> createFrequencyVector(const std::wstring& str) {
        std::map<wchar_t, int> frequencyMap;
        for (wchar_t ch : str) {
            frequencyMap[ch] ++;
        }

        std::vector<std::pair<wchar_t, int>> result(frequencyMap.begin(), frequencyMap.end());
        auto comparator = [](const std::pair<wchar_t, int>& lhs, const std::pair<wchar_t, int>& rhs) {return lhs.second > rhs.second; };
        std::sort(result.begin(), result.end(), comparator);
        return result;
    }

    std::vector<std::pair<wchar_t, int>> createFrequencyVector(std::wistream& wis) {
        std::map<wchar_t, int> frequencyMap;
        std::wstring str;

        while (!wis.eof()) {
            std::getline(wis, str);
           std::getline(wis, str);
           for (wchar_t ch: str) {
               frequencyMap[ch]++;
           }
        }

        std::vector<std::pair<wchar_t, int>> result(frequencyMap.begin(), frequencyMap.end());
        auto comparator = [](const std::pair<wchar_t, int>& lhs, const std::pair<wchar_t, int>& rhs) {return lhs.second > rhs.second; };
        std::sort(result.begin(), result.end(), std::greater<std::pair<wchar_t, int>>());
        return result;
    }

private:

};

#endif //ALPHABET_HELPER_H
