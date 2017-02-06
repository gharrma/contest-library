/**
 * Compute max flow and min cut given an adjacency matrix, source, and sink.
 * Runs in O(mC) time, where m is the number of edges in the graph, and C is
 * the total capacity out of the source.
 */
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
using namespace std;
typedef long long ll;

struct network_flow {
    // cap[i][j]    := capacity from node i to node j
    // flow[i][j]   := flow from node i to node j in max flow
    // source_cc[i] := whether node i is on the source side of the min cut
    // total_flow   := flow out of the source node
    // min_cut      := flow across the min cut
    const vector<vector<ll>>& cap;
    vector<vector<ll>> flow;
    vector<bool> source_cc;
    ll total_flow, min_cut;
    size_t source, sink;
    size_t n_nodes;

    network_flow(const vector<vector<ll>>& cap, size_t source, size_t sink)
        : cap(cap), source(source), sink(sink)
    {
        n_nodes = cap.size();
        flow = vector<vector<ll>>(n_nodes, vector<ll>(n_nodes, 0));
        total_flow = 0;
        min_cut = 0;
        while (augment_or_finish());
    }

private:
    bool augment_or_finish() {
        vector<bool> seen(n_nodes, false);

        vector<size_t> path(n_nodes);
        vector<ll> path_cap(n_nodes);
        path_cap[source] = numeric_limits<ll>::max();

        deque<size_t> frontier;
        frontier.push_back(source);

        // breadth-first search for path from source to sink
        while (!frontier.empty()) {
            size_t n = frontier.front();
            frontier.pop_front();
            seen[n] = true;

            if (n == sink) {
                push_flow(path, path_cap[n]);
                return true;
            }

            for (size_t i = 0; i < n_nodes; i++) {
                if (cap[n][i] - flow[n][i] > 0 && !seen[i]) {
                    path[i] = n;
                    path_cap[i] = min(path_cap[n], cap[n][i]);
                    frontier.push_back(i);
                }
            }
        }

        // no path; remove negative flows, calculate total flow and min cut
        for (size_t i = 0; i < n_nodes; i++)
            for (size_t j = 0; j < n_nodes; j++)
                flow[i][j] = max(0ll, flow[i][j]);

        for (size_t i = 0; i < n_nodes; i++)
            total_flow += flow[source][i];

        source_cc = seen;
        for (size_t i = 0; i < n_nodes; i++)
            for (size_t j = 0; j < n_nodes; j++)
                if (source_cc[i] && !source_cc[j])
                    min_cut += flow[i][j];

        return false;
    }

    void push_flow(vector<size_t>& path, ll amount) {
        for (size_t n = sink; n != source; n = path[n]) {
            size_t prev = path[n];
            flow[prev][n] += amount;
            flow[n][prev] -= amount;
        }
    }
};

int main() {
    cout << "No test cases yet" << endl;
    return 0;
}
