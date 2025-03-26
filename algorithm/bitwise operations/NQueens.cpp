#include <iostream>
#include <vector>

class NQueens {
public:
    // 使用数组表示路径实现的N皇后问题（不推荐）
    // 参数 n 表示棋盘大小和皇后数量
    // 返回值为满足条件的解法总数
    static int totalNQueens1(int n) {
        if (n < 1) {
            return 0; // 如果棋盘大小小于1，直接返回0
        }
        std::vector<int> path(n); // 用于存储每一行皇后所在的列号
        return f1(0, path, n); // 调用递归函数开始求解
    }

    // i : 当前来到的行
    // path : 0...i-1行的皇后，都摆在了哪些列
    // n : 是几皇后问题
    // 返回 : 0...i-1行已经摆完了，i....n-1行可以去尝试的情况下还能找到几种有效的方法
    static int f1(int i, std::vector<int>& path, int n) {
        if (i == n) {
            return 1; // 如果所有行都已经放置完毕，则找到一种解法
        }
        int ans = 0;
        for (int j = 0; j < n; j++) { // 遍历当前行的所有列
            if (check(path, i, j)) { // 检查当前位置是否合法
                path[i] = j; // 如果合法，记录当前位置
                ans += f1(i + 1, path, n); // 继续递归下一行
            }
        }
        return ans; // 返回当前分支找到的有效解法数
    }

    // 检查当前在i行、j列的位置是否可以放置皇后
    // 返回 true 表示不会冲突，false 表示会冲突
    static bool check(const std::vector<int>& path, int i, int j) {
        for (int k = 0; k < i; k++) { // 遍历之前的所有行
            if (j == path[k] || std::abs(i - k) == std::abs(j - path[k])) {
                return false; // 如果有同行或对角线冲突，返回false
            }
        }
        return true; // 如果没有冲突，返回true
    }

    // 使用位信息表示路径实现的N皇后问题（推荐）
    static int totalNQueens2(int n) {
        if (n < 1) {
            return 0; // 如果棋盘大小小于1，直接返回0
        }
        int limit = (1 << n) - 1; // 计算棋盘的限制范围（所有位均为1）
        return f2(limit, 0, 0, 0); // 调用递归函数开始求解
    }

    // limit : 当前是几皇后问题
    // col : 之前皇后的列影响
    // left : 之前皇后的右上 -> 左下对角线影响
    // right : 之前皇后的左上 -> 右下对角线影响
    static int f2(int limit, int col, int left, int right) {
        if (col == limit) {
            return 1; // 如果所有列都已经放置完毕，则找到一种解法
        }
        int ban = col | left | right; // 计算禁止放置皇后的位置
        int candidate = limit & (~ban); // 计算可以放置皇后的位置
        int place = 0; // 用于存储当前放置皇后的位置
        int ans = 0; // 用于统计有效解法数
        while (candidate != 0) { // 遍历所有可以放置皇后的位置
            place = candidate & (-candidate); // 提取最右侧的1（即一个可放置位置）
            candidate ^= place; // 将该位置从候选集中移除
            ans += f2(limit, col | place, (left | place) >> 1, (right | place) << 1); // 递归处理下一行
        }
        return ans; // 返回当前分支找到的有效解法数
    }
};

int main() {
    int n = 14; // 定义棋盘大小为14
    long start, end;

    std::cout << "start" << std::endl;
    std::cout << "solve " << n << std::endl;

    // 测试第一种方法
    start = clock(); // 开始计时
    std::cout << "1 answer : " << NQueens::totalNQueens1(n) << std::endl; // 输出解法数
    end = clock(); // 结束计时
    std::cout << "1 time : " << (end - start) << " ms" << std::endl; // 输出耗时

    // 测试第二种方法
    start = clock(); // 开始计时
    std::cout << "2 answer : " << NQueens::totalNQueens2(n) << std::endl; // 输出解法数
    end = clock(); // 结束计时
    std::cout << "2 time : " << (end - start) << " ms" << std::endl; // 输出耗时

    std::cout << "end" << std::endl;

    std::cout << "=======" << std::endl;
    std::cout << "16" << std::endl;

    // 测试第二种方法在更大规模下的表现
    start = clock(); // 开始计时
    int ans = NQueens::totalNQueens2(16); // 计算16皇后问题的解法数
    end = clock(); // 结束计时
    std::cout << "ans : " << ans << std::endl; // 输出解法数
    std::cout << "time : " << (end - start) << " ms" << std::endl; // 输出耗时
}
