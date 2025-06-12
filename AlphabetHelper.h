#ifndef ALPHABET_HELPER_H
#define ALPHABET_HELPER_H

#include <map>
#include <string>
#include <fstream>

class AlphabetHelper {
public:

    std::map<wchar_t, int> createFrequencyMap(const std::wstring& str) {
        std::map<wchar_t, int> result;
        for (wchar_t ch : str) {
            result[ch] ++;
        }

        return result;
    }

    std::map<wchar_t, int> createFrequencyMap(std::wistream& wis) {
        std::map<wchar_t, int> result;
        std::wstring str;

        while (!wis.eof()) {
            std::getline(wis, str);
           std::getline(wis, str);
           for (wchar_t ch: str) {
               result[ch]++;
           }
        }

        return result;
    }

private:

};

#endif //ALPHABET_HELPER_H
