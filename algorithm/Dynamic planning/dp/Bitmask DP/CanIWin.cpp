#include <vector>

// 我能赢吗
// 给定两个整数n和m
// 两个玩家可以轮流从公共整数池中抽取从1到n的整数（不放回）
// 抽取的整数会累加起来（两个玩家都算）
// 谁在自己的回合让累加和 >= m，谁获胜
// 若先出手的玩家能稳赢则返回true，否则返回false
// 假设两位玩家游戏时都绝顶聪明，可以全盘为自己打算
// 测试链接 : https://leetcode.cn/problems/can-i-win/

class Solution {
private:
    // n: 可选数字的最大值
    // status: 当前可用的数字的状态 (位掩码，第i位为1表示数字i可用)
    // rest: 还需要凑够的数
    // dp: 记忆化数组
    // dp[status] == 0: 代表没算过
    // dp[status] == 1: 算过，当前状态下，先手能赢 (true)
    // dp[status] == -1: 算过，当前状态下，先手不能赢 (false)
    bool solve(int n, int status, int rest, std::vector<int>& dp) {
        if (rest <= 0) {
            // 当前玩家被要求行动，但目标和 rest 已经 <= 0，
            // 这意味着上一个玩家已经使累加和 >= m。
            // 所以当前玩家输。
            return false;
        }
        if (dp[status] != 0) {
            return dp[status] == 1;
        }

        // 当前玩家尝试做出一个选择
        // 遍历所有可选的数字 1 到 n
        for (int i = 1; i <= n; ++i) {
            // 检查数字 i 是否可用 (即 status 的第 i 位是否为 1)
            // 如果当前玩家选择数字 i，那么对手将面临以下状态：
            // - 可用数字变为: status ^ (1 << i) (数字i被选中，变为不可用)
            // - 剩余需要凑够的数为: rest - i
            // 如果对手在那种状态下不能赢 (!solve(...))，那么当前玩家通过选择 i 就赢了。
            if ((status & (1 << i)) != 0 && !solve(n, status ^ (1 << i), rest - i, dp)) {
                dp[status] = 1; // 标记当前状态，先手能赢
                return true;
            }
        }

        // 如果尝试了所有可行的选择，都不能让当前玩家赢
        dp[status] = -1; // 标记当前状态，先手不能赢
        return false;
    }

public:
    bool canIWin(int maxChoosableInteger, int desiredTotal) {
        if (desiredTotal == 0) {
            // 题目规定或通常理解：如果目标和是0，先手什么都不用做就赢了。
            return true;
        }

        // 如果所有可选数字 (1 到 maxChoosableInteger) 的总和都小于目标和 desiredTotal，
        // 那么没有人能赢，先手自然也赢不了。
        // 1到n的总和是 n * (n + 1) / 2。
        if (maxChoosableInteger * (maxChoosableInteger + 1) / 2 < desiredTotal) {
            return false;
        }

        // dp 数组，用于记忆化搜索结果
        // dp 数组的大小需要覆盖所有可能的 status。status 是一个位掩码。
        // 数字从 1 到 maxChoosableInteger，我们用位 i 代表数字 i。
        // 因此，需要 maxChoosableInteger + 1位 (包括位0，尽管数字0不用)。
        // 大小为 1 << (maxChoosableInteger + 1)。
        // LeetCode 中 maxChoosableInteger <= 20, 所以 1 << 21 是可接受的大小。
        std::vector<int> dp(1 << (maxChoosableInteger + 1), 0);

        // 初始状态 (initial_status):
        // 所有从 1 到 maxChoosableInteger 的数字都可用。
        // (1 << (maxChoosableInteger + 1)) - 1 会将位 0 到 maxChoosableInteger 都设置为 1。
        // 在 solve 函数中，我们通过循环 for (int i = 1; i <= n; ++i) 来选择数字，
        // 这确保了我们只考虑数字 1 到 n (即位 1 到 n)。
        // 位 0 虽然在 status 中可能被设置，但不会影响数字的选择逻辑。

        // 调用递归函数，判断先手是否能赢
        // maxChoosableInteger 对应 solve 函数中的 n
        // desiredTotal 对应 solve 函数中的 rest
        return solve(maxChoosableInteger, (1 << (maxChoosableInteger + 1)) - 1, desiredTotal, dp);
    }
};
