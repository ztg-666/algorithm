#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

const int MAXN = 1002;  // 定义最大网格大小

int diff[MAXN][MAXN];  // 差分数组，用于存储差分信息

int n, q;  // n 表示网格的大小，q 表示操作的数量

// 在指定范围内增加值 k
void add(int a, int b, int c, int d, int k) {
    diff[a][b] += k;  // 左上角增加 k
    diff[c + 1][b] -= k;  // 右上角下方减少 k
    diff[a][d + 1] -= k;  // 左下角右侧减少 k
    diff[c + 1][d + 1] += k;  // 右下角下方右侧增加 k
}

// 构建前缀和数组
void build() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            // 当前位置的值等于当前值加上上方和左侧的值，减去重复计算的部分
            diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1];
        }
    }
}

// 清空差分数组
void clear() {
    for (int i = 1; i <= n + 1; i++) {
        for (int j = 1; j <= n + 1; j++) {
            diff[i][j] = 0;  // 将所有元素重置为 0
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 关闭 C 风格输入输出
    cin.tie(0);  // 解除 cin 和 cout 的绑定
    cout.tie(0);  // 解除 cout 和 cin 的绑定

    // 循环读取输入，直到输入结束
    while (cin >> n >> q) {
        // 处理 q 个操作
        for (int i = 1, a, b, c, d; i <= q; i++) {
            cin >> a >> b >> c >> d;  // 读取操作的四个坐标
            add(a, b, c, d, 1);  // 在指定范围内增加值 1
        }
        build();  // 构建前缀和数组

        // 输出结果
        for (int i = 1; i <= n; i++) {
            cout << diff[i][1];  // 输出第一列的值
            for (int j = 2; j <= n; j++) {
                cout << " " << diff[i][j];  // 输出其余列的值
            }
            cout << endl;  // 换行
        }

        clear();  // 清空差分数组以便处理下一个测试用例
    }

    return 0;
}
