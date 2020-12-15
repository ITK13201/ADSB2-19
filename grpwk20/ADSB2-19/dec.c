#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#include <stdbool.h>
#pragma GCC optimize("Ofast")
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)
#define max(a,b) a>b?a:b

int edis(unsigned char *S,int n,unsigned char *T,int m){
  int dp[n+1][m+1];
  rep(i,n+1) rep(j,m+1) dp[i][j]=0;
  rep(i,n+1) dp[i][0]=i;
  rep(i,m+1) dp[0][i]=i;
  for(int i=1;i<=n;i++) for(int j=1;j<=m;j++){
    int min=1e9;
    int add=S[i-1]==T[i-1]?0:1;
    if(dp[i-1][j-1]+add<min) min=dp[i-1][j-1]+add;
    if(dp[i-1][j]+1<min) min=dp[i-1][j]+1;
    if(dp[i][j-1]+1<min) min=dp[i][j-1]+1;
    dp[i][j]=min;
  }
  return dp[n][m];
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
  int N=20;
  unsigned char S[N][210000];
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
    for(int j=cur;j<210000;j++) S[i][j]=4;
  }
  //0,1 > 0 
  //2,3 > 1
  int pt[N];
  rep(i,N) pt[i]=0;
  unsigned char now[N];
  int zero,one,wrong;
  rep(i,200000){
    rep(j,N) now[j]=S[j][pt[j]];
    /*if(i<=100){
      rep(j,N) printf("%d ",now[j]);
      printf("\n");
    }*/
    zero=0,one=0;
    rep(j,N){
      if(now[j]==4) continue;
      else if(now[j]&2) one++;
      else zero++;
    }
    if(zero>=one) fputc('0',dfp),wrong=1;
    else fputc('1',dfp),wrong=0;
    unsigned char base[33];
    rep(j,33){
      int chose=0;
      int n[4]={};
      rep(k,N){
        if(S[k][pt[k]+j]==4) continue;
        else if(wrong){
          if(now[k]>=2) continue; 
          chose++;
          n[S[k][pt[k]+j]]++;
        }
        else{
          if(now[k]<2) continue;
          chose++;
          n[S[k][pt[k]+j]]++;
        }
      }
      int plc=0,maxi=0;
      rep(k,4) if(n[k]>maxi) plc=k,maxi=n[k];
      if(chose==0) base[j]=4;
      else base[j]=plc;
    }
    unsigned char base1[30],base2[30],base3[30],base4[30];
    rep(j,30) base1[j]=base[j],base2[j]=base[j+1],base3[j]=base[j+2],base4[j]=base[j+3];
    rep(j,N){
      if(now[j]==4) continue;
      if(wrong){
        if(now[j]<2) continue;
        unsigned char cmp1[30],cmp2[30],cmp3[30],cmp4[30];
        for(int k=pt[j]+1;k<=pt[j]+30;k++) cmp1[k-pt[j]-1]=S[j][k];
        for(int k=pt[j]+2;k<=pt[j]+31;k++) cmp2[k-pt[j]-2]=S[j][k];
        for(int k=pt[j]+3;k<=pt[j]+32;k++) cmp3[k-pt[j]-3]=S[j][k];
        for(int k=pt[j];k<=pt[j]+29;k++) cmp4[k-pt[j]]=S[j][k];
        int diff[6];
        diff[0]=edis(base1,30,cmp1,30);
        diff[1]=edis(base1,30,cmp2,30);
        diff[2]=edis(base1,30,cmp3,30);
        diff[3]=edis(base2,30,cmp4,30);
        diff[4]=edis(base3,30,cmp4,30);
        diff[5]=edis(base4,30,cmp4,30);
        int mini=60,use=0;
        rep(k,6) if(mini>diff[k]) mini=diff[k],use=k;
        if(use==0) pt[j]++;
        else if(use==3) pt[j]--;
        else if(use==1) pt[j]+=2;
        else if(use==4) pt[j]-=2;
        else if(use==2) pt[j]+=3;
        else pt[j]-=3;
      }
      else{
        if(now[j]>=2) continue;
        unsigned char cmp1[30],cmp2[30],cmp3[30],cmp4[30];
        for(int k=pt[j]+1;k<=pt[j]+30;k++) cmp1[k-pt[j]-1]=S[j][k];
        for(int k=pt[j]+2;k<=pt[j]+31;k++) cmp2[k-pt[j]-2]=S[j][k];
        for(int k=pt[j]+3;k<=pt[j]+32;k++) cmp3[k-pt[j]-3]=S[j][k];
        for(int k=pt[j];k<=pt[j]+29;k++) cmp4[k-pt[j]]=S[j][k];
        int diff[6];
        diff[0]=edis(base1,30,cmp1,30);
        diff[1]=edis(base1,30,cmp2,30);
        diff[2]=edis(base1,30,cmp3,30);
        diff[3]=edis(base2,30,cmp4,30);
        diff[4]=edis(base3,30,cmp4,30);
        diff[5]=edis(base4,30,cmp4,30);
        int mini=60,use=0;
        rep(k,6) if(mini>diff[k]) mini=diff[k],use=k;
        if(use==0) pt[j]++;
        else if(use==3) pt[j]--;
        else if(use==1) pt[j]+=2;
        else if(use==4) pt[j]-=2;
        else if(use==2) pt[j]+=3;
        else pt[j]-=3;
      }
    }
    rep(j,N) pt[j]++;
  }
  //rep(k,N) printf("%d ",pt[k]);
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
