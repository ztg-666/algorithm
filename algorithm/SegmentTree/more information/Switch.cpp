#include <cstdio>       // 引入标准输入输出库，用于scanf和printf
#include <algorithm>    // 引入算法库，尽管在这个特定的代码中没有直接使用，但通常是C++编程的好习惯

// 开关
// 现有n盏灯排成一排，从左到右依次编号为1~n，一开始所有的灯都是关着的
// 操作分两种
// 操作 0 l r : 改变l~r范围上所有灯的状态，开着的灯关上、关着的灯打开
// 操作 1 l r : 查询l~r范围上有多少灯是打开的
// 测试链接 : https://www.luogu.com.cn/problem/P3870

using namespace std;    // 使用标准命名空间，避免每次使用std::前缀

// 定义一个常量，表示线段树节点数组的最大大小
// MAXN 表示原始数据（灯泡）的数量上限，这里是100000
// 线段树通常需要大约 4 * N 的空间来存储节点，所以这里是 MAXN << 2 (即 MAXN * 4)
const int MAXN = 100001;

// light 数组：线段树的存储核心。
// light[i] 存储以节点 i 为根的线段树区间内，当前亮着的灯泡（值为1）的数量。
// MAXN << 2 的大小是为了容纳所有节点，因为一棵满二叉树在最坏情况下需要将近 2N 个叶子节点，总节点数接近 4N。
int light[MAXN << 2];

// rev 数组：延迟翻转标记（Lazy Propagation Tag）。
// rev[i] 为 true 表示以节点 i 为根的区间需要进行翻转操作，但这个操作还没有下推到其子节点。
// 这是一个布尔类型，用于标记是否需要翻转。
bool rev[MAXN << 2];

// --- 辅助函数 ---

// up 函数：向上更新节点信息
// 当子节点（i << 1 和 i << 1 | 1）的信息发生变化后，需要更新它们的父节点 i 的信息。
// 对于这个线段树，父节点亮灯的数量是其左右子节点亮灯数量之和。
void up(int i) {
    light[i] = light[i << 1] + light[i << 1 | 1]; // 父节点亮灯数 = 左子节点亮灯数 + 右子节点亮灯数
}

// lazy 函数：处理单个节点的翻转操作
// i: 当前线段树节点的索引
// n: 当前节点所代表的区间的长度（即该区间内有多少个灯泡）
// 这个函数会翻转当前节点 light[i] 存储的亮灯数量，并切换其延迟翻转标记 rev[i]。
void lazy(int i, int n) {
    // 翻转操作：如果区间长度为 n，原来有 light[i] 个亮灯，
    // 翻转后，原来亮的变灭，原来灭的变亮，所以亮灯的数量会变成 (总数 - 原来亮着的数量)。
    light[i] = n - light[i];
    rev[i] = !rev[i]; // 切换翻转标记：true 变 false，false 变 true。
}

// down 函数：向下推迟标记（Push Down Lazy Tag）
// 当我们访问一个节点 i 的子节点时，如果节点 i 有延迟翻转标记 (rev[i] 为 true)，
// 那么需要将这个标记下推给其子节点。
// i: 当前线段树节点的索引
// ln: 左子节点所代表区间的长度
// rn: 右子节点所代表区间的长度
void down(int i, int ln, int rn) {
    if (rev[i]) { // 如果当前节点 i 有延迟翻转标记
        lazy(i << 1, ln);     // 将翻转操作应用到左子节点 (i << 1)，并更新其标记
        lazy(i << 1 | 1, rn); // 将翻转操作应用到右子节点 (i << 1 | 1)，并更新其标记
        rev[i] = false;       // 完成下推后，清除当前节点 i 的延迟翻转标记
    }
}

// --- 主要线段树操作函数 ---

// build 函数：构建线段树
// l, r: 当前节点 i 所代表的区间范围 [l, r]
// i: 当前线段树节点的索引
// 这是一个递归过程，从根节点开始，逐步向下构建树。
void build(int l, int r, int i) {
    if (l == r) { // 如果是叶子节点（区间只有一个元素）
        light[i] = 0; // 初始时，所有灯泡都是熄灭的（0）
    } else { // 如果是非叶子节点
        int mid = (l + r) >> 1; // 计算中间点
        build(l, mid, i << 1);     // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 构建完成后，向上更新当前节点的信息 (light[i])
    }
    rev[i] = false; // 初始时，所有节点的延迟翻转标记都设置为 false
}

// reverseRange 函数：区间翻转操作
// jobl, jobr: 需要翻转的区间范围 [jobl, jobr] (job = operation)
// l, r: 当前节点 i 所代表的区间范围 [l, r]
// i: 当前线段树节点的索引
// 这个函数会翻转指定区间内的灯泡状态。
void reverseRange(int jobl, int jobr, int l, int r, int i) {
    // 情况1: 当前节点区间 [l, r] 完全包含在目标操作区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        lazy(i, r - l + 1); // 对当前节点进行翻转操作，并设置其延迟标记
    } else { // 情况2: 当前节点区间与目标操作区间有交集，但不是完全包含
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 在递归之前，先向下推迟当前节点的延迟标记，确保子节点状态最新

        // 递归处理左子树：如果目标区间与左子树有交集
        if (jobl <= mid) {
            reverseRange(jobl, jobr, l, mid, i << 1);
        }
        // 递归处理右子树：如果目标区间与右子树有交集
        if (jobr > mid) {
            reverseRange(jobl, jobr, mid + 1, r, i << 1 | 1);
        }
        up(i); // 左右子树处理完成后，向上更新当前节点的信息
    }
}

// query 函数：区间查询操作
// jobl, jobr: 需要查询的区间范围 [jobl, jobr]
// l, r: 当前节点 i 所代表的区间范围 [l, r]
// i: 当前线段树节点的索引
// 这个函数会返回指定区间内亮着灯泡的数量。
int query(int jobl, int jobr, int l, int r, int i) {
    // 情况1: 当前节点区间 [l, r] 完全包含在目标查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return light[i]; // 直接返回当前节点存储的亮灯数量
    }

    int mid = (l + r) >> 1; // 计算中间点
    down(i, mid - l + 1, r - mid); // 在查询之前，向下推迟当前节点的延迟标记，确保子节点状态最新

    int ans = 0; // 初始化查询结果
    // 递归查询左子树：如果目标区间与左子树有交集
    if (jobl <= mid) {
        ans += query(jobl, jobr, l, mid, i << 1);
    }
    // 递归查询右子树：如果目标区间与右子树有交集
    if (jobr > mid) {
        ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans; // 返回累加的查询结果
}

// --- 主函数 ---

int main() {
    int n, m; // n: 灯泡的总数量，m: 操作的总次数
    scanf("%d %d", &n, &m); // 读取 n 和 m

    build(1, n, 1); // 构建线段树，灯泡编号从1到n，根节点索引为1

    // 循环处理 m 次操作
    for (int i = 1, op, jobl, jobr; i <= m; i++) {
        scanf("%d %d %d", &op, &jobl, &jobr); // 读取操作类型 op 和操作区间 [jobl, jobr]
        if (op == 0) { // 如果操作类型是 0，表示区间翻转
            reverseRange(jobl, jobr, 1, n, 1); // 调用翻转函数
        } else { // 如果操作类型是 1，表示区间查询
            printf("%d\n", query(jobl, jobr, 1, n, 1)); // 调用查询函数，并打印结果
        }
    }
    return 0; // 程序正常结束
}

