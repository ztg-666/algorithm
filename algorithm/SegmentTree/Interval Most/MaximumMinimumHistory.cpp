#include <cstdio> // 标准输入输出库，用于scanf和printf
#include <algorithm> // 算法库，提供了max等函数
#include <climits> // 限制值库，提供了LLONG_MIN (long long的最小值)

// 区间最值和历史最值
// 给定两个长度都为n的数组A和B，一开始两个数组完全一样
// 任何操作做完，都更新B数组，B[i] = max(B[i],A[i])
// 实现以下五种操作，一共会调用m次
// 操作 1 l r v : A[l..r]范围上每个数加上v
// 操作 2 l r v : A[l..r]范围上每个数A[i]变成min(A[i],v)
// 操作 3 l r   : 返回A[l..r]范围上的累加和
// 操作 4 l r   : 返回A[l..r]范围上的最大值
// 操作 5 l r   : 返回B[l..r]范围上的最大值
// 1 <= n、m <= 5 * 10^5
// 测试链接 : https://www.luogu.com.cn/problem/P6242

using namespace std; // 使用标准命名空间，避免每次写std::

// 定义常量
const int MAXN = 500001; // 数组最大大小，用于定义线段树能处理的元素数量
const long long LOWEST = LLONG_MIN; // 定义一个极小值，用于初始化次大值，表示没有次大值或者某个值无效

// 线段树节点数据存储数组
int arr[MAXN]; // 原始输入数组，arr[i] 存储第 i 个元素的值

// 线段树的每个节点存储的信息，通常需要开四倍大小 (MAXN << 2 等同于 MAXN * 4)
long long sum[MAXN << 2]; // 区间和
long long max_val[MAXN << 2]; // 区间最大值
int cnt[MAXN << 2]; // 区间最大值的个数 (重要！用于区间取min操作)
long long sem[MAXN << 2]; // 区间次大值 (second maximum value, 也非常重要！)
                           // 如果只有一种值，sem[i] 就是 LOWEST

// 懒惰标记 (Lazy Tags)
long long maxAdd[MAXN << 2]; // 针对当前区间最大值累加的懒惰标记
                             // 表示当前节点的最大值需要加上这个值
long long otherAdd[MAXN << 2]; // 针对当前区间非最大值累加的懒惰标记
                               // 表示当前节点的次大值及其他非最大值需要加上这个值

// 历史懒惰标记 (Historical Lazy Tags)
long long maxHistory[MAXN << 2]; // 区间元素历史最大值
                                 // 维护了该区间内所有元素在过去任何时间点出现过的最大值中的最大值
long long maxAddTop[MAXN << 2]; // 针对当前区间最大值历史累加的懒惰标记
                                // 用于更新maxHistory，表示max_val需要额外加的量
long long otherAddTop[MAXN << 2]; // 针对当前区间非最大值历史累加的懒惰标记
                                  // 用于更新maxHistory，表示sem需要额外加的量

