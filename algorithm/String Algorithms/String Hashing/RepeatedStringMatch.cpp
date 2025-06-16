#include <string>
#include <vector>
#include <iostream> // For cerr if needed
#include <numeric> // For std::iota if needed, though not strictly required here

// 重复叠加字符串匹配
// 给定两个字符串a和b，寻找重复叠加字符串a的最小次数，使得字符串b成为叠加后的字符串a的子串
// 如果不存在则返回-1
// 字符串"abc"重复叠加0次是""
// 重复叠加1次是"abc"
// 重复叠加2次是"abcabc"
// 测试链接 : https://leetcode.cn/problems/repeated-string-match/

using namespace std;
// 根据 repeatedStringMatch 问题的实际约束，重新定义 MAXN。
// str1.length() <= 10^4, str2.length() <= 10^5
// 字符串 s 的最大有效长度约为 str2.length() + 2 * str1.length()
// 即 10^5 + 2 * 10^4 = 120000。
// 我们将其设置为 120005 以留有余地。
const int MAXN_OPTIMIZED = 120005; // 扩大的数组上限，以应对更大的输入规模
const long long MOD = 1e9 + 7;     // 选用一个大素数作为模数，防止溢出
const int base = 499;              // 哈希基数，保持不变

// 将全局数组的大小调整为 MAXN_OPTIMIZED
char s[MAXN_OPTIMIZED];
long long pow_arr[MAXN_OPTIMIZED];
long long hash_arr[MAXN_OPTIMIZED];

// 构建哈希数组和幂次数组，所有计算都进行模运算
void build(int n_len) { // n_len 是实际构造的字符串 s 的长度
    pow_arr[0] = 1;
    for (int i = 1; i < n_len; i++) {
        // 幂次计算： (前一项 * base) % MOD
        pow_arr[i] = (pow_arr[i - 1] * base) % MOD;
    }

    // 第一个字符的哈希值
    // 确保 (s[0] - 'a' + 1) 的结果在模数范围内，虽然通常是正数，但习惯性地加上模数再取模
    hash_arr[0] = (s[0] - 'a' + 1 + MOD) % MOD;
    for (int i = 1; i < n_len; i++) {
        // 哈希计算： (前一项哈希 * base + 当前字符值) % MOD
        // 注意：这里的 (s[i] - 'a' + 1) 已经是正数，但为了规范化，可以加上 MOD 再取模，确保结果始终在 [0, MOD-1)
        long long char_val = (s[i] - 'a' + 1);
        hash_arr[i] = (hash_arr[i - 1] * base + char_val) % MOD;
        // 如果结果是负数（虽然在这个具体表达式中不太可能，但为了通用性），可以写成：
        // hash_arr[i] = (hash_arr[i - 1] * base % MOD + char_val % MOD + MOD) % MOD;
        // 但对于正数相加，直接 % MOD 即可
    }
}

// 获取子字符串的哈希值，引入模运算并处理负数结果
long long get_hash(int l, int r) {
    if (l == 0) {
        return hash_arr[r];
    }
    // 计算子串哈希： H(r) - H(l-1) * base^(len)
    // 这里的 len = r - l + 1
    // 关键是模运算下的减法和乘法
    // 首先计算被减数部分： hash_arr[l - 1] * pow_arr[r - l + 1]
    long long subtracted_part = (hash_arr[l - 1] * pow_arr[r - l + 1]) % MOD;

    // 然后进行减法并确保结果非负： (A - B + MOD) % MOD
    return (hash_arr[r] - subtracted_part + MOD) % MOD;
}

