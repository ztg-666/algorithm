
// 定义两个字符串 s1 和 s2 用于存储输入的两个大数
string s1, s2;
// 定义三个数组 a, b, c 分别用于存储 s1, s2 的每一位数字和它们的乘积结果
int a[210] = {0}, b[210] = {0}, c[500] = {0};

// 从标准输入读取两个大数字符串
getline(cin, s1);
getline(cin, s2);

// 将字符串 s1 的每一位数字逆序存储到数组 a 中
for(int i = 0; i < s1.size(); i++) {
    a[s1.size() - i] = s1[i] - '0';
}

// 将字符串 s2 的每一位数字逆序存储到数组 b 中
for(int i = 0; i < s2.size(); i++) {
    b[s2.size() - i] = s2[i] - '0';
}

// 计算两个大数的乘积，结果存储在数组 c 中
for(int i = 1; i <= s2.size(); i++) {
    for(int j = 1; j <= s1.size(); j++) {
        c[j + i - 1] += a[j] * b[i]; // 计算乘积并累加到对应位置
        // 如果当前位置的值大于等于 10，进行进位处理
        if(c[j + i - 1] >= 10) {
            c[j + i] += c[j + i - 1] / 10; // 将进位加到下一个位置
            c[j + i - 1] %= 10; // 当前位置保留个位数
        }
    }
}

// 找到乘积结果数组 c 中第一个非零元素的位置
int index = 0;
int len = s1.size() + s2.size();
for(int i = len; i > 0; i--) {
    if(c[i] != 0) {
        index = i;
        break;
    }
}

// 从第一个非零元素位置开始输出乘积结果
for(int i = index; i > 0; i--) {
    cout << c[i];
}
