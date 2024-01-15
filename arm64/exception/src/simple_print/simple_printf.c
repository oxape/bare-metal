#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define putchar putchar_

extern void putchar_(char ch);

void print_padding(int width, int num_chars, char pad_char) {
    for (int i = num_chars; i < width; i++) {
        putchar(pad_char);
    }
}

void print_integer(int value, int width) {
    char buffer[10]; // 足够存储32位整数的最大宽度
    int pos = 0;

    // 处理负数
    if (value < 0) {
        putchar('-');
        value = -value; // 转换为正数来打印
        width--; // 减去负号占用的宽度
    }

    // 处理0的特殊情况
    if (value == 0) {
        buffer[pos++] = '0';
    }

    // 将整数转换为字符串表示
    while (value > 0) {
        buffer[pos++] = (value % 10) + '0';
        value /= 10;
    }

    // 打印宽度空格填充
    print_padding(width, pos, ' ');

    // 逆序打印整数的每一位
    while (pos > 0) {
        putchar(buffer[--pos]);
    }
}

void print_string(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void print_hex(unsigned int value, int width) {
    int num_chars = 0;
    unsigned int n = value;
    if (value == 0) {
        num_chars = 1;
    } else {
        while (n > 0) {
            num_chars++;
            n /= 16;
        }
    }
    print_padding(width, num_chars, ' ');

    if (value == 0) {
        putchar('0');
    } else {
        char hex_chars[16] = "0123456789abcdef";
        char buffer[8]; // 一个32位的整数的十六进制表示不会超过8个字符
        int pos = 0;
        while (value > 0) {
            buffer[pos++] = hex_chars[value % 16];
            value /= 16;
        }
        while (pos > 0) {
            putchar(buffer[--pos]);
        }
    }
}

void print_pointer(void* ptr) {
    putchar('0');
    putchar('x');
    print_hex((unsigned int)((uintptr_t)ptr), 0);
}

// ... 其余的simple_printf和print_integer函数保持不变 ...

void simple_printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int width;

    while (*format != '\0') {
        if (*format == '%') {
            ++format;
            width = 0;
            while (*format >= '0' && *format <= '9') {
                width = width * 10 + (*format - '0');
                ++format;
            }

            if (*format == 'd') {
                int value = va_arg(args, int);
                print_integer(value, width);
            } else if (*format == 's') {
                const char* str = va_arg(args, const char*);
                print_string(str);
            } else if (*format == 'p') {
                void* ptr = va_arg(args, void*);
                print_pointer(ptr);
            } else if (*format == 'x') {
                unsigned int num = va_arg(args, unsigned int);
                print_hex(num, width);
            } else {
                putchar('%');
                if (width > 0) {
                    print_padding(width - 1, 1, ' ');
                }
                putchar(*format);
            }
        } else {
            putchar(*format);
        }
        ++format;
    }

    va_end(args);
}