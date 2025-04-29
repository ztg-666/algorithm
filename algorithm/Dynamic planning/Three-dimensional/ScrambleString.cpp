#include <string>
#include <vector>
#include <numeric> // 初始化时可能有用
#include <map>     // 可选的字符计数剪枝需要用到
#include <algorithm> // 可选的字符计数剪枝需要用到
// 扰乱字符串
// 使用下面描述的算法可以扰乱字符串 s 得到字符串 t ：
// 步骤1 : 如果字符串的长度为 1 ，算法停止
// 步骤2 : 如果字符串的长度 > 1 ，执行下述步骤：
//        在一个随机下标处将字符串分割成两个非空的子字符串
//        已知字符串s，则可以将其分成两个子字符串x和y且满足s=x+y
//        可以决定是要 交换两个子字符串 还是要 保持这两个子字符串的顺序不变
//        即s可能是 s = x + y 或者 s = y + x
//        在x和y这两个子字符串上继续从步骤1开始递归执行此算法
// 给你两个 长度相等 的字符串 s1 和 s2，判断 s2 是否是 s1 的扰乱字符串
// 如果是，返回true ；否则，返回false
// 测试链接 : https://leetcode.cn/problems/scramble-string/
class Solution {
private:
    // --- 方法 1：递归法（4 个参数：l1, r1, l2, r2） ---
    bool solve_recursive1(const std::string& s1, int l1, int r1,
                          const std::string& s2, int l2, int r2) {
        // 基本情况：单个字符的子字符串
        if (l1 == r1) {
            return s1[l1] == s2[l2];
        }

        // 可选的剪枝操作：检查字符计数（能显著提高性能）
        if (!checkCounts(s1, l1, r1 - l1 + 1, s2, l2)) {
            return false;
        }

        // 尝试所有可能的分割点
        int len = r1 - l1 + 1;
        for (int i = l1; i < r1; ++i) {
            int left_len1 = i - l1 + 1;
            int right_len1 = len - left_len1;

            // 情况 1：不交换 - s1[l1..i] 与 s2[l2..l2+left_len1-1] 并且 s1[i+1..r1] 与 s2[l2+left_len1..r2]
            if (solve_recursive1(s1, l1, i, s2, l2, l2 + left_len1 - 1) &&
                solve_recursive1(s1, i + 1, r1, s2, l2 + left_len1, r2)) {
                return true;
            }

            // 情况 2：交换 - s1[l1..i] 与 s2[r2-left_len1+1..r2] 并且 s1[i+1..r1] 与 s2[l2..l2+right_len1-1]
            if (solve_recursive1(s1, l1, i, s2, r2 - left_len1 + 1, r2) &&
                solve_recursive1(s1, i + 1, r1, s2, l2, l2 + right_len1 - 1)) {
                 return true;
             }
        }

        return false;
    }

    // --- 方法 2：递归法（3 个参数：l1, l2, len） ---
    bool solve_recursive2(const std::string& s1, const std::string& s2, int l1, int l2, int len) {
        // 基本情况：单个字符
        if (len == 1) {
            return s1[l1] == s2[l2];
        }

        // 可选的剪枝操作：检查字符计数
        if (!checkCounts(s1, l1, len, s2, l2)) {
            return false;
        }


        // 尝试第一个部分的所有分割长度 k
        for (int k = 1; k < len; ++k) {
            // 情况 1：不交换
            // 检查 s1[l1...l1+k-1] 与 s2[l2...l2+k-1] 并且 s1[l1+k...l1+len-1] 与 s2[l2+k...l2+len-1]
            if (solve_recursive2(s1, s2, l1, l2, k) &&
                solve_recursive2(s1, s2, l1 + k, l2 + k, len - k)) {
                return true;
            }

            // 情况 2：交换
            // 检查 s1[l1...l1+k-1] 与 s2[l2+len-k...l2+len-1] 并且 s1[l1+k...l1+len-1] 与 s2[l2...l2+len-k-1]
            if (solve_recursive2(s1, s2, l1, l2 + len - k, k) &&
                solve_recursive2(s1, s2, l1 + k, l2, len - k)) {
                return true;
            }
        }
        return false;
    }


