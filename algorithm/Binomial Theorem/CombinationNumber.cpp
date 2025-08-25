#include <iostream>
#include <vector>

// 组合数问题
// 组合公式c(i, j)，表示从i个物品中选出j个物品的方案数
// 如果该数值是k的整数倍，那么称(i, j)是一个合法对
// 给定具体的一组数字n和m，当i和j满足：0 <= i <= n，0 <= j <= min(i, m)
// 返回有多少合法对
// 一共有t组测试，所有测试的k都为同一个值
// 每组测试给定n和m，打印每组测试的答案
// 1 <= t <= 10^4
// 2 <= k <= 21
// 0 <= n、m <= 2000
// 测试链接 : https://www.luogu.com.cn/problem/P2822

using namespace std;

const int MAXV = 2000;  // 最大的n值
const int MAXN = 2002;  // 用于存储组合数的数组大小

int c[MAXN][MAXN];  // 组合数c[i][j]，表示从i个物品中选择j个的组合数
int f[MAXN][MAXN];  // 用于标记c[i][j]是否是k的整数倍，若是则f[i][j] = 1，否则为0
int sum[MAXN][MAXN];  // 前缀和数组，用于快速查询符合条件的合法对

int t, k, n, m;  // t为测试用例数，k为判断是否为合法对的倍数，n和m为每次查询的参数

// 预处理函数，构造组合数表和合法对的前缀和表
void build() {
    // 构造组合数表c[i][j]，计算从i个物品中选择j个的组合数（模k）
    for (int i = 0; i <= MAXV; i++) {
        c[i][0] = 1;  // 组合数c(i, 0) = 1
        for (int j = 1; j <= i; j++) {
            c[i][j] = (c[i - 1][j] + c[i - 1][j - 1]) % k;  // 组合数递推公式
        }
    }

    // 构造f[i][j]，标记合法的组合数
    for (int i = 1; i <= MAXV; i++) {
        for (int j = 1; j <= i; j++) {
            f[i][j] = (c[i][j] % k == 0) ? 1 : 0;  // 若c[i][j]是k的倍数，则合法
        }
    }

    // 构造前缀和数组sum[i][j]
    for (int i = 2; i <= MAXV; i++) {
        for (int j = 1; j <= i; j++) {
            sum[i][j] = sum[i][j - 1] + sum[i - 1][j] - sum[i - 1][j - 1] + f[i][j];
            // sum[i][j]表示从1到i的所有合法对的个数
        }
        sum[i][i + 1] = sum[i][i];  // 确保sum[n][n+1]正确，避免越界
    }
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(0);  // 解除cin与cout的绑定，提高效率

    // 读取输入
    cin >> t >> k;
    build();  // 预处理

    // 处理每个测试用例
    while (t--) {
        cin >> n >> m;
        // 根据m与n的关系，输出对应的结果
        if (m > n) {
            cout << sum[n][n] << '\n';  // 若m > n，则查询sum[n][n]
        } else {
            cout << sum[n][m] << '\n';  // 否则查询sum[n][m]
        }
    }

    return 0;
}
