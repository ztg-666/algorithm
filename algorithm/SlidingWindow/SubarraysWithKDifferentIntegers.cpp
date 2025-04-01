#include <iostream>
#include <vector>
#include <algorithm>
// K个不同整数的子数组
// 给定一个正整数数组 nums和一个整数 k，返回 nums 中 「好子数组」 的数目。
// 如果 nums 的某个子数组中不同整数的个数恰好为 k
// 则称 nums 的这个连续、不一定不同的子数组为 「好子数组 」。
// 例如，[1,2,3,1,2] 中有 3 个不同的整数：1，2，以及 3。
// 子数组 是数组的 连续 部分。
class Solution {
public:
    static int subarraysWithKDistinct(const std::vector<int>& arr, int k) {
        // 计算最多包含 k 个不同整数的子数组数量减去最多包含 k-1 个不同整数的子数组数量
        return numsOfMostKinds(arr, k) - numsOfMostKinds(arr, k - 1);
    }

private:
    static const int MAXN = 20001; // 定义一个常量 MAXN，表示数组的最大长度
    static int cnts[MAXN]; // 定义一个静态数组 cnts，用于存储每个数字的计数

    // arr中有多少子数组，数字种类不超过k
    // arr的长度是n，arr里的数值1~n之间
    static int numsOfMostKinds(const std::vector<int>& arr, int k) {
        // 初始化 cnts 数组，将所有元素设置为 0
        std::fill(cnts + 1, cnts + arr.size() + 1, 0);
        int ans = 0; // 用于存储符合条件的子数组数量
        for (int l = 0, r = 0, collect = 0; r < arr.size(); r++) {
            // 如果当前字符的计数增加到 1，则增加不同整数的数量
            if (++cnts[arr[r]] == 1) {
                collect++;
            }
            // 如果不同整数的数量超过 k，则移动左指针 l，直到不同整数的数量不超过 k
            while (collect > k) {
                // 如果当前字符的计数减少到 0，则减少不同整数的数量
                if (--cnts[arr[l++]] == 0) {
                    collect--;
                }
            }
            // 更新符合条件的子数组数量
            ans += r - l + 1;
        }
        return ans;
    }
};

// 初始化静态数组 cnts
int Solution::cnts[Solution::MAXN] = {0};

int main() {
    std::vector<int> arr = {1, 2, 1, 2, 3}; // 测试数组
    int k = 2; // 目标不同整数的数量
    // 输出结果
    std::cout << "Number of subarrays with exactly " << k << " distinct integers is: "
              << Solution::subarraysWithKDistinct(arr, k) << std::endl;
    return 0;
}
