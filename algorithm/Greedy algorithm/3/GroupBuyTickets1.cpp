#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
using namespace std;

// 项目数据结构
struct Game {
    int ki;     // 折扣系数
    int bi;     // 门票原价
    int people; // 当前已分配人数

    Game(int k, int b) : ki(k), bi(b), people(0) {}

    // 计算新增一人带来的收益（考虑边际收益递减）
    // 公式推导：新增x人时总收益为 (bi - ki*x)*x - (bi - ki*(x-1))*(x-1)
    long long earn() const {
        return bi - (2LL * people + 1) * ki;
    }
};

// 优先队列比较器（大根堆）
struct CompareGame {
    bool operator()(const Game& a, const Game& b) {
        // 按边际收益降序排列
        return a.earn() < b.earn();
    }
};

// 暴力递归枚举所有可能的分配方案
int f(int i, int n, int m, vector<vector<int>>& games, vector<int>& cnts) {
    if (i == n) { // 所有人员分配完毕
        int ans = 0;
        for (int j = 0; j < m; j++) {
            int k = games[j][0];
            int b = games[j][1];
            int x = cnts[j];
            // 计算当前分配方案的总花费，保证非负
            ans += max((b - k * x) * x, 0);
        }
        return ans;
    } else {
        // 当前人员不选择任何项目的情况
        int ans = f(i + 1, n, m, games, cnts);
        // 尝试将当前人员分配到每个项目
        for (int j = 0; j < m; j++) {
            cnts[j]++; // 选择当前项目
            ans = max(ans, f(i + 1, n, m, games, cnts));
            cnts[j]--; // 回溯
        }
        return ans;
    }
}

// 暴力解法入口（用于验证）
int enough1(int n, vector<vector<int>>& games) {
    int m = games.size();
    vector<int> cnts(m, 0); // 记录每个项目分配的人数
    return f(0, n, m, games, cnts);
}

// 优化解法（贪心策略）
int enough2(int n, vector<vector<int>>& games) {
    // 创建按边际收益排序的大根堆
    priority_queue<Game, vector<Game>, CompareGame> heap;
    for (auto& g : games) {
        heap.emplace(g[0], g[1]); // 初始化所有项目
    }

    long long ans = 0;
    for (int i = 0; i < n; i++) { // 逐个分配人员
        if (heap.empty() || heap.top().earn() <= 0) {
            break; // 没有收益时提前终止
        }
        // 取出当前收益最大的项目
        Game cur = heap.top();
        heap.pop();
        ans += cur.earn(); // 累加收益
        cur.people++;      // 更新分配人数
        heap.push(cur);    // 重新入堆更新排序
    }
    return (int)ans;
}

// 生成随机测试数据
vector<vector<int>> randomGames(int m, int v) {
    vector<vector<int>> ans(m, vector<int>(2));
    for (int i = 0; i < m; i++) {
        ans[i][0] = rand() % v + 1; // ki ∈ [1,v]
        ans[i][1] = rand() % v + 1; // bi ∈ [1,v]
    }
    return ans;
}

int main() {
    srand(time(0)); // 初始化随机种子

    // 测试参数配置
    int N = 8;       // 最大人数
    int M = 8;       // 最大项目数
    int V = 20;      // 参数最大值
    int testTimes = 2000; // 测试次数

    cout << "" << endl;
    for (int i = 1; i <= testTimes; i++) {
        int n = rand() % N + 1;      // 随机人数
        int m = rand() % M + 1;      // 随机项目数
        auto games = randomGames(m, V); // 生成测试数据

        // 验证两种方法结果一致性
        int ans1 = enough1(n, games);
        int ans2 = enough2(n, games);
        if (ans1 != ans2) {
            cout << "" << endl;
            return -1;
        }

        // 进度提示
        if (i % 100 == 0) {
            cout << " " << i << " " << endl;
        }
    }
    cout << "" << endl;
    return 0;
}
