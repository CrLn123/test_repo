# 从 C++ 到 C：可以直接跟着学的教程

> **适合你：** 已经会用 C++ 写基础程序，想迅速弄清“同一件事在 C 中怎么写、哪里容易误判”，补齐 C 的知识，同时把课程上机题做稳。本文用 **C17** 作为示例语言版本。带 `main` 的代码块可单独保存成 `main.c` 运行；没有 `main` 的代码块是片段，放入相应程序中使用。

## 开始前：学习方法与检验方式

你不需要从 `if`、`for` 的语法重新学起。读每一节时按这个顺序做：

1. 看 **C++ → C 映射**，确认自己原本会的东西在 C 中由什么承担。
2. 读 **为什么**，特别关注对象的大小、有效期、类型转换和失败路径。
3. 复制完整例子运行，再故意改一个边界值，预测输出后核对。
4. 不看原文，自己重新写一次。能解释“为什么这样写”，才算会了。
5. 做本节末尾的“动手题”，答案和提示集中在第 16 节。

**主线独立于课件。** 文中的“课件对照”只表示你给的前三章 PPTX 出现过相关内容；它不决定学习顺序，也不等于老师一定在课堂上逐页讲授。C 的后续知识即使尚未出现在前三章，也会完整纳入主线。

### 把环境固定成 C，而不是 C++

保存为 `main.c`，在已安装 GCC 的终端运行：

```powershell
gcc -std=c17 -Wall -Wextra -Wpedantic main.c -o main.exe
./main.exe
```

如果用的是某些 Windows MinGW 版本，`printf` 的 `%zu` 被误报为不支持，可在编译命令中加 `-D__USE_MINGW_ANSI_STDIO=1`。修改代码后先**保存文件**再编译。`main.cpp` 通常按 C++ 编译；你之前截图中 `malloc` 的错误就与语言模式有关。

本教程分三层：第 1～13 节是要反复动手的核心；第 14 节是进阶知识地图；第 15 节是贯穿前面知识的项目；第 16～17 节用于自测和课程对照。时间紧时先做到第 7 节，再补第 8～13 节。

## 1. C 程序的形状：从 `cout` 到 `printf`

**C++ → C：** 你熟悉的 `std::cout`、`std::cin`、类库和命名空间，在 C 中分别由标准输入输出函数、普通函数与头文件承担。两门语言看起来相近，但编译器按不同规则检查它们。

```c
#include <stdio.h>

int main(void)
{
    printf("Hello, C!\n");
    return 0;
}
```

从上到下读：`#include <stdio.h>` 提供 `printf` 的声明；`main` 是程序入口；`void` 说明这里不接收参数；`printf` 中的 `\n` 换行；`return 0` 表示正常结束。与 C++ 的 `#include <iostream>`、`std::cout` 对照，**头文件和输入输出库换了，程序的入口思想没换**。

完整程序运行前，源文件先经过预处理和编译，生成目标代码；链接器再把目标代码与需要的库合成可执行文件。因此三类故障要分开判断：语法/类型错误发生在编译阶段；漏掉函数实现常在链接阶段报错；崩溃或结果错误发生在运行阶段。

**动手题 1：** 把上例改成输出两行文字，然后故意把 `printf` 写成 `Printf`，观察编译器第一条诊断。C 区分大小写。

**课件对照：** 第 1 章幻灯片 22～25、39～40、49～50、57；第 2 章幻灯片 3～5。

## 2. 类型与数值：别把“通常如此”当成语言保证

### 2.1 类型大小、`sizeof` 和固定宽度整数

**C++ → C：** `int`、`long`、`double` 等名称熟悉，但 C 中也不能假设 `int` 永远是 32 位。`sizeof` 的单位是 **C 字节**；它返回 `size_t` 类型的值。一个字节包含多少位由实现决定，可以看 `<limits.h>` 中的 `CHAR_BIT`。

```c
#include <limits.h>
#include <stdio.h>

int main(void)
{
    int scores[4] = {70, 80, 90, 100};
    size_t count = sizeof scores / sizeof scores[0];

    printf("CHAR_BIT=%d\n", CHAR_BIT);
    printf("sizeof(int)=%zu\n", sizeof(int));
    printf("count=%zu\n", count);
    return 0;
}
```

为什么数组元素数是“整个数组字节数 ÷ 一个元素字节数”？因为 `scores` 在这里还是完整数组。如果把数组传给函数，函数形参按指针处理，`sizeof` 得到的就不是原数组大小。不要把这条公式无条件套进函数。

`<stdint.h>` 中的 `uint32_t`（实现提供时）明确是 **32 位无符号整数**。当文件格式、网络协议规定某个字段恰好 32 位时用它；普通循环计数不需要统一换成它。打印固定宽度类型时，可用 `<inttypes.h>` 的格式宏，例如 `printf("%" PRIu32 "\n", value);`。

### 2.2 字面量、进制、字符和布尔值

十进制 `83`、八进制 `0123`、十六进制 `0x53` 表示同一个数。八进制前导是数字 **0**。课件讲二进制数的表示与转换，但本教程的 C17 示例不把 `0b...` 当成标准的二进制整数字面量。

```c
#include <stdio.h>

int main(void)
{
    printf("%d %d %d\n", 83, 0123, 0x53); /* 都是 83 */
    printf("%zu\n", sizeof "A");         /* 2：'A' 和 '\0' */
    return 0;
}
```

`'A'` 是**字符常量**，在 C 中普通字符常量的类型是 `int`；`"A"` 是包含 `'A'` 与终止字符 `\0` 的字符串常量。`\n` 表示换行，`\t` 是水平制表，`\\` 表示反斜杠。C17 中可 `#include <stdbool.h>` 使用 `bool`、`true`、`false`；即使不用它，条件表达式中仍以 0 为假、非 0 为真。

### 2.3 整数与浮点的边界

有符号整数的范围与无符号整数不同；`<limits.h>` 提供诸如 `INT_MAX` 的限值。`unsigned int` 不能保存负数。不要把一个负 `int` 和一个 `unsigned int` 混算后按普通数学心算：C 会执行类型转换，可能得到意外值。计算结果若超出有符号整数的可表示范围，还可能发生未定义行为。

浮点数 `double` 有舍入误差。`0.1 + 0.2` 这类计算不应默认与数学实数完全一样；对于测量值或数值算法，应按题目精度要求比较。`3.5` 通常是 `double` 常量，`3.5f` 是 `float` 常量。不要用浮点数保存必须精确计数的整数数量。

**动手题 2：** 解释 `010` 与 `0x10` 各是多少；再说出 `sizeof "AB"` 的值以及原因。

**课件对照：** 第 3 章幻灯片 2～9、18～36；第 1 章幻灯片 26。

## 3. 表达式与求值：考试爱考，也是实际错误来源

### 3.1 除法与转换发生的时机

两个 `int` 相除先得到整数，丢掉小数部分。转换要发生在除法**之前**，才能进行浮点除法：

