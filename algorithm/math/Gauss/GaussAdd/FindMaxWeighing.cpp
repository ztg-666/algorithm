#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// 有一次错误称重求最重物品
// 一共有n个物品，编号1~n，定义合法方案如下：
// 1，每个物品的重量都是确定的
// 2，每个物品的重量一定都是正整数
// 3，最重的物品有且仅有1个
// 每次称重格式类似：3 2 5 6 10，代表本次称重涉3个物品，编号为2、5、6，总重量10
// 一共有n+1条称重数据，称重数据整体有效的条件为：
// 错误的称重数据有且仅有1条，只有排除这条错误称重，才能求出一种合法方案
// 如果称重数据有效，打印最重三角形的编号
// 如果称重数据无效，打印"illegal"
// 1 <= m <= n <= 100
// 测试链接 : https://www.luogu.com.cn/problem/P5027

using namespace std;

const int MAXN = 102;           // 最大物品数量
const double EPS = 1e-7;        // 浮点数精度控制

int dt[MAXN][MAXN];           // 存储称重数据的系数矩阵
double mat[MAXN][MAXN];         // 高斯消元用的浮点数矩阵
int n;                          // 物品数量

/**
 * 高斯消元法解线性方程组
 * @param n 方程组的未知数个数（物品个数）
 *
 * 算法流程：
 * 1. 对于每一行，找到当前列绝对值最大的元素作为主元
 * 2. 将主元所在行交换到当前行
 * 3. 将主元化为1，其他行对应位置化为0
 */
void gauss(int n) {
    // 对每一个未知数进行消元
    for (int i = 1; i <= n; i++) {
        int maxRow = i;  // 记录当前列中绝对值最大元素的行号

        // 寻找当前列(第i列)中绝对值最大的元素，用作主元
        for (int j = 1; j <= n; j++) {
            // 如果该行的对角元素已经被处理过，跳过
            if (j < i && abs(mat[j][j]) >= EPS) {
                continue;
            }
            // 找到绝对值更大的元素
            if (abs(mat[j][i]) > abs(mat[maxRow][i])) {
                maxRow = j;
            }
        }

        // 如果主元不在当前行，交换行
        if (maxRow != i) {
            for (int k = 1; k <= n + 1; k++) {
                swap(mat[i][k], mat[maxRow][k]);
            }
        }

        // 如果主元不为0，进行消元操作
        if (abs(mat[i][i]) >= EPS) {
            double tmp = mat[i][i];
            // 将当前行的主元化为1
            for (int j = i; j <= n + 1; j++) {
                mat[i][j] /= tmp;
            }

            // 将其他行对应位置化为0
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

/**
 * 检查高斯消元的解是否为合法方案
 * @return 如果合法返回最重物品的编号，否则返回0
 *
 * 合法方案的条件：
 * 1. 方程组有唯一解（对角线元素都不为0）
 * 2. 所有物品重量都是正整数
 * 3. 最重物品有且仅有一个
 */
int check() {
    gauss(n);  // 先进行高斯消元

    double maxv = -1e9;  // 记录最大重量
    int maxt = 0;        // 记录有多少个物品达到最大重量
    int ans = 0;         // 记录最重物品的编号

    // 检查每个物品的重量
    for (int i = 1; i <= n; i++) {
        // 检查是否有唯一解（对角线元素不能为0）
        if (abs(mat[i][i]) < EPS) {
            return 0;  // 无解或无穷解
        }

        // 检查重量是否为正整数
        if (mat[i][n + 1] <= 0 || abs(mat[i][n + 1] - round(mat[i][n + 1])) > EPS) {
            return 0;  // 重量不是正整数
        }

        // 更新最大重量和对应的物品编号
        if (maxv < mat[i][n + 1]) {
            maxv = mat[i][n + 1];
            maxt = 1;
            ans = i;
        } else if (abs(maxv - mat[i][n + 1]) < EPS) {
            maxt++;  // 有多个物品达到最大重量
        }
    }

    // 检查最重物品是否唯一
    if (maxt > 1) {
        return 0;  // 最重物品不唯一
    }
    return ans;
}

/**
 * 交换数据矩阵中的两行
 * @param i 第一行的索引
 * @param j 第二行的索引
 */
void swapdt(int i, int j) {
    for (int k = 1; k <= n + 1; k++) {
        swap(dt[i][k], dt[j][k]);
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;  // 读入物品数量

    // 初始化数据矩阵为全0
    for (int i = 1; i <= n + 1; i++) {
        for (int j = 1; j <= n + 1; j++) {
            dt[i][j] = 0;
        }
    }

    // 读入n+1条称重数据
    for (int i = 1; i <= n + 1; i++) {
        int m;  // 本次称重涉及的物品数量
        cin >> m;

        // 读入涉及的物品编号，在对应位置标记为1
        for (int j = 1; j <= m; j++) {
            int cur;
            cin >> cur;
            dt[i][cur] = 1;  // 物品cur参与了第i次称重
        }

        cin >> dt[i][n + 1];  // 读入总重量，存储在增广矩阵的最后一列
    }

    int ans = 0;     // 最终答案
    int times = 0;   // 有效方案的数量

    // 尝试删除每一条称重数据，看是否能得到合法方案
    for (int k = 1; k <= n + 1; k++) {
        // 将第k条数据移到最后（相当于排除这条数据）
        swapdt(k, n + 1);

        // 将前n条数据复制到浮点数矩阵中进行高斯消元
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n + 1; j++) {
                mat[i][j] = dt[i][j];
            }
        }

        // 恢复数据顺序
        swapdt(k, n + 1);

        // 检查当前方案是否合法
        int cur = check();
        if (cur != 0) {
            times++;   // 合法方案数量加1
            ans = cur; // 记录答案
        }
    }

    // 判断最终结果
    if (times != 1) {
        // 如果合法方案不是唯一的，数据无效
        cout << "illegal" << endl;
    } else {
        // 如果有且仅有一个合法方案，输出最重物品编号
        cout << ans << endl;
    }
    
    return 0;
}
