#include <iostream>
#include <vector>
using namespace std;

// 矩阵乘法函数
// 参数：a - 第一个矩阵（m×n维），b - 第二个矩阵（n×p维）
// 返回值：矩阵乘积结果（m×p维）
vector<vector<int>> multiply(const vector<vector<int>>& a, const vector<vector<int>>& b) {
    int n = a.size();     // 结果矩阵行数 = a的行数
    int m = b[0].size();  // 结果矩阵列数 = b的列数
    int k = a[0].size();  // 乘积计算次数 = a的列数/b的行数
    vector<vector<int>> ans(n, vector<int>(m, 0));

    // 三重循环实现矩阵乘法
    for (int i = 0; i < n; i++) {       // 遍历结果矩阵行
        for (int j = 0; j < m; j++) {   // 遍历结果矩阵列
            for (int c = 0; c < k; c++) { // 累加乘积
                ans[i][j] += a[i][c] * b[c][j];
            }
        }
    }
    return ans;
}

// 矩阵快速幂函数
// 参数：m - 基底矩阵（必须是方阵），p - 指数
// 返回值：m^p 的结果矩阵
vector<vector<int>> power(vector<vector<int>> m, int p) {
    int n = m.size();
    vector<vector<int>> ans(n, vector<int>(n, 0));

    // 初始化单位矩阵（矩阵乘法中的乘法单位元）
    for (int i = 0; i < n; i++) {
        ans[i][i] = 1;
    }

    // 快速幂算法（二进制分解）
    for (; p != 0; p >>= 1) {
        if ((p & 1) != 0) {  // 当前二进制位为1时累乘
            ans = multiply(ans, m);
        }
        m = multiply(m, m);  // 矩阵平方
    }
    return ans;
}

// 打印矩阵函数
// 参数：m - 要打印的矩阵
// 功能：格式化输出矩阵，保持数字对齐
void print(const vector<vector<int>>& m) {
    for (const auto& row : m) {
        for (int num : row) {
            // 根据数字大小动态调整间距
            if (num < 10) {
                cout << num << "   ";  // 个位数用3空格
            } else if (num < 100) {
                cout << num << "  ";   // 两位数用2空格
            } else {
                cout << num << " ";    // 三位数用1空格
            }
        }
        cout << endl;
    }
}

// 矩阵乘法演示函数
// 展示不同维度矩阵的乘法运算
void f1() {
    // 2x2矩阵相乘
    vector<vector<int>> a = {{1, 3}, {4, 2}};
    vector<vector<int>> b = {{2, 3}, {3, 2}};
    print(multiply(a, b));
    cout << "======" << endl;

    // 2x2矩阵 × 2x3矩阵
    vector<vector<int>> c = {{2, 4}, {3, 2}};
    vector<vector<int>> d = {{2, 3, 2}, {3, 2, 3}};
    print(multiply(c, d));
    cout << "======" << endl;

    // 3x2矩阵 × 2x2矩阵
    vector<vector<int>> e = {{2, 4}, {1, 2}, {3, 1}};
    vector<vector<int>> f = {{2, 3}, {4, 1}};
    print(multiply(e, f));
    cout << "======" << endl;

    // 行向量 × 3x3矩阵
    vector<vector<int>> g = {{3, 1, 2}};
    vector<vector<int>> h = {{1, 2, 1}, {3, 2, 1}, {4, 2, -2}};
    print(multiply(g, h));
}

// 矩阵快速幂演示函数
// 计算矩阵的5次幂
void f2() {
    vector<vector<int>> a = {{1, 2}, {3, 4}};
    print(power(a, 5));
}

// 矩阵乘法解斐波那契数列演示
// 使用矩阵乘法迭代计算斐波那契数列
void f3() {
    vector<vector<int>> start = {{1, 0}};  // 初始状态 [F2, F1]
    vector<vector<int>> m = {{1, 1}, {1, 0}}; // 状态转移矩阵

    // 迭代计算前4个斐波那契数
    vector<vector<int>> a = multiply(start, m);  // [F3, F2]
    print(a);
    cout << "======" << endl;

    vector<vector<int>> b = multiply(a, m);  // [F4, F3]
    print(b);
    cout << "======" << endl;

    vector<vector<int>> c = multiply(b, m);  // [F5, F4]
    print(c);
    cout << "======" << endl;

    vector<vector<int>> d = multiply(c, m);  // [F6, F5]
    print(d);
}

// 矩阵快速幂解斐波那契数列演示
// 使用快速幂直接计算指定位置的斐波那契数
void f4() {
    vector<vector<int>> start = {{1, 0}};  // 初始状态 [F2, F1]
    vector<vector<int>> m = {{1, 1}, {1, 0}}; // 状态转移矩阵

    // 分别计算第3-6项斐波那契数
    vector<vector<int>> a = multiply(start, power(m, 1));  // [F3, F2]
    print(a);
    cout << "======" << endl;

    vector<vector<int>> b = multiply(start, power(m, 2));  // [F4, F3]
    print(b);
    cout << "======" << endl;

    vector<vector<int>> c = multiply(start, power(m, 3));  // [F5, F4]
    print(c);
    cout << "======" << endl;

    vector<vector<int>> d = multiply(start, power(m, 4));  // [F6, F5]
    print(d);
}
int main() {
    cout << "f1() : " << endl;
    f1();
    cout << "end" << endl << endl;

    cout << "f2() : " << endl;
    f2();
    cout << "end" << endl << endl;

    cout << "f3() : " << endl;
    f3();
    cout << "end" << endl << endl;

    cout << "f4() : " << endl;
    f4();
    cout << "end" << endl;

    return 0;
}
