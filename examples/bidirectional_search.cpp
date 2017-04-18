// Moscow Pre-Finals Workshop 2017, Problem K: KingDonalds.
//
// It is sometimes necessary to reassemble a cheeseburger.
// The only feasible operation is the bit-shuffle (burger-ineptly-transformed).
// The bit-shuffle separates the entire burger into four parts of contiguous
// ingredients a, b, c and d and arranges them in the new order c a d b.
// The size of each of the four parts is selectable and may be zero.
// Since the burger cools rapidly we are interested in the minimum required
// bit-shuffles to arrive at an acceptable burger.
// Each given cheeseburger consists of n unique ingredients labeled from 1 to n.
// The correct order is always the natural order 1, 2, …, n.
//
// Input
// First line of the input consists of one integer n (1 ≤ n ≤ 10), where n is
// the number of ingredients used. Second line contains n integers describing
// the order of the ingredients of the given cheeseburger. The ingredients are
// numbered from 1 to n.
//
// Output
// Output the minimum number of bit-shuffles to correct the given cheeseburger.
//
// Solution
// Represent states with integers, then use bidirectional breadth-first-search.
//
#include <iostream>
#include <algorithm>
#include <deque>
#include <unordered_map>
using namespace std;
using ll = long long;

#define FU(i, a, b) for (auto i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define pb push_back

ll n;
unordered_map<ll,ll> dist[2];
deque<ll> q[2];

inline ll shift(ll s, ll l, ll r, ll offset) {
    ll mask = (1ll << (4*r)) - 1ll;
    ll isolated = (s & mask) >> (4*l);
    return isolated << (4*offset);
}

void edges(ll state, ll dir) {
    FU(i, 0, n+1) FU(j, i, n+1) FU(k, j, n+1) {
        ll a = i - 0;
        ll b = j - i;
        ll c = k - j;
        ll d = n - k;
        ll next = 0;
        next |= shift(state, 0, i, dir ? b + d : c);
        next |= shift(state, i, j, dir ? 0 : c + a + d);
        next |= shift(state, j, k, dir ? b + d + a : 0);
        next |= shift(state, k, n, dir ? b : c + a);
        if (!dist[dir].count(next)) {
            q[dir].pb(next);
            dist[dir][next] = dist[dir][state] + 1;
            if (dist[1-dir].count(next)) {
                cout << (dist[dir][next] + dist[1-dir][next]) << endl;
                exit(0);
            }
        }
    }
}

void expand(ll dir, ll maxdist) {
    while (dist[dir][q[dir].front()] < maxdist) {
        ll state = q[dir].front();
        q[dir].pop_front();
        edges(state, dir);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    ll s = 0, f = 0;
    fu(i, n) {
        ll k;
        cin >> k;
        s |= k << (4*i);
        f |= (i+1ll) << (4*i);
    }

    if (s == f) {
        cout << 0 << endl;
        return 0;
    }

    q[0].pb(s);
    dist[0][s] = 0;
    q[1].pb(f);
    dist[1][f] = 0;

    for (ll maxdist = 1;; ++maxdist) {
        expand(0, maxdist);
        expand(1, maxdist);
    }
}
