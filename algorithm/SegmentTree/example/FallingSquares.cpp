#include <vector>     // 引入C++标准库中的vector容器，用于动态数组
#include <algorithm>  // 引入C++标准库中的algorithm头文件，提供排序(sort)和最大值(max)等函数



// 掉落的方块
// 有一个二维平面，x轴是最底的边界
// 给定二维整数数组pos，pos[i] = [ lefti, leni ]
// 表示第i个方块边长为leni，左侧边缘在x = lefti位置，所在高度非常高
// 所有方块都是正方形，依次从高处垂直掉落，也就是左边界顺着x = lefti往下
// 如果掉落的方块碰到已经掉落正方形的顶边或者x轴就停止掉落
// 如果方块掉落时仅仅是擦过已经掉落正方形的左侧边或右侧边，并不会停止掉落
// 一旦停止，它就会固定在那里，无法再移动，俄罗斯方块游戏和本题意思一样
// 返回一个整数数组ans ，其中ans[i]表示在第i块方块掉落后整体的最大高度
// 1 <= pos数组长度 <= 1000，1 <= lefti <= 10^8，1 <= leni <= 10^6
// 测试链接 : https://leetcode.cn/problems/falling-squares


// Solution 类，封装了解决“坠落的方块”问题的所有逻辑
class Solution {
public:
    // fallingSquares 函数：主入口，计算每个方块下落后的最大高度
    std::vector<int> fallingSquares(std::vector<std::vector<int>>& positions) {
        // 步骤 1: 坐标离散化。
        // collect 函数会收集所有方块的起始和结束x坐标，并进行排序去重，
        // 从而将巨大的坐标范围压缩到 manageable 的连续整数区间。
        // n 代表离散化后有效坐标点的数量，也是线段树叶子节点的范围。
        int n = collect(positions);

        // 步骤 2: 构建线段树。
        // build 函数初始化线段树的所有节点，将所有区间的高度初始化为0。
        // 参数 (1, n, 1) 表示线段树覆盖的区间范围是离散化后的坐标索引 1 到 n，
        // 根节点索引为 1。
        build(1, n, 1);

        // 存储每次方块下落后的当前最大高度结果
        std::vector<int> ans;
        // maxHeight: 记录到目前为止所有方块堆叠的最高高度
        int maxHeight = 0;
        // l, r: 离散化后当前方块所占据的左右区间索引
        // h: 当前方块落在已有方块上后的高度
        int l, r, h;

        // 遍历每一个坠落的方块
        for (auto& square : positions) {
            // 步骤 3: 坐标映射。
            // rank 函数将原始的方块起始x坐标映射到离散化后的索引 l。
            l = rank(n, square[0]);
            // rank 函数将原始的方块结束x坐标 (square[0] + square[1] - 1) 映射到离散化后的索引 r。
            // 注意：square[1] 是边长，所以结束点是 起始点 + 边长 - 1。
            r = rank(n, square[0] + square[1] - 1);

            // 步骤 4: 查询区间最大高度。
            // query 函数查询当前方块下落区间 [l, r] 内，已堆叠方块的最高高度。
            // 这个高度就是当前方块底部将要接触的高度。
            // 然后加上当前方块的边长 square[1]，就得到了当前方块自身顶部的高度 h。
            h = query(l, r, 1, n, 1) + square[1];

            // 步骤 5: 更新全局最大高度。
            // 更新当前所有方块堆叠中的最高高度。
            maxHeight = std::max(maxHeight, h);
            // 将当前的最大高度添加到结果列表中。
            ans.push_back(maxHeight);

            // 步骤 6: 更新线段树。
            // update 函数将当前方块所占据的区间 [l, r] 的高度更新为 h。
            // 这是一个区间更新操作，会用到延迟标记。
            update(l, r, h, 1, n, 1);
        }

        // 返回所有方块下落后的最高高度列表
        return ans;
    }

private: // 私有成员，供内部函数使用

    // MAXN: 定义线段树和离散化数组的最大容量。
    // 2001 是一个经验值，因为每个方块会产生两个坐标点，假设有1000个方块，
    // 则最多有2000个离散点。MAXN = 2001 足够。
    static const int MAXN = 2001;
    // arr: 用于存储离散化后的唯一x坐标点。
    int arr[MAXN];
    // max: 线段树的核心数组，max[i] 存储节点 i 所代表区间内的最大高度。
    // MAXN << 2 (即 MAXN * 4) 是线段树数组通常需要的空间，因为线段树在最坏情况下是平衡二叉树，
    // 节点数大约是叶子节点数的 2 倍，再加上递归深度，通常取 4 倍。
    int max[MAXN << 2];
    // change: 延迟标记数组，change[i] 存储节点 i 的延迟更新值。
    int change[MAXN << 2];
    // update_flag: 布尔数组，update_flag[i] 为 true 表示节点 i 有待下推的延迟标记。
    bool update_flag[MAXN << 2];

    // collect 函数：收集并离散化坐标点
    int collect(std::vector<std::vector<int>>& positions) {
        // size: arr 数组的当前有效元素数量，从 1 开始计数。
        int size = 1;
        // 遍历所有方块，将它们的起始x坐标和结束x坐标（右边界）添加到 arr 数组中。
        for (auto& pos : positions) {
            arr[size++] = pos[0];             // 方块的起始x坐标
            arr[size++] = pos[0] + pos[1] - 1; // 方块的结束x坐标 (包含)
        }
        // 对 arr 数组进行排序，以便后续去重和二分查找。
        std::sort(arr + 1, arr + size); // 从索引 1 开始排序

        // 去重操作：将排序后的 arr 数组中的重复元素移除。
        // n: 去重后有效坐标点的数量。
        int n = 1;
        for (int i = 2; i < size; i++) {
            // 如果当前元素与上一个不重复的元素不同，则将其保留。
            if (arr[n] != arr[i]) {
                arr[++n] = arr[i]; // 将不重复的元素移到前面
            }
        }
        // 返回离散化后的坐标点数量。
        return n;
    }

