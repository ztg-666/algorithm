#include <vector>
#include <algorithm>
#include <cstring>

// 树节点的第K个祖先
// 树上有n个节点，编号0 ~ n-1，树的结构用parent数组代表
// 其中parent[i]是节点i的父节点，树的根节点是编号为0
// 树节点i的第k个祖先节点，是从节点i开始往上跳k步所来到的节点
// 实现TreeAncestor类
// TreeAncestor(int n, int[] parent) : 初始化
// getKthAncestor(int i, int k) : 返回节点i的第k个祖先节点，不存在返回-1
// 测试链接 : https://leetcode.cn/problems/kth-ancestor-of-a-tree-node/

using namespace std;
const int MAXN = 50001;  // 最大节点数
const int LIMIT = 16;   // 最大跳跃层数，2^16 = 65536 > 50000

class TreeAncestor {
private:

    int power;  // 实际需要的最大跳跃层数

    // ============== 链式前向星建图相关 ==============
    int head[MAXN];  // head[i]：节点i的第一条边的编号
    int next[MAXN];  // next[e]：编号为e的边的下一条边编号
    int to[MAXN];    // to[e]：编号为e的边指向的节点
    int cnt;         // 边的计数器

    // ============== 倍增法相关 ==============
    int deep[MAXN];  // deep[i]：节点i的深度（层数）

    // stjump[i][p]：节点i往上跳2^p步后到达的节点编号
    // 这是倍增法的核心数据结构
    int stjump[MAXN][LIMIT];

    // 根据节点个数n，计算出需要多少层跳跃表
    // 返回满足2^ans <= n/2的最大ans值
    int log2(int n) {
        int ans = 0;
        while ((1 << ans) <= (n >> 1)) {
            ans++;
        }
        return ans;
    }

    // 添加有向边：u -> v
    void addEdge(int u, int v) {
        next[cnt] = head[u];  // 新边的下一条边是u的当前第一条边
        to[cnt] = v;          // 新边指向v
        head[u] = cnt++;      // 更新u的第一条边为新边
    }

    // DFS预处理：计算每个节点的深度和跳跃表
    // i：当前节点，f：父节点
    void dfs(int i, int f) {
        // 设置当前节点的深度
        if (i == 0) {
            deep[i] = 1;  // 根节点深度为1
        } else {
            deep[i] = deep[f] + 1;  // 子节点深度 = 父节点深度 + 1
        }

        // 初始化跳跃表：跳2^0 = 1步，到达父节点
        stjump[i][0] = f;

        // 填充跳跃表：stjump[i][p] = stjump[stjump[i][p-1]][p-1]
        // 含义：跳2^p步 = 先跳2^(p-1)步，再跳2^(p-1)步
        for (int p = 1; p <= power; p++) {
            stjump[i][p] = stjump[stjump[i][p - 1]][p - 1];
        }

        // 遍历当前节点的所有子节点
        for (int e = head[i]; e != 0; e = next[e]) {
            dfs(to[e], i);  // 递归处理子节点
        }
    }

public:
    // 构造函数：初始化树结构
    TreeAncestor(int n, vector<int>& parent) {
        power = log2(n);  // 计算需要的跳跃层数
        cnt = 1;          // 边编号从1开始（0表示空）

        // 初始化所有数组
        memset(head, 0, sizeof(head));
        memset(next, 0, sizeof(next));
        memset(to, 0, sizeof(to));
        memset(deep, 0, sizeof(deep));
        memset(stjump, 0, sizeof(stjump));

        // 根据parent数组建图
        // parent[i]表示节点i的父节点
        for (int i = 1; i < parent.size(); i++) {
            addEdge(parent[i], i);  // 添加边：parent[i] -> i
        }

        // 从根节点开始DFS预处理
        dfs(0, 0);  // 根节点的父节点设为自己
    }

    // 查找节点i的第k个祖先
    int getKthAncestor(int i, int k) {
        // 如果要往上跳的步数 >= 节点深度，说明会跳出树外
        if (deep[i] <= k) {
            return -1;
        }

        // s是目标层数（想要到达的深度）
        int s = deep[i] - k;

        // 从高位到低位，贪心地跳跃
        // 如果跳2^p步后还不会跳过目标层，就跳
        for (int p = power; p >= 0; p--) {
            if (deep[stjump[i][p]] >= s) {
                i = stjump[i][p];  // 执行跳跃
            }
        }

        return i;  // 返回最终到达的节点
    }
};
