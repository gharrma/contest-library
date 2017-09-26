// Fraudulent Activity Notifications, HackerRank.
//
// HackerLand National Bank has a simple policy for warning clients about
// possible fraudulent account activity. If the amount spent by a client on a
// particular day is greater than or equal to  the client's median spending for
// the last d days, they send the client a notification about potential fraud.
// The bank doesn't send the client any notifications until they have at least
// d prior days of transaction data.
//
// Given the value of d and a client's total daily expenditures for a period of
// n days, find and print the number of times the client will receive a
// notification over all n days.
//
#include <iostream>
#include <set>
#include <vector>
using namespace std;

multiset<int> s;
auto it = s.end();

void add(int x) {
    bool split = s.size() % 2 == 0;
    s.insert(x);
    if (s.size() == 1) {
        it = s.begin();
    } else if (split && x >= *it) {
        ++it;
    } else if (!split && x < *it) {
        --it;
    }
}

void remove(int x) {
    bool split = s.size() % 2 == 0;
    if (x == *it) {
        split ? s.erase(it++) : s.erase(it--);
    } else {
        s.erase(s.find(x));
        if (split && x < *it) {
            ++it;
        } else if (!split && x > *it) {
            --it;
        }
    }
}

int doubleMedian() {
    return s.size() % 2 ? 2*(*it) : *it + *next(it);
}

int main() {
    int n, d;
    cin >> n >> d;
    vector<int> a(n);
    for (int i = 0; i < n; ++i)
         cin >> a[i];
    int res = 0;
    for (int i = 0; i < n; ++i) {
        if (i >= d && a[i] >= doubleMedian())
            ++res;
        if (i >= d)
            remove(a[i-d]);
        add(a[i]);
    }
    cout << res << endl;
}
