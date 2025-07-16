#include <iostream>
using namespace std;

// 数字1的个数
// 给定一个整数n
// 计算所有小于等于n的非负整数中数字1出现的个数
// 测试链接 : https://leetcode.cn/problems/number-of-digit-one/

// 通用数字计数函数
// num: 上界数值，d: 目标数字
long long count(int num, int d) {
    long long ans = 0;        // 结果累加器
    // right = 1;       当前位权值（个位初始为1）
    // tmp = num;       临时数值用于逐位处理

    for (long long right = 1, tmp = num, left, cur; tmp != 0; right *= 10, tmp /= 10) {
        left = tmp / 10;   // 当前位左侧数值
        cur = tmp % 10;    // 当前位数字

        // 处理d=0的特殊情况（非本题需求但保留通用性）
        if (d == 0) {
            left--;
        }

        // 计算左侧组合数贡献
        ans += left * right;

        // 处理当前位的三种情况
        if (cur > d) {
            ans += right;            // 情况1：当前位大于d
        } else if (cur == d) {
            ans += num % right + 1;  // 情况2：当前位等于d
        }
    }
    return ans;
}

// 计算数字1出现的次数
int countDigitOne(int n) {
    return count(n, 1);
}


/*
示例测试：
输入：13
计算过程：
1. 处理个位（cur=3）：
   left=1, ans += 1*1 =1
   cur>1 → ans +=1 → total=2
2. 处理十位（cur=1）：
   left=0, ans +=0*10=0
   cur==1 → ans +=13%10+1=3+1=4 → total=6
输出：6
*/

// 测试用例
int main() {
    cout << countDigitOne(13) << endl;   // 输出6
    cout << countDigitOne(0) << endl;    // 输出0
    cout << countDigitOne(20) << endl;   // 输出12
}
