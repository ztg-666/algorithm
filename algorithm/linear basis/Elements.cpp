#include <bits/stdc++.h>
using namespace std;

// 元素
// 给定n个魔法矿石，每个矿石有状态和魔力，都是整数
// 若干矿石组成的组合能否有效，根据状态异或的结果来决定
// 如果一个矿石组合内部会产生异或和为0的子集，那么这个组合无效
// 返回有效的矿石组合中，最大的魔力和是多少
// 1 <= n <= 1000
// 1 <= 状态 <= 10^18
// 1 <= 魔力 <= 10^4
// 测试链接 : https://www.luogu.com.cn/problem/P4570

const int MAXN = 1001;  // 最大矿石数量
const int BIT = 60;     // 状态最大可能位数（≤ 10^18）

struct Ore {
    long long state; // 矿石状态
    int power;       // 魔力值
};

Ore arr[MAXN];
long long basis[BIT + 1]; // 线性基
int n;

// 尝试将 num 插入线性基
// 如果线性基增加了返回 true，否则返回 false
bool insert(long long num) {
    for (int i = BIT; i >= 0; i--) {
        if ((num >> i) & 1) { // 如果 num 的第 i 位是 1
            if (basis[i] == 0) {
                basis[i] = num; // 该位还没有基向量，直接放入
                return true;
            }
            num ^= basis[i]; // 用已有的基向量消去当前位
        }
    }
    return false; // 可以被现有基向量异或得到，不增加基向量
}

// 计算最大魔力和（即选取能构成最大线性基的矿石组合的最大魔力值）
long long compute() {
    long long ans = 0;
    // 按魔力值从大到小排序
    sort(arr + 1, arr + n + 1, [](const Ore &a, const Ore &b) {
        return a.power > b.power;
    });

    // 依次尝试加入线性基
    for (int i = 1; i <= n; i++) {
        if (insert(arr[i].state)) {
            ans += arr[i].power; // 加入线性基的矿石，累加魔力值
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i].state >> arr[i].power;
    }

    // 输出结果
    cout << compute() << "\n";

    return 0;
}
