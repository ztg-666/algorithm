#include <cstdio> // 引入标准输入输出库，用于scanf和printf
#include <vector> // 引入vector容器，用于insert函数返回结果
#include <algorithm> // 引入算法库，用于min函数

// 瓶子里的花朵
// 给定n个瓶子，编号从0~n-1，一开始所有瓶子都是空的
// 每个瓶子最多插入一朵花，实现以下两种类型的操作
// 操作 1 from flower : 一共有flower朵花，从from位置开始依次插入花朵，已经有花的瓶子跳过
//                     如果一直到最后的瓶子，花也没有用完，就丢弃剩下的花朵
//                     返回这次操作插入的首个空瓶的位置 和 最后空瓶的位置
//                     如果从from开始所有瓶子都有花，打印"Can not put any one."
// 操作 2 left right  : 从left位置开始到right位置的瓶子，变回空瓶，返回清理花朵的数量
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=4614


using namespace std; // 使用标准命名空间，避免每次写std::

const int MAXN = 50001; // 定义线段树支持的最大区间长度（N的最大值）
// 线段树通常需要4倍的空间来存储节点，因为一个节点可能需要存储左右子节点的信息，层数是logN，所以通常是4*N
int sum[MAXN << 2]; // sum数组：线段树的节点，存储对应区间内“有花”（值为1）的数量总和。MAXN << 2 等价于 MAXN * 4
int change[MAXN << 2]; // change数组：懒惰标记（lazy tag），用于区间更新操作。存储需要将当前区间内所有值更新为的特定值（0或1）。
bool update[MAXN << 2]; // update数组：懒惰标记的状态，如果为true，表示当前节点有一个待下推的更新操作。
int n; // 全局变量n，表示当前处理的花坛总长度。

// 辅助函数：向上更新（push-up），根据子节点的值更新父节点的值
// 参数i：当前节点的索引
void up(int i) {
    // 父节点的和等于其左右子节点的和。
    // i << 1 是左子节点的索引 (2*i)
    // i << 1 | 1 是右子节点的索引 (2*i + 1)
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
}

// 辅助函数：向下传递懒惰标记（push-down），将父节点的更新操作下推到子节点
// 参数i：当前节点的索引
// 参数ln：左子节点代表的区间长度
// 参数rn：右子节点代表的区间长度
void down(int i, int ln, int rn) {
    // 检查当前节点是否有待下推的更新标记
    if (update[i]) {
        int lc = i << 1;     // 左子节点索引
        int rc = i << 1 | 1; // 右子节点索引

        // 更新左子节点：
        // 1. sum[lc] 更新为懒惰标记的值 * 左子节点区间长度
        sum[lc] = change[i] * ln;
        // 2. 左子节点的懒惰标记值也更新为父节点的懒惰标记值
        change[lc] = change[i];
        // 3. 标记左子节点也有待下推的更新
        update[lc] = true;

        // 更新右子节点（同左子节点）：
        sum[rc] = change[i] * rn;
        change[rc] = change[i];
        update[rc] = true;

        // 当前节点的懒惰标记已经下推，清除标记
        update[i] = false;
    }
}

// 构建线段树函数
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
void build(int l, int r, int i) {
    sum[i] = 0;       // 初始化当前节点的和为0（表示初始时所有位置都没有花）
    update[i] = false; // 初始化懒惰标记为false

    // 如果l < r，说明当前区间不是叶子节点，需要递归构建子树
    if (l < r) {
        int mid = (l + r) >> 1; // 计算中间点
        build(l, mid, i << 1);     // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        // 这里不需要up(i)，因为子节点初始化为0，父节点自然也是0，或者说up操作在更新时才更有意义
    }
}

