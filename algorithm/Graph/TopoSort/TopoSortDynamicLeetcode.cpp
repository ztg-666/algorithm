#include <vector>
#include <queue>
#include <iostream>
using namespace std;
// 拓扑排序模版（Leetcode）
// 邻接表建图（动态方式）
// 课程表II
// 现在你总共有 numCourses 门课需要选，记为 0 到 numCourses - 1
// 给你一个数组 prerequisites ，其中 prerequisites[i] = [ai, bi]
// 表示在选修课程 ai 前 必须 先选修 bi
// 例如，想要学习课程 0 ，你需要先完成课程 1 ，我们用一个匹配来表示：[0,1]
// 返回你为了学完所有课程所安排的学习顺序。可能会有多个正确的顺序
// 你只要返回 任意一种 就可以了。如果不可能完成所有课程，返回 一个空数组
// 测试链接 : https://leetcode.cn/problems/course-schedule-ii/
vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
    // 邻接表：graph[i] 存储节点i的所有邻居节点（即依赖于课程i的课程）
    vector<vector<int>> graph(numCourses);

    // 入度表：indegree[i] 表示节点i的入度（即课程i有多少前置课程）
    vector<int> indegree(numCourses, 0);

    // 构建图和入度表：
    for (auto& edge : prerequisites) {
        // edge[1] -> edge[0] 表示学习课程edge[0]之前必须完成课程edge[1]
        graph[edge[1]].push_back(edge[0]);
        // 增加课程edge[0]的入度
        indegree[edge[0]]++;
    }

    // 结果队列：存储拓扑排序的结果
    vector<int> queue(numCourses);

    // 初始化双指针：l表示队列头部，r表示队列尾部
    int l = 0, r = 0;

    // 初始化队列：将所有入度为0的节点加入队列（没有前置课程的课程可以先学）
    for (int i = 0; i < numCourses; i++) {
        if (indegree[i] == 0) {
            queue[r++] = i; // 将入度为0的节点加入队列
        }
    }

    // 记录已处理的节点数
    int cnt = 0;

    // 开始拓扑排序：
    while (l < r) { // 当队列不为空时
        int cur = queue[l++]; // 取出队首节点（当前课程）
        cnt++; // 已处理节点数+1

        // 遍历当前课程的所有邻居节点（依赖于当前课程的课程）
        for (int next : graph[cur]) {
            // 减少邻居节点的入度（因为当前课程已完成）
            if (--indegree[next] == 0) {
                // 如果邻居节点的入度变为0，则将其加入队列
                queue[r++] = next;
            }
        }
    }

    // 判断是否完成拓扑排序：
    // 如果已处理的节点数等于总课程数，说明可以完成所有课程
    if (cnt == numCourses) {
        // 返回结果队列中的前numCourses个元素作为拓扑排序的结果
        return vector<int>(queue.begin(), queue.begin() + numCourses);
    } else {
        // 否则返回空数组，表示无法完成所有课程
        return vector<int>();
    }
}

// 主函数
int main() {
    // 输入：课程总数和先修关系
    int numCourses = 4; // 总共有4门课程
    vector<vector<int>> prerequisites = {{1, 0}, {2, 0}, {3, 1}, {3, 2}};
    // 解释：
    // 课程1依赖于课程0
    // 课程2依赖于课程0
    // 课程3依赖于课程1和课程2

    // 调用拓扑排序函数
    vector<int> result = findOrder(numCourses, prerequisites);

    // 输出结果
    if (result.empty()) {
        cout << "false" << endl;
    } else {
        cout << "true: "<< endl;
        for (int course : result) {
            cout << course << " ";
        }
        cout << endl;
    }

    return 0;
}