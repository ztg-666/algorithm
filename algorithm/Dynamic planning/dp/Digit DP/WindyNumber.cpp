#include <iostream>
#include <cstring>
using namespace std;

const int MAXLEN = 11;
int dp[MAXLEN][11][2]; // [长度][前驱数字][自由状态]

// windy数
// 不含前导零且相邻两个数字之差至少为2的正整数被称为windy数
// windy想知道[a,b]范围上总共有多少个windy数
// 测试链接 : https://www.luogu.com.cn/problem/P2657
// 初始化dp数组，将指定长度的所有状态设为未计算

void build(int len) {
    for (int i = 0; i <= len; i++)
        for (int j = 0; j <= 10; j++)
            for (int k = 0; k < 2; k++)
                dp[i][j][k] = -1; // -1表示该状态尚未计算
}

/**
 * 数位DP核心递归函数
 * @param num    原始目标数值
 * @param offset 当前处理的数字位权值(如千位、百位等)
 * @param len    剩余需要处理的位数
 * @param pre    前一位选择的数字(初始为10表示未选择)
 * @param free   是否受限：
 *               0-严格受限，不能超过num对应位的值
 *               1-自由模式，可以任意选择数字
 * @return 返回满足条件的数字个数
 */
int f(int num, int offset, int len, int pre, int free) {
    if (len == 0) return 1; // 所有位数处理完成，得到一个有效解

    // 记忆化搜索：如果该状态已计算过，直接返回结果
    if (dp[len][pre][free] != -1) return dp[len][pre][free];

    int cur = (num / offset) % 10; // 获取当前处理位的数字
    int ans = 0;

    if (free == 0) { // 严格受限模式
        if (pre == 10) { // 尚未选择过数字（最高位处理逻辑）
            // 情况1：继续不选择数字
            ans += f(num, offset/10, len-1, 10, 1);
            // 情况2：选择比当前位小的数字（1~cur-1）
            for (int i=1; i<cur; i++)
                ans += f(num, offset/10, len-1, i, 1);
            // 情况3：严格选择当前位数字
            ans += f(num, offset/10, len-1, cur, 0);
        } else { // 已选择过前驱数字
            // 遍历所有可能的数字，检查windy数条件
            for (int i=0; i<=9; i++) {
                if (i <= pre-2 || i >= pre+2) { // 相邻数字差至少2
                    if (i < cur) // 选择小于当前位的数字，进入自由模式
                        ans += f(num, offset/10, len-1, i, 1);
                    else if (i == cur) // 严格选择当前位，保持受限
                        ans += f(num, offset/10, len-1, cur, 0);
                }
            }
        }
    } else { // 自由选择模式
        if (pre == 10) { // 尚未选择过数字（允许继续不选）
            ans += f(num, offset/10, len-1, 10, 1);
            // 选择1-9作为起始数字（避免前导零）
            for (int i=1; i<=9; i++)
                ans += f(num, offset/10, len-1, i, 1);
        } else { // 已选择前驱数字
            // 遍历所有满足windy条件的数字
            for (int i=0; i<=9; i++) {
                if (i <= pre-2 || i >= pre+2) {
                    ans += f(num, offset/10, len-1, i, 1);
                }
            }
        }
    }

    dp[len][pre][free] = ans; // 存储计算结果
    return ans;
}

// 计算[0, num]范围内的windy数数量
int cnt(int num) {
    if (num == 0) return 1; // 边界处理：0视为有效数字
    // 计算数字位数（如1234的len=4，offset=1000）
    int len = 1, offset = 1;
    int tmp = num / 10;
    while (tmp > 0) {
        len++;
        offset *= 10;
        tmp /= 10;
    }
    build(len); // 初始化记忆化数组
    return f(num, offset, len, 10, 0); // 从最高位开始处理
}

// 计算[a, b]区间内的windy数数量
int compute(int a, int b) {
    return cnt(b) - cnt(a - 1); // 利用前缀和思想
}

int main() {
    ios::sync_with_stdio(false); // 关闭同步提升IO速度
    cin.tie(0); // 解除cin与cout的绑定

    int a, b;
    while (cin >> a >> b) { // 循环读取测试用例
        cout << compute(a, b) << endl;
    }
    return 0;
}
