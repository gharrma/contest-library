#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cassert>
using namespace std;

using ll = long long;

#define FE(i, a, b)  for (ll i = (a); i < (b); ++i)
#define FI(i, a, b)  FE(i, a, (b) + 1)
#define F(i, b)      FE(i, 0, b)
#define FDE(i, b, a) for (ll i = (ll) (b) - 1; i >= (a); --i)
#define FDI(i, b, a) FDE(i, (b) + 1, a)
#define FD(i, b)     FDE(i, b, 0)

#define all(x) (x).begin(), (x).end()
#define mp make_pair
#define pb push_back

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);
    return 0;
}
