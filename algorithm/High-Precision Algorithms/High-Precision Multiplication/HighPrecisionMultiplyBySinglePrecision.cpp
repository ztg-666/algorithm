
// 初始化一个空字符串 s 和一个大小为 250 的数组 a，初始值为 0
string s = "";
int a[250] = {0};
int b = 0; // b 的值小于 10000

// 从标准输入读取字符串 s 和整数 b
cin >> s >> b;

// 将字符串 s 的每一位数字存入数组 a 中，从后往前存储
for(int i = 0; i < s.size(); i++) {
    a[s.size() - i] = s[i] - '0';
}

// 将数组 a 中的每一位数字乘以 b
for(int i = 1; i <= s.size(); i++) {
    a[i] *= b;
}

// 处理数组 a 中的进位
for(int i = 1; i <= s.size() + 4; i++) {
    if(a[i] >= 10) {
        a[i + 1] += a[i] / 10; // 将进位加到下一位
        a[i] %= 10; // 当前位保留个位数
    }
}

// 找到数组 a 中第一个非零元素的位置
int index = 0;
for(int i = s.size() + 4; i >= 1; i--) {
    if(a[i] != 0) {
        index = i;
        break;
    }
}

// 从第一个非零元素开始输出数组 a 中的每一位数字
for(int i = index; i >= 1; i--) {
    cout << a[i];
}

