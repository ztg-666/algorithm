#include <vector>
#include <string>
#include <algorithm> // 用于 std::max
#include <utility>   // 用于 std::pair


// 一和零(多维费用背包)
// 给你一个二进制字符串数组 strs 和两个整数 m 和 n
// 请你找出并返回 strs 的最大子集的长度
// 该子集中 最多 有 m 个 0 和 n 个 1
// 如果 x 的所有元素也是 y 的元素，集合 x 是集合 y 的 子集
// 测试链接 : https://leetcode.cn/problems/ones-and-zeroes/




// 辅助函数，用于统计字符串中零和一的数量
// 返回一个包含 {零的数量, 一的数量} 的 pair
std::pair<int, int> countZerosAndOnes(const std::string& str) {
    int zeros = 0;
    int ones = 0;
    for (char c : str) {
        if (c == '0') {
            zeros++;
        } else {
            ones++;
        }
    }
    return {zeros, ones};
}

class Solution {
public:

    // --- 方法 1：普通递归法 ---

    int findMaxForm1(const std::vector<std::string>& strs, int m, int n) {
        return f1(strs, 0, m, n);
    }

private:
    // strs[i....] : 可供选择的字符串
    // z           : 剩余允许使用的零的数量
    // o           : 剩余允许使用的一的数量
    // 返回可以组成的最大字符串数量
    int f1(const std::vector<std::string>& strs, int i, int z, int o) {
        if (i == strs.size()) {
            // 基本情况：没有更多字符串可供选择
            return 0;
        }

        // 选项 1：不包含当前字符串 strs[i]
        int p1 = f1(strs, i + 1, z, o);

        // 选项 2：包含当前字符串 strs[i]（若可行）
        int p2 = 0;
        std::pair<int, int> counts = countZerosAndOnes(strs[i]);
        int current_zeros = counts.first;
        int current_ones = counts.second;

        if (current_zeros <= z && current_ones <= o) {
            // 只有在有足够的零和一的容量时才继续
            p2 = 1 + f1(strs, i + 1, z - current_zeros, o - current_ones);
        }

        return std::max(p1, p2);
    }

public:
    // --- 方法 2：记忆化搜索（自顶向下动态规划） ---

    int findMaxForm2(const std::vector<std::string>& strs, int m, int n) {
        // 用 -1 初始化动态规划表（或任何表示未计算的值）
        std::vector<std::vector<std::vector<int>>> dp(
            strs.size(),
            std::vector<std::vector<int>>(m + 1, std::vector<int>(n + 1, -1))
        );
        return f2(strs, 0, m, n, dp);
    }

private:
    // dp[i][z][o] 存储 f2(strs, i, z, o) 的结果
    int f2(const std::vector<std::string>& strs, int i, int z, int o,
           std::vector<std::vector<std::vector<int>>>& dp) {
        if (i == strs.size()) {
            return 0;
        }
        // 检查结果是否已经计算过
        if (dp[i][z][o] != -1) {
            return dp[i][z][o];
        }

        // 选项 1：不包含 strs[i]
        int p1 = f2(strs, i + 1, z, o, dp);

        // 选项 2：包含 strs[i]（若可行）
        int p2 = 0;
        std::pair<int, int> counts = countZerosAndOnes(strs[i]);
        int current_zeros = counts.first;
        int current_ones = counts.second;

        if (current_zeros <= z && current_ones <= o) {
            p2 = 1 + f2(strs, i + 1, z - current_zeros, o - current_ones, dp);
        }

        // 在返回结果之前存储结果
        dp[i][z][o] = std::max(p1, p2);
        return dp[i][z][o];
    }

public:
    // --- 方法 3：严格动态规划（三维动态规划表） ---

