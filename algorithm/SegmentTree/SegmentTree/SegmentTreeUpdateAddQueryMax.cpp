#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;

// 线段树同时支持范围重置、范围增加、范围查询
// 维护最大值
// 测试链接 : https://www.luogu.com.cn/problem/P1253

const int MAXN = 1000001;  // 数组最大长度

// 全局数组定义
long long arr[MAXN];           // 原始数组
long long maxval[MAXN << 2];   // 线段树节点存储的最大值（线段树需要4倍空间）
long long add[MAXN << 2];      // 懒标记：区间加法标记
long long change[MAXN << 2];   // 懒标记：区间更新标记
bool update[MAXN << 2];        // 懒标记：是否进行了区间更新操作

/**
 * 向上更新操作
 * 用子节点的最大值更新父节点的最大值
 * @param i 当前节点编号
 */
void up(int i) {
    maxval[i] = max(maxval[i << 1], maxval[i << 1 | 1]);
}

/**
 * 区间更新的懒标记下推
 * 将当前节点的区间更新操作传递给子节点
 * @param i 当前节点编号
 * @param v 更新值
 */
void updateLazy(int i, long long v) {
    maxval[i] = v;      // 当前节点的最大值设为v
    add[i] = 0;         // 清空加法懒标记（更新操作会覆盖加法操作）
    change[i] = v;      // 设置更新懒标记
    update[i] = true;   // 标记该节点有更新操作
}

/**
 * 区间加法的懒标记下推
 * 将当前节点的区间加法操作传递给子节点
 * @param i 当前节点编号
 * @param v 加法值
 */
void addLazy(int i, long long v) {
    maxval[i] += v;  // 当前节点的最大值加上v
    add[i] += v;     // 累加懒标记
}

/**
 * 懒标记下推操作
 * 将当前节点的懒标记传递给子节点
 * @param i 当前节点编号
 */
void down(int i) {
    // 先处理更新懒标记（更新操作优先级高于加法操作）
    if (update[i]) {
        updateLazy(i << 1, change[i]);      // 传递给左子节点
        updateLazy(i << 1 | 1, change[i]);  // 传递给右子节点
        update[i] = false;                  // 清空当前节点的更新标记
    }
    // 再处理加法懒标记
    if (add[i] != 0) {
        addLazy(i << 1, add[i]);      // 传递给左子节点
        addLazy(i << 1 | 1, add[i]);  // 传递给右子节点
        add[i] = 0;                   // 清空当前节点的加法标记
    }
}

/**
 * 构建线段树
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    // 初始化懒标记
    add[i] = 0;
    change[i] = 0;
    update[i] = false;

    if (l == r) {
        // 叶子节点：直接赋值
        maxval[i] = arr[l];
    } else {
        // 非叶子节点：递归构建左右子树，然后向上更新
        int mid = (l + r) >> 1;              // 计算中点
        build(l, mid, i << 1);               // 构建左子树
        build(mid + 1, r, i << 1 | 1);       // 构建右子树
        up(i);                               // 向上更新当前节点
    }
}

/**
 * 区间更新操作
 * 将区间[jobl, jobr]内的所有值更新为jobv
 * @param jobl 目标区间左边界
 * @param jobr 目标区间右边界
 * @param jobv 更新值
 * @param l 当前节点对应区间左边界
 * @param r 当前节点对应区间右边界
 * @param i 当前节点编号
 */
void updateRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全包含在目标区间内，直接更新并设置懒标记
        updateLazy(i, jobv);
    } else {
        // 当前区间与目标区间有交集但不完全包含
        int mid = (l + r) >> 1;
        down(i);  // 先下推懒标记

        // 递归处理左子树（如果目标区间与左子树有交集）
        if (jobl <= mid) {
            updateRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 递归处理右子树（如果目标区间与右子树有交集）
        if (jobr > mid) {
            updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i);  // 向上更新当前节点
    }
}

/**
 * 区间加法操作
 * 将区间[jobl, jobr]内的所有值加上jobv
 * @param jobl 目标区间左边界
 * @param jobr 目标区间右边界
 * @param jobv 加法值
 * @param l 当前节点对应区间左边界
 * @param r 当前节点对应区间右边界
 * @param i 当前节点编号
 */
void addRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全包含在目标区间内，直接加法并设置懒标记
        addLazy(i, jobv);
    } else {
        // 当前区间与目标区间有交集但不完全包含
        int mid = (l + r) >> 1;
        down(i);  // 先下推懒标记

        // 递归处理左子树（如果目标区间与左子树有交集）
        if (jobl <= mid) {
            addRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 递归处理右子树（如果目标区间与右子树有交集）
        if (jobr > mid) {
            addRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i);  // 向上更新当前节点
    }
}

/**
 * 区间查询操作
 * 查询区间[jobl, jobr]内的最大值
 * @param jobl 目标区间左边界
 * @param jobr 目标区间右边界
 * @param l 当前节点对应区间左边界
 * @param r 当前节点对应区间右边界
 * @param i 当前节点编号
 * @return 区间内的最大值
 */
long long query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全包含在目标区间内，直接返回该节点的最大值
        return maxval[i];
    }

    // 当前区间与目标区间有交集但不完全包含
    int mid = (l + r) >> 1;
    down(i);  // 先下推懒标记

    long long ans = LLONG_MIN;  // 初始化答案为最小值

    // 查询左子树（如果目标区间与左子树有交集）
    if (jobl <= mid) {
        ans = max(ans, query(jobl, jobr, l, mid, i << 1));
    }
    // 查询右子树（如果目标区间与右子树有交集）
    if (jobr > mid) {
        ans = max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1));
    }

    return ans;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;  // n为数组长度，m为操作次数
    cin >> n >> m;

    // 读入原始数组（下标从1开始）
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 构建线段树
    build(1, n, 1);

    // 处理m次操作
    for (int i = 0; i < m; i++) {
        int op;  // 操作类型
        cin >> op;

        if (op == 1) {
            // 操作1：区间更新
            int jobl, jobr;
            long long jobv;
            cin >> jobl >> jobr >> jobv;
            updateRange(jobl, jobr, jobv, 1, n, 1);
        } else if (op == 2) {
            // 操作2：区间加法
            int jobl, jobr;
            long long jobv;
            cin >> jobl >> jobr >> jobv;
            addRange(jobl, jobr, jobv, 1, n, 1);
        } else {
            // 操作3：区间查询最大值
            int jobl, jobr;
            cin >> jobl >> jobr;
            cout << query(jobl, jobr, 1, n, 1) << "\n";
        }
    }

    return 0;
}
