#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// 动态开点线段树
// 一共有n个位置，编号从1~n，一开始所有位置的值为0
// 实现如下两个操作，一共会调用m次
// 操作 1 l r v : 把l~r范围的每个数增加v
// 操作 2 l r   : 返回l~r范围的累加和
// 1 <= n <= 10^9
// 1 <= m <= 10^3
// 测试链接 : https://www.luogu.com.cn/problem/P2781

class DynamicSegmentTree {
private:
    // 空间限制：范围1~10^9，线段树高度约30层
    // 查询次数1000，每次查询都有左右两条边线
    // 所以空间占用约1000 * 30 * 2 = 60000，适当调大到80001
    static const int LIMIT = 80001;

    int cnt;                           // 当前已分配的节点数量，从1开始
    vector<int> left, right;           // left[i]和right[i]分别表示节点i的左右子节点编号
    vector<long long> sum, add;        // sum[i]表示节点i管辖范围的总和，add[i]表示懒标记值

    /**
     * 向上更新：用左右子节点的sum值更新当前节点的sum值
     * @param h 当前节点编号
     * @param l 左子节点编号
     * @param r 右子节点编号
     */
    void up(int h, int l, int r) {
        sum[h] = sum[l] + sum[r];
    }

    /**
     * 向下传递懒标记：将当前节点的懒标记传递给左右子节点
     * @param i 当前节点编号
     * @param ln 左子节点管辖的区间长度
     * @param rn 右子节点管辖的区间长度
     */
    void down(int i, int ln, int rn) {
        if (add[i] != 0) {  // 如果当前节点有懒标记需要下发
            // 懒更新任务下发前，左右两侧的空间需要准备好
            // 动态开点：只有在需要访问子节点时才创建
            if (left[i] == 0) {
                left[i] = ++cnt;  // 为左子节点分配新的编号
            }
            if (right[i] == 0) {
                right[i] = ++cnt;  // 为右子节点分配新的编号
            }
            // 将懒标记传递给左右子节点
            lazy(left[i], add[i], ln);
            lazy(right[i], add[i], rn);
            add[i] = 0;  // 清空当前节点的懒标记
        }
    }

    /**
     * 给指定节点添加懒标记
     * @param i 节点编号
     * @param v 要增加的值
     * @param n 该节点管辖的区间长度
     */
    void lazy(int i, long long v, int n) {
        sum[i] += v * n;  // 区间和增加 v * 区间长度
        add[i] += v;      // 懒标记累加
    }

public:
    /**
     * 构造函数：初始化所有数组，cnt从1开始（0号位置不使用）
     */
    DynamicSegmentTree() : cnt(1) {
        left.resize(LIMIT, 0);
        right.resize(LIMIT, 0);
        sum.resize(LIMIT, 0);
        add.resize(LIMIT, 0);
    }

