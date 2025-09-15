// 跳表的实现(C++版)
// 支持以下操作，单次调用 O(log n) 时间复杂度：
// 1. 插入一个数 x（允许重复，重复算多个词频）
// 2. 删除一个数 x（如果有多个，只删掉一个）
// 3. 查询数 x 的排名（排名定义为比 x 小的数的数量 + 1）
// 4. 查询排名为 x 的数
// 5. 查询数 x 的前驱（比 x 小的最大数，不存在返回 INT_MIN）
// 6. 查询数 x 的后继（比 x 大的最小数，不存在返回 INT_MAX）
//
// 测试链接 : https://www.luogu.com.cn/problem/P3369

#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
using namespace std;

const int MAXL = 20;     // 最大层数（跳表高度上限）
const int MAXN = 100001; // 节点总数上限

// 节点存储
int cnt;                          // 已使用的节点数量
int key[MAXN];                    // 节点存储的值
int key_count[MAXN];              // 节点值的出现次数（支持重复）
int level[MAXN];                  // 每个节点的层高（随机生成）
int next_node[MAXN][MAXL + 1];    // 每个节点在不同层的 "下一个节点"
int len[MAXN][MAXL + 1];          // 跨越的节点数（用于排名计算）

// 初始化跳表，建一个头结点（值为 INT_MIN）
void build() {
    cnt = 1;
    key[cnt] = INT_MIN;   // 头节点值
    level[cnt] = MAXL;    // 头节点层数设为最大
}

// 清理内存（测试结束后可调用）
void clear() {
    memset(key + 1, 0, cnt * sizeof(int));
    memset(key_count + 1, 0, cnt * sizeof(int));
    memset(level + 1, 0, cnt * sizeof(int));
    for (int i = 1; i <= cnt; i++) {
        memset(next_node[i], 0, (MAXL + 1) * sizeof(int));
        memset(len[i], 0, (MAXL + 1) * sizeof(int));
    }
    cnt = 0;
}

// 随机生成层数，概率分布接近几何分布
int randomLevel() {
    int ans = 1;
    while ((std::rand() / double(RAND_MAX)) < 0.5) { // 概率 0.5 提升一层
        ans++;
    }
    return min(ans, MAXL);
}

// 查找是否存在值 num，存在返回其节点下标，否则返回 0
int find(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h]; // 向右走
    }
    if (h == 1) {
        // 最底层比较
        if (next_node[i][h] != 0 && key[next_node[i][h]] == num) {
            return next_node[i][h];
        } else {
            return 0;
        }
    }
    return find(i, h - 1, num); // 向下走
}

// 插入已存在元素时，只增加计数
void addCount(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        key_count[next_node[i][h]]++;
    } else {
        addCount(i, h - 1, num);
    }
    len[i][h]++; // 更新跨越长度
}

// 在跳表中插入新节点 j（其值为 key[j]）
// 插入新节点 j 到跳表中
// 参数：
//   i : 当前遍历的节点下标
//   h : 当前所在层数
//   j : 新节点的下标
// 返回：插入位置左侧已有元素的数量（用于排名计算）
int addNode(int i, int h, int j) {
    int rightCnt = 0; // 统计在本层向右走过的节点数（累计排名）

    // 在当前层，向右走，直到找到比 key[j] 大或等于的位置
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        rightCnt += len[i][h];  // 记录向右跨越的节点数量
        i = next_node[i][h];    // 继续向右
    }

    // ----------- 到达底层（h == 1），执行真正的插入 ------------
    if (h == 1) {
        // 将新节点 j 插入到 i 之后
        next_node[j][h] = next_node[i][h];  // j 指向 i 的下一个
        next_node[i][h] = j;                // i 指向 j

        // 更新长度数组 len
        // j 节点跨越的数量 = 其下一个节点的计数（即右边节点的元素数）
        len[j][h] = key_count[next_node[j][h]];
        // i 节点跨越的数量 = 新插入节点的元素数
        len[i][h] = key_count[next_node[i][h]];

        return rightCnt; // 返回本层走过的元素数
    }
    // ----------- 非底层，递归到下一层（h-1）继续处理 ------------
    else {
        // 向下一层递归，得到在下层走过的元素数量
        int downCnt = addNode(i, h - 1, j);

        // 如果当前层高 > 新节点的层高
        // 说明新节点在这一层是“不可见”的，只更新长度即可
        if (h > level[j]) {
            len[i][h]++;
        }
        // 否则，新节点在这一层也要插入
        else {
            next_node[j][h] = next_node[i][h];  // j 指向 i 的下一个
            next_node[i][h] = j;                // i 指向 j

            // 更新长度
            // j 跨越的数量 = 原来 i 跨越的数量 + 1（新节点）
            //              - downCnt（下层走过的节点数） - key_count[j]（j 本身的元素数）
            len[j][h] = len[i][h] + 1 - downCnt - key_count[j];

            // i 跨越的数量 = 下层走过的数量 + 新节点的元素数
            len[i][h] = downCnt + key_count[j];
        }

        // 返回当前层走过的数量 + 下层走过的数量
        return rightCnt + downCnt;
    }
}

