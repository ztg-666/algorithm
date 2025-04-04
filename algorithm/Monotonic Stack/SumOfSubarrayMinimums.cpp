#include <vector>
#include <stack>
#include <iostream>
class SumOfSubarrayMinimums {
public:
    static const int MOD = 1000000007; // 定义模数，用于防止结果溢出

    static int sumSubarrayMins(std::vector<int>& arr) {
        long long ans = 0; // 用于存储最终结果，使用long long防止溢出
        std::stack<int> stack; // 用于存储数组索引的栈

        // 遍历数组中的每个元素
        for (int i = 0; i < arr.size(); i++) {
            // 当栈不为空且当前元素小于等于栈顶元素对应的数组值时
            while (!stack.empty() && arr[stack.top()] >= arr[i]) {
                int cur = stack.top(); // 获取栈顶元素的索引
                stack.pop(); // 弹出栈顶元素

                // 计算当前元素左边第一个小于它的元素的索引
                int left = stack.empty() ? -1 : stack.top();

                // 计算以当前元素为最小值的子数组的贡献
                ans = (ans + (long long)(cur - left) * (i - cur) * arr[cur]) % MOD;
            }
            // 将当前元素的索引压入栈中
            stack.push(i);
        }

        // 处理栈中剩余的元素
        while (!stack.empty()) {
            int cur = stack.top(); // 获取栈顶元素的索引
            stack.pop(); // 弹出栈顶元素

            // 计算当前元素左边第一个小于它的元素的索引
            int left = stack.empty() ? -1 : stack.top();

            // 计算以当前元素为最小值的子数组的贡献
            ans = (ans + (long long)(cur - left) * (arr.size() - cur) * arr[cur]) % MOD;
        }

        // 返回最终结果，转换为int类型
        return static_cast<int>(ans);
    }
};

int main() {
    // 示例数组
    std::vector<int> arr = {3, 1, 2, 4};

    // 调用 sumSubarrayMins 方法计算子数组最小值之和
    int result = SumOfSubarrayMinimums::sumSubarrayMins(arr);

    // 输出结果
    std::cout << "The sum of the minimums of all subarrays is: " << result << std::endl;

    return 0;
}