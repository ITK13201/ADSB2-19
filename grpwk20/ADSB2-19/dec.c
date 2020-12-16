#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#include <stdbool.h>
#pragma GCC optimize("Ofast")
#define rep(i,n) for(int (i)=0;(i)<(n);(i)++)
#define max(a,b) a>b?a:b

int edis(unsigned char *S,int n,unsigned char *T){
  int dp[n+1][n+1];
  rep(i,n+1) dp[i][0]=i;
  rep(i,n) dp[0][i+1]=i+1;
  for(int i=1;i<=n;i++) for(int j=1;j<=n;j++){
    int min;
    int add=S[i-1]==T[i-1]?0:1;
    //if(dp[i-1][j-1]+add<min) min=dp[i-1][j-1]+add;
    min=dp[i-1][j-1]+add;
    if(dp[i-1][j]+1<min) min=dp[i-1][j]+1;
    if(dp[i][j-1]+1<min) min=dp[i][j-1]+1;
    dp[i][j]=min;
  }
  return dp[n][n];
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
  int N=9,M=15;
  unsigned char S[N][205000];
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
    for(int j=cur;j<205000;j++) S[i][j]=4;
  }
  //0,1 > 0 
  //2,3 > 1
  int pt[N];
  rep(i,N) pt[i]=0;
  unsigned char now[N];
  int zero,one,wrong;
  unsigned char base[M+3];
  rep(i,200000){
    rep(j,N) now[j]=S[j][pt[j]];
    /*if(i>=199900){
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
    rep(j,M+3){
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
    unsigned char base1[M],base2[M],base3[M],base4[M];
    rep(j,M) base1[j]=base[j],base2[j]=base[j+1],base3[j]=base[j+2],base4[j]=base[j+3];
    rep(j,N){
      if(now[j]==4) continue;
      if(wrong){
        if(now[j]<2) continue;
        unsigned char cmp1[M],cmp2[M],cmp3[M],cmp4[M];
        for(int k=pt[j]+1;k<=pt[j]+M;k++) cmp1[k-pt[j]-1]=S[j][k];
        for(int k=pt[j]+2;k<=pt[j]+M+1;k++) cmp2[k-pt[j]-2]=S[j][k];
        for(int k=pt[j]+3;k<=pt[j]+M+2;k++) cmp3[k-pt[j]-3]=S[j][k];
        for(int k=pt[j];k<=pt[j]+M-1;k++) cmp4[k-pt[j]]=S[j][k];
        int diff[6];
        diff[0]=edis(base1,M,cmp1);
        diff[1]=edis(base1,M,cmp2);
        diff[2]=edis(base1,M,cmp3);
        diff[3]=edis(base2,M,cmp4);
        diff[4]=edis(base3,M,cmp4);
        diff[5]=edis(base4,M,cmp4);
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
        if(now[j]>=2) continue;
        unsigned char cmp1[M],cmp2[M],cmp3[M],cmp4[M];
        for(int k=pt[j]+1;k<=pt[j]+M;k++) cmp1[k-pt[j]-1]=S[j][k];
        for(int k=pt[j]+2;k<=pt[j]+M+1;k++) cmp2[k-pt[j]-2]=S[j][k];
        for(int k=pt[j]+3;k<=pt[j]+M+2;k++) cmp3[k-pt[j]-3]=S[j][k];
        for(int k=pt[j];k<=pt[j]+M-1;k++) cmp4[k-pt[j]]=S[j][k];
        int diff[6];
        diff[0]=edis(base1,M,cmp1);
        diff[1]=edis(base1,M,cmp2);
        diff[2]=edis(base1,M,cmp3);
        diff[3]=edis(base2,M,cmp4);
        diff[4]=edis(base3,M,cmp4);
        diff[5]=edis(base4,M,cmp4);
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
      if(pt[j]>=204900) pt[j]=201000;
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