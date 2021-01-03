#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#define rep(i, n) for (register int(i) = 0; (i) < (n); (i)++)

int enc()
{
    FILE *ofp;
    if ((ofp = fopen(ORGDATA, "r")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", ORGDATA);
        exit(1);
    }
    FILE *efp;
    if ((efp = fopen(ENCDATA, "w")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", ENCDATA);
        exit(1);
    }
    setvbuf(ofp, NULL, _IOFBF, 1048576);
    setvbuf(efp, NULL, _IOFBF, 1048576);
    char in[200000], out[200000];
    fread(in, sizeof(char), 200000, ofp);
    rep(i, 99999)
    {
        // 偶数bit 0 -> A, 1 -> G
        out[(i << 1)] = (in[(i << 1)] == '1' ? 'G' : 'A');
        // 奇数bit 0 -> T, 1 -> C
        out[(i << 1) + 1] = (in[(i << 1) + 1] == '1' ? 'C' : 'T');
    }
    char c1 = in[199998], c2 = in[199999];
    int pt = (c1 - '0') * 2 + (c2 - '0');
    // (c1, c2) = (0,0) -> A
    // (c1, c2) = (0,1) -> T
    // (c1, c2) = (1,0) -> G
    // (c1, c2) = (1,1) -> C
    out[199998] = (pt == 0 ? 'A' : pt == 1 ? 'T' : pt == 2 ? 'G' : 'C');
    out[199999] = '\n';
    fwrite(out, sizeof(char), 200000, efp);
    fclose(ofp), fclose(efp);
    return (0);
}

int main()
{
    enc();
    return (0);
}
