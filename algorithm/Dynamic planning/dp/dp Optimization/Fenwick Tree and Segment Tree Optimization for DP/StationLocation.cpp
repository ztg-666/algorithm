#include <iostream>
#include <algorithm>
#include <climits>
#include <vector>

// 基站选址
// 一共有n个村庄排成一排，从左往右依次出现1号、2号、3号..n号村庄
// dist[i]表示i号村庄到1号村庄的距离，该数组一定有序且无重复值
// fix[i]表示i号村庄建立基站的安装费用
// range[i]表示i号村庄的接收范围，任何基站和i号村庄的距离不超过这个数字，i号村庄就能得到服务
// warranty[i]表示如果i号村庄最终没有得到任何基站的服务，需要给多少赔偿费用
// 最多可以选择k个村庄安装基站，返回总花费最少是多少，总花费包括安装费用和赔偿费用
// 1 <= n <= 20000
// 1 <= k <= 100
// k <= n
// 1 <= dist[i] <= 10^9
// 1 <= fix[i] <= 10^4
// 1 <= range[i] <= 10^9
// 1 <= warranty[i] <= 10^4
// 测试链接 : https://www.luogu.com.cn/problem/P2605

using namespace std;

const int MAXN = 20002; // 最大村庄数量（包括补充的无穷远村庄）

int n, k; // n: 村庄数量, k: 最多可建基站数量
int dist[MAXN];     // dist[i]: i号村庄到1号村庄的距离
int fix[MAXN];      // fix[i]: i号村庄建立基站的安装费用
int range[MAXN];    // range[i]: i号村庄的接收范围
int warranty[MAXN]; // warranty[i]: i号村庄未得到服务时的赔偿费用
int left_bound[MAXN];  // left_bound[i]: 最左在第几号村庄建基站，i号村庄依然能获得服务
int right_bound[MAXN]; // right_bound[i]: 最右在第几号村庄建基站，i号村庄依然能获得服务

// 链式前向星 - 用于存储每个村庄的预警列表
// 预警列表：如果只有一个基站建在i号村庄，现在要移动到i+1号村庄，哪些村庄会失去服务
int head[MAXN];     // 链表头指针
int next_edge[MAXN]; // 下一条边的索引
int to[MAXN];       // 边的终点
int cnt;            // 边的计数器

// 线段树 - 用于维护最小值和区间加法
int seg_min[MAXN << 2]; // 线段树维护最小值
int seg_add[MAXN << 2]; // 线段树懒标记（区间加法）

// 动态规划表
// dp[i]表示最右的基站建在i号村庄时的最小花费
int dp[MAXN];

/**
 * 在dist数组中二分查找 >= d的最左位置
 * @param d 目标距离
 * @return 满足条件的最左村庄编号
 */
int search(int d) {
    int l = 1, r = n, m;
    int ans = 0;
    while (l <= r) {
        m = (l + r) / 2;
        if (dist[m] >= d) {
            ans = m;
            r = m - 1;
        } else {
            l = m + 1;
        }
    }
    return ans;
}

/**
 * 链式前向星加边 - 将v加入u的预警列表
 * @param u 起点村庄
 * @param v 终点村庄
 */
void addEdge(int u, int v) {
    next_edge[cnt] = head[u];
    to[cnt] = v;
    head[u] = cnt++;
}

/**
 * 线段树向上更新节点值
 */
void up(int i) {
    seg_min[i] = min(seg_min[i << 1], seg_min[i << 1 | 1]);
}

/**
 * 线段树懒标记传递
 * @param i 节点索引
 * @param v 加法值
 */
void lazy(int i, int v) {
    seg_min[i] += v;
    seg_add[i] += v;
}

/**
 * 线段树向下推送懒标记
 */
void down(int i) {
    if (seg_add[i] != 0) {
        lazy(i << 1, seg_add[i]);
        lazy(i << 1 | 1, seg_add[i]);
        seg_add[i] = 0;
    }
}

/**
 * 建立线段树
 * @param l 左边界
 * @param r 右边界
 * @param i 当前节点索引
 */
void build(int l, int r, int i) {
    if (l == r) {
        seg_min[i] = dp[l]; // 叶子节点直接赋值
    } else {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);         // 递归建立左子树
        build(mid + 1, r, i << 1 | 1); // 递归建立右子树
        up(i);                         // 向上更新当前节点
    }
    seg_add[i] = 0; // 初始化懒标记
}

