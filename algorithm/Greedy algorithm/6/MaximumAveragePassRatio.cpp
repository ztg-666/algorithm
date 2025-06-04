#include <vector>
#include <queue>

// 最大平均通过率
// 一所学校里有一些班级，每个班级里有一些学生，现在每个班都会进行一场期末考试
// 给你一个二维数组classes，其中classes[i]=[passi, totali]
// 表示你提前知道了第i个班级总共有totali个学生
// 其中只有 passi 个学生可以通过考试
// 给你一个整数extraStudents，表示额外有extraStudents个聪明的学生，一定能通过期末考
// 你需要给这extraStudents个学生每人都安排一个班级，使得所有班级的平均通过率最大
// 一个班级的 通过率 等于这个班级通过考试的学生人数除以这个班级的总人数
// 平均通过率 是所有班级的通过率之和除以班级数目
// 请你返回在安排这extraStudents个学生去对应班级后的最大平均通过率
// 测试链接 : https://leetcode.cn/problems/maximum-average-pass-ratio/

using namespace std;

double maxAverageRatio(vector<vector<int>>& classes, int m) {
    int n = classes.size();
    // 自定义比较器，提升值大的排在堆顶
    struct Comparator {
        bool operator()(const vector<double>& a, const vector<double>& b) {
            return a[2] < b[2];
        }
    };
    priority_queue<vector<double>, vector<vector<double>>, Comparator> heap;
    // 初始化堆，计算初始提升值
    for (auto& c : classes) {
        double a = c[0], b = c[1];
        double increase = (a + 1) / (b + 1) - a / b;
        heap.push({a, b, increase});
    }

    // 分配额外学生
    while (m--) {
        vector<double> cur = heap.top();
        heap.pop();
        double a = cur[0] + 1, b = cur[1] + 1;
        double newIncrease = (a + 1) / (b + 1) - a / b;
        heap.push({a, b, newIncrease});
    }

    // 计算总通过率
    double sum = 0;
    while (!heap.empty()) {
        vector<double> cur = heap.top();
        sum += cur[0] / cur[1];
        heap.pop();
    }

    return sum / n;
}
