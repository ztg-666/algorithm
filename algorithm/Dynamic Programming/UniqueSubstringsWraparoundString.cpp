#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN = 1e5+10;
class Solution {
public:
    // 时间复杂度O(n)，n是字符串s的长度，字符串base长度为正无穷
    int findSubstringInWraproundString(string str) {
        int n = str.length();
        int s[MAXN]{}; // 将字符映射为0到25的整数
        for (int i = 0; i < n; i++) {
            s[i] = str[i] - 'a'; // 将'a'到'z'映射为0到25
        }

        int dp[27]{}; // dp数组，记录以每个字符结尾的最大延伸长度
        dp[s[0]] = 1; // 初始化第一个字符的最大延伸长度为1

        // 遍历字符串，计算每个字符的最大延伸长度
        for (int i = 1, len = 1; i < n; i++) {
            int cur = s[i];     // 当前字符
            int pre = s[i - 1]; // 前一个字符

            // 判断是否符合环绕规则
            if ((pre == 25 && cur == 0) || (pre + 1 == cur)) {
                len++; // 符合规则，延伸长度加1
            } else {
                len = 1; // 不符合规则，重置延伸长度为1
            }

            dp[cur] = max(dp[cur], len); // 更新当前字符的最大延伸长度
        }

        // 统计所有字符的最大延伸长度之和
        int ans = 0;
        for (int i = 0; i < 26; i++) {
            ans += dp[i];
        }

        return ans; // 返回结果
    }
};
// 环绕字符串中唯一的子字符串
// 定义字符串 base 为一个 "abcdefghijklmnopqrstuvwxyz" 无限环绕的字符串
// 所以 base 看起来是这样的：
// "..zabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd.."
// 给你一个字符串 s ，请你统计并返回 s 中有多少 不同非空子串 也在 base 中出现
// 测试链接 : https://leetcode.cn/problems/unique-substrings-in-wraparound-string/
// 主函数，用于测试findSubstringInWraproundString方法
int main() {
    Solution solution;

    // 测试用例：输入字符串，输出唯一的环绕子串数量
    string str;
    cout << "str: ";
    cin >> str;

    // 调用findSubstringInWraproundString方法并输出结果
    int result = solution.findSubstringInWraproundString(str);
    cout << "length: " << result << endl;

    return 0;
}
