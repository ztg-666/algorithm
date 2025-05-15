#include <vector>    // 用于使用 std::vector (动态数组)
#include <string>    // 用于使用 std::string (字符串)
#include <algorithm> // 用于使用 std::max (求最大值函数)

// 相邻字符不同的最长路径
// 给你一棵 树（即一个连通、无向、无环图），根节点是节点 0
// 这棵树由编号从 0 到 n - 1 的 n 个节点组成
// 用下标从 0 开始、长度为 n 的数组 parent 来表示这棵树
// 其中 parent[i] 是节点 i 的父节点
// 由于节点 0 是根节点，所以 parent[0] == -1
// 另给你一个字符串 s ，长度也是 n ，其中 s[i] 表示分配给节点 i 的字符
// 请你找出路径上任意一对相邻节点都没有分配到相同字符的 最长路径
// 并返回该路径的长度
// 测试链接 : https://leetcode.cn/problems/longest-path-with-different-adjacent-characters/

class Solution {
private:

    // Info 结构体包含两个成员：
    //  maxPathFromHead: 表示必须从当前节点 (u) 出发向下（即走向子节点）的、
    //                   且路径上相邻节点字符不同的最长路径长度。
    //  maxPath:         表示在以当前节点 (u) 为根的整个子树中，
    //                   路径上相邻节点字符不同的最长路径长度。
    //                   (这条最长路径可能包含节点 u，也可能不包含 u 而完全存在于某个子树中)。
    struct Info {
        int maxPathFromHead;
        int maxPath;

        // 构造函数
        Info(int fromHead, int overallMax) : maxPathFromHead(fromHead), maxPath(overallMax) {}
    };


    // s: 表示每个节点字符的字符串。
    // graph: 树的邻接表表示 (graph[i] 存储节点 i 的所有子节点)。
    // u: 当前正在处理的节点。
    Info dfs(const std::string& s, const std::vector<std::vector<int>>& graph, int u) {
        // 基本情况：如果 u 是叶节点 (在图中没有子节点)。

        if (graph[u].empty()) {
            // 从头节点 u 出发的路径长度为 1 (节点 u 本身)。
            // 在这个只包含叶节点的子树中，最长路径也是 1。
            return Info(1, 1);
        }

        int path1Len = 0; // 从 u 向下延伸的最长链的长度。
                          // 这条链必须通过一个子节点 v，且满足 s[u] != s[v]。
        int path2Len = 0; // 从 u 向下延伸的次长链的长度，满足类似条件。

        // 这个变量将存储到目前为止，在以 u 为根的子树中找到的最长路径。
        // 初始化为 1 (代表节点 u 本身)。它会被以下两种情况更新：
        //   1. 完全存在于某个孩子子树内的最长路径。
        //   2. 穿过节点 u 的最长路径。
        int subtreeResultantMaxPath = 1;

        for (int v : graph[u]) { // 对于 u 的每个子节点 v
            Info childInfo = dfs(s, graph, v); // 递归调用 dfs 处理子节点 v

            // 整棵树的最长路径可能完全包含在子节点 v 的子树中。
            subtreeResultantMaxPath = std::max(subtreeResultantMaxPath, childInfo.maxPath);

            // 如果节点 u 的字符与子节点 v 的字符不同，
            // 那么从 v 出发的路径可以扩展一条从 u 开始的路径。
            if (s[u] != s[v]) {
                // childInfo.maxPathFromHead 是从子节点 v 出发的、符合条件的最长路径。
                if (childInfo.maxPathFromHead > path1Len) {
                    path2Len = path1Len;
                    path1Len = childInfo.maxPathFromHead;
                } else if (childInfo.maxPathFromHead > path2Len) {
                    path2Len = childInfo.maxPathFromHead;
                }
            }
        }

        // 从 u 出发向下（且相邻字符不同）的最长路径长度是 1 (计入 u) + path1Len。
        int currentMaxPathFromHead = path1Len + 1;

        // *穿过* 节点 u 的最长路径是由节点 u 本身，
        // 加上向下连接到某个字符不同子节点的最长链 (path1Len)，
        // 再加上向下连接到另一个不同字符子节点的次长链 (path2Len) 组成的。
        // 长度 = 1 (节点 u) + path1Len + path2Len。
        int pathThroughCurrentNode = path1Len + path2Len + 1;

        // 最终，以 u 为根的子树中的最长路径是以下两者中的较大值：
        //   - 完全存在于某个孩子子树内的最长路径 (已在 subtreeResultantMaxPath 中考虑)
        //   - 穿过节点 u 的最长路径 (pathThroughCurrentNode)
        subtreeResultantMaxPath = std::max(subtreeResultantMaxPath, pathThroughCurrentNode);

        return Info(currentMaxPathFromHead, subtreeResultantMaxPath);
    }

public:

    int longestPath(std::vector<int>& parent, std::string s_str) {
        int n = parent.size(); // 获取节点数量

        // 根据 LeetCode 的约束，n >= 1。
        // 如果 n 为 0，我们可能返回 0。如果 n 为 1，当前逻辑也能正确处理。
        if (n == 0) {
            return 0;
        }

        // 构建图 (邻接表，其中 graph[i] 存储节点 i 的所有子节点)
        // std::vector<std::vector<int>> graph(n); 初始化一个包含 n 个空 vector 的 vector。
        std::vector<std::vector<int>> graph(n);
        for (int i = 1; i < n; ++i) { // 节点 0 是根节点, parent[0] 为 -1。
                                      // 从节点 1 遍历到 n-1。
            if (parent[i] != -1) { // parent[i] 是节点 i 的父节点
                graph[parent[i]].push_back(i); // 将 i 添加为 parent[i] 的子节点
            }
        }

        // 从根节点 (节点 0) 开始调用递归辅助函数
        Info result = dfs(s_str, graph, 0);

        // 从根节点调用 dfs 返回的 result.maxPath 即为整棵树的答案
        return result.maxPath;
    }
};

// 如何调用这个类的示例 (例如，在 main 函数中进行测试):

#include <iostream> // 用于 std::cout
int main() {
    Solution sol; // 创建 Solution 类的对象
    std::vector<int> parent1 = {-1, 0, 0, 1, 1, 2};
    std::string s1 = "abacbe";
    std::cout << " 1: " << sol.longestPath(parent1, s1) << std::endl; // 期望输出: 3

    std::vector<int> parent2 = {-1, 0, 0, 0};
    std::string s2 = "aabc";
    std::cout << " 2: " << sol.longestPath(parent2, s2) << std::endl; // 期望输出: 3

    std::vector<int> parent3 = {-1};
    std::string s3 = "z";
    std::cout << " 3: " << sol.longestPath(parent3, s3) << std::endl; // 期望输出: 1

    return 0;
}

