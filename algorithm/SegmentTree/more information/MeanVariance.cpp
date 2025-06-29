#include<bits/stdc++.h> // 引入万能头文件，包含了大部分常用标准库，方便竞赛编程
// 但在正式项目中，建议按需引入具体的头文件，如 <cstdio>, <algorithm>, <cmath> 等

// 平均数和方差
// 给定一个长度为n的数组arr，操作分为三种类型，一共调用m次
// 操作 1 l r : arr数组中[l, r]范围上每个数字加上k，k为double类型
// 操作 2 l r : 查询arr数组中[l, r]范围上所有数字的平均数，返回double类型
// 操作 3 l r : 查询arr数组中[l, r]范围上所有数字的方差，返回double类型
// 测试链接 : https://www.luogu.com.cn/problem/P1471

using namespace std; // 使用标准命名空间，避免每次使用标准库函数都加 std::

const int MAXN = 100001; // 定义数组最大尺寸常量，预留空间，通常为10万+1，以支持1到10万的索引

// --- 全局数据结构定义 ---

// arr数组：存储原始输入数据。虽然线段树直接操作的是其内部维护的聚合值，但初始构建时需要这个数组。
double arr[MAXN];

// sum1数组：线段树的节点存储。sum1[i] 存储的是线段树中第 i 个节点所代表区间内所有元素的和 (ΣX)。
// 由于线段树通常需要4倍于原数组大小的空间，所以用 MAXN << 2 (即 MAXN * 4) 来预留空间。
double sum1[MAXN << 2];

// sum2数组：线段树的节点存储。sum2[i] 存储的是线段树中第 i 个节点所代表区间内所有元素的平方和 (ΣX^2)。
// 维护平方和是为了高效计算方差，因为方差公式 Var(X) = E(X^2) - (E(X))^2 = (ΣX^2 / N) - (ΣX / N)^2。
double sum2[MAXN << 2];

// addv数组：线段树的懒标记（Lazy Tag）。addv[i] 存储的是线段树中第 i 个节点所代表区间需要额外增加的值 k。
// 当区间更新操作涉及到某个节点时，如果该节点对应的区间完全被更新区间覆盖，我们不立即更新其所有子节点，
// 而是将更新值存储在懒标记中，待需要访问子节点时再下传（pushDown）。
double addv[MAXN << 2];

// --- 线段树辅助函数 ---

/**
 * @brief 向上更新父节点的值。
 * @param i 当前父节点的索引。
 *
 * 当子节点（i<<1 和 i<<1|1）的 sum1 和 sum2 值发生变化后，父节点的值需要根据子节点的值重新计算。
 * 这里是求和线段树，所以父节点的值是其左右子节点值的和。
 */
void up(int i) {
    sum1[i] = sum1[i << 1] + sum1[i << 1 | 1]; // 父节点的总和 = 左子节点的总和 + 右子节点的总和
    sum2[i] = sum2[i << 1] + sum2[i << 1 | 1]; // 父节点的平方和 = 左子节点的平方和 + 右子节点的平方和
}

/**
 * @brief 应用懒标记到当前节点。
 * @param i 当前节点的索引。
 * @param v 要增加的值（懒标记的值）。
 * @param n 当前节点代表区间的长度。
 *
 * 这是懒标记核心的更新逻辑。当我们将值 v 应用到节点 i 代表的长度为 n 的区间时：
 * 1. 区间和 (sum1) 的更新：
 * 原和为 ΣX。每个元素 X 变为 (X+v)。
 * 新和为 Σ(X+v) = ΣX + Σv = ΣX + N*v。
 * 因此，sum1[i] += v * n。
 *
 * 2. 区间平方和 (sum2) 的更新：
 * 原平方和为 ΣX^2。每个元素 X 变为 (X+v)，其平方变为 (X+v)^2 = X^2 + 2Xv + v^2。
 * 新平方和为 Σ(X+v)^2 = Σ(X^2 + 2Xv + v^2)
 * = ΣX^2 + Σ(2Xv) + Σ(v^2)
 * = ΣX^2 + 2v * ΣX + N * v^2。
 * 因此，sum2[i] += sum1[i] (旧值) * v * 2 + v * v * n。
 *
 * 3. 懒标记 (addv) 的更新：
 * 当前节点的懒标记累加上 v。这意味着当前节点已经处理了 v 的影响，
 * 未来如果再次有新的值要加，会在此基础上继续累加。
 */
void lazy(int i, double v, int n) {
    // 根据公式更新 sum2：新ΣX^2 = 旧ΣX^2 + 2*v*旧ΣX + N*v^2
    // 注意：sum1[i]在这里是当前的(未应用v之前的)总和，因为它会先被用来计算sum2的增量
    sum2[i] += sum1[i] * v * 2 + v * v * n;
    // 更新 sum1：新ΣX = 旧ΣX + N*v
    sum1[i] += v * n;
    // 累加懒标记
    addv[i] += v;
}

/**
 * @brief 向下传递懒标记（Lazy Propagation）。
 * @param i 当前带有懒标记的节点索引。
 * @param ln 左子节点代表区间的长度。
 * @param rn 右子节点代表区间的长度。
 *
 * 如果当前节点 i 存在懒标记（addv[i] != 0），则将其懒标记的值下传给左右子节点。
 * 下传时调用 lazy 函数，将 addv[i] 的值和对应子区间的长度传递给子节点。
 * 下传后，当前节点的懒标记清零，表示其自身的懒标记已处理完毕，影响已传递给子节点。
 */
