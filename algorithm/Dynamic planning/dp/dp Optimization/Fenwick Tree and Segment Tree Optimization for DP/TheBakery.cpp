#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

// 划分k段的最大得分
// 给定一个长度为n的数组，最多可以分成k段不重合的子数组
// 每个子数组获得的分值为内部不同数字的个数
// 返回能获得的最大分值
// 1 <= n <= 35000
// 1 <= k <= 50
// k <= n
// 1 <= arr[i] <= n
// 测试链接 : https://www.luogu.com.cn/problem/CF833B
// 测试链接 : https://codeforces.com/problemset/problem/833/B

using namespace std;

// 数组最大长度
const int MAXN = 35001;

// n: 数组长度, k: 最多分段数
int n, k;
// arr: 输入数组，下标从1开始
int arr[MAXN];
// dp[i]: 前i个元素分成若干段能获得的最大分值
int dp[MAXN];
// pre[v]: 值v上次出现的位置
int pre[MAXN];
// 线段树相关数组
int maxVal[MAXN << 2];  // 维护区间最大值
int add[MAXN << 2];     // 懒标记，记录需要加的值

// 线段树向上更新：父节点取左右子节点的最大值
void up(int i) {
    maxVal[i] = max(maxVal[i << 1], maxVal[i << 1 | 1]);
}

// 懒标记：给节点i加上值v
void lazy(int i, int v) {
    maxVal[i] += v;
    add[i] += v;
}

// 线段树向下传播懒标记
void down(int i) {
    if (add[i] != 0) {
        lazy(i << 1, add[i]);      // 传给左子树
        lazy(i << 1 | 1, add[i]);  // 传给右子树
        add[i] = 0;                // 清空当前节点的懒标记
    }
}

// 构建线段树，区间[l,r]，节点编号i
// 用当前的dp数组值来初始化线段树
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：直接赋值为dp[l]
        maxVal[i] = dp[l];
    } else {
        int mid = (l + r) >> 1;
        // 递归构建左右子树
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        // 向上更新
        up(i);
    }
    add[i] = 0;  // 初始化懒标记为0
}

// 区间更新：给区间[jobl,jobr]的所有位置加上jobv
void addRange(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 完全覆盖：直接给当前节点打懒标记
        lazy(i, jobv);
    } else {
        // 部分覆盖：先向下传播懒标记，再递归处理子区间
        down(i);
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            addRange(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            addRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        // 向上更新
        up(i);
    }
}

// 区间查询：查询区间[jobl,jobr]的最大值
int query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 完全覆盖：直接返回当前节点的值
        return maxVal[i];
    }
    // 部分覆盖：先向下传播懒标记，再查询子区间
    down(i);
    int mid = (l + r) >> 1;
    int ans = INT_MIN;
    if (jobl <= mid) {
        ans = max(ans, query(jobl, jobr, l, mid, i << 1));
    }
    if (jobr > mid) {
        ans = max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1));
    }
    return ans;
}

// 主要计算函数：动态规划+线段树优化
int compute() {
    // 初始化dp数组为0
    fill(dp + 1, dp + n + 1, 0);

    // 外层循环：枚举分段数t（1到k段）
    for (int t = 1; t <= k; t++) {
        // 每轮开始时重新构建线段树
        build(0, n, 1);
        // 初始化pre数组：记录每个值上次出现的位置
        fill(pre + 1, pre + n + 1, 0);

        // 内层循环：枚举当前考虑的位置i
        for (int i = 1; i <= n; i++) {
            // 关键操作：给区间[pre[arr[i]], i-1]的所有位置加1
            // 这表示以位置j(pre[arr[i]] <= j <= i-1)结尾的前一段，
            // 如果再加上[j+1, i]这一段，会新增一个不同的数字arr[i]
            addRange(pre[arr[i]], i - 1, 1, 0, n, 1);

            // 只有当i >= t时才能分成t段（至少需要t个元素）
            if (i >= t) {
                // dp[i] = max{dp[j] + distinct_count(j+1, i)} for all valid j
                // 这里通过线段树查询[0, i-1]的最大值来得到答案
                dp[i] = query(0, i - 1, 0, n, 1);
            }

            // 更新arr[i]上次出现的位置
            pre[arr[i]] = i;
        }
    }
    return dp[n];  // 返回前n个元素分成最多k段的最大分值
}

int main() {
    // 加速输入输出
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读入n和k
    cin >> n >> k;
    // 读入数组，下标从1开始
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出结果
    cout << compute() << endl;
    
    return 0;
}
