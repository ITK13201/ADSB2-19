#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  char in[200001];
  char out[200000];
  fgets(in,200001,ofp);
  char *ip=in,*op=out;
  rep(i,99999){
    *op++=(*ip++=='1'?'G':'A');
    *op++=(*ip++=='1'?'C':'T');
  }
  int pt=(in[199998]-'0')*2+(in[199999]-'0');
  out[199998]=(pt==0?'A':pt==1?'T':pt==2?'G':'C');
  out[199999]='\n';
  fputs(out,efp);
  fclose(ofp),fclose(efp);
  return(0);
}

int main(){
  enc();
  return(0);
}
