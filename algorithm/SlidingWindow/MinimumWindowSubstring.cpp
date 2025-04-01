#include <iostream>
#include <string>
#include <climits>
#include <vector>
// 最小覆盖子串
// 给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串
// 如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。
class Solution {
public:
    static std::string minWindow(const std::string& str, const std::string& tar) {
        // 创建一个大小为256的向量，用于存储每个字符的欠债情况
        // cnts[i] = 负数，代表字符i有负债
        // cnts[i] = 正数，代表字符i有盈余
        std::vector<int> cnts(256, 0);
        for (char cha : tar) {
            cnts[cha]--;
        }
        // 最小覆盖子串的长度
        int len = INT_MAX;
        // 从哪个位置开头，发现的最小覆盖子串
        int start = 0;
        // 总债务
        int debt = tar.length();
        int n = str.size();

        // 使用双指针法，l表示当前子串的起始位置，r表示当前子串的结束位置
        for (int l = 0, r = 0; r < n; r++) {
            // 窗口右边界向右，给出字符
            // 如果当前字符在目标字符串中存在（即 cnts[str[r]] < 0），则减少债务
            if (cnts[str[r]]++ < 0) {
                debt--;
            }
            // 当债务为0时，表示当前窗口包含了目标字符串中的所有字符
            if (debt == 0) {
                // 窗口左边界向右，拿回字符
                // 移动左指针，直到窗口不再包含所有目标字符
                while (cnts[str[l]] > 0) {
                    cnts[str[l++]]--;
                }
                // 以r位置结尾的达标窗口，更新答案
                if (r - l + 1 < len) {
                    len = r - l + 1;
                    start = l;
                }
            }
        }
        // 如果没有找到符合条件的子串，返回空字符串
        return len == INT_MAX ? "" : str.substr(start, len);
    }
};

int main() {
    // 测试字符串
    std::string s = "ADOBECODEBANC";
    std::string t = "ABC";
    // 输出结果
    std::cout << "The minimum window substring is: "
              << Solution::minWindow(s, t) << std::endl;
    return 0;
}
