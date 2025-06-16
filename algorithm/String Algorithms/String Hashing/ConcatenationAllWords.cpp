#include <string>
#include <vector>
#include <unordered_map> // C++中HashMap的等价物

// 字符串哈希的常量
// MAXN 保持 10001，因为对于本题s的长度（最大10^4）是足够的
const int MAXN = 10001;
const int base = 499; // 哈希基数
// 引入模数 MOD，用于防止 long long 溢出。
// 这是为了解决我们上一轮讨论的核心问题，确保哈希计算的正确性与安全性。
const long long MOD = 1e9 + 7; // 选择一个大素数作为模数

// 串联所有单词的子串
// 给定一个字符串s和一个字符串数组words
// words中所有字符串长度相同
// s中的串联子串是指一个包含 words中所有字符串以任意顺序排列连接起来的子串
// 例如words = { "ab","cd","ef" }
// 那么"abcdef"、"abefcd"、"cdabef"、"cdefab"、"efabcd"、"efcdab"都是串联子串。
// "acdbef"不是串联子串，因为他不是任何words排列的连接
// 返回所有串联子串在s中的开始索引
// 你可以以任意顺序返回答案
// 测试链接 : https://leetcode.cn/problems/substring-with-concatenation-of-all-words/


// 主类和函数
// 测试链接 : https://leetcode.cn/problems/substring-with-concatenation-of-all-words/
class ConcatenationAllWords {
public:
    // 全局数组，用于存储 base 的幂次和字符串s的前缀哈希值。
    // 为了避免与函数名冲突，对数组名进行了必要的修改（添加_arr后缀）。
    long long pow_arr[MAXN];
    long long hash_arr[MAXN];
    // 构建幂次数组和字符串s的前缀哈希数组。
    // build 函数名保持不变。
    // 内部逻辑加入了模运算以防止 long long 溢出。
    void build(const std::string& str) {
        pow_arr[0] = 1;
        // 填充 pow_arr 数组，所有乘法都进行模运算
        for (int j = 1; j < MAXN; j++) { // 循环到 MAXN-1 以填充整个数组
            pow_arr[j] = (pow_arr[j - 1] * base) % MOD;
        }

        // 计算第一个字符的哈希值
        // (str[0] - 'a' + 1) 将字符映射到 1-26，然后取模，确保结果在 [0, MOD-1)
        hash_arr[0] = (str[0] - 'a' + 1 + MOD) % MOD;
        // 计算前缀哈希值，所有乘法和加法都进行模运算
        for (int j = 1; j < str.length(); j++) { // 循环到 str.length() - 1
            long long char_val = (str[j] - 'a' + 1);
            hash_arr[j] = (hash_arr[j - 1] * base + char_val) % MOD;
        }
    }

    // 获取子字符串s[l, r) (左闭右开) 的哈希值。
    // 为了避免与全局数组 hash_arr 和另一个 hash 函数冲突，函数名修改为 get_substring_hash。
    // 内部逻辑加入了模运算并处理了减法可能产生的负数。
    long long get_substring_hash(int l, int r) {
        // Java 代码中 hash[r-1] 对应的是 str[0...r-1] 的哈希值
        long long ans = hash_arr[r - 1];
        if (l == 0) {
            return ans; // 如果从索引0开始，直接返回前缀哈希
        }

        // 计算子串哈希公式：H(l,r) = H(0,r-1) - H(0,l-1) * base^(长度)
        // 子串长度为 (r-1) - l + 1 = r - l
        long long subtracted_part = (hash_arr[l - 1] * pow_arr[r - l]) % MOD;

        // 执行减法，并通过 (A - B + MOD) % MOD 确保结果非负
        ans = (ans - subtracted_part + MOD) % MOD;
        return ans;
    }

