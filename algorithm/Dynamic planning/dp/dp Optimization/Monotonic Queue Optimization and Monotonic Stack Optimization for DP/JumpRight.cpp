#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>

// 向右跳跃获得最大得分
// 给定长度为n+1的数组arr，下标编号0 ~ n，给定正数a、b
// 一开始在0位置，每次可以选择[a,b]之间的一个整数，作为向右跳跃的距离
// 每来到一个位置i，可以获得arr[i]作为得分，位置一旦大于n就停止
// 返回能获得的最大得分
// 1 <= n <= 2 * 10^5
// 1 <= a <= b <= n
// -1000 <= arr[i] <= +1000
// 测试链接 : https://www.luogu.com.cn/problem/P1725

using namespace std;

const int MAXN = 200001;  // 最大数组长度
const int NA = INT_MIN;   // 表示不可达状态的标记值

int arr[MAXN];      // 存储每个位置的得分
int dp[MAXN];       // dp[i]表示到达位置i能获得的最大得分
int queue_arr[MAXN]; // 单调队列，存储位置索引
int l, r;           // 队列的左右指针
int n, a, b;        // n为数组长度，a和b为跳跃距离的范围

// 向单调队列中添加位置j
void add(int j) {
    // 只有当j是有效位置且可达时才添加
    if (j >= 0 && dp[j] != NA) {
        // 维护单调递减队列：移除队尾所有小于等于dp[j]的元素
        while (l < r && dp[queue_arr[r - 1]] <= dp[j]) {
            r--;
        }
        // 将j加入队尾
        queue_arr[r++] = j;
    }
}

// 移除过期的位置t（超出跳跃范围的位置）
void overdue(int t) {
    // 如果队首元素就是过期位置t，则移除
    if (l < r && queue_arr[l] == t) {
        l++;
    }
}

// 计算最大得分的核心函数
int compute() {
    // 初始化：起始位置0的得分就是arr[0]
    dp[0] = arr[0];
    l = r = 0;  // 初始化队列指针

    // 从位置1开始遍历到位置n
    for (int i = 1; i <= n; i++) {
        // 添加可以跳到当前位置i的所有位置
        // 从位置(i-a)可以跳跃距离a到达位置i
        add(i - a);

        // 移除超出跳跃范围的位置
        // 从位置(i-b-1)跳跃距离b+1会超过当前位置i，所以要移除
        overdue(i - b - 1);

        // 计算到达位置i的最大得分
        // 如果队列非空，则取队首元素（最大值）加上当前位置得分
        dp[i] = (l < r) ? dp[queue_arr[l]] + arr[i] : NA;
    }

    // 计算最终答案：在所有可能的结束位置中选择最大值
    int ans = NA;
    // 从位置(n+1-b)到位置n都可能是最后一跳的起始位置
    for (int i = n + 1 - b; i <= n; i++) {
        ans = max(ans, dp[i]);
    }
    return ans;
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读取输入：数组长度n，跳跃距离范围[a,b]
    cin >> n >> a >> b;

    // 读取数组元素：从位置0到位置n的得分
    for (int i = 0; i <= n; i++) {
        cin >> arr[i];
    }

    // 输出最大得分
    cout << compute() << endl;

    return 0;
}
