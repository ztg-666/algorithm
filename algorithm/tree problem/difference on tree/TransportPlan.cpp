#include <bits/stdc++.h>

// 运输计划，C++版，递归不用改迭代
// 有n个节点，给定n-1条边使其连接成一棵树，每条边有正数边权
// 给定很多运输计划，每个运输计划(a,b)表示从a去往b
// 每个运输计划的代价就是沿途边权和，运输计划之间完全互不干扰
// 你只能选择一条边，将其边权变成0
// 你的目的是让所有运输计划代价的最大值尽量小
// 返回所有运输计划代价的最大值最小能是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2680


using namespace std;

const int MAXN = 300001;  // 最大节点数
const int MAXM = 300001;  // 最大查询数

// 全局变量定义
int n, m;                 // n个节点，m个运输计划

// 树的存储结构（邻接表）
int num[MAXN];           // 每个节点的贡献值（用于统计经过该节点的超限路径数）
int headEdge[MAXN];      // 邻接表头指针
int edgeNext[MAXN << 1]; // 边的下一条边索引
int edgeTo[MAXN << 1];   // 边的终点
int edgeWeight[MAXN << 1]; // 边的权重
int tcnt;                // 边的计数器

// 查询的存储结构（用于LCA算法）
int headQuery[MAXN];     // 查询邻接表头指针
int queryNext[MAXM << 1]; // 查询的下一条查询索引
int queryTo[MAXM << 1];   // 查询的终点
int queryIndex[MAXM << 1]; // 查询的索引
int qcnt;                // 查询计数器

// Tarjan离线LCA算法相关
bool visited[MAXN];      // 访问标记数组
int unionfind[MAXN];     // 并查集数组
int quesu[MAXM];         // 查询起点
int quesv[MAXM];         // 查询终点
int dist[MAXN];          // 从根节点到各节点的距离
int lca[MAXM];           // 各查询的最近公共祖先
int cost[MAXM];          // 各运输计划的代价
int maxCost;             // 所有运输计划中的最大代价

// 二分查找相关
int atLeast;             // 至少需要减少的权重
int beyond;              // 超过限制的路径数量

// 初始化函数
void build() {
    tcnt = 1;            // 边计数器从1开始
    qcnt = 1;            // 查询计数器从1开始
    for(int i = 1; i <= n; i++) {
        headEdge[i] = 0;     // 初始化邻接表
        headQuery[i] = 0;    // 初始化查询表
        visited[i] = false;  // 初始化访问标记
        unionfind[i] = i;    // 初始化并查集
    }
    maxCost = 0;         // 初始化最大代价
}

// 添加无向边
void addEdge(int u, int v, int w) {
    edgeNext[tcnt] = headEdge[u];  // 新边指向原头部
    edgeTo[tcnt] = v;              // 设置终点
    edgeWeight[tcnt] = w;          // 设置权重
    headEdge[u] = tcnt++;          // 更新头部为新边
}

// 添加LCA查询
void addQuery(int u, int v, int i) {
    queryNext[qcnt] = headQuery[u]; // 新查询指向原头部
    queryTo[qcnt] = v;              // 设置查询终点
    queryIndex[qcnt] = i;           // 设置查询索引
    headQuery[u] = qcnt++;          // 更新头部为新查询
}

// 并查集查找函数（路径压缩）
int find(int i) {
    if(i != unionfind[i]) {
        unionfind[i] = find(unionfind[i]); // 路径压缩
    }
    return unionfind[i];
}

// Tarjan离线LCA算法
void tarjan(int u, int f, int w) {
    visited[u] = true;           // 标记当前节点已访问
    dist[u] = dist[f] + w;       // 计算从根到当前节点的距离
    
    // 遍历所有子节点
    for(int e = headEdge[u]; e != 0; e = edgeNext[e]) {
        int v = edgeTo[e];
        if(v != f) {  // 避免回到父节点
            tarjan(v, u, edgeWeight[e]); // 递归处理子节点
        }
    }
    
    // 处理与当前节点相关的查询
    for(int e = headQuery[u]; e != 0; e = queryNext[e]) {
        int v = queryTo[e];
        if(visited[v]) {  // 如果查询的另一端已访问
            int i = queryIndex[e];
            lca[i] = find(v);  // 找到LCA
            // 计算路径长度：dist[u] + dist[v] - 2*dist[lca]
            cost[i] = dist[u] + dist[v] - 2 * dist[lca[i]];
            maxCost = max(maxCost, cost[i]); // 更新最大代价
        }
    }
    unionfind[u] = f;  // 将当前节点并入父节点的集合
}

// 深度优先搜索，检查是否存在一条边能使所有超限路径代价降到limit以下
bool dfs(int u, int f, int w) {
    // 先递归处理所有子节点
    for(int e = headEdge[u]; e != 0; e = edgeNext[e]) {
        int v = edgeTo[e];
        if(v != f) {
            if(dfs(v, u, edgeWeight[e])) {
                return true;  // 如果子树中找到了答案，直接返回
            }
        }
    }
    
    // 统计子树中经过的超限路径数
    for(int e = headEdge[u]; e != 0; e = edgeNext[e]) {
        int v = edgeTo[e];
        if(v != f) {
            num[u] += num[v];  // 累加子节点的贡献
        }
    }
    
    // 检查当前边是否能解决所有超限路径问题
    // num[u] == beyond 表示所有超限路径都经过这条边
    // w >= atLeast 表示这条边的权重足够大，删除后能达到目标
    return (num[u] == beyond && w >= atLeast);
}

// 检查能否在删除一条边后，使最大代价不超过limit
bool f(int limit) {
    atLeast = maxCost - limit;  // 计算需要减少的最小权重
    
    // 初始化每个节点的贡献值
    for(int i = 1; i <= n; i++) {
        num[i] = 0;
    }
    beyond = 0;  // 超限路径数量
    
    // 统计超过limit的路径，使用差分数组思想
    for(int i = 1; i <= m; i++) {
        if(cost[i] > limit) {
            num[quesu[i]]++;    // 起点+1
            num[quesv[i]]++;    // 终点+1
            num[lca[i]] -= 2;   // LCA-2（因为路径不经过LCA的父节点）
            beyond++;           // 超限路径数+1
        }
    }
    
    if(beyond == 0) return true;  // 如果没有超限路径，返回true
    
    return dfs(1, 0, 0);  // 从根节点开始搜索
}

// 主计算函数
int compute() {
    tarjan(1, 0, 0);  // 计算所有查询的LCA和路径长度
    
    // 二分查找最优答案
    int l = 0;
    int r = maxCost;
    int ans = 0;
    
    while(l <= r) {
        int mid = (l + r) / 2;
        if(f(mid)) {          // 如果mid是可行的
            ans = mid;        // 更新答案
            r = mid - 1;      // 在左半部分继续搜索更小的值
        } else {
            l = mid + 1;      // 在右半部分搜索
        }
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    build();  // 初始化
    cin >> m;
    
    // 读入树的边
    for(int i = 1; i < n; i++){
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);  // 添加无向边
        addEdge(v, u, w);
    }
    
    // 读入运输计划
    for(int i = 1; i <= m; i++){
        int u, v;
        cin >> u >> v;
        quesu[i] = u;
        quesv[i] = v;
        addQuery(u, v, i);  // 添加查询
        addQuery(v, u, i);  // 查询是无向的
    }
    
    cout << compute() << "\n";
    return 0;
}
