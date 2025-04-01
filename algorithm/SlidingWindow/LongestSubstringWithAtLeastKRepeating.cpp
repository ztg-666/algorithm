#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// 至少有K个重复字符的最长子串
// 给你一个字符串 s 和一个整数 k ，请你找出 s 中的最长子串
// 要求该子串中的每一字符出现次数都不少于 k 。返回这一子串的长度
// 如果不存在这样的子字符串，则返回 0。
class Solution {
public:
    static int longestSubstring(const std::string& str, int k) {
        std::vector<int> cnts(256, 0); // 创建一个大小为 256 的向量 cnts，用于存储每个字符的计数
        int n = str.size(); // 获取字符串的长度
        int ans = 0; // 用于存储最长子串的长度

        // 每次要求子串必须含有 require 种字符，每种字符都必须 >= k 次，这样的最长子串是多长
        for (int require = 1; require <= 26; require++) {
            std::fill(cnts.begin(), cnts.end(), 0); // 初始化 cnts 数组，将所有元素设置为 0
            // collect : 窗口中一共收集到的种类数
            // satisfy : 窗口中达标的种类数(次数 >= k)
            for (int l = 0, r = 0, collect = 0, satisfy = 0; r < n; r++) {
                cnts[str[r]]++; // 增加当前字符的计数
                if (cnts[str[r]] == 1) {
                    collect++; // 如果当前字符的计数增加到 1，则增加不同字符的数量
                }
                if (cnts[str[r]] == k) {
                    satisfy++; // 如果当前字符的计数增加到 k，则增加满足至少 k 次的字符数量
                }
                // l....r 种类超了！
                // l位置的字符，窗口中吐出来！
                while (collect > require) {
                    if (cnts[str[l]] == 1) {
                        collect--; // 如果当前字符的计数减少到 1，则减少不同字符的数量
                    }
                    if (cnts[str[l]] == k) {
                        satisfy--; // 如果当前字符的计数减少到 k，则减少满足至少 k 次的字符数量
                    }
                    cnts[str[l++]]--; // 减少当前字符的计数并移动左指针 l
                }
                // l.....r : 子串以 r 位置的字符结尾，且种类数不超的，最大长度！
                if (satisfy == require) {
                    ans = std::max(ans, r - l + 1); // 更新最长子串的长度
                }
            }
        }
        return ans; // 返回最长子串的长度
    }
};

int main() {
    std::string s = "aaabb"; // 测试字符串
    int k = 3; // 目标字符出现次数
    // 输出结果
    std::cout << "The length of the longest substring with at least " << k << " repeating characters is: "
              << Solution::longestSubstring(s, k) << std::endl;
    return 0;
}
