#include <iostream>
#include <vector>
#include <algorithm> // For std::max
#include <vector>    // Already included, but good to be explicit

// 没有上司的舞会
// 某大学有n个职员，编号为1...n
// 他们之间有从属关系，也就是说他们的关系就像一棵以校长为根的树
// 父结点就是子结点的直接上司
// 现在有个周年庆宴会，宴会每邀请来一个职员都会增加一定的快乐指数
// 但是如果某个职员的直接上司来参加舞会了
// 那么这个职员就无论如何也不肯来参加舞会了
// 所以请你编程计算邀请哪些职员可以使快乐指数最大
// 返回最大的快乐指数。
// 测试链接 : https://www.luogu.com.cn/problem/P1352
// 本题和讲解037的题目7类似
// 链式链接 : https://leetcode.cn/problems/house-robber-iii/

// 使用常量定义最大员工数，方便1基索引
const int MAX_EMPLOYEES = 6001;


// 存储每个员工的快乐指数
std::vector<int> happiness_values(MAX_EMPLOYEES);
// has_superior[i]为true表示员工i有直接上司
std::vector<bool> has_superior(MAX_EMPLOYEES);

// 使用邻接表（链式前向星）存储树的结构 (上司 -> 下属)
std::vector<int> adj_head(MAX_EMPLOYEES);      // adj_head[u] 存储u发出的第一条边的索引
std::vector<int> adj_next_edge(MAX_EMPLOYEES); // adj_next_edge[edge_idx] 存储同起点下一条边的索引
std::vector<int> adj_to_node(MAX_EMPLOYEES);   // adj_to_node[edge_idx] 存储边的目标节点
int adj_edge_counter;                          // 边的计数器，用作adj_next_edge和adj_to_node的索引

// 动态规划数组
// dp_attending[i] 表示以i为根的子树中，i参加舞会时的最大快乐指数
std::vector<int> dp_attending(MAX_EMPLOYEES);
// dp_not_attending[i] 表示以i为根的子树中，i不参加舞会时的最大快乐指数
std::vector<int> dp_not_attending(MAX_EMPLOYEES);

int g_num_employees;  // 全局变量存储员工数量
int g_root_node_id;   // 全局变量存储树的根节点（校长）


// 准备存储结构以处理包含n个员工的树
void initialize_structures(int n) {
    g_num_employees = n;

    // 清理或初始化与员工数量相关的结构
    // 员工编号从1到n
    for (int i = 1; i <= n; ++i) {
        has_superior[i] = false; // 最初假设没有人有上司
        adj_head[i] = 0;         // 0 表示该节点没有发出边
        // DP数组将在DFS计算时被填充，此处无需初始化
    }
    adj_edge_counter = 1; // 边的索引从1开始
}

// 添加一条从 u_from_boss (上司) 到 v_to_subordinate (下属) 的有向边
void add_directed_edge(int u_from_boss, int v_to_subordinate) {
    adj_next_edge[adj_edge_counter] = adj_head[u_from_boss];
    adj_to_node[adj_edge_counter] = v_to_subordinate;
    adj_head[u_from_boss] = adj_edge_counter;
    adj_edge_counter++;
}


void dfs_calculate_dp(int u_current_node) {
    // 如果当前员工 u_current_node 不参加舞会
    dp_not_attending[u_current_node] = 0;
    // 如果当前员工 u_current_node 参加舞会，则其快乐指数计入
    dp_attending[u_current_node] = happiness_values[u_current_node];

    // 遍历 u_current_node 的所有直接下属 v_subordinate_node
    for (int edge_idx = adj_head[u_current_node]; edge_idx != 0; edge_idx = adj_next_edge[edge_idx]) {
        int v_subordinate_node = adj_to_node[edge_idx];

        dfs_calculate_dp(v_subordinate_node); // 递归计算下属子树的DP值

        // 如果 u_current_node 不参加：
        // 那么其下属 v_subordinate_node 可以参加也可以不参加，取两者中的最大值
        dp_not_attending[u_current_node] += std::max(dp_not_attending[v_subordinate_node], dp_attending[v_subordinate_node]);

        // 如果 u_current_node 参加：
        // 那么其下属 v_subordinate_node 必不能参加
        dp_attending[u_current_node] += dp_not_attending[v_subordinate_node];
    }
}

int main() {
    // 优化C++标准输入输出流的性能
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL); // 对于单行输出，cout.tie(NULL) 可能不是必需的，但无害

    int n_input;
    std::cin >> n_input; // 读取员工数量

    // 在此问题背景下，对于单个N的输入，其效果是读取一次N然后处理。

    initialize_structures(n_input);

    // 读取每个员工的快乐指数
    for (int i = 1; i <= g_num_employees; ++i) {
        std::cin >> happiness_values[i];
    }

    // 读取 n-1 条上下级关系
    // 每条关系表示为 (下属 L,上司 K)
    for (int i = 0; i < g_num_employees - 1; ++i) {
        int subordinate_l, boss_k;
        std::cin >> subordinate_l >> boss_k;
        add_directed_edge(boss_k, subordinate_l); // 添加从 boss_k 到 subordinate_l 的边
        has_superior[subordinate_l] = true;       // 标记 subordinate_l 有上司
    }

    // 找到根节点（校长，即没有上司的员工）
    g_root_node_id = -1; // 初始化为无效值
    for (int i = 1; i <= g_num_employees; ++i) {
        if (!has_superior[i]) {
            g_root_node_id = i;
            break;
        }
    }

    // 根据题目约束 N >= 1，总能找到一个根节点。
    // 特殊情况处理：
    // 如果 g_num_employees == 0 (根据约束不可能)，最大快乐指数为0。
    // 如果 g_num_employees == 1，根节点为1。dfs_calculate_dp(1)将正确计算。
    //   dp_not_attending[1] = 0, dp_attending[1] = happiness_values[1]。
    //   输出 std::max(happiness_values[1], 0)，这是正确的。

    if (g_num_employees == 0) {
        std::cout << 0 << std::endl;
    } else {
        // 断言 g_root_node_id != -1 (即根节点已找到)
        dfs_calculate_dp(g_root_node_id);
        std::cout << std::max(dp_attending[g_root_node_id], dp_not_attending[g_root_node_id]) << std::endl;
    }

    return 0;
}

