#include <iostream>
#include <string>
#include <vector>
#include <climits>
// 替换子串得到平衡字符串
// 有一个只含有 'Q', 'W', 'E', 'R' 四种字符，且长度为 n 的字符串。
// 假如在该字符串中，这四个字符都恰好出现 n/4 次，那么它就是一个「平衡字符串」。
// 给你一个这样的字符串 s，请通过「替换一个子串」的方式，使原字符串 s 变成一个「平衡字符串」。
// 你可以用和「待替换子串」长度相同的 任何 其他字符串来完成替换。
// 请返回待替换子串的最小可能长度。
// 如果原字符串自身就是一个平衡字符串，则返回 0。
class Solution {
public:
    static int balancedString(const std::string& str) {
        int n = str.length();
        std::vector<int> s(n);
        std::vector<int> cnts(4, 0);

        // 将字符映射到索引 0, 1, 2, 3 对应 'Q', 'W', 'E', 'R'
        for (int i = 0; i < n; i++) {
            char c = str[i];
            if (c == 'W') {
                s[i] = 1;
            } else if (c == 'E') {
                s[i] = 2;
            } else if (c == 'R') {
                s[i] = 3;
            } else {
                s[i] = 0;
            }
            cnts[s[i]]++;
        }

        int debt = 0;
        // 计算每个字符的债务
        for (int i = 0; i < 4; i++) {
            if (cnts[i] < n / 4) {
                cnts[i] = 0;
            } else {
                cnts[i] = n / 4 - cnts[i];
                debt -= cnts[i];
            }
        }

        // 如果债务为0，说明字符串已经平衡，返回0
        if (debt == 0) {
            return 0;
        }

        int ans = INT_MAX;
        // 使用双指针法，l表示当前子串的起始位置，r表示当前子串的结束位置
        for (int l = 0, r = 0; r < n; r++) {
            // 窗口右边界向右，给出字符
            if (cnts[s[r]]++ < 0) {
                debt--;
            }
            // 当债务为0时，表示当前窗口包含了所有需要替换的字符
            if (debt == 0) {
                // 窗口左边界向右，拿回字符
                while (cnts[s[l]] > 0) {
                    cnts[s[l++]]--;
                }
                // 更新最小替换子串的长度
                ans = std::min(ans, r - l + 1);
            }
        }
        return ans;
    }
};

int main() {
    std::string s = "WWEQERQWQWWRQQQQ";
    std::cout << "The minimum length of the substring to replace is: "
              << Solution::balancedString(s) << std::endl;
    return 0;
}