// 区间更新函数：将jobl到jobr的区间值更新为jobv
// 参数jobl：待更新区间的左边界（用户请求的）
// 参数jobr：待更新区间的右边界（用户请求的）
// 参数jobv：更新的目标值（0或1）
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
void updateTree(int jobl, int jobr, int jobv, int l, int r, int i) {
    // 情况1: 当前节点代表的区间 [l, r] 完全包含在待更新区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        // 直接更新当前节点的和：目标值 * 当前区间长度
        sum[i] = jobv * (r - l + 1);
        // 设置懒惰标记：change值为jobv
        change[i] = jobv;
        // 标记当前节点有待下推的更新
        update[i] = true;
    } else { // 情况2: 当前节点代表的区间与待更新区间有交集，但不是完全包含
        int mid = (l + r) >> 1; // 计算中间点

        // 在递归更新子节点之前，需要先将当前节点的懒惰标记下推给子节点
        // 避免子节点被错误地查询或更新
        down(i, mid - l + 1, r - mid);

        // 如果待更新区间的左边界在左子节点区间内，递归更新左子树
        if (jobl <= mid) {
            updateTree(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果待更新区间的右边界在右子节点区间内，递归更新右子树
        if (jobr > mid) {
            updateTree(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        // 子节点更新完成后，需要向上更新当前节点的值
        up(i);
    }
}

// 区间查询函数：查询jobl到jobr区间的和
// 参数jobl：待查询区间的左边界（用户请求的）
// 参数jobr：待查询区间的右边界（用户请求的）
// 参数l：当前节点代表区间的左边界
// 参数r：当前节点代表区间的右边界
// 参数i：当前节点的索引
int query(int jobl, int jobr, int l, int r, int i) {
    // 情况1: 当前节点代表的区间 [l, r] 完全包含在待查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        // 直接返回当前节点的和
        return sum[i];
    }
    int mid = (l + r) >> 1; // 计算中间点
    // 在递归查询子节点之前，需要先将当前节点的懒惰标记下推给子节点
    down(i, mid - l + 1, r - mid);

    int ans = 0; // 初始化查询结果

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

// 寻找从s开始的第k个0（空位）的函数
// 参数s：搜索的起始位置（1-based index）
// 参数k：要寻找的第k个空位
// 返回值：第k个空位的索引（1-based index），如果找不到则返回0
int findZero(int s, int k) {
    int l = s, r = n, mid; // 定义二分查找的左右边界
    int ans = 0; // 存储找到的答案

    // 二分查找：寻找满足条件的最小位置
    while (l <= r) {
        mid = (l + r) >> 1; // 取中间位置
        // 计算从s到mid区间内的空位数：
        // (mid - s + 1) 是该区间的总长度
        // query(s, mid, 1, n, 1) 是该区间内有花（1）的数量
        // 两者之差就是空位（0）的数量
        int zeros = (mid - s + 1) - query(s, mid, 1, n, 1);

        // 如果找到的空位数大于等于k，说明mid可能是一个解，或者解在mid的左侧
        if (zeros >= k) {
            ans = mid; // 暂存当前的mid作为可能答案
            r = mid - 1; // 继续向左搜索更小的位置
        } else { // 如果空位数小于k，说明mid不够，需要向右搜索
            l = mid + 1;
        }
    }
    return ans; // 返回最终找到的位置
}

// 插入花朵函数
// 参数from：用户指定的起始索引（0-based index）
// 参数flowers：需要插入的花朵数量
// 返回值：一个包含两个整数的vector，表示实际插入花的起始和结束索引（0-based index），
//         如果无法插入任何花，则返回{-1, -1}。
vector<int> insert(int from, int flowers) {
    from++; // 将用户传入的0-based索引转换为1-based索引，因为线段树内部通常使用1-based索引

    int start = 0, end = 0; // 存储实际放置花的起始和结束位置（1-based）
    // 计算从from到n（总长度）区间内有多少个空位（0）
    int zeros = (n - from + 1) - query(from, n, 1, n, 1);

    // 如果从from开始没有任何空位，则无法放置任何花
    if (zeros == 0) {
        start = -1; // 标记为无法放置
        end = -1;
    } else {
        // 找到从from开始的第一个空位的位置
        start = findZero(from, 1);
        // 找到从from开始的第 min(zeros, flowers) 个空位的位置
        // min(zeros, flowers) 确保我们不会尝试放置超过可用空位或用户请求数量的花
        end = findZero(from, min(zeros, flowers));
        // 将从start到end的区间标记为有花（值设为1）
        updateTree(start, end, 1, 1, n, 1);
        // 将1-based索引转换回0-based索引，以便返回给用户
        start--;
        end--;
    }
    return {start, end}; // 返回插入的起始和结束位置
}

// 清理花朵函数
// 参数left：用户指定的清理区间的左边界（0-based index）
// 参数right：用户指定的清理区间的右边界（0-based index）
// 返回值：被清除的花朵总数
int clear(int left, int right) {
    left++;  // 将用户传入的0-based索引转换为1-based索引
    right++; // 同上

    // 查询该区间内当前有多少朵花（值是1的元素数量）
    int ans = query(left, right, 1, n, 1);
    // 将该区间内的所有花清除（值设为0）
    updateTree(left, right, 0, 1, n, 1);
    return ans; // 返回被清除的花朵数量
}

// 主函数
int main() {
    int t, m, op, from, flowers, left, right; // 定义变量：测试用例数t，操作数m，操作类型op，以及操作参数
    scanf("%d", &t); // 读取测试用例数
    while (t--) { // 循环处理每个测试用例
        scanf("%d%d", &n, &m); // 读取花坛长度n和操作数m
        build(1, n, 1); // 构建线段树，初始化所有位置为0（没有花）
        for (int i = 0; i < m; i++) { // 循环处理m个操作
            scanf("%d", &op); // 读取操作类型
            if (op == 1) { // 如果操作类型是1（插入花朵）
                scanf("%d%d", &from, &flowers); // 读取起始位置和花朵数量
                vector<int> ans = insert(from, flowers); // 调用insert函数
                if (ans[0] == -1) { // 如果返回-1，表示无法放置任何花
                    printf("Can not put any one.\n");
                } else { // 否则，打印实际放置的起始和结束位置
                    printf("%d %d\n", ans[0], ans[1]);
                }
            } else { // 如果操作类型是2（清理花朵）
                scanf("%d%d", &left, &right); // 读取清理区间的左右边界
                printf("%d\n", clear(left, right)); // 调用clear函数并打印被清除的花朵数量
            }
        }
        printf("\n"); // 每个测试用例结束后打印一个空行
    }
    return 0; // 程序正常结束
}

