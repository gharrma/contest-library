// March of the King, HackerRank.
// https://www.hackerrank.com/contests/university-codesprint-3/challenges/march-of-the-king
//
// King George invented a new game to keep him occupied. King George has a
// secret word in his mind. It is the word he used to shout instead of the usual
// "checkmate" after beating the black king in a chess game. King George loves
// to march, so he decided to take a tour through the chessboard. He assigns a
// single lowercase letter to each square of the chessboard first. He can then
// start his tour from any square on the chessboard. From a square, the king can
// move to any neighboring square inside the board as in the game of chess.
// However, it is not allowed to move to the same square twice during the
// journey. In other words, all the squares visited by the king must be
// distinct.
//
// After completing his tour (the tour can be stopped anytime), George
// concatenates all the characters in the order of his travel to form a word.
// Since he has nothing better to do, the white king wants to know how many
// different ways he can complete the tour so that the secret word is
// formed afterward.
//
//
// Solution:
//
// Words can have up to 11 letters, so directly counting all paths with length
// 11 will take too long.
//
// Split the secret word into [l, m, r], where m is a single letter in the
// middle. Starting at a position on the board containing m, count the paths
// spelling l (backwards) and then count the paths spelling r. The product of
// these counts gives the number of paths centered at m, but we overcount since
// some of these paths intersect themselves. So we increment a counter for each
// subset of the nodes in each l-path, and then use the inclusion-exclusion
// principle when processing r-paths to subtract off paths that we double-count
// due to intersections.
//
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
using namespace std;

using ll = long long;

#define FU(i, a, b) for (int i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)
#define all(x) (x).begin(), (x).end()

int k;
string s, l, r;
char m, g[10][10];
bool seen[10][10];
ll p[12], o[12], res;
unordered_map<ll,ll> c;

inline int flat(int x, int y) { return 8*x + y + 1; }

void dfs(int x, int y, int d, bool left) {
    auto& str = left ? l : r;
    if (d < str.size()) {
        // Extend path and recurse.
        seen[x][y] = true;
        for (int i = x-1; i <= x+1; ++i) {
            for (int j = y-1; j <= y+1; ++j) {
                if (i < 0 || j < 0 || i >= 8 || j >= 8)
                    continue;
                if (seen[i][j] || g[i][j] != str[d])
                    continue;
                p[d] = flat(i, j);
                dfs(i, j, d + 1, left);
            }
        }
        seen[x][y] = false;
    }
    else {
        // Process path.
        copy(p, p + d, o);
        sort(o, o + d);
        for (int mask = 0; mask < (1 << d); ++mask) {
            ll key = 0;
            int offset = 0;
            fu(i, d)
                if (mask & (1 << i))
                    key |= o[i] << (8 * offset++);
            // Inclusion-exclusion principle.
            left ? ++c[key] : res += (offset % 2 ? -1 : 1) * c[key];
        }
    }
}

int main() {
    cin >> k >> s;
    l = s.substr(0, k/2);
    reverse(all(l));
    m = s[k/2];
    r = s.substr(k/2+1, s.size());
    fu(i, 8) fu(j, 8) cin >> g[i][j];
    fu(i, 8) fu(j, 8) {
        if (g[i][j] == m) {
            c.clear();
            dfs(i, j, 0, true);
            dfs(i, j, 0, false);
        }
    }
    cout << res << endl;
    return 0;
}
