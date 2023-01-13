# fget

**函数原型**

char *fgets(char *buf, int bufsize, FILE *stream);

**参数**

*buf：字符型指针， 指用来存储所得数据的地址

bufsize：整形数据，指明存储数据的大小

*stream：文件结构体指针，将要读取的文件流

**返回值**

1、成功，则返回第一个参数buf

2、在读字符时遇到end-of-file， 则eof指示器被设置，如果还没读入任何字符就遇到这种情况，则buf保持原来内容，返回NULL。

3、如果发生读入错误，error指示器被设置，返回NULL，buf的值可能会被改变

**功能**

stream文件流指针体指向文件内容地址的偏移原则

如果使用fgets()读取某个文件，第一次取bufsize为5， 而文件的第一行有10个字符（算上'\n'）,那么读取文件的指针会偏移至当前读取完的字符之后的位置。因此第二次使用fgets()读取文件时，则会继续读取其后的字符。

如果用fgets()读取文件时，bufsize大于该行的字符总数加2（多出来的两个， 一个保存文件本身的换行符'\n'，另一个保存字符串本身的结束标识'\0'），文件并不会继续读下去，仅仅只是这一行读完，随后文件指针自动偏移至下一行。

**例**

如果一个文件当前位置文本如下：

Love, I have

Since you can do it

如果用fgets(str1, 6, file1);

则执行后str1 = ”Love,“， 读取了6-1 = 5个字符

此时在执行fgets(str1, 20, file1);

则执行后str1 = " I have\n"

```c
    char * s_gets(char * st, int n)
  {
    char * ret_val;
    char * find;

    ret_val = fgets(st, n, stdin);
    if(ret_val)
    {
      find = strchr(st, '\n');
      if(find)
        *find = '\0';
      else
        while (getchar() != '\n')
          continue;
    }
    return ret_val;
  }
```

# (void *)0

(void *) 表示空指针

# 指针

.比*运算符优先级高、

指向结构的指针：struct Student * stu1;

# 结构体

函数参数：传递结构的地址

函数声明：double sum(const struct funds *);   //参数是一个指针

调用：struct funds stan;   sum(&stan); 

