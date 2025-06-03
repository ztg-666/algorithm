#include <vector>
#include <queue>

// 最低加油次数
// 汽车从起点出发驶向目的地，该目的地位于出发位置东面target英里处
// 沿途有加油站，用数组stations表示，其中 stations[i] = [positioni, fueli]
// 表示第i个加油站位于出发位置东面positioni英里处，并且有fueli升汽油
// 假设汽车油箱的容量是无限的，其中最初有startFuel升燃料
// 它每行驶1英里就会用掉1升汽油
// 当汽车到达加油站时，它可能停下来加油，将所有汽油从加油站转移到汽车中
// 为了到达目的地，汽车所必要的最低加油次数是多少？
// 如果无法到达目的地，则返回-1
// 注意：如果汽车到达加油站时剩余燃料为0，它仍然可以在那里加油
// 如果汽车到达目的地时剩余燃料为 0，仍然认为它已经到达目的地
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-refueling-stops/

using namespace std;

class Solution {
public:
    int minRefuelStops(int target, int startFuel, vector<vector<int>>& stations) {
        // 初始油量足够直接到达目标
        if (startFuel >= target) return 0;

        // 大根堆维护可加油量（C++优先队列默认最大堆）
        priority_queue<int> heap;
        int to = startFuel;  // 当前能到达的最远位置
        int cnt = 0;         // 加油次数

        for (auto& station : stations) {
            int position = station[0];
            int fuel = station[1];

            // 当前油量不足以到达该加油站时，尝试用历史最大油量补充
            while (to < position && !heap.empty()) {
                to += heap.top(); // 取出最大油量
                heap.pop();
                cnt++;
                // 补充后若能到达目标则提前返回
                if (to >= target) return cnt;
            }
            // 补充后仍无法到达该加油站则失败
            if (to < position) return -1;

            // 将该加油站的油量加入可选集合
            heap.push(fuel);
        }

        // 处理所有加油站后继续尝试补充油量
        while (!heap.empty()) {
            to += heap.top();
            heap.pop();
            cnt++;
            if (to >= target) return cnt;
        }

        return -1;
    }
};
