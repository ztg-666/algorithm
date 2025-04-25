#include<bits/stdc++.h>
using namespace std;
class Solution {
public:
    // 暴力尝试
    int numDecodings1(const string& s) {
        return f1(s, 0);
    }

    // 辅助函数，递归计算从索引i开始的解码方法数
    int f1(const string& s, int i) {
        // 如果当前索引已经到达字符串末尾，说明找到一种解码方法
        if (i == s.size()) {
            return 1;
        }
        int ans;
        // 如果当前字符是'0'，则无法解码，返回0
        if (s[i] == '0') {
            ans = 0;
        } else {
            // 先尝试单个字符解码
            ans = f1(s, i + 1);
            // 再尝试两个字符解码，前提是下一个字符存在且组合值在1到26之间
            if (i + 1 < s.size() && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26) {
                ans += f1(s, i + 2);
            }
        }
        return ans;
    }


    // 暴力尝试改记忆化搜索
    int numDecodings2(const string& s) {
        int dp[105]; // 假设字符串长度不超过100
        memset(dp, -1, sizeof(dp)); // 初始化dp数组为-1，表示未计算过
        return f2(s, 0, dp);
    }


    // 辅助函数，递归计算从索引i开始的解码方法数，并使用dp数组存储中间结果
    int f2(const string& s, int i, int dp[]) {
        // 如果当前索引已经到达字符串末尾，说明找到一种解码方法
        if (i == s.size()) {
            return 1;
        }
        // 如果当前结果已经计算过，直接返回
        if (dp[i] != -1) {
            return dp[i];
        }
        int ans;
        // 如果当前字符是'0'，则无法解码，返回0
        if (s[i] == '0') {
            ans = 0;
        } else {
            // 先尝试单个字符解码
            ans = f2(s, i + 1, dp);
            // 再尝试两个字符解码，前提是下一个字符存在且组合值在1到26之间
            if (i + 1 < s.size() && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26) {
                ans += f2(s, i + 2, dp);
            }
        }
        // 存储当前结果到dp数组
        dp[i] = ans;
        return ans;
    }


    // 严格位置依赖的动态规划
    int numDecodings3(const string& s) {
        int n = s.length();
        int dp[105]; // 假设字符串长度不超过100
        dp[n] = 1; // 初始化最后一个位置为1，表示空字符串有一种解码方法
        // 从后向前遍历字符串
        for (int i = n - 1; i >= 0; i--) {
            // 如果当前字符是'0'，则无法解码，设置为0
            if (s[i] == '0') {
                dp[i] = 0;
            } else {
                // 先尝试单个字符解码
                dp[i] = dp[i + 1];
                // 再尝试两个字符解码，前提是下一个字符存在且组合值在1到26之间
                if (i + 1 < n && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26) {
                    dp[i] += dp[i + 2];
                }
            }
        }
        return dp[0]; // 返回从索引0开始的解码方法数
    }

    // 严格位置依赖的动态规划 + 空间压缩
    int numDecodings4(const string& s) {
        int next = 1, nextNext = 0; // next表示dp[i+1]，nextNext表示dp[i+2]
        // 从后向前遍历字符串
        for (int i = s.length() - 1, cur; i >= 0; i--) {
            // 如果当前字符是'0'，则无法解码，设置为0
            if (s[i] == '0') {
                cur = 0;
            } else {
                // 先尝试单个字符解码
                cur = next;
                // 再尝试两个字符解码，前提是下一个字符存在且组合值在1到26之间
                if (i + 1 < s.length() && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26) {
                    cur += nextNext;
                }
            }
            // 更新nextNext和next
            nextNext = next;
            next = cur;
        }
        return next; // 返回从索引0开始的解码方法数
    }
};
// 解码方法
// 一条包含字母 A-Z 的消息通过以下映射进行了 编码 ：
// 'A' -> "1"
// 'B' -> "2"
// ...
// 'Z' -> "26"
// 要 解码 已编码的消息，所有数字必须基于上述映射的方法，反向映射回字母（可能有多种方法）
// 例如，"11106" 可以映射为："AAJF"、"KJF"
// 注意，消息不能分组为(1 11 06)，因为 "06" 不能映射为 "F"
// 这是由于 "6" 和 "06" 在映射中并不等价
// 给你一个只含数字的 非空 字符串 s ，请计算并返回 解码 方法的 总数
// 题目数据保证答案肯定是一个 32位 的整数
// 测试链接 : https://leetcode.cn/problems/decode-ways/
#include<bits/stdc++.h>
using namespace std;

// Solution类的定义（省略，见问题中的代码）

int main() {
    // 创建Solution对象
    Solution solution;

    // 测试字符串
    string testStr = "226"; // 示例输入

    // 调用不同的解码方法并输出结果
    cout << "1: " << solution.numDecodings1(testStr) << endl;
    cout << "2: " << solution.numDecodings2(testStr) << endl;
    cout << "3: " << solution.numDecodings3(testStr) << endl;
    cout << "4: " << solution.numDecodings4(testStr) << endl;

    return 0;
}
