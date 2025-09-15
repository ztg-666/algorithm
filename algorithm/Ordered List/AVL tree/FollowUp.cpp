#include <iostream>
#include <algorithm>
#include <climits>
#include <cstring>

using namespace std;

const int MAXN = 2000001;  // 最大节点数（题目数据量很大，预分配数组）

// 全局变量（用数组实现节点池，避免动态分配内存）
int cnt = 0;              // 节点计数器（当前分配到的节点编号）
int head = 0;             // 根节点编号
int key[MAXN];            // 每个节点存储的关键字（数值）
int height[MAXN];         // 节点高度（AVL 平衡因子用）
int ls[MAXN];             // 左子节点编号
int rs[MAXN];             // 右子节点编号
int key_count[MAXN];      // 节点中该数值出现的次数（支持重复数）
int siz[MAXN];            // 子树大小（包含重复数）

// 更新当前节点的 size 和 height
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];   // 左子树大小 + 右子树大小 + 当前节点数
    height[i] = max(height[ls[i]], height[rs[i]]) + 1; // 高度 = max(左右子树高度) + 1
}

// 左旋操作（右子树上升）
int leftRotate(int i) {
    int r = rs[i];
    rs[i] = ls[r];
    ls[r] = i;
    up(i);
    up(r);
    return r;  // 返回新的根
}

// 右旋操作（左子树上升）
int rightRotate(int i) {
    int l = ls[i];
    ls[i] = rs[l];
    rs[l] = i;
    up(i);
    up(l);
    return l;  // 返回新的根
}

// AVL 维护平衡
int maintain(int i) {
    int lh = height[ls[i]];
    int rh = height[rs[i]];
    // 左子树过高
    if (lh - rh > 1) {
        if (height[ls[ls[i]]] >= height[rs[ls[i]]]) {
            i = rightRotate(i);  // LL 型
        } else {
            ls[i] = leftRotate(ls[i]); // LR 型
            i = rightRotate(i);
        }
    }
    // 右子树过高
    else if (rh - lh > 1) {
        if (height[rs[rs[i]]] >= height[ls[rs[i]]]) {
            i = leftRotate(i);   // RR 型
        } else {
            rs[i] = rightRotate(rs[i]); // RL 型
            i = leftRotate(i);
        }
    }
    return i;
}

// 插入操作（递归）
int add(int i, int num) {
    if (i == 0) {  // 新建节点
        key[++cnt] = num;
        key_count[cnt] = siz[cnt] = height[cnt] = 1;
        return cnt;
    }
    if (key[i] == num) {
        key_count[i]++;  // 该数已存在，出现次数 +1
    } else if (key[i] > num) {
        ls[i] = add(ls[i], num); // 插入到左子树
    } else {
        rs[i] = add(rs[i], num); // 插入到右子树
    }
    up(i);               // 更新 size 和 height
    return maintain(i);  // 维持 AVL 平衡
}

// 插入入口（对根操作）
void add(int num) {
    head = add(head, num);
}

// 查询 num 的排名（比 num 小的数的数量）
int getRank(int i, int num) {
    if (i == 0) return 0;
    if (key[i] >= num) {
        return getRank(ls[i], num);
    } else {
        return siz[ls[i]] + key_count[i] + getRank(rs[i], num);
    }
}
int getRank(int num) {
    return getRank(head, num) + 1; // 返回排名（题目要求从 1 开始）
}

// 删除右子树最左节点（中序后继）
int removeMostLeft(int i, int mostLeft) {
    if (i == mostLeft) {
        return rs[i];
    } else {
        ls[i] = removeMostLeft(ls[i], mostLeft);
        up(i);
        return maintain(i);
    }
}

// 删除操作（递归）
int remove(int i, int num) {
    if (key[i] < num) {
        rs[i] = remove(rs[i], num);
    } else if (key[i] > num) {
        ls[i] = remove(ls[i], num);
    } else {
        if (key_count[i] > 1) {
            key_count[i]--;  // 存在多个相同值，只减少计数
        } else {
            // 叶子节点
            if (ls[i] == 0 && rs[i] == 0) {
                return 0;
            }
            // 仅左子树
            else if (ls[i] != 0 && rs[i] == 0) {
                i = ls[i];
            }
            // 仅右子树
            else if (ls[i] == 0 && rs[i] != 0) {
                i = rs[i];
            }
            // 左右子树均存在，找右子树最小值替换
            else {
                int mostLeft = rs[i];
                while (ls[mostLeft] != 0) {
                    mostLeft = ls[mostLeft];
                }
                rs[i] = removeMostLeft(rs[i], mostLeft);
                ls[mostLeft] = ls[i];
                rs[mostLeft] = rs[i];
                i = mostLeft;
            }
        }
    }
    up(i);
    return maintain(i);
}

// 删除入口（确保该数存在）
void remove(int num) {
    if (getRank(num) != getRank(num + 1)) { // 判断是否存在
        head = remove(head, num);
    }
}

// 查询第 k 小的数（顺序统计）
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

// 查询前驱（比 num 小的最大数）
int pre(int i, int num) {
    if (i == 0) return INT_MIN;
    if (key[i] >= num) {
        return pre(ls[i], num);
    } else {
        return max(key[i], pre(rs[i], num));
    }
}
int pre(int num) {
    return pre(head, num);
}

// 查询后继（比 num 大的最小数）
int post(int i, int num) {
    if (i == 0) return INT_MAX;
    if (key[i] <= num) {
        return post(rs[i], num);
    } else {
        return min(key[i], post(ls[i], num));
    }
}
int post(int num) {
    return post(head, num);
}

// 清理数据（重置树）
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

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, lastAns = 0, ans = 0;
    cin >> n; // 初始序列长度
    cin >> m; // 操作次数

    // 插入初始序列
    for (int i = 1, num; i <= n; i++) {
        cin >> num;
        add(num);
    }

    // m 次操作
    for (int i = 1, op, x; i <= m; i++) {
        cin >> op >> x;
        x ^= lastAns; // 题目要求强制在线：输入与上次答案异或

        if (op == 1) {
            add(x);                   // 插入 x
        } else if (op == 2) {
            remove(x);                // 删除 x
        } else if (op == 3) {
            lastAns = getRank(x);     // 查询 x 的排名
            ans ^= lastAns;
        } else if (op == 4) {
            lastAns = index(x);       // 查询第 x 小的数
            ans ^= lastAns;
        } else if (op == 5) {
            lastAns = pre(x);         // 查询 x 的前驱
            ans ^= lastAns;
        } else {
            lastAns = post(x);        // 查询 x 的后继
            ans ^= lastAns;
        }
    }

    cout << ans << endl; // 输出异或和
    clear();             // 清理内存
    return 0;
}
