#include <vector>          // 包含 std::vector
#include <algorithm>       // 包含 std::max, std::min, std::fill

// 从树中删除边的最小分数
// 存在一棵无向连通树，树中有编号从0到n-1的n个节点，以及n-1条边
// 给你一个下标从0开始的整数数组nums长度为n，其中nums[i]表示第i个节点的值
// 另给你一个二维整数数组edges长度为n-1
// 其中 edges[i] = [ai, bi] 表示树中存在一条位于节点 ai 和 bi 之间的边
// 删除树中两条不同的边以形成三个连通组件，对于一种删除边方案，定义如下步骤以计算其分数：
// 分别获取三个组件每个组件中所有节点值的异或值
// 最大 异或值和 最小 异或值的 差值 就是这种删除边方案的分数
// 返回可能的最小分数
// 测试链接 : https://leetcode.cn/problems/minimum-score-after-removals-on-a-tree/

const int MAX_NODES_CONST = 1001; // 最大节点数

class Solution {
public:
    // 成员变量，用于存储 DFS（深度优先搜索）信息。


    // dfn_of_node[original_node_id] 存储该节点的 DFN (发现/遍历顺序编号)。
    // DFN 是 1-索引的。
    int dfn_of_node[MAX_NODES_CONST];

    // xor_sum_at_dfn[dfn_id] 存储以 DFN 为 'dfn_id' 的节点为根的
    // 子树中所有节点值的异或和。
    int xor_sum_at_dfn[MAX_NODES_CONST];

    // size_at_dfn[dfn_id] 存储以 DFN 为 'dfn_id' 的节点为根的
    // 子树中的节点数量。
    int size_at_dfn[MAX_NODES_CONST];

    int dfn_counter; // 用于在遍历期间分配 DFN。

    // DFS 辅助函数。
    // u: 当前节点的原始 ID (0 到 n-1)
    // nums_ref: 输入节点值数组的引用
    // graph_ref: 树的邻接表表示的引用
    void build_dfs_info(int u, const std::vector<int>& nums_ref, const std::vector<std::vector<int>>& graph_ref) {
        dfn_counter++; // 分配一个新的 DFN (1-索引)
        int current_node_dfn = dfn_counter;
        dfn_of_node[u] = current_node_dfn;

        xor_sum_at_dfn[current_node_dfn] = nums_ref[u]; // 异或和从节点自身的值开始
        size_at_dfn[current_node_dfn] = 1;             // 大小从 1 开始 (节点本身)

        for (int v : graph_ref[u]) {
            // 如果 dfn_of_node[v] 为 0，表示节点 'v' 在此 DFS 中尚未被访问。
            // 这个检查也防止了在无向图中立即返回父节点。
            if (dfn_of_node[v] == 0) {
                build_dfs_info(v, nums_ref, graph_ref); // 递归调用子节点 'v'

                // 对 'v' 的递归调用返回后 (即 'v' 的子树已处理完毕):
                // 更新当前节点 'u' 子树的异或和
                xor_sum_at_dfn[current_node_dfn] ^= xor_sum_at_dfn[dfn_of_node[v]];
                // 更新当前节点 'u' 子树的大小
                size_at_dfn[current_node_dfn] += size_at_dfn[dfn_of_node[v]];
            }
        }
    }

