#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>

// 贴海报(数据加强版)
// 有一面墙，有固定高度，无限的宽度，有n张海报，所有海报的高度都和墙的高度相同
// 从第1张海报开始，一张一张往墙上贴，直到n张海报贴完
// 每张海报都给出张贴位置(xi, yi)，表示第i张海报从墙的左边界xi一直延伸到右边界yi
// 有可能发生后面的海报把前面的海报完全覆盖，导致看不到的情况
// 当所有海报贴完，返回能看到海报的数量，哪怕只漏出一点的海报都算
// 1 <= n <= 10^5，1 <= xi、yi <= 10^7

using namespace std;

// 定义最大海报数量的常量，为数组预留足够空间。
// MAXN = 10001, 意味着最多可以处理10000张海报。
const int MAXN = 10001;

// ====== 全局数据结构定义 ======
// pl[i] 和 pr[i] 分别存储第 i 张海报的左、右端点坐标。
int pl[MAXN];
int pr[MAXN];
// num[] 数组用于坐标离散化。它会收集所有海报的左右端点，并插入间隙点。
// 因为每张海报有2个端点，所以空间大小是MAXN的2倍，再加上间隙点，所以大小翻倍。
int num[MAXN << 2]; // MAXN * 4
// poster[] 数组是线段树的核心！poster[i] 存储的是覆盖线段树节点 i 所代表区间的海报ID。
// 这里的节点数需要4倍于离散化后的坐标点数，以确保线段树的构建空间足够。
int poster[MAXN << 4]; // MAXN * 16，为线段树预留足够的空间
// visited[] 数组用于查询阶段，标记某个海报ID是否已经被计入可见海报总数。
// 避免重复计数同一张海报。
bool visited[MAXN];

// ====== 核心函数：坐标离散化与间隙点插入 ======
/**
 * @brief 离散化坐标点并插入间隙点，以保证线段树区间的连续性。
 * @param m 收集到的原始坐标点总数。
 * @return 离散化并处理间隙点后，新的有效坐标点总数。
 */
int collect(int m) {
    // 步骤1：对所有收集到的坐标点进行排序。
    sort(num + 1, num + m + 1);
    
    // 步骤2：去重，只保留唯一的坐标点。
    int size = 1; // size 指向当前不重复坐标的最后一个位置
    for (int i = 2; i <= m; i++) {
        if (num[size] != num[i]) {
            num[++size] = num[i];
        }
    }

    // 步骤3：这是坐标离散化的精髓！😎 插入间隙点（'midpoints'）。
    // 比如，原始坐标有1和5，如果直接离散化，1和5之间就会有一个大的“跳跃”。
    // 我们在1和5之间插入一个虚拟的坐标点2，使得原坐标1和5之间的小区间[1, 5]
    // 能够在线段树中被有效表示为[1, 2]和[5, 5]两个区间，从而不遗漏任何缝隙。
    int cnt = size; // cnt 记录处理后的坐标总数
    for (int i = 2; i <= size; i++) {
        // 如果相邻的两个坐标点之间有大于1的间隙...
        if (num[i-1] + 1 < num[i]) {
            // ...就插入一个间隙点，这个点代表了num[i-1]和num[i]之间的空隙。
            num[++cnt] = num[i-1] + 1;
        }
    }
    
    // 步骤4：对包含间隙点的新坐标集合再次排序，得到最终的离散化坐标序列。
    sort(num + 1, num + cnt + 1);
    
    return cnt; // 返回离散化后的总坐标数，这是线段树的叶子节点数量。
}

// ====== 核心函数：二分查找离散化后的位置 ======
/**
 * @brief 在离散化后的坐标数组中，通过二分查找找到给定原始坐标值v对应的离散化索引。
 * @param l 查找范围的左边界。
 * @param r 查找范围的右边界。
 * @param v 要查找的原始坐标值。
 * @return 坐标值v在离散化数组num中的索引（1-based）。
 */
int rak(int l, int r, int v) {
    int m, ans = 0;
    while (l <= r) {
        m = (l + r) >> 1; // 等价于 (l + r) / 2
        if (num[m] >= v) {
            ans = m; // 找到一个可能的位置，但继续向左找更小的可能
            r = m - 1;
        } else {
            l = m + 1; // 太小了，向右找
        }
    }
    return ans;
}

// ====== 核心函数：线段树的懒惰下传（Lazy Propagation） ======
/**
 * @brief 将父节点的延迟标记（poster ID）下传给其子节点。
 * @param i 当前节点的索引。
 */
void down(int i) {
    // 检查当前节点是否有延迟标记（即是否有海报覆盖整个区间）。
    // poster[i] != 0 表示有海报覆盖。
    if (poster[i] != 0) {
        // 将父节点的海报ID传递给左子节点。
        poster[i << 1] = poster[i]; 
        // 将父节点的海报ID传递给右子节点。
        poster[i << 1 | 1] = poster[i];
        // 传递完毕后，清空父节点的延迟标记。
        poster[i] = 0;
    }
}

// ====== 核心函数：构建线段树（初始化） ======
/**
 * @brief 递归地构建线段树，并初始化所有节点的poster ID为0（表示无海报覆盖）。
 * @param l 当前节点代表区间的左边界（离散化索引）。
 * @param r 当前节点代表区间的右边界（离散化索引）。
 * @param i 当前节点在poster数组中的索引。
 */
