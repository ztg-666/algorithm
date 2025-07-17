#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <climits>

// 增加限制的最长公共子序列问题
// 给定两个字符串s1和s2，s1长度为n，s2长度为m
// 返回s1和s2的最长公共子序列长度
// 注意：
// 两个字符串都只由小写字母组成
// 1 <= n <= 10^6
// 1 <= m <= 10^3

using namespace std;

const int MAXN = 1000005;  // 字符串s1的最大长度
const int MAXM = 1005;     // 字符串s2的最大长度
const int NA = INT_MAX;    // 表示不可达状态的标记值

int n, m;                  // 字符串s1和s2的长度
int rt[26];               // 记录每个字符在s1中从右到左第一次出现的位置
int nxt[MAXN][26];        // nxt[i][j]表示在s1中位置i之后字符j第一次出现的位置
int dp[MAXM][MAXM];       // 动态规划数组，用于记忆化搜索

/**
 * 生成指定长度的随机字符串
 * @param len 字符串长度
 * @return 生成的随机字符串
 */
string randomString(int len) {
    string s(len, 'a');
    for (int i = 0; i < len; i++) {
        s[i] = 'a' + rand() % 26;  // 生成a-z的随机字符
    }
    return s;
}

/**
 * 经典的LCS动态规划算法
 * 时间复杂度：O(n*m)
 * 空间复杂度：O(n*m)
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return 最长公共子序列的长度
 */
int lcs1(const string& s1, const string& s2) {
    int n = s1.length(), m = s2.length();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    // 状态转移：如果字符相同，长度+1；否则取两个方向的最大值
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
            } else {
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
            }
        }
    }
    return dp[n][m];
}

/**
 * 为优化的LCS算法构建预处理数组
 * @param s1 第一个字符串
 */
void build(const string& s1) {
    // 初始化rt数组为NA（不可达）
    fill(rt, rt + 26, NA);

    // 从右到左扫描s1，构建nxt数组
    // nxt[i][j]表示从位置i开始，字符j第一次出现的位置
    for (int i = n; i >= 0; i--) {
        for (int j = 0; j < 26; j++) {
            nxt[i][j] = rt[j];  // 复制当前字符的下一个出现位置
        }
        if (i > 0) {
            rt[s1[i-1] - 'a'] = i;  // 更新字符s1[i-1]的最近出现位置
        }
    }

    // 初始化dp数组为-1（未计算状态）
    for (int i = 0; i <= m; i++) {
        fill(dp[i], dp[i] + m + 1, -1);
    }
}

/**
 * 记忆化搜索函数
 * f(i, j)表示：在s2的前i个字符中选择j个字符，
 * 使得这j个字符在s1中按顺序出现的最小结束位置
 * @param i s2的前i个字符
 * @param j 需要选择的字符数量
 * @param s2 第二个字符串
 * @return 最小结束位置，如果不可能则返回NA
 */
int f(int i, int j, const string& s2) {
    if (i < j) return NA;  // 字符不够选择，不可能
    if (j == 0) return 0;  // 不选择任何字符，结束位置为0
    if (dp[i][j] != -1) return dp[i][j];  // 已计算过，直接返回

    // 选择1：不选择s2[i-1]
    int ans = f(i-1, j, s2);

    // 选择2：选择s2[i-1]
    int pre = f(i-1, j-1, s2);
    if (pre != NA) {
        // 在s1中找到s2[i-1]在位置pre之后的第一次出现
        ans = min(ans, nxt[pre][s2[i-1] - 'a']);
    }

    return dp[i][j] = ans;
}

/**
 * 优化的LCS算法
 * 当n >> m时，时间复杂度约为O(m^2)
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return 最长公共子序列的长度
 */
int lcs2(const string& s1, const string& s2) {
    n = s1.length();
    m = s2.length();
    build(s1);  // 预处理s1

    // 从最大可能长度开始尝试，找到第一个可行的长度
    for (int j = m; j >= 1; j--) {
        if (f(m, j, s2) != NA) {
            return j;
        }
    }
    return 0;  // 没有公共子序列
}

int main() {
    srand(time(nullptr));

    // 功能测试
    cout << "" << endl;
    for (int i = 0; i < 10000; i++) {
        string s1 = randomString(rand() % 100 + 1);
        string s2 = randomString(rand() % 100 + 1);
        if (lcs1(s1, s2) != lcs2(s1, s2)) {
            cout << "!" << endl;
        }
    }
    cout << "\n" << endl;

    // 性能测试
    cout << "" << endl;
    string s1 = randomString(1000000);
    string s2 = randomString(1000);

    clock_t start = clock();
    lcs1(s1, s2);
    cout << "lcs1 " << (double)(clock() - start) / CLOCKS_PER_SEC * 1000 << " " << endl;

    start = clock();
    lcs2(s1, s2);
    cout << "lcs2 " << (double)(clock() - start) / CLOCKS_PER_SEC * 1000 << " " << endl;
    cout << "" << endl;

    return 0;
}
