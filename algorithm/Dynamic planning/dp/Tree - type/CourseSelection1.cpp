#include <iostream>      // 包含 std::cin, std::cout
#include <vector>        // 包含 std::vector
#include <algorithm>     // 包含 std::max

// 选课
// 在大学里每个学生，为了达到一定的学分，必须从很多课程里选择一些课程来学习
// 在课程里有些课程必须在某些课程之前学习，如高等数学总是在其它课程之前学习
// 现在有 N 门功课，每门课有个学分，每门课有一门或没有直接先修课
// 若课程 a 是课程 b 的先修课即只有学完了课程 a，才能学习课程 b
// 一个学生要从这些课程里选择 M 门课程学习
// 问他能获得的最大学分是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2014



const int MAX_NODES_GLOBAL = 301; // 最大课程数 N (1 到 N) + 虚拟根节点 0


int node_scores[MAX_NODES_GLOBAL];         // 存储每个节点的学分
std::vector<std::vector<int>> adj_list; // 树形结构的邻接表

// DP 表: dp_table_for_node[node_idx][child_group_size][courses_to_select_count]
// - node_idx: 我们当前考虑的节点 'u'。
// - child_group_size: 它是 'u' 的子节点中，
//                     包含在当前子问题考虑范围内的数量（特指前 'child_group_size' 个子节点）。
// - courses_to_select_count: 从以 'u' 为根的子树中
//                            （包括 'u'），通过仅使用其前 'child_group_size' 个子节点，
//                            要选择的确切课程数量。
std::vector<std::vector<int>> dp_table_for_node[MAX_NODES_GLOBAL]; // DP 状态的二维向量数组

int N_total_courses_input;      // 问题描述中的 N (实际课程数，例如 1 到 N)
int M_courses_to_pick_plus_1; // 问题描述中的 M + 1 (在选择计数中考虑虚拟根节点 0)

// 递归 DP 函数
// u_node: 当前节点索引。
// children_to_process_count: 为此子问题要考虑的 u_node 的子节点数量。
//                            (例如，如果 adj_list[u_node].size()，则考虑所有子节点；如果为 0，则不考虑)。
// k_items_to_select: 在以 u_node 为根的子树中选择的确切课程数量
//                    (如果 k_items_to_select > 0，则始终包括 u_node 本身)，
//                    通过在 u_node 的前 'children_to_process_count' 个子节点中分配选择。
int solve_dp(int u_node, int children_to_process_count, int k_items_to_select) {
    // 基本情况 1: 如果我们需要选择 0 门课程。
    if (k_items_to_select == 0) {
        return 0; // 得分为 0 (在此路径中未选择 u_node)。
    }

    // 基本情况 2:
    // - 如果 u_node 没有更多子节点可供此路径考虑 (children_to_process_count == 0)。
    // - 或者，如果只需要选择 1 门课程 (必须是 u_node 本身)。
    if (children_to_process_count == 0 || k_items_to_select == 1) {
        // 如果 k_items_to_select == 1，我们必须选择 u_node。得分是它自己的学分。
        // 如果 children_to_process_count == 0:
        //   - 如果 k_items_to_select == 1，选择 u_node。得分为 node_scores[u_node]。
        //   - 如果 k_items_to_select > 1，如果只有 u_node 可用（未考虑子节点），则不可能选择超过 1 个节点。
        //     这是因为如果此状态是旨在实现无法达到的 'k' 的更大总和的一部分，
        //     则整个路径可能不是最优的，或者会被 -1 的传播正确过滤掉（作为一个不可能的状态）。
        return node_scores[u_node];
    }

    // 记忆化搜索: 检查此状态是否已计算。
    if (dp_table_for_node[u_node][children_to_process_count][k_items_to_select] != -1) {
        return dp_table_for_node[u_node][children_to_process_count][k_items_to_select];
    }

    // 递归步骤:
    // 选项 1: 不从当前正在决定的子节点的子树中选择任何课程。
    // "当前子节点" 是 adj_list[u_node][children_to_process_count - 1]。
    // 我们通过将 u_node 要处理的子节点计数减少 1 来递归。
    int max_score_for_state = solve_dp(u_node, children_to_process_count - 1, k_items_to_select);

    // 选项 2: 从当前子节点 (v_child_node) 的子树中选择 's_courses_from_v_child' 门课程。
    int v_child_node = adj_list[u_node][children_to_process_count - 1]; // 0-索引的子节点

    // 迭代 's_courses_from_v_child': 从 v_child_node 的子树中选择的课程数量 (包括 v_child_node)。
    // - s_courses_from_v_child 必须 >= 1 (因为如果其子树有贡献，则 v_child_node 本身被选中)。
    // - (k_items_to_select - s_courses_from_v_child) 必须 >= 1 (因为 u_node 本身被选中)。
    // 所以, 1 <= s_courses_from_v_child <= k_items_to_select - 1。
    for (int s_courses_from_v_child = 1; s_courses_from_v_child < k_items_to_select; ++s_courses_from_v_child) {
        // 来自 u_node 及其 *除* v_child_node 之外的子节点（即前 children_to_process_count - 1 个子节点）的得分，
        // 选择 (k_items_to_select - s_courses_from_v_child) 门课程。
        int score_from_u_and_earlier_children = solve_dp(u_node, children_to_process_count - 1, k_items_to_select - s_courses_from_v_child);

        // 来自 v_child_node 整个子树（考虑其所有子节点）的得分，选择 s_courses_from_v_child 门课程。
        int score_from_v_child_subtree = solve_dp(v_child_node, adj_list[v_child_node].size(), s_courses_from_v_child);

        // 如果任一子问题不可能（返回 -1），它们的和可能为非正数或反映无效路径。
        // 学分是非负的。涉及 -1 的和将小于任何有效的学分和。
        // 这个 max 比较处理了它：如果一项是 -1，和可能小于有效的正分。
        max_score_for_state = std::max(max_score_for_state, score_from_u_and_earlier_children + score_from_v_child_subtree);

    }

    // 存储并返回此状态计算出的最大得分。
    dp_table_for_node[u_node][children_to_process_count][k_items_to_select] = max_score_for_state;
    return max_score_for_state;
}


