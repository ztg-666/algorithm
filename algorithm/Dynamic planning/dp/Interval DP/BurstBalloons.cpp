#include <vector>
#include <algorithm>


// 戳气球
// 有 n 个气球，编号为0到n-1，每个气球上都标有一个数字，这些数字存在数组nums中
// 现在要求你戳破所有的气球。戳破第 i 个气球
// 你可以获得 nums[i - 1] * nums[i] * nums[i + 1] 枚硬币
// 这里的 i - 1 和 i + 1 代表和 i 相邻的两个气球的序号
// 如果 i - 1或 i + 1 超出了数组的边界，那么就当它是一个数字为 1 的气球
// 求所能获得硬币的最大数量
// 测试链接 : https://leetcode.cn/problems/burst-balloons/

class BurstBalloons {
public:
    // 记忆化搜索的辅助函数
    // arr: 扩展后的气球数组，两端填充了1，例如 [1, nums[0], nums[1], ..., nums[n-1], 1]
    // l, r: 当前考虑要戳破的气球在 arr 中的索引起始和结束位置 (闭区间 [l, r])
    //       这些气球是 arr[l], arr[l+1], ..., arr[r]
    //       我们保证 arr[l-1] 和 arr[r+1] 是这两个边界外侧未被戳破的气球 (或者是虚拟的1)
    // dp: 记忆化表格，dp[l][r] 存储戳破 arr[l...r]所有气球能获得的最大硬币数
    static int f(const std::vector<int>& arr, int l, int r, std::vector<std::vector<int>>& dp) {
        // 基本情况：如果 l > r，表示在这个范围内没有气球可戳，得分为0
        if (l > r) {
            return 0;
        }

        // 如果已经计算过，直接返回结果
        if (dp[l][r] != -1) {
            return dp[l][r];
        }

        int ans;
        // 基本情况：如果只剩一个气球 arr[l] (即 l == r)
        // 那么戳破它得到的硬币是 arr[l-1] * arr[l] * arr[r+1] (即 arr[l-1] * arr[l] * arr[l+1])
        if (l == r) {
            ans = arr[l - 1] * arr[l] * arr[r + 1];
        } else {
            // 至少有两个气球在 [l, r] 范围内
            // 尝试将 arr[l] 作为 [l,r] 中最后一个被戳破的气球
            int p1 = arr[l - 1] * arr[l] * arr[r + 1] + f(arr, l + 1, r, dp);
            // 尝试将 arr[r] 作为 [l,r] 中最后一个被戳破的气球
            int p2 = arr[l - 1] * arr[r] * arr[r + 1] + f(arr, l, r - 1, dp);
            ans = std::max(p1, p2);

            // 尝试将 arr[k] (其中 l < k < r) 作为 [l,r] 中最后一个被戳破的气球
            for (int k = l + 1; k < r; ++k) {
                int current_config_score = arr[l - 1] * arr[k] * arr[r + 1] +
                                           f(arr, l, k - 1, dp) +
                                           f(arr, k + 1, r, dp);
                ans = std::max(ans, current_config_score);
            }
        }
        dp[l][r] = ans;
        return ans;
    }

