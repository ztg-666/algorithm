#include <iostream>
#include <vector>
#include <numeric> // 引入数值操作库，尽管在这个特定翻译中不直接使用其函数

const int MAXN_CONST = 1001;        // 数组最大容量
std::vector<int> nums(MAXN_CONST);  // 存储身高的数组
int n_val;                          // 队员数量
const int MOD = 19650827;           // 取模的常量

// dp[l][r][0] : 形成l...r排列的方法数，且l位置的数字是最后加入的
// dp[l][r][1] : 形成l...r排列的方法数，且r位置的数字是最后加入的
int dp_full[MAXN_CONST][MAXN_CONST][2]; // 三维DP数组

// 时间复杂度 O(n^2)
// 严格位置依赖的动态规划 (非空间压缩版本)

// 假设 n_val > 1，因为 main 函数会单独处理 n_val == 1 的情况。
int solve_full_dp() {
    // 初始化DP数组为0
    for (int i = 0; i <= n_val; ++i) {
        for (int j = 0; j <= n_val; ++j) {
            dp_full[i][j][0] = 0;
            dp_full[i][j][1] = 0;
        }
    }

    // 基本情况：处理长度为2的子序列

    // nums 数组的索引是从 1 开始的
    for (int i = 1; i < n_val; ++i) {
        if (nums[i] < nums[i + 1]) {
            // 如果 nums[i] < nums[i+1]，那么 [i, i+1] 可以形成
            // nums[i] 最后加入 (左边) -> 1 种方法
            // nums[i+1] 最后加入 (右边) -> 1 种方法
            dp_full[i][i + 1][0] = 1;
            dp_full[i][i + 1][1] = 1;
        }
    }

    // 状态转移：
    // l 从 n_val-2 向下迭代到 1 (区间左端点)
    for (int l = n_val - 2; l >= 1; --l) {
        // r 从 l+2 向上迭代到 n_val (区间右端点)
        for (int r = l + 2; r <= n_val; ++r) {
            // 计算 dp_full[l][r][0]：nums[l] 是最后加入的，形成 [l...r]
            // 要形成这个状态，nums[l] 必须能够加入到 [l+1...r] 这个已形成的序列的左边。
            // 1. 如果 nums[l] < nums[l+1]（nums[l]可以放在nums[l+1]的左边）
            //    并且 [l+1...r] 是通过最后加入 nums[l+1] 形成的 (dp_full[l+1][r][0])
            if (nums[l] < nums[l + 1]) {
                dp_full[l][r][0] = (dp_full[l][r][0] + dp_full[l + 1][r][0]) % MOD;
            }
            // 2. 如果 nums[l] < nums[r]（nums[l]可以放在整个序列的最左边，当前右端是nums[r]）
            //    并且 [l+1...r] 是通过最后加入 nums[r] 形成的 (dp_full[l+1][r][1])
            if (nums[l] < nums[r]) {
                dp_full[l][r][0] = (dp_full[l][r][0] + dp_full[l + 1][r][1]) % MOD;
            }

            // 计算 dp_full[l][r][1]：nums[r] 是最后加入的，形成 [l...r]
            // 要形成这个状态，nums[r] 必须能够加入到 [l...r-1] 这个已形成的序列的右边。
            // 1. 如果 nums[r] > nums[l]（nums[r]可以放在整个序列的最右边，当前左端是nums[l]）
            //    并且 [l...r-1] 是通过最后加入 nums[l] 形成的 (dp_full[l][r-1][0])
            if (nums[r] > nums[l]) {
                dp_full[l][r][1] = (dp_full[l][r][1] + dp_full[l][r - 1][0]) % MOD;
            }
            // 2. 如果 nums[r] > nums[r-1]（nums[r]可以放在nums[r-1]的右边）
            //    并且 [l...r-1] 是通过最后加入 nums[r-1] 形成的 (dp_full[l][r-1][1])
            if (nums[r] > nums[r - 1]) {
                dp_full[l][r][1] = (dp_full[l][r][1] + dp_full[l][r - 1][1]) % MOD;
            }
        }
    }

    // 最终答案是形成 [1...n_val] 的所有方法数之和
    return (dp_full[1][n_val][0] + dp_full[1][n_val][1]) % MOD;
}

int main() {
    // 使用快速 I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // 读取输入直到文件尾 (EOF)
    while (std::cin >> n_val) {
        if (n_val == 0 && std::cin.eof()) { // 如果输入0且是文件末尾，则结束 (某些题目约定)
            break;
        }
        for (int i = 1; i <= n_val; ++i) { // nums 数组从索引 1 开始存储
            std::cin >> nums[i];
        }

        if (n_val == 1) {
            std::cout << 1 << std::endl;
        } else {
            std::cout << solve_full_dp() << std::endl; // 调用非空间压缩版本的DP求解函数
        }
    }

    return 0;
}

