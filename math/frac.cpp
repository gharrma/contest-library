#include <iostream>
#include <sstream>
#include <cassert>
using namespace std;

using ll = long long;

ll gcd(ll x, ll y) { return y ? gcd(y, x % y) : abs(x); }

struct frac {
    ll a, b;

    frac(ll aa = 1, ll bb = 1): a(aa), b(bb) {
        if (b < 0)
            a = -a, b = -b;
        ll d = gcd(a, b);
        a /= d, b /= d;
    }

    frac operator+(frac o) {
        return frac(a * o.b + o.a * b, b * o.b);
    }

    frac operator-(frac o) {
        return frac(a * o.b - o.a * b, b * o.b);
    }

    frac operator*(frac o) {
        return frac(a * o.a, b * o.b);
    }

    friend ostream& operator<<(ostream& os, frac f) {
        return f.b == 1
            ? os << f.a
            : os << f.a << '/' << f.b;
    }
};

int main() {
    stringstream ss;
    ss << frac(0) << " "
       << frac(1) << " "
       << frac(1,2) << " "
       << frac(2,4) << " "
       << (frac(1,2) * frac(2)) << " "
       << (frac(1,2) + frac(2,3)) << " "
       << (frac(1,2) - frac(2,3));
    assert(ss.str() == "0 1 1/2 1/2 1 7/6 -1/6");
    cout << "All tests passed" << endl;
    return 0;
}
