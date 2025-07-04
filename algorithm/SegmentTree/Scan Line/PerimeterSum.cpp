#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

// 矩形周长并(洛谷测试)
// 测试链接 : https://www.luogu.com.cn/problem/P1856

const int MAXN = 20001;

// 存储n个矩形的坐标信息：[x1, y1, x2, y2]
int rec[MAXN][4];
// 用于坐标离散化的数组
int vsort[MAXN];

// 线段树节点信息
int length[MAXN << 2];  // 线段树某范围的实际长度（离散化后相邻点的距离）
int cover[MAXN << 2];   // 线段树某范围被覆盖的长度
int times[MAXN << 2];   // 线段树某范围被覆盖的次数

// 扫描线结构体
struct Line {
    int pos;    // 扫描线的位置（x坐标或y坐标）
    int start;  // 扫描线覆盖的起始位置
    int end;    // 扫描线覆盖的结束位置
    int val;    // 覆盖值：+1表示矩形左边界，-1表示矩形右边界

    // 排序规则：位置小的优先，位置相同时+1优先于-1
    bool operator<(const Line& other) const {
        if (pos != other.pos) return pos < other.pos;
        return val > other.val; // 先处理+1，再处理-1
    }
};

Line line[MAXN];

/**
 * 坐标离散化预处理
 * @param n 坐标点的数量
 * @return 离散化后不重复坐标的数量
 */
int prepare(int n) {
    sort(vsort + 1, vsort + n + 1);
    int m = 1;
    // 去重
    for (int i = 2; i <= n; i++) {
        if (vsort[m] != vsort[i]) {
            vsort[++m] = vsort[i];
        }
    }
    vsort[m + 1] = vsort[m];  // 哨兵，方便计算长度
    return m;
}

/**
 * 二分查找坐标在离散化数组中的位置
 * @param n 离散化数组的大小
 * @param num 要查找的坐标值
 * @return 坐标在离散化数组中的下标
 */
int rak(int n, int num) {
    int ans = 0;
    int l = 1, r = n, mid;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (vsort[mid] >= num) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

/**
 * 建立线段树
 * @param l 左边界
 * @param r 右边界
 * @param i 当前节点编号
 */
void build(int l, int r, int i) {
    if (l < r) {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);          // 建左子树
        build(mid + 1, r, i << 1 | 1);  // 建右子树
    }
    // 计算当前区间的实际长度
    length[i] = vsort[r + 1] - vsort[l];
    times[i] = 0;   // 初始覆盖次数为0
    cover[i] = 0;   // 初始覆盖长度为0
}

/**
 * 更新线段树节点的覆盖长度
 * @param i 当前节点编号
 */
void up(int i) {
    if (times[i] > 0) {
        // 如果当前区间被覆盖至少一次，则覆盖长度等于区间长度
        cover[i] = length[i];
    } else {
        // 否则覆盖长度等于左右子树覆盖长度之和
        cover[i] = cover[i << 1] + cover[i << 1 | 1];
    }
}

/**
 * 线段树区间修改
 * @param jobl 修改区间左边界
 * @param jobr 修改区间右边界
 * @param jobv 修改值（+1或-1）
 * @param l 当前区间左边界
 * @param r 当前区间右边界
 * @param i 当前节点编号
 */
void add(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        // 完全包含，直接修改覆盖次数
        times[i] += jobv;
    } else {
        // 部分包含，递归修改子树
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
    }
    up(i);  // 更新当前节点
}

/**
 * 扫描线算法核心函数
 * @param n 扫描线的数量
 * @return 周长的一部分（水平边或垂直边的总长度）
 */
long long scan(int n) {
    int m = prepare(n);  // 坐标离散化
    build(1, m, 1);      // 建立线段树

    // 排序扫描线：位置小的优先，同位置时+1优先于-1
    sort(line + 1, line + n + 1);

    long long ans = 0;
    for (int i = 1, pre; i <= n; i++) {
        pre = cover[1];  // 记录处理前的覆盖长度
        // 处理当前扫描线
        add(rak(m, line[i].start), rak(m, line[i].end) - 1, line[i].val, 1, m, 1);
        // 累加覆盖长度的变化量（即新增的周长）
        ans += abs(cover[1] - pre);
    }
    return ans;
}

/**
 * Y方向扫描：计算所有水平边的总长度
 * @param n 矩形数量
 * @return 水平边的总长度
 */
long long scanY(int n) {
    for (int i = 1, j = 1 + n, x1, y1, x2, y2; i <= n; i++, j++) {
        x1 = rec[i][0]; y1 = rec[i][1]; x2 = rec[i][2]; y2 = rec[i][3];
        vsort[i] = y1; vsort[j] = y2;  // Y坐标离散化
        // 左边界扫描线（+1）和右边界扫描线（-1）
        line[i] = {x1, y1, y2, 1};
        line[j] = {x2, y1, y2, -1};
    }
    return scan(n << 1);
}

/**
 * X方向扫描：计算所有垂直边的总长度
 * @param n 矩形数量
 * @return 垂直边的总长度
 */
long long scanX(int n) {
    for (int i = 1, j = 1 + n, x1, y1, x2, y2; i <= n; i++, j++) {
        x1 = rec[i][0]; y1 = rec[i][1]; x2 = rec[i][2]; y2 = rec[i][3];
        vsort[i] = x1; vsort[j] = x2;  // X坐标离散化
        // 下边界扫描线（+1）和上边界扫描线（-1）
        line[i] = {y1, x1, x2, 1};
        line[j] = {y2, x1, x2, -1};
    }
    return scan(n << 1);
}

/**
 * 计算矩形并集的周长
 * @param n 矩形数量
 * @return 矩形并集的周长
 */
long long compute(int n) {
    return scanY(n) + scanX(n);  // 水平边长度 + 垂直边长度
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i <= n; i++) {
        // 输入每个矩形的左下角和右上角坐标
        cin >> rec[i][0] >> rec[i][1];  // 左下角 (x1, y1)
        cin >> rec[i][2] >> rec[i][3];  // 右上角 (x2, y2)
    }

    // 输出矩形并集的周长
    cout << compute(n) << endl;
    return 0;
}
