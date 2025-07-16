#include <vector>

using namespace std;

// 至少有1位重复的数字个数
// 给定正整数n，返回在[1, n]范围内至少有1位重复数字的正整数个数
// 测试链接 : https://leetcode.cn/problems/numbers-with-repeated-digits/

// 递归函数：计算当前前缀下满足条件的数字数量
// 参数：
//   cnt    - 预计算的排列数数组，cnt[i]表示剩余i位时的排列数
//   num    - 原始输入的n值，用于获取当前位的参考值
//   len    - 剩余需要确定的位数
//   offset - 当前处理位的权值（如处理百位时offset=100）
//   status - 位掩码标记已使用的数字（bit 0对应数字0，bit 1对应数字1，依此类推）
// 返回值：当前前缀下满足无重复条件的数字数量
int f(int* cnt, int num, int len, int offset, int status) {
    if (len == 0) return 1; // 基准情况：所有位数已确定

    int ans = 0;
    int first = (num / offset) % 10; // 获取当前处理位的数字

    // 遍历所有小于当前位的候选数字
    for (int cur = 0; cur < first; cur++) {
        // 检查数字是否可用（对应bit位为0表示未使用）
        if (!(status & (1 << cur))) {
            // 累加剩余位数的排列数（确定当前位后，剩余len-1位的可能性）
            ans += cnt[len - 1];
        }
    }

    // 处理等于当前位的情况（需递归处理下一位）
    if (!(status & (1 << first))) { // 当前位数字尚未使用
        // 递归处理下一位：更新offset，标记当前位已使用
        ans += f(cnt, num, len - 1, offset / 10, status | (1 << first));
    }

    return ans;
}

// 计算1~n范围内各位数字不重复的正整数个数
// 算法思路：
//   1. 分位数长度处理：先计算位数少于n的情况，再处理位数相同的情况
//   2. 使用排列数预计算优化时间复杂度
//   3. 时间复杂度：O(logn)（基于数字位数而非数值大小）
int countSpecialNumbers(int n) {
    // 处理边界情况：1位数的个数等于n本身
    if (n < 10) return n;

    // 计算n的位数(len)和最高位的权值(offset)
    // 示例：n=5432 → len=4, offset=1000
    int len = 1, offset = 1;
    for (int tmp = n/10; tmp > 0; tmp /= 10) {
        len++;
        offset *= 10;
    }

    // 预计算排列数数组cnt（动态分配内存）
    // cnt[i]表示：剩余i位时，可用的数字排列数
    // 计算方式：cnt[i] = cnt[i-1] * (可用数字数量)
    int* cnt = new int[len]{1}; // 初始化cnt[0] = 1
    for (int i=1, k=10-len+1; i<len; i++, k++) {
        cnt[i] = cnt[i-1] * k;
    }

    // 计算位数少于len的情况总数
    // 示例：len=4时，计算1位、2位、3位数的总数
    int ans = len >= 2 ? 9 : 0; // 初始化1位数的数量（1-9）
    for (int i=2, a=9, b=9; i<len; i++, b--) {
        a *= b;   // 计算i位数的排列数（9*9*8...）
        ans += a; // 累加不同位数的数量
    }

    // 处理位数等于len的情况（分两种子情况）
    int first = n / offset; // 获取最高位数字
    // 情况1：最高位取小于first的数字
    ans += (first - 1) * cnt[len-1]; // (first-1)个可选数字 × 剩余位的排列数
    // 情况2：最高位取等于first的数字（递归处理剩余位）
    ans += f(cnt, n, len-1, offset/10, 1 << first); // 标记最高位已使用

    delete[] cnt; // 释放动态数组内存
    return ans;
}

// 计算至少有一位重复数字的数量
// 实现策略：总数 - 无重复数字数 = 至少有一个重复的数
// 时间复杂度：与countSpecialNumbers相同，O(logn)
int numDupDigitsAtMostN(int n) {
    return n - countSpecialNumbers(n);
}




