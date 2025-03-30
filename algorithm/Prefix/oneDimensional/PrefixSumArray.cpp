#include<bits/stdc++.h>
using namespace std;

// 利用前缀和快速得到区域累加和
class NumArray {
public:
    // 前缀和数组，sum[i] 表示 nums[0] 到 nums[i-1] 的和
    std::vector<int> sum;

    // 构造函数，初始化前缀和数组
    NumArray(std::vector<int>& nums) {
        // 调整 sum 的大小为 nums.size() + 1
        sum.resize(nums.size() + 1);
        // 计算前缀和
        for (int i = 1; i <= nums.size(); i++) {
            sum[i] = sum[i - 1] + nums[i - 1];
        }
    }

    // 计算 nums[left] 到 nums[right] 的和
    int sumRange(int left, int right) {
        // 返回前缀和数组中对应区间的差值
        return sum[right + 1] - sum[left];
    }
};

int main() {
    // 示例数组
    std::vector<int> nums = {1, 2, 3, 4, 5};

    // 创建 NumArray 对象
    NumArray numArray(nums);

    // 测试 sumRange 方法
    int left = 1;
    int right = 3;
    int result = numArray.sumRange(left, right);

    // 输出结果
    cout << "Sum of elements from index " << left << " to " << right << " is: " << result << endl;

    // 可以添加更多的测试用例
    left = 0;
    right = 4;
    result = numArray.sumRange(left, right);
    cout << "Sum of elements from index " << left << " to " << right << " is: " << result << endl;

    return 0;
}
