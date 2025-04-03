#include <iostream>
#include <vector>
#include <algorithm>

// 找出第K小的数对距离
// 数对 (a,b) 由整数 a 和 b 组成，其数对距离定义为 a 和 b 的绝对差值。
// 给你一个整数数组 nums 和一个整数 k
// 数对由 nums[i] 和 nums[j] 组成且满足 0 <= i < j < nums.length
// 返回 所有数对距离中 第 k 小的数对距离。
class FindKthSmallestPairDistance {
public:
    // 时间复杂度O(n * log(n) + log(max-min) * n)，额外空间复杂度O(1)
    static int smallestDistancePair(std::vector<int>& nums, int k) {
        int n = nums.size();
        std::sort(nums.begin(), nums.end()); // 对数组进行排序
        int ans = 0;
        // [0, 最大-最小]，不停二分
        for (int l = 0, r = nums[n - 1] - nums[0], m, cnt; l <= r;) {
            // m中点，nums中任意两数的差值 <= m
            m = l + ((r - l) >> 1);
            // 返回数字对的数量
            cnt = f(nums, m);
            if (cnt >= k) {
                ans = m;
                r = m - 1;
            } else {
                l = m + 1;
            }
        }
        return ans;
    }

    // nums中任意两数的差值 <= limit
    // 这样的数字配对，有几对？
    static int f(const std::vector<int>& nums, int limit) {
        int ans = 0;
        // O(n)
        for (int l = 0, r = 0; l < nums.size(); l++) {
            // l......r r+1
            while (r + 1 < nums.size() && nums[r + 1] - nums[l] <= limit) {
                r++;
            }
            // nums[l...r]范围上的数差值的绝对值都不超过limit
            // nums[0...3]
            // 0,1
            // 0,2
            // 0,3
            ans += r - l;
        }
        return ans;
    }
};

int main() {
    std::vector<int> nums = {1, 3, 1}; // 示例输入
    int k = 1; // 示例输入

    // 调用smallestDistancePair函数
    int result = FindKthSmallestPairDistance::smallestDistancePair(nums, k);

    // 输出结果
    std::cout << "The " << k << "th smallest pair distance is: " << result << std::endl;

    return 0;
}
