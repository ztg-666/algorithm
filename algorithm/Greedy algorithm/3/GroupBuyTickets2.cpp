#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

// 组团买票找到了在线测试
// 逻辑和课上讲的一样，但是测试中设定的ki为负数
// 实现做了一些小优化，具体可以看注释
// 测试链接 : https://www.luogu.com.cn/problem/P12331

using namespace std;

// 项目数据结构（保存每个项目的核心参数）
struct Game {
    long long ki;     // 折扣系数（注意题目中可能为负数）
    long long bi;     // 基础价格
    int people;       // 当前已分配人数

    // 构造函数初始化参数
    Game(long long k, long long b) : ki(k), bi(b), people(0) {}

    // 计算边际收益（新增1人带来的收益变化）
    long long earn() const {
        return cost(people + 1) - cost(people);
    }

    // 计算总收益（带非负处理，防止出现负价格）
    long long cost(long long p) const {
        long long price = ki * p + bi;      // 计算当前单价
        if (price < 0) price = 0;           // 单价不能为负
        return p * price;                   // 总收益 = 人数 * 单价
    }
};

// 自定义优先队列比较器（实现大根堆）
struct CompareGame {
    // 按边际收益降序排列（优先选择收益最大的项目）
    bool operator()(const Game& a, const Game& b) {
        return a.earn() < b.earn(); // 注意：C++优先队列默认是最大堆，通过比较器实现降序
    }
};

// 快速输入函数（优化大数据量读取）
inline long long readLong() {
    long long x = 0, f = 1;
    char ch = getchar();
    // 跳过非数字字符
    while (ch < '0' || ch > '9') {
        if (ch == '-') f = -1;  // 处理负数
        ch = getchar();
    }
    // 逐位构建数字
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + (ch - '0');
        ch = getchar();
    }
    return x * f;
}

int main() {
    // 关闭流同步提升IO速度（注意：使用后不能混用C和C++风格IO）
    ios::sync_with_stdio(false);
    cin.tie(0);  // 解除cin与cout的绑定

    // 读取总人数和项目数
    int n = readLong();
    int m = readLong();

    // 创建按边际收益排序的大根堆
    priority_queue<Game, vector<Game>, CompareGame> heap;

    // 初始化项目数据
    for (int i = 0; i < m; ++i) {
        long long k = readLong();
        long long b = readLong();
        Game cur(k, b);
        // 过滤初始边际收益<=0的项目（没有增益的直接丢弃）
        if (cur.earn() > 0) {
            heap.push(cur);
        }
    }

    long long ans = 0;
    // 贪心分配人员：每次选择边际收益最大的项目
    for (int i = 0; i < n && !heap.empty(); ++i) {
        Game cur = heap.top();  // 获取当前最优项目
        heap.pop();

        long long money = cur.earn();
        if (money <= 0) break;  // 提前终止条件（没有收益可获取）

        ans += money;      // 累加收益
        cur.people++;      // 更新分配人数

        // 重新计算边际收益，仍有增益才放回堆中
        if (cur.earn() > 0) {
            heap.push(cur);  // 重新入堆参与后续排序
        }
    }

    // 输出最终最大收益
    cout << ans << endl;
    return 0;
}
