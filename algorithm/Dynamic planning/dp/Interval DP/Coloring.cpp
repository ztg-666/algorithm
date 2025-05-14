#include <iostream>  // 用于标准输入输出流
#include <string>    // 用于字符串操作
#include <vector>    // 用于使用vector（动态数组）
#include <algorithm> // 用于 std::min
#include <climits>   // 用于 INT_MAX (虽然在这个特定实现中不是严格必需，因为ans在循环内初始化)

// 涂色 & 奇怪打印机
// 假设你有一条长度为5的木板，初始时没有涂过任何颜色
// 你希望把它的5个单位长度分别涂上红、绿、蓝、绿、红
// 用一个长度为5的字符串表示这个目标：RGBGR
// 每次你可以把一段连续的木板涂成一个给定的颜色，后涂的颜色覆盖先涂的颜色
// 例如第一次把木板涂成RRRRR
// 第二次涂成RGGGR
// 第三次涂成RGBGR，达到目标
// 返回尽量少的涂色次数
// 测试链接 : https://www.luogu.com.cn/problem/P4170
// 测试链接 : https://leetcode.cn/problems/strange-printer/

// 计算最少的打印（涂色）次数
// 参数 str: 输入的目标字符串的常量引用
// 返回值: 达到目标字符串所需的最少打印次数
int strangePrinter(const std::string& str) {
    int n = str.length(); // 获取字符串长度
    if (n == 0) {
        return 0; // 空字符串需要0次操作
    }

    // 初始化动态规划表 (DP table)
    // dp[l][r] 表示打印（涂色）完成子字符串 str[l...r] 所需的最少操作次数
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, 0));

    // 基本情况: 单个字符的子串
    // dp[i][i] = 1 (打印单个字符需要1次操作)
    dp[n - 1][n - 1] = 1;

    // 基本情况: 单个和两个字符的子串
    for (int i = 0; i < n - 1; ++i) {
        dp[i][i] = 1; // 单个字符需要1次操作
        // 两个字符: 如果相同则需要1次操作，不同则需要2次操作
        dp[i][i + 1] = (str[i] == str[i + 1]) ? 1 : 2;
    }

    // 填充DP表，处理长度为3及以上的子串
    // 按子串长度递增的顺序进行计算 (通过 l 递减, r 递增的循环实现)
    // l 代表子串起始索引，r 代表子串结束索引
    for (int l = n - 3; l >= 0; --l) {         // l 从 n-3 向 0 递减
        for (int r = l + 2; r < n; ++r) {      // r 从 l+2 向 n-1 递增 (确保长度 >= 3)
            // 计算 dp[l][r] 的值

            // 情况 1: 如果子串的首尾字符相同 (str[l] == str[r])
            // 那么打印 str[r] 的那次操作可以同时打印 str[l] (或者反过来理解)
            // 因此，打印 str[l...r] 的次数可以等于打印 str[l...r-1] 的次数
            // 或者等价地，等于打印 str[l+1...r] 的次数
            if (str[l] == str[r]) {
                dp[l][r] = dp[l][r - 1];
                // dp[l][r] = dp[l + 1][r]; // 这种写法也是正确的
            } else {
                // 情况 2: 子串的首尾字符不同
                // 必须在某个点 m 将问题分割成两个子问题 str[l...m] 和 str[m+1...r]
                // 需要找到所有可能的分割点中，总操作次数最少的那种分割方式
                int min_ops = INT_MAX; // 初始化当前子问题的最小操作次数为一个极大值
                for (int m = l; m < r; ++m) {
                    // 总操作次数 = 打印左部分(l..m)的次数 + 打印右部分(m+1..r)的次数
                    min_ops = std::min(min_ops, dp[l][m] + dp[m + 1][r]);
                }
                dp[l][r] = min_ops; // 将找到的最小操作次数存入dp表
            }
        }
    }

    // 最终答案是打印整个字符串 str[0...n-1] 所需的最少操作次数
    return dp[0][n - 1];
}

int main() {
    // 优化 C++ 的输入输出操作（在处理大量输入输出时可以提高效率）
    std::ios_base::sync_with_stdio(false); // 关闭C++标准流与C标准流的同步
    std::cin.tie(NULL);                    // 解除 cin 和 cout 的绑定
    std::cout.tie(NULL);                   // 解除 cout 的绑定

    std::string str;
    std::cin >> str; // 从标准输入读取字符串

    std::cout << strangePrinter(str) << std::endl; // 计算结果并输出到标准输出

    return 0; // 程序正常结束
}
