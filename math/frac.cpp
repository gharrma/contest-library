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
