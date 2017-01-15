#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
typedef unsigned long long ull;
constexpr ull mod = 1e9 + 7;

constexpr ull fact_max = 1e6;
vector<ull> fact(fact_max + 1);

ull pow(ull b, ull e) {
    if (e == 0) return 1;
    ull rec = pow(b, e / 2);
    if (e % 2 == 0) return rec * rec % mod;
    else return rec * rec % mod * b % mod;
}

ull inverse(ull n) {
    return pow(n, mod - 2);
}

ull ncr(ull n, ull r) {
    return n < r ? 0 : fact[n] * inverse(fact[r] * fact[n - r] % mod) % mod;
}

int main() {
    fact[0] = fact[1] = 1;
    for (ull i = 2; i <= fact_max; ++i) {
        fact[i] = fact[i-1] * i % mod;
    }
    return 0;
}
