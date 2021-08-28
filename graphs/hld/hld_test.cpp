// timus:https://acm.timus.ru/problem.aspx?space=1&num=1553

#include <bits/stdc++.h>
#define FAST_IO ios::sync_with_stdio(0); cin.tie(nullptr)
#define debug(x) cerr << "debug " << (#x) << ": " << (x) << endl
#define rall(x) (x).rbegin(), (x).rend()
#define all(x) (x).begin(), (x).end()
#define sz(x) int((x).size())
#define ss second
#define ff first
#define endl '\n'
using namespace std;
typedef pair<int, int> ii;
typedef long long Long;
const int N = 250009;
const int LG = 32 - __builtin_clz(N);
const int INF = 1e9;
vector<int> adj[N];

struct SegmentTree {
  int tree[4 * N];
  int _n;
  SegmentTree(int _n): _n(_n) {}
  Long Merge(const int& x) { return x; }
  Long Merge(const Long& x, const Long& y) { return max(x, y); }
  void Build(const vector<int>& a, int id, int tl, int tr) {
    if (tl == tr) tree[id] = Merge(a[tl]);
    else {
      int tm = (tl + tr) >> 1;
      Build(a, 2 * id, tl, tm);
      Build(a, 2 * id + 1, tm + 1, tr);
      tree[id] = Merge(tree[2 * id], tree[2 * id + 1]);
    }
  }
  void Build(const vector<int>& a) { Build(a, 1, 0, _n - 1); }
  void Update(int pos, int val, int id, int tl, int tr) {
    if (tl == tr) tree[id] += Merge(val);
    else {
      int tm = (tl + tr) >> 1;
      if (pos <= tm) Update(pos, val, 2 * id, tl, tm);
      else Update(pos, val, 2 * id + 1, tm + 1, tr);
      tree[id] = Merge(tree[2 * id], tree[2 * id + 1]);
    }
  }
  void Update(int pos, int val) { Update(pos, val, 1, 0, _n - 1); }
  Long Query(int l, int r, int id, int tl, int tr) {
    if (l <= tl && tr <= r) return tree[id];
    int tm = (tl + tr) >> 1;
    if (r <= tm) return Query(l, r, 2 * id, tl, tm);
    if (tm < l) return Query(l, r, 2 * id + 1, tm + 1, tr);
    return Merge(Query(l, r, 2 * id, tl, tm), Query(l, r, 2 * id + 1, tm + 1, tr));
  }
  Long Query(int l, int r) { return Query(l, r, 1, 0, _n - 1); }
};

// define NEUT and oper
int sz[N], dep[N], fat[N], pos[N], head[N];
int cur_pos;

void dfs_size(int from){
  sz[from] = 1;
  for(int to: adj[from]){
    if(to == fat[from]) continue;
    dep[to] = dep[from] + 1;
    fat[to] = from;
    dfs_size(to);
    sz[from] += sz[to];
  }
}

void hld(int from, int prog){
  pos[from] = cur_pos++;
  head[from] = prog;
  int big_ch = -1;
  for(int to: adj[from]){
    if(to == fat[from]) continue;
    if(big_ch < 0 || sz[big_ch] < sz[to]) big_ch = to;
  }
  if(big_ch >= 0) hld(big_ch, prog);
  for(int to: adj[from]){
    if(to == fat[from] || to == big_ch) continue;
    hld(to, to);
  }
}

void hld_init(){
  fat[0] = -1;
  dep[0] = 0;
  dfs_size(0);
  cur_pos = 0;
  hld(0, 0);
}

Long query(int u, int v, SegmentTree& T){
  Long re = 0;
  while(head[u] != head[v]){
    if(dep[head[u]] > dep[head[v]]) swap(u, v);
    re = max(re, T.Query(pos[head[v]], pos[v]));
    v = fat[head[v]];
  }
  if(dep[u] > dep[v]) swap(u, v);
  re = max(re, T.Query(pos[u], pos[v]));
  return re;
}

// for updating: replace Query with Update
// queries on edges: change pos[u] to pos[u + 1]

void solve(){
  int n; cin >> n;
  for(int i = 1; i < n; i++){
    int u, v; cin >> u >> v;
    u--; v--;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  vector<int> a(n, 0);
  SegmentTree T(n);
  T.Build(a);
  hld_init();
  int q; cin >> q;
  while(q--){
    char c; cin >> c;
    if(c == 'G'){
      int u, v; cin >> u >> v;
      u--; v--;
      cout << query(u, v, T) << endl;
    }
    else{
      int u, val; cin >> u >> val;
      u--;
      T.Update(pos[u], val);
    }
  }
}

int main(){
  FAST_IO;
  int tt = 1;
  // cin >> tt;
  while(tt--) solve();
  return 0;
}
