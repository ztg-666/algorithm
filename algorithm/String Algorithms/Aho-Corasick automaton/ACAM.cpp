#include <iostream> // 输入输出流：用于标准输入（cin）和标准输出（cout）
#include <string>   // 字符串操作：用于处理模式串和主文本
#include <vector>   // 动态数组：虽然代码中没有直接使用std::vector，但通常在构建邻接表时会用到，这里作为常用头文件保留
#include <queue>    // 队列：在构建AC自动机的失配指针（setFail函数）时，会用到队列进行广度优先遍历（BFS）
#include <stack>    // 栈：在聚合词频（f2函数）时，可以使用栈进行迭代深度优先遍历（DFS），以替代递归

// AC自动机模版(优化版)
// 给你若干目标字符串，还有一篇文章
// 返回每个目标字符串在文章中出现了几次
// 测试链接 : https://www.luogu.com.cn/problem/P5357

using namespace std; // 引入标准命名空间，方便直接使用cin, cout, string等

// 类型别名定义：将long long定义为ll，方便后续使用
using ll = long long;

// 常量定义，通常比Java的MAXN/MAXS稍微大一点，以防万一
// 为了安全起见，这些常量通常会设置得比实际需求略大，以应对测试用例的极限情况。
const int MAXN = 200005; // 模式串的最大数量：最多支持20万个模式串
const int MAXS = 200005; // 所有模式串字符总数的最大值：Trie树（字典树）中节点的总数上限，即所有模式串长度之和的上限

// --- AC自动机的主要数据结构 ---
// 这些全局变量构成了AC自动机的核心骨架，它们共同维护着Trie树的结构、失配关系和匹配状态。

int end_[MAXN]; // end_[i] 存储第i个模式串在Trie树中的结束节点编号。
                // 为什么要记录这个？因为最后我们要输出每个模式串的出现次数，
                // 就需要知道它在Trie树中对应的唯一“终点”。
int tree[MAXS][26]; // Trie树的节点和转移：这是一个二维数组，模拟Trie树。
                    // tree[u][c] 表示从节点u经过字符c（'a'到'z'对应0-25）所能到达的下一个节点。
                    // 如果tree[u][c]为0，则表示没有对应的子节点。
int fail[MAXS];     // 失配指针（Fail Link）：这是AC自动机的精髓所在！
                    // fail[u] 表示节点u的失配指针指向的节点。
                    // 当在节点u匹配当前字符失败时，可以沿着fail指针跳转到fail[u]节点继续尝试匹配，
                    // 这样就避免了从头开始，大大提高了匹配效率。
int cnt;            // 当前Trie树中节点的总数：从0开始计数，0是根节点。
                    // 每创建一个新节点，cnt就会自增，它也代表了Trie树中已使用的最大节点编号。

// --- 词频统计相关 ---
// 这部分变量用于在主文本中匹配后，统计每个模式串的出现次数。

int times[MAXS];    // times[u] 记录节点u在文本匹配中被访问到的次数（初步计数）。
                    // 注意：这只是初步的直接匹配计数。一个模式串的出现，可能意味着它的某个后缀也被匹配到。
                    // 最终的词频需要通过失配指针构成的“树”进行累加聚合。

// --- 辅助数据结构：用于构建失配树（Fail Tree）并进行深度优先遍历（DFS）聚合词频 ---
// AC自动机的失配指针构成了一棵树状结构（或者说有向无环图，但反向看是树）。
// 为了方便聚合词频，我们通常会构建这棵树的反向图，即从fail[u]指向u，形成一棵“失配树”。

int box[MAXS];      // 一个通用的临时数组，在这里被setFail用作队列（模拟），也被f2用作栈（模拟）。
                    // 这种做法在一些竞技编程中很常见，可以节省内存或避免频繁的new/delete操作。

int head[MAXS];     // 邻接表头数组：用于表示“失配树”的邻接表。
                    // head[u] 存储以u为起点的第一条边的索引。