void build(int l, int r, int i) {
    // 初始化当前节点的poster ID为0，表示该区间目前没有被任何海报覆盖。
    poster[i] = 0;
    // 如果不是叶子节点...
    if (l < r) {
        int mid = (l + r) >> 1;
        // 递归构建左子树。
        build(l, mid, i << 1);
        // 递归构建右子树。
        build(mid + 1, r, i << 1 | 1);
    }
}

// ====== 核心函数：更新线段树（区间覆盖操作）======
/**
 * @brief 对线段树进行区间更新操作，用jobv海报覆盖从jobl到jobr的区间。
 * 使用了延迟标记（Lazy Propagation）来提高效率。
 * @param jobl 要覆盖区间的左边界（离散化索引）。
 * @param jobr 要覆盖区间的右边界（离散化索引）。
 * @param jobv 海报的ID。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点在poster数组中的索引。
 */
void update(int jobl, int jobr, int jobv, int l, int r, int i) {
    // 场景1：如果当前节点代表的区间 [l, r] 完全被要更新的区间 [jobl, jobr] 包含。
    // 这就是懒惰标记发挥作用的地方！我们不需要继续递归到叶子节点。
    if (jobl <= l && r <= jobr) {
        // 直接在这个节点上标记海报ID，表示整个区间都被这层海报覆盖了。
        poster[i] = jobv;
    } else {
        // 场景2：当前节点代表的区间与更新区间有重叠，但不是完全包含。
        // 在进行递归更新前，先将父节点的延迟标记下传给子节点，确保信息同步。
        down(i);
        int mid = (l + r) >> 1;
        // 如果左子区与更新区间有交集，则递归更新左子树。
        if (jobl <= mid) {
            update(jobl, jobr, jobv, l, mid, i << 1);
        }
        // 如果右子区与更新区间有交集，则递归更新右子树。
        if (jobr > mid) {
            update(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
    }
}

// ====== 核心函数：查询可见海报数量 ======
/**
 * @brief 遍历线段树，统计最终可见的海报数量。
 * @param l 当前节点代表区间的左边界。
 * @param r 当前节点代表区间的右边界。
 * @param i 当前节点在poster数组中的索引。
 * @return 当前子树中可见海报的数量。
 */
int query(int l, int r, int i) {
    // 场景1：到达线段树的叶子节点。
    if (l == r) {
        // 检查这个叶子节点是否被某张海报覆盖（poster[i] != 0）。
        // 并且这张海报的ID之前没有被访问过（!visited[poster[i]]）。
        if (poster[i] != 0 && !visited[poster[i]]) {
            // 如果是，说明我们找到了一个新的、可见的海报ID。
            // 标记这张海报为已访问，并返回1，表示找到了一张新的可见海报。
            visited[poster[i]] = true;
            return 1;
        }
        // 如果没有海报覆盖或者海报ID已访问过，则返回0。
        return 0;
    } else {
        // 场景2：不是叶子节点。
        // 在向下递归查询之前，先将当前节点的延迟标记下传。
        // 这一步至关重要，因为只有下传后，叶子节点才能获得正确的海报ID信息。
        down(i);
        int mid = (l + r) >> 1;
        int ans = 0;
        // 递归查询左子树。
        ans += query(l, mid, i << 1);
        // 递归查询右子树。
        ans += query(mid + 1, r, i << 1 | 1);
        return ans; // 返回左右子树可见海报的总和。
    }
}

// ====== 主函数：程序入口与流程控制 ======
int main() {
    int cases, n;
    // 读取测试用例的数量。
    scanf("%d", &cases);
    // 循环处理每个测试用例。
    for (int t = 1; t <= cases; t++) {
        // 读取海报的总数量。
        scanf("%d", &n);
        int m = 0; // m 用于统计收集到的坐标点总数。
        // 循环读取每一张海报的左右端点坐标。
        for (int i = 1; i <= n; i++) {
            scanf("%d%d", &pl[i], &pr[i]);
            // 将海报的左右端点收集到num数组中，用于后续的坐标离散化。
            num[++m] = pl[i];
            num[++m] = pr[i];
        }
        
        // 步骤1：进行坐标离散化，获取离散化后的坐标总数。
        m = collect(m);
        // 步骤2：根据离散化后的坐标总数，构建并初始化线段树。
        build(1, m, 1);
        
        // 步骤3：按照海报的粘贴顺序，依次更新线段树。
        // i 从 1 到 n，表示海报的粘贴顺序。后贴的海报会覆盖先贴的。
        for (int i = 1; i <= n; i++) {
            // 将原始坐标 pl[i] 和 pr[i] 映射到离散化后的索引。
            int jobl = rak(1, m, pl[i]);
            int jobr = rak(1, m, pr[i]);
            // 使用第 i 张海报的ID，覆盖对应的离散化区间。
            // 这里的 i 就是海报的ID，用它来标记区间。
            update(jobl, jobr, i, 1, m, 1);
        }
        
        // 步骤4：查询可见的海报总数，并打印结果。
        printf("%d\n", query(1, m, 1));
        // 步骤5：重置visited数组，为下一个测试用例做准备。
        // 这一步很重要，否则不同测试用例的结果会互相影响。
        for (int i = 1; i <= n; i++) {
            visited[i] = false;
        }
    }
    return 0;
}