    // 方法1: 记忆化搜索
    static int maxCoins1(const std::vector<int>& nums) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }

        // 构建扩展数组 arr，大小为 n+2
        // arr[0] = 1, arr[n+1] = 1
        // arr[1...n] 对应 nums[0...n-1]
        std::vector<int> arr(n + 2);
        arr[0] = 1;
        arr[n + 1] = 1;
        for (int i = 0; i < n; ++i) {
            arr[i + 1] = nums[i];
        }

        // dp[l][r] 表示戳破 arr 中从索引 l 到 r 的所有气球能获得的最大硬币数
        // l 和 r 的有效范围是 [1, n]
        std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, -1));

        // 目标是戳破 arr[1...n] 范围内的所有气球
        return f(arr, 1, n, dp);
    }

    // 方法2: 严格位置依赖的动态规划
    static int maxCoins2(const std::vector<int>& nums) {
        int n = nums.size();
        if (n == 0) {
            return 0;
        }

        std::vector<int> arr(n + 2);
        arr[0] = 1;
        arr[n + 1] = 1;
        for (int i = 0; i < n; ++i) {
            arr[i + 1] = nums[i];
        }

        // dp[l][r] 的含义同上：戳破 arr[l...r] 范围内的所有气球（这些气球的边界是arr[l-1]和arr[r+1]）
        // 所能获得的最大硬币数。
        // 初始化为0，因为如果 l > r (空范围)，得分为0，这在C++中用0初始化vector是默认行为。
        std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, 0));

        // 迭代顺序：l 从 n 向下到 1 (控制子问题的左边界)
        //           r 从 l 向上到 n (控制子问题的右边界)
        //           这样保证计算 dp[l][r] 时，其依赖的更小子问题 (如 dp[l+1][r], dp[l][r-1] 等) 已被计算。

        // 当 l == r 时 (子问题只有一个气球 arr[l])
        // 戳破它，得分为 arr[l-1] * arr[l] * arr[l+1]
        for (int i = 1; i <= n; ++i) {
            dp[i][i] = arr[i - 1] * arr[i] * arr[i + 1];
        }

        // 处理长度大于1的子问题 (即 r > l)
        // 循环变量 len 代表子问题的长度 r - l + 1
        // for (int len = 2; len <= n; ++len) { // 遍历子问题的长度
        //     for (int l = 1; l <= n - len + 1; ++l) { // 遍历子问题的起始点 l
        //         int r = l + len - 1; // 计算子问题的结束点 r
        
        for (int l = n; l >= 1; --l) { // l 从 n 倒推到 1
            for (int r = l + 1; r <= n; ++r) { // r 从 l+1 (保证至少两个气球) 到 n
                // 尝试将 arr[l] 作为 [l,r] 中最后一个被戳破的气球
                // 此时，右边子问题是戳破 arr[l+1...r]，其边界是 arr[l] 和 arr[r+1]
                // 得分是 arr[l-1]*arr[l]*arr[r+1] (戳破l的得分) + dp[l+1][r] (戳破l+1..r的得分)
                int score_l_last = arr[l - 1] * arr[l] * arr[r + 1] + dp[l + 1][r];

                // 尝试将 arr[r] 作为 [l,r] 中最后一个被戳破的气球
                // 此时，左边子问题是戳破 arr[l...r-1]，其边界是 arr[l-1] 和 arr[r]
                // 得分是 arr[l-1]*arr[r]*arr[r+1] (戳破r的得分) + dp[l][r-1] (戳破l..r-1的得分)
                int score_r_last = arr[l - 1] * arr[r] * arr[r + 1] + dp[l][r - 1];
                
                dp[l][r] = std::max(score_l_last, score_r_last);

                // 尝试将 arr[k] (其中 l < k < r) 作为 [l,r] 中最后一个被戳破的气球
                // 此时，左子问题戳破 arr[l...k-1] (边界 arr[l-1], arr[k])
                // 右子问题戳破 arr[k+1...r] (边界 arr[k], arr[r+1])
                // 戳破k的得分是 arr[l-1]*arr[k]*arr[r+1]
                for (int k = l + 1; k < r; ++k) {
                    int current_config_score = arr[l - 1] * arr[k] * arr[r + 1] +
                                               dp[l][k - 1] +
                                               dp[k + 1][r];
                    dp[l][r] = std::max(dp[l][r], current_config_score);
                }
            }
        }
        return dp[1][n]; // 最终结果是戳破 arr[1...n] 所有气球的最大得分
    }
};


#include <iostream>
int main() {
    std::vector<int> nums1 = {3, 1, 5, 8};
    std::cout << "Nums: {3, 1, 5, 8}" << std::endl;
    std::cout << "Max Coins (Memoization): " << BurstBalloons::maxCoins1(nums1) << std::endl; // Expected: 167
    std::cout << "Max Coins (DP): " << BurstBalloons::maxCoins2(nums1) << std::endl;          // Expected: 167
    std::cout << std::endl;

    std::vector<int> nums2 = {1, 5};
    std::cout << "Nums: {1, 5}" << std::endl;
    std::cout << "Max Coins (Memoization): " << BurstBalloons::maxCoins1(nums2) << std::endl; // Expected: 10
    std::cout << "Max Coins (DP): " << BurstBalloons::maxCoins2(nums2) << std::endl;          // Expected: 10
    std::cout << std::endl;

    std::vector<int> nums3 = {};
    std::cout << "Nums: {}" << std::endl;
    std::cout << "Max Coins (Memoization): " << BurstBalloons::maxCoins1(nums3) << std::endl; // Expected: 0
    std::cout << "Max Coins (DP): " << BurstBalloons::maxCoins2(nums3) << std::endl;          // Expected: 0
    std::cout << std::endl;
    
    std::vector<int> nums4 = {8};
    std::cout << "Nums: {8}" << std::endl;
    // arr = [1, 8, 1]
    // f(arr, 1, 1, dp) or dp[1][1] = arr[0]*arr[1]*arr[2] = 1*8*1 = 8
    std::cout << "Max Coins (Memoization): " << BurstBalloons::maxCoins1(nums4) << std::endl; // Expected: 8
    std::cout << "Max Coins (DP): " << BurstBalloons::maxCoins2(nums4) << std::endl;          // Expected: 8
    std::cout << std::endl;

    return 0;
}

