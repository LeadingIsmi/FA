#ifndef TEX_CONVERTIBLE
#define TEX_CONVERTIBLE
#include <iostream>
#include <string.h>

class TeX_convertible {

public:
    virtual std::string convert() const = 0;
};

#endif
