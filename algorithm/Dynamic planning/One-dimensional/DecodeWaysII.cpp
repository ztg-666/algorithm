#include <iostream>
#include <cstring>
#include <string>

using namespace std;

class Solution {
public:
    const int MOD = 1000000007; // 模数，用于防止结果溢出
    const int MAXN = 1e5 + 10;  // 最大字符串长度限制

    // 暴力递归版本（未取模）
    int numDecodings1(const string& str) {
        return f1(str, 0); // 调用递归函数，从索引0开始解码
    }

    // 递归函数：计算从索引i开始的解码方式数量
    int f1(const string& s, int i) {
        if (i == s.size()) return 1; // 如果到达字符串末尾，说明找到一种解码方式
        if (s[i] == '0') return 0;   // 如果当前字符是'0'，无法解码，返回0

        // 单独解码当前字符
        int ans = f1(s, i + 1) * (s[i] == '*' ? 9 : 1); // 如果当前字符是'*'，可以表示1~9共9种可能

        // 尝试与下一个字符组合解码
        if (i + 1 < s.size()) { // 确保有下一个字符
            if (s[i] != '*') { // 当前字符不是'*'
                if (s[i + 1] != '*') { // 下一个字符也不是'*'
                    if ((s[i] - '0') * 10 + (s[i + 1] - '0') <= 26) { // 组合值在1~26之间
                        ans += f1(s, i + 2); // 递归计算剩余部分的解码方式
                    }
                } else { // 下一个字符是'*'
                    if (s[i] == '1') ans += f1(s, i + 2) * 9; // "1*"可以表示11~19，共9种
                    if (s[i] == '2') ans += f1(s, i + 2) * 6; // "2*"可以表示21~26，共6种
                }
            } else { // 当前字符是'*'
                if (s[i + 1] != '*') { // 下一个字符不是'*'
                    if (s[i + 1] <= '6') ans += f1(s, i + 2) * 2; // "*X"可以表示1X或2X，共2种
                    else ans += f1(s, i + 2); // "*X"只能表示1X，共1种
                } else { // 下一个字符也是'*'
                    ans += f1(s, i + 2) * 15; // "**"可以表示11~19和21~26，共15种
                }
            }
        }
        return ans; // 返回总的解码方式数量
    }

    // 带记忆化的递归版本
    int numDecodings2(const string& str) {
        const int n = str.size(); // 字符串长度
        long dp[MAXN]; // 静态数组，存储中间结果
        memset(dp, -1, sizeof(dp)); // 初始化为-1，表示尚未计算
        return static_cast<int>(f2(str, 0, dp)); // 调用带记忆化的递归函数
    }

    // 带记忆化的递归函数
    long f2(const string& s, int i, long dp[]) {
        if (i == s.size()) return 1; // 到达字符串末尾，返回1种解码方式
        if (s[i] == '0') return 0;   // 当前字符为'0'，无法解码
        if (dp[i] != -1) return dp[i]; // 如果已经计算过，直接返回缓存结果

        long ans = f2(s, i + 1, dp) * (s[i] == '*' ? 9 : 1); // 单独解码当前字符

        if (i + 1 < s.size()) { // 尝试与下一个字符组合解码
            if (s[i] != '*') { // 当前字符不是'*'
                if (s[i + 1] != '*') { // 下一个字符也不是'*'
                    if ((s[i] - '0') * 10 + (s[i + 1] - '0') <= 26) { // 组合值在1~26之间
                        ans += f2(s, i + 2, dp); // 递归计算剩余部分的解码方式
                    }
                } else { // 下一个字符是'*'
                    if (s[i] == '1') ans += f2(s, i + 2, dp) * 9; // "1*"可以表示11~19，共9种
                    if (s[i] == '2') ans += f2(s, i + 2, dp) * 6; // "2*"可以表示21~26，共6种
                }
            } else { // 当前字符是'*'
                if (s[i + 1] != '*') { // 下一个字符不是'*'
                    if (s[i + 1] <= '6') ans += f2(s, i + 2, dp) * 2; // "*X"可以表示1X或2X，共2种
                    else ans += f2(s, i + 2, dp); // "*X"只能表示1X，共1种
                } else { // 下一个字符也是'*'
                    ans += f2(s, i + 2, dp) * 15; // "**"可以表示11~19和21~26，共15种
                }
            }
        }
        ans %= MOD; // 对结果取模
        dp[i] = ans; // 缓存结果
        return ans; // 返回总的解码方式数量
    }

