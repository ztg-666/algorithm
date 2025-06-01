#include <vector>
#include <climits>
#include <algorithm>

// 最短无序连续子数组
// 给你一个整数数组nums，你需要找出一个 连续子数组
// 如果对这个子数组进行升序排序，那么整个数组都会变为升序排序
// 请你找出符合题意的最短子数组，并输出它的长度
// 测试链接 : https://leetcode.cn/problems/shortest-unsorted-continuous-subarray/

using namespace std;

int findUnsortedSubarray(vector<int>& nums) {
    int n = nums.size();
    // 确定无序子数组的右边界：
    // 遍历时维护当前遇到的最大值max_val
    // 当发现当前值nums[i] < max_val时，说明i位置需要调整，更新右边界
    int right = -1; // 初始无效值
    int max_val = INT_MIN; // 记录遍历过程中的最大值
    for (int i = 0; i < n; ++i) {
        if (nums[i] < max_val) { // 发现逆序位置
            right = i; // 更新右边界为当前索引
        }
        max_val = max(max_val, nums[i]); // 始终保持max_val为[0,i]区间最大值
    }

    // 确定无序子数组的左边界：
    // 逆向遍历维护当前遇到的最小值min_val
    // 当发现当前值nums[i] > min_val时，说明i位置需要调整，更新左边界
    int left = n; // 初始无效值
    int min_val = INT_MAX; // 记录遍历过程中的最小值
    for (int i = n - 1; i >= 0; --i) {
        if (nums[i] > min_val) { // 发现逆序位置
            left = i; // 更新左边界为当前索引
        }
        min_val = min(min_val, nums[i]); // 始终保持min_val为[i,n-1]区间最小值
    }

    // 计算需要排序的区间长度：
    // 当right未更新时（数组已有序），计算结果为负数，用max处理返回0
    // 时间复杂度O(n)，空间复杂度O(1)
    return max(0, right - left + 1);

    /* 示例分析：
       案例1：[2,6,4,8,10,9,15]
       - 右边界：遍历到9时max_val=10，9<10 → right=5
       - 左边界：遍历到6时min_val=4，6>4 → left=1
       - 结果：5-1+1=5（需排序区间[1,5]）

       案例2：[1,3,2,4,5]
       - 右边界：遍历到2时max_val=3 → right=2
       - 左边界：遍历到3时min_val=2 → left=1
       - 结果：2-1+1=2（区间[1,2]）
       
       已有序数组：[1,2,3,4] → right=-1, left=4 → 0
    */
}
