#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")

int dec(){
  FILE *sfp;
  if((sfp = fopen(SEQDATA, "r")) ==NULL){
    fprintf(stderr, "cannot open %s\n", SEQDATA);
    exit(1);
  }

  FILE *dfp;
  if((dfp = fopen(DECDATA, "w")) ==NULL){
    fprintf(stderr, "cannot open %s\n", DECDATA);
    exit(1);
  }

  unsigned char c, res;
  unsigned int ires1, ires2, dcnt=0, ind=0, j=0;
  #define BITNUMBER 7
  #define SCANCOUNT 10
  #define ARRAYNUMBER 5556 //100000/(25-7)
  #define LASTNUMBER 10 //ORGDATA_LEN/2-(ARRAYNUMBER-1)*(SR_SIZE-BITNUMBER);

  srand(time(NULL));

  struct bitstring{
    int cnt;
    char base[BITNUMBER][SR_SIZE-BITNUMBER];
  };
  struct scantime{
    struct bitstring bs[ARRAYNUMBER];
  };
  struct scantime st[SCANCOUNT];

  for(int i=0; i<SCANCOUNT; i++){
    for(int j=0; j<ARRAYNUMBER; j++){
      st[i].bs[j].cnt = 0;
      for(int k=0; k<BITNUMBER; k++){
        for(int l=0; l<SR_SIZE-BITNUMBER; l++){
          st[i].bs[j].base[k][l] = 'Z';
        }
      }
    }
  }

  for (int i=0; i<SCANCOUNT; i++) {
    int num=0;
    while((c = getc(sfp)) != '\n'){
      if(dcnt < BITNUMBER){
        switch(c){
        case BASE_A:
          ires1 = 0; ires2 = 0;
          break;
        case BASE_C:
          ires1 = 0; ires2 = 1;
          break;
        case BASE_G:
          ires1 = 1; ires2 = 0;
          break;
        case BASE_T:
          ires1 = 1; ires2 = 1;
          break;
        default:
          ires1 = 0; ires2 = 0;
          break;
        }
        ind += pow(2,(BITNUMBER-dcnt)*2-1)*ires1 + pow(2,(BITNUMBER-dcnt)*2-2)*ires2;
        if(dcnt == BITNUMBER-1){
          if(ind >= pow(2,BITNUMBER*2-1)) ind -= (int)pow(2,BITNUMBER*2-1);
          if(ind >= ARRAYNUMBER) ind -= (int)pow(2,BITNUMBER*2-2);
          num++;
        }
      }else if(dcnt < SR_SIZE){
        st[i].bs[ind].base[st[i].bs[ind].cnt][j] = c; j++;
        if(dcnt == SR_SIZE-1){
          st[i].bs[ind].cnt++; dcnt = 0; ind = 0; j = 0;
          continue;
        }
      }
      dcnt++;
    }
  }

  int tmp[SCANCOUNT][2], dif=0;
  int acnt=0, ccnt=0, gcnt=0, tcnt=0;
  for(int i=0; i<SCANCOUNT; i++){
    tmp[i][0] = -1; tmp[i][1] = -1;
  }
  for(int n=0; n<ARRAYNUMBER; n++){
    for(int i=0; i<SCANCOUNT; i++){
      for(int j=0; j<st[i].bs[n].cnt; j++){
        for(int k=i+1; k<SCANCOUNT; k++){
          for(int l=0; l<st[(i+l)%SCANCOUNT].bs[n].cnt; l++){
            dif=0;
            for(int m=0; n==ARRAYNUMBER-1 ? m<LASTNUMBER : m<SR_SIZE-BITNUMBER; m++){
              if(tmp[i][0]==-1){
                if(st[i].bs[n].base[j][m]!=st[k].bs[n].base[l][m]){
                  dif++;
                }
              }else{
                if(st[tmp[i][0]].bs[n].base[tmp[i][1]][m]!=st[k].bs[n].base[l][m]){
                  dif++;
                }
              }
            }
            if(dif < 10){
              if(tmp[i][0]==-1){
                tmp[i][0]=i; tmp[i][1]=j;
                tmp[k][0]=k; tmp[k][1]=l;
              }else{
                tmp[k][0]=k; tmp[k][1]=l;
              }
              break;
            }
          }
        }
      }
    }

    int  mcnt=0;
    for(int i=0;i<SCANCOUNT;i++){
      if(tmp[i][0]==-1) mcnt++;
    }
    for(int i=0; n==ARRAYNUMBER-1 ? i<LASTNUMBER : i<SR_SIZE-BITNUMBER; i++){
      for(int j=0; j<SCANCOUNT; j++){
        if(tmp[j][0]!=-1){
          switch (st[tmp[j][0]].bs[n].base[tmp[j][1]][i]) {
          case BASE_A:
            acnt++;
            break;
          case BASE_C:
            ccnt++;
            break;
          case BASE_G:
            gcnt++;
            break;
          case BASE_T:
            tcnt++;
            break;
          default:
            acnt++;
            break;
          }
        }
      }
      if(mcnt<SCANCOUNT){
        if(acnt > ccnt && acnt > gcnt && acnt > tcnt){
          fputc('0', dfp); fputc('0', dfp);
        }else if(ccnt> gcnt && ccnt > tcnt && ccnt>acnt){
          fputc('0', dfp); fputc('1', dfp);
        }else if(gcnt > tcnt && gcnt>acnt && gcnt > ccnt){
          fputc('1', dfp); fputc('0', dfp);
        }else if(tcnt > acnt && tcnt > ccnt && tcnt > gcnt){
          fputc('1', dfp); fputc('1', dfp);
        }else if(acnt == ccnt){
          fputc('0', dfp); fputc('0', dfp);
        }else if(acnt == gcnt){
          fputc('1', dfp); fputc('0', dfp);
        }else if(acnt == tcnt){
          fputc('0', dfp); fputc('0', dfp);
        }else if(ccnt == gcnt){
          fputc('0', dfp); fputc('1', dfp);
        }else if(ccnt == tcnt){
          fputc('1', dfp); fputc('1', dfp);
        }else if(gcnt == tcnt){
          fputc('1', dfp); fputc('0', dfp);
        }else{
          fputc('0', dfp); fputc('0', dfp);
        }
      }else{
        for(int j=0; j<SCANCOUNT; j++){
          if(st[j].bs[n].cnt > 0){
            switch (st[j].bs[n].base[j][i]) {
            case BASE_A:
            fputc('0', dfp); fputc('0', dfp);
              break;
            case BASE_C:
            fputc('0', dfp); fputc('1', dfp);
              break;
            case BASE_G:
            fputc('1', dfp); fputc('0', dfp);
              break;
            case BASE_T:
            fputc('1', dfp); fputc('1', dfp);
              break;
            default:
            fputc('0', dfp); fputc('0', dfp);
              break;
            }
            break;
          }
          if(j == SCANCOUNT-1){
            int s1 = rand() % 2; fputc(s1-'0', dfp);
            int s2 = rand() % 2; fputc(s2-'0', dfp);
          }
        }
      }
      acnt = 0; ccnt = 0; gcnt = 0; tcnt = 0;
    }
    for(int j=0; j<SCANCOUNT; j++){
      tmp[j][0] = -1; tmp[j][1] = -1;
    }
  }
  res = '\n';
  fputc(res, dfp);

  fclose(sfp);
  fclose(dfp);
  return(0);
}

int main(){
  dec();
  return(0);
}