void prepare_graph_for_testcase(int n_nodes_in_current_testcase) {
    // 确保 adj_list 足够大，并清除先前测试用例的数据。
    if (adj_list.size() <= n_nodes_in_current_testcase) {
        adj_list.resize(n_nodes_in_current_testcase + 1);
    }
    for (int i = 0; i <= n_nodes_in_current_testcase; ++i) {
        adj_list[i].clear();
    }
    // DP 表的初始化在当前测试用例的图完全构建之后，在 main 函数中完成，
    // 因为 dp_table_for_node[i] 的第二维依赖于 adj_list[i].size()。
}

int main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int M_courses_to_select_input;

    // 假设每次运行一个测试用例，这是洛谷在线判题的典型情况。
    // 如果每次执行预期有多个测试用例，则输入读取将在循环中进行。
    if (!(std::cin >> N_total_courses_input >> M_courses_to_select_input)) {
        return 1; // 表示输入错误。
    }

    // 调整 M：要选择的目标节点数是 M_courses_to_select_input 个实际课程 + 1 个虚拟根节点 (节点 0)。
    M_courses_to_pick_plus_1 = M_courses_to_select_input + 1;

    prepare_graph_for_testcase(N_total_courses_input); // 清除节点 0 到 N_total_courses_input 的邻接表。

    node_scores[0] = 0; // 虚拟根节点 0 的学分始终为 0。
    // 读取课程数据：课程 1 到 N_total_courses_input 的先修课程和学分。
    for (int i = 1; i <= N_total_courses_input; ++i) {
        int prereq_node_idx, score_value;
        std::cin >> prereq_node_idx >> score_value;
        adj_list[prereq_node_idx].push_back(i); // prereq_node_idx 是课程 i 的父节点。
        node_scores[i] = score_value;
    }

    // 在当前测试用例的图构建完成后初始化 DP 表。
    for (int i = 0; i <= N_total_courses_input; ++i) {
        // dp_table_for_node[i] 是 std::vector<std::vector<int>>
        // 'children_to_process_count' (第二维) 的最大值是 adj_list[i].size()。
        // 'k_items_to_select' (第三维) 的最大值是 M_courses_to_pick_plus_1。
        // 将所有 DP 状态初始化为 -1 (表示“尚未计算”)。
        dp_table_for_node[i].assign(adj_list[i].size() + 1, std::vector<int>(M_courses_to_pick_plus_1 + 1, -1));
    }

    // 对 DP 函数的初始调用，从虚拟根节点 0 开始。
    // 我们考虑根节点 0 的所有子节点，并旨在选择 M_courses_to_pick_plus_1 个节点。
    int final_max_score = solve_dp(0, adj_list[0].size(), M_courses_to_pick_plus_1);

    // 课程学分是非负的。如果 final_max_score 为 -1，则表示无法选择所需数量的课程。
    // 在这种情况下，可获得的最大分数为 0。
    if (final_max_score < 0) {
        final_max_score = 0;
    }

    std::cout << final_max_score << std::endl;

    return 0;
}
