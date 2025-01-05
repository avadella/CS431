#include <stdio.h>
#include <unistd.h>

int main() {

    // `getconf PAGESIZE` should produce the same result

    long sz = sysconf(_SC_PAGESIZE);
    printf("Page Size: %ld B\n", sz);

    return 0;
}