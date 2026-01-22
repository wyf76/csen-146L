#include <stdio.h>
#include <stdlib.h>

static int ceil_div(int a, int b) {
    return (a + b - 1) / b;
}

int main(int argc, char **argv) {
    //RTTs (ms)
    const int RTT0 = 3;
    const int RTT1 = 20;
    const int RTT2 = 26;
    const int RTTHTTP = 47;

    const int DNS = RTT0 + RTT1 + RTT2; // 49 ms
    
    //a 
    int Ta = DNS + 2 * RTTHTTP;

    //b
    int Tb = DNS + 7 * (2 * RTTHTTP);

    printf("(a) One object: %d ms\n", Ta);
    printf("(b) Base + 6 objects, non-persistent, no parallel: %d ms\n", Tb);

    //c
    int n = 6;
    if (argc >= 2) {
        n = atoi(argv[1]);
        if (n <= 0) n = 1;
    }

    int rounds = ceil_div(6, n);

    int T_nonpersistent = DNS + 2 * RTTHTTP * (1 + rounds);

    int T_persistent = DNS + RTTHTTP * (2 + rounds);

    printf("\n(c) For n=%d parallel TCP connections:\n", n);
    printf("Persistent: %d ms\n", T_persistent);
    printf("Non-persistent: %d ms\n", T_nonpersistent);


    return 0;
}
