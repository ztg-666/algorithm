#include <vector>
#include <algorithm>
#include <climits>

// 在传球游戏中最大化函数值
// 给定一个长度为n的数组receiver和一个整数k
// 总共有n名玩家，编号0 ~ n-1，这些玩家在玩一个传球游戏
// receiver[i]表示编号为i的玩家会传球给下一个人的编号
// 玩家可以传球给自己，也就是说receiver[i]可能等于i
// 你需要选择一名开始玩家，然后开始传球，球会被传恰好k次
// 如果选择编号为x的玩家作为开始玩家
// 函数f(x)表示从x玩家开始，k次传球内所有接触过球的玩家编号之和
// 如果某位玩家多次触球，则累加多次
// f(x) = x + receiver[x] + receiver[receiver[x]] + ...
// 你的任务时选择开始玩家x，目的是最大化f(x)，返回函数的最大值
// 测试链接 : https://leetcode.cn/problems/maximize-value-of-function-in-a-ball-passing-game/

using namespace std;

/**
 * 传球游戏最大化价值问题
 * 问题描述：有n个玩家围成一圈，每个玩家i会将球传给receiver[i]
 * 需要找到一个起始位置，使得经过k次传球后，所有经过位置的索引之和最大
 */
class PassingBallMaximizeValue {
private:
    static const int MAXN = 100001;  // 最大节点数
    static const int LIMIT = 34;     // 最大幂次（log2(10^10) ≈ 33.22）

    int power;                       // k的二进制表示中的最大幂次
    int m;                          // k的二进制表示中1的个数
    int kbits[LIMIT];               // 存储k的二进制表示中为1的位（从高到低）
    int stjump[MAXN][LIMIT];        // 倍增数组：stjump[i][j]表示从位置i经过2^j步后到达的位置
    long long stsum[MAXN][LIMIT];   // 倍增数组：stsum[i][j]表示从位置i经过2^j步的路径和（不包括起始位置）

    /**
     * 构建k的二进制表示
     * 将k分解为2的幂次之和，便于后续快速计算
     * 例如：k=13=8+4+1，则kbits[]={3,2,0}（对应2^3,2^2,2^0）
     */
    inline void build(long long k) {
        power = 0;
        long long temp = k >> 1;  // k除以2
        // 计算需要的最大幂次
        while ((1LL << power) <= temp) {
            power++;
        }

        m = 0;
        // 贪心法：从最大幂次开始，依次减去能减去的最大2的幂次
        for (int p = power; p >= 0; p--) {
            long long powerOf2 = 1LL << p;  // 2^p
            if (powerOf2 <= k) {
                kbits[m++] = p;  // 记录这个幂次
                k -= powerOf2;   // 从k中减去这个幂次
            }
        }
    }

public:
    /**
     * 获取最大函数值
     * @param receiver 传球规则数组，receiver[i]表示玩家i会将球传给玩家receiver[i]
     * @param k 传球次数
     * @return 最大的路径和
     */
    long long getMaxFunctionValue(vector<int>& receiver, long long k) {
        build(k);  // 构建k的二进制表示
        const int n = receiver.size();

        // 初始化倍增数组的第0层（2^0 = 1步）
        for (int i = 0; i < n; i++) {
            const int next = receiver[i];
            stjump[i][0] = next;      // 从位置i走1步到达的位置
            stsum[i][0] = next;       // 从位置i走1步的路径和（不包括起始位置i）
        }

        // 构建倍增数组：动态规划思想
        // stjump[i][p] = stjump[stjump[i][p-1]][p-1]
        // 从位置i走2^p步 = 先走2^(p-1)步到达中间位置，再从中间位置走2^(p-1)步
        for (int p = 1; p <= power; p++) {
            for (int i = 0; i < n; i++) {
                const int mid = stjump[i][p - 1];  // 中间位置
                stjump[i][p] = stjump[mid][p - 1]; // 最终位置
                stsum[i][p] = stsum[i][p - 1] + stsum[mid][p - 1];  // 路径和
            }
        }

        long long ans = 0;

        // 枚举每个可能的起始位置
        for (int i = 0; i < n; i++) {
            int cur = i;           // 当前位置
            long long sum = i;     // 路径和，初始包含起始位置

            // 使用k的二进制表示快速计算k步后的结果
            // 例如：k=13=8+4+1，则分别走8步、4步、1步
            for (int j = 0; j < m; j++) {
                const int bit = kbits[j];    // 当前要走的步数的幂次
                sum += stsum[cur][bit];      // 累加这段路径的和
                cur = stjump[cur][bit];      // 更新当前位置
            }

            // 更新最大值
            if (sum > ans) {
                ans = sum;
            }
        }

        return ans;
    }
};

/**
 * 算法复杂度分析：
 * 时间复杂度：O(n * log(k))
 * - 构建倍增数组：O(n * log(k))
 * - 枚举起始位置：O(n * log(k))
 *
 * 空间复杂度：O(n * log(k))
 * - 倍增数组占用空间
 *
 * 算法核心思想：
 * 1. 倍增法：预处理出从每个位置走2^i步后的结果
 * 2. 二进制分解：将k分解为2的幂次之和，快速计算k步的结果
 * 3. 枚举优化：尝试每个起始位置，找到最优解
 */
