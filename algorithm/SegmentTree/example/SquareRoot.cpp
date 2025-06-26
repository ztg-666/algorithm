#include <cstdio> // 引入标准输入输出库，用于scanf和printf
#include <cmath> // 引入数学库，用于sqrt函数
#include <algorithm> // 引入算法库，用于max和swap函数

// 范围上开平方并求累加和
// 给定一个长度为n的数组arr，实现以下两种类型的操作
// 操作 0 l r : 把arr[l..r]范围上的每个数开平方，结果向下取整
// 操作 1 l r : 查询arr[l..r]范围上所有数字的累加和
// 两种操作一共发生m次，数据中有可能l>r，遇到这种情况请交换l和r
// 1 <= n, m <= 10^5，1 <= arr[i] <= 10^12
// 测试链接 : https://www.luogu.com.cn/problem/P4145

using namespace std; // 使用标准命名空间，避免每次写std::

const int MAXN = 100001; // 定义数组和线段树支持的最大长度。线段树通常需要4倍空间。
long long arr[MAXN]; // 存储原始数组，用于构建线段树
long long sum[MAXN << 2]; // sum数组：线段树的节点，存储对应区间内所有元素的和。MAXN << 2 等价于 MAXN * 4
long long max_val[MAXN << 2]; // max_val数组：线段树的节点，存储对应区间内的最大值。这个是实现剪枝优化的关键！

// 辅助函数：向上更新（push-up），根据子节点的值更新父节点的值
// 参数i：当前节点的索引
void up(int i) {
    // 父节点的和等于其左右子节点的和
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
    // 父节点的最大值等于其左右子节点的最大值中的较大者
    max_val[i] = max(max_val[i << 1], max_val[i << 1 | 1]);
}

// 构建线段树函数
// 参数l：当前节点代表区间的左边界（1-based index）
// 参数r：当前节点代表区间的右边界（1-based index）
// 参数i：当前节点的索引
void build(int l, int r, int i) {
    // 如果当前区间是叶子节点（l == r），则直接从原始数组arr中取值
    if (l == r) {
        sum[i] = arr[l]; // 叶子节点的和就是它本身的值
        max_val[i] = arr[l]; // 叶子节点的最大值也是它本身的值
    } else { // 如果不是叶子节点，则递归构建子树
        int mid = (l + r) >> 1; // 计算中间点
        build(l, mid, i << 1);     // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 子树构建完成后，向上更新当前节点（父节点）的和与最大值
    }
}

// 核心：带剪枝的区间开平方操作
// 这个操作的关键在于，一旦一个数变成了0或1，它再进行开平方操作结果也不会改变。
// 因此，当一个区间的最大值都小于等于1时，我们就可以停止对这个区间的递归操作。
// 参数jobl：待更新区间的左边界（用户请求的）
// 参数jobr：待更新区间的右边界（用户请求的）
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
void sqrt_update(int jobl, int jobr, int l, int r, int i) {
    // 剪枝条件：如果当前节点区间的最大值已经小于等于1，说明该区间内所有数都是0或1。
    // 对0或1进行开平方操作结果不变，因此无需再递归，直接返回。
    if (max_val[i] <= 1) {
        return;
    }

    // 如果当前区间是叶子节点（l == r），则直接对其值进行开平方更新
    if (l == r) {
        // static_cast<long long>(sqrt(max_val[i])) 进行开平方并取整
        long long sqrt_val = static_cast<long long>(sqrt(max_val[i]));
        sum[i] = sqrt_val;   // 更新和
        max_val[i] = sqrt_val; // 更新最大值
    } else { // 如果不是叶子节点，则递归处理
        int mid = (l + r) >> 1; // 计算中间点

        // 如果待更新区间的左边界在左子节点区间内，并且左子节点的最大值大于1，则递归更新左子树
        // max_val[i << 1] > 1 是关键的剪枝判断，避免不必要的递归
        if (jobl <= mid && max_val[i << 1] > 1) {
            sqrt_update(jobl, jobr, l, mid, i << 1);
        }
        // 如果待更新区间的右边界在右子节点区间内，并且右子节点的最大值大于1，则递归更新右子树
        // max_val[i << 1 | 1] > 1 也是关键的剪枝判断
        if (jobr > mid && max_val[i << 1 | 1] > 1) {
            sqrt_update(jobl, jobr, mid + 1, r, i << 1 | 1);
        }
        up(i); // 子节点更新完成后，向上更新当前节点（父节点）的和与最大值
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

// 主函数
int main() {
    int n, m; // 定义变量：数组长度n，操作数m
    scanf("%d", &n); // 读取数组长度
    for (int i = 1; i <= n; i++) {
        scanf("%lld", &arr[i]); // 读取原始数组元素（1-based indexing）
    }
    build(1, n, 1); // 构建线段树，初始化数据

    scanf("%d", &m); // 读取操作数

    // 循环处理m个操作
    for (int i = 1, op, jobl, jobr; i <= m; i++) {
        scanf("%d%d%d", &op, &jobl, &jobr); // 读取操作类型、左右边界
        // 确保jobl <= jobr，因为用户输入可能不按顺序
        if (jobl > jobr) swap(jobl, jobr);

        if (op == 0) { // 如果操作类型是0（区间开平方更新）
            sqrt_update(jobl, jobr, 1, n, 1); // 调用开平方更新函数
        } else { // 如果操作类型是1（区间求和查询）
            printf("%lld\n", query(jobl, jobr, 1, n, 1)); // 调用求和查询函数并打印结果
        }
    }
    return 0; // 程序正常结束
}
