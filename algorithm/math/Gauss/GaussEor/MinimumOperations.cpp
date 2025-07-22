#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 全变成1的最少操作次数
// 一共有n个点，m条无向边，每个点的初始状态都是0
// 可以操作任意一个点，操作后该点以及相邻点的状态都会改变
// 最终是希望所有点都变成1状态，那么可能会若干方案都可以做到
// 那么其中存在需要最少操作次数的方案，打印这个最少操作次数
// 题目保证一定能做到所有点都变成1状态，并且没有重边和自环
// 1 <= n <= 35
// 1 <= m <= 595
// 测试链接 : https://www.luogu.com.cn/problem/P2962

using namespace std;

const int MAXN = 37;  // 最大节点数

int mat[MAXN][MAXN];  // 增广矩阵，用于高斯消元
int op[MAXN];         // 记录每个节点是否需要操作
int n, ans;           // n为节点数，ans为最少操作次数

// 初始化增广矩阵
void prepare() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            mat[i][j] = 0;  // 清空矩阵
        }
        mat[i][i] = 1;      // 操作节点i会影响自身
        mat[i][n + 1] = 1;  // 增广列，表示目标状态（全为1）
        op[i] = 0;          // 初始化操作数组
    }
}

// 高斯消元解决异或方程组
void gauss(int n) {
    // 对每一行进行消元
    for (int i = 1; i <= n; i++) {
        // 寻找第i列的主元（值为1的元素）
        for (int j = 1; j <= n; j++) {
            // 跳过已经处理过的主元行
            if (j < i && mat[j][j] == 1) {
                continue;
            }
            // 找到第i列值为1的行，与第i行交换
            if (mat[j][i] == 1) {
                // 交换第i行和第j行
                for (int k = 1; k <= n + 1; k++) {
                    swap(mat[i][k], mat[j][k]);
                }
                break;
            }
        }

        // 如果找到主元，进行消元操作
        if (mat[i][i] == 1) {
            // 消除其他行第i列的1
            for (int j = 1; j <= n; j++) {
                if (i != j && mat[j][i] == 1) {
                    // 异或消元
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] ^= mat[i][k];
                    }
                }
            }
        }
    }
}

// 深度优先搜索找最优解
void dfs(int i, int num) {
    // 剪枝：如果当前操作数已经不小于已知最优解，直接返回
    if (num >= ans) {
        return;
    }

    // 递归边界：已经处理完所有变量
    if (i == 0) {
        ans = num;  // 更新最优解
        return;
    }

    // 判断当前变量是主元还是自由元
    if (mat[i][i] == 0) {
        // 当前是自由元
        // 自由元可以自由选择是否操作，尝试两种情况
        op[i] = 0;  // 不操作节点i
        dfs(i - 1, num);

        op[i] = 1;  // 操作节点i
        dfs(i - 1, num + 1);
    } else {
        // 当前是主元
        // 主元的值由自由元决定，需要根据已确定的自由元计算
        int cur = mat[i][n + 1];  // 从目标值开始

        // 减去所有已确定的自由元的影响
        for (int j = i + 1; j <=n ; j++) {
            if (mat[i][j] == 1) {
                cur ^= op[j];  // 异或运算
            }
        }

        // cur就是当前主元必须的操作状态
        dfs(i - 1, num + cur);
    }
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    cin >> n;       // 读入节点数
    prepare();      // 初始化矩阵

    int m;
    cin >> m;       // 读入边数

    // 读入边并建立邻接关系
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        mat[u][v] = 1;  // 操作u会影响v
        mat[v][u] = 1;  // 操作v会影响u（无向图）
    }

    gauss(n);  // 高斯消元

    // 判断方程组是否有唯一解
    int sign = 1;
    for (int i = 1; i <= n; i++) {
        if (mat[i][i] == 0) {
            sign = 0;  // 存在自由元，有多解
            break;
        }
    }

    if (sign == 1) {
        // 唯一解情况：直接统计需要操作的节点数
        ans = 0;
        for (int i = 1; i <= n; i++) {
            if (mat[i][n + 1] == 1) {
                ans++;  // 该节点需要操作
            }
        }
    } else {
        // 多解情况：使用DFS搜索最优解
        ans = n;    // 初始化为最大可能值
        dfs(n, 0);  // 从最后一个变量开始搜索
    }

    cout << ans << endl;  // 输出最少操作次数

    return 0;
}
