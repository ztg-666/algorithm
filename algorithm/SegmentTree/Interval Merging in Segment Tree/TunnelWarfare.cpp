#include <iostream>
#include <vector>
#include <string>
#include <stack>
using namespace std;

// 地道相连的房子
// 有n个房子排成一排，编号1~n，一开始每相邻的两个房子之间都有地道
// 实现如下三个操作
// 操作 D x : 把x号房子摧毁，该房子附近的地道也一并摧毁
// 操作 R   : 恢复上次摧毁的房子，该房子附近的地道一并恢复
// 操作 Q x : 查询x号房子能到达的房子数量，包括x号房子自身
// 测试链接 : https://www.luogu.com.cn/problem/P1503


const int MAXN = 50001;

// 线段树节点维护的信息
int pre[MAXN << 2];  // 连续1的最长前缀长度（从区间左端开始的连续1个数）
int suf[MAXN << 2];  // 连续1的最长后缀长度（从区间右端开始的连续1个数）

// 栈用于存储被摧毁的房屋编号，支持恢复操作（后进先出）
int stk[MAXN];
int stackSize = 0;

/**
 * 更新父节点信息
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 */
void up(int l, int r, int i) {
    // 初始化：父节点的前缀等于左子树的前缀
    pre[i] = pre[i << 1];
    // 初始化：父节点的后缀等于右子树的后缀
    suf[i] = suf[i << 1 | 1];

    int mid = (l + r) >> 1;

    // 如果左子树全是1，那么父节点的前缀可能延伸到右子树
    if (pre[i << 1] == mid - l + 1) {
        pre[i] += pre[i << 1 | 1];
    }

    // 如果右子树全是1，那么父节点的后缀可能延伸到左子树
    if (suf[i << 1 | 1] == r - mid) {
        suf[i] += suf[i << 1];
    }
}

/**
 * 构建线段树，初始时所有房屋都存在（值为1）
 * @param l 区间左边界
 * @param r 区间右边界
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    if (l == r) {
        // 叶子节点：单个房屋存在，前缀和后缀都是1
        pre[i] = suf[i] = 1;
    } else {
        int mid = (l + r) >> 1;
        // 递归构建左右子树
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
        // 更新当前节点信息
        up(l, r, i);
    }
}

/**
 * 单点更新：修改某个位置房屋的状态
 * @param jobi 要修改的位置
 * @param jobv 新的值（0表示摧毁，1表示存在）
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 */
void update(int jobi, int jobv, int l, int r, int i) {
    if (l == r) {
        // 到达叶子节点，直接修改
        pre[i] = suf[i] = jobv;
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            // 目标位置在左子树
            update(jobi, jobv, l, mid, i << 1);
        } else {
            // 目标位置在右子树
            update(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
        // 更新当前节点信息
        up(l, r, i);
    }
}

/**
 * 查询以jobi为中心能扩展出的最大连续长度
 * @param jobi 查询的中心位置
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 * @return 返回以jobi为中心的最大连续长度
 */
int query(int jobi, int l, int r, int i) {
    if (l == r) {
        // 叶子节点：返回当前位置的值
        return pre[i];
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            // 查询位置在左子树
            if (jobi > mid - suf[i << 1]) {
                // 如果jobi在左子树的后缀范围内，可以跨越中点扩展到右子树
                return suf[i << 1] + pre[i << 1 | 1];
            } else {
                // 否则只在左子树内查询
                return query(jobi, l, mid, i << 1);
            }
        } else {
            // 查询位置在右子树
            if (mid + pre[i << 1 | 1] >= jobi) {
                // 如果jobi在右子树的前缀范围内，可以跨越中点扩展到左子树
                return suf[i << 1] + pre[i << 1 | 1];
            } else {
                // 否则只在右子树内查询
                return query(jobi, mid + 1, r, i << 1 | 1);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    int n, m;  // n: 房屋数量，m: 操作数量

    while (cin >> n >> m) {
        // 构建线段树，范围是1到n
        build(1, n, 1);
        string op;  // 操作类型
        stackSize = 0;  // 重置栈大小

        for (int i = 1; i <= m; i++) {
            cin >> op;
            if (op == "D") {
                // 摧毁操作
                int x;
                cin >> x;
                update(x, 0, 1, n, 1);  // 将位置x的值设为0
                stk[stackSize++] = x;   // 将摧毁的房屋编号入栈
            } else if (op == "R") {
                // 恢复操作：恢复最近摧毁的房屋
                update(stk[--stackSize], 1, 1, n, 1);  // 从栈顶取出房屋编号并恢复
            } else { // op == "Q"
                // 查询操作
                int x;
                cin >> x;
                cout << query(x, 1, n, 1) << "\n";  // 查询以x为中心的最大连续长度
            }
        }
    }

    return 0;
}
