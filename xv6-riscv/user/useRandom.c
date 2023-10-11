
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/random.h"

int main() {
    rand_init(17);
    for (int i = 0; i < 100; ++i) {
        printf("%d\n", scaled_random(1,1000));
    }
    exit(0);
}
