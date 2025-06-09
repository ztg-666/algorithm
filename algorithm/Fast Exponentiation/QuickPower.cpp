#include <cstdio>

// 乘法快速幂模版
// 求a的b次方，对p取模的结果
// 测试链接 : https://www.luogu.com.cn/problem/P1226

using namespace std;

/**
 * 快速幂算法，计算 (a^b) mod p
 * @param a 底数
 * @param b 指数
 * @param p 模数
 * @return 计算结果 (a^b) mod p
 * 时间复杂度：O(log b)，通过二进制分解指数优化计算
 */
long long power(long long a, long long b, long long p) {
    long long ans = 1;
    // 当指数b不为0时持续分解
    while (b > 0) {
        // 如果当前二进制位为1（即b为奇数），累乘当前底数
        if (b & 1) {
            ans = (ans * a) % p; // 每次乘后取模防止溢出
        }
        a = (a * a) % p;  // 底数平方（对应二进制位权值）
        b >>= 1;          // 右移处理下一位二进制（相当于b /= 2）
    }
    return ans;
}

int main() {
    long long a, b, p;
    // 读取输入，注意输入顺序为a, b, p
    // 使用%lld确保正确读取long long类型（输入样例：2 10 1000）
    scanf("%lld%lld%lld", &a, &b, &p);

    // 计算并按照格式输出结果（输出样例：2^10 mod 1000=24）
    // 注意输出格式中等号两侧没有空格，与题目要求严格一致
    printf("%lld^%lld mod %lld=%lld\n", a, b, p, power(a, b, p));

    return 0;
}
