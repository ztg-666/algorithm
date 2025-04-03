#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
// 机器人跳跃问题
// 机器人正在玩一个古老的基于DOS的游戏
// 游戏中有N+1座建筑，从0到N编号，从左到右排列
// 编号为0的建筑高度为0个单位，编号为i的建筑的高度为H(i)个单位
// 起初机器人在编号为0的建筑处
// 每一步，它跳到下一个（右边）建筑。假设机器人在第k个建筑，且它现在的能量值是E
// 下一步它将跳到第个k+1建筑
// 它将会得到或者失去正比于与H(k+1)与E之差的能量
// 如果 H(k+1) > E 那么机器人就失去H(k+1)-E的能量值，否则它将得到E-H(k+1)的能量值
// 游戏目标是到达第个N建筑，在这个过程中，能量值不能为负数个单位
// 现在的问题是机器人以多少能量值开始游戏，才可以保证成功完成游戏
// 测试链接 : https://www.nowcoder.com/practice/7037a3d57bbd4336856b8e16a9cafd71
class RobotPassThroughBuilding {
public:
    static const int MAXN = 100001; // 定义建筑的最大数量
    static int arr[MAXN];           // 静态数组，用于存储每个建筑的高度
    static int n;                  // 建筑的数量

    // [l,r]通关所需最小能量的范围，不停二分
    // max是所有建筑的最大高度
    // 时间复杂度O(n * log(max))，额外空间复杂度O(1)
    static int compute(int l, int r, int max) {
        int m, ans = -1; // m为二分查找的中间值，ans为最终答案
        while (l <= r) {
            // 计算中间值 m
            m = l + ((r - l) >> 1);
            // 检查初始能量为 m 是否能通关
            if (f(m, max)) {
                // 如果能通关，记录当前能量 m 作为可能的答案，并尝试更小的能量
                ans = m;
                r = m - 1;
            } else {
                // 如果不能通关，尝试更大的能量
                l = m + 1;
            }
        }
        return ans; // 返回最小的初始能量
    }

    // 初始能量为energy，max是建筑的最大高度，返回能不能通关
    // 为什么要给定建筑的最大高度？
    static bool f(int energy, int max) {
        // 注意！
        // 如果给的能量值很大，那么后续能量增长将非常恐怖
        // 完全有可能超出long的范围
        // 所以要在遍历时，一定要加入energy >= max的判断
        // 一旦能量超过高度最大值，后面肯定通关了，可以提前返回了
        // 这里很阴
        for (int i = 1; i <= n; i++) {
            if (energy <= arr[i]) {
                // 如果当前能量小于等于建筑高度，能量减少
                energy -= arr[i] - energy;
            } else {
                // 如果当前能量大于建筑高度，能量增加
                energy += energy - arr[i];
            }
            if (energy >= max) {
                // 如果能量超过最大高度，提前返回 true
                return true;
            }
            if (energy < 0) {
                // 如果能量小于 0，返回 false
                return false;
            }
        }
        return true; // 如果遍历完所有建筑后能量仍然非负，返回 true
    }
};

int RobotPassThroughBuilding::arr[RobotPassThroughBuilding::MAXN]; // 静态数组初始化
int RobotPassThroughBuilding::n;                                  // 建筑数量初始化

int main() {
    std::ios::sync_with_stdio(false); // 加速输入输出
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    // 循环读取输入，直到输入结束
    while (std::cin >> RobotPassThroughBuilding::n) {
        int l = 0; // 二分查找的左边界
        int r = 0; // 二分查找的右边界
        for (int i = 1; i <= RobotPassThroughBuilding::n; i++) {
            std::cin >> RobotPassThroughBuilding::arr[i]; // 读取每个建筑的高度
            r = std::max(r, RobotPassThroughBuilding::arr[i]); // 更新右边界为最大高度
        }
        // 计算并输出所需的最小初始能量
        std::cout << RobotPassThroughBuilding::compute(l, r, r) << std::endl;
    }
}
