#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1002;     // 选手数量上限 (n <= 1000, 多留空间)
const int MAXM = 3001;     // 边数上限 (誓言数+得分数都 <= 1000，留余量)
const int MAXQ = 1000001;  // 队列大小上限 (SPFA 队列)

// ========== 常量定义 ==========
const double INF = 1e10;   // 无穷大，用来初始化 dist
const double SML = 1e-6;   // 精度 (二分 & 处理浮点比较用)

int n, m1, m2; // n 选手数, m1 誓言数, m2 得分数

// ========== 输入数据结构 ==========
// 誓言 vow[i] = {type, u, v, k}
// type = 1 : "u 没有 k 倍杀 v"
// type = 2 : "u 被 v k 倍杀"
struct Vow {
    int type, u, v;
    double k;
} vow[MAXN];

// 得分记录 score[i] = {u, w} (u 选手的得分为 w)
struct Score {
    int u;
    double w;
} score[MAXN];

// ========== 链式前向星存图 ==========
int head[MAXN], nxt[MAXM], to[MAXM], cnt;
double weight[MAXM]; // 边权是 double (log 运算后会有小数)

// ========== SPFA 需要的数据 ==========
double dist[MAXN];      // 最短路估计值
int updateCnt[MAXN];    // 某点被更新的次数 (用于检测负环)
int q[MAXQ];            // 手动维护的队列
bool inQueue[MAXN];     // 是否在队列中

// 初始化图和 SPFA 的辅助数组
void prepare() {
    cnt = 1; // 边从 1 开始编号 (方便用 head[u]=0 表示没有边)
    fill(head, head + n + 2, 0);
    fill(dist, dist + n + 2, INF);
    fill(updateCnt, updateCnt + n + 2, 0);
    fill(inQueue, inQueue + n + 2, false);
}

// 添加一条有向边 u -> v，权值为 w
void addEdge(int u, int v, double w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

// ========== SPFA 检测负环 ==========
// 如果发现负环，返回 true (说明有人穿女装)
// 如果没有负环，返回 false (无人穿女装)
bool spfa(int s) {
    int h = 0, t = 0;
    dist[s] = 0;       // 源点到自己的距离为 0
    updateCnt[s] = 1;  // 入队次数
    q[t++] = s;
    inQueue[s] = true;

    while (h < t) {
        int u = q[h++];
        inQueue[u] = false;
        for (int ei = head[u]; ei; ei = nxt[ei]) {
            int v = to[ei];
            double w = weight[ei];
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (!inQueue[v]) {
                    // 如果某点被更新超过 n+1 次 => 负环
                    if (++updateCnt[v] > n + 1) {
                        return true; // 有人穿女装
                    }
                    q[t++] = v;
                    inQueue[v] = true;
                }
            }
        }
    }
    return false; // 没有负环 => 无人穿女装
}

// ========== 检查给定 ans 是否有人穿女装 ==========
// limit = 当前二分出来的 ans
bool check(double limit) {
    prepare();

    // 0 号点是连通超级源点，保证每个选手都连通
    for (int i = 1; i <= n; i++) {
        addEdge(0, i, 0);
    }

    // 遍历所有誓言，建边
    for (int i = 1; i <= m1; i++) {
        if (vow[i].type == 1) {
            // 类型 1： u 没有 (k - ans) 倍杀 v
            // 转换成： log(S[v]) - log(S[u]) <= -log(k - ans)
            addEdge(vow[i].u, vow[i].v, -log(-limit + vow[i].k));
        } else {
            // 类型 2： u 被 v (k + ans) 倍杀
            // 转换成： log(S[v]) - log(S[u]) >= log(k + ans)
            // 即 log(S[u]) - log(S[v]) <= -log(k + ans)
            // 这里取 log(k+ans-SML) 让严格不等式转成 <=
            addEdge(vow[i].u, vow[i].v, log(limit + vow[i].k - SML));
        }
    }

    // n+1 号点是限制超级源点，保证已知分数选手之间的关系
    for (int i = 1; i <= m2; i++) {
        addEdge(n + 1, score[i].u, log(score[i].w));      // log(S[u]) >= log(w)
        addEdge(score[i].u, n + 1, -log(score[i].w));     // log(S[u]) <= log(w)
    }

    // 检测负环 => 有人穿女装
    return spfa(0);
}

// ========== 二分计算最大 ans ==========
double compute() {
    double l = 0, r = INF, ans = 0;
    while (r - l >= SML) {
        double m = (l + r) / 2;
        if (check(m)) {
            // 如果 limit=m 时依然有人穿女装，说明还可以更大
            ans = m;
            l = m + SML;
        } else {
            // 否则，说明太大了，缩小右边界
            r = m - SML;
        }
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m1 >> m2;
    for (int i = 1; i <= m1; i++) {
        cin >> vow[i].type >> vow[i].u >> vow[i].v >> vow[i].k;
    }
    for (int i = 1; i <= m2; i++) {
        cin >> score[i].u >> score[i].w;
    }

    double ans = compute();
    if (fabs(ans) < 1e-9) {
        // 如果 ans=0，说明原始情况下无人穿女装
        cout << -1 << "\n";
    } else {
        cout << fixed << setprecision(4) << ans << "\n";
    }
    return 0;
}