    // --- 方法 3：记忆化搜索法（自顶向下动态规划） ---
    // dp 状态：0 = 未计算，1 = 真，-1 = 假
    std::vector<std::vector<std::vector<int>>> memo;

    bool solve_memo(const std::string& s1, const std::string& s2, int l1, int l2, int len) {
        if (len == 1) {
            return s1[l1] == s2[l2];
        }

        // 检查记忆化表
        if (memo[l1][l2][len] != 0) {
            return memo[l1][l2][len] == 1;
        }

        // 可选的剪枝操作：检查字符计数
        if (!checkCounts(s1, l1, len, s2, l2)) {
            memo[l1][l2][len] = -1;
            return false;
        }

        bool ans = false;
        // 尝试所有分割长度 k
        for (int k = 1; k < len; ++k) {
            // 情况 1：不交换
            if (solve_memo(s1, s2, l1, l2, k) &&
                solve_memo(s1, s2, l1 + k, l2 + k, len - k)) {
                ans = true;
                break;
            }
            // 情况 2：交换
            if (solve_memo(s1, s2, l1, l2 + len - k, k) &&
                solve_memo(s1, s2, l1 + k, l2, len - k)) {
                ans = true;
                break;
            }
        }

        // 将结果存储在记忆化表中
        memo[l1][l2][len] = ans? 1 : -1;
        return ans;
    }

    // --- 剪枝的辅助函数（可选但推荐使用） ---
    // 检查子字符串 s1[l1...l1+len-1] 和 s2[l2...l2+len-1]
    // 是否具有相同的字符计数。
    bool checkCounts(const std::string& s1, int l1, int len,
                     const std::string& s2, int l2) {
        if (len == 0) return true; // 这里空字符串被认为是相等的
        std::vector<int> counts(26, 0); // 假设是小写英文字母
        for (int i = 0; i < len; ++i) {
            counts[s1[l1 + i] - 'a']++;
            counts[s2[l2 + i] - 'a']--;
        }
        for (int count : counts) {
            if (count != 0) {
                return false;
            }
        }
        return true;
    }


public:
    // --- 方法 1 的接口 ---
    bool isScramble1(const std::string& s1, const std::string& s2) {
        if (s1.length() != s2.length()) {
            return false;
        }
        if (s1 == s2) { // 优化
             return true;
        }
        int n = s1.length();
        if (n == 0) {
            return true;
        }
         // 顶级的可选剪枝操作
        if (!checkCounts(s1, 0, n, s2, 0)) {
             return false;
        }
        return solve_recursive1(s1, 0, n - 1, s2, 0, n - 1);
    }

     // --- 方法 2 的接口 ---
    bool isScramble2(const std::string& s1, const std::string& s2) {
        if (s1.length() != s2.length()) {
            return false;
        }
         if (s1 == s2) { // 优化
             return true;
         }
        int n = s1.length();
         if (n == 0) {
            return true;
        }
         // 顶级的可选剪枝操作
        if (!checkCounts(s1, 0, n, s2, 0)) {
            return false;
        }
        return solve_recursive2(s1, s2, 0, 0, n);
    }

    // --- 方法 3（记忆化搜索法）的接口 ---
    bool isScramble3(const std::string& s1, const std::string& s2) {
        if (s1.length() != s2.length()) {
            return false;
        }
        if (s1 == s2) { // 优化
             return true;
        }
        int n = s1.length();
        if (n == 0) {
            return true;
        }
         // 顶级的可选剪枝操作
        if (!checkCounts(s1, 0, n, s2, 0)) {
             return false;
        }
        // 初始化记忆化表：n x n x (n+1)，用 0（未计算）填充
        memo.assign(n, std::vector<std::vector<int>>(n, std::vector<int>(n + 1, 0)));
        return solve_memo(s1, s2, 0, 0, n);
    }


