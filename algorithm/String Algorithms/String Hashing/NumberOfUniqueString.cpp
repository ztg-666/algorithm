#include <iostream>      // 引入标准输入输出流库，用于调试或可能的将来交互，尽管当前代码中未使用
#include <string>        // 引入字符串库，用于处理std::string类型
#include <unordered_set> // 引入无序集合库，用于存储哈希值并自动处理去重
#include <algorithm>     // 引入算法库，提供如std::fill等泛型算法

// 独特子串的数量
// 给你一个由数字组成的字符串s
// 返回s中独特子字符串数量
// 其中的每一个数字出现的频率都相同
// 测试链接 : https://leetcode.cn/problems/unique-substrings-with-equal-digit-frequency/

using namespace std;     // 使用std命名空间，避免每次使用标准库组件时都加std::前缀

class Solution {
public:
    // equalDigitFrequency 函数：计算具有相同数字频率的独特子字符串的数量
    // 参数：const string& s - 输入的字符串，使用引用传递以避免不必要的复制，并使用const保证不修改原字符串
    int equalDigitFrequency(const string& s) {
        // 定义哈希基数 (base)：选择一个较大的素数，用于多项式哈希，以减少哈希冲突的概率。
        // 选择一个质数可以更好地分散哈希值，降低碰撞率。
        const long long base = 499;

        // 获取输入字符串的长度，便于后续循环迭代
        int n = s.size();

        // unordered_set<long long> hashSet：用于存储所有满足条件的子字符串的哈希值。
        // unordered_set提供O(1)的平均时间复杂度进行插入和查找，并且会自动处理重复元素的插入，确保最终集合中的哈希值都是唯一的。
        unordered_set<long long> hashSet;

        // int cnt[10]：一个长度为10的数组，用于存储当前正在处理的子字符串中每个数字（0-9）出现的频率。
        // 数组索引代表数字本身，值代表其出现次数。
        int cnt[10];

        // 外层循环：遍历所有可能的子字符串的起始位置 'i'
        // 从字符串的第一个字符开始，直到最后一个字符，每个字符都可能是一个子字符串的起点。
        for (int i = 0; i < n; ++i) {
            // 在处理每个新的子字符串起始位置 'i' 时，需要重置相关状态变量。
            // fill(cnt, cnt + 10, 0)：将数字频率计数数组清零，为新的子字符串做准备。
            fill(cnt, cnt + 10, 0);

            // long long hashCode = 0：当前子字符串的哈希值。每次内层循环（扩展子字符串）时都会更新。
            // 每次从新的起始点 'i' 开始构建子字符串时，哈希值也需要重置为0。
            long long hashCode = 0;

            // int curVal = 0：当前处理的字符（转换为数字）的值。
            int curVal = 0;

            // int maxCnt = 0：当前子字符串中，所有出现过的数字中，最大的出现频率。
            int maxCnt = 0;

            // int maxCntKinds = 0：当前子字符串中，有多少种不同的数字具有 'maxCnt' 这个最大频率。
            // 例如，如果子字符串是 "11223"，maxCnt=2，'1'和'2'的频率都是2，则maxCntKinds=2。
            int maxCntKinds = 0;

            // int allKinds = 0：当前子字符串中，总共有多少种不同的数字出现过。
            // 例如，"112" 中有'1'和'2'两种，allKinds=2。
            int allKinds = 0;

            // 内层循环：遍历所有可能的子字符串的结束位置 'j'
            // 从当前的起始位置 'i' 开始，逐步向后扩展子字符串，直到字符串的末尾。
            for (int j = i; j < n; ++j) {
                // 将当前字符 s[j] 从字符类型转换为整数类型。
                curVal = s[j] - '0';

                // 哈希计算：多项式哈希的核心步骤。
                // hashCode = hashCode * base + (curVal + 1)：
                // 1. 将当前哈希值乘以基数 'base'，相当于为新的数字腾出位置。
                // 2. 加上 'curVal + 1'。这里加1的目的是为了避免数字 '0' 在哈希计算中导致的问题。
                //    如果直接加 'curVal'，当 'curVal' 是 '0' 时，hashCode * base + 0 的效果和 hashCode * base 是一样的，
                //    可能会导致不同的子字符串哈希值相同（例如 "10" 和 "1" 可能出现哈希冲突）。
                //    通过加1，确保每个数字都有一个非零的贡献值，增加了哈希值的区分度。
                hashCode = hashCode * base + curVal + 1;

                // 更新当前数字的频率。
                cnt[curVal]++;

                // 如果当前数字 'curVal' 是第一次出现在当前子字符串中，则增加不同数字的种类数。
                if (cnt[curVal] == 1) {
                    allKinds++;
                }

                // 更新最大频率 'maxCnt' 和具有最大频率的数字种类数 'maxCntKinds'。
                // 如果当前数字的频率 'cnt[curVal]' 大于当前的最大频率 'maxCnt'：
                // 说明找到了一个新的最大频率。
                if (cnt[curVal] > maxCnt) {
                    maxCnt = cnt[curVal];     // 更新最大频率为当前数字的频率。
                    maxCntKinds = 1;          // 只有一个数字（即当前这个）拥有这个新的最大频率，所以重置为1。
                }
                // 否则，如果当前数字的频率 'cnt[curVal]' 等于当前的最大频率 'maxCnt'：
                // 说明又发现一个数字拥有相同的最大频率。
                else if (cnt[curVal] == maxCnt) {
                    maxCntKinds++;            // 增加拥有最大频率的数字种类数。
                }

                // 判断当前子字符串是否满足条件：所有出现的数字频率相等。
                // 这个条件等价于：具有最大频率的数字种类数 'maxCntKinds' 等于所有不同数字的种类数 'allKinds'。
                // 只有当所有出现的数字都达到了相同的最大频率时，这个条件才成立。
                if (maxCntKinds == allKinds) {
                    // 如果满足条件，将当前子字符串的哈希值插入到哈希集合中。
                    // unordered_set会自动处理重复插入，确保只存储唯一的哈希值。
                    hashSet.insert(hashCode);
                }
            }
        }

        // 返回哈希集合中元素的数量，这代表了所有满足条件的独特子字符串的总数。
        return (int)hashSet.size();
    }
};
