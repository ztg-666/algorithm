
string s = ""; // 用于存储输入的被除数字符串
int a[250] = {0}; // 用于存储被除数的每一位数字
int b = 0; // 除数
int c[250] = {0}; // 用于存储商的每一位数字
cin >> s >> b; // 从用户输入读取被除数字符串和除数

// 将字符串形式的被除数转换为整数数组
for(int i = 0; i < s.size(); i++) {
    a[i + 1] = s[i] - '0'; // 将字符转换为对应的整数并存储在数组a中
}

int t = 0; // 用于存储当前的余数
// 计算商的每一位数字
for(int i = 1; i <= s.size(); i++) {
    t = t * 10 + a[i]; // 将当前余数乘以10并加上当前位的数字
    if(t >= b) {
        c[i] = t / b; // 计算当前位的商并存储在数组c中
        t = t % b; // 更新余数
    }
    else {
        c[i] = 0; // 如果当前余数小于除数，商为0
    }
}

int index = 1; // 用于找到第一个非零的商的位
// 找到第一个非零的商的位
for(int i = 1; i <= s.size(); i++) {
    if(c[i] != 0) {
        index = i; // 找到第一个非零的商的位
        break;
    }
}

// 输出商的每一位数字，从第一个非零的位开始
for(int i = index; i <= s.size(); i++) {
    cout << c[i];
}

// 输出剩余的余数
cout << "....." << t;
