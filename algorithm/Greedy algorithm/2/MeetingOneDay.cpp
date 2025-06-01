#include <vector>
#include <algorithm>
#include <queue>

// 会议只占一天的最大会议数量
// 给定若干会议的开始、结束时间
// 任何会议的召开期间，你只需要抽出1天来参加
// 但是你安排的那一天，只能参加这个会议，不能同时参加其他会议
// 返回你能参加的最大会议数量
// 测试链接 : https://leetcode.cn/problems/maximum-number-of-events-that-can-be-attended/

using namespace std;

/**
 * 计算可以参加的最大会议数量（贪心算法）
 * 策略：每天优先选择最早结束的会议，为后续会议留出更多时间
 * @param events 会议时间数组，每个元素格式为 [start_day, end_day]
 * @return 最多可参加的会议数量
 */
int maxEvents(vector<vector<int>>& events) {
    int n = events.size();
    if (n == 0) return 0;

    // 按会议开始时间升序排序（便于按天顺序处理）
    sort(events.begin(), events.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0];
    });

    // 确定时间范围（只需处理存在会议的时间段）
    int min_day = events[0][0];
    int max_day = events[0][1];
    for (int i = 1; i < n; i++) {
        if (events[i][1] > max_day) {
            max_day = events[i][1];
        }
    }

    // 最小堆存储当前可参加会议的结束时间（使用小根堆快速获取最早结束会议）
    priority_queue<int, vector<int>, greater<int>> heap;
    int ans = 0;
    int event_idx = 0;  // 遍历events数组的指针

    // 遍历所有可能的天数（从最早到最晚的会议日期）
    for (int day = min_day; day <= max_day; day++) {
        // 将当天开始的会议加入候选队列
        while (event_idx < n && events[event_idx][0] == day) {
            heap.push(events[event_idx++][1]);  // 存储结束时间用于后续判断
        }

        // 清理已过期的会议（结束时间早于当前天的无法参加）
        while (!heap.empty() && heap.top() < day) {
            heap.pop();
        }

        // 贪心选择：参加当前最早结束的会议（堆顶元素）
        if (!heap.empty()) {
            heap.pop();  // 移除已选会议
            ans++;       // 增加计数
        }
    }
    return ans;
}
