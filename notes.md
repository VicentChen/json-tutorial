# JSON-tutorial

标签（空格分隔）： 编译原理

---

##Tutorial 1 启程
 >`case 'n': return lept_parse_literal(c, v, "null", LEPT_NULL);`可以减少一些重复代码，不过可能有少许额外性能开销

开销原因有两个
1. `"null"`：存储于堆中，访问时需要额外的开销；首先是取字符串指针`p`，然后计算偏移`s`，最后再进行取值`*(p+s)`
2. 寄存器使用数量增加：可能需要将变量复制至寄存器，也可能首先需要将寄存器入栈

---

##Tutorial 2 解析数字
![JSON数字][1]

 - case/default 的位置只会是当有 fall-through 时才有影响。
```
switch (*c->json) {
    case 't':  return lept_parse_true(c, v);
    case 'f':  return lept_parse_false(c, v);
    case 'n':  return lept_parse_null(c, v);
    default:   return lept_parse_number(c, v);
    case '\0': return LEPT_PARSE_EXPECT_VALUE;
}
```

---

##Tutorial 3 解析字符串
 ![ASCII][2]
 - CRT - [C Runtime Library][3]
 
```
#ifdef _WINDOWS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

int main() {
#ifdef _WINDOWS
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
```

---

##Tutorial 4 解析Unicode
- Unicode
`codepoint = U+xxxx`
`codepoint = 0x10000 + (H − 0xD800) × 0x400 + (L − 0xDC00)`
- UTF-8
<table><tr><td>码点范围</td><td>码点位数</td><td>字节1</td><td>字节2</td><td>字节3</td><td>字节4</td></tr><tr><td>U+0000 ~ U+007F</td><td>7</td><td>0xxxxxxx</td><td></td><td></td><td></td></tr><tr><td>U+0080 ~ U+07FF</td><td>11</td><td>110xxxxx</td><td>10xxxxxx</td><td></td><td></td></tr><tr><td>U+0800 ~ U+FFFF</td><td>16</td><td>1110xxxx</td><td>10xxxxxx</td><td>10xxxxxx</td><td></td></tr><tr><td>U+10000 ~ U+10FFFF</td><td>21</td><td>11110xxx</td><td>10xxxxxx</td><td>10xxxxxx</td><td>10xxxxxx</td></tr></table>

---

##Tutorial 5 解析数组

##Tutorial 6 解析对象

##Tutorial 7 生成器

---


  [1]: http://www.zcamoure.com/zb_users/upload/2017/05/201705171495005882436399.png
  [2]: https://upload.wikimedia.org/wikipedia/commons/thumb/1/1b/ASCII-Table-wide.svg/1280px-ASCII-Table-wide.svg.png
  [3]: https://msdn.microsoft.com/zh-cn/library/x98tx3cf.aspx