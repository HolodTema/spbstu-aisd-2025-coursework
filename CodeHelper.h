#ifndef CODE_HELPER_H
#define CODE_HELPER_H

class CharInfo {
public:
    CharInfo(const wchar_t& ch, const double& probability):
        ch_(ch),
        probability_(probability)
    { }

private:
    wchar_t ch_;
    double probability_;
};

class CodeHelper {
public:

};

#endif //CODE_HELPER_H
