#include <unordered_map>
#include <vector>
#include <iostream>
// 返回无序数组中累加和为给定值的子数组个数
class NumberOfSubarraySumEqualsAim {
public:
    static int subarraySum(const std::vector<int>& nums, int aim) {
        // 使用哈希表存储前缀和出现的次数
        std::unordered_map<int, int> map;
        // 0这个前缀和，在没有任何数字的时候，已经有1次了
        map[0] = 1;
        int ans = 0; // 记录满足条件的子数组个数
        int sum = 0; // 当前前缀和

        // 遍历数组中的每个元素
        for (int i = 0; i < nums.size(); i++) {
            // 更新当前前缀和
            sum += nums[i];
            // 如果存在一个前缀和为 sum - aim，则说明从该前缀和到当前前缀和之间的子数组和为 aim
            if (map.find(sum - aim) != map.end()) {
                ans += map[sum - aim];
            }
            // 更新当前前缀和出现的次数
            map[sum]++;
        }
        return ans; // 返回满足条件的子数组个数
    }
};


int main() {
    // 测试用例
    std::vector<int> nums = {1, 1, 1};
    int aim = 2;

    // 调用 subarraySum 方法
    int result = NumberOfSubarraySumEqualsAim::subarraySum(nums, aim);

    // 输出结果
    std::cout << "Number of subarrays with sum equals to " << aim << " is: " << result << std::endl;

    return 0;
}