#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

// 最小移动总距离
// 所有工厂和机器人都分布在x轴上
// 给定长度为n的二维数组factory，factory[i][0]为i号工厂的位置，factory[i][1]为容量
// 给定长度为m的一维数组robot，robot[j]为第j个机器人的位置
// 每个工厂所在的位置都不同，每个机器人所在的位置都不同，机器人到工厂的距离为位置差的绝对值
// 所有机器人都是坏的，但是机器人可以去往任何工厂进行修理，但是不能超过某个工厂的容量
// 测试数据保证所有机器人都可以被维修，返回所有机器人移动的最小总距离
// 1 <= n、m <= 100
// -10^9 <= factory[i][0]、robot[j] <= +10^9
// 0 <= factory[i][1] <= m
// 测试链接 : https://leetcode.cn/problems/minimum-total-distance-traveled/

using namespace std;

class Solution {
private:
    static const long long NA = LLONG_MAX;  // 表示不可达状态的常量
    static const int MAXN = 101;            // 工厂数量的最大值
    static const int MAXM = 101;            // 机器人数量的最大值

    int n, m;                               // n: 工厂数量, m: 机器人数量

    // 工厂信息数组，下标从1开始
    // fac[i][0]: 第i个工厂的位置坐标
    // fac[i][1]: 第i个工厂的容量（能修理的机器人数量）
    int fac[MAXN][2];

    // 机器人位置数组，下标从1开始
    // rob[i]: 第i个机器人的位置坐标
    int rob[MAXM];

    // 动态规划数组
    // dp[i][j]: 前i个工厂修理前j个机器人的最小总距离
    long long dp[MAXN][MAXM];

    // 前缀和数组
    // sum[j]: 前j个机器人都去当前工厂的总距离
    long long sum[MAXM];

    // 单调队列，用于优化DP转移
    int queue[MAXM];
    int l, r;                               // 单调队列的左右指针

public:
    /**
     * 预处理函数：排序并重新编号
     * @param factory 工厂数组，每个元素包含[位置, 容量]
     * @param robot 机器人位置数组
     */
    void build(vector<vector<int>>& factory, vector<int>& robot) {
        // 按位置对工厂进行排序，保证工厂位置单调递增
        sort(factory.begin(), factory.end(), [](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0];
        });

        // 按位置对机器人进行排序，保证机器人位置单调递增
        sort(robot.begin(), robot.end());

        n = factory.size();
        m = robot.size();

        // 将工厂信息复制到数组中，下标从1开始（便于DP处理边界）
        for (int i = 1; i <= n; i++) {
            fac[i][0] = factory[i - 1][0];  // 工厂位置
            fac[i][1] = factory[i - 1][1];  // 工厂容量
        }

        // 将机器人位置复制到数组中，下标从1开始
        for (int i = 1; i <= m; i++) {
            rob[i] = robot[i - 1];
        }

        // DP数组初始化
        // dp[0][j] = NA 表示0个工厂无法修理任何机器人（j > 0时）
        for (int j = 1; j <= m; j++) {
            dp[0][j] = NA;
        }
    }

    /**
     * 计算第i个工厂与第j个机器人之间的距离
     * @param i 工厂编号
     * @param j 机器人编号
     * @return 距离（绝对值）
     */
    long long dist(int i, int j) {
        return abs((long long)fac[i][0] - rob[j]);
    }

    /**
     * 计算第i个工厂从第j个机器人开始负责的转移代价
     * 这个函数用于单调队列优化
     * @param i 工厂编号
     * @param j 机器人编号（作为起始位置）
     * @return 如果可行返回转移代价，否则返回NA
     */
    long long value(int i, int j) {
        // 如果前i-1个工厂无法修理前j-1个机器人，则不可行
        if (dp[i - 1][j - 1] == NA) {
            return NA;
        }
        // 返回: 前i-1个工厂修理前j-1个机器人的代价 - 前j-1个机器人到第i个工厂的距离和
        // 这样设计是为了在后续计算中方便加上sum[j]
        return dp[i - 1][j - 1] - sum[j - 1];
    }

    /**
     * 主函数：计算最小总距离
     * 使用单调队列优化的动态规划，时间复杂度O(n*m)
     * @param robot 机器人位置数组
     * @param factory 工厂信息数组
     * @return 最小总移动距离
     */
    long long minimumTotalDistance(vector<int>& robot, vector<vector<int>>& factory) {
        build(factory, robot);

        // 枚举每个工厂
        for (int i = 1; i <= n; i++) {
            int cap = fac[i][1];  // 当前工厂的容量

            // 计算前缀和：sum[j] = 前j个机器人都到第i个工厂的总距离
            for (int j = 1; j <= m; j++) {
                sum[j] = sum[j - 1] + dist(i, j);
            }

            // 初始化单调队列
            l = r = 0;

            // 枚举前j个机器人
            for (int j = 1; j <= m; j++) {
                // 状态转移方程：dp[i][j] = min(dp[i-1][j], min{dp[i-1][k-1] + sum[j] - sum[k-1]})
                // 其中k的范围是[max(1, j-cap), j]，表示第i个工厂负责第k到第j个机器人

                // 不使用第i个工厂的情况
                dp[i][j] = dp[i - 1][j];

                // 维护单调队列：队列中存储可能的最优k值
                // 如果当前j可以作为起始点，加入队列
                if (value(i, j) != NA) {
                    // 维护队列的单调性：移除不优的元素
                    while (l < r && value(i, queue[r - 1]) >= value(i, j)) {
                        r--;
                    }
                    queue[r++] = j;  // 将当前j加入队列尾部
                }

                // 移除超出容量限制的元素
                // 如果队列头部元素表示的起始位置超出了容量限制，移除它
                if (l < r && queue[l] == j - cap) {
                    l++;
                }

                // 如果队列非空，更新dp[i][j]
                if (l < r) {
                    // 使用队列头部的最优起始位置进行状态转移
                    dp[i][j] = min(dp[i][j], value(i, queue[l]) + sum[j]);
                }
            }
        }

        // 返回前n个工厂修理前m个机器人的最小总距离
        return dp[n][m];
    }
};