/*
    函数名：up
    功能：从子节点向上更新父节点的信息
    参数：
        i: 当前父节点的索引
    作用：
        当子节点的信息发生变化后，需要重新计算父节点的数据，确保信息的正确性。
        这类似于线段树的常规操作，但这里需要特别处理max_val、cnt和sem。
*/
void up(int i) {
    int l = i << 1;     // 左子节点索引
    int r = i << 1 | 1; // 右子节点索引

    // 1. 更新历史最大值：父节点的历史最大值是左右子节点历史最大值的较大者
    maxHistory[i] = std::max(maxHistory[l], maxHistory[r]);

    // 2. 更新区间和：父节点的和是左右子节点和的总和
    sum[i] = sum[l] + sum[r];

    // 3. 更新区间最大值和次大值、计数
    // 如果左子节点的最大值大于右子节点的最大值
    if (max_val[l] > max_val[r]) {
        max_val[i] = max_val[l]; // 父节点的最大值是左子节点的最大值
        cnt[i] = cnt[l];         // 父节点最大值的计数是左子节点最大值的计数
        // 父节点的次大值是左子节点的次大值和右子节点最大值中的较大者
        // 因为右子节点的最大值现在是父节点的次大值候选
        sem[i] = std::max(sem[l], max_val[r]);
    }
    // 如果左子节点的最大值小于右子节点的最大值
    else if (max_val[l] < max_val[r]) {
        max_val[i] = max_val[r]; // 父节点的最大值是右子节点的最大值
        cnt[i] = cnt[r];         // 父节点最大值的计数是右子节点最大值的计数
        // 父节点的次大值是右子节点的次大值和左子节点最大值中的较大者
        // 因为左子节点的最大值现在是父节点的次大值候选
        sem[i] = std::max(max_val[l], sem[r]);
    }
    // 如果左右子节点的最大值相等
    else {
        max_val[i] = max_val[l]; // 父节点的最大值就是其中一个子节点的最大值
        cnt[i] = cnt[l] + cnt[r]; // 父节点最大值的计数是左右子节点最大值计数的总和
        // 父节点的次大值是左右子节点次大值中的较大者
        sem[i] = std::max(sem[l], sem[r]);
    }
    // 如果父节点的次大值和最大值相等，那么实际上没有次大值，将其设为LOWEST
    // 这是为了处理所有元素都相同的情况，避免sem等于max_val导致逻辑错误
    if (sem[i] == max_val[i]) sem[i] = LOWEST;
}

/*
    函数名：lazy
    功能：将懒惰标记应用到当前节点 (即更新当前节点的信息，而不是向下传播)
    参数：
        i: 当前节点的索引
        n: 当前节点所代表区间的长度
        maxAddv: 作用于当前节点最大值的增量
        otherAddv: 作用于当前节点非最大值的增量
        maxUpv: 作用于当前节点最大值，用于更新历史最大值的增量 (当前的最大值+maxUpv是新的历史最大值候选)
        otherUpv: 作用于当前节点非最大值，用于更新历史最大值的增量 (当前的次大值+otherUpv是新的历史最大值候选)
    作用：
        这个函数是 Segment Tree Beats 复杂性的核心之一。
        它不仅更新当前节点的sum, max_val, sem，还更新了当前节点的懒惰标记本身，
        并且最重要的是，更新了 `maxHistory` 和 `maxAddTop`/`otherAddTop`。
*/
void lazy(int i, int n, long long maxAddv, long long otherAddv, long long maxUpv, long long otherUpv) {
    // 1. 更新历史最大值：
    // maxHistory[i] 是当前节点所有元素出现过的最大值的最大值
    // 它可能由：1. 之前的历史最大值 2. 当前最大值加上作用于它的历史增量 3. 当前次大值加上作用于它的历史增量 产生
    // 注意：max_val[i] + maxUpv 是指 "当maxUpv作用时，max_val[i]能达到的历史最大值"
    maxHistory[i] = std::max(maxHistory[i], max_val[i] + maxUpv);

    // 2. 更新历史增量懒惰标记 (top标记)
    // maxAddTop[i] 存储了 maxAdd[i] 的历史最大值，即对 max_val[i] 应用过的最大增量
    maxAddTop[i] = std::max(maxAddTop[i], maxAdd[i] + maxUpv);
    // otherAddTop[i] 存储了 otherAdd[i] 的历史最大值，即对 sem[i] 等非最大值应用过的最大增量
    otherAddTop[i] = std::max(otherAddTop[i], otherAdd[i] + otherUpv);

    // 3. 更新当前值
    // 更新区间和：最大值的部分累加 maxAddv * cnt[i]，非最大值的部分累加 otherAddv * (n - cnt[i])
    sum[i] += maxAddv * cnt[i] + otherAddv * (n - cnt[i]);
    // 更新区间最大值：最大值加上 maxAddv
    max_val[i] += maxAddv;
    // 更新区间次大值：如果存在次大值，次大值加上 otherAddv
    if (sem[i] != LOWEST) {
        sem[i] += otherAddv;
    }

    // 4. 更新当前懒惰标记 (add标记)
    // maxAdd[i] 累加作用于最大值的增量
    maxAdd[i] += maxAddv;
    // otherAdd[i] 累加作用于非最大值的增量
    otherAdd[i] += otherAddv;
}

