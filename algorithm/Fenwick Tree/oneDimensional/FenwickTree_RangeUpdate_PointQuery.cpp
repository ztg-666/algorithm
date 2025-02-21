#include <iostream>
#include <vector>

const int MAXN = 500002; // 定义树状数组的最大大小

// 树状数组，用于高效地进行区间更新和前缀和查询
std::vector<int> tree(MAXN);

int n, m; // n 表示元素个数，m 表示操作次数

// 计算最低有效位（lowbit），用于树状数组的索引跳跃
int lowbit(int i) {
    return i & -i;
}

// 在树状数组中增加值 v 到位置 i
void add(int i, int v) {
    while (i <= n) {
        tree[i] += v; // 更新当前位置的值
        i += lowbit(i); // 移动到下一个需要更新的位置
    }
}

// 计算并返回从 1 到 i 的累加和
int sum(int i) {
    int ans = 0;
    while (i > 0) {
        ans += tree[i]; // 累加当前位置的值
        i -= lowbit(i); // 移动到下一个需要累加的位置
    }
    return ans;
}

int main() {
    std::ios::sync_with_stdio(false); // 关闭 C 风格的输入输出流同步
    std::cin.tie(0); // 解除 cin 和 cout 的绑定，加速输入输出
    std::cout.tie(0);

    std::cin >> n >> m; // 读取元素个数 n 和操作次数 m

    // 初始化树状数组，将每个元素的值加入树状数组
    for (int i = 1, v; i <= n; i++) {
        std::cin >> v; // 读取第 i 个元素的值
        add(i, v); // 将值 v 加入位置 i
        add(i + 1, -v); // 通过加 -v 到位置 i+1 来实现区间更新
    }

    // 处理 m 次操作
    for (int i = 1; i <= m; i++) {
        int op;
        std::cin >> op; // 读取操作类型
        if (op == 1) {
            int l, r, v;
            std::cin >> l >> r >> v; // 读取区间 [l, r] 和要增加的值 v
            add(l, v); // 在位置 l 增加 v
            add(r + 1, -v); // 在位置 r+1 减少 v，实现区间 [l, r] 的增加
        } else {
            int index;
            std::cin >> index; // 读取要查询的位置 index
            std::cout << sum(index) << std::endl; // 输出位置 index 的前缀和
        }
    }
    return 0;
}
