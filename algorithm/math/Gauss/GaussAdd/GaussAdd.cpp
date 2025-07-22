#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;

const int MAXN = 52;              // 最大变量个数
const double EPS = 1e-7;          // 精度阈值，小于此值认为是0

double mat[MAXN][MAXN];           // 增广矩阵，存储方程组
int n;                           // 变量个数

/**
 * 交换矩阵的两行
 * @param a 第一行的行号
 * @param b 第二行的行号
 */
void swap_rows(int a, int b) {
    for (int i = 1; i <= n + 1; i++) {
        swap(mat[a][i], mat[b][i]);
    }
}

/**
 * 高斯消元解决加法方程组
 * 将增广矩阵化为行最简阶梯形矩阵
 * @param n 变量个数
 */
void gauss(int n) {
    // 对每一列进行处理
    for (int i = 1; i <= n; i++) {
        // 寻找当前列的主元（绝对值最大的元素）
        int max_row = i;
        for (int j = 1; j <= n; j++) {
            // 如果第j行的对角元素已经被处理过且不为0，跳过
            if (j < i && abs(mat[j][j]) >= EPS) {
                continue;
            }
            // 找到第i列中绝对值最大的元素所在的行
            if (abs(mat[j][i]) > abs(mat[max_row][i])) {
                max_row = j;
            }
        }

        // 将找到的主元行交换到第i行
        swap_rows(i, max_row);

        // 如果主元不为0，进行消元操作
        if (abs(mat[i][i]) >= EPS) {
            // 将主元化为1（行变换：第i行除以主元）
            double tmp = mat[i][i];
            for (int j = i; j <= n + 1; j++) {
                mat[i][j] /= tmp;
            }

            // 消元：将第i列的其他元素都变为0
            for (int j = 1; j <= n; j++) {
                if (i != j) {  // 不处理主元所在行
                    double rate = mat[j][i] / mat[i][i];  // 消元倍数
                    // 第j行 = 第j行 - rate * 第i行
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] -= mat[i][k] * rate;
                    }
                }
            }
        }
    }
}

int main() {
    // 读入变量个数
    cin >> n;

    // 读入增广矩阵
    // mat[i][1] 到 mat[i][n] 存储第i个方程的系数
    // mat[i][n+1] 存储第i个方程的常数项
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n + 1; j++) {
            cin >> mat[i][j];
        }
    }

    // 执行高斯消元
    gauss(n);

    int sign = 1;  // 解的情况标记：1-唯一解，0-多解，-1-无解（矛盾）

    // 检查解的情况
    for (int i = 1; i <= n; i++) {
        // 如果对角元素为0但常数项不为0，说明方程组矛盾（无解）
        // 例如：0*x1 + 0*x2 + ... + 0*xn = 5（矛盾）
        if (abs(mat[i][i]) < EPS && abs(mat[i][n + 1]) >= EPS) {
            sign = -1;  // 矛盾
            break;
        }
        // 如果对角元素为0且常数项也为0，说明有自由变量（多解）
        // 例如：0*x1 + 0*x2 + ... + 0*xn = 0（自由变量）
        if (abs(mat[i][i]) < EPS) {
            sign = 0;   // 多解
        }
    }

    // 根据解的情况输出结果
    if (sign == 1) {
        // 唯一解：输出每个变量的值，保留两位小数
        for (int i = 1; i <= n; i++) {
            cout << "x" << i << "=";
            cout << fixed << setprecision(2) << mat[i][n + 1] << endl;
        }
    } else {
        // 多解输出0，无解输出-1
        cout << sign << endl;
    }

    return 0;
}
