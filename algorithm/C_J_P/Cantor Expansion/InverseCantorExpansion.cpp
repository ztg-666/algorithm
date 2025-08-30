#include <bits/stdc++.h>

// 逆康托展开
// 数字从1到n，可以有很多排列，给定一个长度为n的数组s，表示具体的一个排列
// 求出这个排列的排名假设为x，打印第x+m名的排列是什么
// 1 <= n <= 10^5
// 1 <= m <= 10^15
// 题目保证s是一个由1~n数字组成的正确排列，题目保证x+m不会超过排列的总数
// 测试链接 : https://www.luogu.com.cn/problem/U72177

using namespace std;

const int MAXN = 100001;

long long arr[MAXN];      // 存放输入的排列，之后会被转化为阶乘进制
int sumv[MAXN << 2];      // 线段树节点值，表示区间内剩余可用数字个数
int n;                    // 排列长度
long long m;              // 要向后移动的位移量（排名偏移）

// 建立线段树，把 [l,r] 范围内的 sumv 初始化为 1
// 表示每个数都还没被使用
void build(int l, int r, int i) {
    if (l == r) {
        sumv[i] = 1;  // 单点叶子节点，初始可用
    } else {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);         // 左子树
        build(mid + 1, r, i << 1 | 1); // 右子树
        sumv[i] = sumv[i << 1] + sumv[i << 1 | 1];
    }
}

// 单点更新：把位置 jobi 的值加上 jobv
void add(int jobi, int jobv, int l, int r, int i) {
    if (l == r) {
        sumv[i] += jobv;
    } else {
        int mid = (l + r) >> 1;
        if (jobi <= mid) {
            add(jobi, jobv, l, mid, i << 1);
        } else {
            add(jobi, jobv, mid + 1, r, i << 1 | 1);
        }
        sumv[i] = sumv[i << 1] + sumv[i << 1 | 1];
    }
}

// 区间查询：返回 [jobl, jobr] 范围内有多少个可用数
int query(int jobl, int jobr, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        return sumv[i];
    }
    int mid = (l + r) >> 1;
    int ans = 0;
    if (jobl <= mid) ans += query(jobl, jobr, l, mid, i << 1);
    if (jobr > mid) ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
    return ans;
}

// 在当前可用数集合里找到“第k个数”，并删除它
// 返回的就是这个数的值
int getAndDelete(int k, int l, int r, int i) {
    int ans;
    if (l == r) {
        sumv[i]--;  // 删除这个数
        ans = l;    // 返回这个位置的值
    } else {
        int mid = (l + r) >> 1;
        if (sumv[i << 1] >= k) { // 第k个数在左子树
            ans = getAndDelete(k, l, mid, i << 1);
        } else { // 第k个数在右子树
            ans = getAndDelete(k - sumv[i << 1], mid + 1, r, i << 1 | 1);
        }
        sumv[i] = sumv[i << 1] + sumv[i << 1 | 1]; // 回溯更新
    }
    return ans;
}

void compute() {
    // Step1: 把当前排列转换成阶乘进制
    // build 线段树，表示所有数可用
    build(1, n, 1);
    for (int i = 1; i <= n; i++) {
        int x = (int) arr[i];
        if (x == 1) {
            arr[i] = 0;
        } else {
            // 计算有多少个比 x 小的数还没被使用过
            arr[i] = query(1, x - 1, 1, n, 1);
        }
        // 删除当前数 x，标记为已使用
        add(x, -1, 1, n, 1);
    }

    // Step2: 在阶乘进制数上加上 m
    arr[n] += m;
    for (int i = n; i >= 1; i--) {
        arr[i - 1] += arr[i] / (n - i + 1); // 进位
        arr[i] %= (n - i + 1);              // 当前位
    }

    // Step3: 把新的阶乘进制数转换回排列
    build(1, n, 1);
    for (int i = 1; i <= n; i++) {
        // arr[i] 是阶乘进制里的“第几个可用数”
        arr[i] = getAndDelete((int) arr[i] + 1, 1, n, 1);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    compute();

    // 输出最终排列
    for (int i = 1; i <= n; i++) {
        cout << arr[i] << " ";
    }
    cout << "\n";
    return 0;
}
