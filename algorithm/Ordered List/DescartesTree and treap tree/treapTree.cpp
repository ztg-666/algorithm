// Treap树的实现(C++版)
// 实现一种结构，支持如下操作，要求单次调用的时间复杂度O(log n)
// 1，增加x，重复加入算多个词频
// 2，删除x，如果有多个，只删掉一个
// 3，查询x的排名，x的排名为，比x小的数的个数+1
// 4，查询数据中排名为x的数
// 5，查询x的前驱，x的前驱为，小于x的数中最大的数，不存在返回整数最小值
// 6，查询x的后继，x的后继为，大于x的数中最小的数，不存在返回整数最大值
// 所有操作的次数 <= 10^5
// -10^7 <= x <= +10^7

#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100001;

// 全局变量定义
int cnt = 0;                    // 当前已分配的节点数量
int head = 0;                   // 根节点编号
int key[MAXN];                  // 存储每个节点的键值
int key_count[MAXN];            // 存储每个节点的键值出现次数（支持重复元素）
int ls[MAXN];                   // 存储每个节点的左子树编号
int rs[MAXN];                   // 存储每个节点的右子树编号
int siz[MAXN];                  // 存储每个子树的总节点数量（包含重复）
double priority[MAXN];          // 存储每个节点的随机优先级（堆性质）

// 更新节点i的子树大小
// 子树大小 = 左子树大小 + 右子树大小 + 当前节点的重复次数
void up(int i) {
    siz[i] = siz[ls[i]] + siz[rs[i]] + key_count[i];
}

// 左旋操作：当右子树的优先级大于当前节点时使用
// 将右子树提升为新的根节点，保持BST和堆的性质
int leftRotate(int i) {
    int r = rs[i];          // r是i的右子树
    rs[i] = ls[r];          // i的右子树变为r的左子树
    ls[r] = i;              // i变为r的左子树
    up(i);                  // 更新i的子树信息
    up(r);                  // 更新r的子树信息
    return r;               // 返回新的根节点r
}

// 右旋操作：当左子树的优先级大于当前节点时使用
// 将左子树提升为新的根节点，保持BST和堆的性质
int rightRotate(int i) {
    int l = ls[i];          // l是i的左子树
    ls[i] = rs[l];          // i的左子树变为l的右子树
    rs[l] = i;              // i变为l的右子树
    up(i);                  // 更新i的子树信息
    up(l);                  // 更新l的子树信息
    return l;               // 返回新的根节点l
}

// 向以i为根的子树中添加元素num
int add(int i, int num) {
    // 如果当前节点为空，创建新节点
    if (i == 0) {
        key[++cnt] = num;                                    // 分配新节点并设置键值
        key_count[cnt] = siz[cnt] = 1;                      // 初始化计数和大小为1
        priority[cnt] = static_cast<double>(rand()) / RAND_MAX;  // 设置随机优先级
        return cnt;                                         // 返回新节点编号
    }
    
    // 如果键值相同，增加计数
    if (key[i] == num) {
        key_count[i]++;
    } 
    // 如果num小于当前键值，插入左子树
    else if (key[i] > num) {
        ls[i] = add(ls[i], num);
    } 
    // 如果num大于当前键值，插入右子树
    else {
        rs[i] = add(rs[i], num);
    }
    
    up(i);  // 更新当前节点的子树信息
    
    // 维护堆性质：如果左子树优先级更大，进行右旋
    if (ls[i] != 0 && priority[ls[i]] > priority[i]) {
        return rightRotate(i);
    }
    // 维护堆性质：如果右子树优先级更大，进行左旋
    if (rs[i] != 0 && priority[rs[i]] > priority[i]) {
        return leftRotate(i);
    }
    return i;
}

// 向treap中添加元素num的公共接口
void add(int num) {
    head = add(head, num);
}

// 计算在以i为根的子树中，有多少个数小于num
int small(int i, int num) {
    if (i == 0) {
        return 0;  // 空树返回0
    }
    
    // 如果当前键值 >= num，那么只需要在左子树中查找
    if (key[i] >= num) {
        return small(ls[i], num);
    } 
    // 如果当前键值 < num，那么左子树和当前节点都小于num
    else {
        return siz[ls[i]] + key_count[i] + small(rs[i], num);
    }
}

// 获取num的排名（比num小的数的个数+1）
int getRank(int num) {
    return small(head, num) + 1;
}

