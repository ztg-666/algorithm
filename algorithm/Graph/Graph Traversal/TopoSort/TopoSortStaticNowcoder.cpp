#include <iostream>
#include <cstring>
using namespace std;

const int MAXN = 200001; // 最大节点数
const int MAXM = 200001; // 最大边数

// 链式前向星建图
int head[MAXN];  // head[i] 表示以节点 i 为起点的第一条边的编号
int next_[MAXM]; // next_[i] 表示编号为 i 的边的下一条边的编号
int to[MAXM];    // to[i] 表示编号为 i 的边指向的目标节点
int cnt;         // 边的计数器，用于分配边的唯一编号

// 拓扑排序相关
int queue[MAXN]; // 数组模拟队列，用于存储入度为 0 的节点
int l, r;        // 队列头尾指针，l 表示队头，r 表示队尾
int indegree[MAXN]; // 入度表，indegree[i] 表示节点 i 的入度
int ans[MAXN];     // 存储拓扑排序的结果
int n, m;         // 节点数 n 和边数 m

// 初始化图结构
void build(int n) {
    cnt = 1; // 边编号从 1 开始（避免与默认值 0 冲突）
    memset(head, 0, (n + 1) * sizeof(int)); // 初始化 head 数组为 0，表示每个节点目前没有边
    memset(indegree, 0, (n + 1) * sizeof(int)); // 初始化入度表为 0
}

// 添加一条边 f -> t
void addEdge(int f, int t) {
    next_[cnt] = head[f]; // 新边的下一条边指向原来的第一条边（链式前向星的核心逻辑）
    to[cnt] = t;          // 记录当前边的目标节点
    head[f] = cnt++;      // 更新 head[f] 为当前边的编号，并递增计数器
}

// 拓扑排序函数
bool topoSort() {
    l = r = 0; // 初始化队列头尾指针

    // 将所有入度为 0 的节点加入队列
    for (int i = 1; i <= n; ++i) {
        if (indegree[i] == 0) {
            queue[r++] = i; // 将节点 i 加入队列
        }
    }

    int fill = 0; // 记录已处理的节点数
    while (l < r) { // 当队列不为空时
        int cur = queue[l++]; // 取出队首节点
        ans[fill++] = cur;    // 将当前节点加入结果数组

        // 遍历当前节点的所有出边（链式前向星遍历方式）
        for (int ei = head[cur]; ei != 0; ei = next_[ei]) {
            int t = to[ei]; // 获取目标节点
            if (--indegree[t] == 0) { // 如果目标节点的入度变为 0，则将其加入队列
                queue[r++] = t;
            }
        }
    }

    return fill == n; // 如果处理的节点数等于总节点数，说明存在拓扑排序
}
// 拓扑排序模版（牛客）
// 链式前向星建图（静态方式）
// 测试链接 : https://www.nowcoder.com/practice/88f7e156ca7d43a1a535f619cd3f495c
int main() {
    ios::sync_with_stdio(false); // 关闭 cin 和 scanf 的同步，加速输入输出
    cin.tie(nullptr);            // 解绑 cin 和 cout，进一步加速
    cout.tie(nullptr);

    // 多组测试用例处理
    while (cin >> n >> m) { // 输入节点数 n 和边数 m
        build(n); // 初始化图结构

        // 构建图并维护入度表
        for (int i = 0, f, t; i < m; ++i) {
            cin >> f >> t; // 输入一条边 f -> t
            addEdge(f, t); // 添加边到图中
            indegree[t]++; // 增加目标节点 t 的入度
        }

        // 执行拓扑排序
        if (!topoSort()) {
            cout << -1 << "\n"; // 如果不存在拓扑排序，输出 -1
        } else {
            // 输出拓扑排序的结果
            for (int i = 0; i < n - 1; ++i) {
                cout << ans[i] << " ";
            }
            cout << ans[n - 1] << "\n";
        }
    }

    return 0;
}