函数体：double sum(const struct funds * money){..//用->}

**函数参数：传递结构**

函数声明：double sum(struct funds money);

调用：struct funds stan; sum(stan);

函数体：double sum(struct funds money){...//用.}

⚠️传递结构地址时，函数体实际上处理的是原值； 传递结构时，函数体实际操作的是一个副本。这保护了原数据，缺点也很明显：传递结构浪费时间和存储空间。

C语言允许把一个结构赋值给另一个结构，但是数组不能这样做。函数不仅能把结构本身作为参数传递，还能把结构作为返回值返回。

# const

由于被调函数中的某些操作可能会意外影响原来结构中的数据。ANSI C新增的const限定符解决了这个问题。

如果被改了，编译器会捕捉这个错误。

# fseek

# 内存分配

# segmentation fault

# 存储类别

**对象**：一个内存块，可以用来存一个或多个值。「面向对象编程中的对象是指类对象」

**标识符「identifier」**：是一个名称

`int entity = 3;` 其中entity就是一个标识符：可以用来指定特定对象的内容

> 变量名不是指定对象的唯一途径：还可以用表达式来指定对象。如下：`pt`是一个标识符，它指定了一个存储地址的对象。但是表达式`*pt`也指定了一个对象，且和`entity`指向的对象一样，但`*pt`不是标识符。

```c
int *pt = &entity;
int ranks[10];
```

**左值**：指定对象的表达式。因此`entity`既是左值也是标识符，`*pt`既是左值也是表达式。按照这个思路表达式`*(rank + 2*entity)`是一个左值。

**可修改的左值「modified lvalue」**：如果可以使用左值改变对象的值，那么该左值是一个可修改的左值。再举个例子↘︎

```c
const char *pc = "Behold a string literal!";
```

程序根据该声明把相应的字符串字面量存储在内存中，内含这些字符值的字面量就是一个对象。由于字符值的字面量的每个字符都能被单独访问到，所以每个字符也是一个对象。`pc`指定了存储着字符串地址的对象，`pc`是一个**modified lvalue**。`*pc`指定了存储着`‘B’`字符的数据对象，所以`*pc`是个左值，但，是一个**不可修改的左值**。因为`const`只能保证被`pc`指向的字符串内容不可修改，但无法保证`pc`不指向别的字符串。

## 作用域

**块作用域**「block scope」：花括号括起来的代码区域称为一块。

函数作用域

**函数原型作用域**「function prototype scope」：用于函数原型中的**函数形参**

文件作用域：全局变量

**翻译单元**「translation unit」：C预处理实际上是用包含的头文件内容替换#include 指令。所以编译器把源代码文件和所有的头文件都看成是一个包含信息的单独文件。这个文件被称为翻译文件「translaiton unit」。描述一个具有文件作用域的变量时，它的实际可见范围是整个翻译单元。每个翻译单元均对应一个源代码文件和它所包含的文件。

## 链接

**无链接**：具有块作用域、函数作用域、函数原型作用域的变量都是无链接变量

**内部链接**：只能在一个翻译单元中使用

**外部链接**：可以在多文件程序中使用

> 非正式术语：由于懒，一些程序猿把“内部链接的文件作用域”简称为“文件作用域”，把“外部链接的文件作用域”简称为“全局作用域”或“程序作用域”

如何知道一个文件作用域是内部链接还是外部链接？可以查看外部定义中是否使用了存储类别说明符**static**

```c
int gaints = 5;        /*文件作用域，外部链接*/
static inr dodgers = 3;/*文件作用域，内部链接*/
int main(){
    ...
}
...
```

该文件和同一程序的其他文件都可以使用变量`gaints`。而变量`dodgers`属于文件私有，该文件中的任意函数都可以使用它。

## 静态变量「static variable」

静态的意思是该变量在内存中原地不动，并不是指它的值不变。

# 函数指针

```c
int Func(int x);
int (*p)(int x);
p = Func;
```

赋值时函数`Func`不带括号，也不带参数。由于函数名`Func`代表函数的首地址，因此赋值之后，指针变量`p`就指向函数`Func()`代码的首地址。

来一段代码使用

```c
#include <stdio.h>
int Max(int, int);
int main(void)
{
    int(*p)(int, int);
    int a,b,c;
    p = Max;
    printf("Please enter a and b");
    scanf("%d%d", &a, &b);
    c = (*p)(a,b);
    printf("a = %d\nb=%d\nmax=%d\n", a,b,c); 
    return 0;
}

int Max(int x, int y)
{
    int z;
    if(x>y)
        z = x;
    else
        z = y;
    return z;
}
```

# union共同体

## 1.point：

 就是成员共用一个内存空间。

## 2.定义

和结构体相似：

```c
union name{
    member1;
    member2;
};
//for example

union data{
    int a;
    char b;
    double;
};
```

## 3.应用

学生教师信息表



| Name    | Num | Sex | Profession | Score/Course |
| ------- | --- | --- | ---------- | ------------ |
| David   | 30  | m   | s          | 89.6         |
| Lisa    | 24  | f   | t          | English      |
| Jackson | 1   | m   | t          | Math         |
| Bailey  | 22  | f   | s          | 100          |

除了最后一个变量不同，其他都一样。因此可定义结构体如下

```c
#include <stdio.h>
#include <stdlib.h>

#define TOTAL 4
struct {
    char name[20];
    int num;
    char sex;
    char profession;
    union{
        float score;
        char course[20];
    }sc;    
}bodys[TOTAl];

int main(){
    int i;
    //输入人员信息
    for(i=0; i<TOATL; i++){
    printf("Input info");
    scanf("%s %d %c %c", body[i].name, &(body[i].num), &(body[i].sex), &(body[i].profession));
    if(bodys[i].profession == 's')
        scanf("%f", &(body[i].sc.score));
    else
        scanf("s", body[i].sc.course);
    fflush(stdin);
    }
    //输出成员信息
}
```
