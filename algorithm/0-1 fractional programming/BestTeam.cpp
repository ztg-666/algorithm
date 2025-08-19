#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdio>
#include <cstring>
#include <algorithm>

// 最佳团体
// 给定一棵树，节点编号0~n，0号节点是整棵树的头
// 编号1~n的节点，每个节点都有招募花费和战斗值，0号节点这两个值都是0
// 给定每条边(a,b)，表示节点a的父节点是b，有些节点的父节点是0节点
// 当招募了某个节点，那么该节点及其上方的所有祖先节点都需要招募
// 除了0号节点之外，一共可以招募k个人，希望让
// 战斗值之和 / 招募花费之和，这个比值尽量大，答案只需保留三位小数，更大的精度舍弃
// 1 <= k <= n <= 2500
// 0 <= 招募花费、战斗值 <= 10^4
// 测试链接 : https://www.luogu.com.cn/problem/P4322

using namespace std;

const int MAXN = 3001;      // 最大节点数
const int LIMIT = 10000;    // 最大招募花费
const double NA = -1e9;     // 无效解的标记
const double sml = 1e-6;    // 精度

// 链式前向星
int head[MAXN], nxt[MAXN], to[MAXN];
int edgeCnt; // 边的计数器

// 招募花费，下标为节点原始编号
int cost[MAXN];

// 战斗值，下标为节点原始编号
int strength[MAXN];

// dfn[a] = b，表示原始a号节点的dfn编号为b
int dfn[MAXN];

// dfn序计数
int dfnCnt;

// (战斗值 - x * 招募花费)的结余，下标为节点dfn编号
double value[MAXN];

// 子树大小，下标为节点dfn编号
int siz[MAXN];

// 树型dp
double dp[MAXN][MAXN];

int k, n;  // k表示需要招募的人数，n表示节点数量

// 初始化函数，设置初始值
void prepare() {
    edgeCnt = 1;  // 初始化边计数
    dfnCnt = 0;   // 初始化dfn序计数
    memset(head, 0, sizeof(head));  // 初始化链式前向星头节点数组
}

// 添加边，建立树的结构
void addEdge(int u, int v) {
    nxt[edgeCnt] = head[u];  // 将u的当前边指向上一个边
    to[edgeCnt] = v;         // 设置边的终点
    head[u] = edgeCnt++;     // 更新u的头节点
}

// 深度优先遍历，计算节点dfn编号并返回子树的节点数
int dfs(int u) {
    int i = ++dfnCnt;        // 为当前节点分配dfn编号
    dfn[u] = i;              // 设置节点的dfn编号
    siz[i] = 1;              // 当前节点的子树大小初始为1（包括自身）
    for (int e = head[u]; e != 0; e = nxt[e]) {  // 遍历所有子节点
        int v = to[e];
        siz[i] += dfs(v);  // 递归计算子树大小
    }
    return siz[i];  // 返回当前子树的大小
}

// 根据x的值，计算节点的结余值，在树上选k+1个点
// 一定要形成有效结构，返回最大的结余和
bool check(double x) {
    // 更新每个节点的结余值：战斗值 - x * 招募花费
    for (int i = 0; i <= n; i++) {
        value[dfn[i]] = (double) strength[i] - x * cost[i];
    }

    // 初始化无效解
    for (int j = 1; j <= k; j++) {
        dp[dfnCnt + 1][j] = NA;
    }

    // 树形DP计算最优解
    // 从dfnCnt开始，逆序遍历所有节点，这样可以确保每个节点的子树已经计算完毕，符合树的递归结构
    for (int i = dfnCnt; i >= 2; i--) {  // 从后向前遍历dfn序，dfnCnt是DFS遍历中节点的数量
        // 遍历所有可能的招募数量j，从1到k
        for (int j = 1; j <= k; j++) {  // j表示从当前节点及其子树中招募的节点数
            // 递推关系：我们可以选择当前节点i，或者不选择当前节点i
            // dp[i][j]：表示从节点i开始，选j个节点时的最大结余值

            // 1. 如果我们不选择当前节点i，而是选择它的所有子节点，则该值为dp[i + siz[i]][j]（即跳过当前节点，递归到子树的下一层）。
            // 2. 如果我们选择当前节点i，则当前节点i的战斗值和招募花费通过value[i]给出，再加上选择剩余j-1个节点的最大结余，递归到子树的下一层（即dp[i + 1][j - 1]）。

            // 选择两者中的最大值，保存在dp[i][j]中。
            dp[i][j] = max(dp[i + siz[i]][j], value[i] + dp[i + 1][j - 1]);
        }
    }

    // 判断是否可以选出k个节点使得结余大于等于0
    return dp[2][k] >= 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入节点和边的数量
    cin >> k >> n;
    prepare();

    // 读取所有节点的信息
    for (int i = 1; i <= n; i++) {
        cin >> cost[i] >> strength[i];  // 输入节点的招募花费和战斗值
        int p;
        cin >> p;  // 输入父节点编号
        addEdge(p, i);  // 将该节点加入到父节点的邻接列表中
    }

    // 进行DFS，计算dfn序和子树大小
    dfs(0);

    double l = 0, r = LIMIT, x, ans = 0;

    // 二分查找最优的比值
    while (r - l >= sml) {
        x = (l + r) / 2;  // 计算当前的中值
        if (check(x)) {    // 如果当前比值下存在有效解
            ans = x;       // 更新答案
            l = x + sml;   // 继续向右查找
        } else {
            r = x - sml;   // 否则向左查找
        }
    }

    // 输出最小平均值，保留3位小数
    printf("%.3f\n", ans);

    return 0;
}
