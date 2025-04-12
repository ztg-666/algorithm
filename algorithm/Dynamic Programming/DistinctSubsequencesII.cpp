#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    // 时间复杂度O(n)，n是字符串s的长度
    int distinctSubseqII(string s) {
        const int mod = 1000000007; // 模数，防止整数溢出
        vector<int> cnt(26, 0);    // 记录每个字符最后新增的子序列数量
        int all = 1;               // 包括空串在内的所有子序列数量
        int newAdd;                // 当前字符新增的子序列数量

        // 遍历字符串中的每个字符
        for (char x : s) {
            // 计算当前字符新增的子序列数量
            // newAdd = (all - cnt[x - 'a'] + mod) % mod
            // 解释：
            // 1. all 是当前所有子序列的数量（包括空串）
            // 2. cnt[x - 'a'] 是以字符 x 结尾的子序列数量
            // 3. all - cnt[x - 'a'] 表示不包括以 x 结尾的子序列数量
            // 4. 加上 mod 确保结果为正数
            // 5. 取模 mod 防止整数溢出
            newAdd = (all - cnt[x - 'a'] + mod) % mod;

            // 更新当前字符对应的计数
            // cnt[x - 'a'] = (cnt[x - 'a'] + newAdd) % mod
            // 解释：
            // 1. cnt[x - 'a'] 增加 newAdd，表示以字符 x 结尾的子序列数量增加
            // 2. 取模 mod 防止整数溢出
            cnt[x - 'a'] = (cnt[x - 'a'] + newAdd) % mod;

            // 更新总的子序列数量
            // all = (all + newAdd) % mod
            // 解释：
            // 1. all 增加 newAdd，表示总的子序列数量增加
            // 2. 取模 mod 防止整数溢出
            all = (all + newAdd) % mod;
        }

        // 返回结果（减去空串的数量）
        // (all - 1 + mod) % mod
        // 解释：
        // 1. all 包括空串，因此需要减去1
        // 2. 加上 mod 确保结果为正数
        // 3. 取模 mod 防止整数溢出
        return (all - 1 + mod) % mod;
    }
};

// 不同的子序列 II
// 给定一个字符串 s，计算 s 的 不同非空子序列 的个数
// 因为结果可能很大，答案对 1000000007 取模
// 字符串的 子序列 是经由原字符串删除一些（也可能不删除）
// 字符但不改变剩余字符相对位置的一个新字符串
// 例如，"ace" 是 "abcde" 的一个子序列，但 "aec" 不是
// 测试链接 : https://leetcode.cn/problems/distinct-subsequences-ii/
// 主函数，用于测试distinctSubseqII方法
int main() {
    Solution solution;

    // 测试用例：输入字符串，输出不同的非空子序列数量
    string str;
    cout << "str: ";
    cin >> str;

    // 调用distinctSubseqII方法并输出结果
    int result = solution.distinctSubseqII(str);
    cout << "length: " << result << endl;

    return 0;
}
