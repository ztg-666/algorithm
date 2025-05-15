#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::fill 和 std::max
#include <numeric>   // 此处并非严格需要，但常见

// 选课
// 在大学里每个学生，为了达到一定的学分，必须从很多课程里选择一些课程来学习
// 在课程里有些课程必须在某些课程之前学习，如高等数学总是在其它课程之前学习
// 现在有 N 门功课，每门课有个学分，每门课有一门或没有直接先修课
// 若课程 a 是课程 b 的先修课即只有学完了课程 a，才能学习课程 b
// 一个学生要从这些课程里选择 M 门课程学习
// 问他能获得的最大学分是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2014

// 最优解，链式前向星（使用数组模拟）建图 + 利用 DFN 序 + DP 状态的巧妙定义
// 时间复杂度 O(N*M)，如果觉得困难可以跳过，这个最优解非常巧妙和精彩！

const int MAXN_COURSES = 301; // 最大课程数量 + 虚拟根节点 0

int course_credits[MAXN_COURSES]; // 存储每个课程ID的学分 (对应 Java 中的 nums)

// 图的邻接表 (使用基于数组的链表 / “链式前向星”)
int edge_count;                     // 边的计数器
int head_edge[MAXN_COURSES];      // head_edge[u] 存储从 u 出发的第一条边的索引
int next_node_edge[MAXN_COURSES]; // next_node_edge[edge_idx] 存储从同一个 u 出发的下一条边的索引
int to_node[MAXN_COURSES];        // to_node[edge_idx] 存储边的目标节点 v

// DFN (深度优先编号) 序相关数组
int dfn_counter;                  // DFN 分配计数器
int dfn_credits[MAXN_COURSES + 1];  // 给定 DFN 索引处课程的学分 (对应 Java 中的 val)
int dfn_subtree_size[MAXN_COURSES + 1]; // 以给定 DFN 索引的课程为根的子树大小 (对应 Java 中的 size)

// 动态规划表
// dp[dfn_idx][courses_to_select] (dp[DFN索引][要选择的课程数])
// 最大 dfn_idx 可以是 N+1 (N 个课程 + 1 个虚拟根)。dp 表的基线条件需要到 N+2。
// 最大 courses_to_select 可以是 N。
int dp[MAXN_COURSES + 2][MAXN_COURSES];

int N_total_courses, M_courses_to_select; // 输入的总课程数 N 和要选择的课程数 M

// 为新的测试用例初始化数据结构
void build_structures(int current_n_courses, int current_m_select) {
    edge_count = 1; // 边的索引通常从 1 开始，如果 head_edge 中的 0 表示没有边
    dfn_counter = 0;
    // 初始化节点 0 到 current_n_courses 的 head_edge 数组
    std::fill(head_edge, head_edge + current_n_courses + 1, 0);

    // 初始化 DP 表的基线条件行。
    // dp[current_n_courses + 2] 对应一个超出所有实际课程 DFN 的 DFN 索引。
    // 如果我们处于一个超过所有实际课程的 DFN 索引，我们就不能再选择任何课程，所以学分为 0。
    // 最大 current_n_courses 是 300。所以 current_n_courses + 2 是 302。
    // dp 表的维度允许 dp[302][...]。
    if (current_n_courses + 2 < MAXN_COURSES + 2) {
        std::fill(dp[current_n_courses + 2], dp[current_n_courses + 2] + current_m_select + 1, 0);
    }
    // dp 表的其他部分隐式地零初始化 (全局/静态存储)
    // 或者将被计算。dp[i][0] (选择 0 门课程) 应该得到 0 学分。
}

// 添加一条从 u 到 v 的有向边 (v 的先修课是 u)
void add_edge(int u_prerequisite, int v_course) {
    next_node_edge[edge_count] = head_edge[u_prerequisite];
    to_node[edge_count] = v_course;
    head_edge[u_prerequisite] = edge_count++;
}

// 执行 DFS 来计算 DFN 序、按 DFN 索引的学分以及按 DFN 索引的子树大小。
// 返回以 u_node 为根的子树的大小。
int dfs_calculate_dfn_properties(int u_node) {
    int current_dfn_idx = ++dfn_counter; // 分配下一个 DFN 索引
    dfn_credits[current_dfn_idx] = course_credits[u_node]; // 在其 DFN 索引处存储 u_node 的学分
    dfn_subtree_size[current_dfn_idx] = 1; // 当前节点本身是其 DFN 子树的一部分

    // 对所有子节点 (u_node 是其先修课的那些课程) 进行递归调用
    for (int edge_idx = head_edge[u_node]; edge_idx > 0; edge_idx = next_node_edge[edge_idx]) {
        int v_child_node = to_node[edge_idx];
        dfn_subtree_size[current_dfn_idx] += dfs_calculate_dfn_properties(v_child_node);
    }
    return dfn_subtree_size[current_dfn_idx];
}

// 计算可以获得的最大学分
int compute_max_credits() {
    // 课程 0 是一个虚拟根节点。它的 DFN 将是 1。
    // 实际课程 (1 到 N_total_courses) 的 DFN 将从 2 到 N_total_courses + 1。
    // course_credits[0] 是 0 (虚拟根节点没有学分)。
    dfs_calculate_dfn_properties(0);

    // DP 状态: dp[i_dfn][j_courses] = 从 DFN 序为 i_dfn 到 N_total_courses+1 的课程中选择 j_courses 门课程，
    // 并且形成有效先修结构的所能获得的最大学分。

    // 以 DFN 逆序迭代 (从叶节点向上到虚拟根的子节点)。
    // 实际课程的 DFN 范围从 2 到 N_total_courses + 1。
    // 循环处理对应于实际课程和虚拟根的 DFN 索引。
    // 基线条件 dp[N_total_courses + 2][...] 已经是 0。
    for (int i_dfn = N_total_courses + 1; i_dfn >= 1; i_dfn--) {
        for (int j_courses = 0; j_courses <= M_courses_to_select; j_courses++) {
            dp[i_dfn][j_courses] = std::max(dp[i_dfn + dfn_subtree_size[i_dfn]][j_courses], dfn_credits[i_dfn] + dp[i_dfn + 1][j_courses - 1]);

        }
    }
    // dp[2][m] : 2 ~ n范围上，选择m个节点一定要形成有效结构的情况下，最大的累加和
    // 最后来到dfn序为1的节点，一定是原始的0号节点
    // 原始0号节点下方一定挂着有效结构
    // 并且和补充的0号节点一定能整体连在一起，没有任何跳跃连接
    // 于是整个问题解决
    return dp[2][M_courses_to_select];
}


int main() {
    // C++ 快速 I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    while (std::cin >> N_total_courses >> M_courses_to_select) {
        // course_credits[0] 用于虚拟根，由于全局初始化，隐式为 0。
        // 对于编程竞赛，如果在多个测试用例中重用数组，最好显式管理状态。
        // 在这里，build_structures 处理必要的重新初始化。
        build_structures(N_total_courses, M_courses_to_select);

        for (int i = 1; i <= N_total_courses; i++) {
            int prerequisite_s, credit_c;
            std::cin >> prerequisite_s >> credit_c;
            add_edge(prerequisite_s, i); // 课程 i 的先修课是 prerequisite_s
            course_credits[i] = credit_c;     // 存储课程 i 的学分
        }
        // course_credits[0] 保持为 0 (虚拟根节点 0 的学分)。

        std::cout << compute_max_credits() << std::endl;
    }

    return 0;
}

