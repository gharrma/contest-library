#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;

using ll = long long;
using pll = pair<ll,ll>;

#define FE(i, a, b)  for (ll i = (a), i##__end = (ll) (b); i < i##__end; ++i)
#define FI(i, a, b)  for (ll i = (a), i##__end = (ll) (b); i <= i##__end; ++i)
#define F(i, b)      FE(i, 0, b)
#define FER(i, a, b) for (ll i = (ll) (b), i##__end = (ll) (a); i-- > i##__end; )
#define FIR(i, a, b) for (ll i = (ll) (b), i##__end = (ll) (a); i >= i##__end; --i)
#define FR(i, b)     FER(i, 0, b)

#define all(x) (x).begin(), (x).end()

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
