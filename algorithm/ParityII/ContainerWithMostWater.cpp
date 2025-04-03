#include<bits/stdc++.h>
using namespace std;

// 盛最多水的容器
// 给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。
// 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水
// 返回容器可以储存的最大水量
// 说明：你不能倾斜容器
// 时间复杂度O(n)，额外空间复杂度O(1)
int maxArea(std::vector<int>& height) {
    int ans = 0; // 初始化最大面积为0

    // 使用双指针法，l从左向右，r从右向左
    for (int l = 0, r = height.size() - 1; l < r;) {
        // 计算当前两线段之间的面积，并更新最大面积
        ans = std::max(ans, std::min(height[l], height[r]) * (r - l));

        // 移动较短的线段指针，以尝试找到更大的面积
        if (height[l] <= height[r]) {
            l++; // 如果左边的线段较短或相等，则移动左指针
        } else {
            r--; // 如果右边的线段较短，则移动右指针
        }
    }

    return ans; // 返回最大面积
}

int main() {
    // 示例输入
    std::vector<int> height = {1, 8, 6, 2, 5, 4, 8, 3, 7};

    // 调用maxArea函数并输出结果
    int result = maxArea(height);
    std::cout << "The maximum area is: " << result << std::endl;

    return 0;
}