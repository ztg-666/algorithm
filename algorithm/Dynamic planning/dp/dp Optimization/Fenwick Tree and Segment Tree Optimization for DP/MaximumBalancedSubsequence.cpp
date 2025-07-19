#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

// 平衡子序列的最大和
// 给定一个长度为n的数组nums，下面定义平衡子序列
// 如果下标i和下标j被选进了子序列，i在j的左边
// 那么必须有nums[j] - nums[i] >= j - i
// 如果一个子序列中任意的两个下标都满足上面的要求，那子序列就是平衡的
// 返回nums所有平衡子序列里，最大的累加和是多少
// 1 <= n <= 10^5
// -10^9 <= nums[i] <= +10^9
// 测试链接 : https://leetcode.cn/problems/maximum-balanced-subsequence-sum/

class Solution {
private:
    static const int MAXN = 100001;  // 最大数组长度
    vector<int> sort_arr;            // 存储去重后的指标值，用于坐标压缩
    vector<long long> tree;          // 树状数组，维护区间最大值
    int n, m;                        // n: 原数组长度, m: 去重后指标值的个数

public:
    /**
     * 求平衡子序列的最大和
     * @param nums 输入数组
     * @return 最大平衡子序列和
     */
    long long maxBalancedSubsequenceSum(vector<int>& nums) {
        // 初始化数据结构
        build(nums);

        long long pre;  // 存储前缀最大值

        // 遍历每个位置
        for (int i = 0, k; i < n; i++) {
            // 计算当前位置的指标值，并找到其在压缩坐标中的位置
            k = rank(nums[i] - i);

            // 查询dp[1..k]中的最大值，即所有指标值 <= nums[i]-i 的位置的最大dp值
            pre = max_query(k);

            if (pre < 0) {
                // 如果之前的最好情况是负数，说明加上之前的数会让结果变差
                // 所以当前数字自己单独形成平衡子序列
                update(k, nums[i]);
            } else {
                // 如果之前的最好情况不是负数，那么和当前数字一起形成更大的累加和
                update(k, pre + nums[i]);
            }
        }

        // 返回所有dp值中的最大值
        return max_query(m);
    }

private:
    /**
     * 初始化数据结构
     * 1. 计算每个位置的指标值 nums[i] - i
     * 2. 对指标值进行排序和去重（坐标压缩）
     * 3. 初始化树状数组
     */
    void build(vector<int>& nums) {
        n = nums.size();
        sort_arr.resize(n + 1);  // 下标从1开始使用
        tree.resize(MAXN);

        // 计算每个位置的指标值：nums[i] - i
        for (int i = 1, j = 0; i <= n; i++, j++) {
            sort_arr[i] = nums[j] - j;
        }

        // 对指标值进行排序，为去重做准备
        sort(sort_arr.begin() + 1, sort_arr.begin() + n + 1);

        // 去重：将相同的指标值合并，实现坐标压缩
        m = 1;  // m表示去重后指标值的个数
        for (int i = 2; i <= n; i++) {
            if (sort_arr[m] != sort_arr[i]) {
                sort_arr[++m] = sort_arr[i];
            }
        }

        // 初始化树状数组，所有值设为负无穷
        fill(tree.begin() + 1, tree.begin() + m + 1, LLONG_MIN);
    }

    /**
     * 二分查找：找到指标值v在压缩坐标中的位置
     * @param v 要查找的指标值
     * @return v在压缩坐标中的位置（小于等于v的最大位置）
     */
    int rank(int v) {
        int left = 1, right = m, mid;
        int ans = 0;

        // 二分查找最大的位置，使得sort_arr[位置] <= v
        while (left <= right) {
            mid = (left + right) / 2;
            if (sort_arr[mid] <= v) {
                ans = mid;          // 当前位置满足条件，继续向右寻找更大的位置
                left = mid + 1;
            } else {
                right = mid - 1;    // 当前位置不满足条件，向左寻找
            }
        }
        return ans;
    }

    /**
     * 树状数组更新操作：更新第i个位置的dp值
     * @param i 要更新的位置（压缩后的坐标）
     * @param v 新的dp值
     */
    void update(int i, long long v) {
        // 树状数组的更新：从位置i开始，向上更新所有相关节点
        while (i <= m) {
            tree[i] = max(tree[i], v);  // 取最大值
            i += i & -i;                // 树状数组的经典操作：移动到下一个需要更新的位置
        }
    }

    /**
     * 树状数组查询操作：查询dp[1..i]中的最大值
     * @param i 查询的右边界
     * @return dp[1..i]中的最大值
     */
    long long max_query(int i) {
        long long ans = LLONG_MIN;

        // 树状数组的查询：从位置i开始，向下查询所有相关节点
        while (i > 0) {
            ans = max(ans, tree[i]);    // 取最大值
            i -= i & -i;                // 树状数组的经典操作：移动到下一个需要查询的位置
        }
        return ans;
    }
};

/**
 * 算法复杂度分析：
 * 时间复杂度：O(n log n)
 * - 排序：O(n log n)
 * - 每个位置的查询和更新：O(log n)，总共n个位置，所以是O(n log n)
 *
 * 空间复杂度：O(n)
 * - sort_arr 数组：O(n)
 * - tree 数组：O(n)
 *
 * 核心思想：
 * 1. 将不等式条件转换为指标值的比较
 * 2. 使用坐标压缩处理大范围的指标值
 * 3. 使用树状数组优化动态规划的状态转移
 * 4. 每个位置都可以选择单独成为子序列，或者接在之前的最优子序列后面
 */
