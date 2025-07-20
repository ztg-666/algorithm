#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// 粉刷房子III
// 房子有n个，从左到右排列，编号1..n，颜色有c种，编号1..c
// 给定数组house，house[i]表示房子的颜色，如果house[i]为0说明房子没有涂色
// 你必须给每个没有涂色的房子涂上颜色，如果有颜色的房子不能改变颜色
// 给定二维数组cost，cost[i][v]表示如果i号房涂成v号颜色，需要花费多少钱
// 相邻的、拥有同一种颜色的房子为1个街区
// 比如如果所有房子的颜色为: {1, 1, 2, 3, 2, 2}，那么一共4个街区
// 最终所有的房子涂完颜色，一定要形成t个街区，返回最少的花费
// 1 <= t <= n <= 100
// 1 <= c <= 20
// 0 <= house[i] <= c
// 1 <= cost[i][v] <= 10^4
// 测试链接 : https://leetcode.cn/problems/paint-house-iii/


class PaintHouseIII {
private:
    static const int NA = INT_MAX;      // 表示不可达状态
    static const int MAXN = 101;        // 最大房子数量
    static const int MAXT = 101;        // 最大街区数量
    static const int MAXC = 21;         // 最大颜色数量

    int house[MAXN];                    // 房子颜色数组，从1开始索引
    int cost[MAXN][MAXC];              // 涂色成本数组，从1开始索引
    int n, t, c;                       // n:房子数，t:目标街区数，c:颜色数

    /**
     * 数据预处理函数
     * 将0索引的输入数据转换为1索引的内部表示
     * @param houses 原始房子颜色数组（0索引）
     * @param costs 原始涂色成本数组（0索引）
     * @param hsize 房子数量
     * @param csize 颜色数量
     * @param tsize 目标街区数量
     */
    void build(vector<int>& houses, vector<vector<int>>& costs, int hsize, int csize, int tsize) {
        n = hsize;
        t = tsize;
        c = csize;
        // 将房子颜色数组从0索引转为1索引
        for (int i = 1; i <= n; i++) {
            house[i] = houses[i - 1];
        }
        // 将成本数组从0索引转为1索引
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= c; j++) {
                cost[i][j] = costs[i - 1][j - 1];
            }
        }
    }

    /**
     * 递归函数：记忆化搜索版本
     * @param i 当前处理到第i个房子
     * @param j 剩余需要形成的街区数
     * @param v 第i+1个房子的颜色（0表示虚拟颜色）
     * @param dp 记忆化数组
     * @return 最小花费，NA表示不可能
     *
     * 状态定义：f(i,j,v) 表示处理1~i号房子，第i+1号房子颜色为v，
     * 使得1~i+1号房子形成j个街区的最小花费
     */
    int f1(int i, int j, int v, vector<vector<vector<int>>>& dp) {
        // 边界条件：街区数为0，不可能
        if (j == 0) {
            return NA;
        }
        // 边界条件：没有房子时，只有j=1才可能（虚拟状态）
        if (i == 0) {
            return j == 1 ? 0 : NA;
        }
        // 记忆化：如果已经计算过，直接返回
        if (dp[i][j][v] != -1) {
            return dp[i][j][v];
        }

        int ans = NA;

        if (house[i] != 0) {
            // 第i个房子已经有颜色
            if (house[i] == v) {
                // 与后面房子颜色相同，不增加街区数
                ans = f1(i - 1, j, house[i], dp);
            } else {
                // 与后面房子颜色不同，增加一个街区
                ans = f1(i - 1, j - 1, house[i], dp);
            }
        } else {
            // 第i个房子没有颜色，需要涂色
            for (int color = 1; color <= c; color++) {
                int next;
                if (color == v) {
                    // 与后面房子颜色相同，不增加街区数
                    next = f1(i - 1, j, color, dp);
                } else {
                    // 与后面房子颜色不同，增加一个街区
                    next = f1(i - 1, j - 1, color, dp);
                }
                if (next != NA) {
                    ans = min(ans, next + cost[i][color]);
                }
            }
        }

        // 记忆化存储结果
        dp[i][j][v] = ans;
        return ans;
    }

