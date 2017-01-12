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

#define all(x) (x).begin(), (x).end()
using ll = long long;
using vb = vector<bool>;
using vi = vector<int>;
using vll = vector<ll>;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