// 查找排名为x的数（第x小的数）
int index(int i, int x) {
    // 如果左子树的大小 >= x，答案在左子树中
    if (siz[ls[i]] >= x) {
        return index(ls[i], x);
    } 
    // 如果左子树大小 + 当前节点计数 < x，答案在右子树中
    else if (siz[ls[i]] + key_count[i] < x) {
        return index(rs[i], x - siz[ls[i]] - key_count[i]);
    }
    // 否则答案就是当前节点的键值
    return key[i];
}

// 查找第x小数的公共接口
int index(int x) {
    return index(head, x);
}

// 查找num的前驱（小于num的最大数）
int pre(int i, int num) {
    if (i == 0) {
        return INT_MIN;  // 空树返回最小值
    }
    
    // 如果当前键值 >= num，前驱只可能在左子树中
    if (key[i] >= num) {
        return pre(ls[i], num);
    } 
    // 如果当前键值 < num，当前节点可能是前驱，也要检查右子树
    else {
        return max(key[i], pre(rs[i], num));
    }
}

// 查找前驱的公共接口
int pre(int num) {
    return pre(head, num);
}

// 查找num的后继（大于num的最小数）
int post(int i, int num) {
    if (i == 0) {
        return INT_MAX;  // 空树返回最大值
    }
    
    // 如果当前键值 <= num，后继只可能在右子树中
    if (key[i] <= num) {
        return post(rs[i], num);
    } 
    // 如果当前键值 > num，当前节点可能是后继，也要检查左子树
    else {
        return min(key[i], post(ls[i], num));
    }
}

// 查找后继的公共接口
int post(int num) {
    return post(head, num);
}

// 从以i为根的子树中删除一个num
int remove(int i, int num) {
    // 根据BST性质找到要删除的节点
    if (key[i] < num) {
        rs[i] = remove(rs[i], num);  // 在右子树中删除
    } else if (key[i] > num) {
        ls[i] = remove(ls[i], num);  // 在左子树中删除
    } else {
        // 找到了要删除的节点
        if (key_count[i] > 1) {
            // 如果有多个相同元素，只减少计数
            key_count[i]--;
        } else {
            // 只有一个元素，需要删除整个节点
            if (ls[i] == 0 && rs[i] == 0) {
                // 叶子节点，直接删除
                return 0;
            } else if (ls[i] != 0 && rs[i] == 0) {
                // 只有左子树
                i = ls[i];
            } else if (ls[i] == 0 && rs[i] != 0) {
                // 只有右子树
                i = rs[i];
            } else {
                // 有两个子树，通过旋转将优先级大的子树提升
                if (priority[ls[i]] >= priority[rs[i]]) {
                    i = rightRotate(i);         // 右旋后继续在右子树中删除
                    rs[i] = remove(rs[i], num);
                } else {
                    i = leftRotate(i);          // 左旋后继续在左子树中删除
                    ls[i] = remove(ls[i], num);
                }
            }
        }
    }
    up(i);  // 更新子树信息
    return i;
}

// 删除元素num的公共接口
void remove(int num) {
    // 只有当num确实存在时才删除（通过排名检查）
    if (getRank(num) != getRank(num + 1)) {
        head = remove(head, num);
    }
}

// 清空整个treap（重置所有数据）
void clear() {
    memset(key + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int));
    memset(ls + 1, 0, cnt * sizeof(int));
    memset(rs + 1, 0, cnt * sizeof(int));
    memset(siz + 1, 0, cnt * sizeof(int));
    memset(priority + 1, 0, cnt * sizeof(int));
    cnt = 0;
    head = 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(0));  // 初始化随机数种子
    
    int n;
    cin >> n;
    
    for (int i = 1, op, x; i <= n; i++) {
        cin >> op >> x;
        if (op == 1) {
            add(x);                    // 操作1：添加元素x
        } else if (op == 2) {
            remove(x);                 // 操作2：删除元素x
        } else if (op == 3) {
            cout << getRank(x) << endl;  // 操作3：查询x的排名
        } else if (op == 4) {
            cout << index(x) << endl;    // 操作4：查询排名为x的数
        } else if (op == 5) {
            cout << pre(x) << endl;      // 操作5：查询x的前驱
        } else {
            cout << post(x) << endl;     // 操作6：查询x的后继
        }
    }
    
    clear();  // 清理内存
    return 0;
}
