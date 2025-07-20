#include <iostream>
#include <vector>
using namespace std;

// 模数，用于取模运算
const int MOD = 998244353;
// 最大数组长度
const int MAXN = 10000;
// 数组中最大可能的值
const int m = 200;

// 数组长度
int n;
// 输入数组，下标从1开始
vector<int> arr(MAXN + 1);
// memo[v][s]：当前状态下的方案数
// v表示当前位置的值，s表示状态(0或1)
vector<vector<int>> memo(m + 1, vector<int>(2));
// dp数组用于计算下一轮的状态
vector<vector<int>> dp(m + 1, vector<int>(2));
// suf0[v]：从v到m所有memo[i][0]的前缀和，用于优化枚举
vector<int> suf0(m + 1);
// pre1[v]：从1到v所有memo[i][1]的前缀和，用于优化枚举
vector<int> pre1(m + 1);

/**
 * 预处理函数，计算前缀和和后缀和来优化枚举过程
 * suf0[v]：memo[v][0] + memo[v+1][0] + ... + memo[m][0]
 * pre1[v]：memo[1][1] + memo[2][1] + ... + memo[v][1]
 */
void prepare() {
    // 计算后缀和suf0
    suf0[m] = memo[m][0];
    for (int v = m - 1; v >= 0; v--) {
        suf0[v] = (suf0[v + 1] + memo[v][0]) % MOD;
    }

    // 计算前缀和pre1
    pre1[1] = memo[1][1];
    for (int v = 2; v <= m; v++) {
        pre1[v] = (pre1[v - 1] + memo[v][1]) % MOD;
    }
}

/**
 * 主要计算函数
 * 使用动态规划求解还原数组的方法数
 * 状态定义：memo[v][s]表示在某个位置，值为v，状态为s时的方案数
 * s=0：当前值小于等于相邻值的最大值
 * s=1：当前值大于相邻值的最大值
 */
int compute() {
    // 初始化memo数组
    // 初始状态：所有值的s=0状态方案数为0，s=1状态方案数为1
    for (int v = 0; v <= m; v++) {
        memo[v][0] = 0;  // 初始状态下，没有值能满足小于等于相邻最大值
        memo[v][1] = 1;  // 初始状态下，任何值都可能是最大值
    }

    // 遍历数组的每个位置
    for (int i = 1; i <= n; i++) {
        // 预处理前缀和和后缀和，用于优化转移
        prepare();

        // 枚举当前位置可能的值v
        for (int v = 0; v <= m; v++) {
            // 枚举状态s：0表示v不是最大值，1表示v是最大值
            for (int s = 0; s <= 1; s++) {
                int ans = 0;

                // 如果当前位置的值已经确定（不为0）
                if (arr[i] != 0) {
                    // 检查是否满足约束条件：arr[i] <= max(arr[i-1], arr[i+1])
                    if (arr[i] >= v || s == 1) {
                        // 转移到确定的值arr[i]
                        ans = memo[arr[i]][arr[i] > v ? 0 : 1];
                    }
                } else {
                    // 如果当前位置的值需要填充（为0）

                    // 情况1：填充值大于v的数字
                    if (v + 1 <= m) {
                        ans = (ans + suf0[v + 1]) % MOD;
                    }

                    // 情况2：填充值等于v的数字（v不为0时）
                    if (v != 0) {
                        ans = (ans + memo[v][1]) % MOD;
                    }

                    // 情况3：当s=1时，可以填充值小于v的数字
                    if (s == 1) {
                        if (v - 1 >= 0) {
                            ans = (ans + pre1[v - 1]) % MOD;
                        }
                    }
                }

                // 更新dp数组
                dp[v][s] = ans;
            }
        }

        // 交换memo和dp数组，为下一轮计算做准备
        swap(memo, dp);
    }

    // 返回最终答案：memo[0][1]表示整个数组构建完成后的方案数
    return memo[0][1];
}

/**
 * 主函数
 * 处理输入输出
 */
int main() {
    // 持续读取测试用例直到EOF
    while (cin >> n) {
        // 读取数组，下标从1开始
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }
        // 计算并输出结果
        cout << compute() << endl;
    }
    return 0;
}