    // --- 方法 4：迭代的自底向上动态规划法 ---
    bool isScramble(const std::string& s1, const std::string& s2) {
         if (s1.length() != s2.length()) {
            return false;
        }
        if (s1 == s2) { // 优化
            return true;
        }
        int n = s1.length();
        if (n == 0) {
            return true;
        }
        // 顶级的可选剪枝操作
        if (!checkCounts(s1, 0, n, s2, 0)) {
            return false;
        }

        // dp[l1][l2][len]：如果 s2[l2...l2+len-1] 是 s1[l1...l1+len-1] 的扰乱字符串，则为真
        std::vector<std::vector<std::vector<bool>>> dp(
            n, std::vector<std::vector<bool>>(n, std::vector<bool>(n + 1, false))
        );

        // 基本情况：len = 1
        for (int l1 = 0; l1 < n; ++l1) {
            for (int l2 = 0; l2 < n; ++l2) {
                dp[l1][l2][1] = (s1[l1] == s2[l2]);
            }
        }

        // 填充 len 从 2 到 n 的动态规划表
        for (int len = 2; len <= n; ++len) {
            // s1 子字符串的起始索引 l1
            for (int l1 = 0; l1 <= n - len; ++l1) {
                // s2 子字符串的起始索引 l2
                for (int l2 = 0; l2 <= n - len; ++l2) {
                    // 尝试所有可能的分割长度 k（1 到 len-1）
                    for (int k = 1; k < len; ++k) {
                        // 情况 1：不交换
                        // 检查 dp[l1][l2][k] 并且 dp[l1+k][l2+k][len-k]
                        if (dp[l1][l2][k] && dp[l1 + k][l2 + k][len - k]) {
                            dp[l1][l2][len] = true;
                            break; // 找到了一个有效的 (l1, l2, len) 扰乱字符串
                        }

                        // 情况 2：交换
                        // 检查 dp[l1][l2+len-k][k] 并且 dp[l1+k][l2][len-k]
                        if (dp[l1][l2 + len - k][k] && dp[l1 + k][l2][len - k]) {
                            dp[l1][l2][len] = true;
                            break; // 找到了一个有效的 (l1, l2, len) 扰乱字符串
                        }
                    }
                }
            }
        }

        // s2[0...n-1] 是 s1[0...n-1] 的扰乱字符串吗？
        return dp[0][0][n];
    }
};

// 示例用法（可选）

#include <iostream>

int main() {
    Solution sol;
    std::string s1_1 = "great", s2_1 = "rgeat";
    std::cout << "s1 = " << s1_1 << ", s2 = " << s2_1 << std::endl;
    std::cout << "isScramble1: " << std::boolalpha << sol.isScramble1(s1_1, s2_1) << std::endl; // 真
    std::cout << "isScramble2: " << std::boolalpha << sol.isScramble2(s1_1, s2_1) << std::endl; // 真
    std::cout << "isScramble3: " << std::boolalpha << sol.isScramble3(s1_1, s2_1) << std::endl; // 真
    std::cout << "isScramble (DP): " << std::boolalpha << sol.isScramble(s1_1, s2_1) << std::endl; // 真
    std::cout << "--------------------" << std::endl;

    std::string s1_2 = "abcde", s2_2 = "caebd";
    std::cout << "s1 = " << s1_2 << ", s2 = " << s2_2 << std::endl;
    std::cout << "isScramble1: " << std::boolalpha << sol.isScramble1(s1_2, s2_2) << std::endl; // 假
    std::cout << "isScramble2: " << std::boolalpha << sol.isScramble2(s1_2, s2_2) << std::endl; // 假
    std::cout << "isScramble3: " << std::boolalpha << sol.isScramble3(s1_2, s2_2) << std::endl; // 假
    std::cout << "isScramble (DP): " << std::boolalpha << sol.isScramble(s1_2, s2_2) << std::endl; // 假
    std::cout << "--------------------" << std::endl;

    std::string s1_3 = "a", s2_3 = "a";
    std::cout << "s1 = " << s1_3 << ", s2 = " << s2_3 << std::endl;
    std::cout << "isScramble (DP): " << std::boolalpha << sol.isScramble(s1_3, s2_3) << std::endl; // 真
    std::cout << "--------------------" << std::endl;


    return 0;
}

