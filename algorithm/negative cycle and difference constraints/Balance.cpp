#include <bits/stdc++.h>

// 天平
// 一共有n个砝码，编号1~n，每个砝码的重量均为1克，或者2克，或者3克 
// 砝码与砝码之间的关系是一个n * n的二维数组s
// s[i][j] == '+'，砝码i比砝码j重        s[i][j] == '-'，砝码i比砝码j轻
// s[i][j] == '='，砝码i和砝码j重量一样   s[i][j] == '?'，砝码i和砝码j关系未知
// 数据保证至少存在一种情况符合该矩阵
// 给定编号为a和b的砝码，这两个砝码一定会放在天平的左边，你要另选两个砝码放在天平右边
// 返回有多少种方法，一定让天平左边重(ans1)，一定让天平一样重(ans2)，一定让天平右边重(ans3)
// 1 <= n <= 50
// 测试链接 : https://www.luogu.com.cn/problem/P2474

using namespace std;

const int MAXN = 55;

int dminn[MAXN][MAXN]; // 表示 i 比 j 最小可能的相对重量差
int dmaxx[MAXN][MAXN]; // 表示 i 比 j 最大可能的相对重量差
char s[MAXN][MAXN];    // 输入的关系矩阵：'=','+','-','?'

int n, a, b;
int ans1, ans2, ans3;  // 答案：左边必重、必相等、右边必重

void compute() {
    // Step 1: 初始化两两之间的重量关系范围
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            if (s[i][j] == '=') {
                // i 和 j 一定相等
                dminn[i][j] = 0;
                dmaxx[i][j] = 0;
            } else if (s[i][j] == '+') {
                // i 比 j 重（范围 1 到 2）
                dminn[i][j] = 1;
                dmaxx[i][j] = 2;
            } else if (s[i][j] == '-') {
                // i 比 j 轻（范围 -2 到 -1）
                dminn[i][j] = -2;
                dmaxx[i][j] = -1;
            } else { // '?'
                // 不确定，可能比对方轻 2，重 2，或者相等
                dminn[i][j] = -2;
                dmaxx[i][j] = 2;
            }
        }
    }
    // 自身与自身重量差为 0
    for (int i = 1; i <= n; i++) {
        dminn[i][i] = 0;
        dmaxx[i][i] = 0;
    }

    // Step 2: Floyd 传递闭包，推导间接关系
    // 例如：若 A 比 B 重至少 1，B 比 C 重至少 1，则 A 比 C 重至少 2
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                // 更新最小可能重量差：走 i->k->j 的路径
                dminn[i][j] = max(dminn[i][j], dminn[i][k] + dminn[k][j]);
                // 更新最大可能重量差：走 i->k->j 的路径
                dmaxx[i][j] = min(dmaxx[i][j], dmaxx[i][k] + dmaxx[k][j]);
            }
        }
    }

    // Step 3: 枚举所有可能的对手组合 (i, j)，判断天平结果
    ans1 = ans2 = ans3 = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j < i; j++) { // 避免重复对 (i, j) 与 (j, i)
            if (i != a && i != b && j != a && j != b) {
                // ------- 判断左边一定重 -------
                // 条件：无论如何分配重量，a+i 组合都比 b+j 组合重
                if (dminn[a][i] > dmaxx[j][b] || dminn[a][j] > dmaxx[i][b]) {
                    ans1++;
                }
                // ------- 判断右边一定重 -------
                // 条件：无论如何分配重量，b+j 组合都比 a+i 组合重
                if (dmaxx[a][i] < dminn[j][b] || dmaxx[a][j] < dminn[i][b]) {
                    ans3++;
                }
                // ------- 判断一定相等 -------
                // 条件：两边最小差和最大差都相等，且值相同
                if (dminn[a][i] == dmaxx[a][i] && dminn[j][b] == dmaxx[j][b] && dminn[a][i] == dminn[j][b]) {
                    ans2++;
                }
                // 另一种对称情况
                else if (dminn[b][i] == dmaxx[b][i] && dminn[j][a] == dmaxx[j][a] && dminn[b][i] == dminn[j][a]) {
                    ans2++;
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 输入：n 个物品，a 和 b 是固定放在天平左右两边的物品
    cin >> n >> a >> b;
    for (int i = 1; i <= n; i++) {
        string line;
        cin >> line;
        for (int j = 1; j <= n; j++) {
            s[i][j] = line[j - 1];
        }
    }

    // 计算答案
    compute();
    // 输出：左边必重数、必相等数、右边必重数
    cout << ans1 << " " << ans2 << " " << ans3 << "\n";

    return 0;
}
