#include <cstdio>     // 引入标准输入输出库，用于 scanf 和 printf
#include <cstring>    // 引入字符串处理库，这里虽然用得不多，但通常会包含
#include <algorithm>  // 引入算法库，主要用于 std::swap 函数


// 色板游戏
// 一共有L个色板，编号1~L，一开始所有色板都是1号颜色
// 一共有T种颜色，编号1~T，可以往色板上涂色
// 一共有O次操作，操作类型有如下两种
// 操作 C A B C : A~B范围的色板都涂上C颜色 
// 操作 P A B   : 查询A~B范围的色板一共有几种颜色
// L <= 10^5, T <= 30, O <= 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P1558

using namespace std;  // 使用标准命名空间，避免每次写 std::

const int MAXN = 100001; // 定义最大区间长度，通常线段树数组大小会是 MAXN 的 4 倍左右

// --- 线段树的核心数据数组 ---
// color[i]：存储线段树节点 i 所代表的区间内包含的颜色集合的位掩码（bitmask）。
//           例如，如果颜色1和颜色3存在，那么 color[i] = (1 << 1) | (1 << 3)。
//           颜色0通常不用，颜色1-T。如果颜色值是v，对应的位掩码是 1 << v。
int color[MAXN << 2];
// change[i]：懒更新的值。如果 update[i] 为 true，则表示节点 i 及其代表的整个区间都应该被涂成 change[i] 存储的颜色。
int change[MAXN << 2];
// update[i]：懒更新标记。如果为 true，表示节点 i 有一个待下传的懒更新操作。
bool update[MAXN << 2];


// --- 辅助函数：将子节点信息上推到父节点 ---
// 这个函数负责根据左右子节点的颜色状态，更新父节点的颜色状态。
// 由于父节点的颜色集合是左右子节点颜色集合的并集，所以使用按位或（OR）操作。
void up(int i) {
    // 父节点的颜色位掩码 = 左子节点的颜色位掩码 | 右子节点的颜色位掩码
    color[i] = color[i << 1] | color[i << 1 | 1];
}

// --- 辅助函数：下传懒更新标记 ---
// 当一个节点有懒更新标记时，这意味着它所代表的整个区间都被涂成了同一种颜色。
// 在访问它的子节点之前（无论是查询还是进一步更新），必须将这个懒更新下传给子节点。
void down(int i) {
    // 检查当前节点 i 是否有懒更新标记
    if (update[i]) {
        int v = change[i]; // 获取要下传的颜色值

        // --- 更新左子树 ---
        // 将左子节点的颜色直接设置为该单一颜色对应的位掩码 (1 << v)，因为整个子区间现在都是这个颜色。
        color[i << 1] = 1 << v;
        // 将懒更新的颜色值传递给左子节点
        change[i << 1] = v;
        // 标记左子节点也有了懒更新
        update[i << 1] = true;

        // --- 更新右子树 (同左子树) ---
        color[i << 1 | 1] = 1 << v;
        change[i << 1 | 1] = v;
        update[i << 1 | 1] = true;

        // --- 清除当前节点的懒更新标记 ---
        // 标记已经下传，当前节点不再需要保留此懒更新标记
        update[i] = false;
    }
}

// --- 线段树构建函数 ---
// 初始化线段树，将所有叶子节点的初始颜色设置为1，然后逐层向上合并。
// l, r: 当前节点 i 所代表的区间范围
// i: 当前节点的编号
void build(int l, int r, int i) {
    change[i] = 0;   // 初始化懒更新颜色值为0 (或者其他无效值)
    update[i] = false; // 初始化懒更新标记为 false

    // 如果是叶子节点 (区间只有一个点)
    if (l == r) {
        // 初始颜色是1，其位掩码是 1 << 1 (即2)。
        // 题目中的颜色是从1开始的，所以这里是 1<<1。
        color[i] = 2;
        return;
    }

    // 非叶子节点，递归构建左右子树
    int mid = (l + r) >> 1; // 计算中间点
    build(l, mid, i << 1);        // 构建左子树，节点编号 i*2
    build(mid + 1, r, i << 1 | 1); // 构建右子树，节点编号 i*2+1

    // 左右子树构建完成后，将子节点信息上推到当前节点
    up(i);
}

