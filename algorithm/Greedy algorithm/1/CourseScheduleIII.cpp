#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// 课程表III
// 这里有n门不同的在线课程，按从1到n编号
// 给你一个数组courses
// 其中courses[i]=[durationi, lastDayi]表示第i门课将会持续上durationi天课
// 并且必须在不晚于lastDayi的时候完成
// 你的学期从第 1 天开始
// 且不能同时修读两门及两门以上的课程
// 返回你最多可以修读的课程数目
// 测试链接 : https://leetcode.cn/problems/course-schedule-iii/

int scheduleCourse(vector<vector<int>>& courses) {
    // 将课程按截止时间升序排序
    // 策略：优先处理截止时间早的课程，为后续课程留出更多时间
    sort(courses.begin(), courses.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];  // 按截止时间从早到晚排序
    });

    // 大根堆维护已选课程的持续时间（堆顶为最长持续时间）
    // 作用：快速获取可替换的候选课程（持续时间最长的课程）
    priority_queue<int> maxHeap;

    // 记录当前累计的学习时间
    int currentTime = 0;

    // 遍历所有课程（按截止时间顺序处理）
    for (const auto& course : courses) {
        int duration = course[0];   // 当前课程持续时间
        int deadline = course[1];   // 当前课程截止时间

        // 情况1：当前时间可以完成该课程
        if (currentTime + duration <= deadline) {
            maxHeap.push(duration);          // 选课（记录持续时间）
            currentTime += duration;         // 更新累计时间
        }
        // 情况2：时间冲突但存在优化空间
        else {
            // 只有当堆中有课程 且 当前课程比已选最长课程更优时进行替换
            // 替换条件：当前课程持续时间 < 已选课程最大持续时间
            if (!maxHeap.empty() && maxHeap.top() > duration) {
                // 替换策略：用当前课程替换持续时间最长的课程
                currentTime += duration - maxHeap.top();  // 更新时间差
                maxHeap.pop();              // 移除被替换课程
                maxHeap.push(duration);     // 加入当前课程
            }
        }
    }

    // 堆的大小即为最大可选课程数
    return maxHeap.size();
}

