#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

  unsigned char c1, c2, res;
  unsigned int bt, bcnt;
  #define BITNUMBER 7
  #define ARRAYNUMBER 5556
  #define LASTNUMBER 10 //ORGDATA_LEN/2-(ARRAYNUMBER-1)*(SR_SIZE-BITNUMBER);

  for(int i=0; i<ARRAYNUMBER; i++){
    bcnt = i;
    for(int j=BITNUMBER-1; j>0; j--){
      bt = (int)bcnt / pow(2,2*j+1); c1 = bt -'0';
      bcnt -= pow(2,2*j+1) * bt;
      bt = (int)bcnt / pow(2,2*j); c2 = bt -'0';
      bcnt -= pow(2,2*j) * bt;

      switch( ( (c1 & 0x1) << 7) >> 6 | ( c2 & 0x1) ){
      case 0:
        res = BASE_A;
        break;
      case 1:
        res = BASE_C;
        break;
      case 2:
        res = BASE_G;
        break;
      case 3:
        res = BASE_T;
        break;
      }
      fputc(res, efp);
    }
    bt = (int)bcnt / 2; c1 = bt -'0';
    bcnt -= 2 * bt;
    bt = (int)bcnt % 2; c2 = bt -'0';

    switch( ( (c1 & 0x1) << 7) >> 6 | ( c2 & 0x1) ){
    case 0:
      res = BASE_A;
      break;
    case 1:
      res = BASE_C;
      break;
    case 2:
      res = BASE_G;
      break;
    case 3:
      res = BASE_T;
      break;
    }
    fputc(res, efp);

    for(int k=0; i==ARRAYNUMBER-1 ? k<LASTNUMBER : k<SR_SIZE-BITNUMBER ; k++){
      c1 = getc(ofp);
      c2 = getc(ofp);

      switch( ( (c1 & 0x1) << 7) >> 6 | ( c2 & 0x1) ){
      case 0:
        res = BASE_A;
        break;
      case 1:
        res = BASE_C;
        break;
      case 2:
        res = BASE_G;
        break;
      case 3:
        res = BASE_T;
        break;
      }
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
