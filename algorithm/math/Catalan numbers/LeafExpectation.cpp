#include <bits/stdc++.h>

// 叶子节点数的期望
// 一共有n个节点，认为节点之间无差别，能形成很多不同结构的二叉树
// 假设所有不同结构的二叉树，等概率出现一棵，返回叶子节点的期望
// 1 <= n <= 10^9
// 答案误差小于10的-9次方
// 测试链接 : https://www.luogu.com.cn/problem/P3978

using namespace std;

int main() {
    // 输入的节点个数 n (范围 1 <= n <= 1e9)
    long long n;
    cin >> n;

    // 根据题意，所有不同结构的二叉树等概率出现
    // 叶子节点数的期望公式是：
    // E = n * (n + 1) / ( (2n - 1) * 2 )
    // 这里用 double 做浮点运算，避免整数除法截断
    double ans = (double)n * (n + 1) / ((2.0 * n - 1) * 2.0);

    // 按照题目要求，输出结果保留 9 位小数
    cout << fixed << setprecision(9) << ans << endl;

    return 0;
}