```c
#include <stdio.h>

int main(void)
{
    int a = 5, b = 2;
    printf("%d\n", a / b);              /* 2 */
    printf("%.1f\n", (double)a / b);   /* 2.5 */
    printf("%.1f\n", (double)(a / b)); /* 2.0 */
    printf("%d\n", a % b);              /* 1 */
    return 0;
}
```

第三个输出先计算 `a / b == 2`，才把结果转为 `double`。强制转换不会倒回去改变已经算完的步骤。`%` 是整数求余，除法与求余的右操作数都不能为 0。

### 3.2 关系、逻辑与短路

数学里的 `0 <= x <= 100` **不能**原样翻译成 C。C 会先算 `0 <= x`，结果只能是 0 或 1，再拿这个结果比较 `<= 100`。要写 `0 <= x && x <= 100`。`=` 是赋值，`==` 才是相等比较。

```c
#include <stdio.h>

int main(void)
{
    int x = 75;
    int divisor = 0;

    if (0 <= x && x <= 100) {
        puts("范围正确");
    }
    if (divisor != 0 && x / divisor > 2) {
        puts("商大于 2");
    }
    return 0;
}
```

第二个判断不会除以 0，因为 `&&` 的左侧为假，右侧不再求值；`||` 在左侧为真时也会短路。这个规则既能防错，也可能让带副作用的右侧表达式根本不执行。

### 3.3 `++`、优先级和求值顺序

`++i` 先增加，再产生新的值；`i++` 先产生旧值，再增加。例如 `int i=3; int a=i++;` 之后 `a=3, i=4`。复杂表达式里不要反复修改同一变量来“考验优先级”；实际代码拆成多行更可靠。

优先级先记真正影响读题的顺序：括号 → 一元运算 → `* / %` → `+ -` → 关系/相等 → `&&` → `||` → `?:` → 赋值 → 逗号。**优先级说明怎样结合，不等于各部分都按这个顺序求值。** 不确定时加括号并写中间变量。

**动手题 3：** 分别计算 `5*4/2` 与 `5/2*4`；解释 `int a=0,b=1; if (a=b)` 为什么会进入 `if`。

### 3.4 把已有的条件与循环知识迁移过来

`if/else`、`for`、`while`、`do...while` 在你学过的 C++ 基础用法中大体相同。迁移时重点检查**条件表达式**和**循环边界**。`switch` 的 `case` 标签必须是整型常量表达式；执行到一个 `case` 后，若没有 `break`，会继续执行后续语句，这叫贯穿。它有时是有意设计，但成绩分档等题目通常需要 `break`。

```c
#include <stdio.h>

int main(void)
{
    int score = 100;
    switch (score / 10) {
    case 10: /* 100 分也归 A */
    case 9: puts("A"); break;
    case 8: puts("B"); break;
    case 7: puts("C"); break;
    case 6: puts("D"); break;
    default: puts("E"); break;
    }

    int total = 0;
    for (int i = 1; i <= 5; ++i) {
        total += i;
    }
    printf("1 到 5 的和=%d\n", total);
    return 0;
}
```

输入是否在 `0..100` 范围，最好在进入 `switch` 前检查；上例只展示分档规则。循环读题时写出不变量或至少列出第一轮、最后一轮与退出时的 `i`。`break` 跳出当前循环或 `switch`；`continue` 跳过本轮剩余部分，开始下一轮。课程的流程图和伪代码可帮助检查这些路径，但最终仍要把每条边界落实到 C 表达式上。

### 3.5 位运算、条件运算和逗号表达式

位运算作用于整数的位，常用于标志位：`&` 按位与、`|` 按位或、`^` 按位异或、`~` 按位取反、`<<` 左移、`>>` 右移。不要把单个 `&`、`|` 与会短路的 `&&`、`||` 混淆。做位运算尽量用无符号类型，并确认移位数非负且小于类型位数；对负数或超范围值移位不要想当然按数学乘除解释。

```c
#include <stdio.h>

int main(void)
{
    unsigned int flags = 0u;
    flags |= 1u << 2;                   /* 设置第 2 位 */
    printf("%u\n", (flags >> 2) & 1u); /* 输出 1 */
    flags &= ~(1u << 2);                /* 清除第 2 位 */
    printf("%u\n", flags);             /* 输出 0 */
    return 0;
}
```

`condition ? a : b` 根据条件只求值其中一个分支，例如 `max = x > y ? x : y;`。逗号**运算符** `x=1, y=2` 会先算左式、再算右式，整个表达式的值是右式的值；函数调用 `f(x,y)` 中分隔参数的逗号并不是这个运算符，别用它推断参数求值先后。实际业务代码很少需要把多个副作用挤进一个表达式，读课件计算题时理解即可。

**动手题 3B：** `unsigned int x=5u, y=3u;` 时 `x & y`、`x | y`、`x ^ y` 分别是多少？

**课件对照：** 第 3 章幻灯片 37～60。课件有一些复杂复合赋值例子；把它们当作阅读练习，实际程序优先拆开。

## 4. 声明、作用域、存储期与初始化

**C++ → C：** 你知道局部变量、全局变量，但 C 没有构造函数和析构函数自动管理对象。先确认一个对象**在哪里声明、何时开始存在、何时失效**。

```c
#include <stdio.h>

void visit(void)
{
    int ordinary = 0;        /* 每次调用重新产生并初始化 */
    static int persistent = 0; /* 整个程序期间存在 */
    ordinary += 1;
    persistent += 1;
    printf("%d %d\n", ordinary, persistent);
}

int main(void)
{
    visit(); /* 1 1 */
    visit(); /* 1 2 */
    return 0;
}
```

函数内的普通局部变量有自动存储期，函数调用结束就不再存在。`static` 局部变量一直保留；上例第二次调用时 `persistent` 没有重新变成 0。函数外定义的对象通常整个程序期间存在。`extern` 用于跨源文件声明外部对象或函数，`static` 放在文件作用域能把名字限制在当前源文件；第 11 节再做多文件例子。

未初始化的普通局部变量不要读取。例如 `int score; printf("%d", score);` 没有可靠含义。定义时初始化：`int score=0;`，或保证在每条使用路径前赋值。`int a[3]={0};` 会把数组元素初始化为 0。

**C 中值得额外学习的初始化工具：** 指定初始化器可写 `int a[5] = {[3] = 7};`，其余元素为 0；结构体可以写 `.member = value`。复合字面量能临时构造对象，如 `(struct Point){.x=1,.y=2}`。这两种写法先理解用途，等用到结构体时再练。

`typedef` 给已有类型起别名，不定义新结构的字段，例如 `typedef unsigned long Count;`。`const` 限制通过某个左值修改对象，第 5 节会把它放到指针声明中看；它不保证对象在任何途径下都不会改变。`volatile` 告诉实现访问可能有常规代码无法预测的效果，常见于特定底层接口；它**不是**线程同步工具。初学阶段不用把每个变量都加上这些修饰词，先理解它们的约束。

**动手题 4：** 若函数返回 `&local`，其中 `local` 是函数内部普通局部 `int`，返回后为什么不能解引用？

