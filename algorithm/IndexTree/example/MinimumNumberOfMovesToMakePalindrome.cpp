#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

// 得到回文串的最少操作次数
// 给你一个只包含小写英文字母的字符串s
// 每一次操作可以选择s中两个相邻的字符进行交换
// 返回将s变成回文串的最少操作次数
// 输入数据会确保s一定能变成一个回문串
// 测试链接 : https://leetcode.cn/problems/minimum-number-of-moves-to-make-palindrome/

class Solution {
public:
    static const int MAXN = 2001;    // 字符串最大长度
    static const int MAXV = 26;      // 字母表大小(a-z)

    int n;              // 字符串长度
    string s;           // 输入字符串

    // 所有字符的位置列表 - 用于维护每个字符的位置链表
    int end[MAXV];      // end[i]表示字符i的最后一个位置的链表头
    int pre[MAXN];      // pre[i]表示位置i的前一个同字符位置

    // 树状数组 - 用于动态查询前缀和(统计还有多少个位置未被使用)
    int tree[MAXN];

    // 归并分治 - 用于计算逆序对数量
    int arr[MAXN];      // arr[i]记录位置i的字符最终要移动到的目标位置
    int help[MAXN];     // 归并排序的辅助数组

    // 初始化所有数据结构
    void build() {
        memset(end, 0, sizeof(end));      // 清空字符位置链表头
        memset(arr, 0, sizeof(arr));      // 清空目标位置数组
        memset(tree, 0, sizeof(tree));    // 清空树状数组
        // 初始化树状数组，每个位置初始值为1(表示该位置可用)
        for (int i = 1; i <= n; i++) {
            add(i, 1);
        }
    }

    // 将字符v在位置j的信息加入位置链表
    void push(int v, int j) {
        pre[j] = end[v];    // j的前驱是当前v字符的最后位置
        end[v] = j;         // 更新v字符的最后位置为j
    }

    // 弹出并返回字符v的最后一个位置
    int pop(int v) {
        int ans = end[v];           // 获取最后位置
        end[v] = pre[end[v]];       // 更新链表头为前一个位置
        return ans;
    }

    // 树状数组相关操作
    int lowbit(int i) {
        return i & -i;      // 获取i的最低位1
    }

    // 在位置i上增加值v
    void add(int i, int v) {
        while (i <= n) {
            tree[i] += v;
            i += lowbit(i);
        }
    }

    // 查询[1,i]区间的前缀和
    int sum(int i) {
        int ans = 0;
        while (i > 0) {
            ans += tree[i];
            i -= lowbit(i);
        }
        return ans;
    }

    // 主算法 - 时间复杂度O(n * logn)
    int minMovesToMakePalindrome(string str) {
        s = str;
        n = s.length();
        build();

        // 将每个字符的位置信息存入链表
        for (int i = 0, j = 1; i < n; i++, j++) {
            push(s[i] - 'a', j);
        }

        // 确定每个位置的字符最终要移动到的目标位置
        for (int i = 0, l = 1, r, k; i < n; i++, l++) {
            if (arr[l] == 0) {  // 如果位置l还没有确定目标
                // 找到与当前字符相同的配对字符位置
                r = pop(s[i] - 'a');

                if (l < r) {
                    // l和r需要配对成为回文的对称位置
                    k = sum(l);              // 查询l位置前面还有多少可用位置
                    arr[l] = k;              // l位置的字符移动到第k个位置
                    arr[r] = n - k + 1;      // r位置的字符移动到对称位置
                } else {
                    // l == r的情况，说明是中心字符(奇数长度回文串的中心)
                    arr[l] = (1 + n) / 2;    // 移动到正中央
                }
                add(r, -1);  // 标记r位置已被使用
            }
        }

        // 通过归并排序计算逆序对数量，即最少交换次数
        return number(1, n);
    }

    // 归并分治计算逆序对数量
    int number(int l, int r) {
        if (l >= r) {
            return 0;    // 区间长度<=1，无逆序对
        }
        int m = (l + r) / 2;
        // 分治：左半部分逆序对 + 右半部分逆序对 + 跨越中点的逆序对
        return number(l, m) + number(m + 1, r) + merge(l, m, r);
    }

    // 合并过程，统计跨越中点的逆序对数量
    int merge(int l, int m, int r) {
        int ans = 0;
        // 对于左半部分的每个元素，统计右半部分有多少个比它小的元素
        for (int i = m, j = r; i >= l; i--) {
            while (j >= m + 1 && arr[i] <= arr[j]) {
                j--;
            }
            ans += j - m;    // arr[i]与右半部分的j-m个元素构成逆序对
        }

        // 标准归并排序的合并过程
        int i = l;
        int a = l;      // 左半部分指针
        int b = m + 1;  // 右半部分指针

        // 合并两个有序数组
        while (a <= m && b <= r) {
            help[i++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];
        }
        // 处理剩余元素
        while (a <= m) {
            help[i++] = arr[a++];
        }
        while (b <= r) {
            help[i++] = arr[b++];
        }

        // 将排序结果复制回原数组
        for (i = l; i <= r; i++) {
            arr[i] = help[i];
        }
        return ans;
    }
};
