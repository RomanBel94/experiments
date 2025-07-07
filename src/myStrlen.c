#include <stddef.h>
#include <stdio.h>

ptrdiff_t my_strlen(const char* str)
{
    const char* end = str;
    while (*end)
        ++end;

    return end - str;
}

int main()
{
    const char str[] = "Hello World";
    printf("%s\n", str);
    printf("%td\n", my_strlen(str));

    return 0;
}
