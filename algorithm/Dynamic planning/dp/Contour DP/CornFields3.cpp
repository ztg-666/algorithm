#include <iostream>
#include <vector>
#include <cstring>

// 种草的方法数(轮廓线dp+空间压缩)
// 给定一个n*m的二维网格grid
// 网格里只有0、1两种值，0表示该田地不能种草，1表示该田地可以种草
// 种草的时候，任何两个种了草的田地不能相邻，相邻包括上、下、左、右四个方向
// 你可以随意决定种多少草，只要不破坏上面的规则即可
// 返回种草的方法数，答案对100000000取模
// 1 <= n, m <= 12
// 测试链接 : https://www.luogu.com.cn/problem/P1879

using namespace std;

const int MAXN = 12;       // 最大行数
const int MAXM = 12;       // 最大列数
const int MOD = 100000000; // 取模数

int grid[MAXN][MAXM];           // 输入的网格，0表示不能种草，1表示可以种草
int dp[MAXM + 1][1 << MAXM];    // dp[j][s]表示从第j列开始，状态为s时的方案数
int prepare[1 << MAXM];         // 上一行处理完后的状态数组
int n, m, maxs;                 // n行数，m列数，maxs=2^m总状态数

// 获取状态s中第j位的值（0或1）
int get(int s, int j) {
    return (s >> j) & 1;
}

// 设置状态s中第j位的值为v（0或1）
int set(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

int compute() {
    // 初始化prepare数组，表示最后一行下面的虚拟行，所有状态方案数都是1
    fill(prepare, prepare + maxs, 1);

    // 从最后一行开始，逐行向上处理
    for (int i = n - 1; i >= 0; i--) {
        // 边界条件：j == m时，直接使用上一行的结果
        for (int s = 0; s < maxs; s++) {
            dp[m][s] = prepare[s];
        }

        // 从右到左处理每一列
        for (int j = m - 1; j >= 0; j--) {
            // 遍历所有可能的状态
            for (int s = 0; s < maxs; s++) {
                // 默认当前位置不种草的方案数
                int ans = dp[j + 1][set(s, j, 0)];

                // 判断当前位置是否可以种草
                // 条件1：grid[i][j] == 1，当前位置允许种草
                // 条件2：j == 0 || get(s, j - 1) == 0，左边没有种草或者是第一列
                // 条件3：get(s, j) == 0，下面没有种草
                if (grid[i][j] == 1 && (j == 0 || get(s, j - 1) == 0) && get(s, j) == 0) {
                    // 加上当前位置种草的方案数
                    ans = (ans + dp[j + 1][set(s, j, 1)]) % MOD;
                }

                dp[j][s] = ans;
            }
        }

        // 当前行处理完毕，更新prepare数组为当前行的结果
        // 这里的状态s表示当前行的种草情况，prepare[s]表示从当前行开始的方案数
        for (int s = 0; s < maxs; s++) {
            prepare[s] = dp[0][s];
        }
    }

    // 返回从第0行第0列开始，初始状态为0（上面没有种草）的方案数
    return dp[0][0];
}

int main() {
    // 读取网格大小
    cin >> n >> m;
    maxs = 1 << m;  // 计算总状态数，每列有种草/不种草两种状态

    // 读取网格数据
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> grid[i][j];
        }
    }

    // 输出结果
    cout << compute() << endl;
    return 0;
}
