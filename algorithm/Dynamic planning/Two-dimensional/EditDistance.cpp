#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// 编辑距离
// 给你两个单词 word1 和 word2
// 请返回将 word1 转换成 word2 所使用的最少代价
// 你可以对一个单词进行如下三种操作：
// 插入一个字符，代价a
// 删除一个字符，代价b
// 替换一个字符，代价c
// 测试链接 : https://leetcode.cn/problems/edit-distance/

class  EditDistance {
public:
    // 外部接口，默认三个操作的代价都是1
    int minDistance(string word1, string word2) {
        return editDistance2(word1, word2, 1, 1, 1);
    }

    // 最初尝试版本：四种情况全部显式判断
    static int editDistance1(const string& str1, const string& str2, int a, int b, int c) {
        int n = str1.length();
        int m = str2.length();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        // 初始化第一列（str2为空，只能删除str1的字符）
        for (int i = 1; i <= n; i++) {
            dp[i][0] = i * b;
        }

        // 初始化第一行（str1为空，只能插入str2的字符）
        for (int j = 1; j <= m; j++) {
            dp[0][j] = j * a;
        }

        // 动态规划填表
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                int p1 = INT_MAX; // 不用操作（字符相同）
                if (str1[i - 1] == str2[j - 1]) {
                    p1 = dp[i - 1][j - 1];
                }
                int p2 = INT_MAX; // 替换
                if (str1[i - 1] != str2[j - 1]) {
                    p2 = dp[i - 1][j - 1] + c;
                }
                int p3 = dp[i][j - 1] + a;     // 插入
                int p4 = dp[i - 1][j] + b;     // 删除
                dp[i][j] = min({p1, p2, p3, p4});
            }
        }

        return dp[n][m];
    }

    // 优化版：减少分支判断，统一处理逻辑
    static int editDistance2(const string& str1, const string& str2, int a, int b, int c) {
        int n = str1.length();
        int m = str2.length();
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

        // 初始化边界
        for (int i = 1; i <= n; i++) dp[i][0] = i * b;
        for (int j = 1; j <= m; j++) dp[0][j] = j * a;

        // 填表
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                if (str1[i - 1] == str2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1]; // 相同则无需操作
                } else {
                    // 不相同时，取三种操作中的最小代价
                    dp[i][j] = min({
                        dp[i - 1][j] + b,        // 删除
                        dp[i][j - 1] + a,        // 插入
                        dp[i - 1][j - 1] + c     // 替换
                    });
                }
            }
        }

        return dp[n][m];
    }

    // 空间优化版：压缩为一维数组
    static int editDistance3(const string& str1, const string& str2, int a, int b, int c) {
        int n = str1.length();
        int m = str2.length();
        vector<int> dp(m + 1, 0);

        // 初始化第一行
        for (int j = 1; j <= m; j++) {
            dp[j] = j * a;
        }

        // 遍历每一行（i表示str1的前缀长度）
        for (int i = 1; i <= n; i++) {
            int leftUp = (i - 1) * b;  // 对应于 dp[i-1][j-1]
            int backup;
            dp[0] = i * b;             // 第一列，只能通过删除获得

            for (int j = 1; j <= m; j++) {
                backup = dp[j];        // 暂存当前位置的旧值，稍后更新 leftUp
                if (str1[i - 1] == str2[j - 1]) {
                    dp[j] = leftUp;    // 字符相等，无需操作
                } else {
                    // 插入、删除、替换三选一
                    dp[j] = min({
                        dp[j] + b,         // 删除
                        dp[j - 1] + a,     // 插入
                        leftUp + c         // 替换
                    });
                }
                leftUp = backup;       // 更新 leftUp 为下轮使用
            }
        }

        return dp[m];
    }
};

// 主函数：简单测试示例
int main() {
    EditDistance solver;
    string word1 = "intention";
    string word2 = "execution";

    // 使用默认代价：插入=1，删除=1，替换=1
    int result = solver.minDistance(word1, word2);
    cout << "从 \"" << word1 << "\" 转换成 \"" << word2 << "\" 的最小代价是: " << result << endl;

    // 测试不同实现版本，确保一致性
    cout << "editDistance1 结果: " << EditDistance::editDistance1(word1, word2, 1, 1, 1) << endl;
    cout << "editDistance2 结果: " << EditDistance::editDistance2(word1, word2, 1, 1, 1) << endl;
    cout << "editDistance3 结果: " << EditDistance::editDistance3(word1, word2, 1, 1, 1) << endl;

    return 0;
}
