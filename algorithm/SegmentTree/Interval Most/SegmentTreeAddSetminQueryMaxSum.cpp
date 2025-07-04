#include <iostream>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>

// 线段树范围增加操作 + 区间最值操作
// 给定一个长度为n的数组arr，实现支持以下四种操作的结构
// 操作 0 l r x : 把arr[l..r]范围的每个数v，增加x
// 操作 1 l r x : 把arr[l..r]范围的每个数v，更新成min(v, x)
// 操作 2 l r   : 查询arr[l..r]范围上的最大值
// 操作 3 l r   : 查询arr[l..r]范围上的累加和
// 对数器验证

using namespace std;

const int MAXN = 500001;
const long long LOWEST = LLONG_MIN;

// ============== 数据存储 ==============

int arr[MAXN];                    // 原始数组

// ============== 线段树节点信息 ==============

long long sum[MAXN << 2];         // 区间和
long long maxVal[MAXN << 2];      // 区间最大值
int cnt[MAXN << 2];               // 最大值的个数
long long sem[MAXN << 2];         // 严格次大值（第二大的值）

// ============== 懒标记信息 ==============

long long maxAdd[MAXN << 2];      // 最大值的增加幅度
long long otherAdd[MAXN << 2];    // 非最大值的增加幅度

/**
 * 向上更新节点信息
 * @param i 当前节点编号
 */
void up(int i) {
    int l = i << 1;      // 左儿子
    int r = i << 1 | 1;  // 右儿子

    // 更新区间和
    sum[i] = sum[l] + sum[r];

    // 更新最大值
    maxVal[i] = max(maxVal[l], maxVal[r]);

    // 更新最大值个数和严格次大值
    if (maxVal[l] > maxVal[r]) {
        // 左儿子的最大值更大
        cnt[i] = cnt[l];
        sem[i] = max(sem[l], maxVal[r]);  // 次大值可能是左儿子的次大值或右儿子的最大值
    } else if (maxVal[l] < maxVal[r]) {
        // 右儿子的最大值更大
        cnt[i] = cnt[r];
        sem[i] = max(maxVal[l], sem[r]);  // 次大值可能是右儿子的次大值或左儿子的最大值
    } else {
        // 两边最大值相等
        cnt[i] = cnt[l] + cnt[r];
        sem[i] = max(sem[l], sem[r]);
    }
}

/**
 * 对节点施加懒标记
 * @param i 节点编号
 * @param n 节点代表的区间长度
 * @param maxAddv 最大值增加的值
 * @param otherAddv 非最大值增加的值
 */
void lazy(int i, int n, long long maxAddv, long long otherAddv) {
    // 更新区间和：最大值个数 × 最大值增量 + 非最大值个数 × 非最大值增量
    sum[i] += maxAddv * cnt[i] + otherAddv * (n - cnt[i]);

    // 更新最大值
    maxVal[i] += maxAddv;

    // 更新次大值（如果次大值不存在则保持LOWEST）
    sem[i] += (sem[i] == LOWEST) ? 0 : otherAddv;

    // 累积懒标记
    maxAdd[i] += maxAddv;
    otherAdd[i] += otherAddv;
}

/**
 * 向下传递懒标记
 * @param i 当前节点编号
 * @param ln 左儿子区间长度
 * @param rn 右儿子区间长度
 */
void down(int i, int ln, int rn) {
    int l = i << 1;
    int r = i << 1 | 1;

    // 找到当前区间的最大值
    long long tmp = max(maxVal[l], maxVal[r]);

    // 对左儿子施加懒标记
    if (maxVal[l] == tmp) {
        // 左儿子的最大值等于全局最大值，区分对待
        lazy(l, ln, maxAdd[i], otherAdd[i]);
    } else {
        // 左儿子的最大值小于全局最大值，统一增加otherAdd
        lazy(l, ln, otherAdd[i], otherAdd[i]);
    }

    // 对右儿子施加懒标记
    if (maxVal[r] == tmp) {
        // 右儿子的最大值等于全局最大值，区分对待
        lazy(r, rn, maxAdd[i], otherAdd[i]);
    } else {
        // 右儿子的最大值小于全局最大值，统一增加otherAdd
        lazy(r, rn, otherAdd[i], otherAdd[i]);
    }

    // 清空当前节点的懒标记
    maxAdd[i] = otherAdd[i] = 0;
}

/**
 * 建树
 * @param l 区间左端点
 * @param r 区间右端点
 * @param i 节点编号
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点
        sum[i] = maxVal[i] = arr[l];
        sem[i] = LOWEST;  // 单个元素没有次大值
        cnt[i] = 1;       // 最大值个数为1
    } else {
        // 非叶子节点，递归建树
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        up(i);  // 向上更新信息
    }
    // 初始化懒标记
    maxAdd[i] = otherAdd[i] = 0;
}

/**
 * 区间加法操作
 * @param jobl 操作区间左端点
 * @param jobr 操作区间右端点
 * @param jobv 增加的值
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 */
void add(int jobl, int jobr, long long jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 当前区间完全在操作区间内，统一增加jobv
        lazy(i, r - l + 1, jobv, jobv);
    } else {
        // 部分覆盖，需要递归处理
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i);  // 向上更新信息
    }
}

/**
 * 区间取最小值操作（核心操作）
 * @param jobl 操作区间左端点
 * @param jobr 操作区间右端点
 * @param jobv 最小值限制
 * @param l 当前区间左端点
 * @param r 当前区间右端点
 * @param i 当前节点编号
 */
