#include <iostream>
#include <climits>
#include <cmath>
#include <cstdlib>

// 分成k份的最大乘积
// 一个数字n一定要分成k份，得到的乘积尽量大是多少
// 数字n和k，可能非常大，到达10^12规模
// 结果可能更大，所以返回结果对 1000000007 取模

using namespace std;

const int MOD = 1000000007; // 定义全局模数常量

/**
 * 暴力递归解法（仅用于小数据验证）
 * @param rest 剩余需要分割的数字
 * @param k 剩余需要分割的次数
 * @return 返回当前分割方案的最大乘积
 * @note 时间复杂度O(n^k)，仅适用于极小规模数据验证
 */
int f1(int rest, int k) {
    if (k == 1) return rest; // 基准情况：只剩1次分割时直接返回剩余值
    int ans = INT_MIN; // 初始化为最小整数值
    // 遍历所有可能的分割方案（保证剩余数足够后续分割）
    for (int cur = 1; cur <= rest && (rest - cur) >= (k - 1); cur++) {
        int curAns = cur * f1(rest - cur, k - 1); // 递归计算子问题
        ans = max(ans, curAns); // 保留最大值
    }
    return ans;
}

// 暴力解法的入口函数
int maxValue1(int n, int k) {
    return f1(n, k);
}

/**
 * 快速幂算法（带模运算）
 * @param x 底数
 * @param n 指数
 * @return 返回x^n mod MOD的结果
 * @note 使用二进制分解法优化计算效率，时间复杂度O(logn)
 */
long long power(long long x, int n) {
    long long ans = 1;
    while (n > 0) {
        if (n & 1) ans = (ans * x) % MOD; // 处理当前二进制位
        x = (x * x) % MOD; // 平方底数
        n >>= 1; // 右移处理下一位
    }
    return ans;
}

/**
 * 贪心最优解法（数学最优解）
 * @param n 要分割的数字
 * @param k 分割份数
 * @return 返回最大乘积的模值
 * @note 核心思想：均分后处理余数，时间复杂度O(logk)
 */
int maxValue2(int n, int k) {
    long long a = n / k; // 基础分割值
    int b = n % k;       // 余数数量
    // 将余数分配给前b个数（每个加1），剩余k-b个数保持a
    long long part1 = power(a + 1, b);    // (a+1)^b
    long long part2 = power(a, k - b);    // a^(k-b)
    return (int)((part1 * part2) % MOD);  // 合并结果取模
}

/**
 * 对数器验证函数
 * @note 通过随机测试验证两种算法的正确性
 *       使用暴力解法作为基准验证贪心解法
 */
int main() {
    srand(time(0)); // 初始化随机种子
    const int N = 30;       // 最大测试数值
    const int testTimes = 2000; // 测试次数

    cout << "测试开始" << endl;
    for (int i = 1; i <= testTimes; i++) {
        // 生成随机测试用例（保证k <= n）
        int n = rand() % N + 1; // [1, N]
        int k = rand() % n + 1; // [1, n]

        // 计算两种算法的结果
        int ans1 = maxValue1(n, k);
        int ans2 = maxValue2(n, k);

        // 结果对比验证
        if (ans1 != ans2) {
            cout << "n=" << n << " k=" << k
                 << " =" << ans1 << " =" << ans2 << endl;
        }
        // 进度提示
        if (i % 100 == 0) {
            cout << " " << i << " " << endl;
        }
    }
    cout << "" << endl;
    return 0;
}
