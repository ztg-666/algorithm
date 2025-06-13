#include <vector>
#include <string>


// KMP算法模版
// 测试链接 : https://leetcode.cn/problems/find-the-index-of-the-first-occurrence-in-a-string/

using namespace std;

// 生成部分匹配表（前缀函数）
// 参数：s - 模式串，m - 模式串长度
// 返回值：next数组，记录每个位置的最长公共前后缀长度
vector<int> nextArray(const string& s, int m) {
    vector<int> next(m);
    if (m == 1) {             // 边界情况处理
        next[0] = -1;
        return next;
    }

    // 初始化已知值
    next[0] = -1;            // 首字符无前缀
    next[1] = 0;             // 单个字符无公共前后缀
    int i = 2;               // 当前计算位置
    int cn = 0;              // 当前最长公共前后缀长度

    // 构建部分匹配表 O(m)
    while (i < m) {
        if (s[i - 1] == s[cn]) {    // 当前字符与前缀下一个字符匹配
            next[i++] = ++cn;       // 设置当前位置的匹配值并后移指针
        } else if (cn > 0) {        // 存在更短的前缀可以尝试
            cn = next[cn];          // 回溯到前一个前缀位置
        } else {                    // 无公共前缀可用
            next[i++] = 0;          // 匹配值设为0并后移指针
        }
    }
    return next;
}


// KMP核心算法实现
// 参数：s1 - 主字符串，s2 - 模式串
// 返回值：匹配成功的起始位置
int kmp(const string& s1, const string& s2) {
    int n = s1.size(), m = s2.size();
    int x = 0, y = 0;          // x-主串指针，y-模式串指针
    vector<int> next = nextArray(s2, m); // 获取部分匹配表

    // 主匹配循环 O(n)
    while (x < n && y < m) {
        if (s1[x] == s2[y]) {  // 字符匹配成功
            x++;
            y++;
        } else if (y == 0) {   // 模式串第一个字符就不匹配
            x++;               // 主串指针后移
        } else {               // 利用部分匹配表回溯模式串指针
            y = next[y];       // 关键回溯操作
        }
    }
    // 判断是否完全匹配
    return y == m ? x - y : -1;
}


// 字符串匹配入口函数
// 参数：s1 - 主字符串，s2 - 模式串
// 返回值：模式串在主串中首次出现的位置，未找到返回-1
int strStr(string s1, string s2) {
    // 处理边界情况
    if (s2.empty()) return 0;   // 空模式串约定返回0
    if (s1.empty()) return -1;  // 主串为空直接返回未找到
    return kmp(s1, s2);
}

