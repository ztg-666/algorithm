#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <climits>

using namespace std;

// 使数组和能被P整除
// 给你一个正整数数组 nums，请你移除 最短 子数组（可以为 空）
// 使得剩余元素的 和 能被 p 整除。 不允许 将整个数组都移除。
// 请你返回你需要移除的最短子数组的长度，如果无法满足题目要求，返回 -1 。
// 子数组 定义为原数组中连续的一组元素。
class MakeSumDivisibleByP {
public:
    static int minSubarray(vector<int>& nums, int p) {
        // 计算整个数组的和对 p 的余数
        int mod = 0;
        for (int num : nums) {
            mod = (mod + num) % p;
        }
        // 如果整个数组的和已经能被 p 整除，直接返回 0
        if (mod == 0) {
            return 0;
        }

        // 使用哈希表存储前缀和对 p 的余数及其最晚出现的位置
        // key : 前缀和%p的余数
        // value : 最晚出现的位置
        unordered_map<int, int> map;
        map[0] = -1; // 初始化，表示前缀和为 0 的位置为 -1

        int ans = INT_MAX; // 初始化答案为最大值

        // 遍历数组，计算前缀和并对 p 取余
        for (int i = 0, cur = 0, find; i < nums.size(); i++) {
            // 计算当前前缀和对 p 的余数
            cur = (cur + nums[i]) % p;

            // 计算需要找到的前缀和的余数，使得移除的子数组和对 p 的余数为 mod
            find = cur >= mod ? (cur - mod) : (cur + p - mod);

            // 如果找到的余数存在于哈希表中，更新答案
            if (map.find(find) != map.end()) {
                ans = min(ans, i - map[find]);
            }

            // 更新当前前缀和的余数及其位置
            map[cur] = i;
        }

        // 如果答案仍然是最大值，说明没有找到合适的子数组，返回 -1
        // 否则返回答案
        return ans == nums.size() ? -1 : ans;
    }
};




int main() {
    // 测试用例 1
    vector<int> nums1 = {3, 1, 4, 2};
    int p1 = 6;
    cout << "Test Case 1: " << MakeSumDivisibleByP::minSubarray(nums1, p1) << endl; // 输出: 1

    // 测试用例 2
    vector<int> nums2 = {6, 3, 5, 2};
    int p2 = 9;
    cout << "Test Case 2: " << MakeSumDivisibleByP::minSubarray(nums2, p2) << endl; // 输出: 2

    // 测试用例 3
    vector<int> nums3 = {1, 2, 3};
    int p3 = 3;
    cout << "Test Case 3: " << MakeSumDivisibleByP::minSubarray(nums3, p3) << endl; // 输出: 0

    // 测试用例 4
    vector<int> nums4 = {1, 2, 3};
    int p4 = 7;
    cout << "Test Case 4: " << MakeSumDivisibleByP::minSubarray(nums4, p4) << endl; // 输出: -1

    return 0;
}
