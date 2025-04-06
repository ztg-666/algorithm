#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 31; // 定义最大情侣对数
int father[MAXN];    // 并查集的父节点数组
int sets;            // 当前连通分量的数量

// 初始化并查集
void build(int m) {
    for (int i = 0; i < m; ++i) {
        father[i] = i; // 每个节点的父节点初始化为其自身
    }
    sets = m; // 初始时，每个节点都是一个独立的集合
}

// 查找节点 i 的根节点，并进行路径压缩
int find(int i) {
    if (father[i] != i) {
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
        --sets;          // 连通分量数量减一
    }
}

// 计算最少交换次数，使得每对情侣可以并肩坐在一起
int minSwapsCouples(vector<int>& row) {
    int n = row.size();
    build(n / 2); // 初始化并查集，情侣对数为 n/2
    for (int i = 0; i < n; i += 2) {
        int group1 = row[i] / 2;     // 计算第 i 个位置的情侣对编号
        int group2 = row[i + 1] / 2; // 计算第 i+1 个位置的情侣对编号
        unionSet(group1, group2);    // 合并这两个情侣对所在的集合
    }
    return (n / 2) - sets; // 返回需要交换的次数
}
// 情侣牵手
// n对情侣坐在连续排列的 2n 个座位上，想要牵到对方的手
// 人和座位由一个整数数组 row 表示，其中 row[i] 是坐在第 i 个座位上的人的ID
// 情侣们按顺序编号，第一对是 (0, 1)，第二对是 (2, 3)，以此类推，最后一对是 (2n-2, 2n-1)
// 返回 最少交换座位的次数，以便每对情侣可以并肩坐在一起
// 每次交换可选择任意两人，让他们站起来交换座位
// 测试链接 : https://leetcode.cn/problems/couples-holding-hands/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    // 示例座位数组
    vector<int> row = {0, 2, 1, 3}; // 示例1
    // vector<int> row = {3, 2, 0, 1}; // 示例2

    int result = minSwapsCouples(row);
    cout << "minNum: " << result << endl;

    return 0;
}