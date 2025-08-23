#include <bits/stdc++.h>

// 背包(两次转圈法)
// 一共有n种物品，第i种物品的体积为v[i]，价值为c[i]，每种物品可以选择任意个，个数不能是负数
// 一共有m条查询，每次查询都会给定jobv，代表体积的要求
// 要求挑选物品的体积和一定要严格是jobv，返回能得到的最大价值和
// 如果没有方案能正好凑满jobv，返回-1
// 1 <= n <= 50
// 1 <= m <= 10^5
// 1 <= v[i] <= 10^5
// 1 <= c[i] <= 10^6
// 10^11 <= jobv <= 10^12
// 测试链接 : https://www.luogu.com.cn/problem/P9140


using namespace std;

const int MAXN = 100001;
const long long NEG_INF = LLONG_MIN; // 代表不可达状态（初始化用）

int v[MAXN];        // v[i] : 第 i 种物品的体积
int c[MAXN];        // c[i] : 第 i 种物品的价值
long long dp[MAXN]; // dp[r] : 当总体积 % x == r 时，能得到的最大“补偿值”

int n, m, x, y; // n: 物品数量, m: 查询数量, (x, y): 基准物品的体积和价值

// 计算最大公约数（辗转相除法）
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// 预处理 DP 数组
// 思路：
// - 选定单位价值最大的物品作为基准物品 (x, y)
// - dp[r] 表示当总体积模 x 为 r 时，能够获得的最大额外价值补偿
// - “两次转圈法”保证每个余数类能被完全更新
void compute() {
    // 初始化 dp，所有余数类都不可达（-INF）
    for (int i = 0; i < x; i++) {
        dp[i] = NEG_INF;
    }
    dp[0] = 0; // 余数 0 的初始状态可达，补偿值为 0

    // 遍历所有物品（除了基准物品）
    for (int i = 1; i <= n; i++) {
        if (v[i] != x) { // 跳过基准物品
            int d = gcd(v[i], x); // gcd 决定余数环的划分
            // 枚举每个子环起点
            for (int j = 0; j < d; j++) {
                int cur = j;
                int circle = 0;
                // 两次转圈，保证子环内的状态都能被松弛
                while (circle < 2) {
                    int nxt = (cur + v[i]) % x; // 下一余数位置
                    if (dp[cur] != NEG_INF) {
                        // 状态转移：
                        // 当前余数 cur → 下一余数 nxt
                        // 补偿值更新：扣掉多用的基准物品价值，再加上当前物品价值
                        dp[nxt] = max(dp[nxt],
                                      dp[cur] - 1LL * ((cur + v[i]) / x) * y + c[i]);
                    }
                    cur = nxt;
                    if (cur == j) circle++; // 回到起点，计一圈
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n >> m;
    double best = 0.0; // 记录当前最大性价比
    // 读入所有物品
    for (int i = 1; i <= n; i++) {
        cin >> v[i] >> c[i];
        double ratio = (double)c[i] / v[i]; // 单位价值
        // 更新基准物品（选取单位价值最高的物品）
        if (ratio > best) {
            best = ratio;
            x = v[i]; // 基准物品体积
            y = c[i]; // 基准物品价值
        }
    }

    // 预处理 DP
    compute();

    // 处理 m 次查询
    while (m--) {
        long long jobv; // 目标体积
        cin >> jobv;
        int rem = (int)(jobv % x); // 目标体积对基准物品体积取模
        if (dp[rem] == NEG_INF) {
            // 如果不可达，输出 -1
            cout << -1 << "\n";
        } else {
            // 否则：基准物品撑满大部分体积，再加上补偿值
            cout << jobv / x * 1LL * y + dp[rem] << "\n";
        }
    }

    return 0;
}
