#include <vector>
#include <string>
#include <algorithm>

// 统计重复个数
// 如果字符串x删除一些字符，可以得到字符串y，那么就说y可以从x中获得
// 给定s1和a，代表s1拼接a次，记为字符串x
// 给定s2和b，代表s2拼接b次，记为字符串y
// 现在把y拼接m次之后，得到的字符串依然可能从x中获得，返回尽可能大的m
// s1、s2只由小写字母组成
// 1 <= s1长度、s2长度 <= 100
// 1 <= a、b <= 10^6

using namespace std;

class Solution {
public:


    // 时间复杂度O(s1长度 * s2长度)，与a、b的值无关
    int getMaxRepetitions(string str1, int a, string str2, int b) {
        int n = str1.length();

        // next[i][j] : 从s1的i位置出发，至少需要多少长度，能找到字符j
        // 这是一个预处理数组，用于快速查找字符位置
        vector<vector<int>> next(n, vector<int>(26));

        // 预处理next数组，并检查s2的所有字符是否都能在s1中找到
        if (!find(str1, n, next, str2)) {
            return 0;  // 如果s2中有字符在s1中找不到，直接返回0
        }

        // st[i][p] : 从s1的i位置出发，至少需要多少长度，可以获得2^p个s2
        // 这是倍增算法的核心数组，用于快速计算大量s2的匹配
        vector<vector<long long>> st(n, vector<long long>(30));

        // 初始化st数组的第0层：从每个位置出发，获得1个s2需要的长度
        for (int i = 0; i < n; i++) {
            int cur = i;        // 当前在s1中的位置
            long long len = 0;  // 累计需要的长度

            // 遍历s2的每个字符，计算从位置i开始匹配完整个s2需要的长度
            for (char c : str2) {
                len += next[cur][c - 'a'];                    // 找到字符c需要的长度
                cur = (cur + next[cur][c - 'a']) % n;         // 更新当前位置（循环）
            }
            st[i][0] = len;  // 从位置i开始，获得1个s2需要的长度
        }

        // 倍增预处理：st[i][p] = st[i][p-1] + st[新位置][p-1]
        // 即：获得2^p个s2 = 获得2^(p-1)个s2 + 再获得2^(p-1)个s2
        for (int p = 1; p <= 29; p++) {
            for (int i = 0; i < n; i++) {
                // 从位置i开始，先获得2^(p-1)个s2，然后从新位置再获得2^(p-1)个s2
                st[i][p] = st[i][p - 1] + st[(i + st[i][p - 1]) % n][p - 1];
            }
        }

        long long ans = 0;     // 最终能获得的s2的个数
        long long start = 0;   // 当前在s1中的起始位置

        // 贪心 + 倍增：从高位到低位，尽可能多地匹配s2
        for (long long p = 29; p >= 0; p--) {
            // 检查是否能再获得2^p个s2而不超出s1的总长度限制
            if (st[start % n][p] + start <= (long long)n * a) {
                ans += 1LL << p;                    // 增加2^p个s2
                start += st[start % n][p];          // 更新起始位置
            }
        }

        // 返回结果：总共获得的s2个数 除以 每个y包含的s2个数
        return (int)(ans / b);
    }

private:
    // 预处理函数：构建next数组，并检查s2的所有字符是否都能在s1中找到
    bool find(const string& s1, int n, vector<vector<int>>& next, const string& s2) {
        // right[j] : 字符j在s1中最右边的位置（考虑循环）
        vector<int> right(26, -1);

        // 第一遍扫描：记录每个字符在s1中的最左位置（下一轮的位置）
        for (int i = n - 1; i >= 0; i--) {
            right[s1[i] - 'a'] = i + n;  // +n表示下一轮s1中的位置
        }

        // 第二遍扫描：从右到左构建next数组
        for (int i = n - 1; i >= 0; i--) {
            right[s1[i] - 'a'] = i;  // 当前轮次中字符的位置

            // 对于位置i，计算找到每个字符需要的距离
            for (int j = 0; j < 26; j++) {
                if (right[j] != -1) {
                    next[i][j] = right[j] - i + 1;  // 从i到right[j]的距离
                } else {
                    next[i][j] = -1;  // 该字符在s1中不存在
                }
            }
        }

        // 检查s2中的所有字符是否都能在s1中找到
        for (char c : s2) {
            if (next[0][c - 'a'] == -1) {
                return false;  // 如果有字符找不到，返回false
            }
        }
        return true;
    }
};
