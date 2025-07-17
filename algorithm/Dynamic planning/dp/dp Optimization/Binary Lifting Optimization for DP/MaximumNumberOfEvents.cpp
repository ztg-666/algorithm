#include <vector>
#include <algorithm>

// 参加会议II
// 给定n个会议，每个会议有开始时间、结束时间、收益三个值
// 参加会议就能得到收益，但是同一时间只能参加一个会议
// 一共能参加k个会议，如果选择参加某个会议，那么必须完整的参加完这个会议
// 会议结束日期是包含在会议内的，一个会议的结束时间等于另一个会议的开始时间，不能两个会议都参加
// 返回能得到的会议价值最大和
// 1 <= n * k <= 10^6
// 1 <= 开始时间、结束时间 <= 10^9
// 1 <= 收益 <= 10^6
// 测试链接 : https://leetcode.cn/problems/maximum-number-of-events-that-can-be-attended-ii/

using namespace std;

class Solution {
public:
    // 参加会议II - 动态规划解法
    // events[i][0] : 开始时间
    // events[i][1] : 结束时间
    // events[i][2] : 收益
    int maxValue(vector<vector<int>>& events, int k) {
        int n = events.size();

        // 按结束时间升序排序，这样可以确保当前会议结束后，后续的会议都在时间上不冲突
        sort(events.begin(), events.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];
        });

        // dp[i][j] : 在前i+1个会议中（0到i），最多选择j个会议参加能获得的最大收益
        // 状态转移：对于第i个会议，可以选择参加或不参加
        vector<vector<int>> dp(n, vector<int>(k + 1, 0));

        // 初始化：只有第一个会议可选时，选择任意数量的会议（>=1）都只能选第一个会议
        for (int j = 1; j <= k; j++) {
            dp[0][j] = events[0][2];
        }

        // 填充dp表，遍历每个会议
        for (int i = 1; i < n; i++) {
            // 找到在当前会议开始之前结束的最后一个会议的索引
            // 这是为了确保会议时间不冲突（题目要求结束时间等于开始时间也算冲突）
            int pre = find(events, i - 1, events[i][0]);

            // 遍历所有可能的会议数量
            for (int j = 1; j <= k; j++) {
                // 状态转移方程：
                // dp[i][j] = max(不选第i个会议, 选择第i个会议)
                // 不选第i个会议：dp[i-1][j]
                // 选择第i个会议：(pre == -1 ? 0 : dp[pre][j-1]) + events[i][2]
                //   - 如果pre == -1，说明没有可以在当前会议前结束的会议，收益为0
                //   - 否则，在前pre个会议中选择j-1个会议的最大收益 + 当前会议收益
                dp[i][j] = max(dp[i - 1][j],
                              (pre == -1 ? 0 : dp[pre][j - 1]) + events[i][2]);
            }
        }

        // 返回在所有会议中选择k个会议的最大收益
        return dp[n - 1][k];
    }

private:
    // 二分查找函数
    // 在已按结束时间排序的events的0到i范围内，找到结束时间严格小于s的最右边的会议编号
    // 参数：events - 会议数组，i - 搜索范围的右边界，s - 目标开始时间
    // 返回：符合条件的最右边会议的索引，如果不存在则返回-1
    int find(const vector<vector<int>>& events, int i, int s) {
        int l = 0;      // 左边界
        int r = i;      // 右边界
        int mid;        // 中点
        int ans = -1;   // 结果，初始化为-1表示未找到

        // 标准二分查找
        while (l <= r) {
            mid = (l + r) / 2;

            // 如果当前会议的结束时间严格小于目标开始时间s
            if (events[mid][1] < s) {
                ans = mid;      // 更新答案
                l = mid + 1;    // 在右半部分继续搜索更大的索引
            } else {
                r = mid - 1;    // 在左半部分搜索
            }
        }

        return ans;
    }
};

/*
算法思路总结：
1. 将会议按结束时间排序，便于处理时间冲突
2. 使用动态规划，dp[i][j]表示前i+1个会议中选择j个会议的最大收益
3. 对于每个会议，考虑选择或不选择两种情况
4. 使用二分查找快速找到与当前会议不冲突的前一个会议
5. 时间复杂度：O(n²k)，空间复杂度：O(nk)

注意事项：
- 会议结束时间等于另一个会议开始时间时，两个会议不能都参加
- 必须完整参加会议，不能部分参加
- 同一时间只能参加一个会议
*/
