#include <iostream>
#include <vector>
#include <algorithm>

// 按奇偶排序数组II
// 给定一个非负整数数组 nums。nums 中一半整数是奇数 ，一半整数是偶数
// 对数组进行排序，以便当 nums[i] 为奇数时，i也是奇数
// 当 nums[i] 为偶数时， i 也是 偶数
// 你可以返回 任何满足上述条件的数组作为答案
class SortArrayByParityII {
public:
    // 时间复杂度O(n)，额外空间复杂度O(1)
    std::vector<int> sortArrayByParityII(std::vector<int>& nums) {
        int n = nums.size();

        // 初始化奇数索引和偶数索引
        for (int odd = 1, even = 0; odd < n && even < n;) {
            // 如果最后一个元素是奇数且当前奇数索引位置的元素不是奇数，则交换
            if ((nums[n - 1] & 1) == 1) {
                std::swap(nums[odd], nums[n - 1]);
                odd += 2; // 移动到下一个奇数索引位置
            } else {
                // 如果最后一个元素是偶数且当前偶数索引位置的元素不是偶数，则交换
                std::swap(nums[even], nums[n - 1]);
                even += 2; // 移动到下一个偶数索引位置
            }
        }
        return nums;
    }
};


int main() {
    // 创建一个示例数组
    std::vector<int> nums = {4, 2, 5, 7};

    // 创建 SortArrayByParityII 类的实例
    SortArrayByParityII sorter;

    // 调用 sortArrayByParityII 方法
    std::vector<int> sortedNums = sorter.sortArrayByParityII(nums);

    // 打印排序后的数组
    std::cout << "Sorted array by parity II: ";
    for (int num : sortedNums) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