/**
 * 线段树区间加法
 * @param jobl 操作左边界
 * @param jobr 操作右边界
 * @param jobv 加法值
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点索引
 */
void add(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全被操作区间包含
        lazy(i, jobv);
    } else {
        down(i); // 先推送懒标记
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1); // 递归处理左子树
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1); // 递归处理右子树
        }
        up(i); // 向上更新
    }
}

/**
 * 线段树区间查询最小值
 * @param jobl 查询左边界
 * @param jobr 查询右边界
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点索引
 * @return 区间最小值
 */
int query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全被查询区间包含
        return seg_min[i];
    }
    down(i); // 先推送懒标记
    int mid = (l + r) >> 1;
    int ans = INT_MAX;
    if (jobl <= mid) {
        ans = min(ans, query(jobl, jobr, l, mid, i << 1)); // 查询左子树
    }
    if (jobr > mid) {
        ans = min(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1)); // 查询右子树
    }
    return ans;
}

/**
 * 预处理函数
 * 1. 计算每个村庄的服务范围边界
 * 2. 建立预警列表
 */
void prepare() {
    cnt = 1; // 边计数器从1开始（0用作链表结束标志）

    for (int i = 1; i <= n; i++) {
        // 计算i号村庄能被服务的基站位置范围
        left_bound[i] = search(dist[i] - range[i]);   // 最左边界
        right_bound[i] = search(dist[i] + range[i]);  // 最右边界

        // 如果最右位置的基站实际上无法服务i号村庄，则边界向左移动
        if (dist[right_bound[i]] > dist[i] + range[i]) {
            right_bound[i]--;
        }

        // 建立预警列表：如果基站从right_bound[i]移动到right_bound[i]+1，
        // i号村庄将失去服务
        addEdge(right_bound[i], i);
    }
}

/**
 * 主要计算函数 - 使用动态规划求解最小总花费
 * @return 最小总花费
 */
int compute() {
    // 第一阶段：最多建1个基站的情况
    // dp[i] = 前面所有无法覆盖村庄的赔偿费用 + 在i建基站的费用
    for (int i = 1, w = 0; i <= n; i++) {
        dp[i] = w + fix[i];
        // 检查i号村庄的预警列表，累计赔偿费用
        for (int ei = head[i]; ei != 0; ei = next_edge[ei]) {
            w += warranty[to[ei]];
        }
    }

    // 第二阶段：最多建t >= 2个基站的情况
    for (int t = 2; t <= k + 1; t++) {
        build(1, n, 1); // 用当前dp值重建线段树

        for (int i = 1; i <= n; i++) {
            if (i >= t) {
                // dp[i] = min(之前的dp[i], 前面最优解 + 在i建基站的费用)
                dp[i] = min(dp[i], query(1, i - 1, 1, n, 1) + fix[i]);
            }

            // 处理i号村庄的预警列表
            // 如果基站从i移动到i+1，预警列表中的村庄将失去服务
            for (int ei = head[i]; ei != 0; ei = next_edge[ei]) {
                int uncover = to[ei]; // 将要失去服务的村庄

                if (left_bound[uncover] > 1) {
                    add(1, left_bound[uncover] - 1, warranty[uncover], 1, n, 1);
                }
            }
        }
    }

    return dp[n]; // 返回最优解（包括补充村庄的解）
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入基本信息
    cin >> n >> k;

    // 读入距离信息（1号村庄距离为0，从2号开始）
    for (int i = 2; i <= n; i++) {
        cin >> dist[i];
    }

    // 读入建站费用
    for (int i = 1; i <= n; i++) {
        cin >> fix[i];
    }

    // 读入接收范围
    for (int i = 1; i <= n; i++) {
        cin >> range[i];
    }

    // 读入赔偿费用
    for (int i = 1; i <= n; i++) {
        cin >> warranty[i];
    }

    // 关键技巧：补充一个位于无穷远处的虚拟村庄
    // 这样dp[n]就表示真实村庄的最优解，简化边界处理
    dist[++n] = INT_MAX;
    fix[n] = range[n] = warranty[n] = 0;

    // 预处理和求解
    prepare();
    cout << compute() << endl;
    
    return 0;
}
