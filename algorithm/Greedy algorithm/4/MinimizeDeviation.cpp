#include<bits/stdc++.h>

// 数组的最小偏移量
// 给你一个由n个正整数组成的数组nums
// 你可以对数组的任意元素执行任意次数的两类操作：
// 如果元素是偶数，除以2
// 例如如果数组是[1,2,3,4]
// 那么你可以对最后一个元素执行此操作，使其变成[1,2,3,2]
// 如果元素是奇数，乘上2
// 例如如果数组是[1,2,3,4]
// 那么你可以对第一个元素执行此操作，使其变成[2,2,3,4]
// 数组的偏移量是数组中任意两个元素之间的最大差值
// 返回数组在执行某些操作之后可以拥有的最小偏移量
// 测试链接 : https://leetcode.cn/problems/minimize-deviation-in-array/

using namespace std;

class Solution {
public:
    int minimumDeviation(vector<int>& nums) {
        // 使用有序集合维护所有可能调整后的数值
        set<int> s;

        // 预处理所有奇数元素，转换为偶数形式（保证后续可操作）
        for (int num : nums) {
            if (num % 2 == 0) {
                s.insert(num);    // 偶数直接加入集合
            } else {
                s.insert(num * 2); // 奇数乘2转化为偶数（后续可继续调整）
            }
        }

        // 计算当前集合的偏移量（最大值 - 最小值）
        int ans = *s.rbegin() - *s.begin();

        // 当偏移量未达最优且最大值仍可调整时循环
        while (ans > 0 && (*s.rbegin()) % 2 == 0) {
            // 获取当前最大值并移除
            int max_val = *s.rbegin();
            s.erase(max_val);

            // 将最大值折半后重新插入集合（缩小最大值）
            s.insert(max_val / 2);

            // 更新最小偏移量
            ans = min(ans, *s.rbegin() - *s.begin());
        }

        return ans;
    }
};