void setMin(int jobl, int jobr, long long jobv, int l, int r, int i) {
    if (jobv >= maxVal[i]) {
        // 限制值大于等于区间最大值，无需操作
        return;
    }

    if (jobl <= l && r <= jobr && sem[i] < jobv) {
        // 当前区间完全在操作区间内，且次大值小于限制值
        // 这意味着只有最大值会被影响，可以直接用懒标记处理
        lazy(i, r - l + 1, jobv - maxVal[i], 0);
    } else {
        // 需要递归处理
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

        if (jobl <= mid) {
            setMin(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            setMin(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }

        up(i);  // 向上更新信息
    }
}

/**
 * 查询区间和
 */
long long querySum(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return sum[i];
    } else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

        long long ans = 0;
        if (jobl <= mid) {
            ans += querySum(jobl, jobr, l, mid, i << 1);
        }
        if (jobr > mid) {
            ans += querySum(jobl, jobr, mid + 1, r, i << 1 | 1);
        }
        return ans;
    }
}

/**
 * 查询区间最大值
 */
long long queryMax(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return maxVal[i];
    } else {
        int mid = (l + r) >> 1;
        down(i, mid - l + 1, r - mid);  // 先向下传递懒标记

        long long ans = LLONG_MIN;
        if (jobl <= mid) {
            ans = max(ans, queryMax(jobl, jobr, l, mid, i << 1));
        }
        if (jobr > mid) {
            ans = max(ans, queryMax(jobl, jobr, mid + 1, r, i << 1 | 1));
        }
        return ans;
    }
}

// ============== 测试验证函数 ==============

/**
 * 生成随机数组
 */
void randomArray(int n, int v) {
    for (int i = 1; i <= n; i++) {
        arr[i] = rand() % (v * 2 + 1) - v;  // 生成[-v, v]范围的随机数
    }
}

/**
 * 暴力实现区间加法（用于验证）
 */
void checkAdd(long long check[], int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] += jobv;
    }
}

/**
 * 暴力实现区间取最小值（用于验证）
 */
void checkSetMin(long long check[], int jobl, int jobr, long long jobv) {
    for (int i = jobl; i <= jobr; i++) {
        check[i] = min(check[i], jobv);
    }
}

/**
 * 暴力查询区间最大值（用于验证）
 */
long long checkQueryMax(long long check[], int jobl, int jobr) {
    long long ans = LLONG_MIN;
    for (int i = jobl; i <= jobr; i++) {
        ans = max(ans, check[i]);
    }
    return ans;
}

/**
 * 暴力查询区间和（用于验证）
 */
long long checkQuerySum(long long check[], int jobl, int jobr) {
    long long ans = 0;
    for (int i = jobl; i <= jobr; i++) {
        ans += check[i];
    }
    return ans;
}

// ============== 主函数：随机测试 ==============

int main() {
    srand(time(nullptr));
    cout << "start" << endl;

    // 测试参数（减小规模避免栈溢出）
    int n = 1000;      // 数组大小
    int v = 1000;      // 数值范围[-v, v]
    int t = 10000;     // 测试次数

    // 生成随机数组
    randomArray(n, v);

    // 创建对照数组（暴力实现）
    long long* check = new long long[n + 1];
    for (int i = 1; i <= n; i++) {
        check[i] = arr[i];
    }

    // 建立线段树
    build(1, n, 1);

    int errorCount = 0;

    // 随机测试
    for (int i = 1; i <= t; i++) {
        int op = rand() % 4;           // 随机选择操作类型
        int a = rand() % n + 1;
        int b = rand() % n + 1;
        int jobl = min(a, b);          // 确保jobl <= jobr
        int jobr = max(a, b);

        if (op == 0) {
            // 测试区间加法
            long long jobv = rand() % (v * 2 + 1) - v;
            add(jobl, jobr, jobv, 1, n, 1);
            checkAdd(check, jobl, jobr, jobv);
        } else if (op == 1) {
            // 测试区间取最小值
            long long jobv = rand() % (v * 2 + 1) - v;
            setMin(jobl, jobr, jobv, 1, n, 1);
            checkSetMin(check, jobl, jobr, jobv);
        } else if (op == 2) {
            // 测试查询最大值
            long long ans1 = queryMax(jobl, jobr, 1, n, 1);
            long long ans2 = checkQueryMax(check, jobl, jobr);
            if (ans1 != ans2) {
                cout << "查询最大值出错! 测试轮次: " << i << endl;
                cout << "线段树结果: " << ans1 << ", 暴力结果: " << ans2 << endl;
                errorCount++;
                if (errorCount > 5) break; // 避免过多错误输出
            }
        } else {
            // 测试查询区间和
            long long ans1 = querySum(jobl, jobr, 1, n, 1);
            long long ans2 = checkQuerySum(check, jobl, jobr);
            if (ans1 != ans2) {
                cout << "查询区间和出错! 测试轮次: " << i << endl;
                cout << "线段树结果: " << ans1 << ", 暴力结果: " << ans2 << endl;
                errorCount++;
                if (errorCount > 5) break;
            }
        }

        // 进度提示
        if (i % 1000 == 0) {
            cout << " " << i << " " << endl;
        }
    }

    // 清理内存
    delete[] check;

    // 输出测试结果
    if (errorCount == 0) {
        cout << "! " << endl;
    } else {
        cout << "发现 " << errorCount << " 个错误 ✗" << endl;
    }
    cout << "end" << endl;

    return 0;
}
