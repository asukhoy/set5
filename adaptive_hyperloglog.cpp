#ifndef HYPERLOGLOG
#define HYPERLOGLOG

#include <cstdint>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_set>

class HyperLogLog {
private:
    bool optimized;
    int b;
    int q;
    double alpha;
    std::vector<uint8_t> rank;
    std::unordered_set<uint32_t> st;

    void toNormal() {
        rank.resize(q);
        for (auto i : st) {
            add(i);
        }
        st.clear();
    }

public:
    HyperLogLog(int b) {
        this->b = b;
        q = 1 << b;
        optimized = true;
        alpha = 0.7213 / (1.0 + 1.079 / static_cast<double>(q));
    }

    void clear() {
        // std::fill(rank.begin(), rank.end(), 0);
        rank.clear();
        optimized = true;
        st.clear();
    }

    void add(uint32_t a) {
        if (optimized) {
            st.insert(a);
            if (st.size() == 4000) {
                optimized = false;
                toNormal();
            }
            return;
        }
        uint32_t reg = a >> (32 - b);
        uint8_t pos = 32 - b;
        for (int i = 31 - b; i >= 0; --i) {
            if (a & (1 << i)) {
                pos = 31 - b - i + 1;
                break;
            }
        }
        
        rank[reg] = std::max(pos, rank[reg]);
    }

    double count() {
        if (optimized) {
            return st.size();
        }

        double sum = 0;
        for (int i = 0; i < q; ++i) {
            sum += 1.0 / static_cast<double>(1 << rank[i]);
        }

        double res = alpha * q * q / sum;

        if (res < 5 * q / 2) {
            double v = 0;
            for (int i = 0; i < q; ++i) {
                if (rank[i] == 0) {
                    ++v;
                }
            }

            if (v != 0) {
                res = static_cast<double>(q) * std::log(static_cast<double>(q) / v);
            }
        }

        return res;
    }
};

#endif