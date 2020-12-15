#include <stdio.h>
#include <stdlib.h>
#include "grpwk20.h"
#define BLOCK_LEN 5556
#define DATA_LEN 200000
#define ITERATE 3

typedef struct{
  int count;
  int flg; // 確定であればそのインデックスを そうでなければ-1
  unsigned char data[100][18];  
}BLOCK;

BLOCK block[ITERATE][BLOCK_LEN]; // インデックスに応じた文字列を格納
unsigned char mdres[20]; // 多数決結果を一時的に格納 majority decision result

// FILE *sfp; // seqdata

/*init_block: blockの初期化-------------------------------------------------------------------*/
void init_block(){
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<BLOCK_LEN; j++){ 
      block[i][j].count = 0;
      block[i][j].flg = -1;
      for(int k=0;k<100;k++)
        block[i][j].data[k][0] = 'P'; // Pであれば空
    }
  }
}

/* hexa_sub: ATGCを数値に変換 (index用)-------------------------------------------------------*/
int hexa_sub(unsigned char ATGC){
  switch(ATGC){
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
  }
  return 0;
}
/* hexa2num: 4進数を10進数に変換 (index用)----------------------------------------------------*/
int hexa2num(unsigned char hexa[]){
  int num=0;
  int tmp = 0;
  int dis = 0;
  int i = 4096;
  for(int j=0; j<7; j++){
    tmp = hexa_sub(hexa[j]);
    num += tmp * i;
    if(j!=6) i = i/4;
  }
  i = 1;
  if(num > 5555){ // 置換エラーで明らかにアウトなインデックスが出た時 GGCTGGA以上
    /*if(hexa[0] == 'T'){
      num -= 4096*2;
    }else if(hexa[0]=='G'){
      num -= 4096;
    }else{
      tmp = num % 5;
      dis = 5555 - num;
      for(int j=0; j<tmp; j++){
        i = i*4;
        if(i>dis){
          i = i/4;
          break;
        }
      }
      num += i;
    }*/
    num = rand() % 5555;
  }
  return num;
}

/* ATGC2c1: 文字cから左側を取り出す---------------------------------------------------------*/
unsigned char ATGC2c1(unsigned char c){
  // unsigned char res = '0';
  switch(c){
    case 'A': return '0';
    case 'C': return '0';
    case 'G': return '1';
    case 'T': return '1';
  }
  return '0';
  /*switch(c){
    case 'A': res = '0';
              break;
    case 'C': res = '0';
              break;
    case 'G': res = '1';
              break;
    case 'T': res = '1';
              break;
  }
  return res;*/
}
/* ATGC2c2: 文字cから右側を取り出す-------------------------------------------------------*/
unsigned char ATGC2c2(unsigned char c){
  // unsigned char res = '0';
  switch(c){
    case 'A': return '0';
    case 'C': return '1';
    case 'G': return '0';
    case 'T': return '1';
  }
  return '0';
  /*switch(c){
    case 'A': res = '0';
              break;
    case 'C': res = '1';
              break;
    case 'G': res = '0';
              break;
    case 'T': res = '1';
              break;
  }
  return res;*/
}

/* hamming: 二つのデータのハミング距離を計算 (文字が異なれば+1)*/
int  hamming(unsigned char v1[18], unsigned char v2[18]){
  int distance = 0;
  for(int i=0; i<18; i++){
    if(v1[i] != v2[i]) distance++;
  }
  return distance;
}

/*check: 指定したインデックスの各文字列に対して、当たりがあるかを探索、あれば印をつける
　　　　　返却値は当たりがあれば1 無ければ0*/
int check(int index){
  int res =0;
  int search_cnt = 0;
  int iter_sub[100] = {-1};
  int count_sub[100] = {-1};
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<block[ITERATE][index].count; j++){
      iter_sub[search_cnt] = i; // iterateの番号を入れる
      count_sub[search_cnt] = j; // countの番号を入れる
      search_cnt++;
    }
  }
  int ham_min = 18; //出現したハミング距離の最小値
  int tmp = 0;
  int min_pos[ITERATE] = {-1}; // 最小となったときの
  for(int i=0; i<search_cnt; i++){
    for(int j=i+1; j<search_cnt; j++){
      tmp = hamming(block[iter_sub[i]][index].data[count_sub[i]], block[iter_sub[j]][index].data[count_sub[j]]);
      if(tmp < ham_min){

      }
    }
  }



}


