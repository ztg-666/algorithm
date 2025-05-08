#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // 用于 std::min
// 让字符串成为回文串的最少插入次数
// 给你一个字符串 s
// 每一次操作你都可以在字符串的任意位置插入任意字符
// 请你返回让s成为回文串的最少操作次数
// 测试链接 : https://leetcode.cn/problems/minimum-insertion-steps-to-make-a-string-palindrome/
class MinimumInsertionToPalindrome {
public:
    // 方法1：递归 (暴力方法)
    // s[l....r] 这个范围的字符串，使其变成回文串
    // 返回至少需要插入多少个字符
    static int f1(const std::string& s, int l, int r) {
        // l <= r
        if (l >= r) { // 基本情况：如果 l == r (单个字符) 或 l > r (视为空范围)
            return 0;
        }
        if (l + 1 == r) { // 基本情况：两个字符
            return (s[l] == s[r]) ? 0 : 1;
        }
        // l...r 不止两个字符
        if (s[l] == s[r]) {
            return f1(s, l + 1, r - 1);
        } else {
            return std::min(f1(s, l, r - 1), f1(s, l + 1, r)) + 1;
        }
    }

    static int minInsertions1(const std::string& str) {
        if (str.empty()) {
            return 0;
        }
        return f1(str, 0, str.length() - 1);
    }

    // 方法2：记忆化搜索
    static int f2(const std::string& s, int l, int r, std::vector<std::vector<int>>& dp) {
        if (dp[l][r] != -1) {
            return dp[l][r];
        }
        int ans;
        if (l >= r) {
            ans = 0;
        } else if (l + 1 == r) {
            ans = (s[l] == s[r]) ? 0 : 1;
        } else {
            if (s[l] == s[r]) {
                ans = f2(s, l + 1, r - 1, dp);
            } else {
                ans = std::min(f2(s, l, r - 1, dp), f2(s, l + 1, r, dp)) + 1;
            }
        }
        dp[l][r] = ans;
        return ans;
    }

    static int minInsertions2(const std::string& str) {
        if (str.empty()) {
            return 0;
        }
        int n = str.length();
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));
        return f2(str, 0, n - 1, dp);
    }

    // 方法3：严格位置依赖的动态规划
    static int minInsertions3(const std::string& str) {
        if (str.empty()) {
            return 0;
        }
        int n = str.length();
        if (n < 2) {
            return 0;
        }
        std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

        // 基本情况：长度为2的子串
        // l 的范围从 0 到 n-2。对于每个 l，r = l+1
        for (int l = 0; l < n - 1; ++l) {
            dp[l][l + 1] = (str[l] == str[l + 1]) ? 0 : 1;
        }

        // 填充dp表，处理长度从3到n的子串

        // l 从 n-3 递减到 0
        // r 从 l+2 递增到 n-1
        for (int l = n - 3; l >= 0; --l) {
            for (int r = l + 2; r < n; ++r) {
                if (str[l] == str[r]) {
                    dp[l][r] = dp[l + 1][r - 1];
                } else {
                    dp[l][r] = std::min(dp[l][r - 1], dp[l + 1][r]) + 1;
                }
            }
        }
        return dp[0][n - 1];
    }

    // 方法4：空间优化的动态规划
    // 以及一维数组如何模拟二维数组。
    // 然后从中推导出最小插入次数 (n - lps_length)。

    static int minInsertions4(const std::string& str) {
        int n = str.length();
        if (n < 2) {
            return 0;
        }

        std::vector<int> dp(n, 0); // dp[j] 存储当前 i (即 l) 对应的 dp[i][j]
        
        // 最后的 dp[n-1] 是 dp[0][n-1]

        // 初始状态：对于第一个 'l' (l=n-2)，dp 数组代表了相关的最后一行。
        // 对于 l = n-2：我们需要 dp[n-2][n-1]。
        // dp[n-1] 将存储 dp[n-2][n-1]。
        dp[n-1] = (str[n-2] == str[n-1]) ? 0 : 1; // 这是 (l=n-2, r=n-1) 的情况

        // l 从 n-3 递减到 0
        for (int l = n - 3; l >= 0; --l) {
            int leftDown; // 存储 dp[l+1][r-1]。
                        // leftDown 在 dp[l+1] 被覆盖之前用 dp[l+1] 初始化。
                        // 所以，leftDown = (来自前一个 l 迭代的 dp[l+1] 的值，即 dp[l+1][l+1])。
                        // dp[l+1][l+1] 总是 0。所以对于 r=l+2，初始的 leftDown 是正确的，因为 dp[l+1][(l+2)-1] = 0。

            leftDown = dp[l+1]; // 这是 dp[prev_L_iteration][l+1]，确实是 dp[l+1][l+1]，应该为0。

            // 为当前的 L 迭代设置 dp[l+1]，即 dp[l][l+1]
            // 当 r = l+2 时，它将作为 dp[r-1]。
            dp[l+1] = (str[l] == str[l+1]) ? 0 : 1;


            for (int r = l + 2; r < n; ++r) {
                int backUp = dp[r]; // 这是 dp[l+1][r] (来自前一个 L 迭代在当前列 r 的值)
                if (str[l] == str[r]) {
                    dp[r] = leftDown; // dp[l][r] = dp[l+1][r-1]
                } else {
                    // dp[r-1] 是 dp[l][r-1] (已为当前 l 更新)
                    // dp[r] (即 backUp) 是 dp[l+1][r]
                    dp[r] = std::min(dp[r-1], backUp) + 1;
                }
                leftDown = backUp; // 对于下一个 r 迭代，leftDown 变成 dp[l+1][current_r]
            }
        }
        return dp[n-1]; // 这是 dp[0][n-1]
    }
};


