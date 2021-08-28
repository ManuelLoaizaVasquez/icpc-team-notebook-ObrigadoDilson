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

int query(int u, int v, SegmentTree& T){
  int re = NEUT;
  while(head[u] != head[v]){
    if(dep[head[u]] > dep[head[v]]) swap(u, v);
    re = oper(re, T.Query(pos[head[v]], pos[v]));
    v = fat[head[v]];
  }
  if(dep[u] > dep[v]) swap(u, v);
  re = oper(re, T.Query(pos[u], pos[v]));
  return re;
}

// for updateing: replace Query with Update
// queries on edges: change pos[u] to pos[u + 1]
