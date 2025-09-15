// AVL树的实现(C++版)
// 支持的操作：
// 1. 插入一个数（允许重复，重复时计数增加）
// 2. 删除一个数（如果有多个，只删掉一个）
// 3. 查询一个数的排名（排名 = 比它小的数的个数 + 1）
// 4. 查询排名为x的数（即第x小的数）
// 5. 查询前驱（小于x的最大数，不存在返回 INT_MIN）
// 6. 查询后继（大于x的最小数，不存在返回 INT_MAX）
//
// 所有操作复杂度 O(log n)
// 测试题链接：https://www.luogu.com.cn/problem/P3369

#include <iostream>
#include <algorithm>
#include <climits>
#include <cstring>

using namespace std;

const int MAXN = 100001; // 节点最大数量（题目给出的操作数上限）

// ===== 全局数组模拟节点池 =====
int cnt = 0;            // 当前节点总数（相当于节点编号计数器）
int head = 0;           // 根节点编号
int key[MAXN];          // 每个节点存放的值
int height[MAXN];       // 每个节点的高度（用于AVL平衡）
int ls[MAXN];           // 左孩子节点编号
int rs[MAXN];           // 右孩子节点编号
int key_count[MAXN];    // 当前值出现的次数（支持重复值）
int siz[MAXN];          // 以该节点为根的子树大小（含重复值）

// ===== 工具函数 =====

// 更新节点信息：重新计算子树大小和高度
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];          // 子树大小 = 左子树 + 右子树 + 本节点重复数
    height[i] = max(height[ls[i]], height[rs[i]]) + 1;        // 节点高度 = max(左子树高度, 右子树高度) + 1
}

// 左旋操作（右子树上升，适用于右高不平衡）
int leftRotate(int i) {
    int r = rs[i];        // 右孩子
    rs[i] = ls[r];        // 把右孩子的左子树挂到i的右子树
    ls[r] = i;            // i成为r的左子树
    up(i);                // 更新i节点信息
    up(r);                // 更新r节点信息
    return r;             // 返回新的根
}

// 右旋操作（左子树上升，适用于左高不平衡）
int rightRotate(int i) {
    int l = ls[i];        // 左孩子
    ls[i] = rs[l];        // 把左孩子的右子树挂到i的左子树
    rs[l] = i;            // i成为l的右子树
    up(i);                // 更新i节点信息
    up(l);                // 更新l节点信息
    return l;             // 返回新的根
}

// 平衡维护：根据AVL树高度差进行旋转调整
int maintain(int i) {
    int lh = height[ls[i]], rh = height[rs[i]];
    if (lh - rh > 1) { // 左子树过高
        if (height[ls[ls[i]]] >= height[rs[ls[i]]]) {
            // 左-左型：单右旋
            i = rightRotate(i);
        } else {
            // 左-右型：先左旋再右旋
            ls[i] = leftRotate(ls[i]);
            i = rightRotate(i);
        }
    } else if (rh - lh > 1) { // 右子树过高
        if (height[rs[rs[i]]] >= height[ls[rs[i]]]) {
            // 右-右型：单左旋
            i = leftRotate(i);
        } else {
            // 右-左型：先右旋再左旋
            rs[i] = rightRotate(rs[i]);
            i = leftRotate(i);
        }
    }
    return i;
}

// ===== 插入操作 =====

// 向子树i中插入一个数
int add(int i, int num) {
    if (i == 0) { // 新建节点
        key[++cnt] = num;
        key_count[cnt] = siz[cnt] = height[cnt] = 1;  // 初始化信息
        return cnt; // 返回新节点编号
    }
    if (key[i] == num) {
        key_count[i]++; // 该值已存在，次数+1
    } else if (key[i] > num) {
        ls[i] = add(ls[i], num); // 插入左子树
    } else {
        rs[i] = add(rs[i], num); // 插入右子树
    }
    up(i);             // 更新节点信息
    return maintain(i); // 保持平衡
}

// 插入接口（从根节点开始）
void add(int num) {
    head = add(head, num);
}

// ===== 排名查询 =====

// 查询小于num的数的个数
int getRank(int i, int num) {
    if (i == 0) return 0; // 空树
    if (key[i] >= num) {
        return getRank(ls[i], num); // 往左子树找
    } else {
        // 左子树数量 + 本节点数量 + 右子树中继续找
        return siz[ls[i]] + key_count[i] + getRank(rs[i], num);
    }
}

