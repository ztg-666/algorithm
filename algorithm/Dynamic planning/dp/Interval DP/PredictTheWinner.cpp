#include <vector>
#include <numeric>      // For std::accumulate
#include <algorithm>    // For std::min, std::max
#include <vector>       // For std::vector (though already included by numeric or algorithm often)

// 预测赢家
// 给你一个整数数组 nums 。玩家 1 和玩家 2 基于这个数组设计了一个游戏
// 玩家 1 和玩家 2 轮流进行自己的回合，玩家 1 先手
// 开始时，两个玩家的初始分值都是 0
// 每一回合，玩家从数组的任意一端取一个数字
// 取到的数字将会从数组中移除，数组长度减1
// 玩家选中的数字将会加到他的得分上
// 当数组中没有剩余数字可取时游戏结束
// 如果玩家 1 能成为赢家，返回 true
// 如果两个玩家得分相等，同样认为玩家 1 是游戏的赢家，也返回 true
// 你可以假设每个玩家的玩法都会使他的分数最大化
// 测试链接 : https://leetcode.cn/problems/predict-the-winner/

// 确保类名与文件名或上下文的预期一致
class PredictTheWinner {
public:
    // 暴力尝试 (方法1)
    // nums[l...r]范围上的数字进行游戏，轮到当前玩家
    // 返回当前玩家最终能获得多少分数，两玩家都绝顶聪明
    static int f1(const std::vector<int>& nums, int l, int r) {
        if (l == r) { // 只剩一个数，直接拿走
            return nums[l];
        }
        if (l == r - 1) { // 只剩两个数，拿走较大的那个
            return std::max(nums[l], nums[r]);
        }
        // l....r 不只两个数
        // 可能性1 ：当前玩家拿走nums[l]。对手会在剩下的l+1...r上玩。
        // 对手会选择拿走nums[l+1]或者nums[r]，使得留给当前玩家的分数最小。
        // 如果对手拿走nums[l+1]，当前玩家在 l+2...r 上继续，得分为 f1(nums, l + 2, r)
        // 如果对手拿走nums[r]，当前玩家在 l+1...r-1 上继续，得分为 f1(nums, l + 1, r - 1)
        // 所以，对手会让当前玩家得到 min(f1(nums, l + 2, r), f1(nums, l + 1, r - 1))
        int p1 = nums[l] + std::min(f1(nums, l + 2, r), f1(nums, l + 1, r - 1));

        // 可能性2 ：当前玩家拿走nums[r]。对手会在剩下的l...r-1上玩。
        // 对手会选择拿走nums[l]或者nums[r-1]，使得留给当前玩家的分数最小。
        // 如果对手拿走nums[l]，当前玩家在 l+1...r-1 上继续，得分为 f1(nums, l + 1, r - 1)
        // 如果对手拿走nums[r-1]，当前玩家在 l...r-2 上继续，得分为 f1(nums, l, r - 2)
        // 所以，对手会让当前玩家得到 min(f1(nums, l + 1, r - 1), f1(nums, l, r - 2))
        int p2 = nums[r] + std::min(f1(nums, l + 1, r - 1), f1(nums, l, r - 2));
        
        return std::max(p1, p2); // 当前玩家选择让自己得分最大的那个方案
    }

    static bool predictTheWinner1(const std::vector<int>& nums) {
        long long sum = 0; // 使用 long long 避免潜在的整数溢出，尽管题目分数范围可能不大
        for (int num : nums) {
            sum += num;
        }
        int n = nums.size();
        if (n == 0) {
            return true; 
        }
        int firstPlayerScore = f1(nums, 0, n - 1);
        long long secondPlayerScore = sum - firstPlayerScore;
        return firstPlayerScore >= secondPlayerScore;
    }

    // 记忆化搜索 (方法2)
    static int f2(const std::vector<int>& nums, int l, int r, std::vector<std::vector<int>>& dp) {
        if (dp[l][r] != -1) {
            return dp[l][r];
        }
        int ans;
        if (l == r) {
            ans = nums[l];
        } else if (l == r - 1) {
            ans = std::max(nums[l], nums[r]);
        } else {
            int p1 = nums[l] + std::min(f2(nums, l + 2, r, dp), f2(nums, l + 1, r - 1, dp));
            int p2 = nums[r] + std::min(f2(nums, l + 1, r - 1, dp), f2(nums, l, r - 2, dp));
            ans = std::max(p1, p2);
        }
        dp[l][r] = ans;
        return ans;
    }