int nxt[MAXS];      // 邻接表下一条边数组：nxt[i] 存储与第i条边同起点的下一条边的索引。
int to[MAXS];       // 邻接表目标数组：to[i] 存储第i条边的终点节点编号。
int edge = 0;       // 边的计数器：用于在构建邻接表时给每条边分配唯一索引。

// 迭代DFS中使用的访问标记，防止重复处理
bool visited[MAXS]; // 用于f2函数（迭代DFS）中标记节点是否已访问，避免死循环或重复计算。

// --- AC自动机核心功能 ---

/**
 * @brief 将一个模式串插入到AC自动机（Trie树）中。
 * @param i 当前模式串的编号 (1-indexed)。
 * @param str 要插入的模式串。
 *
 * 这个函数是构建Trie树的基础。它遍历模式串的每一个字符，
 * 依次在Trie树中创建或查找对应的节点，直到模式串的最后一个字符。
 */
void insert(int i, const std::string& str) {
    int u = 0; // 从Trie树的根节点（编号0）开始遍历
    for (char ch : str) { // 遍历模式串中的每一个字符
        int s = ch - 'a'; // 将字符 'a'-'z' 映射到 0-25 的整数索引
        if (tree[u][s] == 0) {
            // 如果当前节点u没有指向字符s的子节点（即路径不存在）
            // 则创建一个新的节点。
            tree[u][s] = ++cnt; // cnt自增，作为新节点的编号，并赋值给tree[u][s]
        }
        u = tree[u][s]; // 移动到下一个节点，继续处理模式串的下一个字符
    }
    // 模式串的所有字符都已处理完毕，当前节点u就是该模式串的结束节点。
    end_[i] = u; // 记录第i个模式串的结束节点编号，方便后续查找其匹配次数。
}

/**
 * @brief 构建AC自动机的失配指针（Fail Link）并优化Trie树的转移。
 * 使用BFS广度优先遍历完成。
 *
 * 这是AC自动机最核心的部分之一。失配指针的作用是：当在Trie树中某个节点u匹配主文本字符c失败时，
 * 可以沿着失配指针fail[u]跳转到另一个节点，继续尝试匹配字符c，这样可以避免从头开始匹配，
 * 极大地提高了匹配效率。同时，它也“优化”了Trie树的转移，使得任何路径都能平滑地沿着失配链回溯。
 */
void setFail() {
    int l = 0; // 队列头部指针（模拟队列的左边界）
    int r = 0; // 队列尾部指针（模拟队列的右边界）

    // 初始化：将Trie树根节点（0）的所有非空子节点（即深度为1的节点）加入队列。
    // 这些节点的失配指针都指向根节点（0），因为它们没有更短的公共前缀可以回溯。
    for (int i = 0; i < 26; ++i) { // 遍历所有可能的字符 'a' 到 'z'
        if (tree[0][i] != 0) { // 如果根节点有指向字符i的子节点
            // 根节点的子节点的失配指针都指向根节点0
            fail[tree[0][i]] = 0; // 这一行其实可以省略，因为全局变量初始化时fail数组默认为0
            box[r++] = tree[0][i]; // 将该子节点加入队列尾部
        }
    }

    // BFS遍历构建失配指针
    // 队列不为空，循环继续
    while (l < r) {
        int u = box[l++]; // 从队列头部取出一个节点u（当前处理的节点）

        // 遍历当前节点u的所有子节点（即Trie树中的下一层节点）
        for (int i = 0; i <= 25; i++) { // 遍历所有可能的字符 'a' 到 'z'
            if (tree[u][i] == 0) {
                // 如果节点u没有指向字符i的子节点（路径中断）
                // 此时，为了“优化转移”，我们让 tree[u][i] 直接指向 fail[u] 的相同字符转移节点。
                // 这样，在匹配主文本时，如果当前节点u不能通过字符i转移，就会自动“跳到”其失配节点处尝试转移，
                // 实现了类似沿着失配链回溯的效果，但更加高效（一步到位）。
                tree[u][i] = tree[fail[u]][i];
            } else {
                // 如果节点u有指向字符i的子节点 v = tree[u][i]
                // 那么节点 v 的失配指针应该指向：其父节点u的失配指针fail[u]所指向的节点，
                // 经过字符i所能到达的节点。
                // 简而言之：fail[v] = tree[fail[u]][i]
                fail[tree[u][i]] = tree[fail[u]][i];
                box[r++] = tree[u][i]; // 将新处理的子节点加入队列，等待后续处理
            }
        }
    }
}

