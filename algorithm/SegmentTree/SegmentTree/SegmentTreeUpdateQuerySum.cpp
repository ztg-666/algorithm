#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

/**
 * 线段树支持范围重置、范围查询
 * 维护累加和
 * 对数器验证
 * 当你写线段树出错了，就需要用对数器的方式来排查
 * 所以本题选择对数器验证，来展示一下怎么写测试
 */
class SegmentTreeUpdateQuerySum {
private:
    static const int MAXN = 100001;  // 最大数组大小

    std::vector<long long> arr;      // 原始数组
    std::vector<long long> sum;      // 线段树数组，存储区间和
    std::vector<long long> change;   // 懒标记数组，存储待更新的值
    std::vector<bool> update;        // 懒标记标志数组，标记是否有更新操作

public:
    /**
     * 构造函数，初始化各个数组
     * sum数组大小为4*MAXN，因为线段树需要4倍空间
     */
    SegmentTreeUpdateQuerySum() : arr(MAXN), sum(MAXN << 2), change(MAXN << 2), update(MAXN << 2) {}

    /**
     * 向上更新节点值
     * 父节点的值等于左右子节点值的和
     * @param i 当前节点编号
     */
    void up(int i) {
        sum[i] = sum[i << 1] + sum[i << 1 | 1];  // i<<1是左子节点，i<<1|1是右子节点
    }

    /**
     * 向下传递懒标记
     * 将父节点的懒标记传递给子节点
     * @param i 当前节点编号
     * @param ln 左子树包含的元素个数
     * @param rn 右子树包含的元素个数
     */
    void down(int i, int ln, int rn) {
        if (update[i]) {  // 如果当前节点有懒标记
            lazy(i << 1, change[i], ln);      // 将懒标记传递给左子节点
            lazy(i << 1 | 1, change[i], rn);  // 将懒标记传递给右子节点
            update[i] = false;  // 清除当前节点的懒标记
        }
    }

    /**
     * 给节点添加懒标记
     * @param i 节点编号
     * @param v 要设置的值
     * @param n 该节点代表的区间包含的元素个数
     */
    void lazy(int i, long long v, int n) {
        sum[i] = v * n;      // 区间和 = 值 * 元素个数
        change[i] = v;       // 记录懒标记值
        update[i] = true;    // 标记该节点有懒标记
    }

    /**
     * 构建线段树
     * @param l 区间左边界
     * @param r 区间右边界
     * @param i 当前节点编号
     */
    void build(int l, int r, int i) {
        if (l == r) {
            // 叶子节点，直接赋值
            sum[i] = arr[l];
        } else {
            // 非叶子节点，递归构建左右子树
            int mid = (l + r) >> 1;  // 中点位置
            build(l, mid, i << 1);          // 构建左子树
            build(mid + 1, r, i << 1 | 1);  // 构建右子树
            up(i);  // 向上更新当前节点的值
        }
        // 初始化懒标记
        change[i] = 0;
        update[i] = false;
    }

