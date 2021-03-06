#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

using ll = long long;

int gcd(int x, int y) { return y ? gcd(y, x % y) : abs(x); }

int lcm(int x, int y) { return x && y ? abs(x) / gcd(x, y) * abs(y) : 0; }

// Finds a and b such that a*x + b*y == gcd(x,y).
pair<int,int> bezout(int x, int y) {
    if (y == 0) return pair<int,int>(1, 0);
    pair<int,int> u = bezout(y, x % y);
    return pair<int,int>(u.second, u.first - (x/y) * u.second);
}

// Note: pi(n) = the number of primes less than n, approximately n / (ln n).
struct sieve {
    ll n;
    vector<ll> primes; // Primes up to n.
    vector<ll> least_prime; // Smallest prime divisor of each number up to n.

    // O(n).
    sieve(ll n) : n(n), least_prime(n+1) {
        for (ll x = 2; x <= n; ++x) {
            if (least_prime[x] == 0) {
                least_prime[x] = x;
                primes.push_back(x);
            }
            for (auto p : primes) {
                if (x * p > n || p > least_prime[x]) break;
                least_prime[x * p] = p;
            }
        }
    }

    // O(1) for x <= n.
    bool is_prime(ll x) {
        assert(0 <= x && x <= n);
        return x <= 1 ? false : least_prime[x] == x;
    }

    // O(lg x) for x <= n.
    vector<pair<ll,ll>> prime_factors(ll x) {
        assert(0 <= x && x <= n);
        vector<pair<ll,ll>> res;
        while (x > 1) {
            ll p = least_prime[x], count = 0;
            while (x % p == 0) ++count, x /= p;
            res.emplace_back(p, count);
        }
        return res;
    }

    // O(pi(sqrt(x))) for x <= n*n.
    bool is_prime_slow(ll x) {
        assert(0 <= x && x <= n*n);
        if (x <= 1) return false;
        for (auto p : primes) {
            if (p * p > x) break;
            if (x % p == 0) return false;
        }
        return true;
    }

    // O(pi(sqrt(x))) for x <= n*n.
    vector<pair<ll,ll>> prime_factors_slow(ll x) {
        assert(0 <= x && x <= n*n);
        vector<pair<ll,ll>> res;
        for (auto p : primes) {
            if (p * p > x) break;
            ll count = 0;
            while (x % p == 0) ++count, x /= p;
            if (count) res.emplace_back(p, count);
        }
        if (x > 1) res.emplace_back(x, 1);
        return res;
    }
};

vector<int> divisors(int n) {
    vector<int> small, large;
    for (int i = 1; i*i <= n; ++i) {
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

    sieve s(100);
    assert(s.is_prime(0) == false);
    assert(s.is_prime(1) == false);
    assert(s.is_prime(2) == true);
    assert(s.is_prime(3) == true);
    assert(s.is_prime(4) == false);
    assert(s.is_prime(5) == true);
    assert(s.is_prime(6) == false);

    assert(s.is_prime_slow(0) == false);
    assert(s.is_prime_slow(1) == false);
    assert(s.is_prime_slow(2) == true);
    assert(s.is_prime_slow(3) == true);
    assert(s.is_prime_slow(4) == false);
    assert(s.is_prime_slow(5) == true);
    assert(s.is_prime_slow(6) == false);

    assert(sieve(10).primes == vector<ll>({2, 3, 5, 7}));

    auto pfactors = vector<pair<ll,ll>>({make_pair(2, 2), make_pair(3, 1)});
    assert(s.prime_factors(12) == pfactors);
    assert(s.prime_factors_slow(12) == pfactors);

    pfactors = vector<pair<ll,ll>>({make_pair(37, 1)});
    assert(s.prime_factors(37) == pfactors);
    assert(s.prime_factors_slow(37) == pfactors);

    vector<int> divs({1, 2, 3, 4, 6, 12});
    assert(divisors(12) == divs);

    cout << "All tests passed" << endl;
    return 0;
}
