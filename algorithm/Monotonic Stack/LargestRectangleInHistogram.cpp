#include <vector>
#include <algorithm> // 用于std::max函数
#include <iostream>
const int MAXN = 100001;       // 定义最大可能的柱子数量
int stack[MAXN]{};               // 用数组模拟栈结构
int r = 0;                         // 栈顶指针
// 柱状图中最大的矩形
// 给定 n 个非负整数，用来表示柱状图中各个柱子的高度
// 每个柱子彼此相邻，且宽度为 1 。求在该柱状图中，能够勾勒出来的矩形的最大面积
int largestRectangleArea(const std::vector<int>& height) {
    int n = height.size();
    r = 0;                            // 重置栈顶指针
    int ans = 0;                      // 存储最大面积结果

    // 第一轮遍历：计算每个柱子作为高度的右边界
    for (int i = 0; i < n; i++) {
        // 维护单调递增栈，遇到更矮柱子时计算面积
        while (r > 0 && height[stack[r - 1]] >= height[i]) {
            int cur = stack[--r];     // 弹出栈顶作为当前高度
            int left = r == 0 ? -1 : stack[r - 1]; // 左边界为栈中前一个元素
            ans = std::max(ans, height[cur] * (i - left - 1)); // 计算矩形面积
        }
        stack[r++] = i; // 当前索引入栈
    }

    // 第二轮处理：栈中剩余的柱子（右边界为总长度）
    while (r > 0) {
        int cur = stack[--r];         // 弹出栈顶
        int left = r == 0 ? -1 : stack[r - 1];
        ans = std::max(ans, height[cur] * (n - left - 1));
    }

    return ans;
}




int main() {
    std::vector<int> hist = {2, 1, 5, 6, 2, 3}; // 经典测试案例
    int result = largestRectangleArea(hist);
    std::cout << "max: " << result << std::endl; // 应输出10
    return 0;
}
