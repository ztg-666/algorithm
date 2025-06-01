#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// 会议必须独占时间段的最大会议数量
// 给定若干会议的开始、结束时间
// 你参加某个会议的期间，不能参加其他会议
// 返回你能参加的最大会议数量
// 同学找到了Leetcode的在线测试，题意类似
// 测试链接 :https://leetcode.cn/problems/non-overlapping-intervals/

using namespace std;

// 计算需要删除的最小会议数（原题转化为求最大不重叠会议数）
// 策略：按结束时间排序，使用贪心算法选择最早结束的会议
int eraseOverlapIntervals(vector<vector<int>>& meetings) {
    // 按会议结束时间升序排序
    sort(meetings.begin(), meetings.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });

    int ans = 0;
    // cur记录当前所选会议的结束时间，初始值小于最小可能值
    for (int i = 0, cur = -50001; i < meetings.size();) {
        if (cur <= meetings[i][0]) { // 找到不冲突的会议
            ans++;
            cur = meetings[i][1];    // 更新当前结束时间
        }
        i++;
    }
    // 总会议数 - 最大不重叠数 = 需要删除的最小数量
    return meetings.size() - ans;
}

// 交换会议数组中两个元素的位置
void swap(vector<vector<int>>& meetings, int i, int j) {
    vector<int> tmp = meetings[i];
    meetings[i] = meetings[j];
    meetings[j] = tmp;
}

// 暴力递归：全排列所有会议，计算最大不重叠数（用于验证）
// 参数：n-总会议数，i-当前处理到的位置
int f(vector<vector<int>>& meetings, int n, int i) {
    int ans = 0;
    if (i == n) { // 完成一种排列
        // 遍历排列后的会议，计算最大可参加数
        for (int j = 0, cur = -1; j < n;) {
            if (cur <= meetings[j][0]) {
                ans++;
                cur = meetings[j][1];
            }
            j++;
        }
    } else {
        // 生成所有排列可能性
        for (int j = i; j < n; j++) {
            swap(meetings, i, j);     // 交换产生新排列
            ans = max(ans, f(meetings, n, i + 1)); // 递归处理
            swap(meetings, i, j);     // 恢复原始排列
        }
    }
    return ans;
}

// 暴力解法入口：O(n!)
int maxMeeting1(vector<vector<int>>& meetings) {
    return f(meetings, meetings.size(), 0);
}

// 优化解法：贪心算法 O(nlogn)
int maxMeeting2(vector<vector<int>>& meetings) {
    // 按会议结束时间升序排序
    sort(meetings.begin(), meetings.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[1] < b[1];
    });

    int ans = 0;
    // cur记录当前所选会议的结束时间，初始值-1
    for (int i = 0, cur = -1; i < meetings.size();) {
        if (cur <= meetings[i][0]) { // 找到不冲突的会议
            ans++;
            cur = meetings[i][1];    // 更新当前结束时间
        }
        i++;
    }
    return ans;
}

// 生成随机会议数据
// 参数：n-会议数量，m-时间最大值
vector<vector<int>> randomMeeting(int n, int m) {
    vector<vector<int>> ans(n, vector<int>(2));
    for (int i = 0; i < n; i++) {
        int a = rand() % m;
        int b = rand() % m;
        if (a == b) { // 处理相同时间的情况
            ans[i][0] = a;
            ans[i][1] = a + 1; // 确保结束时间晚于开始时间
        } else {
            ans[i][0] = min(a, b); // 确保时间区间有效
            ans[i][1] = max(a, b);
        }
    }
    return ans;
}

int main() {
    srand(time(0)); // 初始化随机种子
    const int N = 10;   // 最大会议数量
    const int M = 12;   // 时间最大值
    const int testTimes = 2000; // 测试次数

    cout << "" << endl;
    for (int i = 1; i <= testTimes; i++) {
        int n = (rand() % N) + 1; // 随机生成1~N个会议
        vector<vector<int>> meeting = randomMeeting(n, M);

        // 验证暴力解与贪心解结果一致
        int ans1 = maxMeeting1(meeting);
        int ans2 = maxMeeting2(meeting);
        if (ans1 != ans2) {
            cout << "" << endl;
        }

        // 每100次输出进度
        if (i % 100 == 0) {
            cout << " " << i << " " << endl;
        }
    }
    cout << "" << endl;
    return 0;
}
