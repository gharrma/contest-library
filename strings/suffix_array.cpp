/*
 * Suffix array for an arbitrary sequence of values.
 * Gives the lexicographic ordering of all suffixes of the input sequence,
 *   enabling quick operations such as longest common prefix (LCP).
 * O(n lg n lg n) construction, but reducible to O(n lg n) with counting sort.
 * O(lg n) to find the longest common prefix of two suffixes.
 * O(|W| lg n) to search for a substring W.
 *
 * v := the sequence of values (usually a string)
 * log := lg(n) (ceil)
 * dp[k][i] := sorted position of substring i among (2^k)-length substrings.
 * order[i] := the start index of the ith lexicographically lowest suffix
 */
template <typename Sequence>
struct suffix_array {
    const Sequence v;
    int n, log;
    vector<vector<int>> dp;
    vector<int> order;

    suffix_array(const Sequence v)
        : v(v), n(distance(begin(v), end(v))), log(0), order(n)
    {
        while (1 << log < n)
            ++log;
        dp.resize(log + 1, vector<int>(n));
        for (int i = 0; i < n; ++i)
            order[i] = i;

        for (int pow = 0; pow <= log; ++pow) {
            auto cmp = [&](int i, int j) {
                if (pow == 0)
                    return v[i] < v[j];
                if (dp[pow-1][i] != dp[pow-1][j])
                    return dp[pow-1][i] < dp[pow-1][j];
                auto step = 1 << (pow-1);
                if (i + step < n && j + step < n)
                    return dp[pow-1][i+step] < dp[pow-1][j+step];
                return i > j;
            };
            sort(order.begin(), order.end(), cmp);
            for (int i = 0; i < n; ++i) {
                auto diff = i == 0 || cmp(order[i-1], order[i]);
                dp[pow][order[i]] = diff ? i : dp[pow][order[i-1]];
            }
        }
    }

    int lcp(int i, int j) {
        int len = 0;
        for (int pow = log; pow >= 0 && i < n && j < n; --pow)
            if (dp[pow][i] == dp[pow][j])
                len += 1 << pow, i += 1 << pow, j += 1 << pow;
        return len;
    }

    int lower_bound(const Sequence& key) {
        int i = -1;
        for (int step = 1 << log; step > 0; step >>= 1)
            if (i + step < n)
                if (lexicographical_compare(
                        begin(v) + order[i+step], end(v),
                        begin(key), end(key)))
                    i += step;
        return i + 1;
    }

    bool has_substr(const Sequence& key) {
        int lb = lower_bound(key);
        return distance(begin(key), end(key)) <= n - order[lb]
            && equal(begin(key), end(key), begin(v) + order[lb]);
    }
};
