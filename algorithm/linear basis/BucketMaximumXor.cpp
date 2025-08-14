#include <bits/stdc++.h>

// P哥的桶
// 一共有n个桶，排成一排，编号1~n，每个桶可以装下任意个数字
// 高效的实现如下两个操作
// 操作 1 k v : 把数字v放入k号桶中
// 操作 2 l r : 可以从l..r号桶中随意拿数字，返回异或和最大的结果
// 1 <= n、m <= 5 * 10^4
// 0 <= v <= 2^31 - 1
// 测试链接 : https://www.luogu.com.cn/problem/P4839


using namespace std;

const int MAXN = 50001;  // 最大 m（坐标范围）
const int BIT = 30;      // 最大考虑的二进制位数（假设数据 <= 2^30）

// 线段树节点的线性基，每个节点存一个 BIT+1 大小的数组
// treeBasis[i][j] 表示线段树节点 i 的线性基中第 j 位的向量
int treeBasis[MAXN << 2][BIT + 1];

// 临时全局线性基，用于 query 时合并
int basis[BIT + 1];

/**
 * 向线性基 b[] 中插入一个数 num
 * 返回 true 表示插入成功（产生了新基向量）
 * 返回 false 表示插入失败（num 可由已有基向量异或得到）
 */
bool insertBasis(int b[], int num) {
    for (int i = BIT; i >= 0; i--) {        // 从高位到低位处理
        if ((num >> i) & 1) {               // num 在第 i 位有 1
            if (b[i] == 0) {                 // 如果该位没有基向量
                b[i] = num;                  // 把 num 加入基向量
                return true;
            }
            num ^= b[i];                     // 消去最高位，继续尝试插入
        }
    }
    return false; // num 已经可以由现有基向量组合得到
}

/**
 * 向线段树中 idx 位置添加一个值 val
 * i 表示当前线段树节点编号，区间 [l, r] 表示该节点管理的范围
 */
void add(int idx, int val, int l, int r, int i) {
    insertBasis(treeBasis[i], val); // 把 val 加入当前节点的线性基
    if (l < r) {                    // 不是叶子节点，递归向下更新
        int mid = (l + r) >> 1;
        if (idx <= mid) {
            add(idx, val, l, mid, i << 1);      // 左子树
        } else {
            add(idx, val, mid + 1, r, i << 1 | 1); // 右子树
        }
    }
}

/**
 * 从线段树中查询 [ql, qr] 区间内的所有值，并合并到全局 basis 中
 */
void mergeQuery(int ql, int qr, int l, int r, int i) {
    if (ql <= l && r <= qr) { // 当前节点完全在查询区间内
        for (int j = BIT; j >= 0; j--) {
            if (treeBasis[i][j] != 0) {
                insertBasis(basis, treeBasis[i][j]);
            }
        }
    } else { // 部分覆盖，递归左右子树
        int mid = (l + r) >> 1;
        if (ql <= mid) mergeQuery(ql, qr, l, mid, i << 1);
        if (qr > mid) mergeQuery(ql, qr, mid + 1, r, i << 1 | 1);
    }
}

/**
 * 查询区间 [ql, qr] 的最大异或值
 * m 是线段树管理的最大位置
 */
int query(int ql, int qr, int m) {
    memset(basis, 0, sizeof(basis));      // 清空临时线性基
    mergeQuery(ql, qr, 1, m, 1);          // 合并区间内所有节点的基向量
    int ans = 0;
    for (int j = BIT; j >= 0; j--) {      // 贪心从高位到低位选择
        ans = max(ans, ans ^ basis[j]);
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m; // n 个操作, 坐标范围 [1, m]

    for (int _ = 0; _ < n; _++) {
        int op;
        cin >> op;
        if (op == 1) {
            // 插入操作：1 idx val
            int idx, val;
            cin >> idx >> val;
            add(idx, val, 1, m, 1);
        } else {
            // 查询操作：2 l r
            int l, r;
            cin >> l >> r;
            cout << query(l, r, m) << "\n";
        }
    }
    return 0;
}
