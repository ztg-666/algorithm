#include <iostream>
#include <vector>
#include <string>

using namespace std;
// 交错字符串
// 给定三个字符串 s1、s2、s3
// 请帮忙验证s3是否由s1和s2交错组成
// 测试链接 : https://leetcode.cn/problems/interleaving-string/
class InterleavingString {
public:
    // 方法1：普通动态规划实现
    static bool isInterleave1(const string& str1, const string& str2, const string& str3) {
        // 如果长度不匹配，直接返回 false
        if (str1.length() + str2.length() != str3.length()) {
            return false;
        }

        int n = str1.length();
        int m = str2.length();
        // dp[i][j] 表示 str1 前 i 个字符 和 str2 前 j 个字符 能否组成 str3 前 i+j 个字符
        vector<vector<bool>> dp(n + 1, vector<bool>(m + 1, false));
        dp[0][0] = true; // 空串和空串组成空串

        // 初始化第一列：只使用 str1 的前 i 个字符匹配 str3
        for (int i = 1; i <= n; i++) {
            if (str1[i - 1] != str3[i - 1]) break;
            dp[i][0] = true;
        }

        // 初始化第一行：只使用 str2 的前 j 个字符匹配 str3
        for (int j = 1; j <= m; j++) {
            if (str2[j - 1] != str3[j - 1]) break;
            dp[0][j] = true;
        }

        // 动态规划填表
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                // 如果 str1 的当前字符匹配 str3，并且前面的状态成立
                bool fromStr1 = str1[i - 1] == str3[i + j - 1] && dp[i - 1][j];
                // 如果 str2 的当前字符匹配 str3，并且前面的状态成立
                bool fromStr2 = str2[j - 1] == str3[i + j - 1] && dp[i][j - 1];
                dp[i][j] = fromStr1 || fromStr2;
            }
        }

        return dp[n][m];
    }

    // 方法2：空间压缩的动态规划实现
    static bool isInterleave2(const string& str1, const string& str2, const string& str3) {
        // 长度不匹配直接返回 false
        if (str1.length() + str2.length() != str3.length()) {
            return false;
        }

        int n = str1.length();
        int m = str2.length();
        // 使用一维数组压缩空间，dp[j] 表示当前行的状态
        vector<bool> dp(m + 1, false);
        dp[0] = true;  // 空串匹配空串

        // 初始化第一行，只依赖 str2
        for (int j = 1; j <= m; j++) {
            if (str2[j - 1] != str3[j - 1]) break;
            dp[j] = true;
        }

        // 状态转移
        for (int i = 1; i <= n; i++) {
            // 第一列只依赖 str1
            dp[0] = str1[i - 1] == str3[i - 1] && dp[0];
            for (int j = 1; j <= m; j++) {
                bool fromStr1 = str1[i - 1] == str3[i + j - 1] && dp[j];      // 来自上一行（str1）
                bool fromStr2 = str2[j - 1] == str3[i + j - 1] && dp[j - 1];  // 来自左边（str2）
                dp[j] = fromStr1 || fromStr2;
            }
        }

        return dp[m];
    }
};

int main() {
    string s1 = "aab";
    string s2 = "axy";
    string s3 = "aaxaby";

    cout << "使用 isInterleave1 判断是否为交错字符串: "
         << (InterleavingString::isInterleave1(s1, s2, s3) ? "是" : "否") << endl;

    cout << "使用 isInterleave2 判断是否为交错字符串: "
         << (InterleavingString::isInterleave2(s1, s2, s3) ? "是" : "否") << endl;

    return 0;
}
