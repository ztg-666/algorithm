#include <vector>
#include <iostream>
// 寻找重复数
// 给定一个包含 n + 1 个整数的数组 nums ，其数字都在 [1, n] 范围内（包括 1 和 n）
// 可知至少存在一个重复的整数。
// 假设 nums 只有 一个重复的整数 ，返回 这个重复的数 。
// 你设计的解决方案必须 不修改 数组 nums 且只用常量级 O(1) 的额外空间。
class FindTheDuplicateNumber {
public:
    // 时间复杂度O(n)，额外空间复杂度O(1)
    static int findDuplicate(const std::vector<int>& nums) {
        // 检查输入数组是否为空或长度小于2，如果是则返回-1
        if (nums.empty() || nums.size() < 2) {
            return -1;
        }

        // 初始化慢指针和快指针
        int slow = nums[0];
        int fast = nums[nums[0]];

        // 使用Floyd's Tortoise and Hare算法找到相遇点
        while (slow != fast) {
            slow = nums[slow];          // 慢指针每次移动一步
            fast = nums[nums[fast]];    // 快指针每次移动两步
        }

        // 相遇后，将快指针重置到数组起始位置
        fast = 0;

        // 找到入口点，即重复数字
        while (slow != fast) {
            fast = nums[fast];          // 快指针每次移动一步
            slow = nums[slow];          // 慢指针每次移动一步
        }

        // 返回重复数字
        return slow;
    }
};


int main() {
    // 示例数组，其中包含一个重复数字
    std::vector<int> nums = {1, 3, 4, 2, 2};

    // 调用findDuplicate方法并输出结果
    int duplicate = FindTheDuplicateNumber::findDuplicate(nums);
    std::cout << "The duplicate number is: " << duplicate << std::endl;

    return 0;
}