/**
 * @brief 添加一条边到“失配树”的邻接表中。
 * “失配树”的边方向与失配指针相反，即如果fail[v] = u，则在失配树中有一条边从u指向v。
 * 这样，一个节点的子节点（在失配树中）都是它的失配指针指向它的节点。
 * @param u 边的起点（父节点）
 * @param v 边的终点（子节点）
 */
void addEdge(int u, int v) {
    nxt[++edge] = head[u]; // 新增边的next指针指向当前head[u]指向的旧边，形成链表
    head[u] = edge;        // 更新head[u]为新边的索引，使新边成为链表头
    to[edge] = v;          // 记录新边的终点节点为v
}

/**
 * @brief 递归深度优先遍历（DFS）函数，用于聚合词频。
 * 遍历“失配树”（fail树的反向图），将子节点的词频累加到父节点。
 * 这样做的目的是：如果主文本匹配到了一个较长的模式串，比如“abc”，
 * 那么它的所有后缀（如“bc”，“c”）也可能被视为匹配。
 * 通过沿着失配树从下往上累加，可以确保每个节点（代表一个模式串的后缀）
 * 的`times`值包含了所有以它为后缀的模式串的匹配次数。
 * @param u 当前进行DFS的节点。
 */
void f1(int u) {
    // 遍历当前节点u在“失配树”中的所有子节点
    for (int i = head[u]; i > 0; i = nxt[i]) {
        int v = to[i]; // 获取当前子节点v
        f1(v);         // 递归调用f1，先处理子节点v及其子树
        times[u] += times[v]; // 从子节点v返回后，将其累积的词频加到父节点u的times中
    }
}

/**
 * @brief 迭代深度优先遍历（DFS）函数，用于聚合词频（f1的非递归版本）。
 * 同样遍历“失配树”，通过手动管理一个栈来实现DFS，避免递归深度过大导致的栈溢出问题。
 * @param u 起始遍历的节点（通常是根节点0）。
 */
void f2(int u) {
    // box当做栈来使用。box[r] 是栈顶元素的下一个可用位置。
    // r代表栈中元素的数量。
    int r = 0;
    box[r++] = u; // 将起始节点u压入栈
    int cur;      // 当前正在处理的节点

    // 当栈不为空时，循环继续
    while (r > 0) {
        cur = box[r - 1]; // 获取栈顶节点（不弹出，因为需要再次访问它来累加子节点的值）
        if (!visited[cur]) {
            // 如果当前节点cur尚未被完全访问（即第一次访问）
            visited[cur] = true; // 标记为已访问，防止重复入栈
            // 按照DFS的顺序，将它的所有子节点（在失配树中）压入栈中
            // 注意：这里是先处理子节点
            for (int i = head[cur]; i > 0; i = nxt[i]) {
                box[r++] = to[i]; // 将子节点压入栈，它们将在后续循环中被处理
            }
        } else {
            // 如果当前节点cur已经被访问过（即第二次访问，说明它的所有子节点都已处理完毕）
            r--; // 弹出栈顶节点cur（因为它的子节点已经处理完，现在可以累加了）
            // 累加所有子节点的词频到当前节点cur
            for (int i = head[cur]; i > 0; i = nxt[i]) {
                times[cur] += times[to[i]]; // 将子节点to[i]的词频累加到cur节点
            }
        }
    }
}