public:
    /**
     * 方法1：递归 + 记忆化搜索，不优化枚举
     * 时间复杂度：O(n * t * c²)
     * 空间复杂度：O(n * t * c)
     */
    int minCost1(vector<int>& houses, vector<vector<int>>& costs, int hsize, int csize, int tsize) {
        build(houses, costs, hsize, csize, tsize);
        t++; // 增加虚拟街区，简化边界处理

        // 初始化记忆化数组，-1表示未计算
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(t + 1, vector<int>(c + 1, -1)));

        int ans = f1(n, t, 0, dp); // 0表示虚拟颜色
        return ans == NA ? -1 : ans;
    }

    /**
     * 方法2：严格位置依赖的动态规划，不优化枚举
     * 时间复杂度：O(n * t * c²)
     * 空间复杂度：O(n * t * c)
     */
    int minCost2(vector<int>& houses, vector<vector<int>>& costs, int hsize, int csize, int tsize) {
        build(houses, costs, hsize, csize, tsize);
        t++; // 增加虚拟街区

        // dp[i][j][v] 表示前i个房子，第i个房子颜色为v，形成j个街区的最小花费
        vector<vector<vector<int>>> dp(n + 1, vector<vector<int>>(t + 1, vector<int>(c + 1)));

        // 初始化：街区数为0的情况都不可能
        for (int i = 0; i <= n; i++) {
            for (int v = 0; v <= c; v++) {
                dp[i][0][v] = NA;
            }
        }

        // 初始化：没有房子的情况
        for (int j = 1; j <= t; j++) {
            for (int v = 0; v <= c; v++) {
                dp[0][j][v] = j == 1 ? 0 : NA; // 只有j=1时虚拟状态成立
            }
        }

        // 动态规划填表
        for (int i = 1; i <= n; i++) {            // 枚举房子
            for (int j = 1; j <= t; j++) {        // 枚举街区数
                for (int v = 0; v <= c; v++) {    // 枚举当前房子颜色
                    int ans = NA;

                    if (house[i] != 0) {
                        // 房子已有颜色
                        if (house[i] == v) {
                            // 与指定颜色相同，从相同颜色转移
                            ans = dp[i - 1][j][house[i]];
                        } else {
                            // 与指定颜色不同，从不同颜色转移（增加街区）
                            ans = dp[i - 1][j - 1][house[i]];
                        }
                    } else {
                        // 房子需要涂色，尝试所有颜色
                        for (int color = 1; color <= c; color++) {
                            int next;
                            if (color == v) {
                                // 颜色相同，不增加街区
                                next = dp[i - 1][j][color];
                            } else {
                                // 颜色不同，增加街区
                                next = dp[i - 1][j - 1][color];
                            }
                            if (next != NA) {
                                ans = min(ans, next + cost[i][color]);
                            }
                        }
                    }
                    dp[i][j][v] = ans;
                }
            }
        }

        int ans = dp[n][t][0]; // 最终答案：n个房子，t个街区，虚拟颜色
        return ans == NA ? -1 : ans;
    }

    /**
     * 方法3：空间压缩版本，不优化枚举
     * 时间复杂度：O(n * t * c²)
     * 空间复杂度：O(t * c)
     */
    int minCost3(vector<int>& houses, vector<vector<int>>& costs, int hsize, int csize, int tsize) {
        build(houses, costs, hsize, csize, tsize);
        t++;

        // 使用滚动数组优化空间
        vector<vector<int>> memo(t + 1, vector<int>(c + 1)); // 上一轮的结果
        vector<vector<int>> dp(t + 1, vector<int>(c + 1));   // 当前轮的结果

        // 初始化：街区数为0都不可能
        for (int v = 0; v <= c; v++) {
            memo[0][v] = dp[0][v] = NA;
        }

        // 初始化：i=0时的状态
        for (int j = 1; j <= t; j++) {
            for (int v = 0; v <= c; v++) {
                memo[j][v] = j == 1 ? 0 : NA;
            }
        }

        // 滚动更新
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= t; j++) {
                for (int v = 0; v <= c; v++) {
                    int ans = NA;

                    if (house[i] != 0) {
                        // 房子已有颜色
                        if (house[i] == v) {
                            ans = memo[j][house[i]];     // 颜色相同
                        } else {
                            ans = memo[j - 1][house[i]]; // 颜色不同
                        }
                    } else {
                        // 房子需要涂色
                        for (int color = 1; color <= c; color++) {
                            int next;
                            if (color == v) {
                                next = memo[j][color];     // 颜色相同
                            } else {
                                next = memo[j - 1][color]; // 颜色不同
                            }
                            if (next != NA) {
                                ans = min(ans, next + cost[i][color]);
                            }
                        }
                    }
                    dp[j][v] = ans;
                }
            }
            // 交换数组，准备下一轮
            swap(memo, dp);
        }

        int ans = memo[t][0];
        return ans == NA ? -1 : ans;
    }

    /**
     * 方法4：最优解 - 优化枚举 + 空间压缩
     * 时间复杂度：O(n * t * c)
     * 空间复杂度：O(t * c)
     *
     * 核心优化：使用前缀后缀预处理避免重复枚举
     */
    int minCost4(vector<int>& houses, vector<vector<int>>& costs, int hsize, int csize, int tsize) {
        build(houses, costs, hsize, csize, tsize);
        t++;

        // 滚动数组
        vector<vector<int>> memo(t + 1, vector<int>(c + 1));
        vector<vector<int>> dp(t + 1, vector<int>(c + 1));

        // 初始化
        for (int v = 0; v <= c; v++) {
            memo[0][v] = dp[0][v] = NA;
        }
        for (int j = 1; j <= t; j++) {
            for (int v = 0; v <= c; v++) {
                memo[j][v] = j == 1 ? 0 : NA;
            }
        }

        // 优化枚举的辅助数组
        vector<int> pre(c + 2);  // pre[s]: 前缀1...s中的最小值
        vector<int> suf(c + 2);  // suf[s]: 后缀s...c中的最小值
        pre[0] = suf[c + 1] = NA;

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= t; j++) {
                // 预处理前缀最小值：dp[i-1][j-1][1...s] + cost[i][1...s]
                for (int v = 1; v <= c; v++) {
                    pre[v] = pre[v - 1]; // 继承前一个位置的最小值
                    if (memo[j - 1][v] != NA) {
                        pre[v] = min(pre[v], memo[j - 1][v] + cost[i][v]);
                    }
                }

                // 预处理后缀最小值：dp[i-1][j-1][s...c] + cost[i][s...c]
                for (int v = c; v >= 1; v--) {
                    suf[v] = suf[v + 1]; // 继承后一个位置的最小值
                    if (memo[j - 1][v] != NA) {
                        suf[v] = min(suf[v], memo[j - 1][v] + cost[i][v]);
                    }
                }

                // 填写DP表
                for (int v = 0; v <= c; v++) {
                    int ans = NA;

                    if (house[i] != 0) {
                        // 房子已有颜色，处理方式同前面方法
                        if (house[i] == v) {
                            ans = memo[j][house[i]];
                        } else {
                            ans = memo[j - 1][house[i]];
                        }
                    } else {
                        // 房子需要涂色，使用优化后的枚举
                        if (v == 0) {
                            // v=0时，可以选择任意颜色，取所有颜色的最小值
                            ans = suf[1];
                        } else {
                            // v!=0时，可以选择除了v以外的任意颜色
                            // 分为两部分：1...v-1 和 v+1...c
                            ans = min(pre[v - 1], suf[v + 1]);

                            // 也可以选择颜色v（街区数不变）
                            if (memo[j][v] != NA) {
                                ans = min(ans, memo[j][v] + cost[i][v]);
                            }
                        }
                    }
                    dp[j][v] = ans;
                }
            }
            // 滚动更新
            swap(memo, dp);
        }

        int ans = memo[t][0];
        return ans == NA ? -1 : ans;
    }
};
