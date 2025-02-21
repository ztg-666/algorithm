#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 100001; // 定义最大数组长度

// 维护原始数组的差分信息：Di
vector<long long> info1(MAXN, 0);

// 维护原始数组的差分加工信息：(i-1) * Di
vector<long long> info2(MAXN, 0);

int n, m; // n 表示数组长度，m 表示操作次数

// 计算最低有效位
int lowbit(int i) {
    return i & -i;
}

// 在树状数组中增加值
void add(vector<long long>& tree, int i, long long v) {
    while (i <= n) {
        tree[i] += v; // 更新树状数组中的值
        i += lowbit(i); // 移动到下一个需要更新的位置
    }
}

// 计算树状数组中前 i 个元素的和
long long sum(const vector<long long>& tree, int i) {
    long long ans = 0;
    while (i > 0) {
        ans += tree[i]; // 累加树状数组中的值
        i -= lowbit(i); // 移动到前一个需要累加的位置
    }
    return ans;
}

// 原始数组中 [l..r] 每个数值 + v
void add(int l, int r, long long v) {
    add(info1, l, v); // 更新差分数组 info1
    add(info1, r + 1, -v); // 更新差分数组 info1
    add(info2, l, (l - 1) * v); // 更新差分加工数组 info2
    add(info2, r + 1, -(r * v)); // 更新差分加工数组 info2
}

// 原始数组中 [l..r] 范围上的累加和
long long range(int l, int r) {
    // 计算范围 [l..r] 的累加和
    return sum(info1, r) * r - sum(info2, r) - sum(info1, l - 1) * (l - 1) + sum(info2, l - 1);
}

int main() {
    ios::sync_with_stdio(false); // 关闭 C 风格的输入输出
    cin.tie(0); // 解除 cin 和 cout 的绑定，加速输入输出
    cout.tie(0);

    cin >> n >> m; // 读取数组长度 n 和操作次数 m
    long long cur;
    for (int i = 1; i <= n; i++) {
        cin >> cur; // 读取数组中的每个元素
        add(i, i, cur); // 初始化树状数组
    }
    long long v;
    for (int i = 1, op, l, r; i <= m; i++) {
        cin >> op; // 读取操作类型
        if (op == 1) {
            cin >> l >> r >> v; // 读取范围 [l..r] 和增加值 v
            add(l, r, v); // 执行范围增加操作
        } else {
            cin >> l >> r; // 读取范围 [l..r]
            cout << range(l, r) << "\n"; // 输出范围 [l..r] 的累加和
        }
    }
    return 0;
}
