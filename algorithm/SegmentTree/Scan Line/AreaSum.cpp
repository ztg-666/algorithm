#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 300001;

// 矩形结构体：左下角(x1, y1)，右上角(x2, y2)
struct Rectangle {
    int x1, y1, x2, y2;
};

// 扫描线结构体：x为当前扫描位置，y1~y2为覆盖区间，flag表示是加入(+1)还是删除(-1)
struct Line {
    int x, y1, y2, flag;
};

vector<Rectangle> rec(MAXN);    // 存储矩形
vector<Line> line(MAXN);        // 存储扫描线
vector<int> ysort(MAXN);        // 存储所有y坐标（用于离散化）

// 线段树节点信息
vector<int> length(MAXN << 2);  // 每个区间的实际长度（由ysort计算）
vector<int> cover(MAXN << 2);   // 每个区间被覆盖的长度
vector<int> times(MAXN << 2);   // 每个区间被覆盖的次数

// 对y坐标进行离散化并去重
int prepare(int n) {
    sort(ysort.begin() + 1, ysort.begin() + n + 1);
    int m = 1;
    for (int i = 2; i <= n; i++) {
        if (ysort[m] != ysort[i]) {
            ysort[++m] = ysort[i];
        }
    }
    ysort[m + 1] = ysort[m]; // 哨兵值
    return m;
}

// 在离散化后的数组中查找 y 值对应的下标位置
int rak(int n, int num) {
    int ans = 0;
    int l = 1, r = n, mid;
    while (l <= r) {
        mid = (l + r) >> 1;
        if (ysort[mid] >= num) {
            ans = mid;
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return ans;
}

// 构建线段树，初始化长度
void build(int l, int r, int i) {
    if (l < r) {
        int mid = (l + r) >> 1;
        build(l, mid, i << 1);
        build(mid + 1, r, i << 1 | 1);
    }
    length[i] = ysort[r + 1] - ysort[l]; // 区间长度为 ysort[r+1] - ysort[l]
    times[i] = 0;
    cover[i] = 0;
}

// push-up 更新父节点的cover值
void up(int i) {
    if (times[i] > 0) {
        cover[i] = length[i]; // 当前区间完全被覆盖
    } else {
        cover[i] = cover[i << 1] + cover[i << 1 | 1]; // 合并子区间
    }
}

// 修改线段树中的区间覆盖次数
void add(int jobl, int jobr, int jobv, int l, int r, int i) {
    if (jobl <= l && r <= jobr) {
        times[i] += jobv;
    } else {
        int mid = (l + r) >> 1;
        if (jobl <= mid) {
            add(jobl, jobr, jobv, l, mid, i << 1);
        }
        if (jobr > mid) {
            add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
        }
    }
    up(i); // 每次修改后都要更新父节点
}

// 计算矩形并面积
long long compute(int n) {
    // 处理每个矩形，拆分为两条竖线（加入线和删除线）
    for (int i = 1, j = 1 + n; i <= n; i++, j++) {
        int x1 = rec[i].x1, y1 = rec[i].y1, x2 = rec[i].x2, y2 = rec[i].y2;
        ysort[i] = y1;
        ysort[j] = y2;
        line[i] = {x1, y1, y2, 1};   // 左边界：加入
        line[j] = {x2, y1, y2, -1};  // 右边界：删除
    }

    n <<= 1; // 现在有 2n 条线
    int m = prepare(n); // 离散化y坐标
    build(1, m, 1);     // 构建线段树

    // 按照 x 坐标排序扫描线
    sort(line.begin() + 1, line.begin() + n + 1, [](const Line& a, const Line& b) {
        return a.x < b.x;
    });

    long long ans = 0;
    for (int i = 1, pre = 0; i <= n; i++) {
        // 在当前线和上一条线之间的 x 差值 * 当前总覆盖 y 长度
        ans += (long long)cover[1] * (line[i].x - pre);
        pre = line[i].x;

        // 更新线段树：在对应y区间内增加/减少覆盖
        add(rak(m, line[i].y1), rak(m, line[i].y2) - 1, line[i].flag, 1, m, 1);
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int i = 1; i <= n; i++) {
        cin >> rec[i].x1 >> rec[i].y1 >> rec[i].x2 >> rec[i].y2;
    }

    cout << compute(n) << endl;

    return 0;
}
