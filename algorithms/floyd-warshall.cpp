#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
using ll = long long;
constexpr ll infty = numeric_limits<ll>::max() / 2;

int main() {
    int n, m;
    vector<vector<ll>> dist(n, vector<ll>(n, infty));
    for (ll i = 0; i < n; ++i)
        dist[i][i] = 0;
    for (ll i = 0; i < m; ++i) {
        ll a, b, d;
        cin >> a >> b >> d;
        --a, --b;
        dist[a][b] = dist[b][a] = d;
    }

    for (ll k = 0; k < n; ++k)
        for (ll i = 0; i < n; ++i)
            for (ll j = 0; j < n; ++j)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

    return 0;
}
