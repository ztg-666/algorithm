#include <vector>    // 用于输入矩阵 'mat'
#include <algorithm> // 用于 std::max
#include <cstring>   // 用于 std::memset
#include <iostream>
using namespace std;

const int MAXM = 151; // 定义最大可能的列数 + 1

// 统计全1子矩形的数量
// 给你一个 m * n 的矩阵 mat，其中只有0和1两种值
// 请你返回有多少个 子矩形 的元素全部都是1
class Solution {
private:
    // 声明高度数组和栈数组（成员变量）
    int height[MAXM]{};
    int stack[MAXM]{};
    // 'r' 用于跟踪栈中的元素数量（栈指针）
    int r=0;

    // 该函数计算当前行（由 'height' 表示）对总子矩阵数量的贡献。
    // 使用静态成员数组 'height' 和 'stack'。
    // m: 当前问题实例中矩阵的实际列数。
    int countFromBottom(int m) {
        r = 0; // 每行计算前重置栈指针
        int ans = 0;

        // 遍历列以构建和处理单调栈
        for (int i = 0; i < m; ++i) {
            // 当栈不为空且栈顶高度大于或等于当前高度时...
            while (r > 0 && height[stack[r - 1]] >= height[i]) {
                // 从栈中弹出索引
                int cur = stack[--r];

                // 仅当弹出的高度严格大于当前高度时进行计算。
                // 如果它们相等，则计算延迟到 'i' 处理时进行。
                if (height[cur] > height[i]) {
                    // 确定左侧边界（左侧第一个小于 'cur' 的柱形索引）
                    int left = (r == 0) ? -1 : stack[r - 1]; // 如果栈不为空，则查看栈顶元素
                    // 计算高度在 height[cur] 和 bottom 之间的矩形的宽度
                    int len = i - left - 1;
                    // 确定矩形底部高度限制
                    int bottom = max(left == -1 ? 0 : height[left], height[i]);
                    // 计算以 'cur' 位置结束的矩形数量
                    // 公式：(高度差) * (可能的宽度数量)
                    // 长度为 'len' 的可能宽度数量为 len*(len+1)/2（即 1+2+...+len）
                    ans += (height[cur] - bottom) * len * (len + 1) / 2;
                }
            }
            // 将当前列索引压入栈
            stack[r++] = i;
        }

        // 处理遍历所有列后栈中剩余的元素
        while (r > 0) {
            // 弹出索引
            int cur = stack[--r];
            // 确定左侧边界
            int left = (r == 0) ? -1 : stack[r - 1]; // 查看栈顶元素
            // 计算宽度（右侧边界为矩阵末尾 'm'）
            int len = m - left - 1;
            // 确定矩形底部高度限制（现在仅受限于左侧）
            int down = left == -1 ? 0 : height[left];
            // 添加剩余元素的贡献
            ans += (height[cur] - down) * len * (len + 1) / 2;
        }
        return ans;
    }

public:
    // 主函数，计算所有元素均为1的子矩阵的总数
    int numSubmat(vector<vector<int>>& mat) {
        if (mat.empty() || mat[0].empty()) {
            return 0;
        }
        int n = mat.size();
        int m = mat[0].size();

        // 确保矩阵尺寸不超过静态数组的边界。
        // LeetCode 约束通常使此检查不必要，但这是一个良好的实践。
        if (m >= MAXM) {
            // 处理错误：矩阵对于静态数组来说太宽
            return -1; // 或者抛出异常
        }

        int ans = 0;
        // 在开始之前将高度数组初始化为全零。
        // 使用 memset 对整数数组设置为 0 是高效的。
        memset(height, 0, sizeof(height[0]) * m); // 只需要清除 'm' 个元素

        // 遍历矩阵的每一行
        for (int i = 0; i < n; ++i) {
            // 根据当前行更新高度数组
            for (int j = 0; j < m; ++j) {
                height[j] = (mat[i][j] == 0) ? 0 : height[j] + 1;
            }
            // 添加使用更新后的高度计算出的当前行的子矩阵数量
            ans += countFromBottom(m); // 传递实际列数 'm'
        }
        return ans;
    }
};


int main() {
    // 创建一个测试矩阵
    vector<vector<int>> testMat = {
        {1, 0, 1},
        {1, 1, 0},
        {1, 1, 0}
    };

    // 创建 Solution 类的实例
    Solution solution;

    // 调用 numSubmat 函数并输出结果
    int result = solution.numSubmat(testMat);
    cout << "Number of submatrices with all ones: " << result << endl;

    return 0;
}