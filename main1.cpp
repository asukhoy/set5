#include <iostream>
#include <unordered_set>
#include <string>
#include <vector>
#include <cstdint>
#include "HashFuncGen.cpp"
#include "RandomStreamGen.cpp"
#include "hyperloglog.cpp"


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    RandomStreamGen gen;
    HashFuncGen hash;

    

    HyperLogLog log(14);

    int cnt = 10;

    int t = 10;

    std::vector<std::vector<double>> stats(t);

    while (t--) {
        hash.generateFunc();

        gen.generateStream(100000);
        std::cout << gen.getStream().size() << '\n';

        double part = 0.1;

        while (part <= 1.0) {
            auto v = gen.getStream(part);
            std::unordered_set<uint32_t> hashes;
            for (auto i : v) {
                auto hs = hash.getHash(i);
                hashes.insert(hs);
                log.add(hs);
            }
            int a = hashes.size();
            double b = log.count();
            stats[t].emplace_back(b);
            std::cout << a << ' ' << b << '\n';
            log.clear();
            part += 0.1;
        }
    }
    
    std::cout << '\n';

    std::vector<double> mean(cnt);
    std::vector<double> diff(cnt);
    
    for (int i = 0; i < cnt; ++i) {
        for (auto j : stats) {
            mean[i] += j[i];
        }
        mean[i] /= static_cast<double>(cnt);

        double tmp = 0;

        for (auto j : stats) {
            tmp += (j[i] - mean[i]) * (j[i] - mean[i]);
        }

        tmp /= cnt;
        diff[i] = std::sqrt(tmp);
    }

    for (int i = 0; i < cnt; ++i) {
        std::cout << mean[i] << ' ' << diff[i] << '\n';
    }
}