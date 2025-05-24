#include <vector>

// 完全没有重复的数字个数
// 给定正整数n，返回在[1, n]范围内每一位都互不相同的正整数个数
// 测试链接 : https://leetcode.cn/problems/count-special-integers/

using namespace std;
// 递归函数：计算当前前缀下符合条件的数字数量
// 参数说明：
// cnt    : 不同剩余位数对应的排列数数组
// num    : 原始n值
// len    : 剩余需要确定的位数
// offset : 当前处理位的权值（如千位对应1000）
// status : 位掩码，记录已使用的数字（bit 0表示数字0是否已用）
int f(int* cnt, int num, int len, int offset, int status) {
    if (len == 0) {
        return 1; // 所有位数都已确定，且满足条件
    }

    int ans = 0;
    int first = (num / offset) % 10; // 获取当前处理的数字位

    // 处理所有小于当前位的可选数字
    for (int cur = 0; cur < first; cur++) {
        if ((status & (1 << cur)) == 0) { // 检查数字是否未被使用
            ans += cnt[len - 1]; // 加上剩余位数的排列数
        }
    }

    // 处理等于当前位的情况（递归处理下一位）
    if ((status & (1 << first)) == 0) { // 当前位数字未被使用
        ans += f(cnt, num, len - 1, offset / 10, status | (1 << first));
    }

    return ans;
}

// 主函数：统计1~n范围内各位不重复的数字个数
int countSpecialNumbers(int n) {
    // 特殊情况处理：当n<10时直接返回n
    if (n < 10) return n;

    // 计算数字n的位数（len）和最高位权值（offset）
    int len = 1;
    int offset = 1;
    int tmp = n / 10;
    while (tmp > 0) {
        len++;
        offset *= 10;
        tmp /= 10;
    }

    // 生成排列数数组cnt
    // cnt[i]表示：剩余i位时，可用的排列数
    int* cnt = new int[len];
    cnt[0] = 1; // base case
    for (int i = 1, k = 10 - len + 1; i < len; i++, k++) {
        // 递推公式：剩余i位的排列数 = 前一位的排列数 * 可用数字数量
        cnt[i] = cnt[i - 1] * k;
    }

    int ans = 0;
    // 计算位数小于len的所有情况
    if (len >= 2) {
        ans = 9; // 1位数的数量
        for (int i = 2, a = 9, b = 9; i < len; i++, b--) {
            a *= b;    // 9*9*8*...的递推计算
            ans += a;  // 累加不同位数的排列数
        }
    }

    // 处理位数等于len的情况
    int first = n / offset; // 获取最高位数字
    // 1. 最高位取小于first的情况
    ans += (first - 1) * cnt[len - 1];
    // 2. 最高位取等于first的情况（递归处理后续位数）
    ans += f(cnt, n, len - 1, offset / 10, 1 << first);

    delete[] cnt; // 释放动态数组内存
    return ans;
}