int main() {
    std::string s1 = "zzazz";
    std::cout << "1: " << s1 << std::endl;
    std::cout << " 1 (): " << MinimumInsertionToPalindrome::minInsertions1(s1) << std::endl;
    std::cout << " 2 (): " << MinimumInsertionToPalindrome::minInsertions2(s1) << std::endl;
    std::cout << " 3 (): " << MinimumInsertionToPalindrome::minInsertions3(s1) << std::endl;
    std::cout << " 4 (): " << MinimumInsertionToPalindrome::minInsertions4(s1) << std::endl;
    std::cout << std::endl;

    std::string s2 = "mbadm";
    std::cout << ": " << s2 << std::endl;
    std::cout << " 1 (): " << MinimumInsertionToPalindrome::minInsertions1(s2) << std::endl;
    std::cout << " 2 (): " << MinimumInsertionToPalindrome::minInsertions2(s2) << std::endl;
    std::cout << " 3 (): " << MinimumInsertionToPalindrome::minInsertions3(s2) << std::endl;
    std::cout << " 4 ( DP): " << MinimumInsertionToPalindrome::minInsertions4(s2) << std::endl;
    std::cout << std::endl;

    std::string s3 = "leetcode";
    std::cout << ": " << s3 << std::endl;
    std::cout << " 1 (): " << MinimumInsertionToPalindrome::minInsertions1(s3) << std::endl;
    std::cout << " 2 (): " << MinimumInsertionToPalindrome::minInsertions2(s3) << std::endl;
    std::cout << " 3 (DP): " << MinimumInsertionToPalindrome::minInsertions3(s3) << std::endl;
    std::cout << " 4 ( DP): " << MinimumInsertionToPalindrome::minInsertions4(s3) << std::endl;
    std::cout << std::endl;

    std::string s4 = "a";
    std::cout << ": " << s4 << std::endl;
    std::cout << " 1 (): " << MinimumInsertionToPalindrome::minInsertions1(s4) << std::endl;
    std::cout << " 2 (): " << MinimumInsertionToPalindrome::minInsertions2(s4) << std::endl;
    std::cout << " 3 (DP): " << MinimumInsertionToPalindrome::minInsertions3(s4) << std::endl;
    std::cout << " 4 ( DP): " << MinimumInsertionToPalindrome::minInsertions4(s4) << std::endl;
    std::cout << std::endl;

    std::string s5 = "ab";
    std::cout << ": " << s5 << std::endl;
    std::cout << " 4 ( DP): " << MinimumInsertionToPalindrome::minInsertions4(s5) << std::endl;

    std::cout << std::endl;

    return 0;
}