**课件对照：** 第 3 章幻灯片 32、36涉及定义和初始化；这里对作用域、存储期与链接的解释属于额外补充。

## 5. 指针与数组：从“会看地址”到“能保证地址有效”

### 5.1 指针到底保存什么

`int x=7; int *p=&x;` 中，`p` 保存 `x` 的地址；`*p` 访问该地址上的 `int`。`&` 取地址，`*` 在表达式中解引用。下面的例子把值从 7 改成 9：

```c
#include <stdio.h>

int main(void)
{
    int x = 7;
    int *p = &x;
    *p = 9;
    printf("x=%d, *p=%d\n", x, *p);
    return 0;
}
```

合法解引用要求指针指向一个仍存在、类型合适、位置有效的对象。`NULL` 表示不指向可用对象；`*NULL` 错误。`free` 之后的旧指针也是无效地址来源。指针不是“只要内存中碰巧有数字就能访问”。

`const int *p` 表示不能**通过 p** 修改所指的整数；`int *const p` 表示 p 自己不能改指向。`void *` 是通用对象指针，`malloc` 返回它；使用前要知道实际对象类型和大小。

### 5.2 数组在何时变成指针

```c
#include <stdio.h>

int sum_array(const int *items, size_t count)
{
    int sum = 0;
    for (size_t i = 0; i < count; ++i) {
        sum += items[i];
    }
    return sum;
}

int main(void)
{
    int values[] = {2, 4, 6};
    size_t count = sizeof values / sizeof values[0];
    printf("sum=%d\n", sum_array(values, count));
    return 0;
}
```

`values` 本身是三个整数构成的**数组**；传给函数时，它在此表达式中转换成首元素地址。函数里的 `items` 是指针，不携带原数组长度，所以还需要 `count`。`int a[3]` 的有效下标是 0、1、2；`a[3]` 已越界。指针加 1 会前进**一个所指元素的大小**，不是固定前进 1 字节；只在同一数组允许的范围内做指针运算。可以形成“一过末尾”指针用于比较，但不能解引用它。

二维数组也不是“指向指针的指针”。例如 `int grid[2][3];` 是两行、每行三个整数的连续数组；传给函数时必须让函数知道每行的列数，例如形参 `int grid[][3]`。把它强行当作 `int **` 通常是类型错误。

### 5.3 再认三种容易混淆的声明

`int **pp` 是指向 `int *` 对象的指针，可在函数内改变调用者保存的 `int *`；它**不是**二维整数数组的通用替身。`int *rows[3]` 是含三个指针元素的数组，每个指针可以各自指向一行；`int (*row)[3]` 才是指向“含三个 `int` 的数组”的指针。读声明时先找变量名，再看它紧邻的 `[]` 或 `*`，括号会改变结合方式。

```c
#include <stdio.h>

void point_to(int **target, int *value)
{
    *target = value;
}

int main(void)
{
    int number = 42;
    int *p = NULL;
    point_to(&p, &number);
    printf("%d\n", *p);
    return 0;
}
```

这里 `&p` 的类型是 `int **`；函数修改的是调用者的指针变量 `p`。若只是改 `number`，形参用 `int *` 就够了，不必无端增加指针层数。

**动手题 5：** 在函数里写 `sizeof items / sizeof items[0]`，其中 `items` 是 `const int *` 参数，为什么算不出调用者数组长度？

**课件对照：** 第 2 章幻灯片 31～36介绍数组、函数；第 1 章幻灯片 18只是将指针列为后续章节。本节绝大多数指针内容是补充主线。

## 6. 函数、输出参数与回调

**C++ → C：** C++ 的 `void swap(int& a,int& b)` 在 C 中常写 `void swap_int(int *a,int *b)`；调用时 `swap_int(&x,&y)`。函数形参仍按值传递，只是传递的值可以是地址。

```c
#include <stdio.h>

void swap_int(int *a, int *b)
{
    int temporary = *a;
    *a = *b;
    *b = temporary;
}

int main(void)
{
    int x = 3, y = 8;
    swap_int(&x, &y);
    printf("x=%d y=%d\n", x, y); /* x=8 y=3 */
    return 0;
}
```

这个函数约定调用者提供两个有效的 `int` 地址。真实公共接口若允许 `NULL`，应先检查；若不允许，就要在说明中写清楚，而不是让使用者猜。

### 一个函数怎样同时返回“结果”和“是否成功”

C 没有异常。可以让返回值表达状态，用指针参数填写结果。例如安全地做整数除法：

```c
#include <stdio.h>

/* 成功返回 0，除数为 0 或 out 为空则返回 -1。 */
int divide_int(int numerator, int denominator, int *out)
{
    if (denominator == 0 || out == NULL) {
        return -1;
    }
    /* 除了 0，还要避免 INT_MIN / -1；见下文。 */
    *out = numerator / denominator;
    return 0;
}

int main(void)
{
    int answer;
    if (divide_int(9, 2, &answer) == 0) {
        printf("商=%d\n", answer);
    }
    return 0;
}
```

上例刻意暴露一个更深的边界：在二补码、且 `INT_MIN / -1` 超出 `int` 范围的实现上，不能执行这个除法。要做完整通用接口，可包含 `<limits.h>` 并加 `if (numerator == INT_MIN && denominator == -1) return -1;`。这也是学习 C 的方法：先说出接口约束，再逐一找边界，而不是只看正常样例。

C 没有函数重载和默认参数，通常把用途写进函数名，例如 `parse_int` 与 `parse_double`。函数声明放在使用前，定义提供函数体；以后多文件时声明放进头文件。

### 函数指针：把一段行为传给函数

```c
#include <stdio.h>

int square(int x) { return x * x; }

int apply(int value, int (*transform)(int))
{
    return transform(value);
}

int main(void)
{
    printf("%d\n", apply(4, square)); /* 16 */
    return 0;
}
```

`int (*transform)(int)` 读作“指向函数的指针；该函数收一个 `int`，返回一个 `int`”。回调、排序比较函数、底层接口里经常出现它。以后遇到 `qsort` 时再复习一遍。

**动手题 6：** 把 `divide_int` 改到能拒绝 `INT_MIN / -1`，并分别测试正常、除数为 0、溢出三种情况。

**课件对照：** 第 2 章幻灯片 32～36讲函数声明、定义、调用和形参与实参；输出参数与回调是补充。

## 7. C 字符串与文本：没有 `std::string` 后要自己管容量

### 7.1 长度、容量与结束标记

