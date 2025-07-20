#include <iostream>
#include <vector>
#include <cstring>

// 还原数组的方法数(不优化枚举)
// 原本有一个长度为n的数组arr，下标从1开始，数组中都是<=200的正数
// 并且任意i位置的数字都满足 : arr[i] <= max(arr[i-1], arr[i+1])
// 特别的，arr[1] <= arr[2]，arr[n] <= arr[n-1]
// 但是输入的arr中有些数字丢失了，丢失的数字用0表示
// 返回还原成不违规的arr有多少种方法，答案对 998244353 取模
// 3 <= n <= 10^4
// 测试链接 : https://www.nowcoder.com/practice/49c5284278974cbda474ec13d8bd86a9

using namespace std;

const int MOD = 998244353;  // 模数
const int MAXN = 10001;     // 最大数组长度

int arr[MAXN];  // 输入数组，下标从1开始
int n;          // 数组长度
int m = 200;    // 数组中元素的最大值

// 记忆化搜索的dp数组
// dp[i][v][s] 表示在位置i，下一个位置值为v，状态为s时的方案数
int dp[MAXN][201][2];

/**
 * 递归函数：计算还原数组的方法数
 * @param i 当前处理的位置（从n到1）
 * @param v i+1位置的数字值（0表示没有下一个位置）
 * @param s 状态标志：
 *          0 - 表示i+1位置的数字v > i+2位置的数字
 *          1 - 表示i+1位置的数字v <= i+2位置的数字
 * @return 方案数
 */
int f1(int i, int v, int s) {
    // 递归终止条件：处理完所有位置
    if (i == 0) {
        return s;  // 如果s=1说明满足条件，返回1；否则返回0
    }

    // 如果已经计算过，直接返回结果
    if (dp[i][v][s] != -1) {
        return dp[i][v][s];
    }

    int ans = 0;

    // 如果当前位置的值已经确定（不为0）
    if (arr[i] != 0) {
        // 检查是否满足约束条件：arr[i] <= max(arr[i-1], arr[i+1])
        if (arr[i] >= v || s == 1) {
            // 递归处理前一个位置
            ans = f1(i - 1, arr[i], arr[i] > v ? 0 : 1);
        }
    } else {
        // 如果当前位置的值需要填充（为0）

        // 情况1：当前位置的值大于下一个位置的值
        for (int cur = v + 1; cur <= m; cur++) {
            ans = (ans + f1(i - 1, cur, 0)) % MOD;
        }

        // 情况2：当前位置的值等于下一个位置的值（v != 0时）
        if (v != 0) {
            ans = (ans + f1(i - 1, v, 1)) % MOD;
        }

        // 情况3：当前位置的值小于下一个位置的值（当s=1时）
        if (s == 1) {
            for (int cur = 1; cur < v; cur++) {
                ans = (ans + f1(i - 1, cur, 1)) % MOD;
            }
        }
    }

    // 记忆化存储结果
    dp[i][v][s] = ans;
    return ans;
}

/**
 * 方法1：递归 + 记忆化搜索，不优化枚举
 * 时间复杂度：O(n * m²)
 * 空间复杂度：O(n * m)
 */
int compute1() {
    // 初始化记忆化数组为-1，表示未计算
    memset(dp, -1, sizeof(dp));
    // 从位置n开始，初始状态为v=0, s=1
    return f1(n, 0, 1);
}

/**
 * 方法2：严格位置依赖的动态规划，不优化枚举
 * 时间复杂度：O(n * m²)
 * 空间复杂度：O(n * m)
 */
int compute2() {
    // 三维dp数组：dp[位置][下一个位置的值][状态]
    int dp[MAXN][201][2];

    // 边界条件初始化：位置0
    for (int v = 0; v <= m; v++) {
        dp[0][v][0] = 0;  // 状态0时方案数为0
        dp[0][v][1] = 1;  // 状态1时方案数为1
    }

    // 从位置1到n逐层填充dp表
    for (int i = 1; i <= n; i++) {
        for (int v = 0; v <= m; v++) {        // 枚举下一个位置的值
            for (int s = 0; s <= 1; s++) {    // 枚举状态
                int ans = 0;

                if (arr[i] != 0) {
                    // 当前位置值已确定
                    if (arr[i] >= v || s == 1) {
                        ans = dp[i - 1][arr[i]][arr[i] > v ? 0 : 1];
                    }
                } else {
                    // 当前位置值需要填充

                    // 枚举所有可能的当前位置值
                    for (int cur = v + 1; cur <= m; cur++) {
                        ans = (ans + dp[i - 1][cur][0]) % MOD;
                    }

                    if (v != 0) {
                        ans = (ans + dp[i - 1][v][1]) % MOD;
                    }

                    if (s == 1) {
                        for (int cur = 1; cur < v; cur++) {
                            ans = (ans + dp[i - 1][cur][1]) % MOD;
                        }
                    }
                }
                dp[i][v][s] = ans;
            }
        }
    }
    // 返回最终结果：位置n，下一个位置值为0，状态为1
    return dp[n][0][1];
}

/**
 * 方法3：空间压缩版本，不优化枚举
 * 时间复杂度：O(n * m²)
 * 空间复杂度：O(m)
 */
int compute3() {
    // 使用滚动数组优化空间
    int memo[201][2];  // 上一层的结果
    int dp[201][2];    // 当前层的结果

    // 初始化边界条件
    for (int v = 0; v <= m; v++) {
        memo[v][0] = 0;
        memo[v][1] = 1;
    }

    // 逐层计算
    for (int i = 1; i <= n; i++) {
        for (int v = 0; v <= m; v++) {
            for (int s = 0; s <= 1; s++) {
                int ans = 0;

                if (arr[i] != 0) {
                    // 当前位置值已确定
                    if (arr[i] >= v || s == 1) {
                        ans = memo[arr[i]][arr[i] > v ? 0 : 1];
                    }
                } else {
                    // 当前位置值需要填充
                    for (int cur = v + 1; cur <= m; cur++) {
                        ans = (ans + memo[cur][0]) % MOD;
                    }

                    if (v != 0) {
                        ans = (ans + memo[v][1]) % MOD;
                    }

                    if (s == 1) {
                        for (int cur = 1; cur < v; cur++) {
                            ans = (ans + memo[cur][1]) % MOD;
                        }
                    }
                }
                dp[v][s] = ans;
            }
        }

        // 滚动数组：将当前层结果复制到memo中
        for (int v = 0; v <= m; v++) {
            for (int s = 0; s <= 1; s++) {
                memo[v][s] = dp[v][s];
            }
        }
    }

    return memo[0][1];
}

/**
 * 主函数：处理输入输出
 */
int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 处理多组测试用例
    while (cin >> n) {
        // 读取数组（下标从1开始）
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }

        // 输出结果（使用方法1：记忆化搜索）
        cout << compute1() << endl;
    }

    return 0;
}
