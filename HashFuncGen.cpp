#ifndef HASH_FUNC_GEN
#define HASH_FUNC_GEN

#include <cstdlib>
#include <random>

class HashFuncGen {
private:
    std::random_device rd;
    std::mt19937 gen;
    uint64_t p;
    uint64_t mod;

public:
    HashFuncGen() : gen(rd()), mod(static_cast<uint64_t>(1) << 32) {}

    void generateFunc() {
        p = gen() % mod;
        p = p % 2 ? p : p + 1; 
    }

    uint32_t getHash(std::string s) {
        uint64_t res = 0;
        uint64_t mult = 1;

        for (auto i : s) {
            res = (res * mult + i) % mod;
            mult = mult * p % mod;
        }

        return static_cast<uint32_t>(res);
    }
};

#endif