**C++ → C：** `std::string s="Ada"` 可增长并记住长度；C 的 `char s[10]="Ada"` 只有**固定 10 字节容量**，当前字符串长度是 3，第四个位置存 `\0`。`strlen(s)` 找当前长度，不包括 `\0`；`sizeof s` 在数组仍是数组的作用域里给出容量 10。若只是 `char *p=s;`，`sizeof p` 是指针大小。

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    char name[10] = "Ada";
    printf("长度=%zu 容量=%zu\n", strlen(name), sizeof name);

    if (strcmp(name, "Ada") == 0) {
        puts("内容相同");
    }
    return 0;
}
```

为什么不用 `name == "Ada"`？因为数组在比较表达式中转换为地址，`==` 比较地址而不是每个字符。`strcmp` 返回 0 才表示内容相同。`strlen`、`strcmp` 等要求参数指向**有效且以 `\0` 结尾**的字符串；`char s[4]="abcd";` 虽能保存四个字符，但没有空间放终止符，不能拿它当普通 C 字符串交给这些函数。

字符串字面量应通过 `const char *message="hello";` 引用；不要修改其字符。需要修改时创建自己的 `char message[]="hello";` 数组。

### 7.2 安全读取与写入

```c
#include <stdio.h>
#include <string.h>

int main(void)
{
    char input[32];
    if (fgets(input, sizeof input, stdin) == NULL) {
        fputs("没有读到输入\n", stderr);
        return 1;
    }

    size_t length = strlen(input);
    if (length > 0 && input[length - 1] == '\n') {
        input[length - 1] = '\0';
    }

    char output[48];
    int needed = snprintf(output, sizeof output, "你好，%s", input);
    if (needed < 0 || (size_t)needed >= sizeof output) {
        fputs("输出内容过长\n", stderr);
        return 1;
    }
    puts(output);
    return 0;
}
```

`fgets` 最多读入“容量减 1”个字符，并留下 `\0`。上例若读不到换行，输入可能过长，也可能刚好到达文件末尾；严谨的程序应根据任务决定继续读剩余内容还是报错。`snprintf` 的第二个参数是目标容量；返回值可检测是否被截断。`strcpy` 不知道目标容量，只有确定空间足够时才可用。

`memcpy` 按字节复制内存，源和目标区域**不能重叠**；可能重叠时用 `memmove`。这两者处理的是字节，并不会替你寻找或补上字符串的 `\0`。

### 7.3 编码：字节不等于汉字

在 UTF-8 文本中，一个汉字常由多个字节表示，`strlen` 返回的是**字节数**，不是人在屏幕上看到的字数。`char name[10]` 说的是 10 个 C 字节，不能保证能装下 9 个汉字。需要可靠处理 Unicode 字符时，要明确文件/终端编码，并选择合适的多字节或宽字符库；不要凭 `char` 数组下标切割汉字。

**动手题 7：** `char word[6]="China";` 中 `strlen(word)` 与 `sizeof word` 分别是多少？如果把 `word[5]` 改成 `'!'`，还能安全地调用 `strlen(word)` 吗？

**课件对照：** 第 3 章幻灯片 26～30有字符、字符串和结束标记；第 1 章幻灯片 26列出字符串库；容量、截断与编码属于补充。

## 8. 动态内存与所有权：C 里最重要的习惯变化

### 8.1 `malloc` 的完整使用链

**C++ → C：** C++ 的 `std::vector<int> numbers(5)` 会安排内存并在生命周期结束时自动清理；C 的 `malloc` 只负责申请一段**字节空间**。你要自己决定容量、初始化元素、处理失败和调用 `free`。

```c
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t count = 5;
    int *numbers = malloc(count * sizeof *numbers);
    if (numbers == NULL) {
        fputs("分配失败\n", stderr);
        return 1;
    }

    for (size_t i = 0; i < count; ++i) {
        numbers[i] = (int)(i + 1); /* 先写，再读 */
    }
    printf("第三个数=%d\n", numbers[2]);

    free(numbers);
    numbers = NULL;
    return 0;
}
```

`count * sizeof *numbers` 是“元素个数 × 一个元素字节数”。`malloc` 得到的内容未初始化。C 中 `malloc` 返回的 `void *` 可直接赋给对象指针，通常不加类型强制转换；你的 `main.cpp` 是 C++ 文件，不能直接按 C 规则检查这一行。`5 * int` 错在把类型名当成数值；要写 `5 * sizeof(int)`。

这个顺序要变成习惯：**算大小 → 申请 → 检查 → 初始化 → 使用 → 释放**。`free` 后不能再使用旧地址。`free(NULL)` 是安全的，但不意味着可以重复释放同一块实际内存。`malloc/free` 与 C++ 的 `new/delete` 不能交叉配对。

### 8.2 容量变化和失败处理

`calloc(count, sizeof *numbers)` 分配空间并把字节清零；`malloc` 不清零。`realloc` 可改变之前动态分配的块，成功时可能搬到新地址，失败时旧块仍有效：

```c
/* 片段：numbers 已由 malloc/calloc/realloc 获得，new_count > 0；
   调用前已确认乘法不溢出。 */
int *resized = realloc(numbers, new_count * sizeof *numbers);
if (resized == NULL) {
    /* numbers 仍指向旧块；按需要继续使用或 free。 */
} else {
    numbers = resized;
}
```

不要直接 `numbers = realloc(numbers, ...)`，否则失败后可能丢失旧地址。若长度来自用户、文件或网络，乘法前检查 `new_count <= SIZE_MAX / sizeof *numbers`；`SIZE_MAX` 来自 `<stdint.h>`。长大容量时还要检查翻倍本身是否溢出。

### 8.3 所有权：谁负责 `free`

一个返回指针的函数要说清它返回的是：**新分配、调用者负责释放**的地址，还是**借用、不得释放**的地址。C 没有析构函数强制替你执行清理。函数中途失败时，已经成功打开的文件和申请的内存也要处理。

例如 `make_copy` 如果在内部 `malloc`，接口约定就应明确“成功返回的地址由调用者 `free`”；若返回的是自己内部静态数组的地址，调用者不能释放，也不能随意假定能永久保留某次读取内容。

**动手题 8：** 用 `malloc` 申请 `n` 个 `int`，输入每个元素并求和；列出输入失败、分配失败、正常结束时各该做哪些清理。

**课件对照：** 第 1 章幻灯片 26列出分配函数名称，幻灯片 18将动态存储管理列为后续内容；前三章并未系统讲上述所有权规则。

## 9. 结构体、枚举、联合与数据布局

### 9.1 用 `struct` 和函数代替类的最小用法

**C++ → C：** 你可能写 `student.birthday()`；C 常写 `birthday(&student)`。`struct` 负责组织字段，函数负责行为。

```c
#include <stdio.h>

typedef struct {
    char name[32];
    int age;
} Student;

void birthday(Student *student)
{
    student->age += 1; /* 等价于 (*student).age += 1 */
}

