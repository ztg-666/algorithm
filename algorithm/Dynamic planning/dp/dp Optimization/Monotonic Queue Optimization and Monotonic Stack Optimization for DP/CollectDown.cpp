#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <climits>

// 向下收集获得最大能量
// 有一个n * m的区域，行和列的编号从1开始
// 每个能量点用(i, j, v)表示，i行j列上有价值为v的能量点
// 一共有k个能量点，并且所有能量点一定在不同的位置
// 一开始可以在第1行的任意位置，然后每一步必须向下移动
// 向下去往哪个格子是一个范围，如果当前在(i, j)位置
// 那么往下可以选择(i+1, j-t)...(i+1, j+t)其中的一个格子
// 到达最后一行时，收集过程停止，返回能收集到的最大能量价值
// 1 <= n、m、k、t <= 4000
// 1 <= v <= 100
// 测试链接 : https://www.luogu.com.cn/problem/P3800

using namespace std;

const int MAXN = 4001;  // 最大行数
const int MAXM = 4001;  // 最大列数

int dp[MAXN][MAXM];     // dp[i][j] 表示到达第i行第j列时能收集到的最大能量
int queue_arr[MAXM];    // 单调队列，存储列的索引
int l, r;               // 单调队列的左右指针
int n, m, k, t;         // n行数, m列数, k个能量点, t移动范围

// 初始化dp数组
void build() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            dp[i][j] = 0;
        }
    }
}

// 向单调队列中添加元素
// 维护单调递减队列，队头是当前窗口的最大值
void add(int i, int j) {
    if (j <= m) {
        // 如果新加入的元素比队尾元素大或相等，则移除队尾元素
        // 保证队列单调递减
        while (l < r && dp[i][queue_arr[r - 1]] <= dp[i][j]) {
            r--;
        }
        queue_arr[r++] = j;  // 将新元素加入队尾
    }
}

// 移除过期元素
// 当窗口左边界移动时，需要检查队头元素是否过期
void overdue(int t) {
    if (l < r && queue_arr[l] == t) {
        l++;  // 移除队头过期元素
    }
}

// 计算最大能量值
int compute() {
    // 从第2行开始计算（第1行可以任意选择起始位置）
    for (int i = 2; i <= n; i++) {
        l = r = 0;  // 重置单调队列

        // 初始化窗口：对于第j=1列，其可达范围是[1-t, 1+t]
        // 但由于列索引从1开始，所以先加入[1, t]范围内的元素
        for (int j = 1; j <= t; j++) {
            add(i - 1, j);
        }

        // 遍历当前行的每一列
        for (int j = 1; j <= m; j++) {
            // 扩展窗口右边界：添加新进入窗口的元素
            add(i - 1, j + t);

            // 收缩窗口左边界：移除离开窗口的元素
            overdue(j - t - 1);

            // 状态转移：当前位置的最大能量 = 原有能量 + 上一行可达范围内的最大能量
            dp[i][j] += dp[i - 1][queue_arr[l]];
        }
    }

    // 找到最后一行中的最大值
    int ans = INT_MIN;
    for (int j = 1; j <= m; j++) {
        ans = max(ans, dp[n][j]);
    }
    return ans;
}

int main() {
    // 快速输入输出优化
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 读取基本参数
    cin >> n >> m >> k >> t;
    build();  // 初始化dp数组

    // 读取k个能量点的位置和价值
    for (int i = 1; i <= k; i++) {
        int r, c, v;  // 行、列、能量值
        cin >> r >> c >> v;
        dp[r][c] = v;  // 在对应位置设置能量值
    }

    // 输出结果
    cout << compute() << endl;

    return 0;
}
