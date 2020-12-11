#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"

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

  unsigned char c, res1, res2, res;
  for(int i=0; i<ORGDATA_LEN; i++){
    c = getc(ofp);
    switch(c & 0x1){
    case 0:
      res1 = 'A'; res2='T';// 0 -> AT
      break;
    case 1:
      res1 = 'G'; res2='C';// 1 -> GC    
      break;
    }
    fputc(res1, efp); fputc(res2, efp);
    
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