int main(void)
{
    Student student = {.name = "Lin", .age = 20};
    birthday(&student);
    printf("%s: %d\n", student.name, student.age);
    return 0;
}
```

这里的 `typedef` 让我们直接写 `Student`；`.name` 和 `.age` 是指定初始化器。结构体可以按值复制。如果成员是数组，数组内容会随结构体一起复制；如果成员是指针，复制的只是地址，两个结构体可能指向同一块数据，清理责任要另行设计。

### 9.2 `enum`、`union` 与填充

`enum` 为有限状态取名，常用于返回错误码：

```c
typedef enum {
    STATUS_OK,
    STATUS_BAD_INPUT,
    STATUS_NO_MEMORY
} Status;
```

C 的枚举不等同于 C++ 的 `enum class`：枚举常量不形成那样的独立作用域。`union` 的成员共用一块存储，适合确切知道当前有效成员的底层表示；不要把它当成“同时拥有所有字段的 struct”。

结构体为了让成员在适当地址上访问，成员间或末尾可能有**填充字节**。所以 `sizeof(struct)` 可能大于各成员 `sizeof` 之和。不要把结构体的原始内存直接写成可移植的文件格式；字段大小、对齐和字节序在其他平台上可能不同。位字段可描述紧凑字段，但其布局细节同样不宜作为跨平台二进制格式的唯一依据。

**动手题 9：** 把 `Student` 增加一个 `int score` 字段，写 `is_passed(const Student *)` 判断成绩是否至少为 60；说明函数为什么接收 `const Student *`。

**课件对照：** 第 3 章幻灯片 13、18只列出 `enum/struct/union` 类型名；第 1 章幻灯片 18将结构和联合列为后续章节。

## 10. 标准输入输出与文件

### 10.1 `printf`、`scanf` 与三个标准流

**C++ → C：** `cout` 对应向 `stdout` 输出，`cerr` 对应向 `stderr` 报错，`cin` 对应从 `stdin` 输入。C 函数用**格式字符串**描述数据类型，格式与实际参数必须匹配。

- `printf` 的 `%d` 对应 `int`、`%u` 对应 `unsigned int`、`%zu` 对应 `size_t`、`%c` 对应字符、`%s` 对应有效 C 字符串、`%f` 对应 `double`。
- `scanf("%d", &number)` 需要地址，因为要修改 `number`；读 `float` 用 `%f`，读 `double` 用 `%lf`。不要把 `printf` 与 `scanf` 的浮点格式规则混为一谈。
- `scanf` 返回成功读入的项目数。输入一个整数时检查是否等于 1，再使用变量。

```c
#include <stdio.h>

int main(void)
{
    int score;
    if (scanf("%d", &score) != 1) {
        fputs("请输入整数\n", stderr);
        return 1;
    }
    printf("成绩=%d\n", score);
    return 0;
}
```

`fputs("错误\n", stderr)` 只是把文字写到标准错误流，`%d` 在这里不会格式化；要带变量则 `fprintf(stderr, "成绩=%d\n", score)`。`fputs` 不自动换行，要自己写 `\n`。正常输出与错误输出分开，方便终端分别重定向。

### 10.2 更可靠地解析一行整数

`scanf` 对简单练习很方便；若要清楚区分“没输入、不是整数、超范围、后面有垃圾字符”，可以先 `fgets` 读一整行，再用 `strtol`：

```c
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char line[64];
    if (fgets(line, sizeof line, stdin) == NULL) {
        fputs("没有读到一行\n", stderr);
        return 1;
    }

    errno = 0;
    char *end;
    long parsed = strtol(line, &end, 10);
    if (end == line || errno == ERANGE ||
        parsed < INT_MIN || parsed > INT_MAX ||
        (*end != '\n' && *end != '\0')) {
        fputs("不是有效范围内的整数\n", stderr);
        return 1;
    }

    int value = (int)parsed;
    printf("读到 %d\n", value);
    return 0;
}
```

`strtol` 的第 3 个参数 10 表示按十进制解析；`end` 指向停止的位置。`end == line` 意味着一个数字都没读到；`ERANGE` 表示超出 `long` 范围；最后还要检查能否装进 `int`。此示例只接受数字后直接换行或行尾；如果输入行长于缓冲区，还要继续读完剩余内容，不能把截断前缀当完整输入。这段方法适合后来写较稳妥的程序；前三章上机入门题可以先用带返回值检查的 `scanf`。

### 10.3 文件：打开、操作、关闭

```c
#include <stdio.h>

int main(void)
{
    FILE *file = fopen("result.txt", "w");
    if (file == NULL) {
        perror("无法打开 result.txt");
        return 1;
    }

    if (fputs("42\n", file) == EOF) {
        perror("写入失败");
        fclose(file);
        return 1;
    }
    if (fclose(file) == EOF) {
        perror("关闭文件失败");
        return 1;
    }
    return 0;
}
```

`"w"` 会创建文件，已有文件则覆盖原内容，因此运行前确认 `result.txt` 不需要保留。读文本常用 `fgets`，写文本可用 `fputs`/`fprintf`；读写二进制数据可用 `fread`/`fwrite`，并检查实际项目数。文件定位使用 `fseek`/`ftell` 时也要检查返回值，不能假设所有输入流都支持随机定位。`feof` 表示尝试读取后观察到文件末尾，`ferror` 判断流错误；不要用 `while (!feof(file))` 作为“下一次读取一定成功”的保证。

**动手题 10：** 把上例改成先写入三行整数，再重新打开文件，逐行读回并求和；说明任何 `fopen` 失败时应如何处理。

**课件对照：** 第 1 章幻灯片 27～32与第 2 章幻灯片 24～30涉及基础输入输出；第 1 章幻灯片 18将文件列为后续章节。本节解析和文件错误处理属于主线补充。

## 11. 预处理、头文件与分文件编译

### 11.1 用三个文件理解“声明”和“定义”

**C++ → C：** 你已见过 `#include`；C 中也需要让每个源文件在编译时知道函数接口。把声明放头文件，定义放源文件，可以让使用者只依赖接口。

文件 `math_util.h`：

```c
#ifndef MATH_UTIL_H
#define MATH_UTIL_H

int add(int a, int b);

#endif
```

文件 `math_util.c`：

```c
#include "math_util.h"

int add(int a, int b)
{
    return a + b;
}
```

文件 `main.c`：

```c
#include <stdio.h>
#include "math_util.h"

int main(void)
{
    printf("%d\n", add(2, 3));
    return 0;
}
```

同目录运行 `gcc -std=c17 -Wall -Wextra -Wpedantic main.c math_util.c -o app.exe`。只给出 `main.c`，编译器能看到 `add` 的声明，但链接器找不到函数定义；在头文件里放普通全局变量定义，又容易让多个 `.c` 各定义一次，产生重复定义。头文件保护宏阻止同一翻译单元反复展开同一个头文件。文件作用域的 `static` 函数仅供当前 `.c` 使用；跨文件共享可见声明时才考虑 `extern`，初学先避免随意共享可变全局变量。

### 11.2 宏只做适合文本替换的事

```c
#define PI 3.1415926
#define SQUARE(x) ((x) * (x))
```

`#define` 是预处理指令，行末不加分号。`SQUARE(2+3)` 因为括号得到 25；`SQUARE(i++)` 会让 `i++` 出现两次，所以不该这样用。多数计算优先普通函数；需要短小头文件函数时，再学习 `static inline`。`#if/#ifdef` 可按条件选择编译内容，头文件保护就是最常见的入门例子。

### 11.3 命令行参数与程序结束状态