    // 动态规划版本
    int numDecodings3(const string& str) {
        const int n = str.size(); // 字符串长度
        long dp[MAXN]; // 静态数组，存储动态规划状态
        memset(dp, 0, sizeof(dp)); // 初始化为0
        dp[n] = 1; // 哨兵位，表示空字符串有1种解码方式

        for (int i = n - 1; i >= 0; --i) { // 从后向前遍历
            if (str[i] != '0') { // 当前字符不是'0'，可以解码
                dp[i] = (str[i] == '*' ? 9 : 1) * dp[i + 1]; // 单独解码当前字符

                if (i + 1 < n) { // 尝试与下一个字符组合解码
                    if (str[i] != '*') { // 当前字符不是'*'
                        if (str[i + 1] != '*') { // 下一个字符也不是'*'
                            if ((str[i] - '0') * 10 + (str[i + 1] - '0') <= 26) { // 组合值在1~26之间
                                dp[i] += dp[i + 2]; // 加上组合解码的方式
                            }
                        } else { // 下一个字符是'*'
                            if (str[i] == '1') dp[i] += dp[i + 2] * 9; // "1*"可以表示11~19，共9种
                            if (str[i] == '2') dp[i] += dp[i + 2] * 6; // "2*"可以表示21~26，共6种
                        }
                    } else { // 当前字符是'*'
                        if (str[i + 1] != '*') { // 下一个字符不是'*'
                            if (str[i + 1] <= '6') dp[i] += dp[i + 2] * 2; // "*X"可以表示1X或2X，共2种
                            else dp[i] += dp[i + 2]; // "*X"只能表示1X，共1种
                        } else { // 下一个字符也是'*'
                            dp[i] += dp[i + 2] * 15; // "**"可以表示11~19和21~26，共15种
                        }
                    }
                }
                dp[i] %= MOD; // 对结果取模
            }
        }
        return static_cast<int>(dp[0]); // 返回从索引0开始的解码方式数量
    }

    // 空间优化的动态规划版本
    int numDecodings4(const string& str) {
        const int n = str.size(); // 字符串长度
        long cur = 0, next = 1, nextNext = 0; // 使用三个变量代替dp数组

        for (int i = n - 1; i >= 0; --i) { // 从后向前遍历
            if (str[i] != '0') { // 当前字符不是'0'，可以解码
                cur = (str[i] == '*' ? 9 : 1) * next; // 单独解码当前字符

                if (i + 1 < n) { // 尝试与下一个字符组合解码
                    if (str[i] != '*') { // 当前字符不是'*'
                        if (str[i + 1] != '*') { // 下一个字符也不是'*'
                            if ((str[i] - '0') * 10 + (str[i + 1] - '0') <= 26) { // 组合值在1~26之间
                                cur += nextNext; // 加上组合解码的方式
                            }
                        } else { // 下一个字符是'*'
                            if (str[i] == '1') cur += nextNext * 9; // "1*"可以表示11~19，共9种
                            if (str[i] == '2') cur += nextNext * 6; // "2*"可以表示21~26，共6种
                        }
                    } else { // 当前字符是'*'
                        if (str[i + 1] != '*') { // 下一个字符不是'*'
                            if (str[i + 1] <= '6') cur += nextNext * 2; // "*X"可以表示1X或2X，共2种
                            else cur += nextNext; // "*X"只能表示1X，共1种
                        } else { // 下一个字符也是'*'
                            cur += nextNext * 15; // "**"可以表示11~19和21~26，共15种
                        }
                    }
                }
                cur %= MOD; // 对结果取模
            }
            nextNext = next; // 更新nextNext为当前的next
            next = cur;      // 更新next为当前的cur
            cur = 0;         // 重置cur
        }
        return static_cast<int>(next); // 返回最终结果
    }
};
// 解码方法 II
// 一条包含字母 A-Z 的消息通过以下的方式进行了 编码 ：
// 'A' -> "1"
// 'B' -> "2"
// ...
// 'Z' -> "26"
// 要 解码 一条已编码的消息，所有的数字都必须分组
// 然后按原来的编码方案反向映射回字母（可能存在多种方式）
// 例如，"11106" 可以映射为："AAJF"、"KJF"
// 注意，像 (1 11 06) 这样的分组是无效的，"06"不可以映射为'F'
// 除了上面描述的数字字母映射方案，编码消息中可能包含 '*' 字符
// 可以表示从 '1' 到 '9' 的任一数字（不包括 '0'）
// 例如，"1*" 可以表示 "11"、"12"、"13"、"14"、"15"、"16"、"17"、"18" 或 "19"
// 对 "1*" 进行解码，相当于解码该字符串可以表示的任何编码消息
// 给你一个字符串 s ，由数字和 '*' 字符组成，返回 解码 该字符串的方法 数目
// 由于答案数目可能非常大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/decode-ways-ii/
int main() {
    Solution sol;
    string input = "1*"; // 测试输入
    cout << "numDecodings1: " << sol.numDecodings1(input) << endl; // 输出暴力递归结果
    cout << "numDecodings2: " << sol.numDecodings2(input) << endl; // 输出带记忆化递归结果
    cout << "numDecodings3: " << sol.numDecodings3(input) << endl; // 输出动态规划结果
    cout << "numDecodings4: " << sol.numDecodings4(input) << endl; // 输出空间优化动态规划结果
    return 0;
}
