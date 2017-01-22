/*
 * Suffix array for an arbitrary sequence of values.
 * O(n lg n lg n), but can be reduced to O(n lg n) using counting sort.
 * v := the sequence of values (usually a string)
 * log := lg(n) (ceil)
 * dp[k][i] := ordered position of substring i among (2^k)-length substrings.
 * order[i] := the start index of the lexicographically ith lowest suffix
 * lcp := longest common prefix
 */
#include <iostream>
#include <vector>
#include <tuple>
#include <cassert>
using namespace std;

template <typename Container>
struct suffix_array {
    const Container v;
    size_t n, log;
    vector<vector<size_t>> dp;
    vector<size_t> order;

    suffix_array(Container v): v(v), n(v.size()), order(n) {
        for (log = 0; 1 << log < n; ++log);
        dp.resize(log + 1, vector<size_t>(n));
        for (size_t i = 0; i < n; ++i)
            order[i] = i;

        for (size_t pow = 0; pow <= log; ++pow) {
            auto cmp_base = [&] (size_t i, size_t j) { return v[i] < v[j]; };
            auto cmp_rec  = [&] (size_t i, size_t j) {
                if (dp[pow-1][i] != dp[pow-1][j])
                    return dp[pow - 1][i] < dp[pow - 1][j];
                auto step = 1 << (pow - 1);
                if (i + step < n && j + step < n)
                    return dp[pow - 1][i + step] < dp[pow - 1][j + step];
                return i > j;
            };

            function<bool(size_t, size_t)> cmp;
            if (pow == 0) cmp = cmp_base; else cmp = cmp_rec;
            sort(order.begin(), order.end(), cmp);

            for (size_t i = 0; i < n; ++i)
                if (i == 0 || cmp(order[i-1], order[i]))
                    dp[pow][order[i]] = i;
                else
                    dp[pow][order[i]] = dp[pow][order[i-1]];
        }
    }

    size_t lcp(size_t i, size_t j) {
        size_t ret = 0;
        for (size_t pow = log; pow != -1 && i < n && j < n; )
            if (dp[pow][i] == dp[pow][j])
                ret += 1 << pow, i += 1 << pow, j += 1 << pow;
        return ret;
    }
};

int main() {
    suffix_array<string> s("bobocel");
    assert(s.dp.back() == vector<size_t>({0,5,1,6,2,3,4}));
    assert(s.order == vector<size_t>({0,2,4,5,6,1,3}));
    cout << "All tests passed" << endl;
    return 0;
}