// 查询num的排名（比它小的数的个数 + 1）
int getRank(int num) {
    return getRank(head, num) + 1;
}

// ===== 删除操作 =====

// 删除子树中最左节点（即最小值节点）
int removeMostLeft(int i, int mostLeft) {
    if (i == mostLeft) {
        return rs[i]; // 最小节点被删除后，用其右子树顶替
    } else {
        ls[i] = removeMostLeft(ls[i], mostLeft);
        up(i);
        return maintain(i);
    }
}

// 删除值为num的节点（只删一次）
int remove(int i, int num) {
    if (key[i] < num) {
        rs[i] = remove(rs[i], num);
    } else if (key[i] > num) {
        ls[i] = remove(ls[i], num);
    } else { // 找到目标值
        if (key_count[i] > 1) {
            key_count[i]--; // 有重复值，仅减少计数
        } else {
            if (ls[i] == 0 && rs[i] == 0) {
                return 0; // 叶子节点，直接删除
            } else if (ls[i] != 0 && rs[i] == 0) {
                i = ls[i]; // 只有左子树
            } else if (ls[i] == 0 && rs[i] != 0) {
                i = rs[i]; // 只有右子树
            } else {
                // 有左右子树：找到右子树最小值来替代
                int mostLeft = rs[i];
                while (ls[mostLeft] != 0) {
                    mostLeft = ls[mostLeft];
                }
                rs[i] = removeMostLeft(rs[i], mostLeft); // 删除右子树最小节点
                ls[mostLeft] = ls[i]; // 接管原节点的左右子树
                rs[mostLeft] = rs[i];
                i = mostLeft; // 替换节点
            }
        }
    }
    up(i);
    return maintain(i);
}

// 删除接口：确保值存在才删除
void remove(int num) {
    if (getRank(num) != getRank(num + 1)) {
        head = remove(head, num);
    }
}

// ===== 第k小值查询 =====

// 在子树i中查询第x小的值
int index(int i, int x) {
    if (siz[ls[i]] >= x) {
        return index(ls[i], x); // 在左子树
    } else if (siz[ls[i]] + key_count[i] < x) {
        // 在右子树，去掉左子树和本节点的数量
        return index(rs[i], x - siz[ls[i]] - key_count[i]);
    }
    return key[i]; // 正好落在当前节点
}

int index(int x) {
    return index(head, x);
}

// ===== 前驱后继查询 =====

// 查询前驱（小于num的最大值）
int pre(int i, int num) {
    if (i == 0) return INT_MIN; // 不存在
    if (key[i] >= num) {
        return pre(ls[i], num); // 去左子树找
    } else {
        return max(key[i], pre(rs[i], num)); // 当前值也可能是前驱
    }
}

int pre(int num) {
    return pre(head, num);
}

// 查询后继（大于num的最小值）
int post(int i, int num) {
    if (i == 0) return INT_MAX; // 不存在
    if (key[i] <= num) {
        return post(rs[i], num); // 去右子树找
    } else {
        return min(key[i], post(ls[i], num)); // 当前值也可能是后继
    }
}

int post(int num) {
    return post(head, num);
}

// ===== 清空树 =====
void clear() {
    memset(key + 1, 0, cnt * sizeof(int));
    memset(height + 1, 0, cnt * sizeof(int));
    memset(ls + 1, 0, cnt * sizeof(int));
    memset(rs + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int));
    memset(siz + 1, 0, cnt * sizeof(int));
    cnt = 0;
    head = 0;
}

// ===== 主程序 =====
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    for (int i = 1, op, x; i <= n; i++) {
        cin >> op >> x;
        if (op == 1) {
            add(x);           // 插入
        } else if (op == 2) {
            remove(x);        // 删除
        } else if (op == 3) {
            cout << getRank(x) << endl; // 查询排名
        } else if (op == 4) {
            cout << index(x) << endl;   // 查询第k小
        } else if (op == 5) {
            cout << pre(x) << endl;     // 查询前驱
        } else {
            cout << post(x) << endl;    // 查询后继
        }
    }
    clear();
    return 0;
}
