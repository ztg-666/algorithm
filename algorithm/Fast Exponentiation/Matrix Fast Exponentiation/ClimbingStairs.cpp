#include <vector>
using namespace std;
using ll = long long;

/**
 * 矩阵乘法函数
 * @param a 第一个矩阵，维度为 n x k
 * @param b 第二个矩阵，维度为 k x m
 * @return 矩阵乘积结果，维度为 n x m
 */
vector<vector<ll>> multiply(const vector<vector<ll>>& a, const vector<vector<ll>>& b) {
    ll n = a.size();
    ll m = b[0].size();
    ll k = a[0].size();
    vector<vector<ll>> ans(n, vector<ll>(m, 0));
    // 三重循环实现矩阵乘法
    for (ll i = 0; i < n; i++) {       // 遍历第一个矩阵的行
        for (ll j = 0; j < m; j++) {   // 遍历第二个矩阵的列
            for (ll c = 0; c < k; c++) { // 计算点积
                ans[i][j] += a[i][c] * b[c][j];
            }
        }
    }
    return ans;
}

/**
 * 矩阵快速幂算法
 * @param m 基底矩阵，必须是方阵
 * @param p 指数
 * @return 矩阵的p次幂结果
 */
vector<vector<ll>> power(vector<vector<ll>> m, ll p) {
    ll n = m.size();
    vector<vector<ll>> ans(n, vector<ll>(n, 0));
    // 初始化单位矩阵（矩阵乘法中的乘法单位元）
    for (ll i = 0; i < n; i++) {
        ans[i][i] = 1;
    }
    // 快速幂算法，时间复杂度 O(log p)
    while (p > 0) {
        if ((p & 1) != 0) {  // 当最低位为1时，累积结果
            ans = multiply(ans, m);
        }
        m = multiply(m, m);  // 矩阵平方
        p >>= 1;             // 右移一位
    }
    return ans;
}

/**
 * 使用矩阵快速幂求解爬楼梯问题（斐波那契数列变种）
 * 时间复杂度：O(log n)
 * 空间复杂度：O(1)（固定大小的矩阵）
 * @param n 楼梯阶数
 * @return 不同爬法的总数
 */
ll climbStairs(ll n) {
    if (n == 0 || n == 1) {  // 边界条件处理
        return 1;
    }
    // 初始状态向量 [F(2), F(1)] = [2, 1]
    vector<vector<ll>> start = {{1, 1}};
    // 递推矩阵 [[1, 1], [1, 0]]，用于快速计算斐波那契数列
    vector<vector<ll>> base = {
        {1, 1},
        {1, 0}
    };
    // 通过矩阵快速幂计算：start * (base)^(n-1)
    vector<vector<ll>> ans = multiply(start, power(base, n - 1));
    return ans[0][0];  // 返回结果矩阵的第一个元素
}