    int findMaxForm3(const std::vector<std::string>& strs, int m, int n) {
        int len = strs.size();
        // dp[i][z][o]: 考虑 strs[i...len - 1] 且零的容量为 z、一的容量为 o 时的最大组成数量
        std::vector<std::vector<std::vector<int>>> dp(
            len + 1, // 需要 len + 1 的大小来处理基本情况 dp[len]...
            std::vector<std::vector<int>>(m + 1, std::vector<int>(n + 1, 0)) // 初始化为 0
        );

        // 从字符串数组的末尾开始反向迭代
        for (int i = len - 1; i >= 0; --i) {
            std::pair<int, int> counts = countZerosAndOnes(strs[i]);
            int current_zeros = counts.first;
            int current_ones = counts.second;

            for (int z = 0; z <= m; ++z) {
                for (int o = 0; o <= n; ++o) {
                    // 选项 1：不包含 strs[i]
                    // 结果与从 i + 1 开始的子问题相同
                    int p1 = dp[i + 1][z][o];

                    // 选项 2：包含 strs[i]（若可行）
                    int p2 = 0;
                    if (current_zeros <= z && current_ones <= o) {
                        p2 = 1 + dp[i + 1][z - current_zeros][o - current_ones];
                    }

                    // dp[i][z][o] 的结果是两个选项中的最大值
                    dp[i][z][o] = std::max(p1, p2);
                }
            }
        }
        // 最终答案是从索引 0 开始、零和一的容量为 m 和 n 的子问题的结果
        return dp[0][m][n];
    }


    // --- 方法 4：空间优化的动态规划（二维动态规划表） ---

    int findMaxForm(const std::vector<std::string>& strs, int m, int n) {
        // dp[z][o]: 考虑到目前已处理的字符串，零的容量为 z、一的容量为 o 时的最大组成数量
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // 遍历输入数组中的每个字符串
        for (const std::string& s : strs) {
            std::pair<int, int> counts = countZerosAndOnes(s);
            int current_zeros = counts.first;
            int current_ones = counts.second;

            // 对容量（z 和 o）进行反向迭代
            // 这对于空间优化的正确性至关重要。
            // 我们需要在计算 “包含 s” 选项时使用 *上一次* 迭代（在考虑字符串 s 之前）的 dp 值。
            // 正向迭代会使用同一字符串处理循环中可能已经更新过的值。
            for (int z = m; z >= current_zeros; --z) {
                for (int o = n; o >= current_ones; --o) {
                    // dp[z][o] 当前保存的是 *不考虑* s 时的最大组成数量
                    // 1 + dp[z - current_zeros][o - current_ones] 是 *如果* 包含 s 时的最大组成数量
                    dp[z][o] = std::max(
                        dp[z][o], // 选项 1：不包含字符串 s
                        1 + dp[z - current_zeros][o - current_ones] // 选项 2：包含字符串 s
                    );
                }
            }
        }
        // 处理完所有字符串后，最终答案存储在 dp[m][n] 中
        return dp[m][n];
    }
};

// 示例用法（可选 - 通常在 main 函数或测试框架中完成）：

#include <iostream>
int main() {
    Solution sol;
    std::vector<std::string> strs1 = {"10", "0001", "111001", "1", "0"};
    int m1 = 5, n1 = 3;
    std::cout << "Result 1 (findMaxForm): " << sol.findMaxForm(strs1, m1, n1) << std::endl; // 输出: 4
    std::cout << "Result 1 (findMaxForm3): " << sol.findMaxForm3(strs1, m1, n1) << std::endl; // 输出: 4
    std::cout << "Result 1 (findMaxForm2): " << sol.findMaxForm2(strs1, m1, n1) << std::endl; // 输出: 4
    std::cout << "Result 1 (findMaxForm1): " << sol.findMaxForm1(strs1, m1, n1) << std::endl; // 输出: 4


    std::vector<std::string> strs2 = {"10", "1", "0"};
    int m2 = 1, n2 = 1;
    std::cout << "Result 2 (findMaxForm): " << sol.findMaxForm(strs2, m2, n2) << std::endl; // 输出: 2
    std::cout << "Result 2 (findMaxForm3): " << sol.findMaxForm3(strs2, m2, n2) << std::endl; // 输出: 2
    std::cout << "Result 2 (findMaxForm2): " << sol.findMaxForm2(strs2, m2, n2) << std::endl; // 输出: 2
    std::cout << "Result 2 (findMaxForm1): " << sol.findMaxForm1(strs2, m2, n2) << std::endl; // 输出: 2

    return 0;
}