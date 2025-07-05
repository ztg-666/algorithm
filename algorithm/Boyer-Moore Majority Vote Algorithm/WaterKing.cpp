#include <vector>
#include <iostream>
// 出现次数大于n/2的数
// 给定一个大小为n的数组nums
// 水王数是指在数组中出现次数大于n/2的数
// 返回其中的水王数，如果数组不存在水王数返回-1
// 测试链接 : https://leetcode.cn/problems/majority-element/
class Solution {
public:
    /**
     * @brief 查找出现次数大于 n/2 的数（水王数）。
     * @param nums 输入的整数向量。
     * @return 如果存在水王数，则返回该数；否则返回 -1。
     */
    int majorityElement(const std::vector<int>& nums) {
        // 如果数组为空，没有水王数
        if (nums.empty()) {
            return -1;
        }

        int cand = 0;
        int hp = 0; // hp (Health Points) 可以理解为候选者的“血量”或“计数器”

        // 阶段一：选举候选者 (Boyer-Moore Voting Algorithm)
        for (int num : nums) {
            if (hp == 0) {
                // 如果没有候选者（或者前一个候选者血量耗尽），当前数成为新的候选者
                cand = num;
                hp = 1;
            } else if (num != cand) {
                // 遇到一个不同的数，候选者血量减一
                hp--;
            } else { // num == cand
                // 遇到相同的数，候选者血量加一
                hp++;
            }
        }

        // 如果遍历结束后血量为 0，意味着没有一个明确的候选者胜出。
        // 例如数组 [1, 2, 3, 4]，遍历完后 hp 会是 0。
        // 这种情况下，不可能存在水王数，直接返回 -1。
        if (hp == 0) {
            return -1;
        }

        // 阶段二：验证候选者
        // 选举阶段只保证了：如果存在水王数，那么它一定是 cand。
        // 但 cand 不一定是水王数，例如数组 [1, 1, 2, 2, 3]，选举出的 cand 是 3。
        // 所以需要验证 cand 的真实出现次数。

        // 复用 hp 变量来统计真实出现次数
        hp = 0;
        for (int num : nums) {
            if (num == cand) {
                hp++;
            }
        }

        // 如果 cand 的出现次数大于数组大小的一半，它就是水王数
        // 注意：C++ 中整数除法 `nums.size() / 2` 会自动向下取整，行为正确。
        return hp > nums.size() / 2 ? cand : -1;
    }
};

// --- 测试代码 ---
void test(const std::vector<int>& nums, int expected) {
    Solution sol;
    int result = sol.majorityElement(nums);
    std::cout << "Array: [";
    for (size_t i = 0; i < nums.size(); ++i) {
        std::cout << nums[i] << (i == nums.size() - 1 ? "" : ", ");
    }
    std::cout << "], Majority Element: " << result;
    std::cout << (result == expected ? " (Correct)" : " (Incorrect)") << std::endl;
}

int main() {
    test({3, 2, 3}, 3);                                     // LeetCode 示例 1
    test({2, 2, 1, 1, 1, 2, 2}, 2);                         // LeetCode 示例 2
    test({1}, 1);                                           // 单个元素
    test({1, 2, 1, 2, 3}, -1);                              // 不存在水王数，选举出的 cand 是 3
    test({1, 2, 1, 2}, -1);                                 // 不存在水王数，选举阶段结束后 hp=0
    test({}, -1);                                           // 空数组
    return 0;
}
