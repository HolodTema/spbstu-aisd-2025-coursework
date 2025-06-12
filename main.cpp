#include <iostream>

#include "AlphabetHelper.h"
#include <bits/stdc++.h>

int main() {
    // AlphabetHelper alphabetHelper;
    //
    // std::string str = "Hello world!";
    // std::map<char, int> map = alphabetHelper.createFrequencyMap(str);
    // for (std::map<char, int>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
    //     std::cout << it->first << " " << it->second << '\n';
    // }

    // AlphabetHelper helper;
    // std::map<char, int> map = helper.createFrequencyMap(std::cin);
    // for (std::map<char, int>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
    //     std::cout << it->first << " " << it->second << '\n';
    // }

    std::ifstream fis("../gatsby.txt");
    if (!fis.is_open()) {
        std::cerr << "Error: unable to open file.\n";
        return 1;
    }

    AlphabetHelper helper;
    std::map<char, int> map = helper.createFrequencyMap(fis);
    fis.close();
    for (std::map<char, int>::const_iterator it = map.cbegin(); it != map.cend(); ++it) {
        std::cout << it->first << " " << it->second << '\n';
    }
    return 0;
}