/*Toblock_sub: ファイルから25文字文字を読み出し、インデックスの箇所に挿入する*/
void Toblock_sub(int num, FILE *fp){
  unsigned char c1[7], c2[18];
  unsigned char dis;
  int index = 0;
  int cum_count =0;
  if(num != 0) dis = fgetc(fp); /*改行を退避させる*/
  for(int i=0; i<25; i++){
    if(i<7) c1[i] = fgetc(fp);
    else    c2[i-7] = fgetc(fp);
  }
  index = hexa2num(c1); // インデックスを計算
  cum_count = block[num][index].count;
  for(int i=0; i<18; i++){ // 指定箇所にデータの書込み
    block[num][index].data[cum_count][i] = c2[i];
  }
  block[num][index].count++;

}

/*Toblock: ファイルから全ての文字列を読み出し、インデックスの箇所に挿入する*/
void Toblock(FILE *fp){
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<BLOCK_LEN; j++){
      Toblock_sub(i, fp);
    }
  }
}

/*md_sub: 4つの中から最大のものを取り出す*/
int md_sub(int a[]){
  int index1 = 0, index2 = 2;
  if(a[0] < a[1]){a[0] = a[1]; index1 = 1;}
  if(a[2] < a[3]){a[2] = a[3]; index2 = 3;}
  if(a[0] < a[2]){a[0] = a[2]; index1 = index2;}
  if(a[0] == 0) index1 = -1;
  return index1;
}

/* Majority: 多数決をとる関数　結果→Mdres*/
void Majority(int index){
  int vote[4] = {0,0,0,0}; // 0→A 1→C 2→G 3→T
  int vote_id = 0;
  for(int i=0; i<18; i++){
    vote[0]=0; vote[1]=0; vote[2]=0, vote[3]=0;
    for(int j=0; j<ITERATE; j++){
      for(int k=0; k<block[j][index].count; k++){
        switch(block[j][index].data[k][i]){
          case 'A': vote[0]++;
                    break;
          case 'C': vote[1]++;
                    break;
          case 'G': vote[2]++;
                    break;
          case 'T': vote[3]++;
                    break;
          default : break;
        }
      }
    }
    vote_id = md_sub(vote);
    if(vote_id == -1) mdres[i] = 'A';
    if(vote_id == 0) mdres[i] = 'A';
    else if(vote_id == 1) mdres[i] = 'C';
    else if(vote_id == 2) mdres[i] = 'G';
    else if(vote_id == 3) mdres[i] = 'T';

    
  }
  return;
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
  
  init_block(); //blockの初期化
  Toblock(sfp); // 文字列をblockに格納


  unsigned char c1, c2, res;
  int all_count=0;
  for(int m=0; m<BLOCK_LEN; m++){
    Majority(m);
    for(int n=0; n<18; n++){
       c1 = ATGC2c1(mdres[n]);
       c2 = ATGC2c2(mdres[n]);
       fputc(c1, dfp);
       fputc(c2, dfp);
       all_count+=2;
       if(all_count == DATA_LEN)
        break;
    }
  }
  printf("all_count: %d\n", all_count);
  int count_max=0;
  int zero_cnt = 0;
  for(int i=0; i<BLOCK_LEN; i++){
    printf("%d: ", i);
    zero_cnt = 0;
    for(int j=0; j<ITERATE; j++){
      printf("%d ", block[j][i].count);
      if(block[j][i].count == 0) zero_cnt++;
    }
    if(zero_cnt == ITERATE) count_max++; 
    printf("\n");
  }
  printf("count_max: %d\n", count_max);

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
