#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
// 01背包(模版)
// 给定一个正数t，表示背包的容量
// 有m个货物，每个货物可以选择一次
// 每个货物有自己的体积costs[i]和价值values[i]
// 返回在不超过总容量的情况下，怎么挑选货物能达到价值最大
// 返回最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1048
using namespace std;

// 定义最大物品数量和最大背包容量
const int MAXM = 101;
const int MAXT = 1001;

// cost数组存储每个物品的体积，val数组存储每个物品的价值
int cost[MAXM];
int val[MAXM];

// dp数组用于动态规划计算，空间优化后使用一维数组
int dp[MAXT];

// t表示背包容量，n表示物品数量
int t, n;

// 方法1：二维DP（未优化空间）
int compute1() {
    // 初始化一个二维数组dp2来存储中间结果
    int dp2[MAXM][MAXT];
    memset(dp2, 0, sizeof(dp2));

    // 动态规划求解
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= t; ++j) {
            // 不选第i个物品时的最大价值
            dp2[i][j] = dp2[i - 1][j];
            if (j >= cost[i]) {
                // 选择第i个物品时的最大价值
                dp2[i][j] = max(dp2[i][j], dp2[i - 1][j - cost[i]] + val[i]);
            }
        }
    }

    // 返回最终结果
    return dp2[n][t];
}

// 方法2：一维DP（空间优化）
int compute2() {
    // 初始化dp数组
    memset(dp, 0, sizeof(dp));

    // 动态规划求解
    for (int i = 1; i <= n; ++i) {
        // 逆序遍历背包容量，以确保每次更新只使用上一轮的状态
        for (int j = t; j >= cost[i]; --j) {
            // 更新当前背包容量下的最大价值
            dp[j] = max(dp[j], dp[j - cost[i]] + val[i]);
        }
    }

    // 返回最终结果
    return dp[t];
}

int main() {
    // 输入背包容量和物品数量
    scanf("%d %d", &t, &n);

    // 输入每个物品的体积和价值
    for (int i = 1; i <= n; ++i) {
        scanf("%d %d", &cost[i], &val[i]);
    }

    // 输出最大价值
    printf("%d\n", compute2());

    return 0;
}