    int minimumScore(std::vector<int>& nums, std::vector<std::vector<int>>& edges) {
        int n = nums.size(); // 节点数量

        // 构建图的邻接表
        std::vector<std::vector<int>> adj_list(n);
        for (const auto& edge : edges) {
            adj_list[edge[0]].push_back(edge[1]);
            adj_list[edge[1]].push_back(edge[0]);
        }

        // 初始化 dfn_of_node。值为 0 表示未访问的节点。
        // 需要覆盖从 0 到 n-1 的节点。
        std::fill(dfn_of_node, dfn_of_node + n, 0);

        // xor_sum_at_dfn 和 size_at_dfn 按 DFN (1 到 n) 索引。
        // 它们将由 DFS 填充。如果 MAX_NODES_CONST 足够大且 Solution 对象
        // 则隐式处理为 0 初始化。

        dfn_counter = 0; // 为当前调用重置 DFN 计数器

        // 从节点 0 开始 DFS (假设它是连通树的一部分)。
        // 问题说明它是一棵连通树。
        // 分配给节点 0 的 DFN 将是 1。
        build_dfs_info(0, nums, adj_list);

        int min_overall_score = INT_MAX; // 初始化最小分数为最大整数
        int num_edges = edges.size(); // 边的数量

        // 遍历所有唯一的边对以进行移除
        for (int i = 0; i < num_edges; ++i) {
            int u_edge1 = edges[i][0];
            int v_edge1 = edges[i][1];
            // 这条边的子节点（通过切断这条边形成的子树的根）的 DFN
            int dfn_root_subtree1 = (dfn_of_node[u_edge1] > dfn_of_node[v_edge1]) ? dfn_of_node[u_edge1] : dfn_of_node[v_edge1];

            for (int j = i + 1; j < num_edges; ++j) {
                int u_edge2 = edges[j][0];
                int v_edge2 = edges[j][1];
                int dfn_root_subtree2 = (dfn_of_node[u_edge2] > dfn_of_node[v_edge2]) ? dfn_of_node[u_edge2] : dfn_of_node[v_edge2];

                // 对两个被切断子树的 DFN 进行排序：
                // 'dfn_first_cut' 是较小的 DFN，'dfn_second_cut' 是较大的 DFN。
                int dfn_first_cut, dfn_second_cut;
                if (dfn_root_subtree1 < dfn_root_subtree2) {
                    dfn_first_cut = dfn_root_subtree1;
                    dfn_second_cut = dfn_root_subtree2;
                } else {
                    dfn_first_cut = dfn_root_subtree2;
                    dfn_second_cut = dfn_root_subtree1;
                }

                int component_xor1, component_xor2, component_xor3; // 三个连通分量的异或和

                // 分量 1: 由第二次切割定义的子树 (根 DFN: dfn_second_cut)
                component_xor1 = xor_sum_at_dfn[dfn_second_cut];

                // 检查嵌套：'dfn_second_cut' 的子树是否在 'dfn_first_cut' 的子树内部？
                // 子树 'dfn_first_cut' 的 DFN 范围是 [dfn_first_cut, dfn_first_cut + size_at_dfn[dfn_first_cut] - 1]。
                // 如果 dfn_second_cut 落在这个范围内 (并且 dfn_first_cut < dfn_second_cut)，则它是嵌套的。
                if (dfn_second_cut < (dfn_first_cut + size_at_dfn[dfn_first_cut])) {
                    // 情况 1: 嵌套切割。子树 'dfn_second_cut' 在 'dfn_first_cut' 内部。
                    // 分量 2: 'dfn_first_cut' 子树中排除 'dfn_second_cut' 子树的部分。
                    component_xor2 = xor_sum_at_dfn[dfn_first_cut] ^ xor_sum_at_dfn[dfn_second_cut];
                    // 分量 3: 树的其余部分。总异或和是 xor_sum_at_dfn[1] (DFS 起始根节点的 DFN)。
                    component_xor3 = xor_sum_at_dfn[1] ^ xor_sum_at_dfn[dfn_first_cut];
                } else {
                    // 情况 2: 切割是不相交的，或者一个是另一个的祖先但不是特定的嵌套情况。
                    // 由于 dfn_first_cut < dfn_second_cut 并且 dfn_second_cut 不在 dfn_first_cut 的子树中，
                    // 这两个子树 (由 DFN dfn_first_cut 和 dfn_second_cut 为根) 是分离的分量。
                    // 分量 2: 由第一次切割定义的子树 (根 DFN: dfn_first_cut)。
                    component_xor2 = xor_sum_at_dfn[dfn_first_cut];
                    // 分量 3: 树的其余部分。总异或和 ^ component_xor1 ^ component_xor2。
                    component_xor3 = xor_sum_at_dfn[1] ^ component_xor1 ^ component_xor2;
                }

                // 计算此移除配置的分数
                int current_max_xor = std::max({component_xor1, component_xor2, component_xor3});
                int current_min_xor = std::min({component_xor1, component_xor2, component_xor3});
                min_overall_score = std::min(min_overall_score, current_max_xor - current_min_xor);
            }
        }
        return min_overall_score;
    }
};
