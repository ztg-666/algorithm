#include <iostream>
using namespace std;
using ll = long long;

/**
 * 快速幂算法计算模幂
 * @param b 底数
 * @param n 指数（必须是正数）
 * @param mod 模数（必须是质数）
 * @return 计算结果 (b^n) % mod
 *
 * 算法原理：
 * 利用二进制分解指数，时间复杂度O(logn)
 * 通过循环右移指数n，当最低位为1时累乘结果
 */
ll power(ll b, ll n, ll mod) {
    ll ans = 1;
    while (n > 0) {
        if ((n & 1) == 1) { // 当二进制最低位为1时
            ans = (ans * b) % mod; // 累乘当前基数
        }
        b = (b * b) % mod; // 基数平方
        n >>= 1; // 右移一位处理下一个二进制位
    }
    return ans;
}

/**
 * 直接除法取模计算
 * @param a 被除数（必须能被b整除）
 * @param b 除数
 * @param mod 模数
 * @return (a/b) % mod 的结果
 *
 * 适用条件：
 * 1. 必须保证a能被b整除
 * 2. 当b与mod不互质时仍可使用
 */
ll compute1(ll a, ll b, ll mod) {
    return (ll)((a / b) % mod);
}

/**
 * 逆元法计算模除
 * @param a 被除数
 * @param b 除数
 * @param mod 模数（必须是质数）
 * @return (a/b) % mod 的结果
 *
 * 算法原理：
 * 利用费马小定理，当mod是质数时：
 * b^(mod-2) 即为b的模逆元
 * 时间复杂度：O(logmod)
 */
ll compute2(ll a, ll b, ll mod) {
    ll inv = power(b, mod - 2, mod); // 计算逆元
    return (ll)(((a % mod) * inv) % mod); // 用乘法代替除法
}

int main() {
    // 测试参数设置（需满足以下条件）：
    // 1. a必须能被b整除
    // 2. mod必须是质数
    // 3. b和mod必须互质
    ll mod = 41;        // 质数模数
    ll b = 3671613LL;   // 除数
    ll a = 67312LL * b; // 构造可被整除的被除数

    // 输出两种计算方式的结果（应该相同）
    cout << "Direct method: " << compute1(a, b, mod) << endl;
    cout << "Inverse method: " << compute2(a, b, mod) << endl;

    return 0;
}
