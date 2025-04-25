#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Solution {
public:
    // 时间复杂度O(n)，n是字符串的长度
    int longestValidParentheses(string str) {
        if (str.empty()) return 0; // 空字符串直接返回0

        vector<int> dp(str.size(), 0); // dp数组，初始化为0
        int ans = 0; // 记录最大有效括号长度

        for (int i = 1; i < str.size(); i++) {
            if (str[i] == ')') { // 只有右括号才需要处理
                int p = i - dp[i - 1] - 1; // 找到可能匹配的左括号位置
                if (p >= 0 && str[p] == '(') { // 如果找到匹配的左括号
                    dp[i] = dp[i - 1] + 2; // 当前有效长度至少是dp[i-1] + 2
                    if (p - 1 >= 0) { // 如果左括号前面还有字符
                        dp[i] += dp[p - 1]; // 加上前一个有效括号的长度
                    }
                }
            }
            ans = max(ans, dp[i]); // 更新最大有效括号长度
        }

        return ans; // 返回结果
    }
};

// 主函数，用于测试longestValidParentheses方法
int main() {
    Solution solution;

    // 测试用例：输入字符串，输出最长有效括号子串的长度
    string str;
    cout << "str: ";
    cin >> str;

    // 调用longestValidParentheses方法并输出结果
    int result = solution.longestValidParentheses(str);
    cout << "length: " << result << endl;

    return 0;
}
