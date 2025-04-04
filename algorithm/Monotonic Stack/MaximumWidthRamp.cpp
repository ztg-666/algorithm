#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
// 最大宽度坡
// 给定一个整数数组 A，坡是元组 (i, j)，其中  i < j 且 A[i] <= A[j]
// 这样的坡的宽度为 j - i，找出 A 中的坡的最大宽度，如果不存在，返回 0
int maxWidthRamp(vector<int>& arr) {
    vector<int> stack;
    stack.push_back(0); // 初始化栈，放入第一个元素的索引
    
    // 构建单调递减栈
    // 遍历数组，将所有比栈顶元素小的元素的索引压入栈中
    for (int i = 1; i < arr.size(); i++) {
        if (arr[i] < arr[stack.back()]) {
            stack.push_back(i);
        }
    }

    int ans = 0;
    // 从后往前找最大坡
    // 遍历数组，从最后一个元素开始向前
    for (int j = arr.size() - 1; j >= 0; j--) {
        // 当栈不为空且当前元素大于等于栈顶元素对应的数组值时
        // 计算当前元素与栈顶元素的索引差，并更新最大坡度
        while (!stack.empty() && arr[j] >= arr[stack.back()]) {
            ans = max(ans, j - stack.back());
            stack.pop_back(); // 弹出栈顶元素
        }
    }
    return ans; // 返回最大坡度
}


int main() {
    vector<int> arr = {6,0,8,2,1,5};
    int result = maxWidthRamp(arr);
    cout << result << endl;
    return 0;
}