void down(int i, int ln, int rn) {
    if (addv[i] != 0) { // 如果当前节点有懒标记
        lazy(i << 1, addv[i], ln);    // 将懒标记下传给左子节点
        lazy(i << 1 | 1, addv[i], rn); // 将懒标记下传给右子节点
        addv[i] = 0;                  // 清除当前节点的懒标记
    }
}

/**
 * @brief 构建线段树。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点的索引。
 *
 * 这是一个递归函数，用于从底向上构建线段树。
 * 叶子节点（l == r）直接赋值为对应的原始数组元素及其平方。
 * 非叶子节点通过递归构建左右子树，然后调用 up 函数更新自己的 sum1 和 sum2 值。
 * 同时初始化所有节点的懒标记为0。
 */
void build(int l, int r, int i) {
    if (l == r) { // 如果是叶子节点
        sum1[i] = arr[l];        // 叶子节点存储原始元素值
        sum2[i] = arr[l] * arr[l]; // 叶子节点存储原始元素值的平方
    } else { // 如果是非叶子节点
        int mid = (l + r) >> 1;     // 计算中间点
        build(l, mid, i << 1);        // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 更新当前节点的值
    }
    addv[i] = 0; // 初始化懒标记为0
}

/**
 * @brief 对线段树进行区间增加操作。
 * @param jobl 操作区间的左边界。
 * @param jobr 操作区间的右边界。
 * @param jobv 要增加的值。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点的索引。
 *
 * 这是一个递归函数，用于在线段树上对指定区间 [jobl, jobr] 加上 jobv。
 * 它利用懒标记进行高效的区间更新。
 */
void add(int jobl, int jobr, double jobv, int l, int r, int i) {
    // 如果当前节点代表的区间 [l, r] 完全包含在操作区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        lazy(i, jobv, r - l + 1); // 直接应用懒标记到当前节点，并更新其 sum1 和 sum2
    } else { // 否则，当前区间与操作区间有交叠，需要向下递归
        int mid = (l + r) >> 1; // 计算中间点
        // 在向下递归之前，需要先下传当前节点的懒标记，确保子节点是最新的
        down(i, mid - l + 1, r - mid);

        // 如果操作区间与左子树有交叠，则递归更新左子树
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果操作区间与右子树有交叠，则递归更新右子树
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i); // 递归返回后，更新当前节点的值（根据其子节点）
    }
}

/**
 * @brief 对线段树进行区间查询操作（求和或平方和）。
 * @param sum 指向要查询的聚合数组（sum1 或 sum2）。
 * @param jobl 查询区间的左边界。
 * @param jobr 查询区间的右边界。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点的索引。
 * @return 查询区间内的和或平方和。
 *
 * 这是一个递归函数，用于在线段树上查询指定区间 [jobl, jobr] 内的 sum1 或 sum2。
 * 查询时同样需要处理懒标记。
 */
double query(double *sum, int jobl, int jobr, int l, int r, int i) {
    // 如果当前节点代表的区间 [l, r] 完全包含在查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return sum[i]; // 直接返回当前节点存储的聚合值
    }
    int mid = (l + r) >> 1; // 计算中间点
    // 在向下递归之前，需要先下传当前节点的懒标记
    down(i, mid - l + 1, r - mid);
    double ans = 0; // 初始化查询结果

    // 如果查询区间与左子树有交叠，则递归查询左子树
    if (jobl <= mid) {
        ans += query(sum, jobl, jobr, l, mid, i << 1);
    }
    // 如果查询区间与右子树有交叠，则递归查询右子树
    if (jobr > mid) {
        ans += query(sum, jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans; // 返回累加的查询结果
}

// --- 主函数 ---

int main() {
    int n, m;
    scanf("%d %d", &n, &m); // 读取序列长度n和操作次数m

    // 读取原始数组元素
    for (int i = 1; i <= n; i++) {
        scanf("%lf", &arr[i]);
    }

    // 构建线段树，初始化 sum1, sum2 和 addv
    build(1, n, 1);

    // 处理 m 次操作
    for (int i = 1; i <= m; i++) {
        int op, jobl, jobr;
        scanf("%d", &op); // 读取操作类型
        if (op == 1) { // 类型1操作：区间增加
            double jobv;
            scanf("%d %d %lf", &jobl, &jobr, &jobv); // 读取区间范围和增加值
            add(jobl, jobr, jobv, 1, n, 1); // 调用区间增加函数
        } else if (op == 2) { // 类型2操作：查询区间平均数
            scanf("%d %d", &jobl, &jobr); // 读取查询区间范围
            // 区间平均数 = 区间总和 / 区间长度
            double ans = query(sum1, jobl, jobr, 1, n, 1) / (jobr - jobl + 1);
            printf("%.4f\n", ans); // 输出结果，保留4位小数
        } else { // 类型3操作：查询区间方差
            scanf("%d %d", &jobl, &jobr); // 读取查询区间范围
            // 方差公式：Var(X) = E(X^2) - (E(X))^2
            // E(X) = (ΣX) / N
            // E(X^2) = (ΣX^2) / N
            double sum_val = query(sum1, jobl, jobr, 1, n, 1); // 获取区间总和 (ΣX)
            double sum_sq_val = query(sum2, jobl, jobr, 1, n, 1); // 获取区间平方和 (ΣX^2)
            double size = jobr - jobl + 1; // 区间长度 (N)

            double mean = sum_val / size; // 期望值 E(X)
            double mean_sq = sum_sq_val / size; // 期望的平方 E(X^2)

            double ans = mean_sq - mean * mean; // 方差 = E(X^2) - (E(X))^2
            printf("%.4f\n", ans); // 输出结果，保留4位小数
        }
    }
    return 0; // 程序正常结束
}
