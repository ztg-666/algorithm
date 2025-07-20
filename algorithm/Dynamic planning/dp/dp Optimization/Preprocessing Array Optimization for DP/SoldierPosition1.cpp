#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>

// 炮兵阵地
// 给定一个n * m的二维数组grid，其中的1代表可以摆放炮兵，0代表不可以摆放
// 任何炮兵攻击范围是一个"十字型"的区域，具体是上下左右两个格子的区域
// 你的目的是在gird里摆尽量多的炮兵，但要保证任何两个炮兵之间无法互相攻击
// 返回最多能摆几个炮兵
// 1 <= n <= 100
// 1 <= m <= 10
// 0 <= grid[i][j] <= 1
// 测试链接 : https://www.luogu.com.cn/problem/P2704

using namespace std;

const int MAXN = 100;  // 最大行数
const int MAXM = 10;   // 最大列数
const int MAXS = 60;   // 最大状态数

int grid[MAXN][MAXM];    // 地图，1表示可以放炮兵，0表示不可以
int dp[MAXN][MAXS][MAXS]; // dp[i][a][b]表示第i行状态为a，第i-1行状态为b时的最大炮兵数
int sta[MAXS];           // 存储所有合法的状态（一行内炮兵不冲突的状态）
int n, m, k;            // n行数，m列数，k合法状态数

/**
 * 预处理所有合法状态
 * j: 当前处理的列
 * s: 当前状态（用位表示哪些位置放了炮兵）
 *
 * 炮兵攻击范围是十字型，上下左右各2格
 * 所以同一行内，两个炮兵之间至少要间隔2个位置
 */
void prepare(int j, int s) {
    if (j >= m) {
        // 处理完所有列，将当前状态加入合法状态数组
        sta[k++] = s;
    } else {
        // 当前位置不放炮兵
        prepare(j + 1, s);
        // 当前位置放炮兵（跳过接下来2个位置，确保不冲突）
        prepare(j + 3, s | (1 << j));
    }
}

/**
 * 计算第i行在状态s下能放置多少炮兵
 * i: 行号
 * s: 状态（二进制表示）
 */
int cnt(int i, int s) {
    int ans = 0;
    for (int j = 0; j < m; j++) {
        // 如果状态s在第j位为1（要放炮兵）且地图允许放置
        if (((s >> j) & 1) == 1 && grid[i][j] == 1) {
            ans++;
        }
    }
    return ans;
}

/**
 * 动态规划求解最大炮兵数
 * 状态定义：dp[i][a][b] = 第i行状态为a，第i-1行状态为b时的最大炮兵数
 *
 * 转移条件：
 * 1. 第i行状态a与第i-1行状态b不冲突（位与结果为0）
 * 2. 第i-1行状态b与第i-2行状态c不冲突
 * 3. 第i行状态a与第i-2行状态c不冲突（因为炮兵攻击范围是2格）
 */
int compute() {
    // 初始化dp数组为0
    memset(dp, 0, sizeof(dp));

    // 初始化第0行
    // 第0行没有前一行，所以第二维状态设为0
    for (int a = 0; a < k; a++) {
        int count = cnt(0, sta[a]);  // 第0行状态a能放置的炮兵数
        dp[0][a][0] = count;
    }

    // 状态转移：从第1行开始
    for (int i = 1; i < n; i++) {
        for (int a = 0; a < k; a++) {          // 枚举第i行的状态
            int count = cnt(i, sta[a]);         // 第i行状态a能放置的炮兵数
            for (int b = 0; b < k; b++) {       // 枚举第i-1行的状态
                // 检查第i行和第i-1行是否冲突
                if ((sta[a] & sta[b]) == 0) {
                    for (int c = 0; c < k; c++) { // 枚举第i-2行的状态
                        // 检查三行之间都不冲突
                        if ((sta[b] & sta[c]) == 0 && (sta[a] & sta[c]) == 0) {
                            // 状态转移：从dp[i-1][b][c]转移到dp[i][a][b]
                            dp[i][a][b] = max(dp[i][a][b], dp[i - 1][b][c] + count);
                        }
                    }
                }
            }
        }
    }

    // 找出最后一行的最优解
    int ans = 0;
    for (int a = 0; a < k; a++) {
        for (int b = 0; b < k; b++) {
            ans = max(ans, dp[n - 1][a][b]);
        }
    }
    return ans;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入地图大小
    cin >> n >> m;
    k = 0;  // 合法状态数初始化为0

    // 读入地图
    for (int i = 0; i < n; i++) {
        string line;
        cin >> line;
        for (int j = 0; j < m; j++) {
            // 'H'表示山地不能放炮兵，'P'表示平原可以放炮兵
            grid[i][j] = (line[j] == 'H') ? 0 : 1;
        }
    }

    // 预处理所有合法状态
    prepare(0, 0);

    // 计算并输出答案
    cout << compute() << endl;

    return 0;
}
