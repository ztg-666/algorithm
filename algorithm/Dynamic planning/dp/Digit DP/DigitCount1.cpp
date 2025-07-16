#include <iostream>
using namespace std;

// 统计1~num范围内数字d出现的次数
// 参数说明：
// num : 要统计的上界（包含）
// d   : 要统计的数字（0-9）
int count(int num, int d) {
    int ans = 0;           // 累计结果
    //right = 1;         当前位右边的基数（初始为个位）
    //tmp = num;         临时处理数值（用于逐位分解）
    // left, cur;        left-当前位左边的数值，cur-当前位的数字

    // 逐位处理数字，从最低位到最高位
    for (int right = 1, tmp = num, left, cur; tmp != 0; right *= 10, tmp /= 10) {
        left = tmp / 10;   // 当前位左边的数值（高位部分）
        cur = tmp % 10;    // 当前位的数字

        // 处理d=0的特殊情况：左边数值不能全为0
        if (d == 0) {
            left--;
        }

        // 基本组合数计算：左边数值 * 右边基数
        ans += left * right;

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

// 计算区间[a, b]内数字d出现的次数
// 算法原理：前缀和差 => count(b) - count(a-1)
int digitsCount(int d, int a, int b) {
    return count(b, d) - count(a - 1, d);
}

// 测试用例
int main() {
    // 测试1：1-12中数字1出现5次（1,10,11,12中的1）
    cout << digitsCount(1, 1, 12) << endl;   // 输出5

    // 测试2：10-20中数字0出现2次（10,20的个位0）
    cout << digitsCount(0, 10, 20) << endl;  // 输出2

    // 测试3：100-250中数字3出现35次
    // 包含103,113,123,130-139,143,153,163,173,183,193等
    cout << digitsCount(3, 100, 250) << endl;// 输出35
}
