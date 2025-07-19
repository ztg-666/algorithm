#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// 最长理想子序列
// 给定一个长度为n，只由小写字母组成的字符串s，给定一个非负整数k
// 字符串s可以生成很多子序列，下面给出理想子序列的定义
// 子序列中任意相邻的两个字符，在字母表中位次的差值绝对值<=k
// 返回最长理想子序列
// 1 <= n <= 10^5
// 0 <= k <= 25
// s只由小写字母组成
// 测试链接 : https://leetcode.cn/problems/longest-ideal-subsequence/

class Solution {
private:
   static const int n = 26;  // 字母表大小，a-z共26个字母
   vector<int> maxVal;       // 线段树数组，存储区间最大值

   /**
    * 线段树向上更新操作
    * 将子节点的最大值传递给父节点
    * @param i 当前节点在线段树中的索引
    */
   void up(int i) {
       maxVal[i] = max(maxVal[i << 1], maxVal[i << 1 | 1]);
   }

   /**
    * 线段树单点更新操作
    * 将指定位置的值更新为新值，并向上传播更新
    * @param jobi 要更新的位置（字母对应的数字，1-26）
    * @param jobv 要更新的值（以该字母结尾的最长理想子序列长度）
    * @param l 当前节点代表的区间左边界
    * @param r 当前节点代表的区间右边界
    * @param i 当前节点在线段树中的索引
    */
   void update(int jobi, int jobv, int l, int r, int i) {
       if (l == r && jobi == l) {
           // 找到叶节点，直接更新
           maxVal[i] = jobv;
       } else {
           int m = (l + r) >> 1;  // 计算中点
           if (jobi <= m) {
               // 目标位置在左子树
               update(jobi, jobv, l, m, i << 1);
           } else {
               // 目标位置在右子树
               update(jobi, jobv, m + 1, r, i << 1 | 1);
           }
           up(i);  // 向上更新父节点
       }
   }

   /**
    * 线段树区间查询操作
    * 查询指定区间内的最大值
    * @param jobl 查询区间的左边界
    * @param jobr 查询区间的右边界
    * @param l 当前节点代表的区间左边界
    * @param r 当前节点代表的区间右边界
    * @param i 当前节点在线段树中的索引
    * @return 指定区间内的最大值
    */
   int queryMax(int jobl, int jobr, int l, int r, int i) {
       if (jobl <= l && r <= jobr) {
           // 当前区间完全包含在查询区间内，直接返回
           return maxVal[i];
       }
       int m = (l + r) >> 1;  // 计算中点
       int ans = 0;
       if (jobl <= m) {
           // 查询区间与左子树有交集
           ans = max(ans, queryMax(jobl, jobr, l, m, i << 1));
       }
       if (jobr > m) {
           // 查询区间与右子树有交集
           ans = max(ans, queryMax(jobl, jobr, m + 1, r, i << 1 | 1));
       }
       return ans;
   }

public:
   /**
    * 求最长理想子序列的长度
    * 使用动态规划 + 线段树优化
    *
    * 算法思路：
    * 1. 对于每个字符，维护以该字符结尾的最长理想子序列长度
    * 2. 当遍历到字符c时，查询所有与c差值不超过k的字符中，最长子序列的长度
    * 3. 以c结尾的最长子序列长度 = 上述查询结果 + 1
    * 4. 使用线段树来优化区间查询和单点更新操作
    *
    * 时间复杂度：O(n * log e)，n为字符串长度，e为字符集大小(26)
    * 空间复杂度：O(e)
    *
    * @param s 输入字符串
    * @param k 允许的最大字符差值
    * @return 最长理想子序列的长度
    */
   int longestIdealString(string s, int k) {
       // 初始化线段树，大小为 4*26，全部设为0
       maxVal.assign((n + 1) << 2, 0);

       int v, p;      // v: 当前字符对应的数字, p: 查询到的最大长度
       int ans = 0;   // 全局最长理想子序列长度

       // 遍历字符串中的每个字符
       for (char cha : s) {
           // 将字符转换为1-26的数字（a=1, b=2, ..., z=26）
           v = cha - 'a' + 1;

           // 查询与当前字符差值不超过k的所有字符中，最长子序列的长度
           // 查询区间：[max(v-k, 1), min(v+k, 26)]
           p = queryMax(max(v - k, 1), min(v + k, n), 1, n, 1);

           // 更新全局答案
           ans = max(ans, p + 1);

           // 更新以当前字符结尾的最长理想子序列长度
           update(v, p + 1, 1, n, 1);
       }

       return ans;
   }
};
