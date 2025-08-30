#include <bits/stdc++.h>

// 有趣的数列(重要! 因子计数法)
// 求第 n 项卡特兰数 Cn = (1/(n+1)) * C(2n, n)，要求答案对 mod 取模
// 1 <= n <= 10^6
// 1 <= mod <= 10^9
// 注意：mod 可能不是质数，因此不能用逆元，需要用因子分解统计质数指数的方法
// 题目链接 : https://www.luogu.com.cn/problem/P3200

using namespace std;

const int MAXN = 2000001; // 需要分解到 2n

int minpf[MAXN];   // minpf[i] = i 的最小质因子 (Minimum Prime Factor)，若为 0 表示 i 是质数
int prime[MAXN];   // 存放所有质数
int cnt;           // 质数表大小
int counts[MAXN];  // 记录每个数的指数（因子计数表）

// 欧拉筛：O(n)，同时得到所有质数和每个数的最小质因子
void euler(int n) {
    fill(minpf, minpf + n + 1, 0);
    cnt = 0;
    for (int i = 2; i <= n; i++) {
        if (minpf[i] == 0) {
            // i 是质数
            prime[cnt++] = i;
        }
        // 用当前质数更新 i*prime[j] 的最小质因子
        for (int j = 0; j < cnt; j++) {
            if (1LL * i * prime[j] > n) break;
            minpf[i * prime[j]] = prime[j]; // 记录最小质因子
            if (i % prime[j] == 0) break;   // 保证最小质因子正确
        }
    }
}

// 快速幂：计算 (x^p) % mod
long long power(long long x, long long p, int mod) {
    long long ans = 1 % mod;
    x %= mod;
    while (p > 0) {
        if (p & 1) ans = ans * x % mod;
        x = x * x % mod;
        p >>= 1;
    }
    return ans;
}

// 计算第 n 个卡特兰数 mod
int compute(int n, int mod) {
    // 第一步：筛出 [2 .. 2n] 的最小质因子
    euler(2 * n);

    // 第二步：构建因子计数表
    // 卡特兰数公式：Cn = (2n)! / (n! * (n+1)!)
    // 等价于： (n+2)(n+3)...(2n) / (2*3*...*n)

    // 初始化为 0
    fill(counts, counts + (2 * n + 1), 0);

    // 分母部分：n! -> 2..n 每个数贡献 -1
    for (int i = 2; i <= n; i++) counts[i] = -1;

    // 分子部分：(n+2)(n+3)...(2n) -> 每个数贡献 +1
    for (int i = n + 2; i <= 2 * n; i++) counts[i] = 1;

    // 第三步：分解合数 -> 只保留质数的指数
    // 从大到小分解，避免重复累加
    for (int i = 2 * n; i >= 2; i--) {
        if (minpf[i] != 0) {  // i 是合数
            // 把 i 的指数分解给它的两个因子
            counts[minpf[i]] += counts[i];
            counts[i / minpf[i]] += counts[i];
            counts[i] = 0; // i 已经分解完，不再需要
        }
    }

    // 第四步：把质数的幂次累乘起来
    long long ans = 1 % mod;
    for (int i = 2; i <= 2 * n; i++) {
        if (counts[i] != 0) {
            ans = ans * power(i, counts[i], mod) % mod;
        }
    }
    return (int)ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, mod;
    cin >> n >> mod;
    cout << compute(n, mod) << "\n";
    return 0;
}
