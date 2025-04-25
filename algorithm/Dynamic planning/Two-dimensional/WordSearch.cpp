#include <vector>
#include <string>
#include <iostream> // 可选：用于测试/调试

// 使用标准命名空间以简化代码（可选）
using namespace std;
// 单词搜索（无法改成动态规划）
// 给定一个 m x n 二维字符网格 board 和一个字符串单词 word
// 如果 word 存在于网格中，返回 true ；否则，返回 false 。
// 单词必须按照字母顺序，通过相邻的单元格内的字母构成
// 其中"相邻"单元格是那些水平相邻或垂直相邻的单元格
// 同一个单元格内的字母不允许被重复使用
// 测试链接 : https://leetcode.cn/problems/word-search/
class Solution {
private:
    // 递归辅助函数（深度优先搜索）
    // b: 对 board 的引用（允许修改以标记访问过的单元格）
    // i, j: 当前单元格坐标
    // rows, cols: board 的维度（为提高效率而传递）
    // w: 对要搜索的单词 word 的引用
    // k: 当前试图匹配的字符在 'w' 中的索引
    bool f(vector<vector<char>>& b, int i, int j, int rows, int cols, const string& w, int k) {
        // 基本情况 1：成功匹配单词中的所有字符
        if (k == w.length()) {
            return true;
        }

        // 基本情况 2：无效移动 - 检查边界和字符匹配
        // 检查 i 或 j 是否越界，或者 b[i][j] 处的字符是否与 w[k] 不匹配
        if (i < 0 || i >= rows || j < 0 || j >= cols || b[i][j] != w[k]) {
            return false;
        }

        // 当前单元格 (i, j) 有效且匹配 w[k]

        // 通过更改其值临时将当前单元格标记为已访问
        // 使用 '\0'（空字符），类似于 Java 代码中的 0
        char tmp = b[i][j];
        b[i][j] = '\0';

        // 探索邻居（上、下、左、右）以查找下一个字符 (k + 1)
        // 如果任何递归调用返回 true，则表示找到了路径
        bool found = f(b, i - 1, j, rows, cols, w, k + 1) || // 上
                     f(b, i + 1, j, rows, cols, w, k + 1) || // 下
                     f(b, i, j - 1, rows, cols, w, k + 1) || // 左
                     f(b, i, j + 1, rows, cols, w, k + 1);  // 右

        // 回溯：恢复单元格中的原始字符
        // 这允许该单元格在其他潜在路径中使用
        b[i][j] = tmp;

        return found;
    }

public:
    // 主函数，检查单词是否存在于 board 中
    // board: 字符的二维网格（通过引用传递）
    // word: 要搜索的单词（通过引用传递）
    bool exist(vector<vector<char>>& board, string& word) {
        // 处理边界情况：空的 board
        if (board.empty() || board[0].empty()) {
            return false;
        }

        // LeetCode 的约束通常意味着 word 不为空。
        // 如果 word 可能为空，添加：if (word.empty()) return true; （或根据定义返回 false）

        int rows = board.size();
        int cols = board[0].size();

        // 遍历 board 的每个单元格作为潜在的起始点
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                // 从单元格 (i, j) 开始 DFS，尝试匹配第一个字符 (k = 0)
                if (f(board, i, j, rows, cols, word, 0)) {
                    return true; // 找到单词
                }
            }
        }

        // 如果循环完成而没有找到单词，则返回 false
        return false;
    }
};

// 示例用法（可选）
int main() {
    Solution sol;
    vector<vector<char>> board = {{'A','B','C','E'},
                                  {'S','F','C','S'},
                                  {'A','D','E','E'}};
    string word1 = "ABCCED";
    string word2 = "SEE";
    string word3 = "ABCB";

    cout << "单词 \"" << word1 << "\" 是否存在: " << boolalpha << sol.exist(board, word1) << endl; // true
    // 重要提示：在搜索过程中会修改 board。对于在同一个 board 上进行多次搜索，
    // 可能需要传递副本或重置 board（如果修改持续存在）。
    // 然而，在此实现中，回溯会在每次 f() 调用返回后恢复 board 状态。
    // 但是，如果单词很早就被找到，对 exist() 的初始调用可能会使 board 处于修改状态。
    // 为了清晰起见，在调用之间重新初始化，或者更好的是，如果需要多次独立调用，则按值传递 board。
    // 或者确保实现完全恢复状态。这个实现*确实*通过回溯恢复了状态。

    vector<vector<char>> board_copy1 = board; // 如果需要，创建副本
    cout << "单词 \"" << word2 << "\" 是否存在: " << boolalpha << sol.exist(board_copy1, word2) << endl; // true

    vector<vector<char>> board_copy2 = board; // 如果需要，创建副本
    cout << "单词 \"" << word3 << "\" 是否存在: " << boolalpha << sol.exist(board_copy2, word3) << endl; // false

    return 0;
}
