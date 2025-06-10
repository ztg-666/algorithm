#include <cstdio>
#include <cstring>

// 连续数字逆元的线性递推
// 给定n、p，求1∼n中所有整数在模p意义下的乘法逆元
// 1 <= n <= 3 * 10^6
// n < p < 20000528
// 输入保证p为质数
// 测试链接 : https://www.luogu.com.cn/problem/P3811

using namespace std;

const int MAXN = 3000001; // 最大数据量（根据题目要求）

int inv[MAXN]; // 逆元存储数组
int n, p;      // 输入参数

// 快速生成逆元数组（线性递推）
void build(int n) {
    inv[1] = 1;
    for (int i = 2; i <= n; ++i) {
        // 递推公式：inv[i] = (p - p/i) * inv[p%i] % p
        // 使用 long long 防止中间结果溢出
        inv[i] = (long long)(p - p/i) * inv[p%i] % p;
    }
}

// 快速读入整数（优化IO）
inline int readInt() {
    int x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar(); // 跳过非数字字符
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = getchar();
    }
    return x;
}

// 快速输出整数（优化IO）
inline void writeInt(int x) {
    if(x == 0) {
        putchar('0');
        return;
    }
    // 数字缓冲区（倒序存储）
    char num[10];
    int len = 0;
    while (x > 0) {
        num[len++] = x % 10 + '0';
        x /= 10;
    }
    // 倒序输出
    while (len--) {
        putchar(num[len]);
    }
    putchar('\n');
}

int main() {
    // 读取输入（使用快速读入）
    n = readInt();
    p = readInt();

    // 生成逆元数组
    build(n);

    // 输出结果（使用快速输出）
    for (int i = 1; i <= n; ++i) {
        writeInt(inv[i]);
    }

    return 0;
}
