#include <vector>
#include <numeric> // 对于这段特定代码并非严格必需，但在处理向量时通常很有用
#include <algorithm> // 用于 std::max
#include <iostream>
#include <random>   // 用于生成随机数
#include <ctime>    // 用于为随机数生成器设置种子

// 命名空间，避免反复写 std::（在头文件中使用时需谨慎）
// using namespace std;

// --- 递归暴力求解法（用于验证）---

// 递归求解的辅助函数
int f(const std::vector<int>& nums, int i, int s) {
    if (i == nums.size()) {
        // 如果当前和 's' 能被 7 整除，返回 s，否则返回 0（无效路径）
        return (s % 7 == 0)? s : 0;
    }
    // 选项 1：子序列中不包含 nums[i]
    int res1 = f(nums, i + 1, s);
    // 选项 2：子序列中包含 nums[i]
    int res2 = f(nums, i + 1, s + nums[i]);
    // 返回两个选项中找到的最大和
    return std::max(res1, res2);
}

// 递归暴力求解法的主函数
int maxSum1(const std::vector<int>& nums) {
    // 从索引 0 开始递归，初始和为 0
    return f(nums, 0, 0);
}

// --- 动态规划解法（高效）---

int maxSum2(const std::vector<int>& nums) {
    int n = nums.size();
    // dp[i][j]：使用 nums[0...i-1] 中的元素构成的子序列的最大和
    // 使得该和对 7 取模的结果等于 j。
    // dp[i][j] = -1 表示不存在这样的子序列。
    // 维度：(n + 1) 行，用于考虑 0 到 n 个元素；7 列，用于表示余数 0 到 6。
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(7, -1));

    // 基础情况：有 0 个元素（i=0）时，唯一可能的和是 0，其对 7 取模的余数为 0。
    dp[0][0] = 0;
    // 有 0 个元素时，其他所有余数都是不可能的情况。（已经初始化为 -1）

    // 遍历输入数组 nums 中的每个数字
    for (int i = 1; i <= n; ++i) {
        int x = nums[i - 1];      // 正在考虑的当前数字
        int current_rem = x % 7; // 当前数字的余数

        // 遍历和的所有可能余数（0 到 6）
        for (int j = 0; j < 7; ++j) {
            // 选项 1：不包含当前数字 x（nums[i-1]）。
            // 使用前 i 个元素得到余数为 j 的最大和，初始时与使用前 i-1 个元素得到余数为 j 的最大和相同。
            dp[i][j] = dp[i - 1][j];

            // 选项 2：尝试包含当前数字 x。
            // 如果我们包含 x（余数为 current_rem）以得到总余数 j，
            // 我们必须来自一个先前的状态，其余数为 'needed_rem'，使得
            // (needed_rem + current_rem) % 7 == j。
            // 这意味着 needed_rem = (j - current_rem + 7) % 7。

            // 计算上一步（i-1）所需的余数
            int needed_rem = (j - current_rem + 7) % 7; // +7 用于处理 j - current_rem 可能出现的负数结果

            // 检查在步骤 i-1 时，对于所需余数是否存在有效的子序列
            if (dp[i - 1][needed_rem] != -1) {
                // 如果存在，计算新的可能的和
                int potential_sum = dp[i - 1][needed_rem] + x;
                // 如果这个新的和大于当前的最佳和，则更新 dp[i][j]
                dp[i][j] = std::max(dp[i][j], potential_sum);
            }
        }
    }

    // 最终答案是使用所有 n 个元素（第 n 行）
    // 且余数为 0 时得到的最大和。
    // 如果 dp[n][0] 仍然是 -1，这意味着没有子序列的和是 7 的倍数。
    // 然而，空子序列（和为 0）总是可能的，所以 dp[n][0] 至少为 0。
    return dp[n][0];
}


// --- 测试辅助函数 ---

// 生成一个由非负整数组成的随机向量
std::vector<int> randomArray(int n, int v) {
    // 设置随机数生成
    static std::random_device rd; // 从硬件获取一个随机数
    static std::mt19937 gen(rd()); // 为生成器设置种子
    // 定义分布范围 [0, v-1]
    std::uniform_int_distribution<> distrib(0, v - 1);

    std::vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[i] = distrib(gen); // 生成随机数
    }
    return ans;
}

// --- 主测试函数（对数测试器）---

int main() {
    const int n = 15; // 数组的最大长度
    const int v = 30; // 数组中的最大值
    const int testTime = 20000; // 测试用例的数量

    std::cout << "Testing started..." << std::endl;

    // 为用于生成数组长度的随机数生成器设置种子
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    for (int i = 0; i < testTime; ++i) {
        // 生成 1 到 n（包含 1 和 n）之间的随机长度
        int len = (std::rand() % n) + 1;
        // 生成随机数组
        std::vector<int> nums = randomArray(len, v);

        // 用两种方法计算结果
        int ans1 = maxSum1(nums);
        int ans2 = maxSum2(nums);

        // 比较结果
        if (ans1 != ans2) {
            std::cout << "Error!" << std::endl;
            std::cout << "Input: {";
            for(size_t k=0; k<nums.size(); ++k) {
                std::cout << nums[k] << (k == nums.size()-1 ? "" : ", ");
            }
            std::cout << "}" << std::endl;
            std::cout << "Brute force (ans1): " << ans1 << std::endl;
            std::cout << "DP (ans2): " << ans2 << std::endl;
            // 可选：在第一次出错时停止
            // break;
        }
    }

    std::cout << "Testing finished." << std::endl;

    return 0; // 表示执行成功
}