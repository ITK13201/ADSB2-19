#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)
#define max(a,b) a>b?a:b
#define N 5
#define M 13

inline static unsigned char edis(unsigned char *S,unsigned char *T){
  unsigned char dp[M+1][M+1];
  rep(i,M+1) dp[i][0]=i;
  rep(i,M) dp[0][i+1]=i+1;
  for(int i=1;i<=M;i++) for(int j=1;j<=M;j++){
    int min;
    min=dp[i-1][j-1]+(S[i-1]==T[j-1]?0:1);
    if(dp[i-1][j]+1<min) min=dp[i-1][j]+1;
    if(dp[i][j-1]+1<min) min=dp[i][j-1]+1;
    dp[i][j]=min;
  }
  return dp[M][M];
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

  unsigned char c, res;
  int cur;
  unsigned char S[N][202000];
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
  int pt[N];
  rep(i,N) pt[i]=0;
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
    if(zero>=one) fputc('0',dfp),wrong=1;
    else fputc('1',dfp),wrong=0;
    rep(j,M+3){
      int n[5]={};
      rep(k,N){
        if(S[k][pt[k]+j]==4) n[4]++;
        else if(wrong){
          if(now[k]>=2) continue;
          if(((S[k][pt[k]+j])&1)^((i+j)&1)) continue;
          n[S[k][pt[k]+j]]++;
        }
        else{
          if(now[k]<2) continue;
          if(((S[k][pt[k]+j])&1)^((i+j)&1)) continue;
          n[S[k][pt[k]+j]]++;
        }
      }
      int plc=0,maxi=0;
      rep(k,5) if(n[k]>maxi) plc=k,maxi=n[k];
      base[j]=plc;
    }
    unsigned char* p=base;
    rep(j,N){
      if(now[j]==4) continue;
      if(wrong){
        if(now[j]<2 && !((now[j]&1)^(i&1))) continue;
        unsigned char *p2=&S[j][pt[j]+1];
        unsigned char diff[6];
        diff[0]=edis(p,p2);
        diff[1]=edis(p,++p2);
        diff[2]=edis(p,++p2);
        p2=&S[j][pt[j]];
        diff[3]=edis(++p,p2);
        diff[4]=edis(++p,p2);
        diff[5]=edis(++p,p2);
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
        unsigned char diff[6];
        diff[0]=edis(p,p2);
        diff[1]=edis(p,++p2);
        diff[2]=edis(p,++p2);
        p2=&S[j][pt[j]];
        diff[3]=edis(++p,p2);
        diff[4]=edis(++p,p2);
        diff[5]=edis(++p,p2);
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
      if(pt[j]>=201900) pt[j]=201000;
    } 
  }
  int last[4]={};
  rep(i,N) if(S[i][pt[i]]!=4) last[S[i][pt[i]]]++;
  int ls=0,maxi=0;
  rep(i,4) if(maxi<last[i]) maxi=last[i],ls=i;
  char c2=ls&1;
  ls/=2;
  char c1=ls;
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
