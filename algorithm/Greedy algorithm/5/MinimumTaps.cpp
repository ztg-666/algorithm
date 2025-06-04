#include <vector>
#include <algorithm> // 需要包含 algorithm 头文件用于 std::max

// 灌溉花园的最少水龙头数目
// 在x轴上有一个一维的花园，花园长度为n，从点0开始，到点n结束
// 花园里总共有 n + 1 个水龙头，分别位于[0, 1, ... n]
// 给你一个整数n和一个长度为n+1的整数数组ranges
// 其中ranges[i]表示
// 如果打开点i处的水龙头，可以灌溉的区域为[i-ranges[i], i+ranges[i]]
// 请你返回可以灌溉整个花园的最少水龙头数目
// 如果花园始终存在无法灌溉到的地方请你返回-1
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-taps-to-open-to-water-a-garden/

using namespace std;

int minTaps(int n, vector<int>& ranges) {
    vector<int> right(n + 1, 0); // 记录每个左边界能覆盖的最远右边界

    // 预处理每个左边界的最远覆盖范围
    for (int i = 0; i <= n; ++i) {
        int start = max(0, i - ranges[i]);
        right[start] = max(right[start], i + ranges[i]);
    }

    int cur = 0;    // 当前覆盖的最远右边界
    int next = 0;   // 下一步能覆盖的最远右边界
    int ans = 0;    // 打开水龙头数量

    for (int i = 0; i < n; ++i) { // 注意遍历到n-1即可
        next = max(next, right[i]); // 更新下一步能到达的最远位置

        if (i == cur) { // 到达当前覆盖边界
            if (next > i) { // 还能继续扩展
                cur = next;
                ans++;
            } else { // 无法覆盖后续区域
                return -1;
            }
        }
    }
    return cur >= n ? ans : -1; // 最终检查是否覆盖到终点
}
