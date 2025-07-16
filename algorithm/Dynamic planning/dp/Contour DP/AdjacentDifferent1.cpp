#include <iostream>
#include <vector>
#include <cstring>

// 相邻不同色的染色方法数(轮廓线dp)
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

// 常量定义
const int LIMIT1 = 100001;  // 最大列数限制
const int LIMIT2 = 101;     // 最大行数限制
const int LIMIT3 = 8;       // 最大列数限制（用于状态压缩）
const int MOD = 376544743;  // 模数

// 全局变量
int start[LIMIT1];  // 第0行的颜色状况
int ed[LIMIT1];     // 第n-1行的颜色状况
int dp[LIMIT2][LIMIT3][1 << (LIMIT3 << 1)];  // 记忆化搜索的缓存数组

int startStatus, edStatus;  // 起始状态和结束状态的压缩表示
int n, m, k, maxs;          // n行m列，k种颜色，maxs为状态总数

/**
 * 特殊情况：当k=2时的优化处理
 * 对于只有2种颜色的情况，相邻格子必须不同色
 * 如果行数n为偶数，则第0行和第n-1行每个位置的颜色必须不同
 * 如果行数n为奇数，则第0行和第n-1行每个位置的颜色必须相同
 */
int special() {
    if ((n & 1) == 0) {  // n为偶数
        for (int i = 0; i < m; i++) {
            if (start[i] == ed[i]) {  // 如果相同则无解
                return 0;
            }
        }
    } else {  // n为奇数
        for (int i = 0; i < m; i++) {
            if (start[i] != ed[i]) {  // 如果不同则无解
                return 0;
            }
        }
    }
    return 1;  // 有解，方案数为1
}

/**
 * 从状态s中提取第j列的颜色
 * 每个颜色用2位二进制表示，所以第j列的颜色在第(j<<1)位开始
 */
int get(int s, int j) {
    return (s >> (j << 1)) & 3;  // 右移j*2位，然后取最低2位
}

/**
 * 在状态s中设置第j列的颜色为v
 * 先清除原来的颜色，再设置新颜色
 */
int set(int s, int j, int v) {
    return (s & (~(3 << (j << 1)))) | (v << (j << 1));
    // s & (~(3 << (j << 1))) 清除第j列的原颜色
    // | (v << (j << 1)) 设置第j列的新颜色
}

/**
 * 判断两个状态a和b是否在每一列都不同
 * 用于检查相邻行之间是否满足不同色的要求
 */
bool different(int a, int b) {
    for (int j = 0; j < m; j++) {
        if (get(a, j) == get(b, j)) {  // 如果某列颜色相同
            return false;
        }
    }
    return true;  // 所有列都不同
}

/**
 * 动态规划主函数（记忆化搜索）
 * i: 当前处理的行号
 * j: 当前处理的列号
 * s: 轮廓线状态，表示当前位置周围已确定的颜色状况
 *    - 对于第i行第j列，s的高位存储第i-1行[j...m-1]列的颜色
 *    - s的低位存储第i行[0...j-1]列的颜色
 * 返回：从当前状态开始的染色方案数
 */
int f(int i, int j, int s) {
    // 边界条件：到达最后一行，检查是否与结束状态匹配
    if (i == n - 1) {
        return different(s, edStatus) ? 1 : 0;
    }

    // 当前行处理完毕，转到下一行
    if (j == m) {
        return f(i + 1, 0, s);
    }

    // 记忆化：如果已经计算过，直接返回结果
    if (dp[i][j][s] != -1) {
        return dp[i][j][s];
    }

    int ans = 0;
    // 尝试所有可能的颜色
    for (int color = 0; color < k; color++) {
        // 检查约束条件：
        // 1. 与左边格子颜色不同（如果j>0）
        // 2. 与上边格子颜色不同
        if ((j == 0 || get(s, j - 1) != color) && get(s, j) != color) {
            // 递归计算，更新状态s
            ans = (ans + f(i, j + 1, set(s, j, color))) % MOD;
        }
    }

    // 记忆化存储结果
    dp[i][j][s] = ans;
    return ans;
}

/**
 * 计算染色方案数的主函数
 */
int compute() {
    // 初始化起始状态和结束状态
    startStatus = edStatus = 0;
    for (int j = 0; j < m; j++) {
        startStatus = set(startStatus, j, start[j]);
        edStatus = set(edStatus, j, ed[j]);
    }

    // 初始化记忆化数组
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            for (int s = 0; s < maxs; s++) {
                dp[i][j][s] = -1;  // -1表示未计算
            }
        }
    }

    // 从第1行第0列开始计算（第0行已给定）
    return f(1, 0, startStatus);
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 读取输入
    cin >> n >> m >> k;
    maxs = 1 << (m << 1);  // 状态总数 = 2^(m*2)，每列用2位表示颜色

    // 读取第0行的颜色
    for (int i = 0; i < m; i++) {
        cin >> start[i];
    }

    // 读取第n-1行的颜色
    for (int i = 0; i < m; i++) {
        cin >> ed[i];
    }

    // 根据颜色数量选择算法
    if (k == 2) {
        cout << special() << endl;  // k=2时使用特殊优化
    } else {
        cout << compute() << endl;  // k>2时使用轮廓线DP
    }
    
    return 0;
}
