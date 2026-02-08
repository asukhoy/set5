#ifndef RANDOM_STREAM_GEN
#define RANDOM_STREAM_GEN

#include <vector>
#include <random>

class RandomStreamGen {
    private:
        std::vector<std::string> stream;
        std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-";
        std::random_device rd;
        std::mt19937 gen;
    public:
        RandomStreamGen() : gen(rd()) {}
        void generateStream(int n) {
            stream.clear();
            stream.resize(n);

            for (int i = 0; i < n; ++i) {
                int sz = gen() % 30 + 1;
                std::string s(sz, 'a');

                for (int j = 0; j < sz; ++j) {
                    s[j] = alphabet[gen() % alphabet.size()];
                }

                stream[i] = s;
            }
        }

        std::vector<std::string> getStream(double part = 1) {
            int sz = static_cast<double>(stream.size()) * part;

            return std::vector<std::string>(stream.begin(), stream.begin() + sz);
        }
};

#endif