#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

// 知识竞赛
// 最近部门要选两个员工去参加一个需要合作的知识竞赛，
// 每个员工均有一个推理能力值ai，以及一个阅读能力值bi
// 如果选择第i个人和第j个人去参加竞赛，
// 两人在推理方面的能力为X = (ai + aj)/2
// 两人在阅读方面的能力为Y = (bi + bj)/2
// 现在需要最大化他们表现较差一方面的能力
// 即让min(X,Y) 尽可能大，问这个值最大是多少
// 测试链接 : https://www.nowcoder.com/practice/2a9089ea7e5b474fa8f688eae76bc050

using namespace std;

// 自定义比较函数：按（推理能力-阅读能力）的绝对值升序排列
bool compare(const pair<int, int>& a, const pair<int, int>& b) {
    return abs(a.first - a.second) < abs(b.first - b.second);
}

int compute(vector<pair<int, int>>& nums) {
    // 按能力差绝对值排序，确保处理顺序最优
    sort(nums.begin(), nums.end(), compare);

    // 初始化最大推理和阅读能力值为第一个元素的值
    int maxA = nums[0].first;   // 维护左侧最大的推理能力
    int maxB = nums[0].second;  // 维护左侧最大的阅读能力
    int ans = 0;

    // 遍历后续元素寻找最佳配对
    for (int i = 1; i < nums.size(); ++i) {
        // 当推理能力<=阅读能力时，尝试用当前推理能力与左侧最大推理能力配对
        if (nums[i].first <= nums[i].second) {
            ans = max(ans, maxA + nums[i].first); // X = (maxA + a_i)/2
        }
        // 否则用当前阅读能力与左侧最大阅读能力配对
        else {
            ans = max(ans, maxB + nums[i].second); // Y = (maxB + b_i)/2
        }
        // 更新左侧最大值，确保后续比较使用最新最大值
        maxA = max(maxA, nums[i].first);
        maxB = max(maxB, nums[i].second);
    }
    return ans;
}

int main() {
    int n;
    // 处理多组测试数据
    while (scanf("%d", &n) != EOF) {
        vector<pair<int, int>> nums(n);
        // 读取每个员工的推理和阅读能力
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &nums[i].first, &nums[i].second);
        }
        // 计算结果并转换格式输出
        int ans = compute(nums);
        printf("%.1f\n", ans / 2.0); // 最终结果为整数除以2保留1位小数
    }
    return 0;
}