/*
    函数名：down
    功能：将当前节点的懒惰标记向下传播给子节点
    参数：
        i: 当前父节点的索引
        ln: 左子节点代表的区间长度
        rn: 右子节点代表的区间长度
    作用：
        在访问子节点之前（例如查询或修改时），必须将父节点累积的懒惰标记下推。
        这是 Segment Tree Beats 最复杂的部分之一，因为它需要区分左右子节点哪个是当前的最大值，
        并应用不同的懒惰标记。
*/
void down(int i, int ln, int rn) {
    int l = i << 1;     // 左子节点索引
    int r = i << 1 | 1; // 右子节点索引

    // 确定左右子节点中哪个是父节点的当前最大值 (即继承 maxAdd / maxAddTop)
    long long tmp = std::max(max_val[l], max_val[r]);

    // 如果左子节点的最大值是父节点的当前最大值 (或与右子节点相等)
    if (max_val[l] == tmp) {
        // 左子节点继承父节点的 maxAdd 和 maxAddTop，其他部分继承 otherAdd 和 otherAddTop
        lazy(l, ln, maxAdd[i], otherAdd[i], maxAddTop[i], otherAddTop[i]);
    } else {
        // 左子节点继承父节点的 otherAdd 和 otherAddTop (因为它不是最大值)
        lazy(l, ln, otherAdd[i], otherAdd[i], otherAddTop[i], otherAddTop[i]);
    }

    // 如果右子节点的最大值是父节点的当前最大值 (或与左子节点相等)
    if (max_val[r] == tmp) {
        // 右子节点继承父节点的 maxAdd 和 maxAddTop，其他部分继承 otherAdd 和 otherAddTop
        lazy(r, rn, maxAdd[i], otherAdd[i], maxAddTop[i], otherAddTop[i]);
    } else {
        // 右子节点继承父节点的 otherAdd 和 otherAddTop (因为它不是最大值)
        lazy(r, rn, otherAdd[i], otherAdd[i], otherAddTop[i], otherAddTop[i]);
    }

    // 下推完成后，清空当前父节点的懒惰标记，因为它们已经传递给子节点
    maxAdd[i] = otherAdd[i] = maxAddTop[i] = otherAddTop[i] = 0;
}

/*
    函数名：build
    功能：构建线段树
    参数：
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        递归地构建线段树。叶子节点直接用原始数组的值初始化，非叶子节点则通过up函数合并子节点信息。
*/
void build(int l, int r, int i) {
    // 如果是叶子节点 (区间只包含一个元素)
    if (l == r) {
        sum[i] = max_val[i] = maxHistory[i] = arr[l]; // 和、最大值、历史最大值都等于当前元素值
        sem[i] = LOWEST; // 叶子节点没有次大值
        cnt[i] = 1;      // 叶子节点最大值的计数为1
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        build(l, mid, i << 1);   // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 从子节点更新父节点信息
    }
    // 初始化所有懒惰标记为0，表示当前没有待处理的修改
    maxAdd[i] = otherAdd[i] = maxAddTop[i] = otherAddTop[i] = 0;
}

/*
    函数名：add
    功能：对指定区间进行区间加操作
    参数：
        jobl: 操作区间的左边界
        jobr: 操作区间的右边界
        jobv: 要加上的值
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        标准的线段树区间修改操作。如果当前节点区间完全包含在操作区间内，则直接应用懒惰标记。
        否则，向下传播懒惰标记，并递归处理子节点。
*/
void add(int jobl, int jobr, long long jobv, int l, int r, int i) {
    // 如果当前节点区间 [l, r] 完全包含在操作区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        // 直接对当前节点应用懒惰标记：最大值和非最大值都加jobv，历史最大值也要考虑加上jobv
        lazy(i, r - l + 1, jobv, jobv, jobv, jobv);
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 向下传播懒惰标记，确保子节点数据最新
        // 如果操作区间与左子节点区间有交集
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1); // 递归处理左子树
        }
        // 如果操作区间与右子节点区间有交集
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1); // 递归处理右子树
        }
        up(i); // 从子节点更新父节点信息
    }
}

