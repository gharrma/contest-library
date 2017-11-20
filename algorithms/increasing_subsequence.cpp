/*
 * Returns a longest increasing subsequence in O(n lg n).
 */
template <typename Seq>
vector<typename Seq::value_type>
increasing_subsequence(const Seq& seq, bool strict = true) {
    auto len = distance(begin(seq), end(seq));
    vector<int> best, pred(len);
    auto cmp = [&](int i, int j) { return seq[i] < seq[j]; };
    for (int i = 0; i < len; ++i) {
        auto find = strict
            ? lower_bound(best.begin(), best.end(), i, cmp)
            : upper_bound(best.begin(), best.end(), i, cmp);
        pred[i] = find == best.begin() ? -1 : *prev(find);
        if (find == best.end()) {
            best.push_back(i);
        } else {
            *find = i;
        }
    }
    // Can return best.size() here if just need length.
    vector<typename Seq::value_type> res(best.size());
    int curr = best.empty() ? -1 : best.back();
    for (int i = best.size(); i > 0; curr = pred[curr])
        res[--i] = seq[curr];
    return res;
}
