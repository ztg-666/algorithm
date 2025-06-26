#include <iostream>   // 用于输入输出，例如 std::cout
#include <vector>     // 虽然这里用的是C风格数组，但std::vector是C++中更现代的选择
#include <algorithm>  // 用于 std::max 函数
#include <climits>    // 用于 LLONG_MIN (long long 的最小值)
#include <cstdlib>    // 用于 rand() 和 srand() 函数
#include <ctime>      // 用于 time() 函数，为随机数生成器提供种子

// ===== 线段树核心功能实现 =====
// 线段树支持范围增加、范围查询最大值
// 维护最大值，并使用懒惰标记（Lazy Propagation）优化范围操作
// 本代码通过对数器（暴力验证法）来检查线段树的正确性，这在调试复杂数据结构时非常有用！👍

// 定义最大数组大小常量
const int MAXN = 100001;

// 原始数组，因为线段树一般从1开始索引，所以这里也定义为MAXN大小，实际用到下标1到n
long long arr[MAXN];

// 线段树的节点数组，用于存储区间最大值。
// 一个大小为N的线段树通常需要大约4N的空间。MAXN << 2 等同于 MAXN * 4。
long long max_val[MAXN << 2];

// 懒惰标记数组，用于存储每个区间需要下推的增量值。
// 同样需要大约4N的空间。
long long add_lazy[MAXN << 2];

/**
 * @brief 向上更新节点信息：将子节点的最大值合并到父节点。
 * @param i 当前节点的索引。
 */
void up(int i) {
    // 父节点的最大值是其左右子节点最大值中的较大者。
    max_val[i] = std::max(max_val[i << 1], max_val[i << 1 | 1]);
}

/**
 * @brief 应用懒惰标记：将值v加到当前节点的最大值和懒惰标记上。
 * @param i 当前节点的索引。
 * @param v 需要增加的值。
 */
void lazy(int i, long long v) {
    max_val[i] += v;   // 更新当前节点代表区间内的最大值
    add_lazy[i] += v;  // 更新当前节点的懒惰标记
}

/**
 * @brief 向下推送懒惰标记：将当前节点的懒惰标记传递给子节点。
 * @param i 当前节点的索引。
 */
void down(int i) {
    // 如果当前节点有懒惰标记（即add_lazy[i]不为0）
    if (add_lazy[i] != 0) {
        // 将懒惰标记应用到左子节点 (i << 1)
        lazy(i << 1, add_lazy[i]);
        // 将懒惰标记应用到右子节点 (i << 1 | 1)
        lazy(i << 1 | 1, add_lazy[i]);
        // 清除当前节点的懒惰标记，因为它已经下推完成
        add_lazy[i] = 0;
    }
}

/**
 * @brief 构建线段树。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点在max_val和add_lazy数组中的索引。
 */
void build(int l, int r, int i) {
    if (l == r) { // 如果是叶子节点 (区间只有一个元素)
        max_val[i] = arr[l]; // 直接从原始数组中取值
    } else { // 如果是内部节点
        int mid = (l + r) >> 1; // 计算中点
        build(l, mid, i << 1);     // 递归构建左子树
        build(mid + 1, r, i << 1 | 1); // 递归构建右子树
        up(i); // 左右子树构建完成后，更新当前节点的最大值
    }
    add_lazy[i] = 0; // 初始化懒惰标记为0
}

/**
 * @brief 对指定范围 [jobl, jobr] 执行增加操作。
 * @param jobl 操作范围的左边界。
 * @param jobr 操作范围的右边界。
 * @param jobv 需要增加的值。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点在max_val和add_lazy数组中的索引。
 */
void add(int jobl, int jobr, long long jobv, int l, int r, int i) {
    // 如果当前节点代表的区间 [l, r] 完全包含在操作范围 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        lazy(i, jobv); // 直接对当前节点应用懒惰标记
    } else { // 当前节点代表的区间与操作范围有交集，或部分包含
        down(i); // 在向下递归之前，先将当前节点的懒惰标记下推
        int mid = (l + r) >> 1; // 计算中点
        // 如果左子区间与操作范围有交集
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果右子区间与操作范围有交集
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
        up(i); // 递归操作完成后，更新当前节点的最大值
    }
}

/**
 * @brief 查询指定范围 [jobl, jobr] 内的最大值。
 * @param jobl 查询范围的左边界。
 * @param jobr 查询范围的右边界。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点在max_val和add_lazy数组中的索引。
 * @return 查询范围内的最大值。
 */
