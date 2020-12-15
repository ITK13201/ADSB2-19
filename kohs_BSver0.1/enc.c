#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"

unsigned char index[7] = "";

unsigned char conv_sub(int a){
  switch(a){
    case 0: return 'A';
    case 1: return 'C';
    case 2: return 'G';
    case 3: return 'T';
  }
  return 'A';
}

/* num2hexa: 数値をATGCの4進数の文字列に変換*/
void num2hexa(int num){
  int i = 4096; // 4^6
  int tmp = num; // 各桁の値
  int j=0; 
  for(j=0; j<7; j++){
    tmp = num / i;
    index[j] = conv_sub(tmp);
    num = num % i;
    i = i / 4;
  }
  return;
}

unsigned char num2ATGC(unsigned char c1, unsigned char c2){
  switch( ( (c1 & 0x1) << 7) >> 6 | ( c2 & 0x1) ){
    case 0: return 'A';
    case 1: return 'C';      
    case 2: return 'G';
    case 3: return 'T';
  }
  return 'A';
}

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

  unsigned char c1, c2, res;
  int cnt=0;
  for(int i=0; i<ORGDATA_LEN; i+=36){ // 1ブロックずつループを回す
    num2hexa(cnt); /*通し番号の計算*/
    cnt++;
    for(int j=0; j<7; j++){
      fputc(index[j], efp);
    }
    for(int j=0; j<36; j+=2){
      c1 = getc(ofp);
      if(c1 == '\n') break;
      c2 = getc(ofp);
      res = num2ATGC(c1, c2);
      fputc(res, efp);
    }
  }
    
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
