#include <vector>
#include <algorithm>

// 大楼扔鸡蛋问题
// 一共有k枚相同的鸡蛋，一共有n层楼
// 已知一定存在楼层f(0<=f<=n)，从>f的楼层扔鸡蛋一定会碎，从<=f的楼层扔鸡蛋，扔几次都不会碎
// 鸡蛋一旦碎了就不能再使用，只能选择另外的鸡蛋
// 现在想确定f的值，返回最少扔几次鸡蛋，可以确保测出该值
// 1 <= k <= 100
// 1 <= n <= 10^4
// 测试链接 : https://leetcode.cn/problems/super-egg-drop/

using namespace std;

class Solution {
public:
    // 最优解 - 二维DP版本
    // 思路：逆向思维，不是求"给定k个鸡蛋和n层楼，最少需要扔多少次"
    // 而是求"给定k个鸡蛋和m次尝试机会，最多能测试多少层楼"
    // 当dp[i][j] >= n时，说明i个鸡蛋扔j次就足以测试n层楼
    int superEggDrop1(int k, int n) {
        // 特殊情况：只有1个鸡蛋，只能从1楼开始逐层测试
        if (k == 1) {
            return n;
        }

        // dp[i][j] 表示有i个鸡蛋，扔j次最多能测试多少层楼
        vector<vector<int>> dp(k + 1, vector<int>(n + 1, 0));

        // j表示扔的次数，从1开始逐渐增加
        for (int j = 1; j <= n; j++) {
            // i表示鸡蛋个数，从1到k
            for (int i = 1; i <= k; i++) {
                // 状态转移方程：dp[i][j] = dp[i-1][j-1] + dp[i][j-1] + 1
                // dp[i-1][j-1]：如果鸡蛋碎了，剩下i-1个鸡蛋，j-1次机会能测试的楼层数
                // dp[i][j-1]：如果鸡蛋没碎，还有i个鸡蛋，j-1次机会能测试的楼层数
                // +1：当前这一层楼
                dp[i][j] = dp[i - 1][j - 1] + dp[i][j - 1] + 1;

                // 如果i个鸡蛋扔j次能测试的楼层数 >= n，说明j次就够了
                if (dp[i][j] >= n) {
                    return j;
                }
            }
        }
        return -1; // 理论上不会执行到这里
    }

    // 最优解空间压缩的版本
    // 优化：由于dp[i][j]只依赖于dp[i-1][j-1]和dp[i][j-1]
    // 可以用一维数组来优化空间复杂度
    int superEggDrop2(int k, int n) {
        // 特殊情况：只有1个鸡蛋，只能从1楼开始逐层测试
        if (k == 1) {
            return n;
        }

        // dp[i] 表示有i个鸡蛋，当前扔的次数下最多能测试多少层楼
        vector<int> dp(k + 1, 0);

        // j表示扔的次数，从1开始逐渐增加
        for (int j = 1; j <= n; j++) {
            int pre = 0; // 保存dp[i-1]的上一轮值，即dp[i-1][j-1]

            // i表示鸡蛋个数，从1到k
            for (int i = 1; i <= k; i++) {
                int tmp = dp[i]; // 保存当前dp[i]的值，即dp[i][j-1]

                // 状态转移：dp[i] = dp[i] + pre + 1
                // pre是dp[i-1][j-1]，dp[i]是dp[i][j-1]
                dp[i] = dp[i] + pre + 1;

                pre = tmp; // 更新pre为下一轮的dp[i-1][j-1]

                // 如果i个鸡蛋扔j次能测试的楼层数 >= n，说明j次就够了
                if (dp[i] >= n) {
                    return j;
                }
            }
        }
        return -1; // 理论上不会执行到这里
    }
};