    /**
     * 区间更新：将[jobl, jobr]范围内的所有数增加jobv
     * @param jobl 更新任务的左边界
     * @param jobr 更新任务的右边界
     * @param jobv 要增加的值
     * @param l 当前节点管辖范围的左边界
     * @param r 当前节点管辖范围的右边界
     * @param i 当前节点编号
     */
    void update(int jobl, int jobr, long long jobv, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前节点完全被更新任务覆盖，直接加懒标记
            lazy(i, jobv, r - l + 1);
        } else {
            // 当前节点部分被更新任务覆盖，需要继续向下递归
            int mid = (l + r) >> 1;  // 计算中点，等价于 (l + r) / 2
            down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

            if (jobl <= mid) {
                // 更新任务与左半部分有交集，需要访问左子节点
                if (left[i] == 0) {
                    left[i] = ++cnt;  // 动态开点：不得不去左侧才会申请空间
                }
                update(jobl, jobr, jobv, l, mid, left[i]);
            }
            if (jobr > mid) {
                // 更新任务与右半部分有交集，需要访问右子节点
                if (right[i] == 0) {
                    right[i] = ++cnt;  // 动态开点：不得不去右侧才会申请空间
                }
                update(jobl, jobr, jobv, mid + 1, r, right[i]);
            }
            // 用子节点信息更新当前节点
            up(i, left[i], right[i]);
        }
    }

    /**
     * 区间查询：返回[jobl, jobr]范围内所有数的累加和
     * @param jobl 查询任务的左边界
     * @param jobr 查询任务的右边界
     * @param l 当前节点管辖范围的左边界
     * @param r 当前节点管辖范围的右边界
     * @param i 当前节点编号
     * @return 查询范围内的累加和
     */
    long long query(int jobl, int jobr, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前节点完全被查询任务覆盖，直接返回该节点的sum值
            return sum[i];
        }

        int mid = (l + r) >> 1;  // 计算中点
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记
        long long ans = 0;

        if (jobl <= mid) {
            // 查询任务与左半部分有交集
            if (left[i] != 0) {
                // 只有左侧申请过空间才有必要去查询
                // 如果左侧从来没有申请过空间，说明该部分从未被更新过，查询结果就是0
                ans += query(jobl, jobr, l, mid, left[i]);
            }
        }
        if (jobr > mid) {
            // 查询任务与右半部分有交集
            if (right[i] != 0) {
                // 只有右侧申请过空间才有必要去查询
                // 如果右侧从来没有申请过空间，说明该部分从未被更新过，查询结果就是0
                ans += query(jobl, jobr, mid + 1, r, right[i]);
            }
        }
        return ans;
    }

    /**
     * 清空所有数据，用于多组测试数据的情况
     * 如果一次会执行多组测试数据，那么每组测试完成后要clear空间
     */
    void clear() {
        // 只清空已使用的部分，提高效率
        fill(left.begin() + 1, left.begin() + cnt + 1, 0);
        fill(right.begin() + 1, right.begin() + cnt + 1, 0);
        fill(sum.begin() + 1, sum.begin() + cnt + 1, 0);
        fill(add.begin() + 1, add.begin() + cnt + 1, 0);
        cnt = 1;  // 重置节点计数器
    }
};

// 动态开点线段树
// 一共有n个位置，编号从1~n，一开始所有位置的值为0
// 实现如下两个操作，一共会调用m次
// 操作 1 l r v : 把l~r范围的每个数增加v
// 操作 2 l r   : 返回l~r范围的累加和
// 1 <= n <= 10^9
// 1 <= m <= 10^3
// 测试链接 : https://www.luogu.com.cn/problem/P2781

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);  // 关闭C++与C的同步，提高cin/cout速度
    cin.tie(nullptr);             // 解除cin与cout的绑定，进一步提高速度

    int n, m;  // n：位置范围1~n，m：操作次数
    cin >> n >> m;

    DynamicSegmentTree dst;  // 创建动态开点线段树对象

    // 处理m个操作
    for (int i = 0; i < m; i++) {
        int op;  // 操作类型
        cin >> op;

        if (op == 1) {
            // 操作1：区间更新 - 将[jobl, jobr]范围内的每个数增加jobv
            int jobl, jobr;      // 更新范围的左右边界
            long long jobv;      // 要增加的值
            cin >> jobl >> jobr >> jobv;
            dst.update(jobl, jobr, jobv, 1, n, 1);  // 从根节点开始更新
        } else {
            // 操作2：区间查询 - 返回[jobl, jobr]范围内所有数的累加和
            int jobl, jobr;      // 查询范围的左右边界
            cin >> jobl >> jobr;
            cout << dst.query(jobl, jobr, 1, n, 1) << "\n";  // 从根节点开始查询并输出结果
        }
    }

    // 清空数据结构（本题每组测试数据都单独运行，可以不写clear方法）
    // 但是如果多组测试数据串行调用，就需要加上清空逻辑
    dst.clear();

    return 0;
}
