#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>
#include <cstring>

using namespace std;

// 替罪羊树实现普通有序表，数据加强的测试，C++版
// 这个文件课上没有讲，测试数据加强了，而且有强制在线的要求
// 测试链接 : https://www.luogu.com.cn/problem/P6136

// 平衡因子 ALPHA：子树大小不平衡时触发 rebuild
const double ALPHA = 0.7;

// 最大节点数（题目数据范围决定，2e6 足够）
const int MAXN = 2000001;

// 树相关数组定义
int head = 0;                  // 根节点编号
int cnt = 0;                   // 已用节点数量（节点编号递增）
int key[MAXN];                 // 节点存储的值
int key_count[MAXN];           // 节点值的重复次数
int ls[MAXN];                  // 左子节点编号
int rs[MAXN];                  // 右子节点编号
int siz[MAXN];                 // 子树大小（含重复元素）
int diff[MAXN];                // 子树不同元素个数（key_count>0 的节点数）

// rebuild 时的辅助数组
int collect[MAXN];             // 中序遍历的节点集合
int ci;                        // collect 数组下标
int top;                       // 待重建的子树根节点
int father;                    // 待重建子树的父节点
int side;                      // 待重建子树是父节点的左(1)/右(2)

// 创建一个新节点
int init(int num) {
    key[++cnt] = num;
    ls[cnt] = rs[cnt] = 0;
    key_count[cnt] = siz[cnt] = diff[cnt] = 1;
    return cnt;
}

// 上推：更新 siz 和 diff
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
    diff[i] = diff[ls[i]] + diff[rs[i]] + (key_count[i] > 0 ? 1 : 0);
}

// 中序遍历收集节点（用于 rebuild）
void inorder(int i) {
    if (i != 0) {
        inorder(ls[i]);
        if (key_count[i] > 0) {    // 只收集有效节点
            collect[++ci] = i;
        }
        inorder(rs[i]);
    }
}

// 通过有序数组重建平衡子树
int build(int l, int r) {
    if (l > r) return 0;
    int m = (l + r) / 2;
    int h = collect[m];
    ls[h] = build(l, m - 1);
    rs[h] = build(m + 1, r);
    up(h);
    return h;
}

// rebuild 某个子树
void rebuild() {
    if (top != 0) {
        ci = 0;
        inorder(top);
        if (ci > 0) {
            if (father == 0) {          // top 是根
                head = build(1, ci);
            } else if (side == 1) {     // top 是父节点的左子树
                ls[father] = build(1, ci);
            } else {                    // top 是父节点的右子树
                rs[father] = build(1, ci);
            }
        }
    }
}

// 判断当前节点是否平衡
bool balance(int i) {
    return ALPHA * diff[i] >= max(diff[ls[i]], diff[rs[i]]);
}

// 插入节点（递归）
void add(int i, int f, int s, int num) {
    if (i == 0) { // 插入到空位置
        if (f == 0) {
            head = init(num);
        } else if (s == 1) {
            ls[f] = init(num);
        } else {
            rs[f] = init(num);
        }
    } else {
        if (key[i] == num) {
            key_count[i]++;  // 已存在，数量+1
        } else if (key[i] > num) {
            add(ls[i], i, 1, num);
        } else {
            add(rs[i], i, 2, num);
        }
        up(i);
        if (!balance(i)) {   // 不平衡，记录需要重建的节点
            top = i;
            father = f;
            side = s;
        }
    }
}

// 插入接口
void add(int num) {
    top = father = side = 0;
    add(head, 0, 0, num);
    rebuild();   // 若有不平衡，重建
}

// 统计 < num 的元素个数
int small(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) {
        return small(ls[i], num);
    } else {
        return siz[ls[i]] + key_count[i] + small(rs[i], num);
    }
}

// 查询 x 的排名（比它小的数的数量+1）
int getRank(int num) {
    return small(head, num) + 1;
}

// 根据排名找到值
int index(int i, int x) {
    if (siz[ls[i]] >= x) {
        return index(ls[i], x);
    } else if (siz[ls[i]] + key_count[i] < x) {
        return index(rs[i], x - siz[ls[i]] - key_count[i]);
    }
    return key[i];
}

int index(int x) {
    return index(head, x);
}

// 找 num 的前驱（小于 num 的最大值）
int pre(int num) {
    int kth = getRank(num);
    if (kth == 1) {
        return INT_MIN;  // 没有前驱
    } else {
        return index(kth - 1);
    }
}

// 找 num 的后继（大于 num 的最小值）
int post(int num) {
    int kth = getRank(num + 1);
    if (kth == siz[head] + 1) {
        return INT_MAX;  // 没有后继
    } else {
        return index(kth);
    }
}

// 删除节点（递归）
void remove(int i, int f, int s, int num) {
    if (key[i] == num) {
        key_count[i]--;   // 只减少计数
    } else if (key[i] > num) {
        remove(ls[i], i, 1, num);
    } else {
        remove(rs[i], i, 2, num);
    }
    up(i);
    if (!balance(i)) {   // 不平衡则记录
        top = i;
        father = f;
        side = s;
    }
}

// 删除接口
void remove(int num) {
    // 判断 num 是否存在（通过排名判断）
    if (getRank(num) != getRank(num + 1)) {
        top = father = side = 0;
        remove(head, 0, 0, num);
        rebuild();
    }
}

// 清空所有数组
void clear() {
    memset(key, 0, sizeof(key));
    memset(key_count, 0, sizeof(key_count));
    memset(ls, 0, sizeof(ls));
    memset(rs, 0, sizeof(rs));
    memset(siz, 0, sizeof(siz));
    memset(diff, 0, sizeof(diff));
    cnt = 0;
    head = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, lastAns = 0, ans = 0;
    cin >> n;
    cin >> m;

    // 读入初始序列
    for (int i = 1, num; i <= n; i++) {
        cin >> num;
        add(num);
    }

    // m 次操作
    for (int i = 1, op, x; i <= m; i++) {
        cin >> op >> x;
        x ^= lastAns;   // 强制在线处理

        if (op == 1) {
            add(x);
        } else if (op == 2) {
            remove(x);
        } else if (op == 3) {     // 查询 x 的排名
            lastAns = getRank(x);
            ans ^= lastAns;
        } else if (op == 4) {     // 查询第 x 小的数
            lastAns = index(x);
            ans ^= lastAns;
        } else if (op == 5) {     // 查询前驱
            lastAns = pre(x);
            ans ^= lastAns;
        } else {                  // 查询后继
            lastAns = post(x);
            ans ^= lastAns;
        }
    }

    cout << ans << endl;
    clear();
    return 0;
}
