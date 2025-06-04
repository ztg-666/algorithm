#include <vector>
#include <algorithm> // 用于std::max和std::abs

// 超级洗衣机
// 假设有n台超级洗衣机放在同一排上
// 开始的时候，每台洗衣机内可能有一定量的衣服，也可能是空的
// 在每一步操作中，你可以选择任意 m (1 <= m <= n) 台洗衣机
// 与此同时将每台洗衣机的一件衣服送到相邻的一台洗衣机
// 给定一个整数数组machines代表从左至右每台洗衣机中的衣物数量
// 请给出能让所有洗衣机中剩下的衣物的数量相等的最少的操作步数
// 如果不能使每台洗衣机中衣物的数量相等则返回-1
// 测试链接 : https://leetcode.cn/problems/super-washing-machines/

using namespace std;

int findMinMoves(const vector<int>& machines) {
    int n = machines.size();
    int sum = 0;
    // 计算所有洗衣机的衣服总数
    for (int num : machines) {
        sum += num;
    }
    // 无法平均分配直接返回-1
    if (sum % n != 0) {
        return -1;
    }
    const int avg = sum / n;      // 每台洗衣机最终需要的衣服数
    int leftSum = 0;              // 当前洗衣机左侧[0..i-1]的累计衣服数
    int ans = 0;                  // 记录全局最大瓶颈值

    for (int i = 0; i < n; ++i) {
        /* 计算当前洗衣机左右两侧的需求：
           leftNeed  = 左侧应有衣服总数 - 左侧实际衣服总数
                     = (左侧洗衣机数量 * avg) - leftSum
                     >0表示左侧需要衣服，<0表示左侧多余衣服

           rightNeed = 右侧应有衣服总数 - 右侧实际衣服总数
                     = (右侧洗衣机数量 * avg) - (总衣服数 - leftSum - 当前洗衣机衣服数)
                     >0表示右侧需要衣服，<0表示右侧多余衣服
        */
        const int leftNeed = i * avg - leftSum;
        const int rightNeed = (n - i - 1) * avg - (sum - leftSum - machines[i]);

        /* 判断当前洗衣机位置的瓶颈：
           情况1：左右都需要衣服(leftNeed>0 && rightNeed>0)
                需要操作次数为两者之和，因为衣服必须经过当前节点传递

           情况2：至少一侧可以提供衣服
                瓶颈由需求绝对值最大的那侧决定，因为可以并行操作
        */
        int bottleneck;
        if (leftNeed > 0 && rightNeed > 0) {
            bottleneck = leftNeed + rightNeed; // 两侧需求叠加
        } else {
            bottleneck = max(abs(leftNeed), abs(rightNeed)); // 单侧决定瓶颈
        }

        ans = max(ans, bottleneck);    // 维护全局最大瓶颈值
        leftSum += machines[i];        // 更新左侧累计衣服数
    }

    return ans; // 全局最大瓶颈即为最小操作次数
}
