#include <vector>
#include <algorithm>

// 森林中的兔子
// 森林中有未知数量的兔子
// 你问兔子们一个问题: "还有多少只兔子与你颜色相同?"
// 你将答案收集到了一个数组answers中
// 你可能没有收集到所有兔子的回答，可能只是一部分兔子的回答
// 其中answers[i]是第i只兔子的答案
// 所有兔子都不会说错，返回森林中兔子的最少数量
// 测试链接 : https://leetcode.cn/problems/rabbits-in-forest/

using namespace std;

class Solution {
public:
    int numRabbits(vector<int>& answers) {
        // 对回答数组进行排序，便于处理相同数值的连续元素
        sort(answers.begin(), answers.end());
        int n = answers.size();
        int ans = 0;

        // 使用双指针遍历数组，i指向当前组的起始位置
        for (int i = 0, j = 1; i < n; ) {
            int x = answers[i]; // 当前组的回答值

            // 寻找相同回答的连续元素，j最终指向下一个不同值的起始位置
            while (j < n && answers[j] == x) {
                j++;
            }

            int cnt = j - i; // 当前组共有cnt个相同的回答

            /* 计算该颜色组所需的最少兔子数量：
               1. 每个颜色组最多允许有(x + 1)只兔子（x为回答值）
               2. (cnt + x) / (x + 1) 实现向上取整，计算需要多少组
               3. 乘以(x + 1)得到该颜色总兔子数 */
            ans += (cnt + x) / (x + 1) * (x + 1);

            i = j; // 移动到下一组的起始位置
            j++;   // j指针始终保持在当前i的下一个位置
        }

        return ans;
    }
};
