// Bob's Game, HackerRank.
// https://www.hackerrank.com/contests/university-codesprint-3/challenges/bobs-game
//
// Bob recently invented a new game. The game is played on an chessboard.
// The rules of the game are:
// - Initially, there are kings in some of the cells.
//     A cell can contain more than one king.
// - A king is only allowed to move in one of three directions:
//     up, left or up-left.
// - Some of the cells on the board might be damaged.
//     A king is not allowed to move to a damaged cell.
// - A king cannot be moved outside the board.
// - There are two players in the game. They make moves alternately.
// - In a single move, a player must choose a single king
//   and move it to a new cell.
// - If a player can't make a move, they lose the game.
//
// Bob is playing the game with his friend Alice. Bob always makes the
// first move. Given the configuration of the board, Find the number of ways
// Bob can make the first move to ensure that he will win the game,
// assuming that both players will play optimally.
//
// Solution:
// Use the Sprague–Grundy theorem, explained
// at http://web.mit.edu/sp.268/www/nim.pdf
//
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
using namespace std;

using ll = long long;

#define FU(i, a, b) for (int i = (a); i < (b); ++i)
#define fu(i, b) FU(i, 0, b)

char g[1010][1010];
int grundy[1010][1010];

int get(int x, int y) {
    if (x < 0 || y < 0)
        return 4;
    return grundy[x][y];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0); cout.tie(0);
    int q;
    cin >> q;
    while (q--) {
        int n;
        cin >> n;
        fu(i, n) fu(j, n)
            cin >> g[i][j];
        fu(i, n) fu(j, n) {
            if (g[i][j] == 'X')
                grundy[i][j] = 4;
            else {
                bool taken[] = {false, false, false, false, false};
                taken[get(i-1,j)] = true;
                taken[get(i,j-1)] = true;
                taken[get(i-1,j-1)] = true;
                fu(k, 5) {
                    if (!taken[k]) {
                        grundy[i][j] = k;
                        break;
                    }
                }
            }
        }
        int res = 0;
        int xx = 0;
        fu(i, n) fu(j, n)
            if (g[i][j] == 'K')
                xx ^= grundy[i][j];
        fu(i, n) fu(j, n) {
            if (g[i][j] == 'K') {
                xx ^= grundy[i][j];
                if ((xx ^ get(i-1,j)) == 0)
                    ++res;
                if ((xx ^ get(i,j-1)) == 0)
                    ++res;
                if ((xx ^ get(i-1,j-1)) == 0)
                    ++res;
                xx ^= grundy[i][j];
            }
        }
        res
            ? cout << "WIN " << res << endl
            : cout << "LOSE" << endl;
    }
    return 0;
}
