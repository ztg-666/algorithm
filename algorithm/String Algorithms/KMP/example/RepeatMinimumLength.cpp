#include <iostream>
#include <cstring>

// 最短循环节的长度
// 给你一个字符串s，它一定是由某个循环节不断自我连接形成的
// 题目保证至少重复2次，但是最后一个循环节不一定完整
// 现在想知道s的最短循环节是多长
// 测试链接 : https://www.luogu.com.cn/problem/P4391

using namespace std;

const int MAXN = 1000001;  // 定义最大字符串长度

int nxt[MAXN];  // KMP算法中的next数组（前缀函数），nxt[i]表示子串s[0:i-1]的最长公共前后缀长度
int n;          // 输入字符串的长度
string s;       // 输入的字符串

/**
 * 构建KMP算法的next数组（前缀函数）
 * 核心思想：利用已知前缀信息推导后续位置的前缀值
 */
void nextArray() {
    nxt[0] = -1;  // 边界值，-1表示不存在公共前后缀
    nxt[1] = 0;   // 单个字符的公共前后缀长度为0

    int i = 2;    // 当前计算的数组下标（从2开始）
    int cn = 0;   // 当前比较的公共前后缀长度候选值

    while (i <= n) {  // 遍历整个字符串长度
        // 情况1：当前字符与前缀下一个字符匹配
        if (s[i - 1] == s[cn]) {
            nxt[i++] = ++cn;  // 设置next值并同步移动双指针
        }
        // 情况2：不匹配但存在可回溯的公共前缀
        else if (cn > 0) {
            cn = nxt[cn];     // 利用已计算的next值回溯比较位置
        }
        // 情况3：无任何公共前后缀
        else {
            nxt[i++] = 0;     // 设置next值为0并移动主指针
        }
    }
}

/**
 * 计算字符串的最小循环节长度
 * 原理：最小循环节长度 = 字符串总长度 - 最后一个字符的next值
 */
int compute() {
    nextArray();  // 构建next数组
    return n - nxt[n];  // 返回最小循环节长度
}

int main() {
    ios::sync_with_stdio(false);  // 关闭C++与C的流同步
    cin.tie(0);                   // 解除cin与cout的绑定

    cin >> n >> s;                // 读取字符串长度和内容
    cout << compute() << endl;    // 输出最小循环节长度

    return 0;
}
