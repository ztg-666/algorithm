#include <cstdio>
#include <algorithm>
#include <cstring>

// HH的项链问题
// 问题描述：给定n个位置的项链，每个位置有一种颜色
// 有m个查询，每个查询问某个区间[l,r]内有多少种不同的颜色
// 解法：使用树状数组+离线处理，按查询右端点排序处理

using namespace std;

const int MAXN = 1000001;

int arr[MAXN];          // 存储每个位置的颜色
int tree[MAXN];         // 树状数组，用于快速计算区间和
int last_occur[MAXN];   // 记录每种颜色最后出现的位置
int ans[MAXN];          // 存储每个查询的答案

// 查询结构体
struct Query {
    int l, r;      // 查询区间的左右端点
    int idx;       // 原始查询的编号（用于还原顺序）
} q[MAXN];

int n, m;  // n个位置，m个查询

// 树状数组的核心函数：获取x的最低位1
inline int lowbit(int x) {
    return x & -x;
}

// 树状数组：在位置i上加上值v
void add(int i, int v) {
    while (i <= n) {
        tree[i] += v;
        i += lowbit(i);  // 向上更新父节点
    }
}

// 树状数组：计算前缀和[1, i]
int sum(int i) {
    int res = 0;
    while (i > 0) {
        res += tree[i];
        i -= lowbit(i);  // 向下找需要的节点
    }
    return res;
}

// 比较函数：按查询的右端点升序排序
// 这是离线处理的关键：从左到右扫描，按右端点处理查询
bool cmp(const Query& a, const Query& b) {
    return a.r < b.r;
}

int main() {
    // 读入数据
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }

    scanf("%d", &m);
    for (int i = 1; i <= m; i++) {
        scanf("%d %d", &q[i].l, &q[i].r);
        q[i].idx = i;  // 记录原始查询编号
    }

    // 核心步骤1：按查询右端点排序
    sort(q + 1, q + m + 1, cmp);

    // 核心步骤2：初始化
    memset(last_occur, 0, sizeof(last_occur));  // 所有颜色初始都没出现过

    int s = 1;  // 当前处理到的位置指针

    // 核心步骤3：处理每个查询
    for (int i = 1; i <= m; i++) {
        // 将位置指针s移动到当前查询的右边界
        while (s <= q[i].r) {
            int color = arr[s];  // 当前位置的颜色

            // 关键思路：对于每种颜色，只在树状数组中保留最右边的出现位置
            // 这样可以保证查询区间[l,r]时，每种颜色最多只被计算一次

            // 如果这种颜色之前出现过，先移除之前的贡献
            if (last_occur[color] != 0) {
                add(last_occur[color], -1);  // 在之前位置减1
            }

            // 在当前位置添加这种颜色的贡献
            add(s, 1);

            // 更新这种颜色的最后出现位置
            last_occur[color] = s;
            s++;
        }

        // 计算区间[l, r]内不同颜色的数量
        // 利用前缀和：sum(r) - sum(l-1)
        ans[q[i].idx] = sum(q[i].r) - sum(q[i].l - 1);
    }

    // 按原始查询顺序输出结果
    for (int i = 1; i <= m; i++) {
        printf("%d\n", ans[i]);
    }

    return 0;
}
