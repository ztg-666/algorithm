#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

/**
 * 线段树类 - 支持范围更新、范围增加、范围查询
 * 维护区间累加和，使用懒惰传播优化
 */
class SegmentTree {
private:
    static const int MAXN = 1000001;  // 最大数组长度

    // 线段树的各种数组
    std::vector<long long> arr;     // 原始数组
    std::vector<long long> sum;     // 线段树节点存储的区间和
    std::vector<long long> add;     // 懒惰标记：区间增加值
    std::vector<long long> change;  // 懒惰标记：区间更新值
    std::vector<bool> update;       // 懒惰标记：是否有更新操作

    /**
     * 向上更新父节点
     * @param i 当前节点编号
     */
    void up(int i) {
        sum[i] = sum[i << 1] + sum[i << 1 | 1];  // 父节点 = 左子节点 + 右子节点
    }

    /**
     * 向下传播懒惰标记
     * @param i 当前节点编号
     * @param ln 左子树节点数量
     * @param rn 右子树节点数量
     */
    void down(int i, int ln, int rn) {
        // 先处理更新操作（优先级更高）
        if (update[i]) {
            updateLazy(i << 1, change[i], ln);      // 更新左子树
            updateLazy(i << 1 | 1, change[i], rn);  // 更新右子树
            update[i] = false;  // 清除当前节点的更新标记
        }
        // 再处理增加操作
        if (add[i] != 0) {
            addLazy(i << 1, add[i], ln);      // 增加左子树
            addLazy(i << 1 | 1, add[i], rn);  // 增加右子树
            add[i] = 0;  // 清除当前节点的增加标记
        }
    }

    /**
     * 设置更新懒惰标记
     * @param i 节点编号
     * @param v 更新值
     * @param n 该节点覆盖的元素个数
     */
    void updateLazy(int i, long long v, int n) {
        sum[i] = v * n;     // 区间和 = 更新值 × 元素个数
        add[i] = 0;         // 清除增加标记（因为更新会覆盖之前的增加）
        change[i] = v;      // 设置更新值
        update[i] = true;   // 标记有更新操作
    }

    /**
     * 设置增加懒惰标记
     * @param i 节点编号
     * @param v 增加值
     * @param n 该节点覆盖的元素个数
     */
    void addLazy(int i, long long v, int n) {
        sum[i] += v * n;  // 区间和 += 增加值 × 元素个数
        add[i] += v;      // 累加增加值
    }

public:
    /**
     * 构造函数：初始化所有数组
     */
    SegmentTree() : arr(MAXN), sum(MAXN << 2), add(MAXN << 2),
                    change(MAXN << 2), update(MAXN << 2) {}

    /**
     * 建立线段树
     * @param l 当前区间左边界
     * @param r 当前区间右边界
     * @param i 当前节点编号
     */
    void build(int l, int r, int i) {
        if (l == r) {
            // 叶子节点：直接使用原数组的值
            sum[i] = arr[l];
        } else {
            // 非叶子节点：递归建立左右子树
            int mid = (l + r) >> 1;           // 中点
            build(l, mid, i << 1);            // 建立左子树
            build(mid + 1, r, i << 1 | 1);   // 建立右子树
            up(i);                            // 向上更新当前节点
        }
        // 初始化懒惰标记
        add[i] = 0;
        change[i] = 0;
        update[i] = false;
    }

    /**
     * 范围更新操作
     * @param jobl 操作区间左边界
     * @param jobr 操作区间右边界
     * @param jobv 更新值
     * @param l 当前节点区间左边界
     * @param r 当前节点区间右边界
     * @param i 当前节点编号
     */
    void updateRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前区间完全被操作区间包含，直接更新
            updateLazy(i, jobv, r - l + 1);
        } else {
            // 当前区间与操作区间有交集但不完全包含
            int mid = (l + r) >> 1;
            down(i, mid - l + 1, r - mid);  // 先向下传播懒惰标记

            // 递归更新左子树
            if (jobl <= mid) {
                updateRange(jobl, jobr, jobv, l, mid, i << 1);
            }
            // 递归更新右子树
            if (jobr > mid) {
                updateRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
            }
            up(i);  // 向上更新当前节点
        }
    }

    /**
     * 范围增加操作
     * @param jobl 操作区间左边界
     * @param jobr 操作区间右边界
     * @param jobv 增加值
     * @param l 当前节点区间左边界
     * @param r 当前节点区间右边界
     * @param i 当前节点编号
     */
    void addRange(int jobl, int jobr, long long jobv, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前区间完全被操作区间包含，直接增加
            addLazy(i, jobv, r - l + 1);
        } else {
            // 当前区间与操作区间有交集但不完全包含
            int mid = (l + r) >> 1;
            down(i, mid - l + 1, r - mid);  // 先向下传播懒惰标记

            // 递归增加左子树
            if (jobl <= mid) {
                addRange(jobl, jobr, jobv, l, mid, i << 1);
            }
            // 递归增加右子树
            if (jobr > mid) {
                addRange(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
            }
            up(i);  // 向上更新当前节点
        }
    }

    /**
     * 范围查询操作
     * @param jobl 查询区间左边界
     * @param jobr 查询区间右边界
     * @param l 当前节点区间左边界
     * @param r 当前节点区间右边界
     * @param i 当前节点编号
     * @return 查询区间的累加和
     */
    long long query(int jobl, int jobr, int l, int r, int i) {
        if (jobl <= l && r <= jobr) {
            // 当前区间完全被查询区间包含，直接返回
            return sum[i];
        }

        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传播懒惰标记
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
     * 设置原数组的值
     * @param index 数组下标
     * @param value 设置的值
     */
    void setArray(int index, long long value) {
        arr[index] = value;
    }

    /**
     * 获取原数组的值
     * @param index 数组下标
     * @return 数组该位置的值
     */
    long long getArray(int index) {
        return arr[index];
    }
};

