#include <cstdio>

// 树屋阶梯
// 地面高度是0，想搭建一个阶梯，要求每一个台阶上升1的高度，最终到达高度n
// 有无穷多任意规格的矩形材料，但是必须选择n个矩形，希望能搭建出阶梯的样子
// 返回搭建阶梯的不同方法数，答案可能很大，不取模！就打印真实答案
// 1 <= n <= 500
// 测试链接 : https://www.luogu.com.cn/problem/P2532

// 定义高精度结构体，存储大数
struct HighPrec {
    int L, A[10001];  // L 表示高精度数的长度，A[] 存储各位数字（逆序存储）
};

// 初始化高精度数，返回一个初值为 1 的高精度
inline HighPrec Init() {
    HighPrec H;
    H.L = 1, H.A[1] = 1;  // 初始化为 1
    return H;
}

// 高精度乘以单精度整数 k
HighPrec Mul(HighPrec A, int k) {
    HighPrec H;

    // 先乘每一位
    for (int i = 1; i <= A.L; i++)
        H.A[i] = A.A[i] * k;  // 乘法：直接乘 k

    // 处理进位问题
    for (int i = 2; i <= A.L; i++) {
        H.A[i] += H.A[i - 1] / 10;  // 当前位加上进位
        H.A[i - 1] %= 10;  // 保留个位数字
    }

    H.L = A.L;

    // 处理最高位是否需要进位，可能会增加位数
    while (H.A[H.L] >= 10) {
        H.A[H.L + 1] = H.A[H.L] / 10;  // 进位到下一位
        H.A[H.L] %= 10;  // 保留当前位的个位数
        H.L++;  // 增加高精度数的长度
    }

    return H;
}

// 高精度除以单精度整数 k
HighPrec Div(HighPrec A, int k) {
    HighPrec H;
    int t = 0;  // 用于保存竖式的临时变量

    // 从高位开始计算
    for (int i = A.L; i >= 1; i--) {
        t = t * 10 + A.A[i];  // 竖式的处理，将每一位数字组合成临时值
        H.A[i] = t / k;  // 当前位结果
        t %= k;  // 余数更新
    }

    H.L = A.L;

    // 如果最高位为零，则减少长度
    while (H.A[H.L] == 0)
        H.L--;  // 去除不必要的前导零

    return H;
}

// 输出高精度数
void OutPut(HighPrec A) {
    // 逆序输出高精度数
    for (int i = A.L; i >= 1; i--)
        printf("%d", A.A[i]);
}

HighPrec T;
int n;

int main() {
    // 输入整数 n
    scanf("%d", &n);

    T = Init();  // 初始化高精度数 T 为 1

    // 使用公式的简化版本进行计算
    // 公式：T = (n+2) * (n+3) * ... * (2n) / (1 * 2 * ... * n)
    // 计算乘法部分 (n+2) * (n+3) * ... * (2n)
    for (int i = n + 2; i <= n * 2; i++)
        T = Mul(T, i);

    // 计算除法部分 1 * 2 * ... * n
    for (int i = 1; i <= n; i++)
        T = Div(T, i);

    // 输出最终结果
    OutPut(T);
    return 0;
}
