#include <iostream>
#include <vector>
#include <cstring>

// 种草的方法数(轮廓线dp)
// 给定一个n*m的二维网格grid
// 网格里只有0、1两种值，0表示该田地不能种草，1表示该田地可以种草
// 种草的时候，任何两个种了草的田地不能相邻，相邻包括上、下、左、右四个方向
// 你可以随意决定种多少草，只要不破坏上面的规则即可
// 返回种草的方法数，答案对100000000取模
// 1 <= n, m <= 12
// 测试链接 : https://www.luogu.com.cn/problem/P1879

using namespace std;

const int MAXN = 12;       // 最大行数
const int MAXM = 12;       // 最大列数
const int MOD = 100000000; // 取模值

int grid[MAXN][MAXM];          // 输入的二维网格，0表示不能种草，1表示可以种草
int dp[MAXN][MAXM][1 << MAXM]; // 记忆化搜索的dp数组
int n, m, maxs;                // n行数，m列数，maxs = 2^m 表示状态总数

// 获取状态s中第j位的值（0或1）
int get(int s, int j)
{
    return (s >> j) & 1;
}

// 设置状态s中第j位的值为v（0或1）
int set(int s, int j, int v)
{
    return v == 0 ? (s & (~(1 << j))) : (s | (1 << j));
}

/**
 * 轮廓线动态规划的核心函数
 * @param i 当前处理的行号
 * @param j 当前处理的列号
 * @param s 轮廓线状态：
 *          - 对于i-1行的[j..m-1]列：表示这些位置是否种草
 *          - 对于i行的[0..j-1]列：表示这些位置是否种草
 * @return 从当前状态开始的种草方法数
 */
int f(int i, int j, int s)
{
    // 递归终止条件：处理完所有行，返回1种方法
    if (i == n)
    {
        return 1;
    }

    // 当前行处理完毕，进入下一行
    if (j == m)
    {
        return f(i + 1, 0, s);
    }

    // 记忆化搜索：如果已经计算过，直接返回结果
    if (dp[i][j][s] != -1)
    {
        return dp[i][j][s];
    }

    // 选择1：当前位置不种草
    int ans = f(i, j + 1, set(s, j, 0));

    // 选择2：当前位置种草（需要满足约束条件）
    if (grid[i][j] == 1 &&                // 当前位置可以种草
        (j == 0 || get(s, j - 1) == 0) && // 左边没有种草（或者是第一列）
        get(s, j) == 0)
    { // 上面没有种草
        ans = (ans + f(i, j + 1, set(s, j, 1))) % MOD;
    }

    // 记录计算结果
    dp[i][j][s] = ans;
    return ans;
}

// 计算总的种草方法数
int compute()
{
    // 初始化dp数组为-1，表示未计算
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            for (int s = 0; s < maxs; s++)
            {
                dp[i][j][s] = -1;
            }
        }
    }
    // 从第0行第0列开始，初始轮廓线状态为0
    return f(0, 0, 0);
}

int main()
{
    // 读取网格大小
    cin >> n >> m;
    maxs = 1 << m; // 计算状态总数：2^m

    // 读取网格数据
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> grid[i][j];
        }
    }

    // 输出结果
    cout << compute() << endl;

    return 0;
}
