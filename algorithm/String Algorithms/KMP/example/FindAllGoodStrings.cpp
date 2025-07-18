#include <cstring> // 提供 memset 等内存操作函数
#include <utility>
#include <vector>  // 虽然代码中没有直接使用，但通常用于动态数组，此处可能作为通用习惯引入
#include <string>  // 用于处理字符串类型

// 找到所有好字符串
// 给你两个长度为n的字符串s1和s2，以及一个字符串evil
// 好字符串的定义为 : 长度为n，字典序大于等于s1，字典序小于等于s2，且不包含evil字符串
// 返回好字符串的数量
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/find-all-good-strings/

using namespace std; // 方便使用标准库中的函数和类型

// 定义常量，提升代码可读性和修改便利性
const int MOD = 1000000007; // 模数，用于结果取模，防止整数溢出
const int MAXN = 501;       // 字符串的最大长度 N，用于 dp 数组的第一维
const int MAXM = 51;        // 模式串的最大长度 M，用于 dp 数组的第二维

// dp 数组：动态规划的核心状态存储
// dp[i][j][free] 表示：
//   i: 当前正在考虑字符串的第 i 个字符 (从左往右，0 到 n-1)
//   j: 模式串 e 当前匹配到的最长前缀的长度 (KMP状态)
//   free: 0 表示当前前缀受到 s1 或 s2 的限制 (还未选择比限制更小的字符)，1 表示当前前缀已经比限制小，后续字符可以随意选择 ('a' 到 'z')
// 值为从当前状态 (i, j, free) 到字符串末尾，不包含模式串 e 的有效字符串数量
int dp[MAXN][MAXM][2];

// nxt 数组：KMP算法中的“部分匹配表”或“失配函数”
// nxt[k] 表示模式串 e 的前 k 个字符中，最长的真前缀和真后缀相等的长度。
// 当模式串匹配失败时，可以根据 nxt 数组快速跳转到下一个可能匹配的位置，避免从头开始比较。
int nxt[MAXM]; // 避免关键字冲突，此处变量名修改为 nxt

// clear 函数：初始化 dp 数组
// 将 dp 数组的所有元素初始化为 -1，表示该状态尚未计算
// 这样在 f 函数中可以通过检查 dp[i][j][free] 是否为 -1 来判断是否已计算过，避免重复计算（记忆化搜索）
void clear(int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dp[i][j][0] = -1;
            dp[i][j][1] = -1;
        }
    }
}

// buildNextArray 函数：构建 KMP 的 nxt 数组
// 输入参数 e: 模式串，m: 模式串的长度
void buildNextArray(string e, int m) {
    nxt[0] = -1; // KMP约定，当 j=0 时，下一个匹配位置为 -1 (通常表示从头开始比较)
    if (m > 1) nxt[1] = 0; // KMP约定，当 j=1 时，下一个匹配位置为 0
    int i = 2, cn = 0; // i: 当前计算 nxt 数组的索引，cn: 当前已匹配的长度
    while (i < m) {
        // 如果当前字符 e[i-1] 等于已匹配的字符 e[cn]
        if (e[i - 1] == e[cn]) {
            nxt[i++] = ++cn; // nxt[i] 设置为 cn+1，并且 i 和 cn 都向前推进
        }
        // 如果当前字符不匹配，并且 cn > 0 (表示之前有匹配的前缀)
        else if (cn > 0) {
            cn = nxt[cn]; // 通过 nxt 数组回溯，尝试更短的前缀匹配
        }
        // 如果当前字符不匹配，且 cn == 0 (表示没有匹配的前缀，或者回溯到头了)
        else {
            nxt[i++] = 0; // nxt[i] 设置为 0，并且 i 向前推进
        }
    }
}

// kmp 函数：经典的 KMP 字符串查找算法
// 用于判断字符串 s 是否包含模式串 e
// 输入参数 s: 主字符串，e: 模式串，n: 主字符串长度，m: 模式串长度
// 返回值: 如果找到模式串 e，返回其在 s 中结束的位置索引 (0-based)；否则返回 -1
int kmp(string s, string e, int n, int m) {
    int x = 0, y = 0; // x: s 的当前索引，y: e 的当前索引
    while (x < n && y < m) {
        if (s[x] == e[y]) { // 字符匹配，两者都向前推进
            x++;
            y++;
        } else if (y == 0) { // 字符不匹配，且模式串已回溯到起始位置，主字符串向前推进
            x++;
        } else { // 字符不匹配，但模式串有已匹配前缀，通过 nxt 数组回溯
            y = nxt[y];
        }
    }
    return y == m ? x - y : -1; // 如果 y 等于模式串长度 m，表示模式串已完全匹配，返回其结束位置；否则返回 -1
}

// jump 函数：KMP状态转移的核心逻辑，在数位DP中用于计算下一个匹配状态
// 当我们选择一个字符 pick 时，根据 KMP 的原理，更新当前匹配模式串 e 的长度 j
// 输入参数 pick: 当前选择的字符，e: 模式串，j: 当前匹配模式串 e 的长度
// 返回值: 选择 pick 后，新的匹配长度
int jump(char pick, string e, int j) {
    // 循环：当 j 大于等于 0 且当前选择的字符 pick 不等于模式串 e 的第 j 个字符时
    // (注意：这里 KMP 状态 j 对应的是模式串 e[0...j-1] 已经匹配成功，所以要和 e[j] 比较)
    while (j >= 0 && pick != e[j]) {
        j = nxt[j]; // 通过 nxt 数组回溯，寻找更短的匹配前缀
    }
    return j; // 返回回溯后的 j 值
}

