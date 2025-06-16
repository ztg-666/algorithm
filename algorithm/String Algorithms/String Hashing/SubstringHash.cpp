#include<bits/stdc++.h>

// 字符串哈希得到子串哈希
// 利用字符串哈希的便利性替代KMP算法
// 测试链接 : https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/

using namespace std;
// 统一使用 long long 类型来存储 MOD，防止溢出
const long long MOD = 1e9 + 7; // 一个常用的、足够大的素数模数
const int BASE = 499;          // 统一的哈希基数，保持你原有的选择

// 使用命名空间可以避免全局变量污染，但在这个简单场景下无所谓
using ll = long long;

// MAXN 保持不变
const int MAXN = 100005;

// 定义为全局变量以便在多个函数中使用，或作为类的成员变量
ll pow_[MAXN]; // 存储 BASE 的幂次，用于计算子串哈希
ll hash_[MAXN]; // 存储字符串 s1 的前缀哈希值

// 构建 pow_ 数组和 hash_ 数组
// s: 待处理的字符串
// n: 字符串 s 的长度
void build(const string& s, int n) { // 传入 const 引用，避免不必要的拷贝
    pow_[0] = 1;
    for (int i = 1; i <= n; i++) { // 注意：这里循环到 n 而不是 n-1，因为 pow_[n] 可能会被用到
                                   // hash_ 数组通常也计算到 n-1，但 pow_ 可能会需要到 n
                                   // 调整为 i <= n 确保 pow_[n] 可用
        pow_[i] = (pow_[i - 1] * BASE) % MOD;
    }

    hash_[0] = 0; // 习惯上 hash_[0] 为空字符串的哈希，hash_[i] 为前 i 个字符的哈希
                  // 但你的原代码是 hash_[0] = s[0]-'a'+1, hash_[i]为前i+1个字符哈希
                  // 我们保持你的原始习惯，即 hash_[i] 存储 s[0...i] 的哈希
    hash_[0] = (s[0] - 'a' + 1); // 第一个字符的哈希
    for (int i = 1; i < n; i++) {
        hash_[i] = (hash_[i - 1] * BASE + (s[i] - 'a' + 1)) % MOD;
    }
}

// 返回s[l...r]的哈希值
// l: 子字符串的起始索引 (0-based)
// r: 子字符串的结束索引 (0-based)
ll get_substring_hash(int l, int r) {
    ll ans = hash_[r];
    if (l > 0) {
        // 计算 hash_[l-1] * pow_[r-l+1] 的模结果
        ll term = (hash_[l - 1] * pow_[r - l + 1]) % MOD;
        // 模减法：(A - B + MOD) % MOD 确保结果非负
        ans = (ans - term + MOD) % MOD;
    }
    return ans;
}

// 查找 s2 在 s1 中首次出现的位置
// s1: 主字符串 (haystack)
// s2: 待查找的字符串 (needle)
int strStr(string s1, string s2) {
    int n = s1.size();
    int m = s2.size();

    // 处理边缘情况
    if (m == 0) return 0; // 如果 s2 是空字符串，返回 0
    if (n < m) return -1; // 如果 s1 比 s2 短，s2 不可能存在于 s1 中

    // 构建 s1 的哈希前缀和幂次数组
    build(s1, n);

    // 计算 s2 的哈希值
    ll h2 = 0;
    for (int i = 0; i < m; i++) {
        h2 = (h2 * BASE + (s2[i] - 'a' + 1)) % MOD; // 统一使用 BASE 并进行模运算
    }

    // 遍历 s1 的所有长度为 m 的子字符串，并比较哈希值
    for (int l = 0; l <= n - m; l++) { // l 从 0 到 n-m
        int r = l + m - 1;              // r 是当前子字符串的结束索引
        if (get_substring_hash(l, r) == h2) {
            return l; // 找到匹配，返回起始索引
        }
    }

    return -1; // 未找到匹配
}
