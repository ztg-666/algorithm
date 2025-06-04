#include <iostream>
#include <algorithm>
#include <cstring>

// 砍树
// 一共有n棵树，每棵树都有两个信息：
// 第一天这棵树的初始重量、这棵树每天的增长重量
// 你每天最多能砍1棵树，砍下这棵树的收益为：
// 这棵树的初始重量 + 这棵树增长到这一天的总增重
// 从第1天开始，你一共有m天可以砍树，返回m天内你获得的最大收益
// 测试链接 : https://pintia.cn/problem-sets/91827364500/exam/problems/91827367873

using namespace std;

// 最大树数量+1（索引从1开始使用）
const int MAXN = 251;

// 树的信息存储数组
// tree[i][0]: 第i棵树第一天的初始重量
// tree[i][1]: 第i棵树每天的增长重量
int tree[MAXN][2];

// 动态规划表
// dp[i][j]: 前i棵树在j天内砍伐可获得的最大收益
int dp[MAXN][MAXN];

int t, n, m; // 测试用例数、当前用例树数、可用天数

/*
* 核心计算函数
* 实现策略：
* 1. 对树按日增重量升序排序（增重慢的树先处理）
* 2. 使用动态规划计算最大收益
* 排序原理：增重快的树越晚砍伐收益越大，通过排序保证最优决策顺序
*/
int compute() {
    // 按日增重量升序排序（lambda表达式自定义比较器）
    // 排序范围：tree[1]到tree[n]（闭区间）
    sort(tree + 1, tree + n + 1, [](const int* a, const int* b) {
        return a[1] < b[1]; // 比较日增重量
    });

    // 动态规划填表过程
    for (int i = 1; i <= n; ++i) {       // 遍历每棵树
        for (int j = 1; j <= m; ++j) {   // 遍历每个可能的天数
            dp[i][j] = max(
                // 情况1：不砍第i棵树，继承前i-1棵树j天的结果
                dp[i - 1][j],
                // 情况2：砍第i棵树，收益计算为：
                // 前i-1棵树j-1天的最大收益 +
                // 当前树的初始重量 +
                // 日增重量 * (砍伐天数-1)（因为第j天砍时已增长j-1天）
                dp[i - 1][j - 1] + tree[i][0] + tree[i][1] * (j - 1)
            );
        }
    }
    return dp[n][m]; // 返回前n棵树m天的最大收益
}

/*
* 主函数处理流程：
* 1. 加速IO流处理
* 2. 循环处理多个测试用例
* 3. 每个用例：读取树数、天数，初始化树数据
* 4. 清空动态规划表（复用数组需要重置）
*/
int main() {
    ios::sync_with_stdio(false); // 关闭同步提升IO速度
    cin.tie(0);                  // 解除cin与cout的绑定
    cout.tie(0);

    cin >> t;
    while (t--) {
        cin >> n >> m;
        // 读取初始重量到tree[][0]
        for (int j = 1; j <= n; ++j) cin >> tree[j][0];
        // 读取日增重量到tree[][1]
        for (int j = 1; j <= n; ++j) cin >> tree[j][1];

        // 初始化dp数组为全0（使用memset快速清空）
        memset(dp, 0, sizeof(dp));

        cout << compute() << '\n';
    }
    return 0;
}
