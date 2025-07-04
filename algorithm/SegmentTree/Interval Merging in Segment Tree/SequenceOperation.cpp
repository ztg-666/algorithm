#include <iostream>
#include <algorithm>
#include <cstdio>
using namespace std;

// 序列操作
// 给定一个长度为n的数组arr，内部只有01两种值，下标从0开始
// 对于这个序列有五种变换操作和询问操作
// 操作 0 l r : 把l~r范围上所有数字全改成0
// 操作 1 l r : 把l~r范围上所有数字全改成1
// 操作 2 l r : 把l~r范围上所有数字全取反
// 操作 3 l r : 询问l~r范围上有多少个1
// 操作 4 l r : 询问l~r范围上连续1的最长子串长度
// 测试链接 : https://www.luogu.com.cn/problem/P2572

const int MAXN = 100001;

// ==================== 数据结构定义 ====================
// 原始数组
int arr[MAXN];

// 线段树节点信息
int sum[MAXN << 2];     // 累加和，统计区间内1的数量
int len0[MAXN << 2];    // 区间内连续0的最长子串长度
int pre0[MAXN << 2];    // 区间内连续0的最长前缀长度
int suf0[MAXN << 2];    // 区间内连续0的最长后缀长度
int len1[MAXN << 2];    // 区间内连续1的最长子串长度
int pre1[MAXN << 2];    // 区间内连续1的最长前缀长度
int suf1[MAXN << 2];    // 区间内连续1的最长后缀长度

// 懒标记信息
int change[MAXN << 2];  // 区间重置值（0或1）
bool update[MAXN << 2]; // 是否有区间重置操作
bool reve[MAXN << 2];   // 是否有区间翻转操作

// ==================== 线段树维护函数 ====================
/**
 * 向上更新节点信息
 * @param i 当前节点编号
 * @param ln 左子树节点数
 * @param rn 右子树节点数
 */
void up(int i, int ln, int rn) {
    int l = i << 1;     // 左子节点
    int r = i << 1 | 1; // 右子节点

    // 更新1的数量总和
    sum[i] = sum[l] + sum[r];

    // 更新连续0的信息
    // 最长子串：左子树最长、右子树最长、跨越中间的最长
    len0[i] = max(max(len0[l], len0[r]), suf0[l] + pre0[r]);
    // 最长前缀：如果左子树全是0，则加上右子树的前缀
    pre0[i] = len0[l] < ln ? pre0[l] : (pre0[l] + pre0[r]);
    // 最长后缀：如果右子树全是0，则加上左子树的后缀
    suf0[i] = len0[r] < rn ? suf0[r] : (suf0[l] + suf0[r]);

    // 更新连续1的信息，逻辑同上
    len1[i] = max(max(len1[l], len1[r]), suf1[l] + pre1[r]);
    pre1[i] = len1[l] < ln ? pre1[l] : (pre1[l] + pre1[r]);
    suf1[i] = len1[r] < rn ? suf1[r] : (suf1[l] + suf1[r]);
}

/**
 * 区间重置懒标记
 * @param i 节点编号
 * @param v 重置值（0或1）
 * @param n 区间长度
 */
void updateLazy(int i, int v, int n) {
    sum[i] = v * n; // 1的数量 = 重置值 * 区间长度

    // 如果重置为0，则所有连续0的信息都是n，连续1的信息都是0
    len0[i] = pre0[i] = suf0[i] = v == 0 ? n : 0;
    len1[i] = pre1[i] = suf1[i] = v == 1 ? n : 0;

    change[i] = v;      // 记录重置值
    update[i] = true;   // 标记有重置操作
    reve[i] = false;    // 清除翻转标记
}

/**
 * 区间翻转懒标记
 * @param i 节点编号
 * @param n 区间长度
 */
void reveLazy(int i, int n) {
    sum[i] = n - sum[i]; // 1的数量变为总数减去原来1的数量

    // 交换0和1的所有信息
    swap(len0[i], len1[i]);
    swap(pre0[i], pre1[i]);
    swap(suf0[i], suf1[i]);

    reve[i] = !reve[i]; // 翻转标记取反
}

/**
 * 向下推送懒标记
 * @param i 当前节点
 * @param ln 左子树节点数
 * @param rn 右子树节点数
 */
void down(int i, int ln, int rn) {
    // 先处理重置操作（优先级更高）
    if (update[i]) {
        updateLazy(i << 1, change[i], ln);      // 更新左子树
        updateLazy(i << 1 | 1, change[i], rn);  // 更新右子树
        update[i] = false;
    }
    // 再处理翻转操作
    if (reve[i]) {
        reveLazy(i << 1, ln);       // 翻转左子树
        reveLazy(i << 1 | 1, rn);   // 翻转右子树
        reve[i] = false;
    }
}

