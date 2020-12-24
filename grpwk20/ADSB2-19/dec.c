#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)
#define max(a,b) a>b?a:b
#define N 4
#define M 14

int dp[M+1][M+1];

inline static int edis(unsigned char *S,unsigned char *T){
  bool chk=true;
  rep(i,M){
    if(S[i]!=T[i]){
      chk=false;
      break;
    }
  }
  if(chk) return 0;
  int *p=&dp[1][1];
  for(int i=1;i<=M;i++){
    for(int j=1;j<=M;j++){
      int min=dp[i-1][j-1]+(S[i-1]==T[j-1]?0:1);
      if(dp[i-1][j]+1<min) min=dp[i-1][j]+1;
      if(dp[i][j-1]+1<min) min=dp[i][j-1]+1;
      *p++=min;
    }
    p++;
  }
  p--,p--;
  return *p;
}

inline static int edis2(unsigned char *S,unsigned char *T,int m){
  bool chk=true;
  rep(i,m){
    if(S[i]!=T[i]){
      chk=false;
      break;
    }
  }
  if(chk) return 0;
  int *p=&dp[1][1];
  for(int i=1;i<=m;i++){
    for(int j=1;j<=m;j++){
      int min=dp[i-1][j-1]+(S[i-1]==T[j-1]?0:1);
      if(dp[i-1][j]+1<min) min=dp[i-1][j]+1;
      if(dp[i][j-1]+1<min) min=dp[i][j-1]+1;
      *p++=min;
    }
    p++;
  }
  p--,p--;
  return *p;
}

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
  rep(i,M+1) dp[i][0]=i;
  rep(i,M) dp[0][i+1]=i+1;

  unsigned char c, res;
  int cur;
  unsigned char S[N][202000];
  unsigned char ans[200000];
  rep(i,N){
    cur=0;
    while((c = getc(sfp)) != '\n'){
      switch(c){
      case BASE_A:
        res = 0;
        break;
      case BASE_T:
        res = 1;      
        break;
      case BASE_G:
        res = 2;      
        break;
      case BASE_C:
        res = 3;      
        break;
      default:
        res = 0;
        break;
      }
      S[i][cur]=res;
      cur++; 
    }
    for(int j=cur;j<202000;j++) S[i][j]=4;
  }
  int pt[N]={};
  unsigned char now[N];
  int zero,one,wrong;
  unsigned char base[M+3];
  rep(i,199998){
    rep(j,N) now[j]=S[j][pt[j]];
    /*if(i>=199900){
      rep(j,N) printf("%d ",now[j]);
      printf("\n");
    }*/
    zero=0,one=0;
    rep(j,N){
      if(now[j]==4) continue;
      else if(now[j]&2 && !((i&1)^(now[j]&1))) one++;
      else if(!(now[j]&2) && !((i&1)^(now[j]&1))) zero++;
    }
    //if(zero>=one) fputc('0',dfp),wrong=1;
    //else fputc('1',dfp),wrong=0;
    if(zero>=one) ans[i]='0',wrong=1;
    else ans[i]='1',wrong=0;
    if(zero==N || one==N){
      rep(j,N) pt[j]++;
      continue;
    }
    int fail[N]={};
    int cut=0;
    rep(j,M+3){
      int n[5]={};
      rep(k,N){
        if(S[k][pt[k]+j]==4){
          n[4]++;
          continue;
        }
        if(fail[k]) continue;
        if(wrong){
          if(now[k]>=2){
            fail[k]=1;
            continue;
          }
          if(((S[k][pt[k]+j])&1)^((i+j)&1)){
            fail[k]=1;
            continue;
          }
          n[S[k][pt[k]+j]]++;
        }
        else{
          if(now[k]<2){
            fail[k]=1;
            continue;
          }
          if(((S[k][pt[k]+j])&1)^((i+j)&1)){
            fail[k]=1;
            continue;
          }
          n[S[k][pt[k]+j]]++;
        }
      }
      int plc=0,maxi=0;
      rep(k,5) if(n[k]>maxi) plc=k,maxi=n[k];
      if(maxi==0 && j>3){
        cut=j;
        break;
      }
      base[j]=plc;
    }
    rep(j,N){
      if(now[j]==4) continue;
      unsigned char* p=base;
      if(wrong){
        if(now[j]<2 && !((now[j]&1)^(i&1))) continue;
        unsigned char *p2=&S[j][pt[j]+1];
        int diff[6];
        if(cut){
          if((diff[0]=edis2(p,p2,cut-3))==0){
            pt[j]++;
            continue;
          }
          if((diff[3]=edis2(++p,--p2,cut-3))==0){
            pt[j]--;
            continue;
          }
          p2++;
          if((diff[1]=edis2(--p,++p2,cut-3))==0){
            pt[j]+=2;
            continue;
          }
          if((diff[2]=edis2(p,++p2,cut-3))==0){
            pt[j]+=3;
            continue;
          }
          p2=&S[j][pt[j]];
          p++;
          if((diff[4]=edis2(++p,p2,cut-3))==0){
            pt[j]-=2;
            continue;
          }
          if((diff[5]=edis2(++p,p2,cut-3))==0){
            pt[j]=-3;
            continue;
          }
        }
        else{
          if((diff[0]=edis(p,p2))==0){
            pt[j]++;
            continue;
          }
          if((diff[3]=edis(++p,--p2))==0){
            pt[j]--;
            continue;
          }
          p--,p2++;
          if((diff[1]=edis(p,++p2))==0){
            pt[j]+=2;
            continue;
          }
          if((diff[2]=edis(p,++p2))==0){
            pt[j]+=3;
            continue;
          }
          p2=&S[j][pt[j]];
          p++;
          if((diff[4]=edis(++p,p2))==0){
            pt[j]-=2;
            continue;
          }
          if((diff[5]=edis(++p,p2))==0){
            pt[j]=-3;
            continue;
          }
        }
        int mini=2*M,use=0;
        rep(k,6) if(mini>diff[k]) mini=diff[k],use=k;
        if(use==0) pt[j]++;
        else if(use==3) pt[j]--;
        else if(use==1) pt[j]+=2;
        else if(use==4) pt[j]-=2;
        else if(use==2) pt[j]+=3;
        else pt[j]-=3;
      }
      else{
        if(now[j]>=2 && !((now[j]&1)^(i&1))) continue;
        unsigned char *p2=&S[j][pt[j]+1];
        int diff[6];
        if(cut){
          if((diff[0]=edis2(p,p2,cut-3))==0){
            pt[j]++;
            continue;
          }
          if((diff[3]=edis2(++p,--p2,cut-3))==0){
            pt[j]--;
            continue;
          }
          p2++;
          if((diff[1]=edis2(--p,++p2,cut-3))==0){
            pt[j]+=2;
            continue;
          }
          if((diff[2]=edis2(p,++p2,cut-3))==0){
            pt[j]+=3;
            continue;
          }
          p2=&S[j][pt[j]];
          p++;
          if((diff[4]=edis2(++p,p2,cut-3))==0){
            pt[j]-=2;
            continue;
          }
          if((diff[5]=edis2(++p,p2,cut-3))==0){
            pt[j]=-3;
            continue;
          }
        }
        else{
          if((diff[0]=edis(p,p2))==0){
            pt[j]++;
            continue;
          }
          if((diff[3]=edis(++p,--p2))==0){
            pt[j]--;
            continue;
          }
          p--,p2++;
          if((diff[1]=edis(p,++p2))==0){
            pt[j]+=2;
            continue;
          }
          if((diff[2]=edis(p,++p2))==0){
            pt[j]+=3;
            continue;
          }
          p2=&S[j][pt[j]];
          p++;
          if((diff[4]=edis(++p,p2))==0){
            pt[j]-=2;
            continue;
          }
          if((diff[5]=edis(++p,p2))==0){
            pt[j]=-3;
            continue;
          }
        }
        int mini=2*M,use=0;
        rep(k,6) if(mini>diff[k]) mini=diff[k],use=k;
        if(use==0) pt[j]++;
        else if(use==3) pt[j]--;
        else if(use==1) pt[j]+=2;
        else if(use==4) pt[j]-=2;
        else if(use==2) pt[j]+=3;
        else pt[j]-=3;
      }
    }
    rep(j,N){
      pt[j]++;
      if(pt[j]>=201900) pt[j]=200500;
    } 
  }
  int last[4]={};
  rep(i,N) if(S[i][pt[i]]!=4) last[S[i][pt[i]]]++;
  int ls=0,maxi=0;
  rep(i,4) if(maxi<last[i]) maxi=last[i],ls=i;
  char c2=ls&1;
  ls/=2;
  char c1=ls;
  unsigned char* lp=ans;
  rep(i,199998) fputc(*lp++,dfp);
  fputc('0'+c1,dfp);
  fputc('0'+c2,dfp);
  /*rep(i,N){
    printf("%d ",pt[i]);
  }*/
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