long long query(int jobl, int jobr, int l, int r, int i) {
    // 如果当前节点代表的区间 [l, r] 完全包含在查询范围 [jobl, jobr] 内
    if (jobl <= l && r <= jobr) {
        return max_val[i]; // 直接返回当前节点的最大值
    }
    down(i); // 在向下递归之前，先将当前节点的懒惰标记下推
    int mid = (l + r) >> 1; // 计算中点
    long long ans = LLONG_MIN; // 初始化最大值为long long的最小值
    // 如果左子区间与查询范围有交集
    if (jobl <= mid) {
        ans = std::max(ans, query(jobl, jobr, l, mid, i << 1));
    }
    // 如果右子区间与查询范围有交集
    if (jobr > mid) {
        ans = std::max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1));
    }
    return ans; // 返回左右子区间查询结果的较大值
}

// ===== 对数器（暴力验证）逻辑 =====
// 这些函数用于与线段树的实现进行对比，确保线段树的正确性。
// 在实际项目中，这样的对数器是调试复杂算法的利器！🛠️

/**
 * @brief 生成随机值填入原始数组arr。
 * @param n 数组的有效长度（1到n）。
 * @param v 随机值的最大绝对范围（生成的值在[0, v)之间）。
 */
void randomArray(int n, int v) {
    for (int i = 1; i <= n; i++) {
        // 生成 [0, v-1] 之间的随机数
        arr[i] = static_cast<long long>(rand()) % v;
    }
}

/**
 * @brief 暴力方式对验证数组check的指定范围 [jobl, jobr] 执行增加操作。
 * @param check 验证数组。
 * @param jobl 操作范围的左边界。
 * @param jobr 操作范围的右边界。
 * @param jobv 需要增加的值。
 */
void checkAdd(long long check[], int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] += jobv;
    }
}

/**
 * @brief 暴力方式查询验证数组check的指定范围 [jobl, jobr] 内的最大值。
 * @param check 验证数组。
 * @param jobl 查询范围的左边界。
 * @param jobr 查询范围的右边界。
 * @return 查询范围内的最大值。
 */
long long checkQuery(long long check[], int jobl, int jobr) {
    long long ans = LLONG_MIN;
    for (int i = jobl; i <= jobr; i++) {
        ans = std::max(ans, check[i]);
    }
    return ans;
}

// ===== 主函数：对数器测试入口 =====
int main() {
    // 使用当前时间作为随机数种子，确保每次运行生成不同的随机序列
    srand(static_cast<unsigned int>(time(0)));

    std::cout << " " << std::endl;
    int n = 1000;      // 原始数组的有效长度
    int v = 2000;      // 随机数值的范围
    int t = 5000000;   // 操作的总次数

    // 1. 生成初始随机数组
    randomArray(n, v);
    // 2. 构建线段树
    build(1, n, 1);

    // 3. 创建一个用于暴力验证的数组check，并复制初始数据
    // 注意：这里使用动态分配，因为它可能是一个很大的数组
    long long* check = new long long[n + 1]; // 1-indexed
    for (int i = 1; i <= n; i++) {
        check[i] = arr[i];
    }

    // 4. 执行大量随机操作并进行验证
    for (int i = 1; i <= t; i++) {
        // 随机选择操作类型：0为增加，1为查询
        int op = rand() % 2;

        // 随机生成操作范围的左右边界 (1到n)
        int a = (rand() % n) + 1;
        int b = (rand() % n) + 1;
        int jobl = std::min(a, b); // 确保 jobl <= jobr
        int jobr = std::max(a, b);

        if (op == 0) { // 增加操作
            // 生成随机的增加值，范围在 [-v, v] 之间
            // rand() % (2*v + 1) 生成 [0, 2v]，然后减去v得到 [-v, v]
            long long jobv = (static_cast<long long>(rand()) % (2 * v + 1)) - v;

            add(jobl, jobr, jobv, 1, n, 1);     // 线段树执行增加操作
            checkAdd(check, jobl, jobr, jobv);  // 暴力验证数组执行增加操作
        } else { // 查询操作
            long long ans1 = query(jobl, jobr, 1, n, 1);     // 线段树查询结果
            long long ans2 = checkQuery(check, jobl, jobr);  // 暴力验证查询结果

            // 对比两者结果，如果不一致则表示线段树有误
            if (ans1 != ans2) {
                std::cout << "!" << std::endl;
                // 为了调试，可以打印出出错的范围和结果
                // std::cout << "操作范围: [" << jobl << ", " << jobr << "]" << std::endl;
                // std::cout << "线段树结果: " << ans1 << std::endl;
                // std::cout << "暴力验证结果: " << ans2 << std::endl;
                break; // 发现错误立即停止测试
            }
        }
    }

    // 释放动态分配的内存，这是C++的好习惯！🧹
    delete[] check;

    std::cout << " " << std::endl;

    return 0; // 程序成功执行
}

