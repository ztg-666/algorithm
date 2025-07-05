#include <vector>
#include <iostream>
// 划分左右使其水王数相同
// 给定一个大小为n的数组nums
// 水王数是指在数组中出现次数大于n/2的数
// 返回其中的一个划分点下标，使得左侧水王数等于右侧水王数
// 如果数组不存在这样的划分返回-1
// 测试链接 : https://leetcode.cn/problems/minimum-index-of-a-valid-split/

class Solution {
public:

    // C++ 中，List<Integer> 对应 std::vector<int>
    // 使用 const std::vector<int>& nums 作为参数可以避免复制数组，效率更高
    int minimumIndex(const std::vector<int>& nums) {
        // 1. 找出整个数组的水王数候选 (Boyer-Moore 投票算法)
        int cand = 0;
        int hp = 0; // health points
        for (int num : nums) {
            if (hp == 0) {
                cand = num;
                hp = 1;
            } else if (cand == num) {
                hp++;
            } else {
                hp--;
            }
        }

        // 2. 统计候选 cand 在整个数组中的总出现次数
        hp = 0; // 重置hp用作计数器
        for (int num : nums) {
            if (num == cand) {
                hp++;
            }
        }

        int n = nums.size();

        // 如果候选者本身就不是整个数组的水王数，那么不可能有任何有效划分
        // 尽管原始代码没有这一步，但加上可以提前终止。
        // 但为了忠实于原代码逻辑，我们将其省略，因为循环内的判断已经包含了这个逻辑。

        // 3. 遍历所有可能的划分点
        // lc : 水王数在左侧出现的词频 (left count)
        // rc : 水王数在右侧出现的词频 (right count)
        for (int i = 0, lc = 0, rc = hp; i < n - 1; ++i) {
            // 将 nums[i] 从右侧移动到左侧
            if (nums[i] == cand) {
                lc++;
                rc--;
            }

            // 检查当前划分点 i 是否有效
            // 左侧部分为 [0...i]，长度为 i + 1
            // 右侧部分为 [i+1...n-1]，长度为 n - i - 1
            // 判断 cand 是否分别为左右两边的水王数
            // lc > (i + 1) / 2  等价于 lc * 2 > i + 1
            // rc > (n - i - 1) / 2 等价于 rc * 2 > n - i - 1
            if (lc > (i + 1) / 2 && rc > (n - i - 1) / 2) {
                // 找到了满足条件的最小划分点，直接返回
                return i;
            }
        }

        // 遍历结束都没有找到，说明不存在这样的划分点
        return -1;
    }
};

// ==================== 用于测试的主函数 (可选) ====================
void test(const std::string& test_name, const std::vector<int>& nums, int expected) {
    Solution sol;
    int result = sol.minimumIndex(nums);
    if (result == expected) {
        std::cout << test_name << " PASSED." << std::endl;
    } else {
        std::cout << test_name << " FAILED. Expected: " << expected << ", Got: " << result << std::endl;
    }
}

int main() {
    test("Test Case 1", {2, 1, 2, 2, 1, 1, 2, 2, 1}, -1);
    test("Test Case 2", {1, 2, 2, 2, 1, 2, 2, 1, 2}, 2);
    test("Test Case 3", {3, 3, 3, 3, 3}, 0);
    test("Test Case 4", {1, 2, 3, 4, 5}, -1);
    test("Test Case 5", {1, 1, 1, 1, 1}, 0);
    test("Test Case 6", {6, 6, 6, 6, 6, 6}, 0);
    return 0;
}
