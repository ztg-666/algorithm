#include <iostream>
#include <vector>
#include <algorithm> // 用于 std::sort, std::min
#include <utility>   // 用于 std::pair
#include <climits>   // 用于 INT_MAX
using namespace std;
// 定义静态数组的最大大小（根据需要调整）
const int MAXN = 100005;


// 全局变量
int n, d;
std::vector<pair<int, int>> arr; // 使用 pair 的 vector 存储水滴

// 静态数组用于单调 deque（存储索引）
int maxDeque[MAXN];
int minDeque[MAXN];
int maxh, maxt, minh, mint; // deque 的头和尾指针


// 检查当前窗口（由 deque 表示）是否满足条件：max_height - min_height >= d
bool ok() {
    // 从 maxDeque 的头部获取最大高度（如果非空）
    // arr[maxDeque[maxh]].second 访问 y 坐标（高度）
    int max_y = (maxh < maxt) ? arr[maxDeque[maxh]].second : 0; // 如果为空，默认为 0，但在有效逻辑流中不应发生
    // 从 minDeque 的头部获取最小高度（如果非空）
    int min_y = (minh < mint) ? arr[minDeque[minh]].second : 0; // 如果为空，默认为 0

    // 需要处理 deque 可能初始为空或逻辑上为空的情况。
    // 如果两个都为空，条件不满足。如果只有一个为空，max_y - 0 >= d 或 0 - min_y >= d 是否有意义？
    // 逻辑依赖于至少有一个元素被推入。
    // 更安全的检查可能是：
    if (maxh >= maxt || minh >= mint) { // 如果任一 deque 实际上为空
        return false; // 无法满足差值条件
    }
    // 如果两个 deque 都有元素，则继续检查：
    max_y = arr[maxDeque[maxh]].second;
    min_y = arr[minDeque[minh]].second;
    return max_y - min_y >= d;
}

// 将索引 'r' 添加到两个单调 deque，保持其属性
void push(int r) {
    // 维护 maxDeque 的递减顺序（按高度）
    // arr[index].second 获取 y 坐标（高度）
    while (maxh < maxt && arr[maxDeque[maxt - 1]].second <= arr[r].second) {
        maxt--; // 从尾部移除较小或相等的元素
    }
    maxDeque[maxt++] = r; // 添加索引 r

    // 维护 minDeque 的递增顺序（按高度）
    while (minh < mint && arr[minDeque[mint - 1]].second >= arr[r].second) {
        mint--; // 从尾部移除较大或相等的元素
    }
    minDeque[mint++] = r; // 添加索引 r
}

// 从 deque 的头部移除索引 'l'（窗口的最左边元素）
// 如果它在头部存在
void pop(int l) {
    // 如果移出的元素（索引 l）是当前最大值，移动头部
    if (maxh < maxt && maxDeque[maxh] == l) {
        maxh++;
    }
    // 如果移出的元素（索引 l）是当前最小值，移动头部
    if (minh < mint && minDeque[minh] == l) {
        minh++;
    }
}
// 接取落水的最小花盆
// 老板需要你帮忙浇花。给出 N 滴水的坐标，y 表示水滴的高度，x 表示它下落到 x 轴的位置
// 每滴水以每秒1个单位长度的速度下落。你需要把花盆放在 x 轴上的某个位置
// 使得从被花盆接着的第 1 滴水开始，到被花盆接着的最后 1 滴水结束，之间的时间差至少为 D
// 我们认为，只要水滴落到 x 轴上，与花盆的边沿对齐，就认为被接住
// 给出 N 滴水的坐标和 D 的大小，请算出最小的花盆的宽度 W
// 测试链接 : https://www.luogu.com.cn/problem/P2698
// 计算最小花盆宽度的函数
int compute() {
    // 根据 x 坐标对水滴进行排序
    // Lambda 函数不捕获任何变量，接受两个 const pair<int, int> 引用 a 和 b
    // 返回 a.first (a 的 x 坐标) < b.first (b 的 x 坐标)
    std::sort(arr.begin(), arr.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first < b.first;
    });

    // 重置 deque 指针
    maxh = maxt = minh = mint = 0;
    int ans = INT_MAX; // 初始化答案为最大可能的整数值
    int r = 0;         // 滑动窗口的右指针

    // 滑动窗口方法
    for (int l = 0; l < n; ++l) {
        // 向右扩展窗口 [l, r)
        // 只要高度差条件（max_y - min_y >= d）不满足，就继续添加水滴（索引）到 deque
        while (r < n && !ok()) {
            push(r++); // 将索引 r 处的水滴添加到 deque 并递增 r
        }

        // 如果条件现在满足（窗口 [l, r-1] 是有效的）
        if (ok()) {
            // 计算当前有效窗口的宽度
            // 宽度 = 最右边水滴的 x 坐标 - 最左边水滴的 x 坐标
            int current_width = arr[r - 1].first - arr[l].first;
            // 更新迄今为止找到的最小宽度
            ans = std::min(ans, current_width);
        }

        // 滑动窗口：移除最左边水滴（索引 l）对 deque 的影响
        // 在下一次迭代之前。
        pop(l);
    }
    return ans;
}


int main() {
    // 加快输入输出
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    // 读取输入直到 EOF
    while (std::cin >> n >> d) {
        // 调整 vector 大小以容纳当前测试用例的 n 个水滴
        arr.resize(n);
        for (int i = 0; i < n; ++i) {
            // 读取每个水滴的 x 和 y 坐标
            std::cin >> arr[i].first >> arr[i].second;
        }

        // 计算当前测试用例的结果
        int ans = compute();

        // 输出结果：ans 或 -1 如果没有找到解
        if (ans == INT_MAX) {
            std::cout << -1 << "\n";
        } else {
            std::cout << ans << "\n";
        }
    }

    return 0;
}
