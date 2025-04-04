#include <vector>
#include<iostream>
using namespace std;

// 每日温度
// 给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer
// 其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后
// 如果气温在这之后都不会升高，请在该位置用 0 来代替。
vector<int> dailyTemperatures(const vector<int>& nums) {
    int n = nums.size();
    vector<int> ans(n, 0); // 初始化结果数组，所有位置默认为0
    vector<int> stack; // 用于构建单调递减栈
    int r = 0; // 栈的当前大小

    // 遍历温度数组
    for (int i = 0; i < n; ++i) {
        // 当栈不为空且栈顶元素对应的温度小于当前温度时，弹出栈顶元素
        while (!stack.empty() && nums[stack.back()] < nums[i]) {
            int cur = stack.back(); // 当前弹出的位置
            stack.pop_back();
            // 计算当前弹出位置与当前温度之间的天数差
            ans[cur] = i - cur;
        }
        // 将当前温度的索引压入栈中
        stack.push_back(i);
    }

    // 返回结果数组
    return ans;
}

int main() {
    // 测试用例
    vector<int> temperatures = {73, 74, 75, 71, 69, 72, 76, 73};
    vector<int> result = dailyTemperatures(temperatures);

    // 输出结果
    for (int i = 0; i < result.size(); ++i) {
        cout << result[i] << " ";
    }
    cout << endl;

    return 0;
}
