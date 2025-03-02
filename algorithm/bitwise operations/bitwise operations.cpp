#include <iostream>

class BinarySystem {
public:
    // 打印一个int类型的数字，32位二进制的状态
    // 左侧是高位，右侧是低位
    static void printBinary(int num) {
        for (int i = 31; i >= 0; i--) {
            // 下面这句写法，可以改成 :
            // std::cout << ((num & (1 << i)) != 0 ? "1" : "0");
            // 但不可以改成 :
            // std::cout << ((num & (1 << i)) == 1 ? "1" : "0");
            // 因为num如果第i位有1，那么(num & (1 << i))是2的i次方，而不一定是1
            // 比如，num = 0010011
            // num的第0位是1，第1位是1，第4位是1
            // (num & (1<<4)) == 16（不是1），说明num的第4位是1状态
            std::cout << ((num & (1 << i)) == 0 ? "0" : "1");
        }
        std::cout << std::endl;
    }

    static void main() {
        // 非负数
        int a = 78;
        std::cout << a << std::endl;
        printBinary(a);
        std::cout << "===a===" << std::endl;
        // 负数
        int b = -6;
        std::cout << b << std::endl;
        printBinary(b);
        std::cout << "===b===" << std::endl;
        // 直接写二进制的形式定义变量
        int c = 0b1001110;
        std::cout << c << std::endl;
        printBinary(c);
        std::cout << "===c===" << std::endl;
        // 直接写十六进制的形式定义变量
        // 0100 -> 4
        // 1110 -> e
        // 0x4e -> 01001110
        int d = 0x4e;
        std::cout << d << std::endl;
        printBinary(d);
        std::cout << "===d===" << std::endl;
        // ~、相反数
        std::cout << a << std::endl;
        printBinary(a);
        printBinary(~a);
        int e = ~a + 1;
        std::cout << e << std::endl;
        printBinary(e);
        std::cout << "===e===" << std::endl;
        // int、long的最小值，取相反数、绝对值，都是自己
        int f = INT_MIN;
        std::cout << f << std::endl;
        printBinary(f);
        std::cout << -f << std::endl;
        printBinary(-f);
        std::cout << ~f + 1 << std::endl;
        printBinary(~f + 1);
        std::cout << "===f===" << std::endl;
        // | & ^
        int g = 0b0001010;
        int h = 0b0001100;
        printBinary(g | h);
        printBinary(g & h);
        printBinary(g ^ h);
        std::cout << "===g h===" << std::endl;
        // 可以这么写 : int num = 3231 | 6434;
        // 可以这么写 : int num = 3231 & 6434;
        // 不能这么写 : int num = 3231 || 6434;
        // 不能这么写 : int num = 3231 && 6434;
        // 因为 ||、&& 是 逻辑或、逻辑与，只能连接bool类型
        // 不仅如此，|、& 连接的两侧一定都会计算
        // 而 ||、&& 有穿透性的特点
        std::cout << "test1" << std::endl;
        bool test1 = returnTrue() | returnFalse();
        std::cout << "test1" << test1 << std::endl;
        std::cout << "test2" << std::endl;
        bool test2 = returnTrue() || returnFalse();
        std::cout << "test2" << test2 << std::endl;
        std::cout << "test3" << std::endl;
        bool test3 = returnFalse() & returnTrue();
        std::cout << "test3" << test3 << std::endl;
        std::cout << "test4" << std::endl;
        bool test4 = returnFalse() && returnTrue();
        std::cout << "test4" << test4 << std::endl;
        std::cout << "===| & || &&===" << std::endl;
        // <<
        int i = 0b0011010;
        printBinary(i);
        printBinary(i << 1);
        printBinary(i << 2);
        printBinary(i << 3);
        std::cout << "===i << ===" << std::endl;
        // 非负数 >>
        printBinary(i);
        printBinary(i >> 2);
        printBinary(i >> 2); // 右移操作符>>默认为算术右移
        std::cout << "===i >>===" << std::endl;
        // 负数 >>
        int j = 0b11110000000000000000000000000000;
        printBinary(j);
        printBinary(j >> 2);
        printBinary(j >> 2); //右移操作符>>默认为算术右移
        std::cout << "===j >>===" << std::endl;
        // 非负数 << 1，等同于乘以2
        // 非负数 << 2，等同于乘以4
        // 非负数 << 3，等同于乘以8
        // 非负数 << i，等同于乘以2的i次方
        // ...
        // 非负数 >> 1，等同于除以2
        // 非负数 >> 2，等同于除以4
        // 非负数 >> 3，等同于除以8
        // 非负数 >> i，等同于除以2的i次方
        // 只有非负数符合这个特征，负数不要用
        int k = 10;
        std::cout << k << std::endl;
        std::cout << (k << 1) << std::endl;
        std::cout << (k << 2) << std::endl;
        std::cout << (k << 3) << std::endl;
        std::cout << (k >> 1) << std::endl;
        std::cout << (k >> 2) << std::endl;
        std::cout << (k >> 3) << std::endl;
        std::cout << "===k===" << std::endl;
    }

private:
    static bool returnTrue() {
        std::cout << "returnTrue" << std::endl;
        return true;
    }

    static bool returnFalse() {
        std::cout << "returnFalse" << std::endl;
        return false;
    }
};

int main() {
    BinarySystem::main();
    return 0;
}
