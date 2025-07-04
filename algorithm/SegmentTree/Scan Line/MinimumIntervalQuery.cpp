#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <utility>

// 包含每个查询的最小区间
// 给你一个二维整数数组intervals，其中intervals[i] = [l, r]
// 表示第i个区间开始于l，结束于r，区间的长度是r-l+1
// 给你一个整数数组queries，queries[i]表示要查询的位置
// 答案是所有包含queries[i]的区间中，最小长度的区间是多长
// 返回数组对应查询的所有答案，如果不存在这样的区间那么答案是-1
// 测试链接 : https://leetcode.cn/problems/minimum-interval-to-include-each-query/

class Solution {
public:
    /**
     * @brief 计算包含每个查询点的最小区间长度。
     *
     * @param intervals 二维整数向量，intervals[i] = {l, r} 表示一个区间。
     * @param queries   一个整数向量，表示要查询的位置。
     * @return std::vector<int> 一个向量，包含每个查询的答案。如果某个查询点不存在于任何区间中，则答案为-1。
     */
    static std::vector<int> minInterval(std::vector<std::vector<int>>& intervals, std::vector<int>& queries) {
        int n = intervals.size();
        int m = queries.size();

        // 1. 按区间起始点对 intervals 进行排序
        std::sort(intervals.begin(), intervals.end(), [](const auto& a, const auto& b) {
            return a[0] < b[0];
        });

        // 2. 创建一个查询数组，额外存储原始索引以便排序后能放回正确位置
        std::vector<std::pair<int, int>> ques;
        for (int i = 0; i < m; ++i) {
            ques.emplace_back(queries[i], i);
        }

        // 3. 按查询点的位置对 ques 进行排序
        std::sort(ques.begin(), ques.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        // 4. 创建一个最小堆，用于存储区间的 {长度, 结束点}
        //    C++ 的 priority_queue 默认是最大堆，所以需要提供 std::greater 使其成为最小堆
        std::priority_queue<
            std::pair<int, int>,
            std::vector<std::pair<int, int>>,
            std::greater<std::pair<int, int>>
        > heap;

        std::vector<int> ans(m);
        int interval_idx = 0; // 指向 intervals 的指针

        // 5. 遍历排序后的查询
        for (int i = 0; i < m; ++i) {
            int current_query_val = ques[i].first;
            int original_index = ques[i].second;

            // 将所有起点小于等于当前查询点的区间加入最小堆
            while (interval_idx < n && intervals[interval_idx][0] <= current_query_val) {
                int length = intervals[interval_idx][1] - intervals[interval_idx][0] + 1;
                int end_point = intervals[interval_idx][1];
                heap.push({length, end_point});
                interval_idx++;
            }

            // 从堆顶移除所有已经不包含当前查询点的区间（即结束点在查询点左侧）
            while (!heap.empty() && heap.top().second < current_query_val) {
                heap.pop();
            }

            // 如果堆不为空，堆顶的区间就是包含当前查询点的、长度最小的区间
            if (!heap.empty()) {
                ans[original_index] = heap.top().first;
            } else {
                ans[original_index] = -1;
            }
        }

        return ans;
    }
};

// // 用于测试的主函数 (可选)
// int main() {
//     std::vector<std::vector<int>> intervals = {{1, 4}, {2, 4}, {3, 6}, {4, 4}};
//     std::vector<int> queries = {2, 3, 4, 5};

//     std::vector<int> result = Solution::minInterval(intervals, queries);

//     std::cout << "查询结果: ";
//     for (int val : result) {
//         std::cout << val << " ";
//     }
//     std::cout << std::endl; // 预期输出: 3 3 1 4

//     return 0;
// }
