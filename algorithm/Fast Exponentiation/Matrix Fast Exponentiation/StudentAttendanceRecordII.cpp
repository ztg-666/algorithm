#include <iostream>
#include <vector>

// 学生出勤记录II
// 可以用字符串表示一个学生的出勤记录，其中的每个字符用来标记当天的出勤情况（缺勤、迟到、到场）
// 记录中只含下面三种字符：
// 'A'：Absent，缺勤
// 'L'：Late，迟到
// 'P'：Present，到场
// 如果学生能够 同时 满足下面两个条件，则可以获得出勤奖励：
// 按 总出勤 计，学生缺勤（'A'）严格 少于两天
// 学生 不会 存在 连续 3 天或 连续 3 天以上的迟到（'L'）记录。
// 给你一个整数n，表示出勤记录的长度（次数）
// 请你返回记录长度为n时，可能获得出勤奖励的记录情况数量
// 答案可能很大，结果对1000000007取模
// 测试链接 : https://leetcode.cn/problems/student-attendance-record-ii/

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
 * 计算有效出勤记录的数量
 * 使用6维状态转移矩阵：
 * 状态0: 0A, 0L结尾
 * 状态1: 0A, 1L结尾
 * 状态2: 0A, 2L结尾
 * 状态3: 1A, 0L结尾
 * 状态4: 1A, 1L结尾
 * 状态5: 1A, 2L结尾
 * 矩阵快速幂优化递推过程
 */
int checkRecord(int n) {
    // 初始状态：n=1时的合法情况
    // [P, L, A] 对应状态0、1、3
    vector<vector<int>> start = {{1, 1, 0, 1, 0, 0}};

    // 状态转移矩阵 (根据题目规则构建)
    vector<vector<int>> base = {
        // 0    1    2    3    4    5  (当前状态)
        { 1,   1,   0,   1,   0,   0 }, // 添加P后的转移
        { 1,   0,   1,   1,   0,   0 }, // 添加L后的转移
        { 1,   0,   0,   1,   0,   0 }, // 连续L超过限制的情况
        { 0,   0,   0,   1,   1,   0 }, // 添加A后的转移
        { 0,   0,   0,   1,   0,   1 }, // 添加L后的转移（已有A）
        { 0,   0,   0,   1,   0,   0 }  // 连续L超过限制的情况（已有A）
    };

    // 计算转移矩阵的(n-1)次幂
    vector<vector<int>> powerMat = matrixPower(base, n - 1);

    // 初始状态矩阵与幂矩阵相乘得到最终状态
    vector<vector<int>> result = multiply(start, powerMat);

    // 求和所有可能的状态
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
             << checkRecord(i) << endl;
    }
    return 0;
}
