#include <iostream>
#include <vector>
#include <cstring>

// 高斯消元解决异或方程组模版题
// 有一个长度为n的数组arr，可能有重复值，数字都是long类型的正数
// 每个数拥有的质数因子一定不超过2000，每个数最多挑选一次
// 在至少要选一个数的情况下，你可以随意挑选数字乘起来
// 乘得的结果需要是完全平方数，请问有几种挑选数字的方法
// 方法数可能很大，答案对 1000000007 取模
// 1 <= n <= 300
// 1 <= arr[i] <= 10^18
// 测试链接 : https://acm.hdu.edu.cn/showproblem.php?pid=5833

using namespace std;

const int MOD = 1000000007;  // 模数
const int MAXV = 2000;       // 最大质数范围
const int MAXN = 305;        // 最大数组长度

long long arr[MAXN];         // 输入的数组
int mat[MAXN][MAXN];         // 高斯消元的系数矩阵
int prime[MAXV + 1];         // 存储质数的数组
int cnt;                     // 质数的个数
bool visit[MAXV + 1];        // 埃氏筛标记数组
int pow2[MAXN];              // 预计算的2的幂次方
int n;                       // 数组长度

// 预处理函数：计算质数和2的幂次方
void prepare() {
    // 使用埃氏筛法求2000以内的所有质数
    memset(visit, false, sizeof(visit));

    // 埃氏筛：标记所有合数
    for (int i = 2; i * i <= MAXV; i++) {
        if (!visit[i]) {  // i是质数
            // 标记i的所有倍数为合数
            for (int j = i * i; j <= MAXV; j += i) {
                visit[j] = true;
            }
        }
    }

    // 收集所有质数
    cnt = 0;
    for (int i = 2; i <= MAXV; i++) {
        if (!visit[i]) {  // i是质数
            prime[++cnt] = i;
        }
    }

    // 预计算2的幂次方，避免重复计算
    pow2[0] = 1;
    for (int i = 1; i < MAXN; i++) {
        pow2[i] = (1LL * pow2[i - 1] * 2) % MOD;
    }
}

// 交换矩阵的两行
void swapRows(int a, int b) {
    for (int i = 1; i <= cnt + 1; i++) {
        swap(mat[a][i], mat[b][i]);
    }
}

// 高斯消元法解异或方程组
void gauss(int n) {
    // 对每一列进行消元
    for (int i = 1; i <= n; i++) {
        // 寻找第i列中第一个为1的元素作为主元
        for (int j = 1; j <= n; j++) {
            // 跳过已经处理过的主元行
            if (j < i && mat[j][j] == 1) {
                continue;
            }
            // 找到第i列为1的行
            if (mat[j][i] == 1) {
                swapRows(i, j);  // 将该行交换到第i行
                break;
            }
        }

        // 如果第i行第i列是主元（值为1），进行消元操作
        if (mat[i][i] == 1) {
            // 消除其他行在第i列的1
            for (int j = 1; j <= n; j++) {
                if (i != j && mat[j][i] == 1) {
                    // 用第i行消除第j行在第i列的1
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] ^= mat[i][k];  // 异或操作
                    }
                }
            }
        }
    }
}

// 主要计算函数
int compute() {
    // 初始化系数矩阵为0
    for (int i = 1; i <= cnt; i++) {
        for (int j = 1; j <= cnt + 1; j++) {
            mat[i][j] = 0;
        }
    }

    // 构建系数矩阵
    // 每个数对应一列，每个质数对应一行
    for (int i = 1; i <= n; i++) {
        long long cur = arr[i];
        // 对每个质数进行质因数分解
        for (int j = 1; j <= cnt && cur != 0; j++) {
            // 统计质数prime[j]在cur中出现的次数
            while (cur % prime[j] == 0) {
                mat[j][i] ^= 1;  // 异或1，表示该质数出现奇数次
                cur /= prime[j];
            }
        }
    }

    // 进行高斯消元
    gauss(cnt);

    // 统计主元（基础解）的个数
    int main_vars = 0;  // 重命名避免与main函数冲突
    for (int i = 1; i <= cnt; i++) {
        if (mat[i][i] == 1) {
            main_vars++;
        }
    }

    // 计算最终结果
    // 自由元个数 = n - main_vars
    // 每个自由元都可以选择选或不选，共有2^(n-main_vars)种选择
    // 但题目要求至少选一个数，所以要排除全不选的情况，即减1
    // 为了防止结果为负数，加上MOD再取模
    return (pow2[n - main_vars] - 1 + MOD) % MOD;
}

int main() {
    // 优化输入输出速度
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 预处理
    prepare();

    int test;
    cin >> test;  // 读取测试用例数量

    // 处理每个测试用例
    for (int t = 1; t <= test; t++) {
        cin >> n;  // 读取数组长度

        // 读取数组元素
        for (int i = 1; i <= n; i++) {
            cin >> arr[i];
        }

        // 输出结果
        cout << "Case #" << t << ":" << endl;
        cout << compute() << endl;
    }

    return 0;
}

/*
算法思路说明：
1. 题目要求选择数字使得它们的乘积是完全平方数
2. 一个数是完全平方数当且仅当它的每个质因子都出现偶数次
3. 我们可以把每个数用质因数分解，用异或方程组来表示：
   - 每个质数对应一个方程
   - 每个数对应一个变量（选或不选）
   - 如果某个数包含某个质数奇数次，则在对应方程中该变量的系数为1
4. 使用高斯消元法求解异或方程组
5. 解的个数 = 2^(自由元个数) - 1（减1是因为至少要选一个数）

时间复杂度：O(质数个数^3 + n*质数个数)
空间复杂度：O(质数个数^2)
*/
