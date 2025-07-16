#include <iostream>
#include <vector>
#include <cstring>

// 种草的方法数(普通状压dp)
// 给定一个n*m的二维网格grid
// 网格里只有0、1两种值，0表示该田地不能种草，1表示该田地可以种草
// 种草的时候，任何两个种了草的田地不能相邻，相邻包括上、下、左、右四个方向
// 你可以随意决定种多少草，只要不破坏上面的规则即可
// 返回种草的方法数，答案对100000000取模
// 1 <= n, m <= 12
// 测试链接 : https://www.luogu.com.cn/problem/P1879

using namespace std;

const int MAXN = 12;          // 最大行数
const int MAXM = 12;          // 最大列数
const int MOD = 100000000;    // 取模数

int grid[MAXN][MAXM];         // 输入的二维网格，0表示不能种草，1表示可以种草
int dp[MAXN][1 << MAXM];      // 记忆化数组，dp[i][s]表示第i行，前一行状态为s时的方案数
int n, m, maxs;               // n行数，m列数，maxs=2^m表示所有可能的状态数

// 得到状态s中第j位的状态（0或1）
int get(int s, int j) {
    return (s >> j) & 1;      // 右移j位后取最低位
}

// 将状态s中第j位设置成v，然后返回新的状态值
int set(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
    // 如果v=0：将第j位置0，其他位不变
    // 如果v=1：将第j位置1，其他位不变
}

int f(int i, int s);          // 前向声明

// DFS函数：枚举当前行的所有可能状态
// i: 当前行号
// j: 当前列号
// s: 上一行的种草状态（用二进制表示）
// ss: 当前行的种草状态（用二进制表示）
// 返回：从当前位置开始的所有可能方案数
int dfs(int i, int j, int s, int ss) {
    // 如果已经处理完当前行的所有列，转到下一行
    if (j == m) {
        return f(i + 1, ss);
    }

    // 选择1：当前位置不种草
    int ans = dfs(i, j + 1, s, ss);

    // 选择2：当前位置种草（需要满足条件）
    if (grid[i][j] == 1 &&                    // 当前位置可以种草
        (j == 0 || get(ss, j - 1) == 0) &&    // 左边没有种草（或者是第一列）
        get(s, j) == 0) {                     // 上面没有种草

        // 在当前位置种草，递归处理下一列
        ans = (ans + dfs(i, j + 1, s, set(ss, j, 1))) % MOD;
    }

    return ans;
}

// 动态规划主函数
// i: 当前行号
// s: 上一行的种草状态
// 返回：从第i行开始到最后一行的所有可能方案数
int f(int i, int s) {
    // 边界条件：如果已经处理完所有行，返回1种方案
    if (i == n) {
        return 1;
    }

    // 记忆化：如果已经计算过，直接返回结果
    if (dp[i][s] != -1) {
        return dp[i][s];
    }

    // 通过DFS枚举当前行的所有可能状态
    int ans = dfs(i, 0, s, 0);

    // 保存结果到记忆化数组
    dp[i][s] = ans;
    return ans;
}

// 计算总方案数
int compute() {
    // 初始化记忆化数组
    for (int i = 0; i < n; i++) {
        for (int s = 0; s < maxs; s++) {
            dp[i][s] = -1;  // -1表示未计算过
        }
    }

    // 从第0行开始，上一行状态为0（表示没有上一行）
    return f(0, 0);
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入网格大小
    cin >> n >> m;
    maxs = 1 << m;  // 计算所有可能的状态数（2^m）

    // 读入网格数据
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    // 计算并输出结果
    cout << compute() << endl;

    return 0;
}