需要从命令行传入文件名时，入口可写 `int main(int argc, char *argv[])`。`argc` 是参数个数，`argv[0]` 通常是程序名，首次自定义参数是 `argv[1]`；使用前先检查 `argc`。`argv` 可看作字符串指针数组的参数形式，正好复习第 5 节。

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "用法：%s 文件名\n", argv[0]);
        return 1;
    }
    printf("要处理的文件：%s\n", argv[1]);
    return 0;
}
```

`return 0` 通常表示成功，非零表示失败，方便终端脚本判断。若代码有多个资源需要清理，先用清晰的早返回和清理函数；复杂 C 函数也可能用一个集中清理出口，但每个资源是否已成功取得必须明确。

**动手题 11：** 把第 6 节的 `swap_int` 拆成 `.h`、`.c` 和调用它的 `main.c`；再故意遗漏实现文件，判断错误发生在哪个阶段。

**课件对照：** 第 1 章幻灯片 24、49～50和第 2 章幻灯片 3、20～23涉及包含、编译与符号常量；多文件接口是补充。

## 12. 标准库：按任务找工具，不死背函数表

**C++ → C：** 没有 `std::vector`、`std::string` 的成员函数，C 的通用工具多在若干标准头文件中。学习时要同时记**参数约束、返回值、失败方式**，不能只记函数名。

- `<stdio.h>`：控制台、文件和流。先熟 `printf`、`fprintf`、`fgets`、`fputs`、`fopen`、`fclose`。
- `<stdlib.h>`：内存分配；`strtol` 等数值转换；`qsort`、`bsearch`；程序退出与伪随机数。
- `<string.h>`：字符串和字节操作。区分 `strlen`、`strcmp`、`memcpy`、`memmove`；它们不会自动知道缓冲区容量。
- `<ctype.h>`：字符分类与大小写转换。参数必须是 `EOF` 或可表示为 `unsigned char` 的值；传可能为负的普通 `char` 前可转成 `(unsigned char)`。
- `<math.h>`：平方根、三角函数、绝对值等。`abs` 是整数绝对值，在 `<stdlib.h>`；`fabs` 是浮点绝对值，在 `<math.h>`。
- `<time.h>`：时间获取与表示。时间值和日历时间的转换要检查接口结果。
- `<limits.h>`、`<float.h>`、`<stdint.h>`、`<inttypes.h>`、`<stddef.h>`：类型大小、范围与可移植整数格式。
- `<errno.h>`、`<assert.h>`：库调用错误与程序内部断言。不要把 `assert` 当成处理用户错误输入的唯一手段。

例如排序，C 的 `qsort` 需要比较函数指针：

```c
#include <stdio.h>
#include <stdlib.h>

int compare_int(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(void)
{
    int values[] = {9, -2, 5, 1};
    size_t count = sizeof values / sizeof values[0];
    qsort(values, count, sizeof values[0], compare_int);

    for (size_t i = 0; i < count; ++i) {
        printf("%d%s", values[i], i + 1 == count ? "\n" : " ");
    }
    return 0;
}
```

比较函数返回负、零、正分别代表“小于、等于、大于”。不要简单返回 `a-b`：当两个数相差很大时，有符号整数减法可能溢出。比较函数必须把传入的 `void *` 按真实元素类型解释。

**动手题 12：** 把排序改成从大到小；再说明比较函数里直接返回 `b-a` 为什么不够稳妥。

## 13. 正确性：未定义行为、移植性和调试

C 程序“能运行一次”并不等于写对。语言中的**未定义行为**表示标准不给结果作保证；**实现定义行为**由实现选择并应说明；**未指定行为**允许标准规定范围内的不同选择。刚开始不用背所有条款，但要知道这三者不是同一个词。

常见问题按类别记：

- **边界：** `int a[3]; a[3]` 越界；字符串没有 `\0` 却调用 `strlen`；格式字符串的类型和参数不匹配。
- **生命周期：** 返回局部变量地址；`free` 后继续用旧指针；同一块动态内存重复释放。
- **数值：** 除以 0；有符号整数溢出；不检查数值转换的范围；把浮点近似当精确相等。
- **表达式：** 在一个表达式里反复修改同一对象而没有理清求值关系；把优先级误认为完整求值顺序。
- **平台：** 假设所有机器 `int` 同宽、同字节序，或把结构体原始字节当作可移植文件格式。

运行前用警告选项编译，先修第一条诊断；运行时用调试器看变量、调用栈和崩溃位置。环境支持时，再用地址和未定义行为检测器，例如 GCC/Clang 的 `-g -fsanitize=address,undefined`；不同 Windows 工具链的支持程度要以实际安装为准。

**自测方法：** 每写一个函数，列出空输入、最小/最大值、失败返回、数组边界。遇到错误先找最小能重现的输入；修改后再跑这个反例。不要只拿课件上的一个正常样例检查。

**动手题 13：** 找出以下代码的问题，并改成不越界的版本：`int a[3]={1,2,3}; for(int i=0;i<=3;++i) printf("%d",a[i]);`。

## 14. 进阶工具箱：先知道边界，之后按方向深入

前 13 节足以完成大多数入门 C 程序。为了让知识版图完整，下面把标准 C 和常见系统开发中还会遇到的内容列出来；它们**不应挤掉当前的课程上机练习**。

### 14.1 语言侧

- **指定初始化器、复合字面量：** `struct Point p={.x=1,.y=2};`，`(struct Point){.x=1,.y=2}`。适合字段多、想明确对应关系的场景。
- **灵活数组成员：** 结构体末尾可以有柔性数组，整块分配时要自行算空间。先掌握普通动态数组，再学它。
- **变长数组 VLA：** 长度可在运行时决定，但编译器与项目可能有不同支持策略；大型或不受信任长度优先考虑受控动态分配。
- **`restrict`：** 对某些指针访问关系作承诺，让实现有优化空间；承诺不真实会破坏程序正确性。
- **`_Generic`、`_Static_assert`、对齐：** 分别用于按类型选择表达式、编译期检查、满足对象的对齐要求。
- **变参函数 `<stdarg.h>`：** 可实现类似 `printf` 的可变参数接口，但失去部分静态类型检查，必须有清晰的参数契约。
- **对象表示与有效类型：** 字节布局、填充、别名规则影响底层代码；不要随意把任意地址强转成另一种对象指针并解引用。

### 14.2 库、并发与平台侧

- **多字节/宽字符与区域设置：** `<wchar.h>`、`<uchar.h>`、`<locale.h>`；需要处理真实文本时按编码方案学习。
- **复数与浮点环境：** `<complex.h>`、`<fenv.h>`；数值计算方向再深入。
- **线程与原子：** `<threads.h>`、`<stdatomic.h>` 涉及同步和内存顺序。`volatile` 不等于线程锁，也不解决数据竞争。
- **信号与非局部跳转：** `<signal.h>`、`<setjmp.h>` 有特殊控制流与限制，优先建立普通返回值和清理路径习惯。
- **嵌入式与系统接口：** ABI、C/C++ 混合链接、硬件寄存器、操作系统 API、字节序与二进制协议都超出纯标准 C 的入门范围，要按目标平台文档学习。
- **标准版本：** 本教程示例选 C17；C23 已发布，但新语法及库功能要逐项核对编译器支持和课程要求，不能把某编译器扩展当作各环境都有的标准。

**本节的验收：** 看到这些词时知道它们属于哪类问题、什么时候该查标准或平台文档；当前不要求一次背完或亲手实现所有特性。

## 15. 综合项目：从零写一个成绩记录程序

这个项目把前面零散的知识合成一次完整练习。目标：每行输入一个**不带空格的姓名**和 `0..100` 的成绩；空行结束输入；程序显示人数、平均分，并把记录保存到 `grades.txt`。姓名最多 31 字节；用英文短姓名练习最省去终端编码因素。运行程序会覆盖同目录已有的 `grades.txt`，先确认文件可覆盖。

先自己拆解任务，再看实现：

1. 一条记录用 `struct Student`；多条记录用“指针 + 已用数量 + 已分配容量”。
2. 先读整行，再解析姓名和整数；非法行不加入数组。
3. 数组满时 `realloc`，用临时指针接结果，失败时旧数组仍可释放。
4. 写文件后检查写入与关闭结果；所有正常和错误路径都要考虑 `free`。

将下面内容保存为 `grades.c`，用 `gcc -std=c17 -Wall -Wextra -Wpedantic grades.c -o grades.exe` 编译，再运行 `./grades.exe`：

```c
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    int score;
} Student;

