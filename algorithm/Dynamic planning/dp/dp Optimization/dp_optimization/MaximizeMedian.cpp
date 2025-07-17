#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>

// 感谢热心的同学，找到了题目5的在线测试
// 最大平均值和中位数
// 给定一个长度为n的数组arr，现在要选出一些数
// 满足 任意两个相邻的数中至少有一个数被选择
// 被选中的数字平均值的最大值，打印的答案为double类型，误差在0.001以内
// 被选中的数字中位数的最大值，打印的答案为int类型，中位数认为是上中位数
// 2 <= n <= 10^5
// 1 <= arr[i] <= 10^9
// 测试链接 : https://atcoder.jp/contests/abc236/tasks/abc236_e

using namespace std;

const int MAXN = 100005;  // 数组最大长度
int n;                    // 数组实际长度
vector<int> arr(MAXN);    // 输入数组

// 求最大平均数需要的辅助数组和DP表
vector<double> help1(MAXN);                      // 存储arr[i] - x的结果
vector<vector<double>> dp1(MAXN, vector<double>(2));  // DP表，dp1[i][0]表示第i个位置不选，dp1[i][1]表示第i个位置选

// 求最大上中位数需要的辅助数组和DP表
vector<int> sorted_arr(MAXN);                    // 排序后的数组
vector<int> help2(MAXN);                         // 存储>=x为1，<x为-1的结果
vector<vector<int>> dp2(MAXN, vector<int>(2));  // DP表，dp2[i][0]表示第i个位置不选，dp2[i][1]表示第i个位置选

/**
 * 检查是否能达到平均值x
 * 思路：将所有数字减去x，如果能选出一些数使得和>=0，说明平均值至少为x
 * @param x 要检查的平均值
 * @return 是否能达到平均值x
 */
bool check1(double x) {
    // 将arr中所有的数字都减去x，得到的数字填入help1
    for (int i = 1; i <= n; i++) {
        help1[i] = (double)arr[i] - x;
    }

    // 使用动态规划求解：任意两个相邻的数中至少有一个数被选择的最大和
    // dp1[i][0]: 从第i个位置到末尾，第i个位置不选择的最大和
    // dp1[i][1]: 从第i个位置到末尾，第i个位置选择的最大和
    dp1[n + 1][0] = dp1[n + 1][1] = 0;  // 边界条件：超出数组范围的位置和为0

    // 从后往前填充DP表
    for (int i = n; i >= 1; i--) {
        // 第i个位置不选择：可以选择下一个位置，也可以不选择下一个位置
        dp1[i][0] = max(help1[i] + dp1[i + 1][0], dp1[i + 1][1]);
        // 第i个位置选择：下一个位置可以不选择
        dp1[i][1] = help1[i] + dp1[i + 1][0];
    }

    // 如果从第1个位置开始的最大和>=0，说明平均值至少为x
    return dp1[1][0] >= 0;
}

/**
 * 使用二分查找求最大平均数
 * 思路：平均值必然在[min_val, max_val]范围内，使用二分查找找到最大可能的平均值
 * @return 最大平均值
 */
double average() {
    double l = INT_MAX, r = INT_MIN, m;  // l为左边界，r为右边界，m为中点

    // 找到数组中的最小值和最大值作为二分查找的范围
    for (int i = 1; i <= n; i++) {
        l = min(l, (double)arr[i]);
        r = max(r, (double)arr[i]);
    }

    // 二分查找60次，足够让误差小于0.001
    for (int i = 1; i <= 60; i++) {
        m = (l + r) / 2;  // 取中点
        if (check1(m)) {
            l = m;  // 如果能达到平均值m，说明最大平均值至少为m，在右半部分继续查找
        } else {
            r = m;  // 如果不能达到平均值m，说明最大平均值小于m，在左半部分继续查找
        }
    }
    return l;
}

/**
 * 检查是否能选出足够多的数使得上中位数至少为x
 * 思路：将>=x的数设为1，<x的数设为-1，如果能选出和>0的数，说明上中位数至少为x
 * @param x 要检查的中位数值
 * @return 是否能达到中位数x
 */
bool check2(int x) {
    // 将>=x的数设为1，<x的数设为-1
    for (int i = 1; i <= n; i++) {
        help2[i] = arr[i] >= x ? 1 : -1;
    }

    // 使用动态规划求解：任意两个相邻的数中至少有一个数被选择的最大和
    // dp2[i][0]: 从第i个位置到末尾，第i个位置不选择的最大和
    // dp2[i][1]: 从第i个位置到末尾，第i个位置选择的最大和
    dp2[n + 1][0] = dp2[n + 1][1] = 0;  // 边界条件

    // 从后往前填充DP表
    for (int i = n; i >= 1; i--) {
        // 第i个位置不选择：可以选择下一个位置，也可以不选择下一个位置
        dp2[i][0] = max(help2[i] + dp2[i + 1][0], dp2[i + 1][1]);
        // 第i个位置选择：下一个位置可以不选择
        dp2[i][1] = help2[i] + dp2[i + 1][0];
    }

    // 如果从第1个位置开始的最大和>0，说明选出的数中>=x的数多于<x的数
    return dp2[1][0] > 0;
}

/**
 * 使用二分查找求最大上中位数
 * 思路：对数组排序后，中位数必然是数组中的某个值，使用二分查找找到最大可能的中位数
 * @return 最大上中位数
 */
int median() {
    // 复制数组并排序
    for (int i = 1; i <= n; i++) {
        sorted_arr[i] = arr[i];
    }
    sort(sorted_arr.begin() + 1, sorted_arr.begin() + n + 1);

    int l = 1, r = n, m, ans = 0;  // l为左边界，r为右边界，m为中点，ans为答案

    // 二分查找最大的能作为中位数的值
    while (l <= r) {
        m = (l + r) / 2;  // 取中点
        if (check2(sorted_arr[m])) {
            ans = sorted_arr[m];  // 如果sorted_arr[m]能作为中位数，更新答案
            l = m + 1;           // 在右半部分继续查找更大的值
        } else {
            r = m - 1;           // 如果不能作为中位数，在左半部分继续查找
        }
    }
    return ans;
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读取输入
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出结果
    cout << fixed << setprecision(10) << average() << endl;  // 输出最大平均值，保留10位小数
    cout << median() << endl;                                // 输出最大上中位数

    return 0;
}
