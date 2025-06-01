#include <cstdio>
#include <cstring> // 用于memset函数
#include<iostream>

// 同学找到了洛谷的在线测试，看似是题目3，其实不是
// 区别在于数据量，如下测试链接中的题目
// 会议的数量10^6，会议的开始、结束时间也是10^6
// 排序会超时，C++的同学可能不会，但是java的同学会
// 而且根据这个数据量，最优解一定不是排序！
// 会议开始、结束时间也是10^6规模，所以不排序也可以做，具体看如下代码和注释
// 并不是课上讲错了，而是这个数据状况太特殊
// 测试链接 : https://www.luogu.com.cn/problem/P1803

using namespace std;

const int MAXN = 1000001; // 定义最大时间范围
int latest[MAXN];         // 记录各结束时间对应的最晚开始时间
int n;                    // 会议数量

// 计算最大可安排会议数
int compute() {
    int ans = 0;
    // 不排序
    // 根据时间遍历
    // cur : 当前时间点，end : 当前考虑的结束时间
    for (int cur = 0, end = 0; end < MAXN; end++) {
        // cur : cur之前不能再安排会议，因为安排会议的人来到了cur时刻
        // end是当前的结束时间
        // 所有以end结束的会议，最晚的开始时间是latest[end]
        // 如果cur <= latest[end]，那么说明可以安排当前以end结束的会议

        // 如果当前时间允许安排以end结束的会议
        if (cur <= latest[end]) {
            ans++;        // 安排会议
            cur = end;    // 更新当前时间到会议结束时间
        }
    }
    return ans;
}

int main() {
    while (std::cin >> n) { // 读取会议数量
        // 初始化latest数组为-1（表示无会议）
        memset(latest, -1, sizeof(latest));

        // 读取所有会议时间并更新latest数组
        for (int i = 0, start, end; i < n; i++) {
            cin >> start >> end;
            // 维护当前结束时间的最晚开始时间
            if(latest[end] == -1) {
                latest[end] = start;
            }else {
                latest[end] = max(latest[end], start);
            }
        }

        // 计算并输出结果
        printf("%d\n", compute());
    }
    return 0;
}
