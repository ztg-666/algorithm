#include <iostream>
#include <vector>

// 统计元音字母序列的数目
// 给你一个整数n，请你帮忙统计一下我们可以按下述规则形成多少个长度为n的字符串：
// 字符串中的每个字符都应当是小写元音字母（'a', 'e', 'i', 'o', 'u'）
// 每个元音 'a' 后面都只能跟着 'e'
// 每个元音 'e' 后面只能跟着 'a' 或者是 'i'
// 每个元音 'i' 后面 不能 再跟着另一个 'i'
// 每个元音 'o' 后面只能跟着 'i' 或者是 'u'
// 每个元音 'u' 后面只能跟着 'a'
// 由于答案可能会很大，结果对1000000007取模
// 测试链接 : https://leetcode.cn/problems/count-vowels-permutation/

using namespace std;

const int MOD = 1000000007; // 定义模数常量

/**
 * 矩阵相乘函数
 * @param a 第一个矩阵，维度为 n x k
 * @param b 第二个矩阵，维度为 k x m
 * @return 矩阵相乘结果，维度为 n x m
 * 每个元素计算时使用 long long 防止乘法溢出
 */
vector<vector<int>> multiply(vector<vector<int>>& a, vector<vector<int>>& b) {
    int n = a.size();
    int m = b[0].size();
    int k = a[0].size();
    vector<vector<int>> ans(n, vector<int>(m)); // 结果矩阵初始化

    // 经典矩阵乘法三重循环实现
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int c = 0; c < k; c++) {
                ans[i][j] = (int)(((long long)a[i][c] * b[c][j] + ans[i][j]) % MOD);
            }
        }
    }
    return ans;
}

/**
 * 矩阵快速幂算法
 * @param m 底数矩阵
 * @param p 指数
 * @return 矩阵的p次幂结果
 * 使用二进制分解法优化计算，时间复杂度O(logp)
 */
vector<vector<int>> matrixPower(vector<vector<int>> m, int p) {
    int n = m.size();
    vector<vector<int>> ans(n, vector<int>(n));
    // 初始化单位矩阵（矩阵乘法中的乘法单位元）
    for (int i = 0; i < n; i++) ans[i][i] = 1;

    // 快速幂核心逻辑：按二进制位分解指数
    while (p > 0) {
        if (p & 1) { // 当前二进制位为1时累乘结果
            ans = multiply(ans, m);
        }
        m = multiply(m, m); // 矩阵平方
        p >>= 1; // 处理下一位
    }
    return ans;
}

/**
 * 计算元音字母序列的数量
 * 使用矩阵快速幂优化状态转移：
 * 根据题目规则构建状态转移矩阵，通过矩阵快速幂加速计算
 */
int countVowelPermutation(int n) {
    // 初始状态：当n=1时，每个元音各有一种可能
    vector<vector<int>> start = {{1, 1, 1, 1, 1}}; // [a, e, i, o, u]

    // 状态转移矩阵 (根据题目规则构建)
    // 矩阵的每个元素base[i][j]表示从状态j转移到状态i的方案数
    vector<vector<int>> base = {
        {0, 1, 0, 0, 0},  // a后面只能跟e
        {1, 0, 1, 0, 0},  // e后面可以跟a或i
        {1, 1, 0, 1, 1},  // i后面可以跟a/e/o/u
        {0, 0, 1, 0, 1},  // o后面可以跟i或u
        {1, 0, 0, 0, 0}   // u后面只能跟a
    };

    // 计算转移矩阵的(n-1)次幂
    vector<vector<int>> powerMat = matrixPower(base, n - 1);

    // 初始状态矩阵与幂矩阵相乘得到最终状态
    vector<vector<int>> result = multiply(start, powerMat);

    // 求和所有可能的结尾状态
    int total = 0;
    for (int num : result[0]) {
        total = (total + num) % MOD;
    }
    return total;
}

// 测试函数
int main() {
    // 测试n=1到5的情况
    for (int i = 1; i <= 5; i++) {
        cout << " " << i << " : "
             << countVowelPermutation(i) << endl;
    }
    return 0;
}