// f 函数：数位DP的递归函数（记忆化搜索实现）
// 核心思想是计算从当前状态 (i, j, free) 开始，到字符串末尾，不包含模式串 e 的有效字符串数量
// 输入参数 s: 参考字符串 (s1 或 s2)，e: 模式串
// n: 目标字符串总长度，m: 模式串长度
// i: 当前正在构建字符串的第 i 位 (从 0 开始)
// j: 当前已匹配模式串 e 的前缀长度 (KMP状态)
// free: 0 表示受 s 的限制，1 表示无限制 (可以随意选择 'a' 到 'z')
// 返回值: 从当前状态 (i, j, free) 到字符串末尾，不包含模式串 e 的字符串数量
int f(string s, string e, int n, int m, int i, int j, int free) {
    // 边界条件 1: 如果模式串 e 已经完全匹配 (j == m)
    // 说明当前路径上已经包含了模式串 e，这种情况下是不符合要求的，返回 0
    if (j == m) return 0;

    // 边界条件 2: 如果已经构建完长度为 n 的字符串 (i == n)
    // 说明成功构建了一个不包含模式串 e 的字符串，返回 1 (表示找到一个有效解)
    if (i == n) return 1;

    // 记忆化搜索: 如果当前状态已经计算过，直接返回之前的结果，避免重复计算
    if (dp[i][j][free] != -1) return dp[i][j][free];

    char cur = s[i]; // 获取参考字符串 s 在当前位 i 的字符，用于处理上限限制
    long long ans = 0; // 用于累计结果，使用 long long 防止中间结果溢出

    // 根据 free 状态分类讨论
    if (free == 0) { // 当前前缀受到 s 的限制 (还未选择比 s[i] 更小的字符)
        // 1. 选择比 s[i] 小的字符
        for (char pick = 'a'; pick < cur; pick++) {
            // 如果选择了比 cur 小的字符 pick，那么从下一位开始，free 状态变为 1 (表示无限制)
            // jmp(pick, e, j) + 1 是 KMP 中匹配长度的更新，j + 1 是因为 KMP 的 j 状态是匹配到 e[j-1]，
            // 下一个字符如果匹配，则匹配长度变为 j+1。这里 jump 函数已经处理了回溯，
            // 它的返回值就是新的匹配长度，所以 +1 表示将 pick 纳入匹配后新的匹配长度。
            ans = (ans + f(s, e, n, m, i + 1, jump(pick, e, j) + 1, 1)) % MOD;
        }
        // 2. 选择与 s[i] 相同的字符
        // 如果选择与 cur 相同的字符，那么从下一位开始，free 状态仍然保持 0 (继续受 s 的限制)
        ans = (ans + f(s, e, n, m, i + 1, jump(cur, e, j) + 1, 0)) % MOD;
    } else { // 当前前缀已经比 s 小，后续字符可以随意选择 ('a' 到 'z')
        // 遍历所有可能的字符 'a' 到 'z'
        for (char pick = 'a'; pick <= 'z'; pick++) {
            // free 状态始终保持 1
            ans = (ans + f(s, e, n, m, i + 1, jump(pick, e, j) + 1, 1)) % MOD;
        }
    }
    // 将计算结果存入 dp 数组，并返回
    return dp[i][j][free] = (int)ans;
}

// findGoodStrings 函数：主函数，计算符合条件的字符串数量
// 输入参数 n: 字符串长度，s1: 范围下限，s2: 范围上限，e: 模式串
// 返回值: 范围 [s1, s2] 内，不包含模式串 e 的字符串数量
int findGoodStrings(int n, string s1, string s2, string e) {
    int m = e.length(); // 获取模式串 e 的长度

    // 1. 构建 KMP 的 nxt 数组
    buildNextArray(e, m);

    // 2. 计算 [0, s2] 范围内不包含模式串 e 的字符串数量
    // 先清空 dp 数组
    clear(n, m);
    // 调用 f 函数，从第 0 位开始，初始匹配长度为 0，free 状态为 0 (受 s2 限制)
    int ans = f(s2, e, n, m, 0, 0, 0);

    // 3. 计算 [0, s1-1] 范围内不包含模式串 e 的字符串数量，并从总数中减去
    // 再次清空 dp 数组
    clear(n, m);
    // 调用 f 函数，从第 0 位开始，初始匹配长度为 0，free 状态为 0 (受 s1 限制)
    // 这里的 ans 是 [0, s2] 的，减去 f(s1, e, ...) 得到的是 (s1, s2] 区间的
    ans = (ans - f(s1, e, n, m, 0, 0, 0) + MOD) % MOD; // 减法结果可能是负数，因此需要加 MOD 再取模

    // 4. 特殊处理 s1 本身
    // 因为 f(s1) 计算的是 (0, s1-1] 区间内的数量，不包含 s1 本身。
    // 所以我们需要单独检查 s1 是否符合条件（不包含模式串 e）。
    // 如果 s1 不包含模式串 e (kmp 返回 -1)，那么 s1 自身是一个“好”字符串，需要将其加回到结果中。
    if (kmp(s1, e, n, m) == -1) {
        ans = (ans + 1) % MOD;
    }
    return ans; // 返回最终结果
}
