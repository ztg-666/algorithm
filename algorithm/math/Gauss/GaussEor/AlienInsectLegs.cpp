#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

// 外星千足虫
// 一共有n种虫子，编号1~n，虫子腿为奇数认为是外星虫，偶数认为是地球虫
// 一共有m条虫子腿的测量记录，记录编号1~m
// 比如其中一条测量记录为，011 1，表示1号虫没参与，2号、3号虫参与了，总腿数为奇数
// 测量记录保证不会有自相矛盾的情况，但是可能有冗余的测量结果
// 也许拥有从第1号到第k号测量记录就够了，k+1~m号测量记录有或者没有都不影响测量结果
// 打印这个k，并且打印每种虫子到底是外星虫还是地球虫
// 如果使用所有的测量结果，依然无法确定每种虫子的属性，打印"Cannot Determine"
// 1 <= n <= 1000
// 1 <= m <= 2000
// 测试链接 : https://www.luogu.com.cn/problem/P2447

using namespace std;

// 外星千足虫问题
// 使用高斯消元法解决异或方程组，判断每种虫子是外星虫(奇数腿)还是地球虫(偶数腿)

const int BIT = 64;           // 每个long long有64位
const int MAXN = 2002;        // 最大行数
const int MAXM = MAXN / BIT + 1;  // 每行需要的long long数量

long long mat[MAXN][MAXM];    // 位图矩阵，用于存储系数矩阵和常数项
int n, m, s;                  // n种虫子，m条记录，s=max(n,m)
int need;                     // 需要的最少测量记录数

/**
 * 设置矩阵中(row, col)位置的值
 * @param row 行号
 * @param col 列号
 * @param v   值(0或1)
 */
void set(int row, int col, int v) {
    if (v == 0) {
        // 将对应位设置为0
        mat[row][col / BIT] &= ~(1LL << (col % BIT));
    } else {
        // 将对应位设置为1
        mat[row][col / BIT] |= 1LL << (col % BIT);
    }
}

/**
 * 获取矩阵中(row, col)位置的值
 * @param row 行号
 * @param col 列号
 * @return 该位置的值(0或1)
 */
int get(int row, int col) {
    return ((mat[row][col / BIT] >> (col % BIT)) & 1) == 1 ? 1 : 0;
}

/**
 * 异或操作：row2行 = row2行 ^ row1行
 * @param row1 源行
 * @param row2 目标行
 * @param bits 总位数
 */
void eor(int row1, int row2, int bits) {
    // 对每个long long段进行异或
    for (int k = 0; k <= bits / BIT; k++) {
        mat[row2][k] ^= mat[row1][k];
    }
}

/**
 * 交换两行
 * @param a 行a
 * @param b 行b
 */
void swap_row(int a, int b) {
    for (int k = 0; k < MAXM; k++) {
        swap(mat[a][k], mat[b][k]);
    }
}

/**
 * 高斯消元法求解异或方程组
 * 将矩阵化为上三角形式，检查是否有唯一解
 * @param n 矩阵大小
 */
void gauss(int n) {
    need = 0;  // 记录需要的最大行号

    // 对每一列进行消元
    for (int i = 1; i <= n; i++) {
        // 寻找当前列中第i行及以下有1的行
        for (int j = i; j <= n; j++) {
            if (get(j, i) == 1) {
                swap_row(i, j);        // 交换到第i行
                need = max(need, j);   // 更新需要的最大行号
                break;
            }
        }

        // 如果主元为0，说明无唯一解
        if (get(i, i) == 0) {
            return;
        }

        // 消元：将其他行的第i列元素变为0
        for (int j = 1; j <= n; j++) {
            if (i != j && get(j, i) == 1) {
                // 异或消元
                // 总共需要异或n+2位（包括系数和常数项）
                eor(i, j, n + 2);
            }
        }
    }
}

int main() {
    // 优化输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入虫子数量和测量记录数量
    cin >> n >> m;
    s = max(n, m);  // 矩阵大小取较大值

    // 初始化矩阵为全0
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXM; j++) {
            mat[i][j] = 0;
        }
    }

    // 读入每条测量记录
    for (int i = 1; i <= m; i++) {
        string line;  // 01字符串，表示哪些虫子参与测量
        int result;   // 测量结果：0表示总腿数为偶数，1表示总腿数为奇数
        cin >> line >> result;

        // 设置系数矩阵：每个虫子对应一列
        for (int j = 1; j <= n; j++) {
            set(i, j, line[j - 1] - '0');
        }
        // 设置常数项（放在第s+1列）
        set(i, s + 1, result);
    }

    // 执行高斯消元
    gauss(s);

    // 检查是否有唯一解
    int sign = 1;  // 标记是否有唯一解
    for (int i = 1; i <= n; i++) {
        if (get(i, i) == 0) {  // 主对角线元素为0，说明无唯一解
            sign = 0;
            break;
        }
    }

    // 输出结果
    if (sign == 0) {
        cout << "Cannot Determine" << endl;
    } else {
        cout << need << endl;  // 输出需要的最少测量记录数

        // 输出每种虫子的类型
        for (int i = 1; i <= n; i++) {
            if (get(i, s + 1) == 1) {
                cout << "?y7M#" << endl;  // 外星虫（奇数腿）
            } else {
                cout << "Earth" << endl;  // 地球虫（偶数腿）
            }
        }
    }

    return 0;
}
