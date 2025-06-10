#include <iostream>
using namespace std;

// 组合数学常用常量
const int MOD = 1000000007;   // 模数，防止数值过大
const int LIMIT = 100;        // 预计算的最大值
long long fac[LIMIT + 1];     // 存储阶乘的数组（fac[i] = i! mod MOD）
long long inv[LIMIT + 1];     // 存储逆元的数组（inv[i] = (i!)^{-1} mod MOD）

/**
 * 快速幂算法计算 x^n mod MOD
 * @param x 底数
 * @param n 指数
 * @return x^n mod MOD 的结果
 */
long long power(long long x, int n) {
    long long ans = 1;
    x %= MOD;  // 先取模确保数值范围
    while (n > 0) {
        if (n & 1) {  // 处理二进制位为1的情况
            ans = (ans * x) % MOD;
        }
        x = (x * x) % MOD;  // 平方处理
        n >>= 1;  // 右移一位
    }
    return ans;
}

/**
 * 预计算阶乘和逆元表
 * 时间复杂度：O(LIMIT)
 */
void build() {
    // 计算阶乘
    fac[0] = 1;
    for (int i = 1; i <= LIMIT; ++i) {
        fac[i] = (i * fac[i - 1]) % MOD;
    }

    // 使用费马小定理计算逆元
    inv[LIMIT] = power(fac[LIMIT], MOD - 2);  // 通过 a^(p-2) ≡ a^{-1} (mod p)
    for (int i = LIMIT - 1; i >= 0; --i) {
        inv[i] = ((i + 1) * inv[i + 1]) % MOD;  // 递推公式：inv[i] = inv[i+1] * (i+1)
    }
}

/**
 * 计算符合要求的音乐播放列表数量
 * @param n 不同歌曲数量
 * @param l 播放列表长度
 * @param k 每首歌曲必须间隔的歌曲数
 * @return 符合条件的播放列表数量 mod MOD
 */
int numMusicPlaylists(int n, int l, int k) {
    build();
    // 边界条件处理：歌曲不足或无法满足间隔要求
    if (n > l || n <= k) return 0;

    long long ans = 0;
    long long sign = 1;  // 容斥原理的符号交替

    // 使用容斥原理计算所有可能的组合
    for (int i = 0; i <= n - k; ++i) {
        // 组合数项 C(n-k, i)
        long long cur = (sign * fac[n]) % MOD;
        cur = (cur * inv[i]) % MOD;           // 除以 i!
        cur = (cur * inv[n - k - i]) % MOD;   // 除以 (n-k-i)!

        // 剩余歌曲的排列方式：(n-k-i)^(l-k)
        cur = (cur * power(n - k - i, l - k)) % MOD;

        ans = (ans + cur + MOD) % MOD;  // 保证结果非负
        sign = (sign == 1) ? (MOD - 1) : 1;  // 交替符号（+1 和 -1）
    }
    return (int)ans;
}

int main() {
    // 测试样例：2首不同的歌，播放列表长度3，允许重复间隔0首歌
    // 预期输出 2（所有排列方式：121, 212）
    cout << numMusicPlaylists(2, 3, 0) << endl;
    return 0;
}
