#include <vector>
#include <algorithm>
#include <queue>
#include <limits>

// 雇佣K名工人的最低成本
// 有n名工人，给定两个数组quality和wage
// 其中quality[i]表示第i名工人的工作质量，其最低期望工资为wage[i]
// 现在我们想雇佣k名工人组成一个工资组
// 在雇佣一组k名工人时，我们必须按照下述规则向他们支付工资：
// 对工资组中的每名工人，应当按其工作质量与同组其他工人的工作质量的比例来支付工资
// 工资组中的每名工人至少应当得到他们的最低期望工资
// 给定整数k，返回组成满足上述条件的付费群体所需的最小金额
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-hire-k-workers/

using namespace std;
struct Employee {
    double ratio; // 薪水与质量的比例（wage/quality）
    int quality;  // 工作质量绝对值
    Employee(double r, int q) : ratio(r), quality(q) {}
};

/*
* 计算雇佣k名工人的最低成本
* 参数说明：
*   quality - 工人的质量数组
*   wage - 工人的最低期望工资数组
*   k - 需要雇佣的工人数量
* 实现原理：
*   1. 将工人按「薪资/质量」比率排序，保证后续遍历时比率递增
*   2. 使用大根堆维护当前最小的k个质量值
*   3. 对每个比率计算当前堆中k个质量的总和 * 当前比率
*/
double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
    int n = quality.size();
    vector<Employee> employees;

    // 步骤1：构建员工信息数组
    for (int i = 0; i < n; ++i) {
        // 计算每个工人的薪资质量比（需满足：实际工资 >= 该比率 * 质量）
        employees.emplace_back(static_cast<double>(wage[i]) / quality[i], quality[i]);
    }

    // 按薪资质量比升序排序（关键：保证后续遍历时使用的比率能满足所有已选工人）
    sort(employees.begin(), employees.end(), [](const Employee& a, const Employee& b) {
        return a.ratio < b.ratio;
    });

    // 步骤2：使用大根堆维护当前最小的k个质量值（堆顶为最大质量）
    priority_queue<int> heap;
    int qualitySum = 0;  // 当前堆中质量总和
    double ans = numeric_limits<double>::max(); // 最终结果初始化为极大值

    for (int i = 0; i < n; ++i) {
        int curQuality = employees[i].quality;

        // 当堆未满时直接加入元素
        if (heap.size() < k) {
            qualitySum += curQuality;
            heap.push(curQuality);

            // 当堆满k个时立即计算当前可能解
            if (heap.size() == k) {
                ans = min(ans, qualitySum * employees[i].ratio);
            }
        } else {
            // 当新质量比堆顶小时，替换堆顶元素优化质量总和
            if (curQuality < heap.top()) {
                qualitySum += curQuality - heap.top(); // 更新总和
                heap.pop();
                heap.push(curQuality);
                ans = min(ans, qualitySum * employees[i].ratio); // 更新最优解
            }
        }
    }
    return ans;
}
