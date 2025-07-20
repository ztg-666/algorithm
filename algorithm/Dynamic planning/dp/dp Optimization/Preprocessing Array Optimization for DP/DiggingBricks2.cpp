#include <iostream>
#include <algorithm>
using namespace std;

const int MAXN = 51;    // 最大层数
const int MAXM = 1301;  // 最大挖掘砖块数量

int grid[MAXN][MAXN];    // 存储砖块的价值，grid[i][j]表示第i层第j个砖块的价值
int dp[MAXN][MAXM];      // dp[j][k]表示挖掘前j个砖块，总共挖掘k个砖块时的最大收益
int maxVal[MAXN][MAXM];  // maxVal[row][col]表示从第row行到最后一行，挖掘col个砖块的最大收益
int n, m;                // n为层数，m为最多可挖掘的砖块数

/**
 * 预处理函数：计算从第rowLimit行及以上的所有行中，挖掘不同数量砖块的最大收益
 * 这是一个后缀最大值的预处理，用于优化枚举过程
 * @param rowLimit 当前处理到的行数限制
 */
void prepare(int rowLimit) {
    // 遍历所有可能的挖掘砖块数量
    for (int col = 0; col <= m; col++) {
        int suf = 0;  // 后缀最大值
        // 从第rowLimit行开始，向上遍历到第0行
        for (int row = rowLimit; row >= 0; row--) {
            suf = max(suf, dp[row][col]);  // 更新后缀最大值
            maxVal[row][col] = suf;        // 存储从第row行到第rowLimit行的最大收益
        }
    }
}

/**
 * 主要计算函数：使用动态规划计算最大收益
 * 优化枚举 + 空间压缩
 * 时间复杂度O(n^2 * m)
 *
 * 状态转移思路：
 * 对于第i层，考虑挖掘前j个砖块（从左到右连续），需要满足约束条件：
 * 如果要挖第i层的第j个砖块，必须先挖掉第i-1层的第j和第j+1个砖块
 *
 * @return 最大收益
 */
int compute() {
    int ans = 0;  // 记录全局最大收益

    // 逐层处理，从第1层到第n层
    for (int i = 1; i <= n; i++) {
        prepare(i - 1);  // 预处理前i-1层的最大收益信息

        int cur = 0;  // 当前层累积的砖块价值

        // 枚举当前层挖掘的砖块数量（从左到右连续挖掘0到i个砖块）
        for (int j = 0; j <= i; j++) {
            cur += grid[i][j];  // 累加当前砖块的价值

            // 枚举总的挖掘砖块数量
            // (j+1)*j/2 是挖掘前j个砖块所需的最少砖块数（考虑约束条件）
            for (int k = (j + 1) * j / 2; k <= m; k++) {
                // 状态转移：当前收益 = 上一层的最大收益 + 当前层的收益
                // maxVal[max(0, j-1)][k-j] 表示上一层在满足约束条件下的最大收益
                dp[j][k] = maxVal[max(0, j - 1)][k - j] + cur;
                ans = max(ans, dp[j][k]);  // 更新全局最大收益
            }
        }
    }
    return ans;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入层数n和最大挖掘数m
    cin >> n >> m;

    // 读入砖块数据
    // 按列优先的顺序读入，保证数据结构与问题描述一致
    for (int j = 1; j <= n; j++) {        // 遍历每一列
        for (int i = n; i >= j; i--) {    // 从底层向上层读入
            cin >> grid[i][j];
        }
    }

    // 输出最大收益
    cout << compute() << endl;

    return 0;
}
