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

#define FU(i, a, b) for (remove_const_t<remove_reference_t<decltype(b)>> i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define FD(i, a, b) for (remove_const_t<remove_reference_t<decltype(b)>> i = (b) - 1; i >= (a); --i)
#define fd(i, b) FD(i, 0, b)

#define all(x) (x).begin(), (x).end()
using ll = long long;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
