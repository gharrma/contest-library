// NAIPC 2017, Problem D: Heaps from Trees.
// https://naipc17.kattis.com/problems/naipc17.heapstrees
//
// You are given a rooted tree with n nodes. The nodes are labeled 1..n
// and node 1 is the root. Each node has a value v_i.
//
// You would like to turn this tree into a heap. That is, you would like to
// choose the largest possible subset of nodes that satisfy this heap property:
// For every node pair i,j in the subset, if node i is an ancestor of node j
// in the tree, then v_i > v_j. Note that equality is not allowed.
//
// Figure out the maximum number of nodes you can choose to form such a subset.
// The subset does not have to form a subtree.
//
// Input
// The first line of input will contain a single integer n (1 ≤ n ≤ 200,000),
// which is the number of nodes in the tree. The nodes are numbered 1..n.
//
// Each of the next n lines will describe the nodes, in order.
// They will each contain two integers v_i and p_i, where v_i
// (0 ≤ v_i ≤ 1,000,000,000) is the value in the node, and p_i (0 ≤ p_i < i) is
// the index of its parent. Every node’s index will be strictly greater than
// its parent node’s index. Only node 1, the root, will have p_1 = 0, since it
// has no parent. For all other nodes (i = 2..n), 1 ≤ p_i < i.
//
// Output
// Output a single integer representing the number of nodes in the
// largest subset satisfying the Heap Property.
//
// Solution
// Use heavy path decomposition and a generalization of longest
// decreasing subsequence. O(n lg n lg n).
//
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

#define FU(i, a, b) for (int i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define FD(i, a, b) for (int i = (int) (b) - 1; i >= (a); --i)
#define fd(i, b) FD(i, 0, b)
#define all(x) (x).begin(), (x).end()
#define mp make_pair
#define pb push_back

vector<int> g[200200];
multiset<int> dp[200200];
int val[200200];

void comp(int x) {
    if (g[x].empty()) {
        dp[x].insert(val[x]);
        return;
    }

    int l = *max_element(all(g[x]), [](int a, int b) {
        return dp[a].size() < dp[b].size();
    });
    dp[x] = move(dp[l]); // Merge into largest child dp.
    for (auto n : g[x])
        if (n != l)
            for (auto v : dp[n])
                dp[x].insert(v);

    auto it = dp[x].lower_bound(val[x]);
    if (it != dp[x].end())
        dp[x].erase(it);
    dp[x].insert(val[x]);
}

int main() {
    ios::sync_with_stdio(false);
    int n;
    cin >> n;
    fu(i, n) {
        int v, p;
        cin >> v >> p;
        val[i+1] = v;
        if (p) {
            g[p].pb(i+1);
        }
    }
    fd(i, n) comp(i+1);
    cout << dp[1].size() << endl;
    return 0;
}
