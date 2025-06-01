#include <vector>
#include <set>
#include <climits>

// 最小区间
// 你有k个非递减排列的整数列表
// 找到一个最小区间，使得k个列表中的每个列表至少有一个数包含在其中
// 测试链接 : https://leetcode.cn/problems/smallest-range-covering-elements-from-k-lists/

using namespace std;

// 节点结构体：存储数值及其来源信息
struct Node {
    int v; // 当前数值
    int i; // 来自第i个数组（数组索引）
    int j; // 来自该数组的第j个位置

    Node(int val, int arr_idx, int pos) : v(val), i(arr_idx), j(pos) {}
};

// 自定义比较器（用于维护有序集合）
// 排序规则：数值升序排列，数值相同时按数组索引升序排列
struct NodeComparator {
    bool operator()(const Node& a, const Node& b) const {
        if (a.v != b.v) return a.v < b.v; // 主要按数值排序
        return a.i < b.i; // 数值相等时按数组索引排序（避免相同数值覆盖）
    }
};

vector<int> smallestRange(vector<vector<int>>& nums) {
    int k = nums.size(); // 获取输入数组的数量
    set<Node, NodeComparator> sortedSet; // 有序集合维护当前窗口

    // 初始化阶段：将每个数组的首元素插入集合
    for (int i = 0; i < k; ++i) {
        if (!nums[i].empty()) { // 防御性编程（根据题意输入数组非空）
            sortedSet.insert(Node(nums[i][0], i, 0));
        }
    }

    int min_range = INT_MAX; // 记录当前找到的最小区间长度
    int start = 0, end = 0;  // 记录最小区间的起止值

    // 核心循环：当集合包含所有k个数组的代表元素时继续处理
    while (sortedSet.size() == k) {
        // 获取当前窗口的最小值（集合首元素）和最大值（集合尾元素）
        auto min_it = sortedSet.begin();
        auto max_it = sortedSet.rbegin();
        int current_min = min_it->v;
        int current_max = max_it->v;

        // 更新最小区间（发现更窄的区间时记录）
        if (current_max - current_min < min_range) {
            min_range = current_max - current_min;
            start = current_min;
            end = current_max;
        }

        // 取出最小值节点，准备替换为所属数组的下一个元素
        int arr_idx = min_it->i;    // 获取所属数组索引
        int next_pos = min_it->j + 1; // 计算下一个位置
        sortedSet.erase(min_it);    // 移除当前最小值

        // 插入下一个元素（如果尚未遍历完当前数组）
        if (next_pos < nums[arr_idx].size()) {
            sortedSet.insert(Node(nums[arr_idx][next_pos], arr_idx, next_pos));
        }
    }

    // 返回找到的最小区间起止值
    return {start, end};

    /* 算法解析：
       时间复杂度：O(nlogk)，n为所有数组元素总数，每次集合操作O(logk)
       空间复杂度：O(k)，集合中最多存储k个元素

       示例说明：
       输入：[[4,10,15,24,26], [0,9,12,20], [5,18,22,30]]
       初始化集合：{0(数组1),4(数组0),5(数组2)} → 窗口[0,5]
       迭代过程：
       1. 取出0，插入9 → 窗口[4,9] → 区间长度5
       2. 取出4，插入10 → 窗口[5,10] → 长度5
       3. 取出5，插入18 → 窗口[9,18] → 长度9
       4. 取出9，插入12 → 窗口[10,12] → 长度2
       5. ...最终找到最小区间[20,24]
    */
}
