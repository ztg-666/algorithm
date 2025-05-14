#include <iostream>  // 用于标准输入输出流
#include <string>    // 用于字符串操作
#include <vector>    // 用于使用vector（动态数组）
#include <algorithm> // 用于 std::min
#include <climits>   // 用于 INT_MAX

// 计算让字符串s在[l...r]范围内的括号正确配对，至少需要插入多少个字符
// 参数 s: 输入字符串的常量引用
// 参数 l: 子串的起始索引
// 参数 r: 子串的结束索引
// 参数 dp: 动态规划表（二维vector），用于记忆化搜索，通过引用传递
// 返回值: s[l...r]配对所需的最少插入字符数
int f(const std::string& s, int l, int r, std::vector<std::vector<int>>& dp) {
    // 基本情况 1: 单个字符
    if (l == r) {
        return 1; // 需要插入一个字符来配对
    }
    // 基本情况 2: 两个字符
    if (l == r - 1) {
        // 如果这两个字符本身就配对
        if ((s[l] == '(' && s[r] == ')') || (s[l] == '[' && s[r] == ']')) {
            return 0; // 已经配对，无需插入
        }
        return 2; // 否则需要插入两个字符
    }
    // l...r 范围内的字符数量 >= 3

    // 记忆化搜索检查：如果dp[l][r]已经计算过，直接返回结果
    if (dp[l][r] != -1) {
        return dp[l][r];
    }

    // 可能性 1: s[l] 和 s[r] 本身就匹配
    int p1 = INT_MAX; // 初始化为一个非常大的值
    if ((s[l] == '(' && s[r] == ')') || (s[l] == '[' && s[r] == ']')) {
        // 如果最外层的字符匹配，则递归计算内部子串 s[l+1...r-1] 的结果
        p1 = f(s, l + 1, r - 1, dp);
    }

    // 可能性 2: 在 l 和 r 之间的每个可能的点 m 进行分割
    int p2 = INT_MAX; // 初始化为一个非常大的值
    for (int m = l; m < r; m++) {
        // 计算分割后的成本：左部分s[l...m]的成本 + 右部分s[m+1...r]的成本
        // 取所有分割点中成本最小的那个
        p2 = std::min(p2, f(s, l, m, dp) + f(s, m + 1, r, dp));
    }

    // 最终答案是以上两种可能性中的最小值
    int ans = std::min(p1, p2);
    dp[l][r] = ans; // 将计算结果存入dp表
    return ans;
}

// 计算整个字符串所需的最少插入字符数
// 参数 str: 输入的完整字符串
// 返回值: 使整个字符串括号配对所需的最少插入数
int compute(const std::string& str) {
    int n = str.length(); // 获取字符串长度
    if (n == 0) {
        return 0; // 空字符串不需要插入
    }
    // 初始化DP表，大小为 n x n，所有值设为 -1 (表示尚未计算)
    std::vector<std::vector<int>> dp(n, std::vector<int>(n, -1));
    // 调用递归函数计算 s[0...n-1] 的结果
    return f(str, 0, n - 1, dp);
}

int main() {
    // 优化 C++ 的输入输出操作（在处理大量输入输出时可以提高效率）
    std::ios_base::sync_with_stdio(false); // 关闭C++标准流与C标准流的同步
    std::cin.tie(NULL);                    // 解除 cin 和 cout 的绑定
    std::cout.tie(NULL);                   // 解除 cout 的绑定

    std::string str;
    std::cin >> str; // 从标准输入读取字符串
    std::cout << compute(str) << std::endl; // 计算结果并输出到标准输出

    return 0; // 程序正常结束
}
