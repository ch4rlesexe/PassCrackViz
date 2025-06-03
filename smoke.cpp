// smoke.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <openssl/md5.h>
#include <sstream>
#include <iomanip>

// Reuse the same md5() implementation:
std::string md5(const std::string &input) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;

    MD5_Init(&ctx);
    MD5_Update(&ctx, input.data(), input.size());
    MD5_Final(digest, &ctx);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        oss << std::hex
            << std::setw(2)
            << std::setfill('0')
            << (int)(static_cast<unsigned char>(digest[i]));
    }
    return oss.str();
}

int main() {
    const std::string target = "0d107d09f5bbe40cade3de5c71e9e9b7"; // MD5("letmein")
    std::ifstream infile("wordlist.txt");
    if (!infile.is_open()) {
        std::cerr << "Could not open example_wordlist.txt\n";
        return 1;
    }

    std::string line;
    int attempts = 0;
    while (std::getline(infile, line)) {
        ++attempts;
        // Trim any trailing CR or whitespace, just in case:
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
            line.pop_back();

        std::string h = md5(line);
        std::cout << "Attempt #" << attempts
                  << ": " << line
                  << " -> " << h << "\n";

        if (h == target) {
            std::cout << "Found!\n";
            return 0;
        }
    }
    std::cout << "Not found\n";
    return 0;
}

