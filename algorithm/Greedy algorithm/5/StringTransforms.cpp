#include <string>
#include <algorithm> // 需要包含 algorithm 头文件用于 std::fill

// 字符串转化
// 给出两个长度相同的字符串str1和str2
// 请你帮忙判断字符串str1能不能在 零次 或 多次 转化后变成字符串str2
// 每一次转化时，你可以将str1中出现的所有相同字母变成其他任何小写英文字母
// 只有在字符串str1能够通过上述方式顺利转化为字符串str2时才能返回true
// 测试链接 : https://leetcode.cn/problems/string-transforms-into-another-string/

using namespace std;

bool canConvert(string str1, string str2) {
    if (str1 == str2) {
        return true;
    }

    int count[26] = {0}; // 统计str2字符出现次数
    int kinds = 0;
    for (char c : str2) {
        if (count[c - 'a']++ == 0) {
            kinds++;
        }
    }
    if (kinds == 26) {
        return false;
    }

    int prev_pos[26];
    fill(prev_pos, prev_pos + 26, -1); // 初始化位置记录数组

    for (int i = 0; i < str1.size(); ++i) {
        int cur = str1[i] - 'a';
        if (prev_pos[cur] != -1) {
            // 检查之前映射是否与当前冲突
            if (str2[prev_pos[cur]] != str2[i]) {
                return false;
            }
        }
        prev_pos[cur] = i; // 更新字符最后出现位置
    }

    return true;
}
