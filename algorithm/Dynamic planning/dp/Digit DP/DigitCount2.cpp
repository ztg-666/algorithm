#include <iostream>
using namespace std;

/*
 * 范围内的数字计数
 * 给定两个正整数a和b，计算区间[a,b]内每个数字(0-9)出现的次数
 * 算法核心：分位数位统计，时间复杂度O(logN)
 * 测试链接：https://www.luogu.com.cn/problem/P2602
 */

// 统计1~num范围内数字d出现的次数
// 参数说明：
// num : 要统计的上界（包含）
// d   : 要统计的数字（0-9）
long long count(long long num, int d) {
    long long ans = 0;        // 累计结果
    // right = 1;     当前位右边的基数（初始为个位）
    //tmp = num;      临时处理数值（用于逐位分解）

    for (long right = 1, tmp = num, left, cur; tmp != 0; right *= 10, tmp /= 10) {
        left = tmp / 10;   // 当前位左边的数值（高位部分）

        // 处理d=0的特殊情况：左边数值不能全为0
        if (d == 0) {
            left--;
        }
        // 基本组合数计算：左边数值 * 右边基数
        ans += left * right;
        cur = tmp % 10;    // 当前位的数字
        // 当前位情况分析
        if (cur > d) {
            // 情况1：当前位数字大于d，可以增加right种可能
            ans += right;
        } else if (cur == d) {
            // 情况2：当前位等于d，增加右边实际数值+1
            ans += num % right + 1; // +1包含自身情况
        }
    }
    return ans;
}

/*
 * 计算区间[a, b]内数字d出现的次数
 * 算法原理：前缀和差 => count(b) - count(a-1)
 * 示例：求[5,12]中2的次数 = count(12,2) - count(4,2)
 */
long long digitsCount(int d, long long a, long long b) {
    // 当a为1时，a-1=0的特殊处理已包含在count函数中
    return count(b, d) - count(a - 1, d);
}

/*
 * 主函数：处理输入输出
 * 输入优化：禁用同步，解除cin/cout绑定
 * 输出格式：0-8号数字空格分隔，9号单独换行
 */
int main() {
    // 输入输出优化（提升IO性能）
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b;
    // 循环处理多组测试用例
    while (cin >> a >> b) {
        // 输出0-8号数字统计结果（空格分隔）
        for (int i = 0; i < 9; ++i) {
            cout << digitsCount(i, a, b) << " ";
        }
        // 单独处理9号数字并换行
        cout << digitsCount(9, a, b) << "\n";
    }

    return 0;
}