// --- 区间更新函数 (涂色操作) ---
// 将区间 [jobl, jobr] 涂成颜色 jobv
// jobl, jobr: 要更新的目标区间
// jobv: 要涂上的颜色值
// l, r: 当前节点 i 所代表的区间范围
// i: 当前节点的编号
void updateTree(int jobl, int jobr, int jobv, int l, int r, int i) {
    // 情况1: 当前节点代表的区间 [l, r] 完全包含在要更新的目标区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        // 直接更新当前节点的颜色为单一颜色位掩码
        color[i] = 1 << jobv;
        // 记录懒更新的颜色值
        change[i] = jobv;
        // 设置懒更新标记为 true
        update[i] = true;
        return; // 更新完成，返回
    }

    // 情况2: 当前节点代表的区间与目标区间有交集，但不是完全包含。
    // 在递归之前，必须先将当前节点的懒更新标记下传给子节点，以保证子节点的状态是正确的。
    down(i);

    int mid = (l + r) >> 1; // 计算中间点

    // 如果目标更新区间与左子树有交集，则递归更新左子树
    if (jobl <= mid) {
        updateTree(jobl, jobr, jobv, l, mid, i << 1);
    }
    // 如果目标更新区间与右子树有交集，则递归更新右子树
    if (jobr > mid) { // 注意这里是 jobr > mid，因为 mid+1 是右子树的起点
        updateTree(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
    }

    // 左右子树更新完成后，将子节点信息上推到当前节点，更新父节点的颜色集合
    up(i);
}

// --- 区间查询函数 (查询颜色种类数量) ---
// 查询区间 [jobl, jobr] 内有多少种不同的颜色
// jobl, jobr: 要查询的目标区间
// l, r: 当前节点 i 所代表的区间范围
// i: 当前节点的编号
int query(int jobl, int jobr, int l, int r, int i) {
    // 情况1: 当前节点代表的区间 [l, r] 完全包含在要查询的目标区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return color[i]; // 直接返回当前节点的颜色位掩码
    }

    // 情况2: 当前节点代表的区间与目标区间有交集，但不是完全包含。
    // 在递归之前，必须先将当前节点的懒更新标记下传给子节点。
    down(i);

    int mid = (l + r) >> 1; // 计算中间点
    int status = 0;         // 用于累计查询到的颜色位掩码

    // 如果目标查询区间与左子树有交集，则递归查询左子树
    if (jobl <= mid) {
        status |= query(jobl, jobr, l, mid, i << 1); // 将左子树的查询结果按位或合并
    }
    // 如果目标查询区间与右子树有交集，则递归查询右子树
    if (jobr > mid) { // 注意这里是 jobr > mid
        status |= query(jobl, jobr, mid + 1, r, i << 1 | 1); // 将右子树的查询结果按位或合并
    }

    return status; // 返回合并后的颜色位掩码
}
int cntOnes(int n) {
    n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
    n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
    n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
    return n;
}
// --- 主函数 ---
int main() {
    int L, T, O; // L: 区间长度, T: 颜色种类上限, O: 操作次数
    scanf("%d%d%d", &L, &T, &O); // 读取 L, T, O

    build(1, L, 1); // 构建并初始化线段树。区间从1到L，根节点编号为1。

    char op[2]; // 用于存储操作类型字符 'C' 或 'P'
    int a, b, c; // 操作参数

    // 循环处理 O 次操作
    while (O--) {
        scanf("%s", op); // 读取操作类型

        // 如果是涂色操作 ('C')
        if (op[0] == 'C') {
            scanf("%d%d%d", &a, &b, &c); // 读取起始位置a, 结束位置b, 颜色c
            if (a > b) swap(a, b);       // 确保起始位置不大于结束位置
            updateTree(a, b, c, 1, L, 1); // 调用更新函数
        }
        // 如果是查询操作 ('P')
        else {
            scanf("%d%d", &a, &b);       // 读取起始位置a, 结束位置b
            if (a > b) swap(a, b);       // 确保起始位置不大于结束位置
            int status = query(a, b, 1, L, 1); // 调用查询函数，获取颜色位掩码

            // 统计位掩码中1的数量（即不同颜色种类数）
            // __builtin_popcount 是 GCC 编译器的一个内置函数，用于计算一个整数二进制表示中1的个数。
            // cntOnes
            int ans = __builtin_popcount(status);
            printf("%d\n", ans); // 打印结果
        }
    }
    return 0; // 程序正常结束
}



