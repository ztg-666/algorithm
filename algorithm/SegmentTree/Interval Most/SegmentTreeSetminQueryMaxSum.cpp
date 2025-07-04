#include <cstdio>
#include <algorithm>
#include <climits>

// 线段树的区间最值操作(hdu测试)
// 给定一个长度为n的数组arr，实现支持以下三种操作的结构
// 操作 0 l r x : 把arr[l..r]范围的每个数v，更新成min(v, x)
// 操作 1 l r   : 查询arr[l..r]范围上的最大值
// 操作 2 l r   : 查询arr[l..r]范围上的累加和
// 三种操作一共调用m次，做到时间复杂度O(n * log n + m * log n)
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=5306

using namespace std;

const int MAXN = 1000001;     // 最大数组长度
const int LOWEST = -1;        // 表示负无穷的标记值

// 线段树节点信息
long long sum[MAXN << 2];     // 区间和
int max_val[MAXN << 2];       // 区间最大值
int cnt[MAXN << 2];           // 最大值的个数
int sem[MAXN << 2];           // 严格次大值(second maximum)

/**
 * 向上更新节点信息
 * 合并左右子树的信息到当前节点
 */
inline void up(int i) {
    int l = i << 1;           // 左子节点
    int r = i << 1 | 1;       // 右子节点

    // 获取左右子树信息
    long long sum_l = sum[l], sum_r = sum[r];
    int max_l = max_val[l], max_r = max_val[r];
    int cnt_l = cnt[l], cnt_r = cnt[r];
    int sem_l = sem[l], sem_r = sem[r];

    // 更新区间和
    sum[i] = sum_l + sum_r;

    // 更新最大值、最大值个数、严格次大值
    if (max_l > max_r) {
        // 左子树最大值更大
        max_val[i] = max_l;
        cnt[i] = cnt_l;
        sem[i] = (sem_l > max_r) ? sem_l : max_r;  // 次大值是左子树次大值和右子树最大值的较大者
    } else if (max_l < max_r) {
        // 右子树最大值更大
        max_val[i] = max_r;
        cnt[i] = cnt_r;
        sem[i] = (max_l > sem_r) ? max_l : sem_r;  // 次大值是左子树最大值和右子树次大值的较大者
    } else {
        // 左右子树最大值相等
        max_val[i] = max_l;
        cnt[i] = cnt_l + cnt_r;                    // 最大值个数相加
        sem[i] = (sem_l > sem_r) ? sem_l : sem_r;  // 次大值是两个次大值的较大者
    }
}

/**
 * 懒标记下推：将区间内所有大于v的值设为v
 * 这是区间取最小值操作的核心
 */
inline void lazy(int i, int v) {
    if (v < max_val[i]) {
        // 只有当v小于当前最大值时才需要更新
        // 减去被修改的值的总和：(原最大值 - 新值) * 最大值个数
        sum[i] -= ((long long)(max_val[i] - v)) * cnt[i];
        max_val[i] = v;  // 更新最大值
    }
}

/**
 * 向下传播懒标记
 * 将当前节点的懒标记传递给子节点
 */
inline void down(int i) {
    int max_i = max_val[i];
    lazy(i << 1, max_i);      // 传递给左子节点
    lazy(i << 1 | 1, max_i);  // 传递给右子节点
}

/**
 * 建树：初始化线段树
 * l, r: 当前区间范围
 * i: 当前节点编号
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：读取并设置初始值
        int val;
        scanf("%d", &val);
        sum[i] = max_val[i] = val;  // 区间和和最大值都是该值
        cnt[i] = 1;                 // 最大值个数为1
        sem[i] = LOWEST;            // 严格次大值设为负无穷
    } else {
        // 内部节点：递归建树
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);          // 建左子树
        build(mid + 1, r, i << 1 | 1);  // 建右子树
        up(i);                          // 更新当前节点信息
    }
}

/**
 * 区间取最小值操作：将[jobl, jobr]区间内所有大于jobv的值设为jobv
 * jobl, jobr: 操作区间
 * jobv: 目标值
 * l, r: 当前节点对应区间
 * i: 当前节点编号
 */
void setMin(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobv >= max_val[i]) {
        // 如果目标值不小于当前区间最大值，无需操作
        return;
    }

    if (jobl <= l && r <= jobr && sem[i] < jobv) {
        // 完全包含且次大值小于目标值，可以直接操作
        // 这意味着区间内只有最大值会被修改
        lazy(i, jobv);
        return;
    }

    // 需要递归到子节点
    down(i);  // 先下推懒标记
    int mid = (l + r) >> 1;
    if (jobl <= mid) {
        setMin(jobl, jobr, jobv, l, mid, i << 1);      // 操作左子树
    }
    if (jobr > mid) {
        setMin(jobl, jobr, jobv, mid + 1, r, i << 1 | 1); // 操作右子树
    }
    up(i);  // 更新当前节点信息
}

/**
 * 查询区间最大值
 * jobl, jobr: 查询区间
 * l, r: 当前节点对应区间
 * i: 当前节点编号
 */
int queryMax(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 完全包含，直接返回
        return max_val[i];
    }

    down(i);  // 下推懒标记
    int mid = (l + r) >> 1;
    int ans = INT_MIN;  // 初始化为负无穷

    if (jobl <= mid) {
        // 查询左子树
        int left_ans = queryMax(jobl, jobr, l, mid, i << 1);
        ans = (ans > left_ans) ? ans : left_ans;
    }
    if (jobr > mid) {
        // 查询右子树
        int right_ans = queryMax(jobl, jobr, mid + 1, r, i << 1 | 1);
        ans = (ans > right_ans) ? ans : right_ans;
    }

    return ans;
}

/**
 * 查询区间和
 * jobl, jobr: 查询区间
 * l, r: 当前节点对应区间
 * i: 当前节点编号
 */
long long querySum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 完全包含，直接返回
        return sum[i];
    }

    down(i);  // 下推懒标记
    int mid = (l + r) >> 1;
    long long ans = 0;

    if (jobl <= mid) {
        // 查询左子树
        ans += querySum(jobl, jobr, l, mid, i << 1);
    }
    if (jobr > mid) {
        // 查询右子树
        ans += querySum(jobl, jobr, mid + 1, r, i << 1 | 1);
    }

    return ans;
}

int main() {
    int testCases;
    scanf("%d", &testCases);

    while (testCases--) {
        int n, m;  // n: 数组长度, m: 操作次数
        scanf("%d %d", &n, &m);

        build(1, n, 1);  // 建树

        for (int i = 0; i < m; i++) {
            int op;
            scanf("%d", &op);

            if (op == 0) {
                // 区间取最小值操作
                int jobl, jobr, jobv;
                scanf("%d %d %d", &jobl, &jobr, &jobv);
                setMin(jobl, jobr, jobv, 1, n, 1);
            } else if (op == 1) {
                // 查询区间最大值
                int jobl, jobr;
                scanf("%d %d", &jobl, &jobr);
                printf("%d\n", queryMax(jobl, jobr, 1, n, 1));
            } else {
                // 查询区间和
                int jobl, jobr;
                scanf("%d %d", &jobl, &jobr);
                printf("%lld\n", querySum(jobl, jobr, 1, n, 1));
            }
        }
    }

    return 0;
}
