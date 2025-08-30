#include <bits/stdc++.h>

// 康托展开
// 数字从1到n，可以有很多排列，给出具体的一个排列，求该排列的名次，答案对 998244353 取模
// 1 <= n <= 10^6
// 测试链接 : https://www.luogu.com.cn/problem/P5367

using namespace std;

const int MAXN = 1000001;   // 最大数组长度
const int MOD = 998244353;  // 模数

int arr[MAXN];   // 存放输入的排列
int fac[MAXN];   // 阶乘数组，用于计算排列数
int tree[MAXN];  // 树状数组（Fenwick Tree），用于高效求前缀和
int n;           // 排列的大小

// 低位运算，获取树状数组索引跨度
inline int lowbit(int x) {
    return x & -x;
}

// 树状数组：求前缀和 sum(1..i)
int sum(int i) {
    int ans = 0;
    while (i > 0) {
        ans = (ans + tree[i]) % MOD;  // 模运算，避免溢出
        i -= lowbit(i);
    }
    return ans;
}

// 树状数组：在位置 i 上加 v
void add(int i, int v) {
    while (i <= n) {
        tree[i] += v;
        i += lowbit(i);
    }
}

// 计算排列的字典序排名（从 1 开始）
long long compute() {
    // 预处理阶乘 fac[i] = i!
    fac[0] = 1;
    for (int i = 1; i < n; i++) {
        fac[i] = 1LL * fac[i - 1] * i % MOD;
    }

    // 初始化树状数组，每个位置初始频率为 1（表示数值 1..n 都可用）
    for (int i = 1; i <= n; i++) {
        add(i, 1);
    }

    long long ans = 0;
    // 遍历排列 arr[1..n]
    for (int i = 1; i <= n; i++) {
        // 统计比 arr[i] 小的数中还有多少未被使用
        int smaller = sum(arr[i] - 1);

        // 每个比 arr[i] 小的数都能形成 (n - i)! 个排列
        ans = (ans + 1LL * smaller * fac[n - i] % MOD) % MOD;

        // 从树状数组中移除 arr[i]（表示该数字已被使用）
        add(arr[i], -1);
    }

    // 最后 +1，使得结果为“排名”，而不是从 0 开始的字典序索引
    ans = (ans + 1) % MOD;
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];  // 读入排列
    }

    cout << compute() << "\n";  // 输出结果
    return 0;
}
