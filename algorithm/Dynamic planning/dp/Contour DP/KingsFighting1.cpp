#include <iostream>
#include <vector>
#include <cstring>


// 摆放国王的方法数(轮廓线dp)
// 给定两个参数n和k，表示n*n的区域内要摆放k个国王
// 国王可以攻击临近的8个方向，所以摆放时不能让任何两个国王打架
// 返回摆放的方法数
// 1 <= n <= 9
// 1 <= k <= n*n
// 测试链接 : https://www.luogu.com.cn/problem/P1896

using namespace std;

// 最大棋盘大小和最大国王数量
const int MAXN = 9;
const int MAXK = 82;

// 五维DP数组：dp[行][列][轮廓线状态][左上角状态][剩余国王数]
// 表示在当前状态下的摆放方法数
long long dp[MAXN][MAXN][1 << MAXN][2][MAXK];
int n, kings, maxs;  // n为棋盘大小，kings为国王总数，maxs为状态数量(2^n)

// 获取状态s中第j位的值（0或1）
int get(int s, int j) {
    return (s >> j) & 1;
}

// 设置状态s中第j位的值为v
int set(int s, int j, int v) {
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

// 轮廓线动态规划函数
// i: 当前行
// j: 当前列
// s: 轮廓线状态，表示：
//    - i-1行中[j..n-1]列的摆放情况
//    - i行中[0..j-1]列的摆放情况
// leftup: 左上角位置(i-1,j-1)是否有国王
// k: 剩余需要摆放的国王数量
long long f(int i, int j, int s, int leftup, int k) {
    // 边界条件：已经处理完所有行
    if (i == n) {
        return k == 0 ? 1 : 0;  // 如果恰好用完所有国王，返回1种方法，否则0种
    }

    // 当前行处理完毕，进入下一行
    if (j == n) {
        return f(i + 1, 0, s, 0, k);
    }

    // 记忆化搜索：如果已经计算过，直接返回结果
    if (dp[i][j][s][leftup][k] != -1) {
        return dp[i][j][s][leftup][k];
    }

    // 获取当前位置相邻位置的状态
    int left = j == 0 ? 0 : get(s, j - 1);     // 左边位置是否有国王
    int up = get(s, j);                         // 上面位置是否有国王
    int rightup = get(s, j + 1);               // 右上角位置是否有国王

    // 情况1：当前位置不放国王
    // 更新轮廓线状态：将当前位置设为0，leftup更新为up
    long long ans = f(i, j + 1, set(s, j, 0), up, k);

    // 情况2：当前位置放国王
    // 条件：还有国王可放，且四个相邻位置都没有国王
    if (k > 0 && left == 0 && leftup == 0 && up == 0 && rightup == 0) {
        // 更新轮廓线状态：将当前位置设为1，leftup更新为up，国王数减1
        ans += f(i, j + 1, set(s, j, 1), up, k - 1);
    }

    // 缓存结果
    dp[i][j][s][leftup][k] = ans;
    return ans;
}

// 计算摆放方法数的主函数
long long compute() {
    // 初始化DP数组为-1（表示未计算）
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int s = 0; s < maxs; s++) {
                for (int leftup = 0; leftup <= 1; leftup++) {
                    for (int k = 0; k <= kings; k++) {
                        dp[i][j][s][leftup][k] = -1;
                    }
                }
            }
        }
    }

    // 从(0,0)位置开始，初始轮廓线状态为0，没有左上角国王，需要摆放kings个国王
    return f(0, 0, 0, 0, kings);
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入：棋盘大小n和国王数量kings
    cin >> n >> kings;
    maxs = 1 << n;  // 计算状态总数，即2^n

    // 输出结果
    cout << compute() << endl;
    
    return 0;
}
