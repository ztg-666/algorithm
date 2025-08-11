#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>

// 工具工厂
// 一共有n种工具，编号1~n，一共有m条记录，其中一条记录格式如下：
// 4 WED SUN 13 18 1 13
// 表示有个工人一共加工了4件工具，从某个星期三开始工作，到某个星期天结束工作
// 加工的工具依次为13号、18号、1号、13号
// 每个工人在工作期间不休息，每件工具都是串行加工的，完成一件后才开始下一件
// 每种工具制作天数是固定的，并且任何工具的制作天数最少3天、最多9天
// 但该数据丢失了，所以现在需要根据记录，推断出每种工具的制作天数
// 如果记录之间存在矛盾，打印"Inconsistent data."
// 如果记录无法确定每种工具的制作天数，打印"Multiple solutions."
// 如果记录能够确定每种工具的制作天数，打印所有结果
// 1 <= n、m <= 300
// 测试链接 : http://poj.org/problem?id=2947

using namespace std;

const int MOD = 7;          // 一周有7天，所有运算都在模7意义下进行
const int MAXN = 302;       // 最大的n和m都是300，留出一些余量

int mat[MAXN][MAXN];        // 增广矩阵，用于高斯消元
int inv_table[MOD];         // 模逆元表，用于除法运算
string days[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"}; // 星期映射
int n, m, s;                // n:工具数, m:记录数, s:矩阵大小

// 预计算模逆元表
// 在模7意义下，每个非零数都有唯一的逆元
void init_inv() {
    inv_table[1] = 1;       // 1的逆元是1
    // 使用递推公式计算逆元: inv[i] = MOD - (MOD/i) * inv[MOD%i] % MOD
    for (int i = 2; i < MOD; i++) {
        inv_table[i] = (int)(MOD - (long long)inv_table[MOD % i] * (MOD / i) % MOD);
    }
}

// 欧几里得算法求最大公约数
int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

// 初始化增广矩阵，清零所有元素
void prepare() {
    for (int i = 1; i <= s; i++) {
        for (int j = 1; j <= s + 1; j++) {
            mat[i][j] = 0;
        }
    }
}

// 将星期字符串转换为索引(0-6)
int getDayIndex(string str) {
    for (int i = 0; i < 7; i++) {
        if (str == days[i]) {
            return i;
        }
    }
    return -1;
}

// 交换矩阵的两行
void swapRows(int a, int b) {
    for (int j = 1; j <= s + 1; j++) {
        swap(mat[a][j], mat[b][j]);
    }
}

// 高斯消元法解决模7的线性同余方程组
// 这是核心算法，用于求解每种工具的制作天数
void gauss(int n) {
    // 对每一行进行处理
    for (int i = 1; i <= n; i++) {
        // 寻找主元（非零元素）进行行交换
        for (int j = 1; j <= n; j++) {
            if (j < i && mat[j][j] != 0) {
                continue;  // 已经处理过的行跳过
            }
            if (mat[j][i] != 0) {
                swapRows(i, j);  // 找到非零元素，交换行
                break;
            }
        }

        // 如果当前行的主元不为0，进行消元操作
        if (mat[i][i] != 0) {
            // 对其他行进行消元
            for (int j = 1; j <= n; j++) {
                if (i != j && mat[j][i] != 0) {
                    // 计算最大公约数，避免系数过大
                    int g = gcd(mat[j][i], mat[i][i]);
                    int a = mat[i][i] / g;  // 第i行的倍数
                    int b = mat[j][i] / g;  // 第j行的倍数

                    // 处理之前的列
                    if (j < i && mat[j][j] != 0) {
                        for (int k = j; k < i; k++) {
                            mat[j][k] = (mat[j][k] * a) % MOD;
                        }
                    }

                    // 消元操作：第j行 = a*第j行 - b*第i行
                    for (int k = i; k <= n + 1; k++) {
                        mat[j][k] = ((mat[j][k] * a - mat[i][k] * b) % MOD + MOD) % MOD;
                    }
                }
            }
        }
    }

    // 将对角线元素化为1（如果该行只有对角线元素非零）
    for (int i = 1; i <= n; i++) {
        if (mat[i][i] != 0) {
            bool flag = false;
            // 检查该行是否还有其他未知数
            for (int j = i + 1; j <= n; j++) {
                if (mat[i][j] != 0) {
                    flag = true;
                    break;
                }
            }
            // 如果只有对角线元素，将其化为1
            if (!flag) {
                mat[i][n + 1] = (mat[i][n + 1] * inv_table[mat[i][i]]) % MOD;
                mat[i][i] = 1;
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    init_inv();  // 初始化模逆元表

    // 持续读取测试用例直到n=0且m=0
    while (cin >> n >> m && (n != 0 || m != 0)) {
        s = max(n, m);  // 矩阵大小取较大值
        prepare();      // 初始化矩阵

        // 处理每条工作记录
        for (int i = 1; i <= m; i++) {
            int k;
            string st, et;  // 开始和结束的星期
            cin >> k >> st >> et;

            // 统计每种工具的数量
            for (int j = 1; j <= k; j++) {
                int tool;
                cin >> tool;
                mat[i][tool] = (mat[i][tool] + 1) % MOD;  // 工具tool的系数+1
            }

            // 计算总工作天数（模7）
            // 从st到et的天数，注意跨周的情况
            mat[i][s + 1] = ((getDayIndex(et) - getDayIndex(st) + 1) % MOD + MOD) % MOD;
        }

        gauss(s);  // 进行高斯消元

        // 判断方程组的解的情况
        int sign = 1;  // 1:唯一解, 0:多解, -1:无解
        for (int i = 1; i <= s; i++) {
            // 如果某行系数全为0但常数项不为0，则无解
            if (mat[i][i] == 0 && mat[i][s + 1] != 0) {
                sign = -1;
                break;
            }
            // 如果某个工具对应的行系数为0，则有多解
            if (i <= n && mat[i][i] == 0) {
                sign = 0;
            }
        }

        // 根据解的情况输出结果
        if (sign == -1) {
            cout << "Inconsistent data.\n";  // 数据矛盾
        } else if (sign == 0) {
            cout << "Multiple solutions.\n";  // 多个解
        } else {
            // 唯一解的情况
            // 确保每种工具的制作天数在3-9天之间
            for (int i = 1; i <= n; i++) {
                if (mat[i][s + 1] < 3) {
                    mat[i][s + 1] += 7;  // 如果小于3天，加7天（模7意义下相等）
                }
            }
            // 输出每种工具的制作天数
            for (int i = 1; i < n; i++) {
                cout << mat[i][s + 1] << " ";
            }
            cout << mat[n][s + 1] << "\n";
        }
    }

    return 0;
}
