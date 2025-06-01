#include <iostream>
using namespace std;

// 砍竹子II
// 现需要将一根长为正整数bamboo_len的竹子砍为若干段
// 每段长度均为正整数
// 请返回每段竹子长度的最大乘积是多少
// 答案需要对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/jian-sheng-zi-ii-lcof/

const int MOD = 1000000007; // 题目要求的模数

/**
 * 快速幂取模算法
 * @param x 底数
 * @param n 指数
 * @return x^n % MOD 的结果
 */
long long power(long long x, int n) {
    long long ans = 1;
    while (n > 0) {
        if (n & 1) { // 当二进制位为1时累乘结果
            ans = (ans * x) % MOD;
        }
        x = (x * x) % MOD; // 平方底数
        n >>= 1; // 右移处理下一位
    }
    return ans;
}

/**
 * 计算竹子切割的最大乘积
 * @param n 竹子原始长度
 * @return 最大乘积结果（取模后）
 */
int cuttingBamboo(int n) {
    // 处理特殊情况
    if (n == 2) return 1;  // 只能切成1+1
    if (n == 3) return 2;  // 只能切成1+2

    int tail; // 存储余数对应的处理值
    if (n % 3 == 0) {      // 能被3整除时，余数为0
        tail = 1;          // 不需要特殊处理
    } else if (n % 3 == 1) { // 余数为1时，拆分为3+1不如2+2乘积大
        tail = 4;            // 取出一个3与1组成4（2+2）
    } else {               // 余数为2时直接保留
        tail = 2;
    }

    // 计算3的幂次方（总长度 = 3*power_val + tail）
    int power_val = (tail == 1 ? n : (n - tail)) / 3;
    // 计算结果：3^power_val * tail % MOD
    return (int)(power(3, power_val) * tail % MOD);
}

/**
 * 测试函数
 * 验证典型测试用例：
 * 4 -> 2*2=4
 * 5 -> 3*2=6
 * 10-> 3*3*2*2=36
 */
int main() {
    cout << cuttingBamboo(4) << endl;   // 预期输出4
    cout << cuttingBamboo(5) << endl;   // 预期输出6
    cout << cuttingBamboo(10) << endl;  // 预期输出36
    return 0;
}
