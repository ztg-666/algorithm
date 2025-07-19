#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

class Solution {
private:
   static const int MAXN = 100002;  // 最大数组长度限制
   int n, m;                        // n: 原数组长度, m: 去重后前缀和的个数
   long long sort_arr[MAXN];        // 存储前缀和并用于排序去重
   int tree[MAXN];                  // 树状数组，用于快速统计区间内元素个数

public:
   /**
    * 计算达标子数组的个数
    * 子数组达标条件：累加和在[lower, upper]范围内
    *
    * @param nums   输入数组
    * @param lower  累加和下界
    * @param upper  累加和上界
    * @return       达标子数组的个数
    */
   int countRangeSum(vector<int>& nums, int lower, int upper) {
       build(nums);           // 构建前缀和数组并初始化树状数组
       long long sum = 0;     // 当前前缀和
       int ans = 0;           // 答案：达标子数组个数

       // 遍历每个位置作为子数组右端点
       for (int i = 0; i < n; i++) {
           sum += nums[i];    // 更新当前前缀和

           // 统计以i为右端点的达标子数组个数
           // 需要找到有多少个前缀和preSum满足：lower <= sum - preSum <= upper
           // 即：sum - upper <= preSum <= sum - lower
           ans += query_sum(rank(sum - lower)) - query_sum(rank(sum - upper - 1));

           // 检查从0到i的子数组是否达标
           if (lower <= sum && sum <= upper) {
               ans++;
           }

           // 将当前前缀和加入树状数组（表示这个前缀和出现了1次）
           add(rank(sum), 1);
       }
       return ans;
   }

private:
   /**
    * 构建前缀和数组，排序去重，初始化树状数组
    *
    * @param nums 输入数组
    */
   void build(vector<int>& nums) {
       n = nums.size();
       sort_arr[0] = 0;       // 前缀和数组初始值为0（表示空前缀的和）

       // 生成前缀和数组
       for (int i = 1, j = 0; i <= n; i++, j++) {
           sort_arr[i] = sort_arr[i - 1] + nums[j];
       }

       // 对前缀和数组排序（为了后续去重和二分查找）
       sort(sort_arr + 1, sort_arr + n + 1);

       // 去重：将相同的前缀和合并，最终有m个不同的前缀和
       m = 1;
       for (int i = 2; i <= n; i++) {
           if (sort_arr[m] != sort_arr[i]) {
               sort_arr[++m] = sort_arr[i];
           }
       }

       // 初始化树状数组，下标范围1~m
       memset(tree + 1, 0, sizeof(int) * m);
   }

   /**
    * 离散化：返回小于等于v且尽量大的前缀和的排名
    * 使用二分查找在排序后的前缀和数组中找位置
    *
    * @param v 目标值
    * @return  排名（1-indexed）
    */
   int rank(long long v) {
       int left = 1, right = m, mid;
       int ans = 0;           // 如果没找到，返回0表示没有小于等于v的值

       // 二分查找最后一个小于等于v的位置
       while (left <= right) {
           mid = (left + right) / 2;
           if (sort_arr[mid] <= v) {
               ans = mid;     // 记录当前位置
               left = mid + 1; // 继续向右找，寻找更大的满足条件的值
           } else {
               right = mid - 1; // 当前值太大，向左找
           }
       }
       return ans;
   }

   /**
    * 树状数组更新操作：将位置i的值增加c
    *
    * @param i 位置（1-indexed）
    * @param c 增加的值
    */
   void add(int i, int c) {
       while (i <= m) {
           tree[i] += c;      // 更新当前节点
           i += i & -i;       // 移动到下一个需要更新的节点
       }
   }

   /**
    * 树状数组查询操作：查询区间[1, i]的和
    *
    * @param i 查询的右端点（1-indexed）
    * @return  区间[1, i]内所有元素的和
    */
   int query_sum(int i) {
       int ans = 0;
       while (i > 0) {
           ans += tree[i];    // 累加当前节点的值
           i -= i & -i;       // 移动到父节点
       }
       return ans;
   }
};

/*
算法思路总结：
1. 使用前缀和将子数组和问题转化为两个前缀和的差值问题
2. 对于每个位置i，需要统计有多少个位置j < i，使得preSum[i] - preSum[j] ∈ [lower, upper]
3. 即统计有多少个preSum[j] ∈ [preSum[i] - upper, preSum[i] - lower]
4. 使用离散化将前缀和映射到较小的范围内
5. 使用树状数组快速统计某个范围内前缀和的个数
6. 时间复杂度：O(n log n)，空间复杂度：O(n)
*/
