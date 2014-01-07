#define NULL (0)

#include "printf.h"
#include "printf.c"

static void debug_putc(void *_p, char c)
{
    putchar(c);
}

int main()
{
    init_printf(NULL, debug_putc);

    printf("test\n");

    printf("qwe %d asd\n", 100);
    printf("qwe %5d asd\n", 100);
    printf("qwe %-5d asd\n", 100);

    printf("qwe %05d asd\n", 100);

    printf("qwe %s asd\n", "zxc");
    printf("qwe %20s asd\n", "zxc");
    printf("qwe %-20s asd\n", "zxc");

    printf("qwe %s asd\n", "zxc123");
    printf("qwe %20s asd\n", "zxc123");
    printf("qwe %-20s asd\n", "zxc123");

}
