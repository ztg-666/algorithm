#include <iostream>
#include <vector>
#include <cstring>


// 相邻不同色的染色方法数(轮廓线dp+空间压缩)
// 给定两个参数n和m，表示n行m列的空白区域，一开始所有格子都没有颜色
// 给定参数k，表示有k种颜色，颜色编号0~k-1
// 你需要给每个格子染色，但是相邻的格子颜色不能相同
// 相邻包括上、下、左、右四个方向
// 并且给定了第0行和第n-1行的颜色状况，输入保证一定有效
// 那么你只能在1~n-2行上染色，返回染色的方法数，答案对376544743取模
// 2 <= k <= 4
// k = 2时，1 <= n <= 10^7，1 <= m <= 10^5
// 3 <= k <= 4时，1 <= n <= 100，1 <= m <= 8
// 测试链接 : https://www.luogu.com.cn/problem/P2435

using namespace std;

const int LIMIT1 = 100001;   // 最大行数限制
const int LIMIT2 = 8;        // 最大列数限制
const int MOD = 376544743;   // 取模数

int start[LIMIT1];           // 第0行的颜色状态
int ed[LIMIT1];              // 第n-1行的颜色状态
int dp[LIMIT2 + 1][1 << (LIMIT2 << 1)];  // 动态规划数组，dp[j][s]表示当前行第j列状态为s的方案数
int prepare[1 << (LIMIT2 << 1)];          // 预处理数组，存储上一行的状态

int startStatus, edStatus;   // 起始行和结束行的状态压缩表示
int n, m, k, maxs;          // n行m列k种颜色，maxs为最大状态数

// 获取状态s中第j列的颜色值
// 每个位置用2位表示颜色（0-3），所以第j列的颜色在第(j<<1)位开始
int get(int s, int j) {
    return (s >> (j << 1)) & 3;
}

// 设置状态s中第j列的颜色为v
// 先清空原来的值，再设置新值
int set(int s, int j, int v) {
    return (s & (~(3 << (j << 1)))) | (v << (j << 1));
}

// 判断两个状态a和b是否在每一列都不同（相邻行不能有相同颜色）
bool different(int a, int b) {
    for (int j = 0; j < m; j++) {
        if (get(a, j) == get(b, j)) {
            return false;
        }
    }
    return true;
}

// k=2时的特殊情况优化
// 当k=2时，相邻行的颜色必须不同，可以直接判断
int special() {
    if ((n & 1) == 0) {  // n为偶数时，第0行和第n-1行应该不同
        for (int i = 0; i < m; i++) {
            if (start[i] == ed[i]) {
                return 0;
            }
        }
    } else {  // n为奇数时，第0行和第n-1行应该相同
        for (int i = 0; i < m; i++) {
            if (start[i] != ed[i]) {
                return 0;
            }
        }
    }
    return 1;
}

// 主要的动态规划计算函数
int compute() {
    // 将起始和结束状态压缩成整数
    startStatus = edStatus = 0;
    for (int j = 0; j < m; j++) {
        startStatus = set(startStatus, j, start[j]);
        edStatus = set(edStatus, j, ed[j]);
    }

    // 初始化prepare数组：只有与结束状态不同的状态才有效
    for (int s = 0; s < maxs; s++) {
        prepare[s] = different(s, edStatus) ? 1 : 0;
    }

    // 从第n-2行开始向上逐行计算
    for (int i = n - 2; i >= 1; i--) {
        // 边界情况：j == m时，直接使用prepare数组的值
        for (int s = 0; s < maxs; s++) {
            dp[m][s] = prepare[s];
        }

        // 轮廓线DP：从右到左处理每一列
        for (int j = m - 1; j >= 0; j--) {
            for (int s = 0; s < maxs; s++) {
                int ans = 0;
                // 尝试每种颜色
                for (int color = 0; color < k; color++) {
                    // 检查约束条件：
                    // 1. 如果不是第0列，当前颜色不能与左边相同
                    // 2. 当前颜色不能与上一行相同位置的颜色相同
                    if ((j == 0 || get(s, j - 1) != color) && get(s, j) != color) {
                        // 更新状态并累加方案数
                        ans = (ans + dp[j + 1][set(s, j, color)]) % MOD;
                    }
                }
                dp[j][s] = ans;
            }
        }

        // 更新prepare数组为当前行的结果，供下一轮使用
        for (int s = 0; s < maxs; s++) {
            prepare[s] = dp[0][s];
        }
    }

    // 返回从起始状态开始的方案数
    return dp[0][startStatus];
}

int main() {
    ios::sync_with_stdio(false);  // 加速输入输出
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> m >> k;
    maxs = 1 << (m << 1);  // 计算最大状态数：每列2位，共m列

    // 读取第0行的颜色状态
    for (int i = 0; i < m; i++) {
        cin >> start[i];
    }

    // 读取第n-1行的颜色状态
    for (int i = 0; i < m; i++) {
        cin >> ed[i];
    }

    // 根据k值选择不同的处理方式
    if (k == 2) {
        cout << special() << endl;  // k=2时使用特殊优化
    } else {
        cout << compute() << endl;  // k>2时使用通用DP
    }
    
    return 0;
}