    /**
     * 区间更新操作
     * 将区间[jobl, jobr]的所有元素设置为jobv
     * @param jobl 目标区间左边界
     * @param jobr 目标区间右边界
     * @param jobv 要设置的值
     * @param l 当前节点代表的区间左边界
     * @param r 当前节点代表的区间右边界
     * @param i 当前节点编号
     */
    void updateRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前区间完全包含在目标区间内，直接添加懒标记
            lazy(i, jobv, r - l + 1);
        } else {
            // 当前区间与目标区间有交集但不完全包含
            int mid = (l + r) >> 1;
            down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

            // 递归更新左子树
            if (jobl <= mid) {
                updateRange(jobl, jobr, jobv, l, mid, i << 1);
            }
            // 递归更新右子树
            if (jobr > mid) {
                updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
            }

            up(i);  // 向上更新当前节点的值
        }
    }

    /**
     * 区间查询操作
     * 查询区间[jobl, jobr]的和
     * @param jobl 目标区间左边界
     * @param jobr 目标区间右边界
     * @param l 当前节点代表的区间左边界
     * @param r 当前节点代表的区间右边界
     * @param i 当前节点编号
     * @return 区间和
     */
    long long query(int jobl, int jobr, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前区间完全包含在目标区间内，直接返回
            return sum[i];
        }

        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

        long long ans = 0;
        // 查询左子树
        if (jobl <= mid) {
            ans += query(jobl, jobr, l, mid, i << 1);
        }
        // 查询右子树
        if (jobr > mid) {
            ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
        }

        return ans;
    }

    /**
     * 生成随机值填入arr数组
     * 用于测试
     * @param n 数组大小
     * @param v 随机值的上界
     */
    void randomArray(int n, int v) {
        std::random_device rd;  // 随机数种子生成器
        std::mt19937 gen(rd()); // 梅森旋转算法随机数生成器
        std::uniform_int_distribution<> dis(0, v - 1);  // 均匀分布

        for (int i = 1; i <= n; i++) {
            arr[i] = dis(gen);  // 生成[0, v-1]范围的随机数
        }
    }

    /**
     * 验证结构的更新操作
     * 暴力更新，用于对比验证线段树的正确性
     * @param check 验证数组
     * @param jobl 区间左边界
     * @param jobr 区间右边界
     * @param jobv 要设置的值
     */
    void checkUpdate(std::vector<long long>& check, int jobl, int jobr, long long jobv) {
        for (int i = jobl; i <= jobr; i++) {
            check[i] = jobv;  // 逐个元素设置为jobv
        }
    }

    /**
     * 验证结构的查询操作
     * 暴力查询，用于对比验证线段树的正确性
     * @param check 验证数组
     * @param jobl 区间左边界
     * @param jobr 区间右边界
     * @return 区间和
     */
    long long checkQuery(const std::vector<long long>& check, int jobl, int jobr) {
        long long ans = 0;
        for (int i = jobl; i <= jobr; i++) {
            ans += check[i];  // 逐个元素累加
        }
        return ans;
    }

    /**
     * 对数器逻辑
     * 展示了线段树的建立和使用
     * 使用验证结构来检查线段树是否正常工作
     */
    void test() {
        std::cout << "" << std::endl;

        int n = 1000;     // 数组大小
        int v = 2000;     // 随机值范围
        int t = 5000000;  // 测试次数

        // 生成随机值填入arr数组
        randomArray(n, v);

        // 建立线段树
        build(1, n, 1);

        // 生成验证的结构（暴力数组）
        std::vector<long long> check(n + 1);
        for (int i = 1; i <= n; i++) {
            check[i] = arr[i];  // 复制原始数组到验证数组
        }

        // 随机数生成器设置
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> opDis(0, 1);        // 操作类型：0更新，1查询
        std::uniform_int_distribution<> indexDis(1, n);     // 索引范围[1, n]
        std::uniform_int_distribution<> valueDis(-v, v);    // 值范围[-v, v]

        // 执行大量随机测试
        for (int i = 1; i <= t; i++) {
            // 生成操作类型
            // op = 0 更新操作
            // op = 1 查询操作
            int op = opDis(gen);

            // 下标从1开始，不从0开始，生成两个随机下标
            int a = indexDis(gen);
            int b = indexDis(gen);

            // 确保jobl <= jobr
            int jobl = std::min(a, b);
            int jobr = std::max(a, b);

            if (op == 0) {
                // 更新操作
                // 线段树、验证结构同步更新
                long long jobv = valueDis(gen);
                updateRange(jobl, jobr, jobv, 1, n, 1);  // 线段树更新
                checkUpdate(check, jobl, jobr, jobv);    // 验证数组更新
            } else {
                // 查询操作
                // 线段树、验证结构同步查询
                // 比对答案
                long long ans1 = query(jobl, jobr, 1, n, 1);   // 线段树查询
                long long ans2 = checkQuery(check, jobl, jobr); // 验证数组查询

                if (ans1 != ans2) {
                    std::cout << "!" << std::endl;
                    std::cout << "[" << jobl << ", " << jobr << "]" << std::endl;
                    std::cout << ": " << ans1 << std::endl;
                    std::cout << ": " << ans2 << std::endl;
                    return;  // 发现错误，停止测试
                }
            }
        }

        std::cout << "" << std::endl;
    }
};

/**
 * 主函数
 * 创建线段树对象并执行测试
 */
int main() {
    SegmentTreeUpdateQuerySum segTree;
    segTree.test();
    return 0;
}
