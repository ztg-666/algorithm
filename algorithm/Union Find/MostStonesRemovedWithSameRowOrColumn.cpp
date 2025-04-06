#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;

const int MAXN = 1001;
unordered_map<int, int> rowFirst; // 记录每行第一次出现的石头编号
unordered_map<int, int> colFirst; // 记录每列第一次出现的石头编号
int father[MAXN];
int sets;

// 初始化并查集和哈希表
void build(int n) {
    rowFirst.clear(); // 清空行哈希表
    colFirst.clear(); // 清空列哈希表
    for (int i = 0; i < n; ++i) {
        father[i] = i; // 每个石头的父节点初始化为其自身
    }
    sets = n; // 初始时每个石头是一个独立集合
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (i != father[i]) {
        father[i] = find(father[i]); // 路径压缩
    }
    return father[i]; // 返回根节点
}

// 合并两个集合
void unionSet(int x, int y) {
    int fx = find(x); // 找到 x 的根节点
    int fy = find(y); // 找到 y 的根节点
    if (fx != fy) {
        father[fx] = fy; // 合并两个集合
        sets--;          // 合并后连通分量减少
    }
}

// 计算可以移除的石头的最大数量
int removeStones(vector<vector<int>>& stones) {
    int n = stones.size();
    build(n); // 初始化并查集和哈希表

    for (int i = 0; i < n; ++i) {
        int row = stones[i][0];
        int col = stones[i][1];

        // 处理行关联
        if (rowFirst.find(row) == rowFirst.end()) {
            rowFirst[row] = i; // 记录该行第一次出现的石头编号
        } else {
            unionSet(i, rowFirst[row]); // 合并当前石头与该行第一次出现的石头
        }

        // 处理列关联
        if (colFirst.find(col) == colFirst.end()) {
            colFirst[col] = i; // 记录该列第一次出现的石头编号
        } else {
            unionSet(i, colFirst[col]); // 合并当前石头与该列第一次出现的石头
        }
    }

    return n - sets; // 最大可移除数 = 总石头数 - 连通分量数
}


// 移除最多的同行或同列石头
// n 块石头放置在二维平面中的一些整数坐标点上。每个坐标点上最多只能有一块石头
// 如果一块石头的 同行或者同列 上有其他石头存在，那么就可以移除这块石头
// 给你一个长度为 n 的数组 stones ，其中 stones[i] = [xi, yi] 表示第 i 块石头的位置
// 返回 可以移除的石子 的最大数量。
// 测试链接 : https://leetcode.cn/problems/most-stones-removed-with-same-row-or-column/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    vector<vector<int>> stones1 = {{0,0},{0,1},{1,0},{1,2},{2,1},{2,2}};
    cout << removeStones(stones1) << endl; // 输出 5

    vector<vector<int>> stones2 = {{0,0},{0,2},{1,1},{2,0},{2,2}};
    cout << removeStones(stones2) << endl; // 输出 3

    return 0;
}
