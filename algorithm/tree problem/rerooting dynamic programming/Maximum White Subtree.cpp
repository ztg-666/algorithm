#include<bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 2e5+5;
const int MAXE = MAXN*2;

int n;
// 链式前向星存图
int head[MAXN], to[MAXE], nxt[MAXE], cnt=1;

// 每个点的状态
int val[MAXN];   // 点权：白色=+1, 黑色=-1
int downv[MAXN]; // 以该点为根的“子树最大值”（必须包含该点）
int up[MAXN];    // 从父亲方向传下来的最大贡献
int ans[MAXN];   // 每个点的最终答案

// 加一条无向边 (u,v)
void add_edge(int u,int v){
    to[cnt]=v;
    nxt[cnt]=head[u];
    head[u]=cnt++;
}

// 第一次 DFS：计算 downv[u]
// downv[u] = val[u] + Σ(max(0, downv[child]))
// 表示在以 u 为根的子树中，包含 u 时能取到的最大值
void dfs1(int u,int f){
    downv[u] = val[u];
    for(int i=head[u]; i; i=nxt[i]){
        int v=to[i];
        if(v==f) continue; // 避免走回父亲
        dfs1(v,u);
        downv[u] += max(0, downv[v]);
    }
}

// 第二次 DFS：利用 reroot 技巧，把父亲方向的贡献传递给孩子
void dfs2(int u,int f){
    // 当前点的答案 = 子树贡献 + 父亲方向贡献
    ans[u] = downv[u] + max(0, up[u]);

    // 先算总贡献 S（把当前点和父亲、所有孩子都考虑进来）
    int S = downv[u] + max(0, up[u]);

    // 对每个孩子，把“除了它自己以外的贡献”当作 up[v]
    for(int i=head[u]; i; i=nxt[i]){
        int v=to[i];
        if(v==f) continue;
        // 把总贡献 S 去掉该子树的贡献，就是传给孩子的“父方向贡献”
        up[v] = max(0, S - max(0, downv[v]));
        dfs2(v,u);
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    // 读入点颜色并转成权值
    for(int i=1;i<=n;i++){
        int c; cin >> c;
        val[i] = (c==1?1:-1);
    }
    // 读入边
    for(int i=1;i<n;i++){
        int u,v; cin >> u >> v;
        add_edge(u,v);
        add_edge(v,u);
    }

    // 两次 DFS
    dfs1(1,0); // 从根开始计算子树贡献
    dfs2(1,0); // 再传递父亲方向的贡献

    // 输出结果
    for(int i=1;i<=n;i++){
        cout << ans[i] << (i==n?'\n':' ');
    }
}
