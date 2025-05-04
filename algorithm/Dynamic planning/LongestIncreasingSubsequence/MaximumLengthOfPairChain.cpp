#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;
// 最长数对链
// 给你一个由n个数对组成的数对数组pairs
// 其中 pairs[i] = [lefti, righti] 且 lefti < righti
// 现在，我们定义一种 跟随 关系，当且仅当 b < c 时
// 数对 p2 = [c, d] 才可以跟在 p1 = [a, b] 后面
// 我们用这种形式来构造 数对链
// 找出并返回能够形成的最长数对链的长度
// 测试链接 : https://leetcode.cn/problems/maximum-length-of-pair-chain/
class Solution {
public:
    // 最长数对链：动态规划 + 二分优化版本
    int findLongestChain(vector<vector<int>>& pairs) {
        // 将所有数对按照第一个元素从小到大排序
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0];
        });

        vector<int> ends; // 保存当前各长度下最小的结束值

        for (auto& p : pairs) {
            // 查找第一个大于等于当前 pair[0] 的位置
            int idx = binarySearch(ends, p[0]);
            if (idx == -1) {
                // 没有找到可接的位置，扩展最长长度
                ends.push_back(p[1]);
            } else {
                // 找到了可以替换的位置，更新该长度下的最小结尾值
                ends[idx] = min(ends[idx], p[1]);
            }
        }

        return ends.size(); // 当前数组长度即为最长链长度
    }

    // 手动实现的二分查找：寻找第一个 >= target 的位置
    int binarySearch(const vector<int>& ends, int target) {
        int left = 0;
        int right = ends.size() - 1;
        int ans = -1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (ends[mid] >= target) {
                ans = mid;      // 记录可能的答案
                right = mid - 1; // 向左继续查找更优解
            } else {
                left = mid + 1; // 向右查找
            }
        }
        return ans;
    }

    // 最优贪心解法（推荐）：按数对的右端点升序排序，每次尽可能选择更小的结尾
    int findLongestChain2(vector<vector<int>>& pairs) {
        // 按照第二个元素升序排列
        sort(pairs.begin(), pairs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        int pre = INT_MIN; // 上一个选择的数对的结尾值
        int ans = 0;       // 最长链长度

        for (auto& p : pairs) {
            // 如果当前数对的起始值 > 上一个数对的结束值，则可以连接
            if (pre < p[0]) {
                pre = p[1]; // 更新为当前数对的结束值
                ans++;      // 链长度+1
            }
        }

        return ans;
    }
};
#include <iostream>

// 假设 Solution 类已经定义在上面

int main() {
    vector<vector<int>> pairs = {{1, 2}, {2, 3}, {3, 4}};

    Solution solution;

    // 调用动态规划 + 二分优化版本
    int result1 = solution.findLongestChain(pairs);
    cout << "1: " << result1 << endl;

    // 调用贪心算法版本
    int result2 = solution.findLongestChain2(pairs);
    cout << "2: " << result2 << endl;

    return 0;
}