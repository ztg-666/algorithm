#include <iostream>     // 包含输入输出流库
#include <vector>       // 包含向量（动态数组）库
#include <string>       // 包含字符串库
#include <algorithm>    // 包含算法库（例如排序 std::sort, 最小值 std::min）
#include <vector>       // 包含向量库
#include <climits>      // 包含整型限制库（例如 INT_MAX）
#include <cstdlib>      // 包含通用库（例如 std::rand, std::srand）
#include <ctime>        // 包含时间库（例如 std::time）

// 使用 std 命名空间，避免重复写 std:: (在竞赛编程中常见)
using namespace std;

// 删除至少几个字符可以变成另一个字符串的子串
// 给定两个字符串s1和s2
// 返回s1至少删除多少字符可以成为s2的子串
// 对数器验证
// --- 暴力方法 (用于验证) ---

// 辅助函数，生成 s1 的所有子序列
// s1: 原始字符串 (使用 const 引用传递以提高效率)
// i: s1 中当前正在考虑的索引
// currentPath: 到目前为止构建的子序列
// list: 用于存储所有生成的子序列的向量 (通过引用传递)
void generateSubsequences(const string& s1, int i, string currentPath, vector<string>& list) {
    if (i == s1.length()) { // 如果到达字符串末尾
        list.push_back(currentPath); // 将当前路径（子序列）添加到列表中
    } else {
        // 选项 1: 不包含字符 s1[i]
        generateSubsequences(s1, i + 1, currentPath, list);
        // 选项 2: 包含字符 s1[i]
        generateSubsequences(s1, i + 1, currentPath + s1[i], list);
    }
}

// 暴力方法：生成 s1 的所有子序列，
// 按长度降序排序，并找到第一个（即最长的）
// 作为 s2 子串的那个子序列。
int minDelete1(const string& s1, const string& s2) {
    if (s1.empty()) {
        return 0; // 从空字符串删除 0 个字符得到空字符串，它是任何 s2 的子串
    }
    if (s2.empty()) {
         // 如果 s2 为空，s1 中唯一可能是 s2 子串的子序列是空字符串 ""。
         // 这需要删除 s1 的所有字符。
         return s1.length();
    }


    vector<string> subsequences; // 存储子序列的向量
    generateSubsequences(s1, 0, "", subsequences); // 生成 s1 的所有子序列

    // 按长度降序对子序列进行排序
    sort(subsequences.begin(), subsequences.end(), [](const string& a, const string& b) {
        return a.length() > b.length(); // 长度长的排前面
    });

    for (const string& sub : subsequences) {
        // 如果子串未找到，string::find 返回 string::npos
        // 并且确保子序列不为空，因为空子序列总是s2的子串（如果s2非空）
        if (!sub.empty() && s2.find(sub) != string::npos) {
            // 找到了 s1 的最长子序列，它同时也是 s2 的子串
            // 需要删除的字符数 = s1 长度 - 该子序列长度
            return s1.length() - sub.length();
        }
    }

    // 如果 s1 的所有非空子序列都不是 s2 的子串
    // (如果循环结束，也隐式包括了空子序列的情况）
    // 或者如果 s1 本身是空的（在顶部处理了），
    // 我们需要删除 s1 的所有字符以得到空字符串，
    // 空字符串技术上是 s2 的子串（除非 s2 也是空的）。
    // 唯一形成 *任何* s2 子串的方法可能需要删除所有内容。
    // 例如：如果 s2 是 "abc" 而 s1 是 "xyz"，s1 的任何子序列都不在 s2 中。
    // 我们需要从 s1 中删除所有 3 个字符。
     if (s2.find("") != string::npos) { // 空字符串总是任何非空字符串的子串，find("") 返回 0
         return s1.length(); // 需要删除 s1 的所有字符
     }
     // 理论上，如果 s2 非空，这个情况不应该到达，
     // 因为空子序列 "" 总是被生成，并且 s2.find("") 总是 0 (true)。
     // 如果 s2 是空的，已经在上面处理了。所以坚持返回 s1.length()。
     return s1.length();
}

// --- 动态规划方法 (官方解法) ---

