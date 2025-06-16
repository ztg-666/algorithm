#include <iostream>  // 用于输入输出
#include <string>    // 用于处理字符串
#include <vector>    // 虽然这里没有直接用到，但通常在C++竞赛中是标配

// 根据匹配定义求匹配子串的数量
// 给定长为n的字符串s，以及长度为m的字符串p，还有一个正数k
// s'与s匹配的定义为，s'与s长度相同，且最多有k个位置字符不同
// 要求查找字符串s中有多少子串与字符串p匹配
// 测试链接 : https://www.luogu.com.cn/problem/P3763

// 字符串哈希的常量和全局数组
const int MAXN = 100001; // 与Java代码保持一致，定义数组最大尺寸

const int base = 499; // 哈希基数，选用一个素数

// 引入模数 MOD，用于防止 long long 溢出。
// 解决上次我们讨论的核心问题，确保哈希计算的正确性与安全性。
const long long MOD = 1e9 + 7; // 选择一个大素数作为模数

// 全局数组，用于存储 base 的幂次和字符串s、p的前缀哈希值
long long pow[MAXN];    // 存储 base 的幂次
long long hashs[MAXN];  // 存储字符串 s 的前缀哈希值
long long hashp[MAXN];  // 存储字符串 p 的前缀哈希值

// 获取子字符串的哈希值
// 参数 hash_arr 是一个指向 long long 数组的指针（即 hashs 或 hashp）
// l 和 r 是字符串的起始和结束索引（包含），用于计算 s[l...r] 的哈希值
long long hash(const long long hash_arr[], int l, int r) {
    long long ans = hash_arr[r]; // 取得前缀 hash_arr[0...r] 的哈希值
    if (l == 0) { // 如果子串从索引0开始，直接返回前缀哈希值
        return ans;
    }

    // 计算子串哈希公式： H(l,r) = H(0,r) - H(0,l-1) * base^(长度)
    // 子串长度为 (r - l + 1)
    // 确保 (hash_arr[l - 1] * pow[r - l + 1]) 在取模后再相减，避免中间值溢出
    long long subtracted_part = (hash_arr[l - 1] * pow[r - l + 1]) % MOD;

    // 执行减法，并通过 (ans - subtracted_part + MOD) % MOD 确保结果非负
    ans = (ans - subtracted_part + MOD) % MOD;
    return ans;
}

// 构建幂次数组和两个字符串的前缀哈希数组
// s 和 p 是字符数组，n 和 m 是它们的长度
void build(const char s[], int n, const char p[], int m) {
    // 初始化 pow[0] 为 1 (base^0 = 1)
    pow[0] = 1;
    // 填充 pow 数组，所有乘法都进行模运算
    for (int j = 1; j < MAXN; j++) {
        pow[j] = (pow[j - 1] * base) % MOD;
    }

    // 计算字符串 s 的前缀哈希值
    // (s[0] - 'a' + 1) 将字符映射到 1-26，然后取模，确保结果在 [0, MOD-1)
    hashs[0] = (s[0] - 'a' + 1 + MOD) % MOD;
    for (int j = 1; j < n; j++) {
        long long char_val = (s[j] - 'a' + 1); // 获取当前字符的数值
        // 滚动哈希计算： (前一项哈希 * base + 当前字符值) % MOD
        hashs[j] = (hashs[j - 1] * base + char_val) % MOD;
    }

    // 计算字符串 p 的前缀哈希值，与 s 的计算方式相同
    hashp[0] = (p[0] - 'a' + 1 + MOD) % MOD;
    for (int j = 1; j < m; j++) {
        long long char_val = (p[j] - 'a' + 1);
        hashp[j] = (hashp[j - 1] * base + char_val) % MOD;
    }
}

// 检查 s[l1...l1+len-1] 和 p[l2...l2+len-1] 是否相同
// 这里的 hashs 和 hashp 是全局数组，函数可以直接访问
bool same(int l1, int l2, int len) {
    if (len == 0) { // 长度为0的子串总是相同的
        return true;
    }
    // 调用全局的 hash 函数来比较子串哈希值
    // 如果哈希值相同，则认为子串相同（字符串哈希有碰撞概率，但对于竞赛通常足够）
    return hash(hashs, l1, l1 + len - 1) == hash(hashp, l2, l2 + len - 1);
}