// 插入一个值 num
void add(int num) {
    if (find(1, MAXL, num) != 0) {
        addCount(1, MAXL, num); // 已存在，计数+1
    } else {
        key[++cnt] = num;
        key_count[cnt] = 1;
        level[cnt] = randomLevel();
        addNode(1, MAXL, cnt);
    }
}

// 删除已存在值时，仅减少计数
void removeCount(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        key_count[next_node[i][h]]--;
    } else {
        removeCount(i, h - 1, num);
    }
    len[i][h]--;
}

// 删除整个节点 j
void removeNode(int i, int h, int j) {
    if (h < 1) {
        return;
    }
    while (next_node[i][h] != 0 && key[next_node[i][h]] < key[j]) {
        i = next_node[i][h];
    }
    if (h > level[j]) {
        len[i][h]--;
    } else {
        next_node[i][h] = next_node[j][h];
        len[i][h] += len[j][h] - 1;
    }
    removeNode(i, h - 1, j);
}

// 删除一个值 num（若有多个，只删一个）
void remove(int num) {
    int j = find(1, MAXL, num);
    if (j != 0) {
        if (key_count[j] > 1) {
            removeCount(1, MAXL, num);
        } else {
            removeNode(1, MAXL, j);
        }
    }
}

// 计算比 num 小的数的个数
int small(int i, int h, int num) {
    int rightCnt = 0;
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        rightCnt += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return rightCnt;
    } else {
        return rightCnt + small(i, h - 1, num);
    }
}

// 查询 num 的排名
int getRank(int num) {
    return small(1, MAXL, num) + 1;
}

// 查询排名为 x 的数
int index(int i, int h, int x) {
    int c = 0;
    while (next_node[i][h] != 0 && c + len[i][h] < x) {
        c += len[i][h];
        i = next_node[i][h];
    }
    if (h == 1) {
        return key[next_node[i][h]];
    } else {
        return index(i, h - 1, x - c);
    }
}
int index(int x) {
    return index(1, MAXL, x);
}

// 查询 num 的前驱
int pre(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        return i == 1 ? INT_MIN : key[i];
    } else {
        return pre(i, h - 1, num);
    }
}
int pre(int num) {
    return pre(1, MAXL, num);
}

// 查询 num 的后继
int post(int i, int h, int num) {
    while (next_node[i][h] != 0 && key[next_node[i][h]] < num) {
        i = next_node[i][h];
    }
    if (h == 1) {
        if (next_node[i][h] == 0) {
            return INT_MAX;
        }
        if (key[next_node[i][h]] > num) {
            return key[next_node[i][h]];
        }
        i = next_node[i][h];
        if (next_node[i][h] == 0) {
            return INT_MAX;
        } else {
            return key[next_node[i][h]];
        }
    } else {
        return post(i, h - 1, num);
    }
}
int post(int num) {
    return post(1, MAXL, num);
}

// 主函数，处理操作
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(0));
    build(); // 初始化跳表
    int n;
    cin >> n;
    for (int i = 1, op, x; i <= n; i++) {
        cin >> op >> x;
        if (op == 1) {
            add(x);
        } else if (op == 2) {
            remove(x);
        } else if (op == 3) {
            cout << getRank(x) << endl;
        } else if (op == 4) {
            cout << index(x) << endl;
        } else if (op == 5) {
            cout << pre(x) << endl;
        } else {
            cout << post(x) << endl;
        }
    }
    clear(); // 释放内存
    return 0;
}
