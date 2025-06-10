#include <iostream>
#include <cstring>

// 多次查询购买方法
// 一共有4种硬币，面值分别为v0、v1、v2、v3，这个永远是确定的
// 每次去购物的细节由一个数组arr来表示，每次购物都是一次查询
// arr[0] = 携带v0面值的硬币数量
// arr[1] = 携带v1面值的硬币数量
// arr[2] = 携带v2面值的硬币数量
// arr[3] = 携带v3面值的硬币数量
// arr[4] = 本次购物一定要花多少钱
// 返回每次有多少种花钱的方法
// 1 <= v0、v1、v2、v3、arr[i] <= 10^5
// 查询数量 <= 1000
// 测试链接 : https://www.luogu.com.cn/problem/P1450

using namespace std;

const int LIMIT = 100000; // 最大金额限制
long long dp[LIMIT + 1];  // 无限制情况下的方法数
int value[4];             // 硬币面值
int cnt[4];               // 当前查询的硬币数量限制
int n, s;                 // 查询次数和当前目标金额

// 预处理完全背包
void build() {
    memset(dp, 0, sizeof(dp));
    dp[0] = 1; // 初始条件：0元有1种方法（不选任何硬币）

    // 完全背包动态规划
    for (int i = 0; i < 4; ++i) {
        for (int j = value[i]; j <= LIMIT; ++j) {
            dp[j] += dp[j - value[i]];
        }
    }
}

// 使用容斥原理计算合法方法数
long long query() {
    long long illegal = 0;
    // 遍历所有非空子集（1到15，二进制0001到1111）
    for (int status = 1; status <= 15; ++status) {
        long long t = s; // 当前调整后的金额
        int sign = -1;   // 容斥符号

        // 检查每个硬币是否被选中
        for (int j = 0; j < 4; ++j) {
            if ((status >> j) & 1) { // 如果第j位为1
                t -= value[j] * (cnt[j] + 1); // 超过限制的部分
                sign *= -1; // 奇数次减，偶数次加
            }
        }

        if (t >= 0) { // 有效金额
            illegal += dp[t] * sign;
        }
    }
    return dp[s] - illegal;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取硬币面值
    while (cin >> value[0] >> value[1] >> value[2] >> value[3] >> n) {
        build(); // 预处理完全背包

        while (n--) {
            // 读取当前查询的硬币限制和目标金额
            cin >> cnt[0] >> cnt[1] >> cnt[2] >> cnt[3] >> s;
            cout << query() << '\n';
        }
    }

    return 0;
}
