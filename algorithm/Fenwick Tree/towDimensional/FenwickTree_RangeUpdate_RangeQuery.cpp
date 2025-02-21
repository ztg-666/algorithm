#include <cstdio>
using namespace std;

// 定义常量，表示二维数组的最大大小
const int MAXN = 2050;
const int MAXM = 2050;

// 定义四个二维数组，用于存储不同维度的前缀和信息
int info1[MAXN][MAXM], info2[MAXN][MAXM], info3[MAXN][MAXM], info4[MAXN][MAXM];
int n, m; // 定义矩阵的行数和列数

// 计算最低有效位（lowbit），用于二进制索引树（Fenwick Tree）的操作
int lowbit(int i) {
    return i & -i;
}

// 在二进制索引树中增加值v到坐标(x, y)
void add(int x, int y, int v) {
    int v1 = v; // 原始值v
    int v2 = x * v; // x * v
    int v3 = y * v; // y * v
    int v4 = x * y * v; // x * y * v
    // 遍历所有受影响的行
    for (int i = x; i <= n; i += lowbit(i)) {
        // 遍历所有受影响的列
        for (int j = y; j <= m; j += lowbit(j)) {
            info1[i][j] += v1; // 更新info1
            info2[i][j] += v2; // 更新info2
            info3[i][j] += v3; // 更新info3
            info4[i][j] += v4; // 更新info4
        }
    }
}

// 计算从(1, 1)到(x, y)的矩形区域的和
int sum(int x, int y) {
    int ans = 0; // 初始化结果
    // 遍历所有受影响的行
    for (int i = x; i > 0; i -= lowbit(i)) {
        // 遍历所有受影响的列
        for (int j = y; j > 0; j -= lowbit(j)) {
            // 计算当前节点的贡献并累加到结果中
            ans += (x + 1) * (y + 1) * info1[i][j] - (y + 1) * info2[i][j] - (x + 1) * info3[i][j] + info4[i][j];
        }
    }
    return ans;
}

// 在二维矩阵中增加值v到矩形区域(a, b)到(c, d)
void add(int a, int b, int c, int d, int v) {
    add(a, b, v); // 增加左上角区域
    add(c + 1, d + 1, v); // 增加右下角区域
    add(a, d + 1, -v); // 减去左下角区域
    add(c + 1, b, -v); // 减去右上角区域
}

// 计算矩形区域(a, b)到(c, d)的和
int range(int a, int b, int c, int d) {
    // 使用容斥原理计算矩形区域的和
    return sum(c, d) - sum(a - 1, d) - sum(c, b - 1) + sum(a - 1, b - 1);
}

int main() {
    char op; // 操作符，'X'表示重新定义矩阵大小，'L'表示增加值，其他表示查询
    int a, b, c, d, v; // 矩形区域的坐标和增加的值
    scanf("%s", &op); // 读取操作符
    scanf("%d%d", &n, &m); // 读取矩阵的行数和列数
    while (scanf("%s", &op) != EOF) { // 循环读取操作
        if (op == 'X') {
            scanf("%d%d", &n, &m); // 重新定义矩阵的行数和列数
        } else if (op == 'L') {
            scanf("%d%d%d%d%d", &a, &b, &c, &d, &v); // 读取矩形区域的坐标和增加的值
            add(a, b, c, d, v); // 增加值到矩形区域
        } else {
            scanf("%d%d%d%d", &a, &b, &c, &d); // 读取矩形区域的坐标
            printf("%d\n", range(a, b, c, d)); // 输出矩形区域的和
        }
    }
    return 0;
}
