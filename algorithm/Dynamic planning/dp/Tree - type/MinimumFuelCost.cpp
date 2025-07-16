#include <vector>
#include <numeric> // 虽然在这个特定代码中没有直接使用 <numeric> 的功能，但它对于处理 vector 等通常是有用的。

// 到达首都的最少油耗
// 给你一棵 n 个节点的树（一个无向、连通、无环图）
// 每个节点表示一个城市，编号从 0 到 n - 1 ，且恰好有 n - 1 条路
// 0 是首都。给你一个二维整数数组 roads
// 其中 roads[i] = [ai, bi] ，表示城市 ai 和 bi 之间有一条 双向路
// 每个城市里有一个代表，他们都要去首都参加一个会议
// 每座城市里有一辆车。给你一个整数 seats 表示每辆车里面座位的数目
// 城市里的代表可以选择乘坐所在城市的车，或者乘坐其他城市的车
// 相邻城市之间一辆车的油耗是一升汽油
// 请你返回到达首都最少需要多少升汽油
// 测试链接 : https://leetcode.cn/problems/minimum-fuel-cost-to-report-to-the-capital/

// 辅助递归函数，执行深度优先搜索
// graph: 图的邻接表表示
// seats: 每辆车的座位数
// u: 当前访问的城市节点
// p: u 的父节点
// size_people: size_people[i] 存储以城市 i 为根的子树中代表的总人数（包括城市 i 自身的代表）
// cost_fuel: cost_fuel[i] 存储将以城市 i 为根的子树中的所有代表聚集到城市 i 所需的最小油耗
void calculate_subtree_costs(const std::vector<std::vector<int>>& graph, int seats, int u, int p,
                             std::vector<int>& size_people, std::vector<long long>& cost_fuel) {
    size_people[u] = 1; // 当前城市 u 有1个代表
    cost_fuel[u] = 0;   // 将 u 子树的代表（目前只有u自己）聚集到 u 的成本为0

    for (int v : graph[u]) {
        if (v == p) { // 跳过父节点
            continue;
        }
        calculate_subtree_costs(graph, seats, v, u, size_people, cost_fuel); // 递归处理子节点 v

        // 子节点 v 的递归调用返回后:
        // size_people[v] 是 v 子树中代表的总数
        // cost_fuel[v] 是将这些代表聚集到城市 v 的油耗

        // 更新 u 子树的代表总数
        size_people[u] += size_people[v];

        // 更新聚集到 u 的油耗:
        // 1. 加上将 v 子树的代表聚集到 v 的油耗
        cost_fuel[u] += cost_fuel[v];
        // 2. 加上将 v 子树的所有 size_people[v] 个代表从城市 v 运送到城市 u 的油耗
        //    所需车辆数 = ceil(size_people[v] / seats)
        //    对于正整数 A, B, ceil(A/B) = (A + B - 1) / B
        //    每辆车从 v到 u 的油耗是 1
        cost_fuel[u] += (static_cast<long long>(size_people[v]) + seats - 1) / seats;
    }
}

long long minimumFuelCost(std::vector<std::vector<int>>& roads, int seats) {
    int n = roads.size() + 1; // 城市总数
    if (n <= 1) { // 如果只有0个或1个城市，不需要油耗
        return 0;
    }

    std::vector<std::vector<int>> graph(n);
    for (const auto& road : roads) {
        graph[road[0]].push_back(road[1]);
        graph[road[1]].push_back(road[0]);
    }

    std::vector<int> size_people(n);         // 存储每个子树的代表人数
    std::vector<long long> cost_fuel(n);     // 存储将子树代表聚集到子树根节点的油耗v

    // 从首都 (城市 0) 开始深度优先搜索，父节点设为 -1 (无效节点)
    calculate_subtree_costs(graph, seats, 0, -1, size_people, cost_fuel);

    // cost_fuel[0] 即为将所有代表聚集到首都 0 的总油耗
    return cost_fuel[0];
}

#include<iostream>

