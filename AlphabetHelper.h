#ifndef ALPHABET_HELPER_H
#define ALPHABET_HELPER_H

#include <map>
#include <string>

class AlphabetHelper {
public:

    std::map<char, int> createFrequencyMap(const std::string& str) {
        std::map<char, int> result;
        for (char ch : str) {
            result[ch] ++;
        }

        return result;
    }

    std::map<char, int> createFrequencyMap(std::istream& is) {
        std::map<char, int> result;
        std::string str;

        while (!is.eof()) {
           std::getline(is, str);
           for (char ch: str) {
               result[ch]++;
           }
        }

        return result;
    }

private:

};

#endif //ALPHABET_HELPER_H
