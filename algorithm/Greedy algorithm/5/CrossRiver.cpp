#include <iostream>
#include <algorithm> // 用于排序
using namespace std;

const int MAXN = 100001;    // 最大人数限制
int nums[MAXN];             // 存储每个人的渡河时间
int dp[MAXN];               // dp[i]表示前i+1个人过河的最短时间

// 计算最少渡河时间的动态规划函数
int minCost(int n) {
    // 按渡河时间升序排序，方便应用贪心策略
    sort(nums, nums + n);

    // 处理边界情况
    if (n >= 1) dp[0] = nums[0];        // 只有1人：直接渡河
    if (n >= 2) dp[1] = nums[1];        // 两人一起过河，取较大值(排序后nums[1]更大)
    if (n >= 3) dp[2] = nums[0] + nums[1] + nums[2]; // 三人策略：最快带最慢，返回，再带次慢

    // 动态规划处理4人及以上的情况
    for (int i = 3; i < n; ++i) {
        /* 两种贪心策略取最小值：
           策略1：最快者来回接送
           - 耗时 = 之前的最优解(dp[i-1])
                   + 最快者回来接人的时间(nums[0])
                   + 当前最慢者渡河时间(nums[i])

           策略2：两人组运输模式
           - 耗时 = 前i-2人的最优解(dp[i-2])
                   + 最快者带次快者过去(耗时nums[1])
                   + 最快者返回(耗时nums[0])
                   + 最慢两人组一起过河(耗时nums[i])
                   + 次快者返回(耗时nums[1])
        */
        dp[i] = min(
            dp[i-1] + nums[i] + nums[0],         // 策略1
            dp[i-2] + nums[0] + 2*nums[1] + nums[i] // 策略2
        );
    }
    return dp[n-1]; // 返回n个人的最优解
}

int main() {
    int n;
    while (cin >> n) {  // 循环读取测试用例
        // 读取渡河时间数据
        for (int i = 0; i < n; ++i) {
            cin >> nums[i];
        }
        // 处理空输入情况
        if (n == 0) {
            cout << 0 << endl;
            continue;
        }
        // 计算并输出结果
        cout << minCost(n) << endl;
    }
    return 0;
}
