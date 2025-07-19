#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

// 粉刷木板的最大收益
// 一共有n个木板，每个木板长度为1，最多粉刷一次，也可以不刷
// 一共有m个工人，每个工人用(li, pi, si)表示：
// 该工人必须刷连续区域的木板，并且连续的长度不超过li
// 该工人每刷一块木板可以得到pi的钱
// 该工人刷的连续区域必须包含si位置的木板
// 返回所有工人最多能获得多少钱
// 1 <= n <= 16000
// 1 <= m <= 100
// 1 <= pi <= 10000
// 测试链接 : http://poj.org/problem?id=1821

using namespace std;

const int MAXN = 16001;  // 木板数量的最大值
const int MAXM = 101;    // 工人数量的最大值

// 工人结构体
struct Worker {
    int l;  // 该工人最多能刷的连续木板长度
    int p;  // 该工人每刷一块木板得到的钱
    int s;  // 该工人必须刷包含第s号木板的连续区域
};

int dp[MAXM][MAXN];      // dp[i][j]表示前i个工人刷前j块木板能获得的最大收益
int queue_arr[MAXN];     // 单调队列，存储状态转移时的候选位置
int l_ptr, r_ptr;        // 单调队列的左右指针
int n, m;                // n为木板数量，m为工人数量

// 比较函数：按工人必须包含的位置s进行排序
bool compareWorkers(const Worker& a, const Worker& b) {
    return a.s < b.s;
}

// 计算以j号木板结尾时的状态转移指标值
// 这是单调队列优化的关键函数
int value(int i, int pi, int j) {
    return dp[i - 1][j] - pi * j;
}

// 主计算函数
int compute(vector<Worker>& workers) {
    // 按工人必须包含的位置排序，确保状态转移的正确性
    sort(workers.begin() + 1, workers.end(), compareWorkers);

    // 遍历每个工人
    for (int i = 1; i <= m; i++) {
        int li = workers[i].l;  // 当前工人最多刷的长度
        int pi = workers[i].p;  // 当前工人每块木板的收益
        int si = workers[i].s;  // 当前工人必须包含的位置

        // 初始化单调队列
        l_ptr = r_ptr = 0;

        // 预处理单调队列：将所有可能作为当前工人起始位置的候选加入队列
        // 如果当前工人要包含si位置，且最多刷li长度，那么起始位置范围是[max(0, si-li), si-1]
        for (int j = max(0, si - li); j < si; j++) {
            // 维护单调队列的单调性：移除队尾所有不优秀的状态
            while (l_ptr < r_ptr && value(i, pi, queue_arr[r_ptr - 1]) <= value(i, pi, j)) {
                r_ptr--;
            }
            queue_arr[r_ptr++] = j;  // 将当前位置加入队列
        }

        // 状态转移：计算dp[i][j]
        for (int j = 1; j <= n; j++) {
            // 不使用当前工人，继承之前的最优解
            dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);

            // 如果当前位置j >= si，说明当前工人可以在此位置结束工作
            if (j >= si) {
                // 维护单调队列：移除过期的状态
                // 如果队列头部的位置已经超出了当前工人的工作范围，就移除它
                if (l_ptr < r_ptr && queue_arr[l_ptr] == j - li - 1) {
                    l_ptr++;
                }

                // 如果队列非空，使用队列头部的最优状态进行转移
                if (l_ptr < r_ptr) {
                    // 状态转移方程：dp[i][j] = max(dp[i][j], dp[i-1][k] + pi*(j-k))
                    // 其中k是工人i的起始位置，简化后就是value(i, pi, k) + pi*j
                    dp[i][j] = max(dp[i][j], value(i, pi, queue_arr[l_ptr]) + pi * j);
                }
            }
        }
    }

    // 返回所有工人刷完所有木板能获得的最大收益
    return dp[m][n];
}

int main() {
    // 优化输入输出速度
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    // 处理多组测试数据
    while (cin >> n >> m) {
        vector<Worker> workers(m + 1);  // 工人数组，从下标1开始使用

        // 读入每个工人的信息
        for (int i = 1; i <= m; i++) {
            cin >> workers[i].l >> workers[i].p >> workers[i].s;
        }

        // 初始化dp数组为0
        memset(dp, 0, sizeof(dp));

        // 输出结果
        cout << compute(workers) << endl;
    }

    return 0;
}
