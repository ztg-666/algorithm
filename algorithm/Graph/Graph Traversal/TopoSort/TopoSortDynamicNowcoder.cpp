#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int MAXN = 200001; // 最大节点数

int queue[MAXN]; // 用于拓扑排序的队列
int l, r;        // 队列的头指针和尾指针

int indegree[MAXN]; // 入度表，记录每个节点的入度
int ans[MAXN];      // 存储拓扑排序的结果

int n, m; // n表示节点数，m表示边数

// 拓扑排序函数
bool topoSort(const vector<vector<int>>& graph) {
    l = r = 0; // 初始化队列头尾指针

    // 初始化入度为0的节点，将其加入队列
    for (int i = 1; i <= n; ++i) {
        if (indegree[i] == 0) {
            queue[r++] = i;
        }
    }

    int fill = 0; // 记录已处理的节点数
    while (l < r) {
        int cur = queue[l++]; // 取出队首节点
        ans[fill++] = cur;    // 将当前节点加入结果数组

        // 处理当前节点的所有后继节点
        for (int next : graph[cur]) {
            if (--indegree[next] == 0) {
                queue[r++] = next; // 如果后继节点的入度变为0，加入队列
            }
        }
    }
    return fill == n; // 如果处理的节点数等于n，说明存在拓扑排序
}

// 拓扑排序模版（牛客）
// 邻接表建图（动态方式）
// 测试链接 : https://www.nowcoder.com/practice/88f7e156ca7d43a1a535f619cd3f495c
int main() {
    ios::sync_with_stdio(false); // 加速输入输出
    cin.tie(nullptr);
    cout.tie(nullptr);

    // 处理多组输入
    while (cin >> n >> m) {
        // 初始化邻接表
        vector<vector<int>> graph(n + 1);
        memset(indegree, 0, sizeof(indegree)); // 初始化入度表为0

        // 构建图
        for (int i = 0, from, to; i < m; ++i) {
            cin >> from >> to; // 读取边的起点和终点
            graph[from].push_back(to); // 添加边到邻接表
            indegree[to]++; // 增加终点的入度
        }

        // 执行拓扑排序
        if (!topoSort(graph)) {
            cout << -1 << endl; // 如果不存在拓扑排序，输出-1
        } else {
            // 输出拓扑排序的结果
            for (int i = 0; i < n - 1; ++i) {
                cout << ans[i] << " ";
            }
            cout << ans[n - 1] << endl;
        }
    }

    return 0;
}
