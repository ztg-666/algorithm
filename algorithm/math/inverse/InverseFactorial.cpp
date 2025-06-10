#include <iostream>
using namespace std;

// 连续阶乘逆元的线性递推
// 实现组合公式C(n,m)的计算
// 最终结果 % 1000000007后返回
// 0 <= m <= n <= 1000

using ll = long long;
// 模数定义（1e9+7是常见的大质数模数）
const ll MOD = 1e9 + 7;
// 预计算的最大值范围（支持计算n<=1000的组合数）
const ll LIMIT = 1000;

// 阶乘表：fac[i]存储i! mod MOD的值
ll fac[LIMIT + 1];
// 阶乘逆元表：inv[i]存储i!的模逆元（即1/(i!) mod MOD）
ll inv[LIMIT + 1];

// 快速幂取模算法
// 参数：x-底数，n-指数
// 返回值：x^n mod MOD 的结果
ll power(ll x, ll n) {
    ll res = 1;
    while (n) {
        if (n & 1)
            res = (res * x) % MOD;  // 当指数为奇数时累乘当前底数
        x = (x * x) % MOD;          // 底数平方
        n >>= 1;                    // 指数折半
    }
    return res;
}

// 初始化阶乘表和逆元表
// 时间复杂度O(LIMIT)，预处理完成后可支持O(1)的组合数查询
void build() {
    // 构建阶乘表
    fac[0] = 1; // 0! = 1
    for (ll i = 1; i <= LIMIT; i++) {
        fac[i] = (fac[i-1] * i) % MOD;  // 递推计算阶乘
    }

    // 构建逆元表（优化线性递推法）
    inv[LIMIT] = power(fac[LIMIT], MOD-2);  // 费马小定理计算最大项的逆元
    for (ll i = LIMIT-1; i >= 0; i--) {
        // 递推公式：inv[i] = inv[i+1] * (i+1) mod MOD
        inv[i] = (inv[i+1] * (i+1)) % MOD;  // 利用i! = (i+1)! / (i+1)的逆元关系
    }
}

// 组合数计算函数（使用预计算结果）
// 参数：n-总数，m-选择数
// 返回值：C(n,m) mod MOD 的结果
// 公式：C(n,m) = n!/(m!(n-m)!) = fac[n] * inv[m] * inv[n-m] mod MOD
ll c2(ll n, ll m) {
    if (m < 0 || m > n) return 0;  // 处理非法参数情况
    return (fac[n] * inv[m] % MOD) * inv[n - m] % MOD;  // 组合数公式计算
}

int main() {
    build();  // 预处理阶乘和逆元表

    // 验证测试（验证n=500时所有组合数的计算正确性）
    // cout << "..." << endl;
    // ll n = 500;
    // for (ll m = 0; m <= n; m++) {
    //     ll ans = c2(n, m);  // 实际计算时只需调用c2
    // }
    // cout << "" << endl;

    // 样例计算演示（计算C(537,367)）
    // 注意：当LIMIT>=537时才能正确计算，否则需要调整LIMIT值
    ll a = 537, b = 367;
    cout << "\n C(" << a << ", " << b << ") mod " << MOD << endl;
    cout << ": " << c2(a, b) << endl;

    return 0;
}
