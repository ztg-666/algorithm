#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

const int MAXN = 30001;
int father[MAXN];    // 并查集的父节点数组，用于记录每个节点的父节点
int maxcnt[MAXN];    // 记录以该节点为根的集合中最大值出现的次数

// 初始化并查集
void build(int n) {
    for (int i = 0; i < n; ++i) {
        father[i] = i;   // 每个节点的父节点初始化为其自身
        maxcnt[i] = 1;   // 每个节点的初始最大值出现次数为 1
    }
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]); // 路径压缩优化查找效率
    }
    return father[i]; // 返回根节点
}

// 合并两个集合，返回新增的好路径数量
int merge(int x, int y, vector<int>& vals) {
    int fx = find(x); // 找到 x 的根节点（同时也是 x 所在集合的最大值）
    int fy = find(y); // 找到 y 的根节点（同时也是 y 所在集合的最大值）
    int add = 0;      // 新增的好路径数量

    if (vals[fx] > vals[fy]) {
        // 如果 x 所在集合的最大值大于 y 所在集合的最大值
        father[fy] = fx; // 将 y 所在集合合并到 x 所在集合
    } else if (vals[fx] < vals[fy]) {
        // 如果 x 所在集合的最大值小于 y 所在集合的最大值
        father[fx] = fy; // 将 x 所在集合合并到 y 所在集合
    } else {
        // 如果两个集合的最大值相等
        add = maxcnt[fx] * maxcnt[fy]; // 计算新增的好路径数量
        father[fy] = fx;               // 将 y 所在集合合并到 x 所在集合
        maxcnt[fx] += maxcnt[fy];      // 更新 x 所在集合的最大值出现次数
    }
    return add; // 返回新增的好路径数量
}

// 计算好路径的数量
int numberOfGoodPaths(vector<int>& vals, vector<vector<int>>& edges) {
    int n = vals.size();
    build(n); // 初始化并查集

    int ans = n; // 初始时，每个节点自身作为一条好路径

    // 按边的最大值升序排序
    sort(edges.begin(), edges.end(), [&vals](const vector<int>& a, const vector<int>& b) {
        int maxA = max(vals[a[0]], vals[a[1]]); // 边 a 的最大值
        int maxB = max(vals[b[0]], vals[b[1]]); // 边 b 的最大值
        return maxA < maxB;                     // 按最大值升序排序
    });

    // 处理每条边
    for (auto& edge : edges) {
        ans += merge(edge[0], edge[1], vals); // 合并两个集合并累加新增的好路径数量
    }
    return ans; // 返回最终的好路径总数
}
// 好路径的数目
// 给你一棵 n 个节点的树（连通无向无环的图）
// 节点编号从0到n-1，且恰好有n-1条边
// 给你一个长度为 n 下标从 0 开始的整数数组 vals
// 分别表示每个节点的值。同时给你一个二维整数数组 edges
// 其中 edges[i] = [ai, bi] 表示节点 ai 和 bi 之间有一条 无向 边
// 好路径需要满足以下条件：开始和结束节点的值相同、 路径中所有值都小于等于开始的值
// 请你返回不同好路径的数目
// 注意，一条路径和它反向的路径算作 同一 路径
// 比方说， 0 -> 1 与 1 -> 0 视为同一条路径。单个节点也视为一条合法路径
// 测试链接 : https://leetcode.cn/problems/number-of-good-paths/
int main() {
    // 测试用例1
    vector<int> vals1 = {2, 1, 1, 2, 2, 1, 1, 2};
    vector<vector<int>> edges1 = {{0,1}, {0,2}, {1,3}, {2,4}, {2,5}, {5,6}, {6,7}};
    cout << numberOfGoodPaths(vals1, edges1) << endl; // 输出 11

    // 测试用例2
    vector<int> vals2 = {1, 2, 2, 3, 1, 2, 2, 1, 1, 3, 3, 3, 3};
    vector<vector<int>> edges2 = {{0,1}, {0,2}, {0,3}, {1,4}, {4,7}, {4,8}, {3,5},
                                 {3,6}, {6,9}, {6,10}, {6,11}, {9,12}};
    cout << numberOfGoodPaths(vals2, edges2) << endl; // 输出 20

    return 0;
}
