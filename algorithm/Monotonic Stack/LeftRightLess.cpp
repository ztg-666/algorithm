#include <iostream>
#include <vector>
using namespace std;
// 单调栈求每个位置左右两侧，离当前位置最近、且值严格小于的位置
// 给定一个可能含有重复值的数组 arr
// 找到每一个 i 位置左边和右边离 i 位置最近且值比 arr[i] 小的位置
// 返回所有位置相应的信息。
// 输入描述：
// 第一行输入一个数字 n，表示数组 arr 的长度。
// 以下一行输入 n 个数字，表示数组的值
// 输出描述：
// 输出n行，每行两个数字 L 和 R，如果不存在，则值为 -1，下标从 0 开始。
// 测试链接 : https://www.nowcoder.com/practice/2a2c00e7a88a498693568cef63a4b7bb
// 计算每个位置左右两侧离当前位置最近且值严格小于的位置
vector<int> compute(const vector<int>& arr) {
    const int n = arr.size();
    vector<int> stack; // 用于构建单调递增栈
    vector<vector<int>> ans(n, vector<int>(2, -1)); // 存储每个位置的左右两侧最近且值严格小于的位置

    // 遍历阶段：构建单调递增栈
    for (int i = 0; i < n; ++i) {
        // 当栈不为空且栈顶元素大于等于当前元素时，弹出栈顶元素
        while (!stack.empty() && arr[stack.back()] >= arr[i]) {
            int cur = stack.back(); // 当前弹出的位置
            stack.pop_back();
            // 记录当前弹出位置的左侧最近且值严格小于的位置
            ans[cur][0] = !stack.empty() ? stack.back() : -1;
            // 记录当前弹出位置的右侧最近且值严格小于的位置
            ans[cur][1] = i;
        }
        // 将当前元素的索引压入栈中
        stack.push_back(i);
    }

    // 清算阶段：处理栈中剩余元素
    while (!stack.empty()) {
        int cur = stack.back(); // 当前弹出的位置
        stack.pop_back();
        // 记录当前弹出位置的左侧最近且值严格小于的位置
        ans[cur][0] = !stack.empty() ? stack.back() : -1;
        // 右侧位置设为-1，因为这些元素没有右侧最近且值严格小于的位置
        ans[cur][1] = -1;
    }

    // 修正阶段：处理右侧相等值情况
    // 从右往左修正，确保右侧答案的正确性
    for (int i = n - 2; i >= 0; --i) {
        // 如果当前元素的右侧位置不为-1且值相等，则更新右侧位置
        if (ans[i][1] != -1 && arr[i] == arr[ans[i][1]]) {
            ans[i][1] = ans[ans[i][1]][1];
        }
    }

    // 结果转换为一维数组输出
    vector<int> res;
    for (auto& p : ans) {
        res.push_back(p[0]); // 左侧最近且值严格小于的位置
        res.push_back(p[1]); // 右侧最近且值严格小于的位置
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false); // 提升输入输出效率
    cin.tie(nullptr); // 解除输入输出流的绑定

    int n;
    while (cin >> n) {
        vector<int> arr(n);
        for (int i = 0; i < n; ++i) {
            cin >> arr[i]; // 读取数组元素
        }
        vector<int> result = compute(arr); // 计算每个位置的左右两侧最近且值严格小于的位置
        for (int i = 0; i < n; ++i) {
            cout << result[2*i] << " " << result[2*i + 1] << "\n"; // 输出结果
        }
    }

    return 0;
}
