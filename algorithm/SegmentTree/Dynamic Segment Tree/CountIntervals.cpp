#include <vector>
#include <algorithm>
using namespace std;

// 统计区间中的整数数目
// 实现CountIntervals类的如下三个方法
// 1) CountIntervals() : 初始化
// 2) void add(int l, int r) : 把[l, r]范围上的数字都设置成1
// 3) int count() : 返回整个区间有多少个1
// CountIntervals类需要支持1 ~ 10^9范围
// 调用add和count方法总共10^5次
// 测试链接 : https://leetcode.cn/problems/count-integers-in-intervals/

class CountIntervals {
private:
    // 支持的最大范围，题目要求支持1 ~ 10^9
    static const int n = 1000000000;

    // 动态开点线段树所需的最大节点数
    // 这个值是通过实验得出的，足以支持10^5次操作
    static const int LIMIT = 700001;

    // 动态开点线段树的数据结构
    vector<int> left;   // left[i]表示节点i的左子节点编号，0表示不存在
    vector<int> right;  // right[i]表示节点i的右子节点编号，0表示不存在
    vector<int> sum;    // sum[i]表示节点i所管辖范围内1的个数
    int cnt;            // 当前已使用的节点数量，从1开始编号

    // 向上更新：将子节点的信息合并到父节点
    // h: 父节点编号, l: 左子节点编号, r: 右子节点编号
    void up(int h, int l, int r) {
        sum[h] = sum[l] + sum[r];  // 父节点的1的个数 = 左子节点的1的个数 + 右子节点的1的个数
    }

    // 核心函数：将指定区间[jobl, jobr]内的所有数字设置为1
    // 这个题的特殊性在于，只有改1的操作，没有改0的操作
    // 理解这个就可以分析出不需要懒更新机制，原因有两个：
    // 1) 查询操作永远查的是整个范围1的数量，不会有小范围的查询，每次都返回sum[1]
    //    这意味着只要能把sum[1]更新正确即可，up函数可以保证这一点
    // 2) 一个范围已经全是1，那以后都会是1，没有必要把全是1的懒更新信息向下传递
    // 这个函数的功能比线段树能做到的范围修改功能简单很多
    // 功能有阉割就意味着存在优化的点
    void setOne(int jobl, int jobr, int l, int r, int i) {
        // 参数说明：
        // jobl, jobr: 要修改的目标区间范围
        // l, r: 当前节点i所管辖的区间范围
        // i: 当前节点的编号

        // 剪枝优化：如果当前范围已经全是1了，直接返回
        if (sum[i] == r - l + 1) {
            return;
        }

        // 如果目标区间完全覆盖当前节点的管辖范围
        if (jobl <= l && r <= jobr) {
            sum[i] = r - l + 1;  // 将当前范围内的所有数字都设置为1
        } else {
            // 否则需要继续向下递归
            int mid = (l + r) >> 1;  // 计算中点，等价于 (l + r) / 2

            // 如果目标区间与左半部分有交集
            if (jobl <= mid) {
                // 动态开点：如果左子节点不存在，创建一个新节点
                if (left[i] == 0) {
                    left[i] = ++cnt;  // 分配新的节点编号
                }
                // 递归处理左子树
                setOne(jobl, jobr, l, mid, left[i]);
            }

            // 如果目标区间与右半部分有交集
            if (jobr > mid) {
                // 动态开点：如果右子节点不存在，创建一个新节点
                if (right[i] == 0) {
                    right[i] = ++cnt;  // 分配新的节点编号
                }
                // 递归处理右子树
                setOne(jobl, jobr, mid + 1, r, right[i]);
            }

            // 向上更新当前节点的信息
            up(i, left[i], right[i]);
        }
    }

public:
    // 构造函数：初始化动态开点线段树
    CountIntervals() : left(LIMIT, 0), right(LIMIT, 0), sum(LIMIT, 0), cnt(1) {
        // 使用初始化列表：
        // left, right, sum: 所有元素初始化为0
        // cnt: 初始化为1，因为节点编号从1开始
        // 注意：节点0被用作"不存在"的标识，实际节点从1开始编号
    }

    // 公共接口：添加区间[l, r]，将其中的数字都设置为1
    void add(int l, int r) {
        setOne(l, r, 1, n, 1);  // 从根节点(编号1)开始，管辖范围[1, n]
    }

    // 公共接口：返回当前有多少个数字被设置为1
    int count() {
        return sum[1];  // 返回根节点的统计值，即整个范围内1的个数
    }
};

/*
算法复杂度分析：
时间复杂度：
- add操作：O(log n)，其中n = 10^9，因为树的深度约为log(10^9) ≈ 30
- count操作：O(1)，直接返回根节点的值
空间复杂度：O(k * log n)，其中k是操作次数(10^5)，每次操作最多创建log n个节点

核心思想：
1. 使用动态开点线段树，只在需要时创建节点，节省空间
2. 利用题目特性(只有设置1的操作)，省略懒更新机制
3. 通过剪枝优化，避免对已经全为1的区间进行重复操作
*/
