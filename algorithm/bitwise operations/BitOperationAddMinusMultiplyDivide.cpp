#include<bits/stdc++.h>
using namespace std;

// 定义整数最小值常量
static const int MIN = INT_MIN;

// 无进位加法
// 通过异或运算计算无进位加法结果，通过与运算和左移计算进位，不断迭代直到没有进位
static int add(int a, int b) {
    int ans = a;
    while (b != 0) {
        // ans : a和b无进位相加的结果
        ans = a ^ b;
        // b : a和b相加时的进位信息
        b = (a & b) << 1;
        a = ans;
    }
    return ans;
}

// 取反操作
// 对n进行按位取反，然后加1，相当于计算-n
static int neg(int n) {
    return add(~n, 1);
}

// 减法操作
// 通过加法和取反实现减法：a - b = a + (-b)
static int minus2(int a, int b) {
    return add(a, neg(b));
}

// 必须保证a和b都不是整数最小值，返回a除以b的结果
// 使用位移和减法实现除法
static int div2(int a, int b) {
    int x = a < 0 ? neg(a) : a; // 将a转换为正数
    int y = b < 0 ? neg(b) : b; // 将b转换为正数
    int ans = 0;
    for (int i = 30; i >= 0; --i) { // 从最高位开始尝试减法
        if ((x >> i) >= y) { // 如果x右移i位后仍然大于等于y
            ans |= (1 << i); // 将结果的第i位置1
            x = minus2(x, y << i); // 从x中减去y左移i位后的值
        }
    }
    return a < 0 ^ b < 0 ? neg(ans) : ans; // 根据a和b的符号确定结果的符号
}

// 除法操作，不使用算术运算符
// 处理各种特殊情况，如a和b都是最小值，b为最小值，b为-1等
static int divide(int a, int b) {
    if (a == MIN && b == MIN) {
        // a和b都是整数最小
        return 1;
    }
    if (a != MIN && b != MIN) {
        // a和b都不是整数最小，那么正常去除
        return div2(a, b);
    }
    if (b == MIN) {
        // a不是整数最小，b是整数最小
        return 0;
    }
    // a是整数最小，b是-1，返回整数最大，因为题目里明确这么说了
    if (b == neg(1)) {
        return INT_MAX;
    }
    // a是整数最小，b不是整数最小，b也不是-1
    a = add(a, b > 0 ? b : neg(b)); // 将a加上b的绝对值
    int ans = div2(a, b); // 计算新的a除以b的结果
    int offset = b > 0 ? neg(1) : 1; // 根据b的符号确定偏移量
    return add(ans, offset); // 返回最终结果
}

// 乘法操作
// 通过位移和加法实现乘法
static int multiply(int a, int b) {
    int ans = 0;
    while (b != 0) {
        if ((b & 1) != 0) {
            // 考察b当前最右的状态！
            ans = add(ans, a);
        }
        a <<= 1; // a左移一位
        b >>= 1; // b右移一位
    }
    return ans;
}

int main() {
    int a = 15;
    int b = 5;

    printf("Addition of %d and %d is %d\n", a, b, add(a, b));
    printf("Subtraction of %d and %d is %d\n", a, b, minus2(a, b));
    printf("Multiplication of %d and %d is %d\n", a, b, multiply(a, b));
    printf("Division of %d by %d is %d\n", a, b, divide(a, b));

    // Test edge cases
    printf("Division of %d by %d is %d\n", INT_MIN, INT_MIN, divide(INT_MIN, INT_MIN));
    printf("Division of %d by %d is %d\n", INT_MIN, -1, divide(INT_MIN, -1));
    printf("Division of %d by %d is %d\n", INT_MIN, 1, divide(INT_MIN, 1));
    printf("Division of %d by %d is %d\n", INT_MIN, 2, divide(INT_MIN, 2));

    return 0;
}
