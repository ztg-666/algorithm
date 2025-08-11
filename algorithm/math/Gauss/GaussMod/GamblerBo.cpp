#include <iostream>
#include <vector>
#include <algorithm>

// 格子全变成0的操作方案
// 有一个n*m的二维网格，给定每个网格的初始值，一定是0、1、2中的一个
// 如果某个网格获得了一些数值加成，也会用%3的方式变成0、1、2中的一个
// 比如有个网格一开始值是1，获得4的加成之后，值为(1+4)%3 = 2
// 有一个神奇的刷子，一旦在某个网格处刷一下，该网格会获得2的加成
// 并且该网格上、下、左、右的格子，都会获得1的加成
// 最终目标是所有网格都变成0，题目保证一定有解，但不保证唯一解
// 得到哪一种方案都可以，打印一共需要刷几下，并且把操作方案打印出来
// 1 <= n、m <= 30
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=5755


using namespace std;

const int MOD = 3;        // 模数，因为网格值只能是0、1、2
const int MAXS = 1001;    // 最大方程组规模（30*30 + 1）

int mat[MAXS][MAXS];      // 增广矩阵，用于高斯消元
int dir[] = {0, -1, 0, 1, 0}; // 方向数组，用于访问上下左右四个方向
int n, m, s;              // n行m列，s=n*m总格子数
int inv[MOD];             // 模逆数组，用于模运算除法

/**
 * 计算模逆数组
 * 在模3运算中，只有1和2有模逆
 * inv[1] = 1, inv[2] = 2 (因为 2*2 ≡ 1 (mod 3))
 */
void calculateInv() {
    inv[1] = 1;
    for (int i = 2; i < MOD; i++) {
        inv[i] = MOD - (long long)inv[MOD % i] * (MOD / i) % MOD;
    }
}

/**
 * 计算最大公约数
 */
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

/**
 * 构建线性方程组的系数矩阵
 * 每个格子对应一个方程和一个未知数（操作次数）
 * 格子(i,j)的编号为 i*m + j + 1（从1开始编号）
 */
void prepare() {
    // 初始化矩阵为0
    for (int i = 1; i <= s; i++) {
        for (int j = 1; j <= s + 1; j++) {
            mat[i][j] = 0;
        }
    }

    int cur, row, col;
    // 为每个格子建立方程
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cur = i * m + j + 1;  // 当前格子的编号
            mat[cur][cur] = 2;    // 在当前格子操作，自己获得2的加成

            // 检查上下左右四个方向
            for (int d = 0; d <= 3; d++) {
                row = i + dir[d];
                col = j + dir[d + 1];
                // 如果相邻格子在边界内
                if (row >= 0 && row < n && col >= 0 && col < m) {
                    // 在当前格子操作，相邻格子获得1的加成
                    mat[cur][row * m + col + 1] = 1;
                }
            }
        }
    }
}

/**
 * 高斯消元求解线性方程组
 * 使用模3运算
 * @param n 方程组的规模
 */
void gauss(int n) {
    // 对每一列进行消元
    for (int i = 1; i <= n; i++) {
        // 寻找第i列的非零元素作为主元
        for (int j = 1; j <= n; j++) {
            // 如果j<i且mat[j][j]!=0，说明第j行已经处理过了
            if (j < i && mat[j][j] != 0) {
                continue;
            }
            // 找到第i列的非零元素
            if (mat[j][i] != 0) {
                // 交换第i行和第j行
                for (int k = 1; k <= n + 1; k++) {
                    swap(mat[i][k], mat[j][k]);
                }
                break;
            }
        }

        // 如果找到了主元，进行消元操作
        if (mat[i][i] != 0) {
            // 消去第i列下面和上面的所有非零元素
            for (int j = 1; j <= n; j++) {
                if (i != j && mat[j][i] != 0) {
                    // 计算消元系数，避免分数运算
                    int g = gcd(mat[j][i], mat[i][i]);
                    int a = mat[i][i] / g;  // 第j行的倍数
                    int b = mat[j][i] / g;  // 第i行的倍数

                    // 特殊处理：如果j<i且mat[j][j]!=0，需要调整主元系数
                    if (j < i && mat[j][j] != 0) {
                        mat[j][j] = (mat[j][j] * a) % MOD;
                    }

                    // 执行消元：第j行 = 第j行*a - 第i行*b
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
                    }
                }
            }
        }
    }

    // 回代求解：计算每个主元变量的值
    for (int i = 1; i <= n; i++) {
        if (mat[i][i] != 0) {
            // 使用模逆进行除法运算
            mat[i][n + 1] = (mat[i][n + 1] * inv[mat[i][i]]) % MOD;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    calculateInv();  // 预计算模逆

    int test;
    cin >> test;

    for (int t = 1; t <= test; t++) {
        cin >> n >> m;
        s = n * m;  // 总格子数
        prepare();  // 构建系数矩阵

        // 读入初始网格状态，构建增广矩阵的右端向量
        for (int i = 1; i <= s; i++) {
            int val;
            cin >> val;
            // 目标是让每个格子变成0，所以右端是(3-val)%3
            // 这样可以确保 (val + x) % 3 = 0，其中x是需要的加成
            mat[i][s + 1] = (3 - val) % MOD;
        }

        gauss(s);  // 求解线性方程组

        // 计算总操作次数
        int ans = 0;
        for (int i = 1; i <= s; i++) {
            ans += mat[i][s + 1];
        }

        cout << ans << "\n";

        // 输出具体的操作方案
        // id是格子编号，从1开始
        for (int i = 1, id = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++, id++) {
                int operations = mat[id][s + 1];  // 第id个格子需要操作的次数
                // 输出每次操作的位置（行列从1开始）
                while (operations-- > 0) {
                    cout << i << " " << j << "\n";
                }
            }
        }
    }

    return 0;
}
