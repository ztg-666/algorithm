#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class Solution {
public:
    // 主函数：生成包含重复元素的数组的所有子集
    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        // 先对数组进行排序，方便后续去重
        sort(nums.begin(), nums.end());
        vector<vector<int>> ans; // 存储最终结果
        vector<int> path(nums.size()); // 用于构建当前路径（子集）
        // 调用递归函数生成所有子集
        f(nums, 0, path, 0, ans);
        return ans;
    }

    // 递归函数：生成子集的核心逻辑
    void f(vector<int>& nums, int i, vector<int>& path, int size, vector<vector<int>>& ans) {
        // 当遍历到数组末尾时，将当前路径加入结果集
        if (i == nums.size()) {
            vector<int> current(path.begin(), path.begin() + size); // 提取当前路径的有效部分
            ans.push_back(current); // 将当前子集加入结果集
            return;
        }

        // 找到下一个不同元素的位置
        int j = i + 1;
        while (j < nums.size() && nums[j] == nums[i]) {
            j++;
        }

        // 不选择当前元素的情况（跳过当前重复元素组）
        f(nums, j, path, size, ans);

        // 选择1到j-i个当前元素的情况（逐步增加当前重复元素的数量）
        int cnt = j - i; // 当前重复元素的数量
        for (int k = 0; k < cnt; ++k) {
            path[size + k] = nums[i]; // 将当前元素加入路径
            // 递归调用，继续处理剩余元素
            f(nums, j, path, size + k + 1, ans);
        }
    }
};
int main() {
    Solution solution;
    vector<int> nums = {1, 2, 2}; // 示例输入
    vector<vector<int>> result = solution.subsetsWithDup(nums);

    // 输出结果
    cout << "Subsets with duplicates:" << endl;
    for (const auto& subset : result) {
        cout << "{ ";
        for (int num : subset) {
            cout << num << " ";
        }
        cout << "}" << endl;
    }

    return 0;
}