// std::vector<std::vector<int>> dp(MAXN_CONST, std::vector<int>(2)); // 动态规划表
// int solve() {
//     // 区间 [n_val-1, n_val] 的基本情况初始化
//     // 这会填充 dp[n_val][...]，它代表 l = n_val-1 且 r = n_val 的区间的状态。
//     // dp[n_val][0] 对应于完整 3D DP 表中的 dp_table[n_val-1][n_val][0]。
//     // dp[n_val][1] 对应于完整 3D DP 表中的 dp_table[n_val-1][n_val][1]。
//     // 首先初始化为 0
//     dp[n_val][0] = 0;
//     dp[n_val][1] = 0;
//     if (nums[n_val - 1] < nums[n_val]) {
//         dp[n_val][0] = 1; // nums[n_val-1] 最后加入以形成 [n_val-1, n_val]
//         dp[n_val][1] = 1; // nums[n_val] 最后加入以形成 [n_val-1, n_val]
//     }
//
//     // l 从 n_val-2 向下迭代到 1。'l' 是区间的左边界。
//     for (int l = n_val - 2; l >= 1; l--) {
//         // 区间 [l, l+1] 的基本情况
//         // dp[l+1][0] 将存储 dp_table[l][l+1][0]
//         // dp[l+1][1] 将存储 dp_table[l][l+1][1]
//         // 首先初始化为 0
//         dp[l + 1][0] = 0;
//         dp[l + 1][1] = 0;
//         if (nums[l] < nums[l + 1]) {
//             dp[l + 1][0] = 1; // nums[l] 最后加入以形成 [l, l+1]
//             dp[l + 1][1] = 1; // nums[l+1] 最后加入以形成 [l, l+1]
//         }
//
//         // r 从 l+2 向上迭代到 n_val。'r' 是区间 [l,r] 的右边界。
//         for (int r = l + 2; r <= n_val; r++) {
//             int ways_l_last = 0; // 存储形成 [l...r] 且 nums[l] 最后加入的方法数 (dp_table[l][r][0])
//             int ways_r_last = 0; // 存储形成 [l...r] 且 nums[r] 最后加入的方法数 (dp_table[l][r][1])
//
//             // 计算 ways_l_last (dp_table[l][r][0])：nums[l] 最后加入到左边。
//             // 前一个区间是 [l+1...r]。
//             // nums[l] 根据之前的状态与 nums[l+1] 或 nums[r] 相邻。
//             // 情况 1：nums[l+1] 是 [l+1...r] 中最后加入的。将 nums[l] 添加到 nums[l+1] 的左侧。
//             if (nums[l] < nums[l + 1]) {
//                 // dp[r][0] 当前持有 dp_table[l+1][r][0]（来自前一个 'l' 迭代或初始设置）
//                 ways_l_last = (ways_l_last + dp[r][0]) % MOD;
//             }
//             // 情况 2：nums[r] 是 [l+1...r] 中最后加入的。将 nums[l] 添加到左侧，与 nums[r]（“另一端”）比较。
//             if (nums[l] < nums[r]) {
//                 // dp[r][1] 当前持有 dp_table[l+1][r][1]（来自前一个 'l' 迭代或初始设置）
//                 ways_l_last = (ways_l_last + dp[r][1]) % MOD;
//             }
//
//             // 计算 ways_r_last (dp_table[l][r][1])：nums[r] 最后加入到右边。
//             // 前一个区间是 [l...r-1]。
//             // nums[r] 根据之前的状态与 nums[r-1] 或 nums[l] 相邻。
//             // 情况 1：nums[l] 是 [l...r-1] 中最后加入的。将 nums[r] 添加到 nums[r-1] 的右侧，与 nums[l] 比较。
//             if (nums[r] > nums[l]) {
//                 // dp[r-1][0] 持有 dp_table[l][r-1][0]（在当前 'l' 迭代的、前一个 'r' 步骤中计算）
//                 ways_r_last = (ways_r_last + dp[r - 1][0]) % MOD;
//             }
//             // 情况 2：nums[r-1] 是 [l...r-1] 中最后加入的。将 nums[r] 添加到 nums[r-1] 的右侧。
//             if (nums[r] > nums[r - 1]) {
//                  // dp[r-1][1] 持有 dp_table[l][r-1][1]（在当前 'l' 迭代的、前一个 'r' 步骤中计算）
//                 ways_r_last = (ways_r_last + dp[r - 1][1]) % MOD;
//             }
//
//             // 存储计算得到的 dp_table[l][r][0] 和 dp_table[l][r][1] 的值
//             // 这些将在下一个 'l' 迭代 (l' = l-1) 中用作 dp_table[l'+1][r][0] 和 dp_table[l'+1][r][1]
//             // 或者在当前 'l' 迭代的下一个 'r' 步骤中用作 dp_table[l][r'-1][0] 和 dp_table[l][r'-1][1]。
//             dp[r][0] = ways_l_last;
//             dp[r][1] = ways_r_last;
//         }
//     }
//     // 最终答案是区间 [1, n_val] 的结果
//     return (dp[n_val][0] + dp[n_val][1]) % MOD;
// }