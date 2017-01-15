#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;

#define TRACE(x) x
#define WATCH(x) TRACE(cout << #x" = " << x << endl)
#define WATCHR(b, e) TRACE(for (auto it = b; it != e; ++it) \
    { if (it != b) cout << " "; cout << *it; } \
    cout << endl)
#define WATCHC(v) TRACE({cout << #v" = "; WATCHR(v.begin(), v.end());})

using ll = long long;
using vll = vector<ll>;
using vvll = vector<vll>;
using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;
using vvb = vector<vb>;

#define FU(i, a, b) for (auto i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define FD(i, a, b) for (auto i = (b) - 1; i >= (a); --i)
#define fd(i, b) FD(i, 0, b)
#define all(x) (x).begin(), (x).end()

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