/* 1=读到完整行；0=输入结束；-1=读取错误；-2=行过长。 */
int read_line(char *buffer, size_t capacity)
{
    if (fgets(buffer, (int)capacity, stdin) == NULL) {
        return feof(stdin) ? 0 : -1;
    }
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
        return 1;
    }
    if (feof(stdin)) {
        return 1; /* 文件末尾的最后一行可以没有换行符 */
    }
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* 丢弃本行剩余部分，避免下次把它当新记录。 */
    }
    return -2;
}

int parse_student(const char *line, Student *out)
{
    char name[32];
    int offset = 0;
    if (sscanf(line, "%31s%n", name, &offset) != 1) {
        return 0;
    }

    errno = 0;
    char *end;
    long score = strtol(line + offset, &end, 10);
    if (end == line + offset || errno == ERANGE ||
        score < 0 || score > 100) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0') {
        return 0;
    }

    strcpy(out->name, name); /* %31s 已保证 name 放得进 32 字节。 */
    out->score = (int)score;
    return 1;
}

int save_students(const char *path, const Student *items, size_t count)
{
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("打开成绩文件失败");
        return 0;
    }
    int ok = 1;
    for (size_t i = 0; i < count; ++i) {
        if (fprintf(file, "%s %d\n", items[i].name, items[i].score) < 0) {
            perror("写入成绩文件失败");
            ok = 0;
            break;
        }
    }
    if (fclose(file) == EOF) {
        perror("关闭成绩文件失败");
        ok = 0;
    }
    return ok;
}

int main(void)
{
    Student *students = NULL;
    size_t count = 0;
    size_t capacity = 0;
    char line[128];

    puts("每行输入：姓名 成绩；直接回车结束。");
    for (;;) {
        int state = read_line(line, sizeof line);
        if (state == 0 || (state == 1 && line[0] == '\0')) {
            break;
        }
        if (state == -1) {
            fputs("读取输入失败\n", stderr);
            free(students);
            return 1;
        }
        if (state == -2) {
            fputs("这一行太长，请重新输入\n", stderr);
            continue;
        }

        Student next;
        if (!parse_student(line, &next)) {
            fputs("格式应为：英文姓名 0..100 的整数\n", stderr);
            continue;
        }

        if (count == capacity) {
            if (capacity > SIZE_MAX / 2) {
                fputs("记录数量过大\n", stderr);
                free(students);
                return 1;
            }
            size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
            if (new_capacity > SIZE_MAX / sizeof *students) {
                fputs("内存大小计算溢出\n", stderr);
                free(students);
                return 1;
            }
            Student *resized = realloc(students,
                                        new_capacity * sizeof *students);
            if (resized == NULL) {
                fputs("分配内存失败\n", stderr);
                free(students);
                return 1;
            }
            students = resized;
            capacity = new_capacity;
        }
        students[count++] = next;
    }

    double sum = 0.0;
    for (size_t i = 0; i < count; ++i) {
        sum += students[i].score;
    }
    printf("人数=%zu\n", count);
    if (count > 0) {
        printf("平均分=%.2f\n", sum / count);
    }

    int saved = save_students("grades.txt", students, count);
    free(students);
    if (!saved) {
        return 1;
    }
    puts("已保存到 grades.txt");
    return 0;
}
```

用这组输入检查正常路径：

```text
Lin 90
Wang 80

```

应输出 `人数=2` 和 `平均分=85.00`，`grades.txt` 中有两行记录。再试 `Li 101`、`Li abc`、超长姓名和超过 127 字节的一整行，观察它们被拒绝后程序能否继续处理下一行。**阅读代码时重点追踪 `count`、`capacity`、`students` 三者在每次添加后是什么状态。** 这个项目以文本文件练习保存；若要读取旧记录，可在第 10 节文件读取练习完成后扩展一个 `load_students` 函数。

## 16. 动手题答案与推导

建议先独立完成，再对照答案。看到和答案不同的输出时，先检查文件是否保存、是否用 `.c` 编译、输入是否一致，然后读第一条编译诊断。

### 第 1～4 题：程序、数值、表达式、生命周期

1. `printf("第一行\n第二行\n");` 即可输出两行。`Printf` 与 `printf` 是两个不同标识符；没有相应声明时编译器会诊断，若仍尝试链接也找不到对应函数。不要只看错误列表最后一条。
2. `010` 是八进制的 8，`0x10` 是十六进制的 16。`"AB"` 里还有终止用的 `\0`，所以 `sizeof "AB"` 为 3 个 `char`，即 3 个 C 字节。它和 `strlen("AB") == 2` 不矛盾。
3. `5*4/2` 按同级运算从左结合，结果 10；`5/2*4` 先做整数除法得到 2，再乘 4 得 8。`a=b` 是把 1 赋给 `a`，赋值表达式的值也是 1，所以进入 `if`。若想比较应写 `a == b`；打开编译警告也有助于发现这类笔误。
   **3B.** `5u` 的低三位是 `101`，`3u` 是 `011`。按位与得 `001=1`，按位或得 `111=7`，按位异或得 `110=6`。这是按位计算，不是逻辑真假的 `&&`、`||`。
4. `local` 的自动存储期在函数返回时结束。即使指针数值看起来像旧地址，也不能再解引用那个已经失效的对象。可以让调用者传入有效地址作为输出参数，或按值返回 `int`。

### 第 5～9 题：数组、函数、内存与结构体

5. `items` 是指针，`sizeof items` 给的是指针本身的字节数，`sizeof items[0]` 给一个 `int` 的字节数。两者的商与调用者传进来的数组长度没有必然关系。把长度作为单独的 `size_t count` 参数传入。
6. 在第 6 节函数前加 `#include <limits.h>`，把判断改为 `if (out == NULL || denominator == 0 || (numerator == INT_MIN && denominator == -1)) return -1;`。只有通过判断才执行 `/`。分别调用 `divide_int(9,2,&answer)`、`divide_int(9,0,&answer)`、`divide_int(INT_MIN,-1,&answer)`，返回值依次应为 `0,-1,-1`；失败时不要假设 `answer` 得到了新值。
7. `strlen(word)` 是 5，`sizeof word` 是 6；末尾的 `word[5]` 存 `\0`。改成 `!` 后数组里不再有终止符，`strlen(word)` 可能继续越过数组寻找 `\0`，不能调用。恢复终止符，或扩容后再追加字符。
8. 示例核心代码如下。读取 `n` 失败时还没申请内存；申请失败时不能解引用 `items`；读取某个元素失败时释放已申请的整块数组；正常完成后也释放。这里用 `double` 累加演示资源清理，若题目要求精确整数求和，还需给累加选择合适范围并检查溢出。

