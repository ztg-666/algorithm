#include <bits/stdc++.h>

// 屠龙勇士
// 一共n只巨龙，每只巨龙都有初始血量hp[i]，每只巨龙都有恢复能力recovery[i]
// 每只巨龙都会在攻击结束后开始恢复，初始一共m把剑，每把剑攻击力init[i]
// 每只巨龙只有当血量恰好为0时，才能被杀死。面对某只具体的龙，只能用固定的剑来攻击，规定如下：
// 攻击力不高于当前巨龙的血量，并且攻击力最大的一把剑，如果没有这样的剑，就选择攻击力最低的一把剑
// 需要按1~n的顺序依次讨伐巨龙，i号巨龙被杀后，那把攻击的剑会消失，同时奖励攻击力reward[i]的剑
// 勇士制定的策略如下，不管面对什么巨龙，攻击过程只打击ans下，让当前巨龙的血量<=0
// 然后在当前巨龙恢复的过程中，如果血量恰好为0，那么当前巨龙被杀死，勇士继续讨伐下一只
// 你的任务是算出最小的ans，让勇士可以在该策略下杀死所有巨龙
// 如果在固定打击次数的策略下，就是无法杀死所有巨龙，返回-1
// 查看数据范围可以打开测试链接 : https://www.luogu.com.cn/problem/P4774

using namespace std;

const int MAXN = 100001;

long long hp[MAXN];          // 每条龙的血量
long long recovery[MAXN];    // 每条龙的恢复力
long long reward[MAXN];      // 每条龙死亡后奖励的剑攻击力
long long init_sword[MAXN];  // 初始持有的剑
long long attack[MAXN];      // 对应每条龙选择的攻击力

map<long long, int> sorted;  // 类似 Java 的 TreeMap，存储当前持有的剑（攻击力->数量）

// ---------------- 扩展欧几里得 ----------------
// 求解 a*x + b*y = gcd(a, b)
long long d, x, y, px, py;
void exgcd(long long a, long long b) {
    if (b == 0) {
        d = a;
        x = 1;
        y = 0;
    } else {
        exgcd(b, a % b);
        px = x;
        py = y;
        x = py;
        y = px - py * (a / b);
    }
}

// ---------------- 龟速乘 ----------------
// 防止 long long * long long 溢出，计算 (a*b) % mod
long long multiply(long long a, long long b, long long mod) {
    a = (a % mod + mod) % mod;
    b = (b % mod + mod) % mod;
    long long ans = 0;
    while (b > 0) {
        if (b & 1) ans = (ans + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return ans;
}

// ---------------- 分配剑 ----------------
// 依次挑战每条龙，确定该龙用哪把剑，返回需要的最大打击次数
long long allocate(int n, int m) {
    sorted.clear();
    // 初始化所有剑
    for (int i = 1; i <= m; i++) {
        sorted[init_sword[i]]++;
    }

    long long max_cnt = 0; // 记录杀死某条龙所需的最大攻击次数
    for (int i = 1; i <= n; i++) {
        // 找到 <= hp[i] 的最大攻击力剑
        auto it = sorted.upper_bound(hp[i]);
        if (it == sorted.begin()) {
            // 没有小于等于 hp[i] 的剑，则选最小的一把
            attack[i] = sorted.begin()->first;
        } else {
            --it;
            attack[i] = it->first;
        }

        // 使用这把剑一次，删除它
        if (--sorted[attack[i]] == 0) {
            sorted.erase(attack[i]);
        }
        // 添加奖励的剑
        sorted[reward[i]]++;

        // 更新最大攻击次数（向上取整）
        max_cnt = max(max_cnt, (hp[i] + attack[i] - 1) / attack[i]);

        // 血量转为余数形式（模恢复力）
        hp[i] %= recovery[i];
    }
    return max_cnt;
}

// ---------------- 主计算 ----------------
// 建立同余方程组，解出最小攻击次数
long long compute(int n, int m) {
    // 第一步：分配剑，得到每条龙的攻击力，并计算最大打击次数
    long long max_cnt = allocate(n, m);

    long long tail = 0; // 当前解
    long long lcm = 1;  // 当前方程组的模（最小公倍数）

    // 构建并解方程组
    for (int i = 1; i <= n; i++) {
        // 方程：ans * attack[i] ≡ hp[i] (mod recovery[i])
        long long a = multiply(attack[i], lcm, recovery[i]); // 系数
        long long b = recovery[i];
        long long c = ((hp[i] - attack[i] * tail) % b + b) % b; // 常数项

        exgcd(a, b);
        if (c % d != 0) return -1; // 无解

        long long x0 = multiply(x, c / d, b / d); // 最小解
        long long tmp = lcm * (b / d);           // 新的模
        tail = (tail + multiply(x0, lcm, tmp)) % tmp; // 更新解
        lcm = tmp;
    }

    // 第二步：特殊处理，保证 ans >= max_cnt
    // 解的通式：ans = tail + k*lcm
    // 若 tail >= max_cnt，则答案就是 tail
    // 否则，取最小的 k 使得 ans >= max_cnt
    long long ans;
    if (tail >= max_cnt) {
        ans = tail;
    } else {
        ans = ((max_cnt - tail + lcm - 1) / lcm) * lcm + tail;
    }
    return ans;
}

// ---------------- 主函数 ----------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int cases;
    cin >> cases;
    while (cases--) {
        int n, m;
        cin >> n >> m;
        for (int i = 1; i <= n; i++) cin >> hp[i];
        for (int i = 1; i <= n; i++) cin >> recovery[i];
        for (int i = 1; i <= n; i++) cin >> reward[i];
        for (int i = 1; i <= m; i++) cin >> init_sword[i];
        cout << compute(n, m) << "\n";
    }
    return 0;
}
