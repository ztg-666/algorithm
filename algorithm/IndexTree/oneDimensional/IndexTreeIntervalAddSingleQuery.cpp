#include <cstdio>  // 引入标准输入输出库，用于scanf和printf等函数
#include <cstring> // 引入字符串处理库，用于memset函数

// 树状数组范围增加、单点查询模版
// 测试链接 : https://www.luogu.com.cn/problem/P3368

// 定义常量MAXN，表示数组的最大长度
// 设置为500002以防止在 add(r + 1, -v) 中当 r=n 时出现数组越界
const int MAXN = 500002;

// 本代码使用树状数组来维护一个“差分数组”。
// 差分数组 D[i] = A[i] - A[i-1]，其中 A 是原始数组。
// 这样做的好处是：
// 1. 对原始数组 A 的一个区间 [l, r] 增加一个值 v，
//    只需要在差分数组上进行两次单点修改：D[l] += v, D[r+1] -= v。
// 2. 查询原始数组 A 中某个位置 i 的值，
//    A[i] = (A[i]-A[i-1]) + (A[i-1]-A[i-2]) + ... + (A[1]-A[0]) = D[1] + D[2] + ... + D[i]。
//    这恰好是差分数组的前缀和。
// 因此，我们可以用树状数组高效地维护差分数组的前缀和，从而实现区间修改和单点查询。

// 树状数组，存储的是差分数组的前缀和信息
int tree[MAXN];
// n 代表原始数组的元素个数，m 代表操作的次数
int n, m;

// 计算一个数的二进制表示中最低位的1所代表的值 (e.g., lowbit(6) = 2)
// 这是树状数组实现中的一个核心辅助函数
inline int lowbit(int i) {
    return i & -i; // 利用位运算高效计算
}

// 在差分数组的第 i 个位置增加 v
// 这会更新树状数组中所有包含该位置的节点
void add(int i, int v) {
    // 从当前节点 i 开始，不断向上更新父节点
    while (i <= n) { // 注意这里的边界是 n，因为差分数组的影响最多到 n+1，但我们只关心到n
        tree[i] += v;
        i += lowbit(i); // 移动到父节点
    }
}

// 计算差分数组的前缀和 (从 D[1] 到 D[i])
// 这个结果就是原始数组 A 在位置 i 的值
int sum(int i) {
    int ans = 0;
    // 从当前节点 i 开始，不断向前累加
    while (i > 0) {
        ans += tree[i];
        i -= lowbit(i); // 移动到前一个区间节点
    }
    return ans; // 返回前缀和
}

int main() {
    // 从标准输入读取数组大小 n 和操作次数 m
    scanf("%d%d", &n, &m);
    // 使用memset将tree数组的所有元素初始化为0
    memset(tree, 0, sizeof(tree));

    // 初始化差分数组
    // 读入原始数组 A 的初始值，并将其转换为对差分数组 D 的修改
    int prev = 0; // a[0] = 0
    for (int i = 1, current; i <= n; i++) {
        scanf("%d", &current); // 读入原始数组的第 i 个元素值 A[i]
        // 计算差分值 D[i] = A[i] - A[i-1]
        // 这里 A[i-1] 是指上一个元素的值。为了简化，可以直接通过两次 add 实现
        // 将 A[i] 加到 i 位置，相当于对区间 [i, n] 都加上 A[i]
        // 再将 -A[i] 加到 i+1 位置，相当于对区间 [i+1, n] 都减去 A[i]
        // 最终效果就是只在位置 i 增加了 A[i] 的值
        // 这一步是构建差分数组的关键
        add(i, current);
        add(i + 1, -current);
    }

    // 处理 m 次操作
    while (m--) {
        int op;
        scanf("%d", &op); // 读取操作类型
        if (op == 1) {
            // 操作 1：区间修改
            // 将原始数组 A 的 [l, r] 区间内所有元素增加 v
            int l, r, v;
            scanf("%d%d%d", &l, &r, &v);
            // 这对应于在差分数组 D 上进行两次单点修改
            add(l, v);       // D[l] 增加 v
            add(r + 1, -v);  // D[r+1] 减少 v
        } else {
            // 操作 2：单点查询
            // 查询原始数组 A 在位置 index 的值
            int index;
            scanf("%d", &index);
            // A[index] 等于差分数组的前缀和 sum(D, 1, index)
            printf("%d\n", sum(index));
        }
    }
    return 0; // 程序正常结束
}
