#include <vector>
#include <algorithm>

using namespace std;

// 俄罗斯套娃信封问题
// 给你一个二维整数数组envelopes ，其中envelopes[i]=[wi, hi]
// 表示第 i 个信封的宽度和高度
// 当另一个信封的宽度和高度都比这个信封大的时候
// 这个信封就可以放进另一个信封里，如同俄罗斯套娃一样
// 请计算 最多能有多少个信封能组成一组“俄罗斯套娃”信封
// 即可以把一个信封放到另一个信封里面，注意不允许旋转信封
// 测试链接 : https://leetcode.cn/problems/russian-doll-envelopes/
class Solution {
public:
    int maxEnvelopes(vector<vector<int>>& envelopes) {
        int n = envelopes.size();

        // 排序策略：
        // 1. 首先按照宽度升序排列
        // 2. 如果宽度相同，则按高度降序排列
        // 这样可以保证在处理高度时，宽度相同的信封不会互相影响（避免重复嵌套）
        sort(envelopes.begin(), envelopes.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] != b[0] ? a[0] < b[0] : a[1] > b[1];
        });

        // 使用一个数组 ends 来维护当前最长递增子序列的“尾部”元素
        // 类似于“最长递增子序列”问题中的贪心 + 二分优化方法
        vector<int> ends(n, 0);
        int len = 0; // 表示当前最长序列长度

        for (int i = 0; i < n; ++i) {
            int num = envelopes[i][1]; // 只关注高度，因为宽度已经排序过

            // 在 ends 数组中查找第一个 >= num 的位置
            // 如果找到，替换它（保持 ends 数组最小可能）
            // 否则，在末尾添加 num（扩展最长序列）
            int find = bs(ends, len, num);
            if (find == -1) {
                ends[len++] = num;
            } else {
                ends[find] = num;
            }
        }

        // 最终 len 就是能组成俄罗斯套娃的最大信封数量
        return len;
    }

    // 二分查找函数：在 ends[0...len-1] 中找第一个 >= num 的下标
    // 如果不存在，返回 -1
    int bs(vector<int>& ends, int len, int num) {
        int l = 0, r = len - 1, ans = -1;
        while (l <= r) {
            int m = (l + r) / 2;
            if (ends[m] >= num) {
                ans = m;
                r = m - 1; // 继续向左找更小的满足条件的位置
            } else {
                l = m + 1;
            }
        }
        return ans;
    }
};
#include <iostream>

int main() {
    Solution sol;

    // 示例输入：二维信封数组
    vector<vector<int>> envelopes = {
        {5, 4},
        {6, 4},
        {6, 7},
        {2, 3}
    };

    // 调用函数并输出结果
    int result = sol.maxEnvelopes(envelopes);
    cout << "最多可以嵌套的信封数量: " << result << endl;

    return 0;
}