#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 定义最大队列长度
int MAXN = 50001;

// 辅助函数，判断在给定条件下是否可以完成m个任务
// tasks: 任务数组，已排序
// tl: 任务数组的起始索引
// tr: 任务数组的结束索引
// workers: 工人类数组，已排序
// wl: 工人类数组的起始索引
// wr: 工人类数组的结束索引
// s: 药丸增加的力量值
// pills: 可用的药丸数量
bool f(const vector<int>& tasks, int tl, int tr, const vector<int>& workers, int wl, int wr, int s, int pills) {
    int deque[50001]; // 双端队列，用于存储可以完成的任务索引
    int h = 0, t = 0; // 队列的头尾指针
    int cnt = 0; // 已使用的药丸数量

    // 遍历每个工人
    for (int i = wl, j = tl; i <= wr; i++) {
        // 尝试分配任务给当前工人，不使用药丸
        for (; j <= tr && tasks[j] <= workers[i]; j++) {
            deque[t++] = j; // 将可完成的任务索引加入队列
        }

        // 如果队列不为空且队首任务可以由当前工人完成，则分配该任务
        if (h < t && tasks[deque[h]] <= workers[i]) {
            h++; // 移动队首指针
        } else {
            // 尝试分配任务给当前工人，使用药丸
            for (; j <= tr && tasks[j] <= workers[i] + s; j++) {
                deque[t++] = j; // 将可完成的任务索引加入队列
            }

            // 如果队列不为空，则分配队首任务并使用药丸
            if (h < t) {
                cnt++; // 增加药丸使用计数
                t--; // 移动队尾指针
            } else {
                return false; // 无法分配任务，返回false
            }
        }
    }
    return cnt <= pills; // 检查使用的药丸数量是否在允许范围内
}

// 你可以安排的最多任务数目
// 给你 n 个任务和 m 个工人。每个任务需要一定的力量值才能完成
// 需要的力量值保存在下标从 0 开始的整数数组 tasks 中，
// 第i个任务需要 tasks[i] 的力量才能完成
// 每个工人的力量值保存在下标从 0 开始的整数数组workers中，
// 第j个工人的力量值为 workers[j]
// 每个工人只能完成一个任务，且力量值需要大于等于该任务的力量要求值，即workers[j]>=tasks[i]
// 除此以外，你还有 pills 个神奇药丸，可以给 一个工人的力量值 增加 strength
// 你可以决定给哪些工人使用药丸，但每个工人 最多 只能使用 一片 药丸
// 给你下标从 0 开始的整数数组tasks 和 workers 以及两个整数 pills 和 strength
// 请你返回 最多 有多少个任务可以被完成。

// 主函数，计算最多可以完成的任务数量
// tasks: 任务数组
// workers: 工人类数组
// pills: 可用的药丸数量
// strength: 药丸增加的力量值
int maxTaskAssign(vector<int>& tasks, vector<int>& workers, int pills, int strength) {
    // 对任务和工人数组进行排序
    sort(tasks.begin(), tasks.end());
    sort(workers.begin(), workers.end());

    int tsize = tasks.size(); // 任务数量
    int wsize = workers.size(); // 工人数
    int ans = 0; // 最多可以完成的任务数量
    int l = 0, r = min(tsize, wsize); // 二分查找的左右边界

    // 二分查找最多可以完成的任务数量
    while (l <= r) {
        int m = (l + r) / 2; // 当前中间值

        // 如果m为0，直接更新答案并继续
        if (m == 0) {
            ans = m;
            l = m + 1;
            continue;
        }

        int taskStart = 0; // 任务数组的起始索引
        int taskEnd = m - 1; // 任务数组的结束索引
        int workerStart = wsize - m; // 工人类数组的起始索引
        int workerEnd = wsize - 1; // 工人类数组的结束索引

        // 如果workerStart小于0，说明当前m不合法，更新右边界
        if (workerStart < 0) {
            r = m - 1;
            continue;
        }

        // 调用辅助函数判断是否可以完成m个任务
        if (f(tasks, taskStart, taskEnd, workers, workerStart, workerEnd, strength, pills)) {
            ans = m; // 更新答案
            l = m + 1; // 更新左边界
        } else {
            r = m - 1; // 更新右边界
        }
    }
    return ans; // 返回最多可以完成的任务数量
}
int main() {
    // 示例数据
    vector<int> tasks = {3, 2, 1};
    vector<int> workers = {3, 3, 2};
    int pills = 1;
    int strength = 1;

    // 调用maxTaskAssign函数计算最多可以完成的任务数量
    int result = maxTaskAssign(tasks, workers, pills, strength);

    // 输出结果
    cout << "最多可以完成的任务数量: " << result << endl;

    return 0;
}