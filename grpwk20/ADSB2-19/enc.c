#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
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

  unsigned char res;
  char S[200000];
  rep(i,200000) S[i]=getc(ofp);
  rep(i,200000) fputc(i%2?S[i]=='1'?'C':'T':S[i]=='1'?'G':'A',efp); // C T G A
  printf("\n");
  res = '\n';
  fputc(res, efp);
  fclose(ofp);
  fclose(efp);
  return(0);
}

int main(){
  enc();
  return(0);
}
