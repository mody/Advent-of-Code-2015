#include <fmt/core.h>
#include <limits>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

using CryptoPP::byte;

int main()
{
    CryptoPP::Weak::MD5 md5;
    std::string digest;

    for (unsigned i = 0; i < std::numeric_limits<unsigned>::max(); ++i) {
        const std::string msg = "ckczppom" + std::to_string(i);
        md5.Update((const byte*)&msg[0], msg.size());
        digest.resize(md5.DigestSize());
        md5.Final((byte*)&digest[0]);

        if (digest[0] == 0 && digest[1] == 0 && (digest[2] & 0xf0) == 0) {
            fmt::print("1: {}\n", i);
            break;
        }
    }

    return 0;
}