// dp[i][j] 的含义：
// s1 的前 i 个字符组成的前缀 (str1[0...i-1])，
// 至少需要删除多少个字符，
// 才能变成 s2 的前 j 个字符组成的前缀 (str2[0...j-1]) 的 *某个后缀*。
int minDelete2(const string& str1, const string& str2) {
    int n = str1.length(); // str1 的长度
    int m = str2.length(); // str2 的长度

    // dp 表初始化: dp[i][j]
    // 维度是 (n+1) x (m+1)
    // dp[i][j] 代表 s1[前i个字符] 至少删多少字符可以变成 s2[前j个字符] 的某个后缀
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // 基础情况: 要使 s1 的前 i 个字符的前缀变成 s2 的空前缀 ("") 的后缀（也就是空串 ""），
    // 必须删除 s1 的所有 i 个字符。
    for (int i = 1; i <= n; ++i) {
        dp[i][0] = i;
    }
    // 基础情况 dp[0][j] = 0 由初始化隐式处理 (从空 s1 前缀删除 0 个字符)

    // 填充 DP 表
    for (int i = 1; i <= n; ++i) { // 遍历 s1 的前缀长度 i
        for (int j = 1; j <= m; ++j) { // 遍历 s2 的前缀长度 j
            // 如果当前字符匹配 (str1[i-1] 和 str2[j-1]，因为是 0-based 索引)
            // 我们不需要删除 str1[i-1]。成本与
            // s1 的前 i-1 个字符的前缀 变成 s2 的前 j-1 个字符的前缀的某个后缀 的成本 相同。
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // 如果字符不匹配，我们 *必须* 删除 str1[i-1]。
                // 成本是 1 (删除操作) +
                // s1 的前 i-1 个字符的前缀 变成 s2 的前 j 个字符的前缀的某个后缀 的成本。
                // 这里的含义是，str1[i-1]必须删掉，所以问题规模缩小为
                // str1[前i-1]至少删多少能变成str2[前j]的某个后缀，即dp[i-1][j]，再加1（删掉str1[i-1]）
                dp[i][j] = dp[i - 1][j] + 1;
            }
        }
    }

    // 最终答案是最后一行 (dp[n][j] for j=0 to m) 中的最小值。
    // 这代表从 *整个* s1 中需要进行的最小删除次数，
    // 使其成为 s2 *任意* 前缀的后缀 (这等价于成为 s2 的子串)。
    int ans = dp[n][0]; // 用匹配空后缀的情况初始化答案 (即删除所有 s1 字符)
    for (int j = 1; j <= m; ++j) {
        // 在 s1[前n] 变成 s2[前j] 的后缀所需删除数中找最小值
        ans = min(ans, dp[n][j]);
    }

     // 特殊情况：如果 s2 为空，唯一可能的子串是 ""，
     // 需要删除 s1 的所有字符。dp[n][0] 正确地捕捉了这一点。
     // 如果 s1 为空，n=0，循环不执行，ans=dp[0][0]=0，这是正确的。

    return ans; // 返回最小删除次数
}


// --- 验证工具 ---

// 生成长度为 n 的随机字符串，字符从 'a' 到 'a' + v - 1
string randomString(int n, int v) {
    if (n <= 0) return ""; // 处理无效长度
    string ans(n, ' '); // 创建长度为 n 的字符串，用空格初始化
    for (int i = 0; i < n; ++i) {
        ans[i] = (char)('a' + (rand() % v)); // 生成 'a' + 0 到 'a' + v-1 之间的随机字符
    }
    return ans;
}

// --- 用于测试的主函数 ---
int main() {
    // 初始化随机数生成器种子
    srand(time(0));

    int n = 12; // 字符串的最大长度 (为了暴力方法能跑完，设置得较小)
    int v = 3;  // 不同字符的数量 ('a', 'b', 'c')
    int testTime = 20000; // 测试次数

    cout << "start" << endl;
    bool error_found = false; // 标记是否发现错误
    for (int i = 0; i < testTime; ++i) {
        int len1 = (rand() % n) + 1; // 生成 1 到 n 之间的随机长度
        int len2 = (rand() % n) + 1; // 生成 1 到 n 之间的随机长度
        string s1 = randomString(len1, v); // 生成随机字符串 s1
        string s2 = randomString(len2, v); // 生成随机字符串 s2

        int ans1 = minDelete1(s1, s2); // 暴力方法结果
        int ans2 = minDelete2(s1, s2); // 动态规划方法结果

        if (ans1 != ans2) { // 如果两种方法结果不一致
            cout << "error!" << endl;
            cout << "s1: " << s1 << endl;
            cout << "s2: " << s2 << endl;
            cout << "way1" << ans1 << endl;
            cout << "way2" << ans2 << endl;
            error_found = true; // 标记发现错误
            break; // 发现第一个错误就停止
        }
    }

    if (!error_found) { // 如果没有发现错误
        cout << "yes" << testTime << " times" << endl;
    }
    cout << "end" << endl;

    return 0; // 程序正常退出
}