    // rank 函数：通过二分查找，将原始坐标值映射到离散化后的索引
    int rank(int n, int v) {
        int ans = 0; // 存储二分查找的结果（索引）
        int l = 1, r = n, m; // l: 左边界，r: 右边界，m: 中间点

        // 标准二分查找（查找第一个大于等于 v 的元素）
        while (l <= r) {
            m = (l + r) >> 1; // 等价于 (l + r) / 2
            if (arr[m] >= v) {
                ans = m;     // 找到一个潜在的答案，继续向左查找更小的索引
                r = m - 1;
            } else {
                l = m + 1;   // 当前值太小，向右查找
            }
        }
        // 返回 v 在离散化数组 arr 中的索引（秩）。
        return ans;
    }

    // up 函数：向上更新操作，用于线段树节点值的维护
    // 当子节点的值发生变化时，父节点的最大值需要更新为子节点中的最大值。
    void up(int i) {
        // max[i] = max(左子节点的最大值, 右子节点的最大值)
        max[i] = std::max(max[i << 1], max[i << 1 | 1]); // i << 1 是 i * 2 (左子)，i << 1 | 1 是 i * 2 + 1 (右子)
    }

    // down 函数：向下推送延迟标记操作
    // 当需要访问某个节点的子节点时（例如进行查询或部分更新），
    // 如果父节点有延迟标记，则需要将这个标记下推给它的子节点。
    void down(int i) {
        // 如果当前节点 i 有延迟标记（update_flag 为 true）
        if (update_flag[i]) {
            // 将当前节点的延迟值 (change[i]) 应用到左子节点
            lazy(i << 1, change[i]);
            // 将当前节点的延迟值 (change[i]) 应用到右子节点
            lazy(i << 1 | 1, change[i]);
            // 下推完成后，清除当前节点的延迟标记
            update_flag[i] = false;
        }
    }

    // lazy 函数：应用延迟标记
    // 这个函数将值 v 应用到节点 i，并设置其延迟标记。
    void lazy(int i, int v) {
        update_flag[i] = true; // 设置延迟标记为 true
        change[i] = v;         // 存储要更新的值 v
        max[i] = v;            // 直接更新当前节点的最大值为 v（因为是覆盖更新）
    }

    // build 函数：构建线段树
    // l: 当前节点代表区间的左边界，r: 当前节点代表区间的右边界，i: 当前节点在线段树数组中的索引
    void build(int l, int r, int i) {
        // 如果当前区间不是叶子节点（即左边界小于右边界）
        if (l < r) {
            int mid = (l + r) >> 1; // 计算中间点
            // 递归构建左子树
            build(l, mid, i << 1);
            // 递归构建右子树
            build(mid + 1, r, i << 1 | 1);
        }
        // 初始化当前节点的最大值为 0
        max[i] = 0;
        // 初始化当前节点的延迟更新值为 0
        change[i] = 0;
        // 初始化当前节点的延迟标记为 false (无待下推更新)
        update_flag[i] = false;
    }

    // update 函数：区间更新操作
    // jobl, jobr: 需要更新的目标区间 [jobl, jobr]
    // jobv: 目标区间内所有点要更新成的值 (新的高度)
    // l, r, i: 当前线段树节点代表的区间 [l, r] 和其索引 i
    void update(int jobl, int jobr, int jobv, int l, int r, int i) {
        // 情况 1: 如果当前节点代表的区间 [l, r] 完全包含在目标区间 [jobl, jobr] 内
        if (jobl <= l && r <= jobr) {
            // 直接对当前节点打上延迟标记
            lazy(i, jobv);
        } else { // 情况 2: 当前节点区间与目标区间有交集，但不是完全包含
            int mid = (l + r) >> 1; // 计算中间点
            down(i); // 在向下递归之前，先将当前节点的延迟标记下推给子节点
            // 如果目标区间与左子树区间有交集
            if (jobl <= mid) {
                update(jobl, jobr, jobv, l, mid, i << 1); // 递归更新左子树
            }
            // 如果目标区间与右子树区间有交集
            if (jobr > mid) {
                update(jobl, jobr, jobv, mid + 1, r, i << 1 | 1); // 递归更新右子树
            }
            up(i); // 左右子树更新完毕后，更新当前节点的最大值 (因为子节点可能已经改变)
        }
    }

    // query 函数：区间查询操作
    // jobl, jobr: 需要查询的目标区间 [jobl, jobr]
    // l, r, i: 当前线段树节点代表的区间 [l, r] 和其索引 i
    int query(int jobl, int jobr, int l, int r, int i) {
        // 情况 1: 如果当前节点代表的区间 [l, r] 完全包含在目标区间 [jobl, jobr] 内
        if (jobl <= l && r <= jobr) {
            return max[i]; // 直接返回当前节点的最大值
        }
        int mid = (l + r) >> 1; // 计算中间点
        down(i); // 在向下递归查询之前，先将当前节点的延迟标记下推给子节点
        int ans = INT_MIN; // 初始化查询结果为最小整数 (确保能正确取到最大值)
        // 如果目标区间与左子树区间有交集
        if (jobl <= mid) {
            ans = std::max(ans, query(jobl, jobr, l, mid, i << 1)); // 递归查询左子树，并取最大值
        }
        // 如果目标区间与右子树区间有交集
        if (jobr > mid) {
            ans = std::max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1)); // 递归查询右子树，并取最大值
        }
        // 返回查询结果，即目标区间内的最大高度
        return ans;
    }
};

