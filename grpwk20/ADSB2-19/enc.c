#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)

int enc(){
  FILE *ofp;
  if((ofp = fopen(ORGDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ORGDATA);
    exit(1);
  }
  FILE *efp;
  if((efp = fopen(ENCDATA, "w")) ==NULL){
    fprintf(stderr, "cannot open %s\n", ENCDATA);
    exit(1);
  }
  rep(i,199998) fputc(getc(ofp)=='1'?i&1?'C':'G':i&1?'T':'A',efp);
  char c1=getc(ofp),c2=getc(ofp);
  int pt=(c1-'0')*2+(c2-'0');
  fputc(pt==0?'A':pt==1?'T':pt==2?'G':'C',efp);
  fputc('\n', efp);
  fclose(ofp),fclose(efp);
  return(0);
}

int main(){
  enc();
  return(0);
}
