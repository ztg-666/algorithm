#include <vector>
#include <queue>
using namespace std;

// 连接棒材的最低费用
// 你有一些长度为正整数的棍子
// 这些长度以数组sticks的形式给出
// sticks[i]是第i个木棍的长度
// 你可以通过支付x+y的成本将任意两个长度为x和y的棍子连接成一个棍子
// 你必须连接所有的棍子，直到剩下一个棍子
// 返回以这种方式将所有给定的棍子连接成一个棍子的最小成本
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-connect-sticks/

int connectSticks(vector<int>& sticks) {
    // 处理边界条件：没有或只有1根木棍无需合并
    if (sticks.size() <= 1) return 0;

    // 创建最小堆（优先队列）
    // 比较器使用greater<int>实现小根堆，确保堆顶始终是最短木棍
    priority_queue<int, vector<int>, greater<int>> minHeap;

    // 初始化堆：将所有木棍加入最小堆
    // 时间复杂度：O(n) 建堆操作（实际是O(n)的heapify过程）
    for (int stick : sticks) {
        minHeap.push(stick);
    }

    int totalCost = 0; // 累计总成本

    // 贪心算法核心：每次合并最短的两根木棍
    // 循环终止条件：堆中只剩1根木棍时停止
    while (minHeap.size() > 1) {
        // 取出当前最短的两根木棍
        int first = minHeap.top(); minHeap.pop(); // O(log n)
        int second = minHeap.top(); minHeap.pop(); // O(log n)

        // 计算本次合并成本并累加
        int cost = first + second;
        totalCost += cost;

        // 将合并后的新木棍长度加入堆中
        // 新长度可能成为后续合并的候选
        minHeap.push(cost); // O(log n)
    }

    // 最终totalCost即为最小总成本
    return totalCost;
}