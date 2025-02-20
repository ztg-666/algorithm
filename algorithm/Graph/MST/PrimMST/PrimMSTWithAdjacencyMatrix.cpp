

// 使用邻接矩阵表示图，计算最小生成树的路径之和
// 请保证graph是连通图
// graph[i][j]表示点i到点j的距离，如果是系统最大值代表无路
// 返回值是最小连通图的路径之和
int prim(int** graph, int size) {
    // 存储从起始节点到每个节点的最短距离
    int* distances = new int[size];
    // 标记节点是否已访问
    bool* visit = new bool[size];

    // 初始化所有节点的距离为系统最大值，访问状态为未访问
    for (int i = 0; i < size; i++) {
        visit[i] = false;
        distances[i] = graph[0][i]; // 初始化距离为起始节点到各节点的距离
    }
    // 起始节点已访问
    visit[0] = true;
    // 最小生成树的路径之和
    int sum = 0;

    // 遍历剩余的节点
    for (int i = 1; i < size; i++) {
        // 当前找到的最小路径权重
        int minPath = INT_MAX;
        // 当前找到的最小路径对应的节点索引
        int minIndex = -1;

        // 查找未访问节点中距离最小的节点
        for (int j = 0; j < size; j++) {
            if (!visit[j] && distances[j] < minPath) {
                minPath = distances[j];
                minIndex = j;
            }
        }
        // 如果找不到新的节点，返回当前路径之和
        if (minIndex == -1) {
            delete[] distances;
            delete[] visit;
            return sum;
        }
        // 标记找到的节点为已访问
        visit[minIndex] = true;
        // 累加最小路径权重
        sum += minPath;
        // 更新未访问节点的距离
        for (int j = 0; j < size; j++) {
            if (!visit[j] && distances[j] > graph[minIndex][j]) {
                distances[j] = graph[minIndex][j];
            }
        }
    }
    // 释放动态分配的内存
    delete[] distances;
    delete[] visit;
    // 返回最小生成树的路径之和
    return sum;
}

