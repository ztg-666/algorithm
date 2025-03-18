#include <iostream>


static bool isPowerOfTwo(int n) {
    return n > 0 && n == (n & -n);
}
static bool isPowerOfThree(int n) {
    // 如果一个数字是3的某次幂，那么这个数一定只含有3这个质数因子
    // 1162261467是int型范围内，最大的3的幂，它是3的19次方
    // 这个1162261467只含有3这个质数因子，如果n也是只含有3这个质数因子，那么
    // 1162261467 % n == 0
    // 反之如果1162261467 % n != 0 说明n一定含有其他因子
    return n > 0 && 1162261467 % n == 0;
}

static int near2power(int n) {
    if (n <= 0) {
        return 1;
    }
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    if (n >= INT_MAX) {
        return INT_MIN; // 如果结果超出int范围，返回整数最小值
    }
    return n + 1;
}

static int rangeBitwiseAnd(int left, int right) {
    while (left < right) {
        right -= right & -right; // 将 right 减去其最低位的 1
    }
    return right;
}
// 逆序二进制的状态
static int reverseBit(int n) {
    n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
    n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
    n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
    n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
    n = (n >> 16) | (n << 16);
    return n;
}



// 返回n的二进制中有几个1
// 这个实现脑洞太大了
static int cntOnes(int n) {
    n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
    n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
    n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
    return n;
}
// 返回n的二进制中有几个1
// 两个整数之间的 汉明距离 指的是这两个数字对应二进制位不同的位置的数目。
// 给你两个整数 x 和 y，计算并返回它们之间的汉明距离
static int hammingDistance(int x, int y) {
    return cntOnes(x ^ y);
}

int main() {
    // Brian Kernighan算法
    // 提取出二进制里最右侧的1
    // 判断一个整数是不是2的幂
    int number = 16; // 示例数字
    if (isPowerOfTwo(number)) {
        std::cout << number << " 2" << std::endl;
    } else {
        std::cout << number << " !2" << std::endl;
    }
    // 判断一个整数是不是3的幂
    int number2 = 27; // 示例数字
    if (isPowerOfThree(number2)) {
        std::cout << number2 << " 3" << std::endl;
    } else {
        std::cout << number2 << " !3" << std::endl;
    }
    // 已知n是非负数
    // 返回大于等于n的最小的2某次方
    // 如果int范围内不存在这样的数，返回整数最小值
    int number3 = 100;
    std::cout << near2power(number3) << std::endl;

    // 给你两个整数 left 和 right ，表示区间 [left, right]
    // 返回此区间内所有数字 & 的结果
    // 包含 left 、right 端点
    int left = 5;
    int right = 7;
    std::cout << rangeBitwiseAnd(left, right) << std::endl;

    int number4 = 0b10101010101010101010101010101010;
    std::cout << reverseBit(number4) << std::endl;

    int x = 1;
    int y = 4;
    std::cout << "Hamming Distance: " << hammingDistance(x, y) << std::endl;

    return 0;
}
