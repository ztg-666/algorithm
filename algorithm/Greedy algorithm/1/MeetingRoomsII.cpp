#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int minMeetingRooms(vector<vector<int>>& intervals) {
    // 处理空输入情况
    if (intervals.empty()) return 0;

    // 将会议按开始时间升序排序
    // 排序目的：按时间顺序处理会议，保证每次处理的是最早开始的会议
    sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[0] < b[0]; // 比较会议开始时间
    });

    // 创建最小堆维护正在使用中的会议室结束时间
    // 堆顶始终保存最早结束的会议室时间
    priority_queue<int, vector<int>, greater<int>> minHeap;

    // 记录所需的最大会议室数量
    int maxRooms = 0;

    // 遍历所有会议
    for (const auto& meeting : intervals) {
        int start = meeting[0];   // 当前会议开始时间
        int end = meeting[1];     // 当前会议结束时间

        // 释放所有已结束的会议室（结束时间 <= 当前会议开始时间）
        // 这个循环确保堆中只保留与当前会议时间重叠的会议室
        while (!minHeap.empty() && minHeap.top() <= start) {
            minHeap.pop(); // 移除可用会议室
        }

        // 将当前会议的结束时间加入堆中
        // 表示分配一个新会议室（可能是复用释放的，也可能是新增的）
        minHeap.push(end);

        // 更新所需的最大会议室数量
        // 堆的大小即表示当前正在同时进行的会议数量
        maxRooms = max(maxRooms, static_cast<int>(minHeap.size()));
    }

    return maxRooms;
}