    static bool predictTheWinner2(const std::vector<int>& nums) {
        long long sum = 0;
        for (int num : nums) {
            sum += num;
        }
        int n = nums.size();
        if (n == 0) {
            return true;
        }
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));
        int firstPlayerScore = f2(nums, 0, n - 1, dp);
        long long secondPlayerScore = sum - firstPlayerScore;
        return firstPlayerScore >= secondPlayerScore;
    }

    // 严格位置依赖的动态规划 (方法3)
    static bool predictTheWinner3(const std::vector<int>& nums) {
        long long sum = 0;
        for (int num : nums) {
            sum += num;
        }
        int n = nums.size();
        if (n == 0) {
            return true;
        }
        if (n == 1) { // 单独处理n=1的情况，避免dp初始化和循环问题
            return true; // 玩家1拿走唯一的数，必赢
        }

        std::vector<std::vector<int>> dp(n, std::vector<int>(n));

        // 初始化dp表
        // 当 l == r 时 (即子数组长度为1)
        for (int i = 0; i < n; i++) {
            dp[i][i] = nums[i];
        }

        // 当 l == r - 1 时 (即子数组长度为2)
        for (int i = 0; i < n - 1; i++) {
            dp[i][i + 1] = std::max(nums[i], nums[i + 1]);
        }
        
        // 填充dp表，子数组长度从3开始到n
        // l 从 n-3 递减到 0
        // r 从 l+2 递增到 n-1
        // 确保 l+2, l+1, r-1, r-2 的访问是有效的，这在dp[...][...]的索引中由循环保证
        // 例如，当计算 dp[l][r] 时，
        // dp[l+2][r] : l' = l+2, r' = r. l' > r' 是不可能的，因为长度至少为1. l+2 <= r
        // dp[l+1][r-1] : l' = l+1, r' = r-1. l+1 <= r-1
        // dp[l][r-2] : l' = l, r' = r-2. l <= r-2
        // 这些子问题的长度 (r'-l'+1) 都比当前问题 (r-l+1) 的长度小2.

        for (int l = n - 3; l >= 0; l--) {
            for (int r = l + 2; r < n; r++) {
                int p1 = nums[l] + std::min(dp[l + 2][r], dp[l + 1][r - 1]);
                int p2 = nums[r] + std::min(dp[l + 1][r - 1], dp[l][r - 2]);
                dp[l][r] = std::max(p1, p2);
            }
        }

        int firstPlayerScore = dp[0][n - 1];
        long long secondPlayerScore = sum - firstPlayerScore;
        return firstPlayerScore >= secondPlayerScore;
    }
};


// 示例用法
#include <iostream>
int main() {
    std::vector<int> nums1 = {1, 5, 2};
    std::cout << "Nums: {1, 5, 2}" << std::endl;
    std::cout << "Predict The Winner 1: " << (PredictTheWinner::predictTheWinner1(nums1) ? "true" : "false") << std::endl; // Expected: false
    std::cout << "Predict The Winner 2: " << (PredictTheWinner::predictTheWinner2(nums1) ? "true" : "false") << std::endl; // Expected: false
    std::cout << "Predict The Winner 3: " << (PredictTheWinner::predictTheWinner3(nums1) ? "true" : "false") << std::endl; // Expected: false
    std::cout << std::endl;

    std::vector<int> nums2 = {1, 5, 233, 7};
    std::cout << "Nums: {1, 5, 233, 7}" << std::endl;
    std::cout << "Predict The Winner 1: " << (PredictTheWinner::predictTheWinner1(nums2) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 2: " << (PredictTheWinner::predictTheWinner2(nums2) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 3: " << (PredictTheWinner::predictTheWinner3(nums2) ? "true" : "false") << std::endl; // Expected: true
    std::cout << std::endl;

    std::vector<int> nums3 = {0};
     std::cout << "Nums: {0}" << std::endl;
    std::cout << "Predict The Winner 1: " << (PredictTheWinner::predictTheWinner1(nums3) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 2: " << (PredictTheWinner::predictTheWinner2(nums3) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 3: " << (PredictTheWinner::predictTheWinner3(nums3) ? "true" : "false") << std::endl; // Expected: true
    std::cout << std::endl;

    std::vector<int> nums4 = {};
    std::cout << "Nums: {}" << std::endl;
    std::cout << "Predict The Winner 1: " << (PredictTheWinner::predictTheWinner1(nums4) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 2: " << (PredictTheWinner::predictTheWinner2(nums4) ? "true" : "false") << std::endl; // Expected: true
    std::cout << "Predict The Winner 3: " << (PredictTheWinner::predictTheWinner3(nums4) ? "true" : "false") << std::endl; // Expected: true
    std::cout << std::endl;
    
    return 0;
}






