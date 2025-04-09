#include <vector>
#include <algorithm>
using namespace std;

vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edges, vector<vector<int>>& queries) {
    // 将边按权重升序排序，确保后续可以逐步处理小权重的边
    sort(edges.begin(), edges.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[2] < b[2];
    });

    int m = edges.size(); // 边的数量
    int k = queries.size(); // 查询的数量
    vector<vector<int>> questions(k, vector<int>(4)); // 用于存储查询及其原始索引

    // 处理查询数组，保存原始索引以便最终结果按输入顺序返回
    for (int i = 0; i < k; ++i) {
        questions[i][0] = queries[i][0]; // 起点
        questions[i][1] = queries[i][1]; // 终点
        questions[i][2] = queries[i][2]; // 权重限制
        questions[i][3] = i; // 原始索引
    }

    // 按查询的limit升序排序，确保从小到大处理查询
    sort(questions.begin(), questions.end(), [](const vector<int>& a, const vector<int>& b) {
        return a[2] < b[2];
    });

    // 初始化并查集，father数组记录每个节点的父节点
    vector<int> father(n);
    for (int i = 0; i < n; ++i) {
        father[i] = i; // 初始时每个节点的父节点是自身
    }

    // 查找函数（路径压缩优化）
    auto find = [&](int x) {
        while (father[x] != x) { // 如果当前节点不是根节点
            father[x] = father[father[x]]; // 路径压缩，将x直接连接到祖父节点
            x = father[x]; // 继续向上查找
        }
        return x; // 返回根节点
    };

    vector<bool> ans(k, false); // 存储每个查询的结果
    int j = 0; // 当前处理的边索引

    // 遍历所有查询
    for (int i = 0; i < k; ++i) {
        int limit = questions[i][2]; // 当前查询的权重限制

        // 合并所有边权小于当前limit的边
        while (j < m && edges[j][2] < limit) {
            int fx = find(edges[j][0]); // 找到边起点的根节点
            int fy = find(edges[j][1]); // 找到边终点的根节点
            if (fx != fy) { // 如果两个节点不在同一个集合中
                father[fx] = fy; // 合并两个集合
            }
            ++j; // 处理下一条边
        }

        // 检查当前查询的起点和终点是否连通
        ans[questions[i][3]] = (find(questions[i][0]) == find(questions[i][1]));
    }

    return ans; // 返回结果数组
}
#include<iostream>
// 检查边长度限制的路径是否存在
// 给你一个 n 个点组成的无向图边集 edgeList
// 其中 edgeList[i] = [ui, vi, disi] 表示点 ui 和点 vi 之间有一条长度为 disi 的边
// 请注意，两个点之间可能有 超过一条边 。
// 给你一个查询数组queries ，其中 queries[j] = [pj, qj, limitj]
// 你的任务是对于每个查询 queries[j] ，判断是否存在从 pj 到 qj 的路径
// 且这条路径上的每一条边都 严格小于 limitj 。
// 请你返回一个 布尔数组 answer ，其中 answer.length == queries.length
// 当 queries[j] 的查询结果为 true 时， answer 第 j 个值为 true ，否则为 false
// 测试链接 : https://leetcode.cn/problems/checking-existence-of-edge-length-limited-paths/
// 主函数
int main() {
    // 测试数据
    int n = 5; // 节点数量
    vector<vector<int>> edges = {
        {0, 1, 5}, // 边: 起点, 终点, 权重
        {1, 2, 3},
        {2, 3, 4},
        {3, 4, 1}
    };
    vector<vector<int>> queries = {
        {0, 4, 6}, // 查询: 起点, 终点, 权重限制
        {0, 3, 3},
        {1, 4, 5}
    };

    // 调用函数
    vector<bool> result = distanceLimitedPathsExist(n, edges, queries);

    // 输出结果
    cout << "result: ";
    for (bool res : result) {
        cout << (res ? "true" : "false") << " ";
    }
    cout << endl;

    return 0;
}