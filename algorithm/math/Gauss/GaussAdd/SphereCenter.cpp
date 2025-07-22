#include <iostream>
#include <iomanip>
#include <cmath>

// 球形空间的中心点
// 如果在n维空间中，那么表达一个点的位置，需要n个坐标的值
// 现在给定n+1个点，每个点都有n个坐标的值，代表在n维空间中的位置
// 假设这n+1个点都在n维空间的球面上，请返回球心的位置
// 球心的位置当然也是n个坐标的值，打印出来
// 在n维空间中，计算任意两点的距离，请用经典的欧式距离
// 1 <= n <= 10
// 坐标信息精确到小数点后6位，绝对值都不超过20000
// 测试链接 : https://www.luogu.com.cn/problem/P4035

using namespace std;

const int MAXN = 12;
double dt[MAXN][MAXN];
double mat[MAXN][MAXN];
int n;
const double sml = 1e-7;

// 高斯消元解决线性方程组
void gauss(int n) {
    for (int i = 1; i <= n; i++) {
        int max_row = i;
        for (int j = 1; j <= n; j++) {
            if (j < i && abs(mat[j][j]) >= sml) {
                continue;
            }
            if (abs(mat[j][i]) > abs(mat[max_row][i])) {
                max_row = j;
            }
        }
        // 交换行
        if (max_row != i) {
            for (int k = 1; k <= n + 1; k++) {
                swap(mat[i][k], mat[max_row][k]);
            }
        }
        
        if (abs(mat[i][i]) >= sml) {
            double tmp = mat[i][i];
            for (int j = i; j <= n + 1; j++) {
                mat[i][j] /= tmp;
            }
            for (int j = 1; j <= n; j++) {
                if (i != j) {
                    double rate = mat[j][i] / mat[i][i];
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] -= mat[i][k] * rate;
                    }
                }
            }
        }
    }
}

int main() {
    cin >> n;
    
    // 读取n+1个点的坐标
    for (int i = 1; i <= n + 1; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> dt[i][j];
        }
    }
    
    // 构建线性方程组的系数矩阵
    for (int i = 1; i <= n; i++) {
        mat[i][n + 1] = 0; // 初始化右端项
        for (int j = 1; j <= n; j++) {
            mat[i][j] = 2 * (dt[i][j] - dt[i + 1][j]);
            mat[i][n + 1] += dt[i][j] * dt[i][j] - dt[i + 1][j] * dt[i + 1][j];
        }
    }
    
    // 求解线性方程组
    gauss(n);
    
    // 输出结果
    for (int i = 1; i <= n; i++) {
        cout << fixed << setprecision(3) << mat[i][n + 1];
        if (i < n) cout << " ";
    }
    cout << endl;
    
    return 0;
}