int repeatedStringMatch(string str1, string str2) {
    if (str2.empty()) {
        return 0; // 如果 str2 是空串，0 次重复即可
    }
    if (str1.empty()) {
        return -1; // 如果 str1 是空串，str2 非空，无法匹配
    }

    int n = str1.length();
    int m = str2.length();

    // 计算 k，即 str1 至少需要重复多少次才能覆盖 str2 的长度
    // k = ceil(m/n)
    int k = (m + n - 1) / n;

    // 构造一个足够长的 s 字符串。通常，我们只需要考虑 str1 重复 k 次或 k+1 次的情况。
    // 在某些边界情况下，可能需要到 k+2 次。
    // 安全起见，我们将 s 的长度设置为能覆盖 str2.length() + 2 * str1.length()。
    // 这是一个经验性的上界，确保能找到所有可能的匹配。
    // 实际 len 应该由循环来控制，确保不会超出 MAXN_OPTIMIZED
    int len = 0;
    // 循环最多重复 k+2 次 str1，或者直到长度超过 m + 2*n
    // 这里采取了原始代码的 `cnt <= k` 循环，但如果 `len` 超过了 `MAXN_OPTIMIZED`，会出问题。
    // 更好的做法是直接构建到 `m + 2*n` 的长度。
    // 考虑到原始代码 `for (int cnt = 0; cnt <= k; cnt++)` 的逻辑，
    // 其生成的 `len` 最大约为 `(k+1)*n`。
    // 如果 `m = 10^5, n = 10^4`, 那么 `k = 10`, `len` 最大 `11 * 10^4 = 110000`。
    // 这在 `MAXN_OPTIMIZED = 120005` 范围内。
    // 理论上，只需要检查到 str1 重复 `q_times`，其中 `q_times = ceil(m/n) + 1` (或 +2)。
    // 这里我们构建的 s 已经包含了足够的重复次数。
    for (int cnt = 0; cnt <= k + 1; cnt++) { // 稍微增加重复次数的上限，更全面覆盖，确保str2+2*str1的长度
        // 每次将 str1 复制到 s 中
        for (int i = 0; i < n; i++) {
            s[len++] = str1[i];
        }
        if (len >= m + 2 * n) { // 如果构建的长度已经足够，提前停止
            break;
        }
    }


    // 构建哈希数组和幂次数组
    build(len);

    // 计算 str2 的哈希值，同样进行模运算
    long long h2 = 0;
    for (int i = 0; i < m; i++) {
        h2 = (h2 * base + (str2[i] - 'a' + 1)) % MOD;
    }

    // 滑动窗口查找匹配项
    for (int l = 0, r = m - 1; r < len; l++, r++) {
        if (get_hash(l, r) == h2) {
            // 如果找到匹配：
            // 判断需要的重复次数。如果匹配子串的结束位置 r 在 str1 重复 k 次的范围内，则返回 k。
            // 否则，说明需要 k+1 次（因为我们实际上构造了更多次的重复）
            // 更精确的判断是：结束位置 r 属于第几次重复的 str1。
            // (r / n) + 1 表示 str1 至少重复了多少次才包含了这个匹配的末尾。
            // k 是 str2 长度除以 str1 长度的向上取整。
            // 举例：str1="a", str2="aaa", n=1, m=3, k=3。
            // s="aaaa...", r=2 (for "aaa"). r/n + 1 = 2/1 + 1 = 3. 返回 3。
            // 举例：str1="ab", str2="bab", n=2, m=3, k=2。
            // s="ababab...", "bab" -> l=1, r=3. r/n + 1 = 3/2 + 1 = 1 + 1 = 2. 返回 2。
            // 这里我们需要返回的是 str1 重复的最小次数。
            // 因为我们构建的 `s` 包含了 `str1` 最多 `(k+2)` 次的重复，
            // 实际需要的次数是覆盖 `str2` 的最小重复次数。
            // `(r / n)` 是从 0 开始的重复块索引。
            // 例如，如果 r=n-1, 表示匹配在第一个 `str1` 块内，`r/n=0`，需要 1 次。
            // 如果 r=2n-1, 表示匹配在第二个 `str1` 块内，`r/n=1`，需要 2 次。
            // 因此，实际需要的重复次数是 `(r / n) + 1`。
            // 而 `k` 是 `ceil(m/n)`。如果 `(r/n)+1` 等于 `k`，则 `k` 次。
            // 如果 `(r/n)+1` 等于 `k+1`，则 `k+1` 次。
            // 理论上，如果 `str2` 被 `str1` 包含，它可以在 `k` 或 `k+1` 次重复中被找到。
            // 那么返回 `(r / n) + 1` 就能得到正确的重复次数。
            // 原始代码的 `r < n * k ? k : k + 1` 是一个简洁的判断，它考虑了两种情况。
            // 结合 (r/n)+1 的解释，如果 r < n*k，那么 r 落在 k 次重复的总长度范围内，结果是 k。
            // 否则是 k+1。这个逻辑是正确的。
            return (r / n) + 1; // 返回实际需要的重复次数
        }
    }

    return -1; // 未找到匹配
}

