#include <bits/stdc++.h>

// 重建队列(做到最优时间复杂度)
// 一共n个人，每个人有(a, b)两个数据，数据a表示该人的身高
// 数据b表示该人的要求，站在自己左边的人中，正好有b个人的身高大于等于自己的身高
// 请你把n个人从左到右进行排列，要求每个人的要求都可以满足
// 返回其中一种排列即可，本题的数据保证一定存在这样的排列
// 题解中的绝大多数方法，时间复杂度O(n平方)，但是时间复杂度能做到O(n * log n)
// 测试链接 : https://leetcode.cn/problems/queue-reconstruction-by-height/

using namespace std;

const int MAXN = 2001;

// ------- 全局变量 --------
int cnt = 0, head = 0, fi = 0;
// cnt: 节点总数（作为数组下标模拟指针）
// head: 树根节点的下标
// fi: 遍历时的下标（用于 inOrder）

int key[MAXN], value[MAXN], leftc[MAXN], rightc[MAXN], sizec[MAXN], heightc[MAXN];
// key[i]: 节点 i 存放的身高
// value[i]: 节点 i 存放的 k 值
// leftc[i], rightc[i]: 左右子节点下标
// sizec[i]: 子树大小
// heightc[i]: 子树高度（用于平衡）

// ------- 工具函数 --------

// 更新节点信息（size, height）
void up(int i) {
    sizec[i] = sizec[leftc[i]] + sizec[rightc[i]] + 1;
    heightc[i] = max(heightc[leftc[i]], heightc[rightc[i]]) + 1;
}

// 左旋
int leftRotate(int i) {
    int r = rightc[i];
    rightc[i] = leftc[r];
    leftc[r] = i;
    up(i);
    up(r);
    return r;  // 新的根
}

// 右旋
int rightRotate(int i) {
    int l = leftc[i];
    leftc[i] = rightc[l];
    rightc[l] = i;
    up(i);
    up(l);
    return l;  // 新的根
}

// 维护 AVL 平衡
int maintain(int i) {
    int lh = heightc[leftc[i]];
    int rh = heightc[rightc[i]];
    // 左子树过高
    if (lh - rh > 1) {
        if (heightc[leftc[leftc[i]]] >= heightc[rightc[leftc[i]]]) {
            i = rightRotate(i);  // 单旋
        } else {
            leftc[i] = leftRotate(leftc[i]);  // 先左旋再右旋
            i = rightRotate(i);
        }
    }
    // 右子树过高
    else if (rh - lh > 1) {
        if (heightc[rightc[rightc[i]]] >= heightc[leftc[rightc[i]]]) {
            i = leftRotate(i);  // 单旋
        } else {
            rightc[i] = rightRotate(rightc[i]);  // 先右旋再左旋
            i = leftRotate(i);
        }
    }
    return i;
}

// 插入新节点（按 rank 插入：rank = 位置）
int addNode(int i, int rank, int num, int idx) {
    if (i == 0) {  // 新建节点
        ++cnt;
        key[cnt] = num;       // 存身高
        value[cnt] = idx;     // 存 k 值
        sizec[cnt] = heightc[cnt] = 1;
        leftc[cnt] = rightc[cnt] = 0;
        return cnt;
    }
    // 插到左子树
    if (sizec[leftc[i]] >= rank) {
        leftc[i] = addNode(leftc[i], rank, num, idx);
    }
    // 插到右子树（要减去左子树大小+1）
    else {
        rightc[i] = addNode(rightc[i], rank - sizec[leftc[i]] - 1, num, idx);
    }
    up(i);          // 更新当前节点信息
    return maintain(i);  // 保持平衡
}

// 插入一个人（num=身高，idx=位置）
void add(int num, int idx) {
    head = addNode(head, idx, num, idx);
}

// 中序遍历（得到最终队列）
void inOrder(vector<vector<int>>& ans, int i) {
    if (i == 0) return;
    inOrder(ans, leftc[i]);
    ans[fi][0] = key[i];
    ans[fi++][1] = value[i];
    inOrder(ans, rightc[i]);
}

void fillAns(vector<vector<int>>& ans) {
    fi = 0;
    inOrder(ans, head);
}

// 清空全局变量（方便多次调用）
void clearAll() {
    for (int i = 1; i <= cnt; i++) {
        key[i] = value[i] = leftc[i] = rightc[i] = sizec[i] = heightc[i] = 0;
    }
    cnt = 0;
    head = 0;
    fi = 0;
}


// 按照身高从大到小，k 从小到大排序，然后依次插入
vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
    sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] != b[0]) return a[0] > b[0];  // 身高降序
        return a[1] < b[1];                    // k 升序
    });

    for (auto &p : people) {
        add(p[0], p[1]);  // 插入到位置 p[1]
    }

    vector<vector<int>> ans(people.size(), vector<int>(2));
    fillAns(ans);   // 中序遍历得到答案
    clearAll();     // 清空全局变量，避免污染
    return ans;
}

int main() {
    vector<vector<int>> people = {{7,0},{4,4},{7,1},{5,0},{6,1},{5,2}};
    auto ans = reconstructQueue(people);
    for (auto &p : ans) {
        cout << "[" << p[0] << "," << p[1] << "] ";
    }
    cout << endl;
    return 0;
}
