#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
using namespace std;

const int MAXN = 100001; // 最大节点数
const int MAXM = 100001; // 最大边数

// 链式前向星建图
int head[MAXN];  // head[i] 表示以节点 i 为起点的第一条边的编号
int next_[MAXM]; // next_[i] 表示编号为 i 的边的下一条边的编号
int to[MAXM];    // to[i] 表示编号为 i 的边指向的目标节点
int cnt;         // 边的计数器，用于分配边的唯一编号

// 拓扑排序相关
priority_queue<int, vector<int>, greater<int>> heap; // 小根堆，用于存储入度为 0 的节点
int indegree[MAXN]; // 入度表，indegree[i] 表示节点 i 的入度
vector<int> ans;    // 存储拓扑排序的结果
int n, m;           // 节点数 n 和边数 m

// 初始化图结构
void build(int n) {
    cnt = 1; // 边编号从 1 开始（避免与默认值 0 冲突）
    ans.clear(); // 清空结果数组
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
void topoSort() {
    // 将所有入度为 0 的节点加入小根堆
    for (int i = 1; i <= n; ++i) {
        if (indegree[i] == 0) {
            heap.push(i); // 将节点 i 加入小根堆
        }
    }

    // 使用小根堆进行拓扑排序
    while (!heap.empty()) { // 当堆不为空时
        int cur = heap.top(); // 取出堆顶节点（当前最小节点）
        heap.pop();
        ans.push_back(cur);   // 将当前节点加入结果数组

        // 遍历当前节点的所有出边（链式前向星遍历方式）
        for (int ei = head[cur]; ei != 0; ei = next_[ei]) {
            int t = to[ei]; // 获取目标节点
            if (--indegree[t] == 0) { // 如果目标节点的入度变为 0，则将其加入堆
                heap.push(t);
            }
        }
    }
}
// 字典序最小的拓扑排序
// 要求返回所有正确的拓扑排序中 字典序最小 的结果
// 建图请使用链式前向星方式，因为比赛平台用其他建图方式会卡空间
// 测试链接 : https://www.luogu.com.cn/problem/U107394
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

        topoSort(); // 执行拓扑排序

        // 输出结果
        if (ans.size() != n) { // 如果结果数组长度不等于节点数，说明不存在拓扑排序
            cout << "-1\n";
        } else {
            // 输出字典序最小的拓扑排序结果
            for (int i = 0; i < n - 1; ++i) {
                cout << ans[i] << " ";
            }
            cout << ans[n - 1] << "\n";
        }
    }

    return 0;
}
