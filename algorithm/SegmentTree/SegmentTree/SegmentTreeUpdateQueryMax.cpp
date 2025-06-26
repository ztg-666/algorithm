#include <iostream>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
using namespace std;

// 线段树支持范围重置、范围查询最大值
// 使用懒标记优化，支持区间更新和区间查询
// 包含对数器验证逻辑

const int MAXN = 100001;  // 数组最大长度

long long arr[MAXN];           // 原始数组，下标从1开始
long long maxVal[MAXN << 2];   // 线段树数组，存储区间最大值，大小为4倍原数组
long long change[MAXN << 2];   // 懒标记数组，存储待更新的值
bool update[MAXN << 2];        // 标记数组，表示该节点是否有懒标记

/**
 * 向上更新，用子节点的值更新父节点
 * @param i 当前节点编号
 */
void up(int i) {
    maxVal[i] = max(maxVal[i << 1], maxVal[i << 1 | 1]);
}

/**
 * 给节点添加懒标记
 * @param i 节点编号
 * @param v 要更新的值
 */
void lazy(int i, long long v) {
    maxVal[i] = v;     // 更新节点的最大值
    change[i] = v;     // 设置懒标记的值
    update[i] = true;  // 标记该节点有懒标记
}

/**
 * 向下传递懒标记，将父节点的懒标记传递给子节点
 * @param i 当前节点编号
 */
void down(int i) {
    if (update[i]) {
        lazy(i << 1, change[i]);      // 传递给左子节点
        lazy(i << 1 | 1, change[i]);  // 传递给右子节点
        update[i] = false;            // 清除当前节点的懒标记
    }
}



/**
 * 建立线段树
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点，直接赋值
        maxVal[i] = arr[l];
    } else {
        // 非叶子节点，递归建立左右子树
        int mid = (l + r) >> 1;        // 中点，等价于 (l + r) / 2
        build(l, mid, i << 1);         // 建立左子树，节点编号 i*2
        build(mid + 1, r, i << 1 | 1); // 建立右子树，节点编号 i*2+1
        up(i);                         // 用子节点更新当前节点
    }
    // 初始化懒标记
    change[i] = 0;
    update[i] = false;
}

/**
 * 区间更新操作
 * @param jobl 要更新的区间左边界
 * @param jobr 要更新的区间右边界
 * @param jobv 要更新的值
 * @param l 当前节点对应的区间左边界
 * @param r 当前节点对应的区间右边界
 * @param i 当前节点编号
 */
void updateRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全被要更新的区间包含，直接添加懒标记
        lazy(i, jobv);
    } else {
        // 当前区间部分被包含，需要向下递归
        down(i);  // 先传递懒标记
        int mid = (l + r) >> 1;

        // 如果左子区间与更新区间有交集
        if (jobl <= mid) {
            updateRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果右子区间与更新区间有交集
        if (jobr > mid) {
            updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i);  // 用子节点更新当前节点
    }
}

/**
 * 区间查询操作
 * @param jobl 要查询的区间左边界
 * @param jobr 要查询的区间右边界
 * @param l 当前节点对应的区间左边界
 * @param r 当前节点对应的区间右边界
 * @param i 当前节点编号
 * @return 查询区间的最大值
 */
long long query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全被查询区间包含，直接返回结果
        return maxVal[i];
    }

    down(i);  // 传递懒标记
    int mid = (l + r) >> 1;
    long long ans = LLONG_MIN;  // 初始化为最小值

    // 如果左子区间与查询区间有交集
    if (jobl <= mid) {
        ans = max(ans, query(jobl, jobr, l, mid, i << 1));
    }
    // 如果右子区间与查询区间有交集
    if (jobr > mid) {
        ans = max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1));
    }

    return ans;
}

/**
 * 生成随机值填入arr数组
 * @param n 数组长度
 * @param v 随机值的范围 [0, v)
 */
void randomArray(int n, int v) {
    for (int i = 1; i <= n; i++) {
        arr[i] = (long long)(rand() % v);
    }
}

/**
 * 验证结构的更新操作（暴力更新，用于对比验证）
 * @param check 验证数组
 * @param jobl 更新区间左边界
 * @param jobr 更新区间右边界
 * @param jobv 更新值
 */
void checkUpdate(long long check[], int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] = jobv;
    }
}

/**
 * 验证结构的查询操作（暴力查询，用于对比验证）
 * @param check 验证数组
 * @param jobl 查询区间左边界
 * @param jobr 查询区间右边界
 * @return 查询区间的最大值
 */
long long checkQuery(long long check[], int jobl, int jobr) {
    long long ans = LLONG_MIN;
    for (int i = jobl; i <= jobr; i++) {
        ans = max(ans, check[i]);
    }
    return ans;
}

/**
 * 主函数 - 对数器测试
 * 通过大量随机测试来验证线段树实现的正确性
 */
int main() {
    srand(time(0)); // 初始化随机数种子

    cout << "" << endl;

    // 测试参数
    int n = 1000;      // 数组长度
    int v = 2000;      // 随机值范围
    int t = 5000000;   // 测试次数

    // 生成随机值填入arr数组
    randomArray(n, v);

    // 建立线段树
    build(1, n, 1);

    // 生成验证的结构（普通数组，用暴力方法操作）
    long long check[n + 1];
    for (int i = 1; i <= n; i++) {
        check[i] = arr[i];
    }

    // 进行大量随机测试
    for (int i = 1; i <= t; i++) {
        // 随机生成操作类型
        // op = 0: 更新操作
        // op = 1: 查询操作
        int op = rand() % 2;

        // 生成两个随机下标（下标从1开始）
        int a = rand() % n + 1;
        int b = rand() % n + 1;

        // 确保 jobl <= jobr
        int jobl = min(a, b);
        int jobr = max(a, b);

        if (op == 0) {
            // 更新操作测试
            int jobv = rand() % (v * 2) - v;  // 生成 [-v, v) 范围内的随机值

            // 线段树更新
            updateRange(jobl, jobr, jobv, 1, n, 1);
            // 验证结构同步更新
            checkUpdate(check, jobl, jobr, jobv);

        } else {
            // 查询操作测试
            // 分别用线段树和验证结构查询，比对结果
            long long ans1 = query(jobl, jobr, 1, n, 1);     // 线段树查询
            long long ans2 = checkQuery(check, jobl, jobr);   // 暴力查询

            if (ans1 != ans2) {
                cout << "!" << endl;
                cout << "[" << jobl << ", " << jobr << "]" << endl;
                cout << ": " << ans1 << endl;
                cout << ": " << ans2 << endl;
                return 1;  // 发现错误，退出程序
            }
        }
    }

    cout << "" << endl;
    cout << " " << t << " " << endl;

    return 0;
}