// ==================== 线段树基本操作 ====================
/**
 * 构建线段树
 * @param l 区间左端点
 * @param r 区间右端点
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    if (l == r) { // 叶子节点
        sum[i] = arr[l];
        // 根据原始值设置连续0和1的信息
        len0[i] = pre0[i] = suf0[i] = arr[l] == 0 ? 1 : 0;
        len1[i] = pre1[i] = suf1[i] = arr[l] == 1 ? 1 : 0;
    } else {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);          // 构建左子树
        build(mid + 1, r, i << 1 | 1);  // 构建右子树
        up(i, mid - l + 1, r - mid);    // 向上更新当前节点
    }
    // 初始化懒标记
    update[i] = false;
    reve[i] = false;
}

/**
 * 区间重置更新
 * @param jobl 操作区间左端点
 * @param jobr 操作区间右端点
 * @param jobv 重置值
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 */
void updateRange(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) { // 完全包含，直接更新
        updateLazy(i, jobv, r - l + 1);
    } else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid); // 向下推送懒标记

        // 递归更新子树
        if (jobl <= mid) {
            updateRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i, mid - l + 1, r - mid); // 向上更新当前节点
    }
}

/**
 * 区间翻转更新
 * @param jobl 操作区间左端点
 * @param jobr 操作区间右端点
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 */
void reveRange(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) { // 完全包含，直接翻转
        reveLazy(i, r - l + 1);
    } else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid); // 向下推送懒标记

        // 递归翻转子树
        if (jobl <= mid) {
            reveRange(jobl, jobr, l, mid, i << 1);
        }
        if (jobr > mid) {
            reveRange(jobl, jobr, mid + 1, r, i << 1 | 1);
        }

        up(i, mid - l + 1, r - mid); // 向上更新当前节点
    }
}

/**
 * 查询区间1的数量
 * @param jobl 查询区间左端点
 * @param jobr 查询区间右端点
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 * @return 区间内1的数量
 */
int querySum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) { // 完全包含，直接返回
        return sum[i];
    }

    int mid = (l + r) >> 1;
    down(i, mid - l + 1, r - mid); // 向下推送懒标记

    int ans = 0;
    // 递归查询子树并累加结果
    if (jobl <= mid) {
        ans += querySum(jobl, jobr, l, mid, i << 1);
    }
    if (jobr > mid) {
        ans += querySum(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans;
}

// ==================== 查询结果结构体 ====================
/**
 * 存储连续1的查询结果
 */
struct Result {
    int len; // 最长连续1的长度
    int pre; // 最长连续1的前缀长度
    int suf; // 最长连续1的后缀长度

    Result(int l = 0, int p = 0, int s = 0) : len(l), pre(p), suf(s) {}
};

/**
 * 查询区间内连续1的最长长度
 * @param jobl 查询区间左端点
 * @param jobr 查询区间右端点
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 * @return 包含最长长度、前缀、后缀信息的结果
 */
Result queryLongest(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) { // 完全包含，直接返回
        return Result(len1[i], pre1[i], suf1[i]);
    } else {
        int mid = (l + r) >> 1;
        int ln = mid - l + 1;    // 左子树长度
        int rn = r - mid;        // 右子树长度
        down(i, ln, rn);         // 向下推送懒标记

        // 只查询左子树
        if (jobr <= mid) {
            return queryLongest(jobl, jobr, l, mid, i << 1);
        }
        // 只查询右子树
        if (jobl > mid) {
            return queryLongest(jobl, jobr, mid + 1, r, i << 1 | 1);
        }

        // 跨越左右子树的查询
        Result left = queryLongest(jobl, jobr, l, mid, i << 1);
        Result right = queryLongest(jobl, jobr, mid + 1, r, i << 1 | 1);

        int llen = left.len, lpre = left.pre, lsuf = left.suf;
        int rlen = right.len, rpre = right.pre, rsuf = right.suf;

        // 合并结果
        int len = max(max(llen, rlen), lsuf + rpre); // 最长长度

        // 计算实际影响的左右区间长度
        int leftActualLen = mid - max(jobl, l) + 1;
        int rightActualLen = min(r, jobr) - mid;

        // 前缀：如果左侧不是全1，则为左侧前缀；否则为左右前缀之和
        int pre = llen < leftActualLen ? lpre : (lpre + rpre);
        // 后缀：如果右侧不是全1，则为右侧后缀；否则为左右后缀之和
        int suf = rlen < rightActualLen ? rsuf : (lsuf + rsuf);

        return Result(len, pre, suf);
    }
}

// ==================== 主函数 ====================
int main() {
    int n, m; // n个元素，m个操作
    scanf("%d %d", &n, &m);

    // 读入原始数组（注意下标从1开始存储）
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }

    // 构建线段树
    build(1, n, 1);

    // 处理m个操作
    for (int i = 0; i < m; i++) {
        int op, jobl, jobr;
        scanf("%d %d %d", &op, &jobl, &jobr);

        // 题目给的下标从0开始，转换为从1开始
        jobl++;
        jobr++;

        if (op == 0) {
            // 操作0：区间重置为0
            updateRange(jobl, jobr, 0, 1, n, 1);
        } else if (op == 1) {
            // 操作1：区间重置为1
            updateRange(jobl, jobr, 1, 1, n, 1);
        } else if (op == 2) {
            // 操作2：区间翻转（0变1，1变0）
            reveRange(jobl, jobr, 1, n, 1);
        } else if (op == 3) {
            // 操作3：查询区间内1的数量
            printf("%d\n", querySum(jobl, jobr, 1, n, 1));
        } else {
            // 操作4：查询区间内连续1的最长长度
            printf("%d\n", queryLongest(jobl, jobr, 1, n, 1).len);
        }
    }

    return 0;
}
