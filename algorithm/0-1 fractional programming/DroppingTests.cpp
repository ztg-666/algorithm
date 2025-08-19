#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

// 01分数规划模版题
// 给定n个数据，每个数据有(a, b)两个值，都为整数，并且都是非负的
// 请舍弃掉k个数据，希望让剩下数据做到，所有a的和 / 所有b的和，这个比值尽量大
// 如果剩下数据所有b的和为0，认为无意义
// 最后，将该比值 * 100，小数部分四舍五入的整数结果返回
// 1 <= n <= 100
// 0 <= a、b <= 10^9
// 测试链接 : https://www.luogu.com.cn/problem/P10505
// 测试链接 : http://poj.org/problem?id=2976

using namespace std;

struct Data {
    long long a, b;
    double diff;  // 用来存储 (a - x * b)
};
const int MAXN = 1000 + 10;
Data arr[MAXN];
// 比较器，用于按 diff 排序
bool compare(const Data &d1, const Data &d2) {
    return d1.diff > d2.diff;
}

bool check(double x, int n, int k) {
    // 计算每个数据的结余
    for (int i = 1; i <= n; i++) {
        arr[i].diff = arr[i].a - x * arr[i].b;
    }

    // 按照结余排序
    sort(arr + 1, arr + n + 1, compare);

    // 计算前 k 个数据的结余和
    double sum = 0;
    for (int i = 1; i <= k; i++) {
        sum += arr[i].diff;
    }

    return sum >= 0;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n, k;
    while (cin >> n >> k&& (n != 0 || k != 0)) {
        // 实际上是保留 n-k 个数据
        k = n - k;



        // 输入数据 a 和 b
        for (int i = 1; i <= n; i++) {
            cin >> arr[i].a;
        }
        for (int i = 1; i <= n; i++) {
            cin >> arr[i].b;
        }

        double l = 0, r = 0, x;
        for (int i = 1; i <= n; i++) {
            r += arr[i].a;  // r = sum of all a
        }

        double ans = 0;
        while (r - l >= 1e-6 && l < r) {  // 精度要求
            x = (l + r) / 2;
            if (check(x, n, k)) {
                ans = x;
                l = x + 1e-6;  // 保证收敛
            } else {
                r = x - 1e-6;
            }
        }

        // 输出答案，四舍五入并乘以 100
        cout << static_cast<int>(100 * ans + 0.5) << "\n";
    }

    return 0;
}
