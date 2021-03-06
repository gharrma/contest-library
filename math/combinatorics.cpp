#include <iostream>
using namespace std;
using ll = long long;
constexpr ll mod = 1000000000 + 7;

constexpr size_t max_fact = 1000000;
ll fact[max_fact + 1];
void precomp_fact() {
    fact[0] = fact[1] = 1;
    for (ll i = 2; i <= max_fact; ++i) {
        fact[i] = fact[i-1] * i % mod;
    }
}

constexpr size_t max_ncrf = 1000;
double ncrf[max_ncrf + 1][max_ncrf + 1];
void precomp_ncrf() {
    for (size_t i = 0; i <= max_ncrf; ++i) {
        ncrf[i][0] = ncrf[i][i] = 1.;
        for (size_t j = 1; j < i; ++j)
            ncrf[i][j] = ncrf[i-1][j-1] + ncrf[i-1][j];
    }
}

ll mpow(ll b, ll e) {
    if (e == 0) return 1;
    ll rec = mpow(b * b % mod, e / 2);
    return e % 2 ? rec * b % mod : rec;
}

ll inverse(ll n) {
    return mpow(n, mod - 2);
}

// For n on the order of 1e6.
ll ncr(ll n, ll r) {
    return n < r ? 0 : fact[n] * inverse(fact[r] * fact[n - r] % mod) % mod;
}

// For very large n and relatively small r.
ll ncrx(ll n, ll r) {
    if (n < r || n >= mod)
        return 0;
    ll num = 1, denom = 1;
    for (ll i = 0; i < r; ++i) {
        denom = denom * (i + 1) % mod;
        num = num * (n - i) % mod;
    }
    return num * inverse(denom) % mod;
}

int main() {
    precomp_fact();
    precomp_ncrf();
    cout << "No test cases" << endl;
    return 0;
}
