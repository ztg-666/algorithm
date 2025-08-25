#include <bits/stdc++.h>

// 杨辉三角
// 给定数字n，打印杨辉三角的前n行
// 1 <= n <= 20
// 测试链接 : https://www.luogu.com.cn/problem/P5732

using namespace std;

// 定义最大行数
const int MAXN = 20;

// 二维数组，用来存储杨辉三角的每个数
long long tri[MAXN][MAXN];

int main() {
    // 加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    // 输入杨辉三角的行数
    cin >> n;

    // 初始化杨辉三角的边界值
    // 第一列和对角线上的元素为1
    for (int i = 0; i < n; i++) {
        tri[i][0] = tri[i][i] = 1;  // tri[i][0] 和 tri[i][i] 都为1
    }

    // 递推公式：tri[i][j] = tri[i-1][j-1] + tri[i-1][j]
    // 填充杨辉三角内部的值
    for (int i = 1; i < n; i++) {
        for (int j = 1; j < i; j++) {
            tri[i][j] = tri[i - 1][j] + tri[i - 1][j - 1];  // 计算每个位置的值
        }
    }

    // 输出杨辉三角
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            cout << tri[i][j] << " ";  // 打印每行的数字
        }
        cout << "\n";  // 每输出完一行换行
    }

    return 0;
}
