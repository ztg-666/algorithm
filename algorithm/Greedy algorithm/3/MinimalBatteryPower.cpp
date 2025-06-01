#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

// 暴力递归方法
// 尝试所有排列组合，计算所有可能顺序中的最小初始电量需求
// 参数说明：
// jobs : 任务列表（每个任务包含消耗电量和启动电量）
// n    : 总任务数量
// i    : 当前处理的任务索引
int f1(vector<vector<int>>& jobs, int n, int i) {
    if (i == n) { // 所有任务排列完成
        int ans = 0;
        // 遍历当前排列顺序，计算所需初始电量
        for (auto& job : jobs) {
            // 当前任务启动电量要求 vs 累积消耗后的剩余电量
            ans = max(job[1], ans + job[0]);
        }
        return ans;
    } else {
        int ans = INT_MAX;
        // 生成全排列：交换当前位置任务，递归处理后续位置
        for (int j = i; j < n; j++) {
            swap(jobs[i], jobs[j]);    // 尝试交换位置
            ans = min(ans, f1(jobs, n, i + 1)); // 递归处理后续任务
            swap(jobs[i], jobs[j]);    // 回溯恢复原顺序
        }
        return ans;
    }
}

// 暴力解法入口函数
// 使用值传递保留原始任务顺序，避免影响后续测试
int atLeast1(vector<vector<int>> jobs) {
    return f1(jobs, jobs.size(), 0);
}

// 贪心算法方法
// 时间复杂度：O(n logn)
int atLeast2(vector<vector<int>>& jobs) {
    // 按（消耗电量 - 启动电量需求）降序排序
    // 差值大的任务优先处理，可以最小化初始电量需求
    sort(jobs.begin(), jobs.end(), [](const vector<int>& a, const vector<int>& b) {
        return (b[0] - b[1]) < (a[0] - a[1]); // 比较差值实现降序排列
    });

    int ans = 0; // 记录所需最小初始电量
    for (auto& job : jobs) {
        // 当前任务启动时，需要满足：
        // 1. 满足当前任务启动电量要求 job[1]
        // 2. 之前累积消耗电量 + 当前任务消耗 ans + job[0]
        ans = max(ans + job[0], job[1]);
    }
    return ans;
}

// 生成随机测试数据
// 参数说明：
// n : 生成任务数量
// v : 随机值范围上限（生成值在[1, v]区间）
vector<vector<int>> randomJobs(int n, int v) {
    vector<vector<int>> jobs(n, vector<int>(2));
    for (int i = 0; i < n; i++) {
        jobs[i][0] = rand() % v + 1; // 消耗电量[1, v]
        jobs[i][1] = rand() % v + 1; // 启动电量[1, v]
    }
    return jobs;
}

int main() {
    srand(time(0)); // 初始化随机种子

    // 测试参数配置
    const int N = 10;      // 最大任务数量
    const int V = 20;      // 随机值范围
    const int testTimes = 2000; // 测试次数

    cout << "" << endl;
    for (int i = 1; i <= testTimes; i++) {
        int n = rand() % N + 1; // 随机任务数量[1, N]
        auto jobs = randomJobs(n, V);

        // 验证暴力解与贪心解结果一致性
        int ans1 = atLeast1(jobs);
        int ans2 = atLeast2(jobs);

        if (ans1 != ans2) {
            cout << "" << endl;
        }

        // 每100组输出进度提示
        if (i % 100 == 0) {
            cout << "" << i << "" << endl;
        }
    }
    cout << "" << endl;

    return 0;
}
