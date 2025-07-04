#include <iostream>
#include <algorithm>
#include <cstring>

// 旅馆
// 一共有n个房间，编号1~n，一开始都是空房
// 实现如下两种操作，会一共调用m次
// 操作 1 x   : 找到至少有连续x个空房间的区域，返回最左编号
//              如果有多个满足条件的区域，返回其中最左区域的最左编号
//              如果找不到打印0，并且不办理入住
//              如果找到了打印最左编号，并且从最左编号开始办理x个人的入住
// 操作 2 x y : 从x号房间开始往下数y个房间，一律清空
// 操作1有打印操作，操作2没有
// 1 <= n 、m <= 50000
// 测试链接 : https://www.luogu.com.cn/problem/P2894

using namespace std;

const int MAXN = 50001;

// 线段树节点信息
int len[MAXN << 2];    // 连续空房最长子串长度
int pre[MAXN << 2];    // 连续空房最长前缀长度
int suf[MAXN << 2];    // 连续空房最长后缀长度

// 懒标记相关
int change[MAXN << 2];  // 懒更新信息，范围上所有数字被重置成了什么
bool update[MAXN << 2]; // 懒更新信息，范围上有没有重置任务

/**
 * 向上更新节点信息
 * @param i 当前节点编号
 * @param ln 左子树节点数量
 * @param rn 右子树节点数量
 */
void up(int i, int ln, int rn) {
    int l = i << 1;        // 左子节点
    int r = i << 1 | 1;    // 右子节点

    // 最长连续空房子串长度 = max(左子树最长, 右子树最长, 跨越中点的长度)
    len[i] = max({len[l], len[r], suf[l] + pre[r]});

    // 前缀长度：如果左子树全是空房，则包含右子树前缀；否则就是左子树前缀
    pre[i] = len[l] < ln ? pre[l] : (pre[l] + pre[r]);

    // 后缀长度：如果右子树全是空房，则包含左子树后缀；否则就是右子树后缀
    suf[i] = len[r] < rn ? suf[r] : (suf[l] + suf[r]);
}
/**
 * 给节点打懒标记
 * @param i 节点编号
 * @param v 设置值（0表示空房，1表示有人）
 * @param n 节点覆盖的区间长度
 */
void lazy(int i, int v, int n) {
    if (v == 0) {
        // 设置为空房：所有长度都等于区间长度
        len[i] = pre[i] = suf[i] = n;
    } else {
        // 设置为有人：所有长度都为0
        len[i] = pre[i] = suf[i] = 0;
    }
    change[i] = v;      // 记录变更值
    update[i] = true;   // 标记有更新
}

/**
 * 向下传递懒标记
 * @param i 当前节点编号
 * @param ln 左子树节点数量
 * @param rn 右子树节点数量
 */
void down(int i, int ln, int rn) {
    if (update[i]) {
        // 向左右子节点传递懒标记
        lazy(i << 1, change[i], ln);
        lazy(i << 1 | 1, change[i], rn);
        update[i] = false;  // 清除当前节点的懒标记
    }
}



/**
 * 构建线段树
 * @param l 区间左端点
 * @param r 区间右端点
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：初始都是空房
        len[i] = pre[i] = suf[i] = 1;
    } else {
        int mid = (l + r) >> 1;
        // 递归构建左右子树
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        // 向上更新当前节点信息
        up(i, mid - l + 1, r - mid);
    }
    update[i] = false;  // 初始没有懒标记
}

/**
 * 区间更新操作
 * @param jobl 更新区间左端点
 * @param jobr 更新区间右端点
 * @param jobv 更新值（0表示退房，1表示入住）
 * @param l 当前节点对应区间左端点
 * @param r 当前节点对应区间右端点
 * @param i 当前节点编号
 */
void updateRange(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全被更新区间包含，直接打懒标记
        lazy(i, jobv, r - l + 1);
    } else {
        int mid = (l + r) >> 1;
        // 先向下传递懒标记
        down(i, mid - l + 1, r - mid);

        // 递归更新相关子区间
        if (jobl <= mid) {
            updateRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        // 向上更新当前节点信息
        up(i, mid - l + 1, r - mid);
    }
}

/**
 * 查询最左边能容纳x个连续空房的位置
 * 前提：l..r范围上一定存在连续空房长度>=x的区域
 * @param x 需要的连续空房数量
 * @param l 查询区间左端点
 * @param r 查询区间右端点
 * @param i 当前节点编号
 * @return 最左边的起始位置
 */
int queryLeft(int x, int l, int r, int i) {
    if (l == r) {
        // 叶子节点，直接返回位置
        return l;
    } else {
        int mid = (l + r) >> 1;
        // 先向下传递懒标记
        down(i, mid - l + 1, r - mid);

        // 优先级1：检查左子树是否有足够的连续空房
        if (len[i << 1] >= x) {
            return queryLeft(x, l, mid, i << 1);
        }

        // 优先级2：检查跨越中点的区域是否有足够的连续空房
        if (suf[i << 1] + pre[i << 1 | 1] >= x) {
            // 返回跨越区域的起始位置
            return mid - suf[i << 1] + 1;
        }

        // 优先级3：在右子树中查找
        return queryLeft(x, mid + 1, r, i << 1 | 1);
    }
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n;           // 房间总数
    build(1, n, 1);     // 构建线段树，初始所有房间都是空的
    cin >> m;           // 操作次数

    for (int i = 1; i <= m; i++) {
        int op;
        cin >> op;

        if (op == 1) {
            // 操作1：查找连续x个空房并入住
            int x;
            cin >> x;
            int left;

            if (len[1] < x) {
                // 没有足够的连续空房
                left = 0;
            } else {
                // 找到最左边的位置并办理入住
                left = queryLeft(x, 1, n, 1);
                // 将找到的x个房间标记为已入住(1表示有人)
                updateRange(left, left + x - 1, 1, 1, n, 1);
            }
            cout << left << '\n';

        } else {
            // 操作2：退房操作，从x开始的y个房间清空
            int x, y;
            cin >> x >> y;
            // 将指定区间的房间标记为空闲(0表示空房)
            // 注意边界处理：不能超过总房间数n
            updateRange(x, min(x + y - 1, n), 0, 1, n, 1);
        }
    }

    return 0;
}
