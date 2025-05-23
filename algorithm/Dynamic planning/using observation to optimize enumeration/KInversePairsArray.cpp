#include <vector>
using namespace std;
using ll = long long;

class Solution {
public:
    // 方法1：基础动态规划（未优化枚举版本）
    // 时间复杂度: O(n*k^2)，空间复杂度: O(n*k)
    ll kInversePairs1(ll n, ll k) {
        const ll mod = 1000000007; // 取模基数
        // dp[i][j]表示用1~i的数字组成恰好j个逆序对的排列数
        vector<vector<ll>> dp(n + 1, vector<ll>(k + 1, 0));
        dp[0][0] = 1; // 初始条件：空数组有0个逆序对

        for (ll i = 1; i <= n; ++i) {    // 逐步增加数字个数
            dp[i][0] = 1;                // 没有逆序对只有一种排列（升序）
            for (ll j = 1; j <= k; ++j) { // 遍历不同逆序对数量
                if (i > j) { // 当数字个数多于逆序对时
                    // 累加前i-1个数字所有可能生成0~j个逆序对的情况
                    for (ll p = 0; p <= j; ++p) {
                        dp[i][j] = (dp[i][j] + dp[i - 1][p]) % mod;
                    }
                } else { // 当数字个数不足以生成全部逆序对时
                    // 计算有效范围：j-i+1到j，避免越界
                    for (ll p = max(0LL, j - i + 1); p <= j; ++p) {
                        dp[i][j] = (dp[i][j] + dp[i - 1][p]) % mod;
                    }
                }
            }
        }
        return dp[n][k];
    }

    // 方法2：优化枚举版本（滑动窗口优化）
    // 时间复杂度: O(n*k)，空间复杂度: O(n*k)
    ll kInversePairs2(ll n, ll k) {
        const ll mod = 1000000007;
        vector<vector<ll>> dp(n + 1, vector<ll>(k + 1, 0));
        dp[0][0] = 1;

        for (ll i = 1; i <= n; ++i) {
            dp[i][0] = 1;    // 初始化0逆序对的情况
            ll window = 1;   // 滑动窗口累计值（初始为dp[i-1][0]）
            for (ll j = 1; j <= k; ++j) {
                if (i > j) { // 当i足够大时，窗口直接累加
                    window = (window + dp[i - 1][j]) % mod;
                } else {     // 当i较小时，窗口需要减去超出部分
                    // 窗口滑动：加上新元素，减去超出窗口范围的旧元素
                    // 通过加mod确保减法不出现负数
                    window = (window + dp[i - 1][j] -
                            (j - i >= 0 ? dp[i - 1][j - i] : 0) + mod) % mod;
                }
                dp[i][j] = window; // 更新当前状态为窗口值
            }
        }
        return dp[n][k];
    }
};