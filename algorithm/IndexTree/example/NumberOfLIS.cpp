#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

class Solution {
private:
    static const int MAXN = 2001;  // 最大数组长度

    int sort_arr[MAXN];  // 用于离散化的排序数组

    // 树状数组1：维护以数值i结尾的最长递增子序列的长度
    // treeMaxLen[i] 表示结尾数值<=i的所有子序列中，最长子序列的长度
    int treeMaxLen[MAXN];

    // 树状数组2：维护以数值i结尾的最长递增子序列的个数
    // treeMaxLenCnt[i] 表示结尾数值<=i的所有子序列中，达到最长长度的子序列个数
    int treeMaxLenCnt[MAXN];

    int m;  // 离散化后的数组长度（去重后）

    /**
     * 获取i的lowbit（最低位的1及其后面的0）
     * 用于树状数组的索引计算
     */
    int lowbit(int i) {
        return i & -i;
    }

    // 全局变量用于存储查询结果
    int maxLen, maxLenCnt;  // 查询到的最长长度和对应的个数

    /**
     * 查询操作：查询结尾数值<=i的最长递增子序列信息
     * @param i 查询的上界索引
     * 结果存储在全局变量maxLen和maxLenCnt中
     */
    void query(int i) {
        maxLen = maxLenCnt = 0;  // 初始化查询结果

        // 树状数组查询：从i开始向前查询
        while (i > 0) {
            if (maxLen == treeMaxLen[i]) {
                // 如果当前节点的最长长度等于已查到的最长长度
                // 则累加个数
                maxLenCnt += treeMaxLenCnt[i];
            } else if (maxLen < treeMaxLen[i]) {
                // 如果当前节点有更长的子序列
                // 更新最长长度和对应个数
                maxLen = treeMaxLen[i];
                maxLenCnt = treeMaxLenCnt[i];
            }
            // 树状数组向上查询的关键操作
            i -= lowbit(i);
        }
    }

    /**
     * 更新操作：更新以数值i结尾的最长递增子序列信息
     * @param i 要更新的索引位置
     * @param len 新的子序列长度
     * @param cnt 对应的子序列个数
     */
    void add(int i, int len, int cnt) {
        // 树状数组更新：从i开始向后更新所有相关节点
        while (i <= m) {
            if (treeMaxLen[i] == len) {
                // 如果当前节点的最长长度等于要更新的长度
                // 则累加个数
                treeMaxLenCnt[i] += cnt;
            } else if (treeMaxLen[i] < len) {
                // 如果要更新的长度更长
                // 则更新最长长度和对应个数
                treeMaxLen[i] = len;
                treeMaxLenCnt[i] = cnt;
            }
            // 树状数组向下更新的关键操作
            i += lowbit(i);
        }
    }

    /**
     * 二分查找：在离散化数组中查找值v的排名（索引）
     * @param v 要查找的值
     * @return 值v在离散化数组中的索引位置
     */
    int rank(int v) {
        int ans = 0;
        int l = 1, r = m, mid;

        // 二分查找第一个>=v的位置
        while (l <= r) {
            mid = (l + r) / 2;
            if (sort_arr[mid] >= v) {
                ans = mid;
                r = mid - 1;  // 继续向左找更小的满足条件的位置
            } else {
                l = mid + 1;  // 向右找
            }
        }
        return ans;
    }

public:
    /**
     * 主函数：计算最长递增子序列的个数
     * 算法思路：
     * 1. 对数组进行离散化处理（排序+去重）
     * 2. 使用两个树状数组分别维护长度和个数信息
     * 3. 遍历原数组，对每个元素进行查询和更新操作
     * 时间复杂度：O(n*logn)
     */
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();

        // 步骤1：复制原数组到sort_arr进行离散化
        // 注意：使用1-based索引，便于树状数组操作
        for (int i = 1; i <= n; i++) {
            sort_arr[i] = nums[i - 1];
        }

        // 步骤2：对数组进行排序
        sort(sort_arr + 1, sort_arr + n + 1);

        // 步骤3：去重，获得离散化后的数组
        m = 1;  // 去重后数组的长度
        for (int i = 2; i <= n; i++) {
            if (sort_arr[m] != sort_arr[i]) {
                sort_arr[++m] = sort_arr[i];
            }
        }

        // 步骤4：初始化两个树状数组
        memset(treeMaxLen + 1, 0, m * sizeof(int));
        memset(treeMaxLenCnt + 1, 0, m * sizeof(int));

        // 步骤5：遍历原数组，处理每个元素
        int i;
        for (int num : nums) {
            // 5.1：找到当前数字在离散化数组中的位置
            i = rank(num);

            // 5.2：查询比当前数字小的所有数字结尾的最长子序列信息
            query(i - 1);

            if (maxLen == 0) {
                // 情况1：没有比当前数字小的数字
                // 说明以当前数字结尾的最长子序列长度为1，个数为1
                add(i, 1, 1);
            } else {
                // 情况2：存在比当前数字小的数字
                // 以当前数字结尾的最长子序列长度为maxLen+1
                // 个数为之前查询到的maxLenCnt
                add(i, maxLen + 1, maxLenCnt);
            }
        }

        // 步骤6：查询所有数字的最长子序列信息，返回个数
        query(m);
        return maxLenCnt;
    }
};
