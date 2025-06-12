#ifndef STREAM_GUARD_H
#define STREAM_GUARD_H

#include <ios>
class StreamGuard {
public:
    explicit StreamGuard(std::basic_ios< char >& ios):
        ios_(ios),
        precision_(ios.precision()),
        fmtFlags_(ios.flags()),
        fill_(ios.fill())
    {

    }

    ~StreamGuard() {
        ios_.precision(precision_);
        ios_.flags(fmtFlags_);
        ios_.fill(fill_);
    }

private:
    std::basic_ios< char >& ios_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmtFlags_;
    char fill_;
};

#endif //STREAM_GUARD_H