/**
 * 生成随机数组用于测试
 * @param st 线段树对象
 * @param n 数组长度
 * @param v 随机数范围[0, v-1]
 */
void randomArray(SegmentTree& st, int n, int v) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, v - 1);

    for (int i = 1; i <= n; i++) {
        st.setArray(i, dis(gen));
    }
}

/**
 * 验证用的暴力增加操作
 * @param check 验证数组
 * @param jobl 操作区间左边界
 * @param jobr 操作区间右边界
 * @param jobv 增加值
 */
void checkAdd(std::vector<long long>& check, int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] += jobv;
    }
}

/**
 * 验证用的暴力更新操作
 * @param check 验证数组
 * @param jobl 操作区间左边界
 * @param jobr 操作区间右边界
 * @param jobv 更新值
 */
void checkUpdate(std::vector<long long>& check, int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] = jobv;
    }
}

/**
 * 验证用的暴力查询操作
 * @param check 验证数组
 * @param jobl 查询区间左边界
 * @param jobr 查询区间右边界
 * @return 查询区间的累加和
 */
long long checkQuery(const std::vector<long long>& check, int jobl, int jobr) {
    long long ans = 0;
    for (int i = jobl; i <= jobr; i++) {
        ans += check[i];
    }
    return ans;
}

/**
 * 主函数：测试线段树的正确性
 * 使用对数器方法，将线段树结果与暴力方法对比
 */
int main() {
    std::cout << "" << std::endl;

    // 测试参数
    int n = 1000;       // 数组长度
    int v = 2000;       // 随机数值范围
    int t = 5000000;    // 测试次数

    SegmentTree st;

    // 生成随机值填入arr数组
    randomArray(st, n, v);

    // 建立线段树
    st.build(1, n, 1);

    // 生成验证的结构（暴力方法用的数组）
    std::vector<long long> check(n + 1);
    for (int i = 1; i <= n; i++) {
        check[i] = st.getArray(i);
    }

    // 随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> op_dis(0, 2);     // 操作类型[0,2]
    std::uniform_int_distribution<> idx_dis(1, n);    // 数组下标[1,n]
    std::uniform_int_distribution<> val_dis(-v, v);   // 操作值[-v,v]

    // 进行大量随机测试
    for (int i = 1; i <= t; i++) {
        // 生成操作类型：0=增加，1=更新，2=查询
        int op = op_dis(gen);

        // 生成两个随机下标
        int a = idx_dis(gen);
        int b = idx_dis(gen);

        // 确保左边界 <= 右边界
        int jobl = std::min(a, b);
        int jobr = std::max(a, b);

        if (op == 0) {
            // 增加操作：线段树和验证结构同步操作
            int jobv = val_dis(gen);
            st.addRange(jobl, jobr, jobv, 1, n, 1);  // 线段树增加
            checkAdd(check, jobl, jobr, jobv);       // 暴力增加
        } else if (op == 1) {
            // 更新操作：线段树和验证结构同步操作
            int jobv = val_dis(gen);
            st.updateRange(jobl, jobr, jobv, 1, n, 1);  // 线段树更新
            checkUpdate(check, jobl, jobr, jobv);       // 暴力更新
        } else {
            // 查询操作：比较线段树和暴力方法的结果
            long long ans1 = st.query(jobl, jobr, 1, n, 1);  // 线段树查询
            long long ans2 = checkQuery(check, jobl, jobr);   // 暴力查询
            if (ans1 != ans2) {
                std::cout << "" << std::endl;
                return 1;
            }
        }
    }

    std::cout << "" << std::endl;
    return 0;
}
