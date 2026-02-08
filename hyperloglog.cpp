#ifndef HYPERLOGLOG
#define HYPERLOGLOG

#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>

class HyperLogLog {
private:
    int b;
    int q;
    std::vector<int> rank;

public:
    HyperLogLog(int b) {
        this->b = b;
        q = 1 << b;
        rank.resize(q);
    }

    void clear() {
        std::fill(rank.begin(), rank.end(), 0);
    }

    void add(uint32_t a) {
        uint32_t reg = a >> (32 - b);
        int pos = -1;
        for (int i = 31 - b; i >= 0; --i) {
            if (a & (1 << i)) {
                pos = 31 - b - i + 1;
                break;
            }
        }
        if (pos == -1) {
            rank[reg] = 32 - b;
            return;
        }
        rank[reg] = std::max(pos, rank[reg]);
    }

    double count() {
        double alpha = 0.7213 / (1.0 + 1.079 / static_cast<double>(q));

        double sum = 0;
        for (int i = 0; i < q; ++i) {
            sum += 1.0 / static_cast<double>(1 << rank[i]);
        }

        return alpha * q * q / sum;
    }
};

#endif