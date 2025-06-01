#include <vector>
#include <queue>

// IPO
// 给你n个项目，对于每个项目i
// 它都有一个纯利润profits[i]
// 和启动该项目需要的最小资本capital[i]
// 最初你的资本为w，当你完成一个项目时，你将获得纯利润，添加到你的总资本中
// 总而言之，从给定项目中选择最多k个不同项目的列表
// 以最大化最终资本，并输出最终可获得的最多资本
// 测试链接 : https://leetcode.cn/problems/ipo/

using namespace std;

// 项目结构体
struct Project {
    int p; // 纯利润
    int c; // 所需启动资金

    Project(int profit, int cost) : p(profit), c(cost) {}
};

// 比较器：按启动资金升序排列（小根堆）
struct CompareCost {
    bool operator()(const Project& a, const Project& b) {
        return a.c > b.c; // 注意符号方向与Java相反
    }
};

// 比较器：按利润降序排列（大根堆）
struct CompareProfit {
    bool operator()(const Project& a, const Project& b) {
        return a.p < b.p; // 注意符号方向与Java相反
    }
};

int findMaximizedCapital(int k, int w, vector<int>& profit, vector<int>& cost) {
    int n = profit.size();

    // 小根堆存储未解锁项目（按启动资金排序）
    priority_queue<Project, vector<Project>, CompareCost> heap1;
    // 大根堆存储可解锁项目（按利润排序）
    priority_queue<Project, vector<Project>, CompareProfit> heap2;

    // 初始化未解锁项目堆
    for (int i = 0; i < n; i++) {
        heap1.emplace(profit[i], cost[i]);
    }

    while (k > 0) {
        // 将当前资金能解锁的项目加入候选
        while (!heap1.empty() && heap1.top().c <= w) {
            heap2.push(heap1.top());
            heap1.pop();
        }

        // 无候选项目时提前终止
        if (heap2.empty()) break;

        // 选择利润最大的项目
        w += heap2.top().p;
        heap2.pop();
        k--;
    }
    return w;
}