```c
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t n;
    if (scanf("%zu", &n) != 1 || n == 0) return 1;
    if (n > SIZE_MAX / sizeof(int)) return 1;
    int *items = malloc(n * sizeof *items);
    if (items == NULL) return 1;

    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (scanf("%d", &items[i]) != 1) {
            free(items);
            return 1;
        }
        sum += items[i];
    }
    printf("和=%.0f\n", sum);
    free(items);
    return 0;
}
```

9. 在 `Student` 中增加 `int score;`。可写 `int is_passed(const Student *student) { return student != NULL && student->score >= 60; }`。`const Student *` 说明此函数只读记录，也可以避免误改字段；这里明确把 `NULL` 判为不及格。调用方先决定这种接口约定是否符合题意。

### 第 10～13 题：文件、链接、比较函数与边界

10. 写文件时用 `fprintf(file,"%d\n",value)` 三次，并检查每次返回值；`fclose` 成功后，再以 `"r"` 打开同一路径。读取时可以 `while (fscanf(file,"%d",&value) == 1) sum += value;`；读完若 `ferror(file)` 为真，应报读入错误，若有非数字内容也需按题意处理。任何 `fopen` 失败，都不能对空 `FILE *` 调用读取/写入/关闭函数；如果此前还有动态内存或其他文件，先清理它们，再返回失败状态。
11. 头文件写 `void swap_int(int *a, int *b);` 并加保护宏，实现写在 `swap_int.c`，`main.c` 包含头文件并调用 `swap_int(&x,&y)`。编译命令要同时包含 `main.c swap_int.c`。遗漏后者时，声明足以通过编译，但链接阶段缺少函数定义，通常报告 `undefined reference` 或 `unresolved external symbol`。
12. 降序比较可以返回 `(a < b) - (a > b)`。返回 `b-a` 可能发生有符号整数溢出，例如 `b=INT_MAX, a=-1`；排序只需要比较的符号，无需真的计算差值。
13. 三元素数组的有效下标为 0、1、2，所以条件应为 `i < 3`，也可以写 `size_t count = sizeof a / sizeof a[0]; for (size_t i=0; i<count; ++i) printf("%d",a[i]);`。原程序在 `i==3` 时读取 `a[3]` 越界，属于未定义行为；输出过一次“看似正常”不能证明代码正确。

## 17. 复习路线与课程对照

### 17.1 按能力过关，不靠看完页数

**第一关：基本题写对。** 独立用 `.c` 文件写成绩分档、区间判断、求和、最大值、阶乘、简单数组统计。每题先列输入范围，再测最小值、最大值、无效输入。对应第 1～4 节。课件第 2 章的条件、循环、函数与流程图可作题库，第 3 章的进制与表达式可作计算题；不要只读课件示例的预期输出。

**第二关：对象和数据写稳。** 能解释数组与指针的区别、字符串为什么要多留 `\0`、函数为何需要长度参数、`malloc` 的失败和释放路径，并从空白文件写出第 5～9 节的练习。做第 15 节项目前至少要能自己写出一个动态数组和一个 `struct` 函数。

**第三关：综合题能定位故障。** 完成第 10～13 节，亲手制造一次编译错误、一次链接错误、一次输入错误，分别说明问题在哪一阶段；再独立重写第 15 节程序，并增加读取旧文件或按成绩排序的功能。

### 17.2 考前检查清单

- **手算类：** 进制、整数除法、类型转换、运算符结合、`++` 的结果、短路、数组下标。先写每一步的类型和值，避免凭直觉读式子。
- **补代码类：** 头文件、`main` 形式、函数声明、输入格式与地址、循环边界、`switch` 的 `break`、字符串空间、动态内存的检查与 `free`。
- **上机类：** 文件名用 `.c`；先保存再编译；修第一条编译错误；至少试正常、边界、错误三组输入；运行结果和解释一致才提交。
- **叙述类：** 能说清“数组作为实参为何丢长度”“`sizeof` 与 `strlen` 区别”“`fputs(...,stderr)` 与 `printf` 区别”“`malloc` 为何检查且释放”“声明与定义、编译与链接的区别”。

你给出的第 1 章课件提到平时作业、实验报告和期末闭卷上机考试（幻灯片 21）。这说明**独立编写、编译、调试完整程序**比只背术语更关键；具体分值和考核安排仍以老师当期通知为准。若老师后来发了更多章节，应把其例题加进各关的练习，但主线知识仍按本教程检查。

### 17.3 哪些来自前三章课件，哪些是补充

**前三章课件有明确内容：** C 程序结构与开发流程、基础输入输出、变量与表达式、进制与数据表示、选择/循环、数组与函数的入门示例、宏、流程图。本文对应第 1～3 节、第 5～6 节的基础部分，以及第 10～11 节的基础部分。标注“课件对照”的位置只是**PPTX 页码依据**，不冒充课堂逐字讲授记录。

**本教程为 C++ 转 C 补充的主线：** 存储期与链接、指针有效性、二维数组形参、C 字符串容量与编码、动态内存和所有权、结构体实际使用、健壮输入与文件错误处理、多文件接口、库函数契约、未定义行为和调试，以及第 15 节综合项目。课件第 1 章虽列出指针、结构体、文件等后续主题，这不表示前三章已经系统讲过它们。

**深入时再学：** 第 14 节语言和平台专题。它们属于 C 的完整知识版图，当前考核若未涉及，可以先达到“知道何时查资料”的程度。

### 17.4 资料与下一步

标准条文适合**核对规则**，不适合替代这份入门教程：可查 [WG14 官方项目页](https://www.open-std.org/jtc1/sc22/wg14/www/projects.html) 和 [C11 公开委员会草案 N1570](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)。本教程的练习基线是 C17；遇到版本差异，应以指定标准版本、编译器诊断及课程要求为准。阅读顺序就是：先通读第 1～13 节并做题，再独立完成第 15 节，最后用本节清单查漏；第 14 节按需要回查。
