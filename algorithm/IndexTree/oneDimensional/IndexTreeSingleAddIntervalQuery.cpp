#include <cstdio>  // 引入标准输入输出库，用于scanf和printf等函数
#include <cstring> // 引入字符串处理库，用于memset函数

// 树状数组单点增加、范围查询模版
// 测试链接 : https://www.luogu.com.cn/problem/P3374

// 定义常量MAXN，表示数组的最大长度，这里设置为500001
const int MAXN = 500001;

// 树状数组（下标从1开始）
// tree[i] 存储的是原数组中 (i - lowbit(i), i] 区间内元素的和
int tree[MAXN];
// n 代表原始数组的元素个数，m 代表操作的次数
int n, m;

// 获取一个数的二进制表示中最低位的1所代表的值
// 例如，lowbit(6) = lowbit(110_2) = 2 (10_2)
// 这是树状数组实现中的一个核心辅助函数
inline int lowbit(int i) {
    return i & -i; // 利用位运算高效计算
}

// 单点增加：将原始数组第 i 个元素的值增加 v
// 这会更新树状数组中所有包含第 i 个元素的节点
void add(int i, int v) {
    // 从当前节点 i 开始，不断向上更新父节点
    // 父节点的索引可以通过 i += lowbit(i) 找到
    while (i <= n) {
        tree[i] += v; // 将当前节点的值增加 v
        i += lowbit(i); // 移动到下一个需要更新的节点（父节点）
    }
}

// 前缀和查询：计算原始数组从第 1 个元素到第 i 个元素的和
int sum(int i) {
    int ans = 0; // 初始化和为 0
    // 从当前节点 i 开始，不断向前累加
    // 前一个区间的索引可以通过 i -= lowbit(i) 找到
    while (i > 0) {
        ans += tree[i]; // 累加当前节点的值
        i -= lowbit(i); // 移动到下一个要求和的区间节点
    }
    return ans; // 返回最终的前缀和
}

// 范围查询：计算原始数组中从第 l 个元素到第 r 个元素的和
// 利用前缀和的思想：[l, r] 的和等于 [1, r] 的和减去 [1, l-1] 的和
int range(int l, int r) {
    return sum(r) - sum(l - 1);
}

int main() {
    // 初始化树状数组
    // 使用memset将tree数组的所有元素初始化为0
    memset(tree, 0, sizeof(tree));

    // 从标准输入读取数组大小 n 和操作次数 m
    scanf("%d%d", &n, &m);

    // 初始化原始数组并构建树状数组
    // 循环 n 次，读取原始数组的每个元素
    for (int i = 1, v; i <= n; i++) {
        scanf("%d", &v); // 读取第 i 个元素的值 v
        add(i, v);      // 通过 add 操作将这个元素的值加入到树状数组中
    }

    // 处理 m 次操作
    for (int i = 1, op, b, c; i <= m; i++) {
        // 读取操作类型 op，以及操作涉及的参数 b 和 c
        scanf("%d%d%d", &op, &b, &c);
        if (op == 1) {
            // 如果操作类型是 1，表示这是一个单点增加操作
            // 将原始数组第 b 个元素的值增加 c
            add(b, c);
        } else {
            // 如果操作类型不是 1 (即为 2)，表示这是一个范围查询操作
            // 查询原始数组中从第 b 个元素到第 c 个元素的和，并打印结果
            printf("%d\n", range(b, c));
        }
    }

    return 0; // 程序正常结束
}
