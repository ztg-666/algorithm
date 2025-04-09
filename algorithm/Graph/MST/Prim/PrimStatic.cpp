#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 5001;  // 最大顶点数
const int MAXM = 400001; // 最大边数（无向图，边数是输入的两倍）

// 链式前向星存储图结构
int head[MAXN], next_[MAXM], to[MAXM], weight[MAXM], cnt;

// 手动实现堆结构
int heap[MAXN][2];    // heap[i][0]：节点编号，heap[i][1]：边权
int where[MAXN];      // 节点在堆中的位置状态：
                       // -1 表示未入堆，-2 表示已弹出，>=0 表示在堆中的索引
int heapSize, nodeCnt; // heapSize是堆的大小，nodeCnt是已访问的节点数

int u, w; // 用于存储弹出的节点和权重

// 初始化链式前向星和堆
void build(int n) {
    cnt = 1; // 边计数器从1开始（0表示空）
    heapSize = 0; // 堆初始为空
    nodeCnt = 0; // 已访问节点数初始为0
    memset(head, 0, (n + 1) * sizeof(int)); // 初始化head数组为0
    memset(where, -1, (n + 1) * sizeof(int)); // 初始化where数组为-1（未入堆）
}

// 添加一条边到链式前向星中
void addEdge(int u, int v, int w) {
    next_[cnt] = head[u]; // 新边指向当前u的头边
    to[cnt] = v;          // 边的目标节点是v
    weight[cnt] = w;      // 边的权重是w
    head[u] = cnt++;      // 更新u的头边为新边
}

// 交换堆中两个元素的位置，并更新where数组
void swapHeap(int i, int j) {
    // 交换节点在where中的记录
    where[heap[i][0]] = j; // 更新节点i在堆中的位置为j
    where[heap[j][0]] = i; // 更新节点j在堆中的位置为i
    // 交换堆元素
    swap(heap[i][0], heap[j][0]); // 交换节点编号
    swap(heap[i][1], heap[j][1]); // 交换边权
}

// 堆上浮操作：将index位置的元素上浮到合适位置
void heapInsert(int index) {
    while (heap[index][1] < heap[(index - 1)/2][1]) { // 如果当前元素比父节点小
        swapHeap(index, (index - 1)/2);               // 与父节点交换
        index = (index - 1)/2;                        // 更新当前元素位置
    }
}

// 堆下沉操作：将index位置的元素下沉到合适位置
void heapify(int index) {
    int left = index * 2 + 1; // 左子节点
    while (left < heapSize) { // 当左子节点存在时
        // 找到左右子节点中较小的一个
        int smallest = left + 1 < heapSize && heap[left+1][1] < heap[left][1]
                     ? left + 1 : left;
        // 如果当前节点比最小的子节点还小，则停止下沉
        smallest = heap[smallest][1] < heap[index][1] ? smallest : index;
        if (smallest == index) break; // 如果当前位置已经是最小值，退出循环
        swapHeap(smallest, index); // 否则交换并继续下沉
        index = smallest;
        left = index * 2 + 1; // 更新左子节点
    }
}

// 添加或更新堆中的边信息
void addOrUpdate(int ei) {
    int v = to[ei];       // 目标节点
    int cost = weight[ei]; // 边权重

    if (where[v] == -1) { // 如果节点v从未入堆
        heap[heapSize][0] = v; // 将节点v加入堆
        heap[heapSize][1] = cost; // 设置边权
        where[v] = heapSize; // 记录节点v在堆中的位置
        heapInsert(heapSize++); // 上浮调整堆
    } else if (where[v] >= 0) { // 如果节点v已经在堆中
        if (cost < heap[where[v]][1]) { // 如果发现更小的边权
            heap[where[v]][1] = cost; // 更新堆中的边权
            heapInsert(where[v]); // 上浮调整堆
        }
    }
}

// 弹出堆顶元素（最小边权的节点）
void pop() {
    u = heap[0][0]; // 获取堆顶节点编号
    w = heap[0][1]; // 获取堆顶边权
    swapHeap(0, --heapSize); // 将堆顶与最后一个元素交换并缩小堆
    heapify(0); // 下沉调整堆
    where[u] = -2; // 标记节点u已弹出
    nodeCnt++;     // 已访问节点数加1
}

// Prim算法主逻辑
int prim(int start) {
    nodeCnt = 1; // 从起始点开始，已访问节点数为1
    where[start] = -2; // 标记起始点已访问

    // 初始化堆：将起始点的所有邻接边加入堆
    for (int ei = head[start]; ei != 0; ei = next_[ei]) {
        addOrUpdate(ei);
    }

    int ans = 0; // 最小生成树的总权重
    while (heapSize > 0) { // 当堆不为空时
        pop(); // 弹出堆顶元素
        ans += w; // 累加边权

        // 扩展新节点的边：将当前节点的所有邻接边加入堆
        for (int ei = head[u]; ei != 0; ei = next_[ei]) {
            addOrUpdate(ei);
        }
    }
    return ans; // 返回最小生成树的总权重
}
// Prim算法优化（洛谷）
// 静态空间实现
// 时间复杂度O(n + m) + O((m+n) * log n)
// 测试链接 : https://www.luogu.com.cn/problem/P3366
int main() {
    int n, m;
    scanf("%d%d", &n, &m); // 输入顶点数n和边数m
    build(n);              // 初始化数据结构

    // 输入所有边的信息
    for (int i = 0, u, v, w; i < m; i++) {
        scanf("%d%d%d", &u, &v, &w);
        addEdge(u, v, w); // 添加双向边
        addEdge(v, u, w);
    }

    int ans = prim(1); // 从节点1开始执行Prim算法
    printf(nodeCnt == n ? "%d" : "orz", ans); // 如果覆盖了所有节点，输出结果；否则输出"orz"
    
    return 0;
}