    // 计算单个单词字符串的哈希值。
    // 为了避免与全局数组 hash_arr 和另一个 hash 函数冲突，函数名修改为 get_word_hash。
    // 内部逻辑加入了模运算以防止 long long 溢出。
    long long get_word_hash(const std::string& str) {
        if (str.empty()) {
            return 0;
        }
        int n = str.length();
        long long ans = (str[0] - 'a' + 1 + MOD) % MOD;
        for (int j = 1; j < n; j++) {
            long long char_val = (str[j] - 'a' + 1);
            ans = (ans * base + char_val) % MOD;
        }
        return ans;
    }

    std::vector<int> findSubstring(std::string s, std::vector<std::string>& words) {
        std::vector<int> ans; // C++中 ArrayList 的等价物
        if (s.empty() || words.empty()) {
            return ans;
        }

        // words 的词频表，使用 unordered_map 作为 HashMap 的等价物
        // 变量名 'map' 保留，因为它不是 C++ 关键字，不会引起冲突
        std::unordered_map<long long, int> map;
        for (const std::string& key : words) {
            long long v = get_word_hash(key); // 使用修改后的单词哈希函数
            map[v]++; // 对于 C++ unordered_map，如果 key 不存在，operator[] 会插入并默认初始化为0，然后执行++
        }

        build(s); // 为字符串 s 构建哈希数组和幂次数组
        int n = s.length();
        int wordLen = words[0].length();
        int wordNum = words.size(); // C++ vector 使用 .size() 获取长度
        int allLen = wordLen * wordNum; // 所有单词连接起来的总长度

        // 窗口的词频表
        // 变量名 'window' 保留
        std::unordered_map<long long, int> window;

        // 同余分组，遍历 wordLen 种可能的起始位置
        // init 是当前同余组的起始索引
        for (int init = 0; init < wordLen && init + allLen <= n; init++) {
            int debt = wordNum; // 'debt' 表示窗口中还“缺少”多少个单词（或数量不足）

            // 初始化当前窗口，处理前 wordNum 个单词
            // l 和 r 定义当前要处理的单词子串的起始和结束（不包含）索引
            // part 计数已经处理的单词数量
            for (int l = init, r = init + wordLen, part = 0; part < wordNum; l += wordLen, r += wordLen, part++) {
                long long cur = get_substring_hash(l, r); // 获取当前单词的哈希值
                window[cur]++; // 窗口中该单词计数增加

                // 如果窗口中该单词的计数小于等于 words 词频表中所需的计数
                // 意味着该单词被正确匹配或仍处于不足状态，减少 debt
                if (window[cur] <= map[cur]) { // map[cur] 在 key 不存在时会返回 0
                    debt--;
                }
            }

            if (debt == 0) { // 如果所有单词都已正确匹配 (debt 为 0)
                ans.push_back(init); // 将当前窗口的起始索引加入结果
            }

            // 接下来窗口滑动：每次从左侧“移除”一个单词，从右侧“加入”一个单词
            // l1, r1 定义要移除的单词
            // l2, r2 定义要加入的单词
            for (int l1 = init, r1 = init + wordLen, l2 = init + allLen,
                    r2 = init + allLen + wordLen; r2 <= n; l1 += wordLen, r1 += wordLen, l2 += wordLen, r2 += wordLen) {

                long long out = get_substring_hash(l1, r1); // 获取要移除的单词的哈希值
                long long in = get_substring_hash(l2, r2);   // 获取要加入的单词的哈希值

                // 处理移除的单词
                window[out]--; // 窗口中该单词计数减少

                // 如果窗口中该单词的计数小于 words 词频表中所需的计数
                // 意味着我们现在“缺少”这个单词，增加 debt
                if (window[out] < map[out]) {
                    debt++;
                }

                // 处理加入的单词
                window[in]++; // 窗口中该单词计数增加

                // 如果窗口中该单词的计数小于等于 words 词频表中所需的计数
                // 意味着该单词被正确匹配或仍处于不足状态，减少 debt
                if (window[in] <= map[in]) {
                    debt--;
                }

                if (debt == 0) { // 如果所有单词都已正确匹配
                    ans.push_back(r1); // 将当前窗口的起始索引（即移除单词的下一个单词的起始）加入结果
                }
            }
            window.clear(); // 清空窗口，为下一个同余组做准备
        }
        return ans;
    }
};

