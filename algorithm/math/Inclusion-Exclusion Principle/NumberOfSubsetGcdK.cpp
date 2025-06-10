#include <iostream>
#include <cstring>

using namespace std;

// 最大公约数为1的子序列数量
// 给你一个数组，返回有多少个子序列的最大公约数是1
// 结果可能很大对1000000007取模
// 测试链接 : https://www.luogu.com.cn/problem/CF803F
// 测试链接 : https://codeforces.com/problemset/problem/803/F
// 1 <= n <= 10^5
// 1 <= nums[i] <= 10^5
// 扩展问题
// 最大公约数为k的子序列数量
// 给定一个长度为n的正数数组nums，还有正数k
// 返回有多少子序列的最大公约数为k

const int MOD = 1000000007;         // 题目要求的模数
const int LIMIT = 100000;           // 题目给定的数值范围上限

// 预处理数组：pow2[i]表示2^i mod MOD，用于快速计算子集数量
long long pow2[LIMIT + 1];
// 计数数组：cnt[x]表示数值x在输入中的出现次数
long long cnt[LIMIT + 1];
// DP数组：dp[i]表示最大公约数恰好为i的子序列数量
long long dp[LIMIT + 1];

// 预处理函数：计算2的幂次模MOD
void build() {
    pow2[0] = 1; // 2^0 = 1
    for (int i = 1; i <= LIMIT; ++i) {
        // 每次乘以2并取模，防止溢出
        pow2[i] = (pow2[i - 1] * 2) % MOD;
    }
}

// 核心计算函数：使用逆向DP和容斥原理计算答案
long long compute() {
    // 从大到小遍历所有可能的公约数
    for (int i = LIMIT; i >= 1; --i) {
        // 统计所有i的倍数的元素总数
        long long counts = 0;
        for (int j = i; j <= LIMIT; j += i) {
            counts += cnt[j];
        }

        // 计算公约数包含i的子序列总数：2^counts - 1
        // 减去空集的情况，故减1
        dp[i] = (pow2[counts] - 1 + MOD) % MOD; // 加MOD防止负数

        // 使用容斥原理：减去公约数超过i的情况（即i的倍数的情况）
        for (int j = 2 * i; j <= LIMIT; j += i) {
            dp[i] = (dp[i] - dp[j] + MOD) % MOD;
        }
    }
    // 最终答案为公约数恰好为1的数量
    return dp[1];
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 预处理2的幂次
    build();

    int n;
    // 处理多组测试数据
    while (cin >> n) {
        // 清空计数器数组
        memset(cnt, 0, sizeof(cnt));
        // 读取输入并统计数字出现次数
        for (int i = 0; i < n; ++i) {
            int x;
            cin >> x;
            cnt[x]++;
        }
        // 计算并输出结果
        cout << compute() << '\n';
    }

    return 0;
}
