
class Manacher {
public:
    // 将输入字符串转换为一个在字符之间插入'#'的新字符串，以便处理偶数长度的回文
    static std::vector<char> manacherString(const std::string& str) {
        std::vector<char> charArr(str.begin(), str.end()); // 将输入字符串转换为字符向量
        std::vector<char> res(str.length() * 2 + 1); // 创建一个新向量，长度为原字符串长度的两倍加一
        int index = 0;
        for (int i = 0; i < res.size(); i++) {
            // 在每个字符之间插入'#'
            res[i] = (i & 1) == 0 ? '#' : charArr[index++];
        }
        return res;
    }

    // 计算输入字符串的最长回文子串的长度
    static int maxLcpsLength(const std::string& str) {
        if (str.empty()) {
            return 0; // 如果输入字符串为空，返回0
        }
        std::vector<char> charArr = manacherString(str); // 调用manacherString函数处理输入字符串
        std::vector<int> pArr(charArr.size()); // 创建一个与处理后字符串等长的数组，用于存储每个位置的回文半径
        int C = -1; // 回文中心
        int R = -1; // 回文有边界的再往右一个位置，最右的有效区是R-1位置·
        int max = INT_MIN; // 记录最长回文子串的长度
        for (int i = 0; i < charArr.size(); i++) {
            // 根据回文右边界确定初始的回文半径
            pArr[i] = R > i ? min(pArr[2 * C - i], R - i) : 1;
            // 尝试扩展回文半径
            while (i + pArr[i] < charArr.size() && i - pArr[i] > -1) {
                if (charArr[i + pArr[i]] == charArr[i - pArr[i]])
                    pArr[i]++;
                else {
                    break;
                }
            }
            // 更新回文中心和右边界
            if (i + pArr[i] > R) {
                R = i + pArr[i];
                C = i;
            }
            // 更新最长回文子串的长度
            max = max(max, pArr[i]);
        }
        return max - 1; // 返回最长回文子串的实际长度
    }

};

