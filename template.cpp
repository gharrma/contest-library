#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;

#define TRACE(x) x
#define WATCH(x) TRACE(cout << #x" = " << x << endl)
#define WATCHR(b, e) TRACE( \
    for (auto it = b; it != e; ++it) \
        { if (it != b) cout << " "; cout << *it; } \
    cout << endl)
#define WATCHC(v) TRACE({cout << #v" = "; WATCHR(v.begin(), v.end());})

using ll = long long;

#define FU(i, a, b) for (auto i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define FD(i, a, b) for (auto i = (b) - 1; i >= (a); --i)
#define fd(i, b) FD(i, 0, b)
#define all(x) (x).begin(), (x).end()
#define mp make_pair
#define pb emplace_back

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
