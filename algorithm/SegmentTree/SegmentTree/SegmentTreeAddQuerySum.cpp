#include <iostream>
#include <vector>

// 线段树支持范围增加、范围查询
// 维护累加和
// 测试链接 : https://www.luogu.com.cn/problem/P3372

using namespace std;

// 最大数组长度
const int MAXN = 100001;

// 原始数组，存储初始数据
long long arr[MAXN];

// 线段树数组，存储每个节点对应区间的累加和
// 数组大小为 MAXN << 2，即 MAXN * 4，保证有足够空间存储完全二叉树
long long sum[MAXN << 2];

// 懒标记数组，存储每个节点的延迟更新值
// add[i] 表示节点 i 对应区间内每个元素需要增加的值
long long add[MAXN << 2];

/**
 * 向上更新函数 - 用子节点信息更新父节点
 * @param i 当前节点在线段树中的编号
 *
 * 线段树节点编号规律：
 * - 节点 i 的左孩子编号为 i << 1 (即 i * 2)
 * - 节点 i 的右孩子编号为 i << 1 | 1 (即 i * 2 + 1)
 */
void up(int i) {
    // 父节点的累加和 = 左孩子累加和 + 右孩子累加和
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
}

/**
 * 懒标记处理函数 - 将懒更新应用到当前节点
 * @param i 当前节点编号
 * @param v 需要增加的值
 * @param n 当前节点对应区间的元素个数 (r - l + 1)
 *
 * 懒标记的核心思想：
 * 当需要对一个区间进行相同操作时，不立即更新到叶子节点，
 * 而是在当前节点记录这个操作，等到需要时再向下传递
 */
void lazy(int i, long long v, int n) {
    // 累加和增加：区间内每个数都增加v，所以总和增加 v * n
    sum[i] += v * n;
    // 懒标记累加：记录当前节点还需要向下传递的增加值
    add[i] += v;
}

/**
 * 向下传递懒标记函数 - 将父节点的懒标记传递给子节点
 * @param i 当前节点编号
 * @param ln 左子树区间的元素个数
 * @param rn 右子树区间的元素个数
 */
void down(int i, int ln, int rn) {
    // 只有当前节点有懒标记时才需要向下传递
    if (add[i] != 0) {
        // 将懒标记传递给左孩子
        lazy(i << 1, add[i], ln);
        // 将懒标记传递给右孩子
        lazy(i << 1 | 1, add[i], rn);
        // 清空当前节点的懒标记（已经传递完毕）
        add[i] = 0;
    }
}

/**
 * 建树函数 - 递归构建线段树
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点在线段树中的编号
 *
 * 建树过程：
 * 1. 如果是叶子节点，直接赋值
 * 2. 否则递归建立左右子树，然后向上更新
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：直接存储原数组对应位置的值
        sum[i] = arr[l];
    } else {
        // 非叶子节点：递归建立左右子树
        int mid = (l + r) >> 1;  // 中点，等价于 (l + r) / 2
        build(l, mid, i << 1);           // 建立左子树
        build(mid + 1, r, i << 1 | 1);  // 建立右子树
        up(i);  // 用子节点信息更新当前节点
    }
    // 初始化懒标记为0
    add[i] = 0;
}

/**
 * 区间更新函数 - 对指定区间内所有元素增加指定值
 * @param jobl 目标区间左端点
 * @param jobr 目标区间右端点
 * @param jobv 需要增加的值
 * @param l 当前节点对应区间左端点
 * @param r 当前节点对应区间右端点
 * @param i 当前节点编号
 *
 * 区间更新的三种情况：
 * 1. 当前区间完全包含在目标区间内 -> 直接懒标记
 * 2. 当前区间与目标区间有交集 -> 递归处理子区间
 * 3. 当前区间与目标区间无交集 -> 不处理
 */
void addRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
    // 情况1：当前区间完全在目标区间内，可以直接更新
    if (jobl <= l && r <= jobr) {
        lazy(i, jobv, r - l + 1);
    } else {
        // 情况2：当前区间与目标区间有部分重叠，需要递归处理
        int mid = (l + r) >> 1;
        // 先向下传递懒标记，确保子节点信息是最新的
        down(i, mid - l + 1, r - mid);

        // 如果目标区间与左子区间有交集，递归更新左子树
        if (jobl <= mid) {
            addRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果目标区间与右子区间有交集，递归更新右子树
        if (jobr > mid) {
            addRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        // 子节点更新完毕后，向上更新当前节点
        up(i);
    }
}

/**
 * 区间查询函数 - 查询指定区间的累加和
 * @param jobl 目标区间左端点
 * @param jobr 目标区间右端点
 * @param l 当前节点对应区间左端点
 * @param r 当前节点对应区间右端点
 * @param i 当前节点编号
 * @return 目标区间的累加和
 *
 * 区间查询的三种情况：
 * 1. 当前区间完全包含在目标区间内 -> 直接返回当前节点值
 * 2. 当前区间与目标区间有交集 -> 递归查询子区间并合并结果
 * 3. 当前区间与目标区间无交集 -> 返回0
 */
long long query(int jobl, int jobr, int l, int r, int i) {
    // 情况1：当前区间完全在目标区间内，直接返回
    if (jobl <= l && r <= jobr) {
        return sum[i];
    }

    // 情况2：当前区间与目标区间有部分重叠，需要递归查询
    int mid = (l + r) >> 1;
    // 先向下传递懒标记，确保子节点信息是最新的
    down(i, mid - l + 1, r - mid);

    long long ans = 0;
    // 如果目标区间与左子区间有交集，递归查询左子树
    if (jobl <= mid) {
        ans += query(jobl, jobr, l, mid, i << 1);
    }
    // 如果目标区间与右子区间有交集，递归查询右子树
    if (jobr > mid) {
        ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
    }

    return ans;
}

/**
 * 主函数 - 处理输入输出和调用相应函数
 */
int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;  // n: 数组长度, m: 操作次数
    cin >> n >> m;

    // 输入原始数组（注意：数组下标从1开始）
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 建立线段树（区间[1,n]，根节点编号为1）
    build(1, n, 1);

    // 处理m个操作
    for (int i = 1; i <= m; i++) {
        int op;  // 操作类型
        cin >> op;

        if (op == 1) {
            // 操作1：区间更新
            // 将区间[jobl, jobr]内所有数增加jobv
            int jobl, jobr;
            long long jobv;
            cin >> jobl >> jobr >> jobv;
            addRange(jobl, jobr, jobv, 1, n, 1);
        } else {
            // 操作2：区间查询
            // 查询区间[jobl, jobr]的累加和
            int jobl, jobr;
            cin >> jobl >> jobr;
            cout << query(jobl, jobr, 1, n, 1) << '\n';
        }
    }

    return 0;
}
