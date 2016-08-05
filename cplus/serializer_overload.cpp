#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <new>
#include <string.h>
#include <openssl/md5.h>

#include <iomanip>

#if !defined(uint8_t)
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
#endif

typedef uint8_t byte;

namespace ascava {
class checksum {
public:
    unsigned char raw_[MD5_DIGEST_LENGTH];
    checksum()
    {
        memset(raw_, 0x0B, MD5_DIGEST_LENGTH);
    } 
    checksum(const checksum& cs)
    {
        memcpy(raw_, cs.raw_, MD5_DIGEST_LENGTH);
    }
    checksum& operator=(const checksum& cs)
    {
        memcpy(raw_, cs.raw_, MD5_DIGEST_LENGTH);
        return *this;
    }
    ~checksum()
    {
        memset(raw_, 0xFE, MD5_DIGEST_LENGTH);
    }

    /*
     * Automatic conversion operator to std::string. Compiler will try to autoconvert to std::string
     * to match a function call.
     * This method will also be called if we are doing an explicit cast, such as,
     *     std::cout << static_cast<std::string>(cs) << std::endl;
     */
    operator std::string () const
    {
        return std::string("Hello-string");
        std::stringstream ss;
        for (int i=0; i<MD5_DIGEST_LENGTH; i++) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int)this->raw_[i];
        }
        return ss.str();
    }
};
} //namespace

/*
 * This implements a serialization operator for my user-defined class.
 * Once this global operator is defined, I can simply serialize an instance of my class into an ostream.
 */
std::ostream& operator<<(std::ostream& os, const ascava::checksum& cs)
{
    for (int i=0; i<MD5_DIGEST_LENGTH; i++) {
        os << std::setfill('0') << std::setw(2) << std::hex << (int)cs.raw_[i];
    }
    return os;
}

int main()
{
    ascava::checksum cs;
    std::string temp(cs);
    std::cout << static_cast<std::string>(cs) << std::endl;
    std::cout << cs << std::endl;
    return 0;
}

