#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
// 无重复字符的最长子串
// 给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
class Solution {
public:
    static int lengthOfLongestSubstring(const std::string& str) {
        // 创建一个大小为256的向量，用于存储每个字符上次出现的位置，初始值为-1
        std::vector<int> last(256, -1);
        // 用于存储最长无重复字符子串的长度
        int ans = 0;
        // 字符串的长度
        int n = str.size();

        // 使用双指针法，l表示当前子串的起始位置，r表示当前子串的结束位置
        for (int l = 0, r = 0; r < n; r++) {
            // 更新左指针l的位置，确保子串中没有重复字符
            // last[str[r]] + 1 表示当前字符上次出现位置的下一个位置
            l = std::max(l, last[str[r]] + 1);
            // 更新最长无重复字符子串的长度
            ans = std::max(ans, r - l + 1);
            // 更新当前字符上次出现的位置为当前索引r
            last[str[r]] = r;
        }
        // 返回最长无重复字符子串的长度
        return ans;
    }
};

int main() {
    // 测试字符串
    std::string s = "abcabcbb";
    // 输出结果
    std::cout << "The length of the longest substring without repeating characters is: "
              << Solution::lengthOfLongestSubstring(s) << std::endl;
    return 0;
}
