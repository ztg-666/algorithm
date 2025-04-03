#include <vector>
#include <algorithm>
#include <iostream>
// 救生艇
// 给定数组 people
// people[i]表示第 i 个人的体重 ，船的数量不限，每艘船可以承载的最大重量为 limit
// 每艘船最多可同时载两人，但条件是这些人的重量之和最多为 limit
// 返回 承载所有人所需的最小船数
class BoatsToSavePeople {
public:
    // 时间复杂度O(n * logn)，因为有排序，额外空间复杂度O(1)
    static int numRescueBoats(std::vector<int>& people, int limit) {
        // 对人员重量数组进行排序
        std::sort(people.begin(), people.end());
        int ans = 0;  // 记录所需船只的数量
        int l = 0;    // 左指针，指向最轻的人
        int r = people.size() - 1;  // 右指针，指向最重的人
        int sum = 0;  // 当前左右指针指向的人的重量和

        // 使用双指针方法，从两端向中间移动
        while (l <= r) {
            // 计算当前左右指针指向的人的重量和
            sum = (l == r) ? people[l] : people[l] + people[r];

            // 如果重量和超过限制，只能让最重的人单独乘一艘船
            if (sum > limit) {
                r--;  // 右指针左移
            } else {
                // 如果重量和不超过限制，两人可以共乘一艘船
                l++;  // 左指针右移
                r--;  // 右指针左移
            }
            ans++;  // 增加船只数量
        }

        return ans;  // 返回所需船只的总数
    }
};

int main() {
    // 示例数组
    std::vector<int> people = {3, 2, 2, 1};
    int limit = 3;

    // 调用numRescueBoats方法并输出结果
    int result = BoatsToSavePeople::numRescueBoats(people, limit);
    std::cout << "Number of boats needed: " << result << std::endl;

    return 0;
}
