#include <iostream>
#include <vector>

// 相乘为正或负的子数组数量
// 给定一个长度为n的数组arr，其中所有值都不是0
// 返回有多少个子数组相乘的结果是正
// 返回有多少个子数组相乘的结果是负
// 1 <= n <= 10^6
// -10^9 <= arr[i] <= +10^9，arr[i]一定不是0
// 测试链接 : https://codeforces.com/problemset/problem/1215/B

using namespace std;

const int MAXN = 200001;

int n;
int arr[MAXN];
long long ans1, ans2;  // ans1: 正数乘积的子数组数量, ans2: 负数乘积的子数组数量

void compute() {
    // cnt[0]: 偶数个负数的前缀数量, cnt[1]: 奇数个负数的前缀数量
    int cnt[2] = {1, 0};  // 初始化：空前缀有0个负数（偶数个）
    ans1 = ans2 = 0;

    // cur表示当前位置前缀中负数的个数的奇偶性
    // 0表示偶数个负数，1表示奇数个负数
    for (int i = 1, cur = 0; i <= n; i++) {
        // 如果当前数是负数，则翻转cur的奇偶性
        cur ^= arr[i] > 0 ? 0 : 1;

        // 关键思想：
        // 子数组[j+1, i]的乘积为正 <=> 前缀[1,j]和前缀[1,i]的负数个数奇偶性相同
        // 子数组[j+1, i]的乘积为负 <=> 前缀[1,j]和前缀[1,i]的负数个数奇偶性不同

        ans1 += cnt[cur];        // 与当前前缀奇偶性相同的前缀数量 -> 正数乘积子数组
        ans2 += cnt[cur ^ 1];    // 与当前前缀奇偶性不同的前缀数量 -> 负数乘积子数组

        cnt[cur]++;  // 更新当前奇偶性的前缀计数
    }
}

int main() {
    ios_base::sync_with_stdio(false);  // 优化输入输出速度
    cin.tie(NULL);

    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
    }

    compute();

    // 输出：先输出负数乘积子数组数量，再输出正数乘积子数组数量
    cout << ans2 << " " << ans1 << endl;

    return 0;
}
