#include<bits/stdc++.h>
using namespace std;
// 一开始1~n范围上的数字都是0，一共有m个操作，每次操作为(l,r,s,e,d)
// 表示在l~r范围上依次加上首项为s、末项为e、公差为d的数列
// m个操作做完之后，统计1~n范围上所有数字的最大值和异或和
const int MAXN = 10000005; // 定义数组的最大长度

long long arr[MAXN]; // 用于存储差分数组

int n, m; // n 是数组长度，m 是操作次数

// 设置区间 [l, r] 的值，从 s 增加到 e，增量为 d
void set(int l, int r, int s, int e, int d) {
    arr[l] += s; // 区间起点增加 s
    arr[l + 1] += d - s; // 区间起点下一个位置增加增量差
    arr[r + 1] -= d + e; // 区间终点下一个位置减少增量和终点值
    arr[r + 2] += e; // 区间终点后两个位置增加终点值
}

// 构建前缀和数组
void build() {
    for (int i = 1; i <= n; i++) {
        arr[i] += arr[i - 1]; // 第一次前缀和计算
    }
    for (int i = 1; i <= n; i++) {
        arr[i] += arr[i - 1]; // 第二次前缀和计算，得到最终结果
    }
}

int main() {
    ios::sync_with_stdio(false); // 关闭标准IO同步
    cin.tie(0); // 解绑 cin 和 cout
    cout.tie(0); // 解绑 cin 和 cout

    while (cin >> n >> m) { // 循环读取输入
        for (int i = 0, l, r, s, e; i < m; i++) {
            cin >> l >> r >> s >> e; // 读取区间 [l, r] 和初始值 s，最终值 e
            int d = (e - s) / (r - l); // 计算增量 d
            set(l, r, s, e, d); // 调用 set 函数设置区间
        }
        build(); // 构建前缀和数组
    }
    return 0;
}
