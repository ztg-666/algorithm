高精度数 / 高精度数
// 将数组x的内容复制到数组y中，从y的offset位置开始
void cpy(int x[], int y[], int offset) {
    for (int i = 1; i <= x[0]; i++) {
        y[i + offset] = x[i];
    }
    y[0] = x[0] + offset;
}

// 比较两个数组x和y的大小，返回x是否大于y
bool comp(int x[], int y[]) {
    if (x[0] != y[0]) return x[0] > y[0];
    for (int i = 1; i <= x[0]; i++) {
        if (x[i] != y[i]) return x[i] > y[i];
    }
    return false; // 如果两个数组完全相同，返回false
}

// 从数组x中减去数组y
void sub(int x[], int y[]) {
    for (int i = 1; i <= x[0]; i++) {
        if (x[i] < y[i]) {
            x[i] += 10;
            x[i - 1]--;
        }
        x[i] -= y[i];
    }
}

int main() {
    string s1, s2;
    int a[250] = {0}, b[250] = {0}, c[250] = {0};
    cin >> s1 >> s2;

    // 将输入字符串s1转换为数组a
    a[0] = s1.size();
    for (int i = 0; i < s1.size(); i++) {
        a[i + 1] = s1[i] - '0';
    }

    // 将输入字符串s2转换为数组b
    b[0] = s2.size();
    for (int i = 0; i < s2.size(); i++) {
        b[i + 1] = s2[i] - '0';
    }

    // 初始化数组c的大小
    c[0] = a[0] - b[0] + 1;
    int t[500] = {0};

    // 计算数组a除以数组b的结果，存储在数组c中
    for (int i = 1; i <= c[0]; i++) {
        memset(t, 0, sizeof(t));
        cpy(b, t, i - 1);
        a[0] = t[0];
        while (comp(a, t) == true) {
            sub(a, t);
            c[i]++;
        }
    }

    // 输出商数组c
    int index = 1;
    for (int i = 1; i <= c[0]; i++) {
        if (c[i] != 0) {
            index = i;
            break;
        }
    }
    for (int i = index; i <= c[0]; i++) {
        cout << c[i];
    }
    cout << endl;

    // 输出余数数组a
    int index2 = 1;
    for (int i = 1; i <= a[0]; i++) {
        if (a[i] != 0) {
            index2 = i;
            break;
        }
    }
    for (int i = index2; i <= a[0]; i++) {
        cout << a[i];
    }
}
