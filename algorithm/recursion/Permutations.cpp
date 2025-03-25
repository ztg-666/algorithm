#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Permutations {
public:
    // 主函数，生成给定数组的所有排列
    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> ans; // 用于存储所有排列的结果
        f(nums, 0, ans); // 调用递归函数开始生成排列
        return ans; // 返回所有排列的结果
    }

private:
    // 递归函数，生成从索引i开始的所有排列
    void f(vector<int>& nums, int i, vector<vector<int>>& ans) {
        // 如果索引i已经到达数组末尾，说明生成了一个完整的排列
        if (i == nums.size()) {
            ans.push_back(nums); // 将当前排列添加到结果中
        } else {
            // 遍历从i到数组末尾的所有元素
            for (int j = i; j < nums.size(); j++) {
                swap(nums[i], nums[j]); // 交换当前元素和第j个元素
                f(nums, i + 1, ans); // 递归生成剩余部分的排列
                swap(nums[i], nums[j]); // 还原交换，回溯到上一个状态
            }
        }
    }
};

int main() {
    Permutations solution; // 创建Permutations类的实例
    vector<int> nums = {1, 2, 3}; // 定义输入数组
    vector<vector<int>> ans = solution.permute(nums); // 调用permute函数生成所有排列
    // 遍历并打印所有排列
    for (const auto& list : ans) {
        for (int num : list) {
            cout << num << " "; // 打印当前排列中的每个元素
        }
        cout << endl; // 换行，分隔不同的排列
    }
    return 0; // 返回0，表示程序成功结束
}
