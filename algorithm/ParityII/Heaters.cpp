#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>


// 供暖器
// 冬季已经来临。 你的任务是设计一个有固定加热半径的供暖器向所有房屋供暖。
// 在加热器的加热半径范围内的每个房屋都可以获得供暖。
// 现在，给出位于一条水平线上的房屋 houses 和供暖器 heaters 的位置
// 请你找出并返回可以覆盖所有房屋的最小加热半径。
// 说明：所有供暖器都遵循你的半径标准，加热的半径也一样。
// 测试链接 : https://leetcode.cn/problems/heaters/
class Heaters {
public:
    // 时间复杂度O(n * logn)，因为有排序，额外空间复杂度O(1)
    static int findRadius(std::vector<int>& houses, std::vector<int>& heaters) {
        // 对房屋和供暖器的位置进行排序
        std::sort(houses.begin(), houses.end());
        std::sort(heaters.begin(), heaters.end());

        int ans = 0; // 初始化最大加热半径为0

        // 遍历每个房屋
        for (int i = 0, j = 0; i < houses.size(); i++) {
            // 找到当前房屋的最佳供暖器
            while (!best(houses, heaters, i, j)) {
                j++; // 移动供暖器指针，直到找到最佳供暖器
            }
            // 更新最大加热半径
            ans = std::max(ans, std::abs(heaters[j] - houses[i]));
        }

        return ans; // 返回可以覆盖所有房屋的最小加热半径
    }

private:
    // 这个函数含义：
    // 当前的地点houses[i]由heaters[j]来供暖是最优的吗？
    // 当前的地点houses[i]由heaters[j]来供暖，产生的半径是a
    // 当前的地点houses[i]由heaters[j + 1]来供暖，产生的半径是b
    // 如果a < b, 说明是最优，供暖不应该跳下一个位置
    // 如果a >= b, 说明不是最优，应该跳下一个位置
    static bool best(const std::vector<int>& houses, const std::vector<int>& heaters, int i, int j) {
        // 如果j已经是最后一个供暖器，或者当前供暖器j比下一个供暖器j+1更优
        return j == heaters.size() - 1
               || std::abs(heaters[j] - houses[i]) < std::abs(heaters[j + 1] - houses[i]);
    }
};

int main() {
    // 示例输入
    std::vector<int> houses = {1, 2, 3, 4};
    std::vector<int> heaters = {1, 4};

    // 调用findRadius函数并输出结果
    int result = Heaters::findRadius(houses, heaters);
    std::cout << "The minimum heating radius is: " << result << std::endl;

    return 0;
}
