#include <bits/stdc++.h>

// 墨墨的等式(两次转圈法)
// 一共有n种正数，每种数可以选择任意个，个数不能是负数
// 那么一定有某些数值可以由这些数字累加得到
// 请问在[l...r]范围上，有多少个数能被累加得到
// 0 <= n <= 12
// 0 <= 数值范围 <= 5 * 10^5
// 1 <= l <= r <= 10^12
// 测试链接 : https://www.luogu.com.cn/problem/P2371

using namespace std;

const int MAXN = 500001;
const long long INF = LLONG_MAX;

int v[MAXN];         // 存储输入的 n 个数
long long distArr[MAXN]; // 存储余数对应的最小值
int n, x;            // n: 数字种类，x: 最小非零数值(用于取模)
long long l, r;      // 区间 [l, r]

// 计算最大公约数
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// 核心计算函数
long long compute() {
    sort(v + 1, v + n + 1);  // 排序
    int size = 0;
    // 去掉所有 0
    for (int i = 1; i <= n; i++) {
        if (v[i] != 0) {
            v[++size] = v[i];
        }
    }
    if (size == 0) {
        return 0;  // 如果全部是 0，无法组成任何正数
    }
    x = v[1];  // 基准数 = 最小的非零数
    // 初始化 dist 数组
    for (int i = 0; i < x; i++) {
        distArr[i] = INF;
    }
    distArr[0] = 0;  // 余数 0 的最小和是 0

    // 处理基准数之外的数字
    for (int i = 2; i <= size; i++) {
        int d = gcd(v[i], x);  // 每次处理 gcd 子环
        for (int j = 0; j < d; j++) {  // 每个子环起点
            int cur = j;
            int circle = 0;
            // 两次转圈：直到回到起点 2 次
            while (circle < 2) {
                int next = (cur + v[i]) % x;
                if (distArr[cur] != INF) {
                    distArr[next] = min(distArr[next], distArr[cur] + v[i]);
                }
                cur = next;
                if (cur == j) circle++;
            }
        }
    }

    // 统计答案
    long long ans = 0;
    for (int i = 0; i < x; i++) {
        if (r >= distArr[i]) {
            ans += max(0LL, (r - distArr[i]) / x + 1);
        }
        if (l >= distArr[i]) {
            ans -= max(0LL, (l - distArr[i]) / x + 1);
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cin >> n;
    cin >> l >> r;
    l--; // 处理 [l+1, r]，所以 l--
    for (int i = 1; i <= n; i++) {
        cin >> v[i];
    }

    cout << compute() << "\n";
    return 0;
}
