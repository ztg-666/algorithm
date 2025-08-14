#include <bits/stdc++.h>

// 装备购买
// 一共有n个物品，每个物品都有m个属性值
// 下面定义什么是不必要的物品：如果已经选择了k个物品，此时又有一件当前物品
// 如果给已经选择的物品分配一组相乘的系数，并把属性值相加，就能得到当前物品
// 那么就说当前物品是不必要的，比如下面的例子
// a = { 4, 6, 2 }, b = { 2, 8, 4 }, c = { 6, 19, 9 }
// a * 0.5 + b * 2 = c，那么c物品是不必要的
// 每个物品都有价格，现在希望尽量多的购买物品，但不能出现不必要的物品
// 返回最多能买几件物品和最少的花费
// 1 <= n、m <= 500
// 0 <= 属性值 <= 1000
// 测试链接 : https://www.luogu.com.cn/problem/P3265

using namespace std;

const int MAXN = 502;      // 最大物品数
const int MAXM = 502;      // 最大属性数
const double sml = 1e-5;   // 判断浮点数是否为0的阈值

int n, m;                  // n = 物品数量, m = 属性维度
int cnt, cost;             // cnt = 选中物品个数, cost = 总价格
int basis[MAXM];           // basis[j] 表示线性基在第 j 维的主元来自哪个物品

// 物品结构体：存储 m 维属性 + 最后一列是价格
struct Item {
    double attr[MAXM];  // 下标 1..m 是属性，attr[m+1] 是价格
};

Item items[MAXN];  // 物品数组

// 尝试把当前物品 cur 插入线性基
bool insert_item(Item &cur, int idx) {
    for (int j = 1; j <= m; j++) {
        // 找到当前向量在第 j 维的非零分量（模阈值）
        if (fabs(cur.attr[j]) >= sml) {
            // 如果该维没有主元，则把当前物品放入基
            if (basis[j] == 0) {
                basis[j] = idx;
                return true;  // 成功插入
            }
            // 否则用已存在的基向量消去该分量
            double rate = cur.attr[j] / items[basis[j]].attr[j];
            for (int k = j; k <= m; k++) {
                cur.attr[k] -= rate * items[basis[j]].attr[k];
            }
        }
    }
    // 如果 cur 完全被消掉（说明是线性相关），插入失败
    return false;
}

// 主计算过程
void compute() {
    cnt = 0;
    cost = 0;
    // 按价格升序排序，优先插入便宜的物品
    sort(items + 1, items + n + 1, [&](const Item &a, const Item &b) {
        return a.attr[m + 1] < b.attr[m + 1];
    });
    // 初始化线性基
    fill(basis, basis + m + 1, 0);
    // 依次尝试插入物品
    for (int i = 1; i <= n; i++) {
        if (insert_item(items[i], i)) {
            cnt++;  // 可插入的物品数 +1
            cost += (int)items[i].attr[m + 1];  // 累加价格（向下取整）
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入物品属性
    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            cin >> items[i].attr[j];
        }
    }
    // 输入物品价格
    for (int i = 1; i <= n; i++) {
        cin >> items[i].attr[m + 1]; // 价格存储在最后一列
    }

    // 计算最大可选物品数及总价格
    compute();
    cout << cnt << " " << cost << "\n";
    return 0;
}
