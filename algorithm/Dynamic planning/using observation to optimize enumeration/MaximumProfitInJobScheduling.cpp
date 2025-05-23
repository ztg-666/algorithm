#include <vector>
#include <algorithm>
using namespace std;
// 规划兼职工作
// 你打算利用空闲时间来做兼职工作赚些零花钱，这里有n份兼职工作
// 每份工作预计从startTime[i]开始、endTime[i]结束，报酬为profit[i]
// 返回可以获得的最大报酬
// 注意，时间上出现重叠的 2 份工作不能同时进行
// 如果你选择的工作在时间X结束，那么你可以立刻进行在时间X开始的下一份工作
// 测试链接 : https://leetcode.cn/problems/maximum-profit-in-job-scheduling/


class Solution {
public:
    int jobScheduling(vector<int>& startTime, vector<int>& endTime, vector<int>& profit) {
        // 将输入数据转换为三元组数组（开始时间，结束时间，利润）
        // 便于后续统一排序处理
        int n = startTime.size();
        vector<vector<int>> jobs(n, vector<int>(3));
        for (int i = 0; i < n; ++i) {
            jobs[i][0] = startTime[i];   // 开始时间存入第一个位置
            jobs[i][1] = endTime[i];     // 结束时间存入第二个位置
            jobs[i][2] = profit[i];      // 利润存入第三个位置
        }

        // 关键排序策略：按工作结束时间升序排列
        // 排序目的：保证处理到第i个工作时，之前的所有工作都是按结束时间有序的
        // 这使得可以使用二分查找快速定位不冲突的前序工作
        sort(jobs.begin(), jobs.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[1] < b[1];  // 比较结束时间
        });

        // 动态规划数组：dp[i]表示考虑前i个工作时能获得的最大利润
        vector<int> dp(n);
        dp[0] = jobs[0][2]; // 初始化基准情况：只有第一个工作时，最大利润就是它本身的利润

        // 递推计算每个工作状态
        for (int i = 1; i < n; ++i) {
            int start = jobs[i][0];       // 当前工作开始时间
            int currentProfit = jobs[i][2]; // 当前工作的基础利润

            // 二分查找核心：在jobs[0...i-1]范围内
            // 寻找最后一个结束时间 <= 当前工作开始时间的工作
            int left = 0, right = i - 1;
            int best = -1; // 记录找到的最佳位置

            // 标准二分查找模板
            while (left <= right) {
                int mid = left + (right - left) / 2; // 防止整数溢出的中间值计算
                if (jobs[mid][1] <= start) {
                    best = mid;     // 记录有效候选位置
                    left = mid + 1; // 继续向右寻找更接近的候选
                } else {
                    right = mid - 1; // 向左收缩搜索范围
                }
            }

            // 如果找到不冲突的工作，叠加其最大利润
            if (best != -1) {
                currentProfit += dp[best]; // 累加前序工作的最大利润
            }

            // 状态转移方程：做出选择/不选择的决策
            // 选择当前工作：currentProfit（当前利润+前序利润）
            // 不选择当前工作：dp[i-1]（保持前一个状态的最大利润）
            dp[i] = max(currentProfit, dp[i-1]);
        }

        // 最终结果存储在最后一个元素中
        return dp[n-1];
    }
};