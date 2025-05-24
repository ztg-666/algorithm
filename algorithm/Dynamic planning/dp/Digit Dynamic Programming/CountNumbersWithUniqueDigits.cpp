

// 统计各位数字都不同的数字个数
// 给你一个整数n，代表十进制数字最多有n位
// 如果某个数字，每一位都不同，那么这个数字叫做有效数字
// 返回有效数字的个数，不统计负数范围
// 测试链接 : https://leetcode.cn/problems/count-numbers-with-unique-digits/

// 计算各位数字都不同的数字个数
// 参数说明：
//   - n : 十进制数字的最大位数
// 返回值说明：
//   - 返回[0, 10^n)范围内所有有效数字的个数
int countNumbersWithUniqueDigits(int n) {
    // 处理边界情况：当n=0时，只有0这一个有效数字
    if (n == 0) {
        return 1;
    }
    
    // 初始化结果为10个1位有效数字（0-9）
    int ans = 10;
    
    // 组合数学计算：
    // 从2位数字开始计算，每次循环处理k位数的情况
    // s : 当前位数的有效数字组合数（初始化为9对应2位数的第一位选择）
    // i : 当前位数可用的数字数量（初始化为9对应第二位选择）
    // k : 当前处理的数字位数（从2开始）
    for (int s = 9, i = 9, k = 2; k <= n; i--, k++) {
        s *= i;      // 计算k位数的有效组合数（累乘递减）
        ans += s;    // 将k位数的有效组合数累加到结果
    }
    // 注意：当n>9时，超过10位的数字必然有重复
    return ans;
}
#include <iostream>

int main() {
    // 测试用例数组
    int test_cases[] = {0, 1, 2, 3, 5, 9, 10};

    std::cout << "" << std::endl;
    for (int n : test_cases) {
        std::cout << "n = " << n << " -> "
                  << countNumbersWithUniqueDigits(n) << std::endl;
    }

    return 0;
}