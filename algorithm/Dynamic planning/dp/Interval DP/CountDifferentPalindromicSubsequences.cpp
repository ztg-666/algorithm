#include <bits/stdc++.h>
using namespace std;
// 统计不同回文子序列
// 给你一个字符串s，返回s中不同的非空回文子序列个数
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/count-different-palindromic-subsequences/
static const int MOD = 1'000'000'007;

int countPalindromicSubsequences(const string& s) {
    int n = s.size();
    if (n == 0) return 0;

    // 从左侧记录每个字符（ASCII）上次出现的位置
    vector<int> last(256, -1);
    vector<int> left(n);
    for (int i = 0; i < n; ++i) {
        left[i] = last[s[i]];
        last[s[i]] = i;
    }

    // 从右侧记录每个字符上次出现的位置
    fill(last.begin(), last.end(), n);
    vector<int> right(n);
    for (int i = n - 1; i >= 0; --i) {
        right[i] = last[s[i]];
        last[s[i]] = i;
    }

    // dp[i][j] = 子串 s[i..j] 中不同的非空回文子序列数量
    vector<vector<long long>> dp(n, vector<long long>(n, 0));
    // 基本情况：单个字符
    for (int i = 0; i < n; ++i) {
        dp[i][i] = 1;
    }

    // 按子串长度递增填充 DP 表
    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i + len - 1 < n; ++i) {
            int j = i + len - 1;
            if (s[i] != s[j]) {
                dp[i][j] = dp[i][j - 1] + dp[i + 1][j] - dp[i + 1][j - 1];
            } else {
                int l = right[i];
                int r = left[j];
                if (l > r) {
                    // i 和 j 之间没有相同字符
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 2;
                } else if (l == r) {
                    // i 和 j 之间恰好有一个相同字符
                    dp[i][j] = dp[i + 1][j - 1] * 2 + 1;
                } else {
                    // i 和 j 之间有多个相同字符
                    dp[i][j] = dp[i + 1][j - 1] * 2 - dp[l + 1][r - 1];
                }
            }
            // 确保取模前非负
            dp[i][j] = (dp[i][j] % MOD + MOD) % MOD;
        }
    }

    return static_cast<int>(dp[0][n - 1]);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    // 示例用法：
    // 输入：一个字符串 s
    // 输出：不同的非空回文子序列数量，模 1e9+7
    if (getline(cin, s)) {
        cout << countPalindromicSubsequences(s) << "\n";
    }

    return 0;
}