// 检查 s[l1...r1] （s的一个子串）和 p[0...m_len_p-1] （p的整个字符串）是否匹配
// 返回这两段上字符不一样的位置是不是<=k个
// l1, r1 是 s 子串的起始和结束索引（包含）
// k 是允许的最大差异数
// m_len_p 是字符串 p 的长度（用于边界检查）
bool check(int l1, int r1, int k, int m_len_p) {
    int diff = 0; // 记录差异数量
    int l2 = 0;   // p 的当前比较起始索引

    // 循环条件：
    // 1. l1 <= r1：s 的当前子段仍有字符可比较
    // 2. diff <= k：当前差异数未超过允许的最大值，否则无需继续
    while (l1 <= r1 && diff <= k) {
        int l_bin = 1;      // 二分查找的左边界：表示公共前缀长度的最小值（从1开始）
        int r_bin = r1 - l1 + 1; // 二分查找的右边界：当前 s 子段的剩余长度（公共前缀可能的最大长度）
        int m_bin, len = 0; // m_bin 是二分中间值，len 是找到的相同前缀的实际长度

        // 在当前剩余子串 s[l1...r1] 和 p[l2...] 之间，二分查找最长公共前缀的长度
        while (l_bin <= r_bin) {
            m_bin = l_bin + (r_bin - l_bin) / 2; // 避免 (l+r) 溢出，计算中间长度
            // 检查条件：
            // 1. l2 + m_bin - 1 < m_len_p：确保 p 的索引不会越界
            // 2. same(l1, l2, m_bin)：检查当前长度 m_bin 的子串是否相同
            if (l2 + m_bin - 1 < m_len_p && same(l1, l2, m_bin)) {
                len = m_bin;      // 更新找到的最长相同前缀长度
                l_bin = m_bin + 1; // 尝试更长的公共前缀
            } else {
                r_bin = m_bin - 1; // 相同前缀更短，缩小查找范围
            }
        }

        // 判断是否找到了差异点：
        // 如果找到的公共前缀长度 `len` 小于当前比较子段的完整长度 `(r1 - l1 + 1)`，
        // 说明在当前子段中存在一个不匹配的字符，因此差异数 `diff` 增加。
        // 如果 `len` 等于 `(r1 - l1 + 1)`，则表示当前子段完全匹配，没有差异。
        if (len < (r1 - l1 + 1)) {
            diff++; // 差异数加一
        }

        l1 += len + 1; // s 的下一个比较起始位置 (跳过匹配部分和第一个不匹配字符)
        l2 += len + 1; // p 的下一个比较起始位置 (跳过匹配部分和第一个不匹配字符)
    }
    // 返回最终差异数是否小于等于 k
    return diff <= k;
}




// s中有多少子串修改最多k个位置的字符就可以变成p
// s长度为n_s，p长度为n_p，时间复杂度O(n_s * k * log n_p)
int compute(const char s_arr[], int n_s, const char p_arr[], int n_p, int k) {
    // 如果 s 的长度小于 p 的长度，不可能匹配，直接返回0
    if (n_s < n_p) {
        return 0;
    }

    // 构建幂次数组和两个字符串的前缀哈希数组
    // 传入 char* 类型的字符串和它们的实际长度
    build(s_arr, n_s, p_arr, n_p);

    int ans = 0; // 记录匹配的子串数量
    // 遍历 s 中所有可能的起始位置，尝试匹配 p
    for (int i = 0; i <= n_s - n_p; i++) {
        // s[i...i+n_p-1] 是 s 中的一个子串，与 p[0...n_p-1] 进行比较
        if (check(i, i + n_p - 1, k, n_p)) { // 调用 check 函数检查差异数
            ans++; // 如果差异数符合要求，则匹配数量加一
        }
    }
    return ans;
}




// C++ 的主函数：程序入口
int main() {
    // 启用快速 I/O：
    // std::ios_base::sync_with_stdio(false) 解除 C++ 流与 C 标准流的同步，提高效率。
    // std::cin.tie(NULL) 解除 cin 和 cout 的绑定，使得 cin 不会每次输入前刷新 cout，进一步提高效率。
    // std::cout.tie(NULL) 确保 cout 不会缓冲，直接输出。
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int n_cases; // 对应 Java 中的 int n，表示测试用例的数量
    std::cin >> n_cases; // 从标准输入读取测试用例数量

    // 循环处理每个测试用例
    for (int i = 0; i < n_cases; i++) {
        std::string s_str, p_str;
        std::cin >> s_str >> p_str; // 从标准输入读取字符串 s 和 p

        // 调用 compute 函数，并传入字符串的 C 风格指针和它们的长度
        // Java 代码中 main 方法的 k 值为 3，这里也保持一致
        std::cout << compute(s_str.c_str(), s_str.length(), p_str.c_str(), p_str.length(), 3) << "\n";
    }

    return 0; // 程序成功执行返回0
}