// --- 主函数：程序执行的入口 ---
int main() {
    // 🚀 竞赛风格：开启高速I/O，告别cin/cout的龟速！
    // 这三行代码是C++竞技编程中常用的优化手段，用于提升输入输出效率。
    // std::ios_base::sync_with_stdio(false); 解除C++标准流与C标准流的同步，使得cin/cout不再等待printf/scanf。
    // std::cin.tie(nullptr); 解除cin与cout的绑定，cin在读取前不再强制刷新cout的缓冲区。
    // std::cout.tie(nullptr); 解除cout与缓冲区的绑定（虽然通常不必要，但有时可以进一步优化，特别是在大量输出时）。
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n;
    std::cin >> n; // 读取模式串的数量

    // 🏆 阶段一：构建Trie树 (模式串入库)
    for (int i = 1; i <= n; ++i) { // 遍历n个模式串
        std::string pattern_str;
        std::cin >> pattern_str; // 读取每一个模式串
        insert(i, pattern_str);   // 将模式串插入到AC自动机的Trie树中，并记录其结束节点。
    }

    // 🎯 阶段二：构建失配指针 (AC自动机的灵魂)
    setFail(); // 调用setFail函数，通过BFS为Trie树中的所有节点计算并设置失配指针，并优化了转移路径。
               // 这一步是AC自动机能够高效进行多模式匹配的关键。

    std::string s;
    std::cin >> s; // 读取需要进行匹配的大文章（主文本）

    // 🔍 阶段三：在主文本中进行匹配并初步统计节点访问次数
    for (int u = 0, i = 0; i < (int)s.size(); i++) { // u表示当前在Trie树中的节点，i是主文本的当前索引
        // 根据当前字符 s[i] 进行转移。由于setFail已经优化了Trie树，
        // 即使在匹配过程中遇到无法直接转移的字符，也会沿着失配链自动跳转到正确的位置。
        u = tree[u][s[i] - 'a'];
        // 增加当前到达节点的访问次数。
        // times[u] 表示主文本中以节点u结尾的字符串出现的次数。
        // 请注意，这仅仅是“直接”的访问次数，还未进行聚合。
        times[u]++;
    }

    // 🌲 阶段四：构建失配树（Fail Tree）
    // 失配树是失配指针的反向图。如果 fail[v] = u，那么在失配树中有一条从 u 到 v 的边。
    // 这样做的目的是为了方便后续的DFS或BFS，从叶子节点向根节点（或反之）聚合词频。
    for (int i = 1; i <= cnt; i++) { // 遍历所有非根节点（节点0是根节点）
        // 根据fail指针建反图（即失配树），addEdge(fail[i], i) 意味着
        // 如果节点i的失配指针指向fail[i]，那么fail[i]就是i在失配树中的父节点。
        // 这样，fail[i]收集了所有指向它的节点的统计信息，非常适合自底向上累加。
        addEdge(fail[i], i);
    }

    // 📊 阶段五：聚合词频 (最终统计)
    // 遍历失配树，将子节点的词频累加到父节点，最终每个模式串的结束节点
    // (end_[i]) 所对应的times值就是该模式串在文本中出现的总次数。
    // 例如，如果匹配了"abc"，那么"bc"和"c"也应该算作匹配。
    // 通过从失配树的叶子向根递归累加，可以确保所有相关的匹配都被计数。
    f1(0); // 从失配树的根节点（Trie树的根节点0）开始进行DFS聚合词频。
           // f2(0) 也可以，是f1的迭代版本，但这里选择了递归实现。

    // 🚀 阶段六：输出结果
    // 遍历每一个模式串，输出它们在主文本中的出现次数。
    for (int i = 1; i <= n; i++) {
         cout << times[end_[i]] << '\n'; // 输出第i个模式串的最终统计次数。
                                        // end_[i] 记录了第i个模式串在Trie中的结束节点，
                                        // times[end_[i]] 就是该模式串的出现次数。
    }

    return 0; // 程序成功结束，撒花！🎉
}