/*
    函数名：setMin
    功能：对指定区间进行区间取min操作
    参数：
        jobl: 操作区间的左边界
        jobr: 操作区间的右边界
        jobv: 要与元素取最小值的目标值
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        Segment Tree Beats 的核心操作之一。
        如果目标值 jobv >= 当前节点的最大值 max_val[i]，说明取min操作不会改变当前节点的最大值，直接返回。
        如果 jobv > 当前节点的次大值 sem[i] 且 jobv < 当前节点的最大值 max_val[i]，
        则只需要更新当前节点的最大值部分，并修改对应的懒惰标记。
        否则，向下传播懒惰标记并递归处理。
*/
void setMin(int jobl, int jobr, long long jobv, int l, int r, int i) {
    // 剪枝条件1: 如果 jobv 大于或等于当前区间的最大值，那么对当前区间的元素取min(jobv)不会改变它们的最大值。
    // 因此，这个操作对当前节点及其子树是无效的，直接返回。
    if (jobv >= max_val[i]) {
        return;
    }
    // 如果当前节点区间 [l, r] 完全包含在操作区间 [jobl, jobr] 内
    // 并且 jobv 严格大于当前节点的次大值 sem[i]
    // 并且 jobv 严格小于当前节点的最大值 max_val[i]
    // 满足这些条件时，说明 jobv 只会影响到当前区间的最大值，而不会影响到次大值或更小的值。
    // 此时可以直接修改当前节点的最大值，并通过懒惰标记向下传递。
    if (jobl <= l && r <= jobr && sem[i] < jobv) {
        // 对当前节点应用懒惰标记：
        // maxAddv = jobv - max_val[i]：最大值需要减少到 jobv
        // otherAddv = 0：次大值及其他值不受影响，因为 jobv 比它们都大 (或等于 sem[i] 但比 max_val[i] 小)
        // maxUpv = jobv - max_val[i]：历史最大值也要考虑这个差值
        // otherUpv = 0：非最大值的历史最大值不受影响
        lazy(i, r - l + 1, jobv - max_val[i], 0, jobv - max_val[i], 0);
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 向下传播懒惰标记，确保子节点数据最新
        // 递归处理左右子树
        if (jobl <= mid) {
            setMin(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            setMin(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i); // 从子节点更新父节点信息
    }
}

/*
    函数名：querySum
    功能：查询指定区间的元素和
    参数：
        jobl: 查询区间的左边界
        jobr: 查询区间的右边界
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        标准的线段树区间查询操作。如果当前节点区间完全包含在查询区间内，则直接返回其和。
        否则，向下传播懒惰标记，并递归查询子节点，然后合并结果。
*/
long long querySum(int jobl, int jobr, int l, int r, int i) {
    // 如果当前节点区间 [l, r] 完全包含在查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return sum[i]; // 直接返回当前节点的区间和
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 向下传播懒惰标记，确保子节点数据最新
        long long ans = 0; // 初始化答案
        // 如果查询区间与左子节点区间有交集
        if (jobl <= mid) {
            ans += querySum(jobl, jobr, l, mid, i << 1); // 递归查询左子树
        }
        // 如果查询区间与右子节点区间有交集
        if (jobr > mid) {
            ans += querySum(jobl, jobr, mid + 1, r, i << 1 | 1); // 递归查询右子树
        }
        return ans; // 返回合并后的结果
    }
}

/*
    函数名：queryMax
    功能：查询指定区间的当前最大值
    参数：
        jobl: 查询区间的左边界
        jobr: 查询区间的右边界
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        与querySum类似，只是返回的是区间最大值。
*/
long long queryMax(int jobl, int jobr, int l, int r, int i) {
    // 如果当前节点区间 [l, r] 完全包含在查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return max_val[i]; // 直接返回当前节点的区间最大值
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 向下传播懒惰标记，确保子节点数据最新
        long long ans = LOWEST; // 初始化答案为极小值
        // 如果查询区间与左子节点区间有交集
        if (jobl <= mid) {
            ans = std::max(ans, queryMax(jobl, jobr, l, mid, i << 1)); // 递归查询左子树，取最大值
        }
        // 如果查询区间与右子节点区间有交集
        if (jobr > mid) {
            ans = std::max(ans, queryMax(jobl, jobr, mid + 1, r, i << 1 | 1)); // 递归查询右子树，取最大值
        }
        return ans; // 返回合并后的结果
    }
}

