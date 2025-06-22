#include <cstdio>
#include <cstring>

using namespace std;

// 定义常量，表示数组的最大容量
const int MAXN = 100001;

// info1: 维护原始数组的差分数组 D[i] = A[i] - A[i-1]
// 通过树状数组，可以快速查询 D 的前缀和。
long long info1[MAXN];

// info2: 维护一个辅助信息 (i-1) * D[i]
// 这个信息用于在计算原始数组A的前缀和时，抵消掉一个多余的项。
long long info2[MAXN];

int n; // 数组的大小
int m; // 操作的数量

/**
 * @brief 计算一个整数在二进制表示下最低位的1所代表的值。
 * 这是树状数组操作的核心，用于确定每次更新/查询时要跳转到的下一个索引。
 * 例如, lowbit(8) = lowbit(1000_2) = 8
 * lowbit(7) = lowbit(0111_2) = 1
 * @param i 输入的整数。
 * @return 最低位的1所代表的值。
 */
int lowbit(int i) {
    return i & -i; // 利用二进制补码的特性进行计算
}

/**
 * @brief 在指定的树状数组中对单个元素进行更新（增加一个值）。
 * @param tree 要更新的树状数组 (info1 或 info2)。
 * @param i 要更新的元素的索引（从1开始）。
 * @param v 要增加的值。
 */
void add(long long tree[], int i, long long v) {
    // 从当前索引 i 开始，不断向其父节点更新
    while (i <= n) {
        tree[i] += v;
        i += lowbit(i); // 跳转到下一个需要更新的节点
    }
}

/**
 * @brief 查询指定树状数组的前缀和（从索引1到i的和）。
 * @param tree 要查询的树状数组 (info1 或 info2)。
 * @param i 前缀的结束索引。
 * @return 索引1到i的元素总和。
 */
long long sum(long long tree[], int i) {
    long long ans = 0;
    // 从当前索引 i 开始，不断累加其所有子节点的值
    while (i > 0) {
        ans += tree[i];
        i -= lowbit(i); // 跳转到下一个要求和的节点
    }
    return ans;
}

/**
 * @brief 对原始数组的指定区间 [l, r] 内的每个元素都增加一个值 v。
 * 这通过更新差分数组来实现：
 * - 在 D[l] 处加上 v，使得从 A[l] 开始的所有元素都增加了 v。
 * - 在 D[r+1] 处减去 v，以抵消从 A[r+1] 开始的元素受到的影响。
 * @param l 区间的左边界。
 * @param r 区间的右边界。
 * @param v 要增加的值。
 */
void addRange(int l, int r, long long v) {
    // 更新 info1 (差分数组 D)
    add(info1, l, v);
    add(info1, r + 1, -v);

    // 更新 info2 (辅助数组 (i-1)*D[i])
    add(info2, l, (l - 1) * v);
    add(info2, r + 1, -r * v); // 此处是 -r*v 而不是 -(r+1-1)*v，因为这是对 D[r+1] 的更新
}

/**
 * @brief 查询原始数组在区间 [l, r] 内的元素总和。
 * 查询利用了差分数组和两个树状数组。
 * 原始数组 A 的前缀和 S[k] = Σ(A[i]) for i=1 to k
 * 可以推导为: S[k] = k * Σ(D[i]) - Σ((i-1)*D[i]) for i=1 to k
 * Σ(D[i]) 就是 sum(info1, k)
 * Σ((i-1)*D[i]) 就是 sum(info2, k)
 * 所以 S[k] = k * sum(info1, k) - sum(info2, k)
 * 区间 [l, r] 的和就是 S[r] - S[l-1]。
 * @param l 区间的左边界。
 * @param r 区间的右边界。
 * @return 区间 [l, r] 内所有元素的总和。
 */
long long queryRange(int l, int r) {
    // 计算前缀和 S[r]
    long long part1 = sum(info1, r) * r - sum(info2, r);
    // 计算前缀和 S[l-1]
    long long part2 = sum(info1, l - 1) * (l - 1) - sum(info2, l - 1);
    // 两者相减得到区间和
    return part1 - part2;
}

int main() {
    // 读取数组大小和操作数量
    scanf("%d%d", &n, &m);

    // 初始化原始数组
    // 读入每个初始值 cur，并将其视为在 [i, i] 区间上增加 cur
    // 这样可以将初始化统一为区间增加操作
    long long cur;
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &cur);
        addRange(i, i, cur); // 使用 addRange 进行单点初始化
    }

    // 处理 m 次操作
    int op, l, r;
    long long v;
    for (int i = 1; i <= m; i++) {
        scanf("%d", &op); // 读取操作类型
        if (op == 1) {
            // 操作 1: 区间增加
            scanf("%d%d%lld", &l, &r, &v);
            addRange(l, r, v);
        } else {
            // 操作 2: 区间查询
            scanf("%d%d", &l, &r);
            printf("%lld\n", queryRange(l, r));
        }
    }
    return 0;
}
