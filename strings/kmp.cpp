/*
 * Knuth-Morris-Pratt algorithm.
 * O(n) precomputation and O(m) query, where n is the size of the
 * needle to find, and m is the size of the haystack.
 * Assumes n > 0.
 */
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

template <typename Container>
struct kmp {
    const Container needle;
    int n;
    vector<int> succ;

    kmp(const Container needle)
        : needle(needle)
        , n(distance(begin(needle), end(needle)))
        , succ(n + 1)
    {
        succ[0] = -1;
        succ[1] = 0;
        for (int cur = 0, i = 2; i < n; ) {
            if (needle[i-1] == needle[cur]) succ[i++] = ++cur;
            else if (cur) cur = succ[cur];
            else succ[i++] = 0;
        }
    }

    int find(const Container& haystack) const {
        int len = distance(begin(haystack), end(haystack));
        for (int m = 0, i = 0; m + i < len; ) {
            if (needle[i] == haystack[m + i]) {
                if (i == n - 1)
                    return m;
                ++i;
            } else if (succ[i] != -1) {
                m = m + i - succ[i];
                i = succ[i];
            } else {
                ++m;
                i = 0;
            }
        }
        return n;
    }

    bool in(const Container& haystack) const {
        return find(haystack) < n;
    }
};

int main() {
    kmp<string> k("ababba");
    assert(k.in("abaaababbaabaaba"));
    assert(!k.in("abaaabbabbba"));
    cout << "All tests passed" << endl;
}
