#include <vector>
using namespace std;

// 不含连续1的非负整数
// 给定一个正整数n，请你统计在[0, n]范围的非负整数中
// 有多少个整数的二进制表示中不存在连续的1
// 测试链接 : https://leetcode.cn/problems/non-negative-integers-without-consecutive-ones/

// 递归辅助函数：计算<=num且不存在连续1的数字个数
// cnt: 预计算的斐波那契数列式辅助数组
// num: 当前处理的数值
// i: 当前处理的二进制位（从最高位开始）
int f(int cnt[], int num, int i) {
    if (i == -1) {
        return 1; // 所有位处理完成，当前路径合法
    }
    int ans = 0;
    // 如果当前位为1，可以累加当前位设为0时的所有可能情况
    if ((num & (1 << i)) != 0) {
        ans += cnt[i]; // 累加当前位设为0时的合法数数量
        // 如果前一位也是1，后续不可能出现合法情况，提前返回
        if ((num & (1 << (i + 1))) != 0) {
            return ans;
        }
    }
    // 递归处理下一位（保持当前位的实际值）
    ans += f(cnt, num, i - 1);
    return ans;
}

// 方法1：递归解法
// 时间复杂度：O(logN)
int findIntegers1(int n) {
    // cnt[len]表示长度为len位的二进制数中不含连续1的数字个数
    int cnt[31] = {0};
    cnt[0] = 1; // 0位数字（空数字）视为合法
    cnt[1] = 2; // 1位数字：0、1
    // 构建斐波那契数列式计数数组
    for (int len = 2; len <= 30; len++) {
        cnt[len] = cnt[len - 1] + cnt[len - 2]; // 状态转移方程
    }
    return f(cnt, n, 30); // 从最高位开始处理
}

// 方法2：迭代解法（避免递归栈溢出）
// 时间复杂度：O(logN)
int findIntegers2(int n) {
    int cnt[31] = {0};
    cnt[0] = 1;
    cnt[1] = 2;
    for (int len = 2; len <= 30; len++) {
        cnt[len] = cnt[len - 1] + cnt[len - 2];
    }
    int ans = 0;
    bool prev_bit_set = false; // 记录前一位是否被设置

    // 从最高位（30位）开始遍历到-1（处理最后一位）
    for (int i = 30; i >= -1; i--) {
        if (i == -1) { // 所有位处理完成
            ans++;    // 原数字自身合法的情况
            break;
        }
        // 检查当前位是否为1
        if ((n & (1 << i)) != 0) {
            ans += cnt[i]; // 累加当前位设为0时的合法数数量
            // 如果连续两位都是1，后续不可能有合法情况，终止循环
            if ((n & (1 << (i + 1))) != 0) {
                break;
            }
        }
    }
    return ans;
}
