#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;
// 每个元音包含偶数次的最长子字符串
// 给你一个字符串 s ，请你返回满足以下条件的最长子字符串的长度
// 每个元音字母，即 'a'，'e'，'i'，'o'，'u'
// 在子字符串中都恰好出现了偶数次。
class EvenCountsLongestSubarray {
public:
    static int findTheLongestSubstring(const string& s) {
        int n = s.length();
        // 只有5个元音字符，状态就5位
        vector<int> map(32, -2);
        // map[0...31] = -2
        // map[01100] = -2, 这个状态之前没出现过
        map[0] = -1;
        int ans = 0;
        for (int i = 0, status = 0, m; i < n; i++) {
            // status : 0....i-1字符串上，aeiou的奇偶性
            // s[i] = 当前字符
            // 情况1 : 当前字符不是元音，status不变
            // 情况2 : 当前字符是元音，a~u(0~4)，修改相应的状态
            m = move(s[i]);
            if (m != -1) {
                status ^= 1 << m;
            }
            // status:  0....i字符串上，aeiou的奇偶性
            // 同样的状态，之前最早出现在哪
            if (map[status] != -2) {
                ans = max(ans, i - map[status]);
            } else {
                map[status] = i;
            }
        }
        return ans;
    }

    static int move(char cha) {
        switch (cha) {
            case 'a': return 0;
            case 'e': return 1;
            case 'i': return 2;
            case 'o': return 3;
            case 'u': return 4;
            default: return -1;
        }
    }
};

int main() {
    // 测试用例 1
    string s1 = "eleetminicoworoep";
    cout << "Test Case 1: " << EvenCountsLongestSubarray::findTheLongestSubstring(s1) << endl; // 输出: 13

    // 测试用例 2
    string s2 = "leetcodeisgreat";
    cout << "Test Case 2: " << EvenCountsLongestSubarray::findTheLongestSubstring(s2) << endl; // 输出: 5

    // 测试用例 3
    string s3 = "bcbcbc";
    cout << "Test Case 3: " << EvenCountsLongestSubarray::findTheLongestSubstring(s3) << endl; // 输出: 6

    // 测试用例 4
    string s4 = "a";
    cout << "Test Case 4: " << EvenCountsLongestSubarray::findTheLongestSubstring(s4) << endl; // 输出: 0

    return 0;
}
