#include <bits/stdc++.h>
using namespace std;

// Kan optimeras genom att långsamt lägga till större kanter

struct MaxFlow {
  struct Edge {
    int flow, capacity;
    Edge(int f, int c) : flow(f), capacity(c) {};
  };

  vector<Edge> edges;
  vector<vector<pair<int, int>>> adj;
  int n;

  MaxFlow(int tn) : adj(tn), n(tn) {}
  void create_edge(int u, int v, int c) {
    adj[u].push_back({v, edges.size()});
    edges.push_back(Edge(0, c));
    adj[v].push_back({u, edges.size()});
    edges.push_back(Edge(0, 0));
  }

  int maxflow(int from, int to) {
    vector<pair<int, int>> vis(n);
    while (1) {
      fill(vis.begin(), vis.end(), make_pair(-1, -1));
      deque<int> que;
      que.push_back(from);
      vis[from] = {-2, -2};
      while (0 < que.size()) {
        int u = que.front();
        que.pop_front();
        if (u == to)
          break;
        for (auto [v, id] : adj[u]) {
          if (vis[v].first != -1)
            continue;
          int left = edges[id].capacity - edges[id].flow;
          if (left <= 0)
            continue;
          vis[v] = {u, id};
          que.push_back(v);
        }
      }

      if (vis[to].first == -1)
        break;

      int u = to;
      int bottle = 1e9;
      while (u != from) {
        auto [nu, id] = vis[u];
        bottle = min(bottle, edges[id].capacity - edges[id].flow);
        u = nu;
      }

      u = to;
      while (u != from) {
        auto [nu, id] = vis[u];
        edges[id].flow += bottle;
        edges[id ^ 1].flow -= bottle;
        u = nu;
      }
    }

    int ans = 0;
    for (int u = 0; u < n; u++) {
      for (auto [v, id] : adj[u]) {
        if (0 < (id & 1))
          continue;
        if (u == from)
          ans += edges[id].flow;
        if (v == from)
          ans -= edges[id].flow;
      }
    }

    return ans;
  }

  bool get_path(int u, int to, vector<int> &path) {
    path.push_back(u);
    if (u == to)
      return true;
    for (auto [v, id] : adj[u])
      if (0 < edges[id].flow) {
        edges[id].flow--;
        return get_path(v, to, path);
      }
    return false;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  cin >> n >> m;

  MaxFlow flow(n);
  for (int i = 0; i < m; i++) {
    int a, b;
    cin >> a >> b;
    a--;
    b--;
    flow.create_edge(a, b, 1);
  }

  int ans = flow.maxflow(0, n - 1);
  cout << ans << endl;

  vector<int> path;
  while (flow.get_path(0, n - 1, path)) {
    cout << path.size() << "\n";
    for (int node : path)
      cout << node + 1 << " ";
    cout << "\n";
    path.clear();
  }

  return 0;
}
