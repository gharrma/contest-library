/*
 * Counting sort: O(n + max). Can be 3-5 times faster than std::sort
 * for n on the order of 1000 or so. Also stable.
 * f := a function mapping elements to integers in the range [0, max).
 */
#include <iostream>
#include <vector>
#include <cassert>
using namespace std;

template <typename Iterator, typename Mapping>
void counting_sort(Iterator begin, Iterator end, size_t max, Mapping f) {
    vector<size_t> count(max + 1);
    for (auto it = begin; it != end; ++it)
        ++count[f(*it) + 1];
    for (size_t i = 1; i < max; ++i)
        count[i] += count[i-1];
    vector<typename Iterator::value_type> sorted(distance(begin, end));
    for (auto it = begin; it != end; ++it)
        sorted[count[f(*it)]++] = *it;
    for (auto val : sorted)
        *(begin++) = val;
}

template <typename Iterator>
void counting_sort(Iterator begin, Iterator end, size_t max) {
    using T = typename Iterator::value_type;
    return counting_sort(begin, end, max, [] (T n) -> size_t { return n; });
}

int main() {
    int n = 1000;
    vector<int> a(n), b(n);
    for (int t = 0; t < 10000; ++t) {
        for (int i = 0; i < n; ++i)
            a[i] = b[i] = rand() % n;
        sort(a.begin(), a.end());
        counting_sort(b.begin(), b.end(), n);
        assert(a == b);
    }
    cout << "All tests passed" << endl;
    return 0;
}
