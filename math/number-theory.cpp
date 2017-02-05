#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

template <typename T>
T gcd(T x, T y) { return y ? gcd(y, x % y) : abs(x); }

template <typename T>
T lcm(T x, T y) { return x && y ? abs(x) / gcd(x, y) * abs(y) : 0; }

// Finds a and b such that a*x + b*y == gcd(x,y).
template <typename T>
pair<T,T> bezout(T x, T y) {
    if (y == 0) return pair<T,T>(1, 0);
    pair<T,T> u = bezout(y, x % y);
    return pair<T,T>(u.second, u.first - (x/y) * u.second);
}

vector<bool> sieve(int n) {
    if (n == 0)
        return vector<bool>(1);
    vector<bool> prime(n+1, true);
    prime[0] = prime[1] = false;
    for (int i = 2; i <= n; ++i)
        if (prime[i])
            for (int j = i + i; j <= n; j += i)
                prime[j] = false;
    return prime;
}

// Returns all primes up to and including n.
vector<int> primes(int n) {
    auto s = sieve(n);
    vector<int> p;
    for (int i = 0; i <= n; ++i)
        if (s[i])
            p.emplace_back(i);
    return p;
}

// Note: can precompute primes if needed.
template <typename T>
vector<pair<T,int>> prime_factors(T n) {
    vector<pair<T,int>> factors;
    for (T i = 2; i*i <= n; ++i) {
        if (n % i == 0) {
            factors.emplace_back(i, 0);
            while (n % i == 0) {
                ++factors.back().second;
                n /= i;
            }
        }
    }
    if (n > 1)
        factors.emplace_back(n, 1);
    return factors;
}

template <typename T>
vector<T> divisors(T n) {
    vector<T> small, large;
    for (T i = 1; i*i <= n; ++i) {
        if (n % i == 0) {
            small.emplace_back(i);
            if (i*i != n) {
                large.emplace_back(n / i);
            }
        }
    }
    copy(large.rbegin(), large.rend(), back_inserter(small));
    return small;
}

int main() {
    assert(gcd(12, 8) == 4);
    assert(lcm(12, 8) == 24);
    assert(bezout(11, 8) == make_pair(3, -4));

    assert(primes(10) == vector<int>({2, 3, 5, 7}));

    vector<pair<int,int>> pfactors({make_pair(2, 2), make_pair(3, 1)});
    assert(prime_factors(12) == pfactors);

    vector<int> divs({1, 2, 3, 4, 6, 12});
    assert(divisors(12) == divs);

    return 0;
}
