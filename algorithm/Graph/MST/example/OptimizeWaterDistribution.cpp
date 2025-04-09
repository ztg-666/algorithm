#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

class Solution {
private:
    // 定义最大节点数，包括虚拟水源节点
    static const int MAXN = 10010;
    // 使用vector存储所有边信息，每条边包含 [起点, 终点, 成本]
    vector<vector<int>> edges;
    // 并查集的父节点数组
    int father[MAXN]{};
    // 记录当前边的数量
    int cnt = 0;

public:
    /**
     * 主函数：计算为所有房子供水的最低成本
     * @param n 房子数量
     * @param wells 每个房子打井的成本
     * @param pipes 管道连接信息，每条管道包含 [房子1, 房子2, 成本]
     * @return 最低总成本
     */
    int minCostToSupplyWater(int n, vector<int>& wells, vector<vector<int>>& pipes) {
        // 初始化并查集和计数器
        build(n);

        // 将每个房子与虚拟水源（节点0）建立边，成本为打井成本
        for (int i = 0; i < n; ++i, ++cnt) {
            edges.push_back({0, i + 1, wells[i]});  // 虚拟水源节点, 当前房子编号, 打井成本
        }

        // 添加所有管道连接作为边
        for (auto& pipe : pipes) {
            edges.push_back(pipe);  // 管道起点, 管道终点, 管道成本
            ++cnt;                  // 增加边计数
        }

        // 按照成本从小到大对所有边进行排序
        sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
            return a[2] < b[2];  // 根据成本排序
        });

        int ans = 0;  // 总成本
        // 遍历所有边，使用Kruskal算法构建最小生成树
        for (int i = 0; i < cnt; ++i) {
            if (unite(edges[i][0], edges[i][1])) {  // 如果两个节点不在同一集合
                ans += edges[i][2];                // 将该边加入结果，并累加成本
            }
        }
        return ans;  // 返回最低总成本
    }

private:
    /**
     * 初始化并查集和计数器
     * @param n 房子数量
     */
    void build(int n) {
        cnt = 0;  // 初始化边计数为0
        // 初始化并查集，每个节点的父节点初始化为自身
        for (int i = 0; i <= n; ++i) {
            father[i] = i;
        }
    }

    /**
     * 查找节点的根节点（路径压缩优化）
     * @param x 节点编号
     * @return 根节点编号
     */
    int find(int x) {
        return x == father[x] ? x : (father[x] = find(father[x]));  // 路径压缩
    }

    /**
     * 合并两个节点所在的集合
     * @param x 节点1
     * @param y 节点2
     * @return 如果合并成功返回true，否则返回false
     */
    bool unite(int x, int y) {
        x = find(x);  // 查找x的根节点
        y = find(y);  // 查找y的根节点
        if (x != y) {  // 如果两个节点不在同一集合
            father[x] = y;  // 合并集合
            return true;    // 合并成功
        }
        return false;  // 已在同一集合，无需合并
    }
};
// 水资源分配优化
// 村里面一共有 n 栋房子。我们希望通过建造水井和铺设管道来为所有房子供水。
// 对于每个房子 i，我们有两种可选的供水方案：一种是直接在房子内建造水井
// 成本为 wells[i - 1] （注意 -1 ，因为 索引从0开始 ）
// 另一种是从另一口井铺设管道引水，数组 pipes 给出了在房子间铺设管道的成本，
// 其中每个 pipes[j] = [house1j, house2j, costj]
// 代表用管道将 house1j 和 house2j连接在一起的成本。连接是双向的。
// 请返回 为所有房子都供水的最低总成本
// 测试链接 : https://leetcode.cn/problems/optimize-water-distribution-in-a-village/
int main() {
    Solution solution;

    // 示例输入
    int n = 3;  // 房子数量
    vector<int> wells = {100, 30};  // 每个房子打井的成本
    vector<vector<int>> pipes = {
        {1, 2, 1},  // 房子1和房子2之间的管道成本为1
        {2, 3, 1}   // 房子2和房子3之间的管道成本为1
    };

    // 计算最低总成本
    int result = solution.minCostToSupplyWater(n, wells, pipes);

    // 输出结果
    cout << ": " << result << endl;

    return 0;
}
