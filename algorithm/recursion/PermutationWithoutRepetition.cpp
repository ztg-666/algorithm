#include <vector>
#include <unordered_set>
#include <iostream>

using namespace std;
using namespace std;

class PermutationWithoutRepetition {
public:
    // 主函数，返回数组 nums 的所有不重复全排列
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> ans; // 用于存储所有不重复的全排列结果
        f(nums, 0, ans); // 调用递归函数 f 从索引 0 开始生成排列
        return ans; // 返回结果
    }

private:
    // 递归函数，生成从索引 i 开始的所有排列
    void f(vector<int>& nums, int i, vector<vector<int>>& ans) {
        if (i == nums.size()) {
            // 当索引 i 达到数组长度时，表示生成了一个完整的排列
            ans.push_back(nums); // 将当前排列添加到结果集中
        } else {
            unordered_set<int> set; // 用于记录当前层已经使用过的数字，避免重复排列
            for (int j = i; j < nums.size(); j++) {
                // 如果 nums[j] 没有在当前层使用过，则进行递归
                if (set.find(nums[j]) == set.end()) {
                    set.insert(nums[j]); // 记录 nums[j] 已经使用过
                    swap(nums, i, j); // 交换 nums[i] 和 nums[j]，生成新的排列
                    f(nums, i + 1, ans); // 递归调用，继续生成下一个位置的排列
                    swap(nums, i, j); // 回溯，恢复原数组状态
                }
            }
        }
    }

    // 辅助函数，交换数组 nums 中索引 i 和 j 的元素
    void swap(vector<int>& nums, int i, int j) {
        int tmp = nums[i]; // 暂存 nums[i]
        nums[i] = nums[j]; // 将 nums[j] 赋值给 nums[i]
        nums[j] = tmp; // 将暂存的 nums[i] 赋值给 nums[j]
    }
};

int main() {
    PermutationWithoutRepetition solution;
    vector<int> nums = {1, 1, 2}; // 示例输入数组
    vector<vector<int>> result = solution.permuteUnique(nums);

    // 打印所有不重复的全排列
    for (const auto& perm : result) {
        for (int num : perm) {
            cout << num << " ";
        }
        cout << endl;
    }

    return 0;
}