/*
    函数名：queryHistoryMax
    功能：查询指定区间的历史最大值
    参数：
        jobl: 查询区间的左边界
        jobr: 查询区间的右边界
        l: 当前节点代表的区间左边界
        r: 当前节点代表的区间右边界
        i: 当前节点的索引
    作用：
        与querySum类似，只是返回的是区间历史最大值。这个值是所有元素在所有操作之后，
        它们曾经达到过的最大值中的最大值。
*/
long long queryHistoryMax(int jobl, int jobr, int l, int r, int i) {
    // 如果当前节点区间 [l, r] 完全包含在查询区间 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return maxHistory[i]; // 直接返回当前节点的区间历史最大值
    } else {
        int mid = (l + r) >> 1; // 计算中间点
        down(i, mid - l + 1, r - mid); // 向下传播懒惰标记，确保子节点数据最新
        long long ans = LOWEST; // 初始化答案为极小值
        // 如果查询区间与左子节点区间有交集
        if (jobl <= mid) {
            ans = std::max(ans, queryHistoryMax(jobl, jobr, l, mid, i << 1)); // 递归查询左子树，取最大值
        }
        // 如果查询区间与右子节点区间有交集
        if (jobr > mid) {
            ans = std::max(ans, queryHistoryMax(jobl, jobr, mid + 1, r, i << 1 | 1)); // 递归查询右子树，取最大值
        }
        return ans; // 返回合并后的结果
    }
}

/*
    函数名：main
    功能：程序入口，处理输入和输出
    作用：
        读取数组大小N和操作次数M。
        读取初始数组元素并构建线段树。
        循环处理M次操作，根据操作类型调用不同的函数，并打印结果。
*/
int main() {
    int n, m; // n: 数组元素个数, m: 操作次数
    scanf("%d%d", &n, &m); // 读取N和M

    // 读取初始数组元素
    for (int i = 1; i <= n; i++) {
        scanf("%d", &arr[i]);
    }

    build(1, n, 1); // 构建线段树，根节点索引为1，表示区间[1, n]

    int op, jobl, jobr; // op: 操作类型, jobl: 操作左边界, jobr: 操作右边界
    long long jobv;     // jobv: 操作值 (对于加法和取min操作)

    // 循环处理M次操作
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &op, &jobl, &jobr); // 读取操作类型和区间边界
        if (op == 1) { // 操作类型1: 区间加法
            scanf("%lld", &jobv); // 读取要加的值
            add(jobl, jobr, jobv, 1, n, 1); // 调用add函数
        } else if (op == 2) { // 操作类型2: 区间取最小值
            scanf("%lld", &jobv); // 读取要取min的值
            setMin(jobl, jobr, jobv, 1, n, 1); // 调用setMin函数
        } else if (op == 3) { // 操作类型3: 查询区间和
            printf("%lld\n", querySum(jobl, jobr, 1, n, 1)); // 调用querySum并打印结果
        } else if (op == 4) { // 操作类型4: 查询区间当前最大值
            printf("%lld\n", queryMax(jobl, jobr, 1, n, 1)); // 调用queryMax并打印结果
        } else if (op == 5) { // 操作类型5: 查询区间历史最大值
            printf("%lld\n", queryHistoryMax(jobl, jobr, 1, n, 1)); // 调用queryHistoryMax并打印结果
        }
    }
    return 0; // 程序正常退出
}
