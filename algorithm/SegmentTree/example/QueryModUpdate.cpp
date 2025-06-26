#include <cstdio> // 引入标准输入输出库，用于scanf和printf
#include <algorithm> // 引入算法库，用于max函数

// 包含取模操作的线段树
// 给定一个长度为n的数组arr，实现如下三种操作，一共调用m次
// 操作 1 l r : 查询arr[l..r]的累加和
// 操作 2 l r x : 把arr[l..r]上每个数字对x取模
// 操作 3 k x : 把arr[k]上的数字设置为x
// 1 <= n, m <= 10^5，操作1得到的结果，有可能超过int范围

using namespace std; // 使用标准命名空间，避免每次写std::

const int MAXN = 100001; // 定义数组和线段树支持的最大长度。线段树通常需要4倍的空间。
int arr[MAXN]; // 存储原始数组，用于构建线段树
long long sum[MAXN << 2]; // sum数组：线段树的节点，存储对应区间内所有元素的和。MAXN << 2 等价于 MAXN * 4
int maxVal[MAXN << 2]; // maxVal数组：线段树的节点，存储对应区间内的最大值。这是实现区间取模操作（mod）剪枝优化的关键！

// 辅助函数：向上更新（push-up），根据子节点的值更新父节点的值
// 参数i：当前节点的索引
void up(int i) {
    // 父节点的和等于其左右子节点的和
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
    // 父节点的最大值等于其左右子节点的最大值中的较大者
    maxVal[i] = max(maxVal[i << 1], maxVal[i << 1 | 1]);
}

// 构建线段树函数
// 参数l：当前节点代表区间的左边界（1-based index）
// 参数r：当前节点代表区间的右边界（1-based index）
// 参数i：当前节点的索引
void build(int l, int r, int i) {
    // 如果当前区间是叶子节点（l == r），则直接从原始数组arr中取值
    if (l == r) {
        sum[i] = maxVal[i] = arr[l]; // 叶子节点的和与最大值都是它本身的值
    } else { // 如果不是叶子节点，则递归构建子树
        int mid = (l + r) >> 1; // 计算中间点
        build(l, mid, i << 1);     // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 子树构建完成后，向上更新当前节点（父节点）的和与最大值
    }
}

// 区间查询函数：查询jobl到jobr区间的和
// 参数jobl：待查询区间的左边界（用户请求的）
// 参数jobr：待查询区间的右边界（用户请求的）
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
long long query(int jobl, int jobr, int l, int r, int i) {
    // 情况1: 当前节点代表的区间 [l, r] 完全包含在待查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return sum[i]; // 直接返回当前节点的和
    }
    int mid = (l + r) >> 1; // 计算中间点
    long long ans = 0; // 初始化查询结果

    // 如果待查询区间的左边界在左子节点区间内，递归查询左子树
    if (jobl <= mid) {
        ans += query(jobl, jobr, l, mid, i << 1);
    }
    // 如果待查询区间的右边界在右子节点区间内，递归查询右子树
    if (jobr > mid) {
        ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans; // 返回查询结果
}

// 核心：带剪枝的区间取模操作
// 这个操作的关键在于，如果一个区间的最大值已经小于模数jobv，
// 那么该区间内的所有数对jobv取模后都将保持不变。
// 因此，当maxVal[i] < jobv时，我们就可以停止对这个区间的递归操作。
// 参数jobl：待更新区间的左边界（用户请求的）
// 参数jobr：待更新区间的右边界（用户请求的）
// 参数jobv：取模的模数
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
void mod(int jobl, int jobr, int jobv, int l, int r, int i) {
    // 剪枝条件：如果当前节点区间的最大值已经小于等于模数jobv，
    // 说明该区间内所有数x都满足 x % jobv = x。
    // 因此，无需再递归，直接返回。
    // 注意：这里的判断条件`jobv > maxVal[i]` 实际上等价于 `maxVal[i] < jobv`
    if (jobv > maxVal[i]) {
        return;
    }

    // 如果当前区间是叶子节点（l == r），则直接对其值进行取模更新
    if (l == r) {
        sum[i] %= jobv;   // 更新和
        maxVal[i] = sum[i]; // 更新最大值（因为取模后值可能变小）
    } else { // 如果不是叶子节点，则递归处理
        int mid = (l + r) >> 1; // 计算中间点

        // 如果待更新区间的左边界在左子节点区间内，则递归更新左子树
        // 这里不再有 maxVal[i << 1] > 1 的剪枝，因为对于取模操作，子区间的最大值与模数的关系才决定是否需要递归。
        // 子区间的具体剪枝会在递归调用内部的 `if (jobv > maxVal[i])` 处进行。
        if (jobl <= mid) {
            mod(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果待更新区间的右边界在右子节点区间内，则递归更新右子树
        if (jobr > mid) {
            mod(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i); // 子节点更新完成后，向上更新当前节点（父节点）的和与最大值
    }
}

// 单点更新函数：将jobi位置的值更新为jobv
// 参数jobi：待更新点的索引（用户请求的）
// 参数jobv：更新的目标值
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
void update(int jobi, int jobv, int l, int r, int i) {
    // 如果当前区间是叶子节点且是目标位置，则直接更新
    if (l == r) {
        sum[i] = maxVal[i] = jobv; // 更新和与最大值
    } else { // 如果不是叶子节点，则递归向下查找
        int mid = (l + r) >> 1; // 计算中间点
        if (jobi <= mid) { // 如果目标点在左子树
            update(jobi, jobv, l, mid, i << 1);
        } else { // 如果目标点在右子树
            update(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i); // 子节点更新完成后，向上更新当前节点（父节点）的和与最大值
    }
}

// 主函数
int main() {
    int n, m; // 定义变量：数组长度n，操作数m
    scanf("%d%d", &n, &m); // 读取数组长度和操作数
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]); // 读取原始数组元素（1-based indexing）
    }
    build(1, n, 1); // 构建线段树，初始化数据

    // 循环处理m个操作
    for (int i = 1, op; i <= m; i++) {
        scanf("%d", &op); // 读取操作类型
        if (op == 1) { // 如果操作类型是1（区间求和查询）
            int jobl, jobr;
            scanf("%d%d", &jobl, &jobr); // 读取查询区间的左右边界
            printf("%lld\n", query(jobl, jobr, 1, n, 1)); // 调用求和查询函数并打印结果
        } else if (op == 2) { // 如果操作类型是2（区间取模更新）
            int jobl, jobr, jobv;
            scanf("%d%d%d", &jobl, &jobr, &jobv); // 读取更新区间的左右边界和模数
            mod(jobl, jobr, jobv, 1, n, 1); // 调用区间取模更新函数
        } else { // 如果操作类型是3（单点更新）
            int jobi, jobv;
            scanf("%d%d", &jobi, &jobv); // 读取更新点的索引和目标值
            update(jobi, jobv, 1, n, 1); // 调用单点更新函数
        }
    }
    return 0; // 程序正常结束
}
