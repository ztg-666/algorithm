#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>

// 牛群的才艺展示
// 一共有n只牛，每只牛有重量和才艺两个属性值
// 要求一定要选若干只牛，使得总重量不少于w，并且选出的牛，希望让
// 才艺的和 / 重量的和，这个比值尽量大
// 返回该比值 * 1000的整数结果，小数部分舍弃
// 1 <= n <= 250
// 1 <= w <= 1000
// 1 <= 牛的重量 <= 10^6
// 1 <= 牛的才艺 <= 10^3
// 测试链接 : https://www.luogu.com.cn/problem/P4377

using namespace std;

const int MAXN = 251;  // 最大牛的数量
const int MAXW = 1001;  // 最大重量
const double NA = -1e9;  // 表示无效解
const double sml = 1e-6; // 最小精度

int weight[MAXN];  // 牛的重量
int talent[MAXN];  // 牛的才艺
double value[MAXN];  // 牛的才艺 - x * 重量
double dp[MAXW];  // 动态规划数组，用于存储最大价值

int n, w;  // n表示牛的数量，w表示目标最小重量

// 检查当前的比值 x 是否可以满足要求
bool check(double x) {
    // 计算每只牛的才艺 - x * 重量
    for (int i = 1; i <= n; i++) {
        value[i] = talent[i] - x * weight[i];
    }

    // 初始化dp数组，dp[0]为0表示不选牛的情况下
    fill(dp, dp + w + 1, NA);  // 初始化dp数组为无效值
    dp[0] = 0;  // 不选牛，重量为0，才艺和为0

    // 动态规划更新：遍历每只牛，更新每个可能的重量
    for (int i = 1; i <= n; i++) {
        for (int p = w; p >= 0; p--) {  // 从后往前遍历，避免重复计算
            int j = p + weight[i];
            if (j >= w) {
                dp[w] = max(dp[w], dp[p] + value[i]);  // 选择牛，更新重量为w的最大值
            } else {
                dp[j] = max(dp[j], dp[p] + value[i]);  // 选择牛，更新相应的重量
            }
        }
    }

    // 如果能达到目标重量w，返回true
    return dp[w] >= 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> w;
    for (int i = 1; i <= n; i++) {
        cin >> weight[i] >> talent[i];
    }

    // 二分查找最优的比值
    double l = 0, r = 0, x;
    for (int i = 1; i <= n; i++) {
        r += talent[i];  // 最大的比值上界为所有才艺的总和
    }

    double ans = 0;
    // 二分查找，精度小于 sml 时结束
    while (r - l >= sml) {
        x = (l + r) / 2;  // 计算当前的中值
        if (check(x)) {
            ans = x;  // 如果当前比值可行，更新答案
            l = x + sml;  // 继续向右查找更大的比值
        } else {
            r = x - sml;  // 否则向左查找更小的比值
        }
    }

    // 输出最终结果，乘以1000并取整数部分
    cout << (int)(ans * 1000) << endl;

    return 0;
}
