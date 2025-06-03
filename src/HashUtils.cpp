#include "HashUtils.h"
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

std::string HashUtils::md5(const std::string &input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;

    MD5_Init(&ctx);
    MD5_Update(&ctx, input.data(), input.size());
    MD5_Final(digest, &ctx);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        // Force two hex digits per byte (0x00 → "00", 0x0f → "0f", etc.)
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << (int)(static_cast<unsigned char>(digest[i]));
    }
    return oss.str();
}