int main() {
    // 测试用例 1 (来自 LeetCode 示例)
    std::vector<std::vector<int>> roads1 = {{0,1},{0,2},{0,3}};
    int seats1 = 5;
    long long result1 = minimumFuelCost(roads1, seats1);
    std::cout << "Test Case 1:" << std::endl;
    std::cout << "Roads: [[0,1],[0,2],[0,3]], Seats: " << seats1 << std::endl;
    std::cout << "Minimum Fuel Cost: " << result1 << std::endl; // 预期输出: 3
    std::cout << "-------------------------" << std::endl;

    // 测试用例 2 (来自 LeetCode 示例)
    std::vector<std::vector<int>> roads2 = {{3,1},{3,2},{1,0},{0,4},{0,5},{4,6}};
    int seats2 = 2;
    long long result2 = minimumFuelCost(roads2, seats2);
    std::cout << "Test Case 2:" << std::endl;
    std::cout << "Roads: [[3,1],[3,2],[1,0],[0,4],[0,5],[4,6]], Seats: " << seats2 << std::endl;
    std::cout << "Minimum Fuel Cost: " << result2 << std::endl; // 预期输出: 7
    std::cout << "-------------------------" << std::endl;

    // 测试用例 3 (只有首都)
    std::vector<std::vector<int>> roads3 = {};
    int seats3 = 1;
    long long result3 = minimumFuelCost(roads3, seats3);
    std::cout << "Test Case 3:" << std::endl;
    std::cout << "Roads: [], Seats: " << seats3 << std::endl;
    std::cout << "Minimum Fuel Cost: " << result3 << std::endl; // 预期输出: 0
    std::cout << "-------------------------" << std::endl;

    // 测试用例 4 (线性图)
    std::vector<std::vector<int>> roads4 = {{0,1},{1,2},{2,3}};
    int seats4 = 1;
    long long result4 = minimumFuelCost(roads4, seats4);
    std::cout << "Test Case 4:" << std::endl;
    std::cout << "Roads: [[0,1],[1,2],[2,3]], Seats: " << seats4 << std::endl;
    std::cout << "Minimum Fuel Cost: " << result4 << std::endl; // 预期输出: 6 (3->2(1), 2->1(2), 1->0(3))
    std::cout << "-------------------------" << std::endl;

    // 测试用例 5 (多座位，线性图)
    std::vector<std::vector<int>> roads5 = {{0,1},{1,2},{2,3}};
    int seats5 = 2;
    long long result5 = minimumFuelCost(roads5, seats5);
    std::cout << "Test Case 5:" << std::endl;
    std::cout << "Roads: [[0,1],[1,2],[2,3]], Seats: " << seats5 << std::endl;
    std::cout << "Minimum Fuel Cost: " << result5 << std::endl; // 预期输出: 3 (3->2(1车), 2->1(1车), 1->0(1车))
                                                             //  代表: 3:1, 2:1, 1:1, 0:1
                                                             //  3->2: 1人, 1车, cost=1. 到2时2有2人
                                                             //  2->1: 2人, 1车, cost=1. 到1时1有3人
                                                             //  1->0: 3人, 2车, cost=2.  等等，应该是 (3+2-1)/2 = 2辆车？
                                                             //  让我们重新计算一下 Test Case 5
                                                             //  f(graph, 2, 3, 2, size, cost)
                                                             //      size[3]=1, cost[3]=0
                                                             //  f(graph, 2, 2, 1, size, cost)
                                                             //      f(graph, 2, 3, 2, size, cost) -> size[3]=1, cost[3]=0
                                                             //      size[2]=1+size[3]=2
                                                             //      cost[2]=0+cost[3]+(size[3]+2-1)/2 = 0+0+(1+2-1)/2 = 1
                                                             //  f(graph, 2, 1, 0, size, cost)
                                                             //      f(graph, 2, 2, 1, size, cost) -> size[2]=2, cost[2]=1
                                                             //      size[1]=1+size[2]=3
                                                             //      cost[1]=0+cost[2]+(size[2]+2-1)/2 = 0+1+(2+2-1)/2 = 1+1 = 2
                                                             //  f(graph, 2, 0, -1, size, cost)
                                                             //      f(graph, 2, 1, 0, size, cost) -> size[1]=3, cost[1]=2
                                                             //      size[0]=1+size[1]=4
                                                             //      cost[0]=0+cost[1]+(size[1]+2-1)/2 = 0+2+(3+2-1)/2 = 2+2 = 4
                                                             //  预期输出: 4
    std::cout << "Minimum Fuel Cost: " << result5 << std::endl; // 修正预期输出: 4
    std::cout << "-------------------------" << std::endl;


    return 0;
}