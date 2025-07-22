#include <bits/stdc++.h>
// 找出满足以下条件的整数三元组（a, b, c）的数量，结果对 998244353 取模。
// 条件：
// 1≤a, b, c≤N
// a、b、c 两两互不相同
// a 除以 b 的余数等于 c
// 约束条件
// 3≤N≤10¹²（N 为 3 到 10¹² 之间的整数）
// N 是整数

using namespace std;

const long long mod = 998244353;  // 模数，用于防止溢出

int main(){
    ios::sync_with_stdio(false), cin.tie(0);  // 优化输入输出速度

    long long n, i, d, nxt, ans;
    cin >> n;

    // 初始化答案为 n*(n+1)/2，这是所有位置的贡献总和
    // 使用模运算防止溢出：n*(n+1)/2 ≡ n*(n+1)*inv(2) (mod 998244353)
    // 其中 mod/2 = 499122177 是 2 在模 998244353 下的逆元
    // mod-mod/2 实际上计算的是 998244353 - 499122177 = 499122176
    ans = n % mod * ((n + 1) % mod) % mod * (mod - mod/2) % mod;

    // 使用整除分块优化计算 ∑(i=1 to n) floor(n/i)
    for(i = 1; i <= n;) {
        d = n / i;          // 当前的 floor(n/i) 值
        nxt = n / d;        // 找到下一个 floor(n/i) 值发生变化的位置

        // 在区间 [i, nxt] 内，floor(n/i) 的值都等于 d
        // 所以这个区间对答案的贡献是 (nxt-i+1) * d
        // 从总答案中减去这个贡献
        ans = (ans + mod - (nxt - i + 1) % mod * (d % mod) % mod) % mod;

        i = nxt + 1;            // 跳到下一个区间的起始位置
    }

    cout << ans << '\n';
    return 0;
}
