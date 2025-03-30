#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
// 表现良好的最长时间段
// 给你一份工作时间表 hours，上面记录着某一位员工每天的工作小时数
// 我们认为当员工一天中的工作小时数大于 8 小时的时候，那么这一天就是 劳累的一天
// 所谓 表现良好的时间段 ，意味在这段时间内，「劳累的天数」是严格 大于 不劳累的天数
// 请你返回 表现良好时间段 的最大长度
class LongestWellPerformingInterval {
public:
    // 计算最长的绩优区间长度
    static int longestWPI(std::vector<int>& hours) {
        // 使用哈希表记录某个前缀和最早出现的位置
        std::unordered_map<int, int> map;

        // 初始化前缀和为0时的位置为-1，表示在数组开始之前的状态
        map[0] = -1;

        // ans用于存储最长绩优区间的长度
        int ans = 0;

        // 遍历hours数组，计算前缀和并更新结果
        for (int i = 0, sum = 0; i < hours.size(); i++) {
            // 如果当前小时数大于8，则视为+1；否则视为-1
            sum += hours[i] > 8 ? 1 : -1;

            // 如果前缀和大于0，说明从开头到当前位置是一个绩优区间
            if (sum > 0) {
                ans = i + 1; // 更新最长绩优区间的长度
            } else {
                // 如果前缀和小于等于0，尝试找到一个更早的前缀和使得差值最大
                if (map.find(sum - 1) != map.end()) {
                    // 当前位置与之前某个位置的差值即为绩优区间的长度
                    ans = std::max(ans, i - map[sum - 1]);
                }
            }

            // 如果当前前缀和未出现在哈希表中，则记录其位置
            if (map.find(sum) == map.end()) {
                map[sum] = i;
            }
        }

        // 返回最长绩优区间的长度
        return ans;
    }
};

int main() {
    // 示例数据
    std::vector<int> hours = {9, 9, 6, 0, 6, 6, 9};

    // 计算最长的绩优区间长度
    int result = LongestWellPerformingInterval::longestWPI(hours);

    // 输出结果
    std::cout << "result: " << result << std::endl;

    return 0;
}
