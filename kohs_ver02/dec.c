#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grpwk20.h"
#define DECDATA_LEN 250000
#define SEARCH 10 // エラー補完のために探索する数
#define ERROR 3   // エラー
#define DELETE 5  // 挿入エラーと判定された、削除対象
#define UNJUDGE 7 // 判定できない

int fir[DECDATA_LEN]={0};
int sec[DECDATA_LEN]={0};

int double_null_cnt = 0; // fir,sec共に同じ位置でエラーが挿入か欠損か判定出来ない個数
int fir_null_cnt = 0; //firのうちnullが見つかった数 (可変)
int sec_null_cnt = 0; //secのうちnullが見つかった数 (可変)
int co_null_cnt = 0;
int fir_null=0; //firのうちnullが見つかった数 (総計)
int sec_null=0; //secのうちnullが見つかった数 (総計)
int fir_len=0, sec_len=0; // firの長さ,　secの長さ

int err_2(int fir[], int sec[], int pos);
int err(int fir[], int sec[], int pos, int which);

/*err: 挿入または欠損エラーが1箇所で発生した際に対処する関数-----------------------------------------------------------*/
int err(int fir[], int sec[], int pos, int which){ 
  // pos: エラー発生箇所のindex   which: エラーがfirなら0 secなら1 
    int search = SEARCH; // 探索数
    int j=0;
    int search_flg=0; // サーチ中に比較要素が異なれば1になる (欠損エラーであれば1, 挿入エラーであれば0)
    int fir_pos = pos - sec_null_cnt; //firのエラー参照箇所
    int sec_pos = pos - fir_null_cnt; //secのエラー参照箇所
    if(which==0){ // firにエラーがあった場合-----------------------------------------------------------
      for(j=1; j<=search;j++){ // エラー箇所から先SEARCH個を探索
        if(fir_pos+j >=fir_len || sec_pos+j-1 >=sec_null_cnt) // 配列の端であれば終わり
          break;
        // fir[pos+1]~fir[pos+SEARCH]
        // sec[pos]~sec[pos+SEARCH-1]　を比較する
        if(fir[fir_pos+j]==ERROR && sec[sec_pos+j-1] == ERROR){ // 両方エラーだったとき----------
          fir[fir_pos+j] = err_2(fir, sec, pos+j);
          if(fir[fir_pos+j] ==UNJUDGE){
            sec[sec_pos+j-1] = UNJUDGE; double_null_cnt++;
          }else if(fir[fir_pos+j]<=1) sec[sec_pos+j-1] = DELETE; 
          else                        sec[sec_pos+j-1] = fir[fir_pos+j+1]; // -----------------
        }else if(fir[fir_pos+j] == ERROR){ // firに途中でエラーが出てきたとき
          fir[fir_pos+j] = err(fir, sec, pos+j, 0);
        }else if(sec[sec_pos+j-1] == ERROR){ // secに途中でエラーが出てきたとき
          sec[sec_pos+j-1] = err(fir, sec, pos+j-1, 1);
        }
        if(fir[fir_pos+j] != sec[sec_pos+j-1]){ // firのエラーは欠損エラーであると判定されたとき
          search_flg = 1; break;
        }
      }
      if(search_flg==0){ // 挿入エラーだった場合
        fir_null_cnt++;  // 項目を後で削除するため
        sec_pos = pos - fir_null_cnt;
        fir_null++;      return DELETE; // エラー箇所は削除される
      }else              return sec[sec_pos]; // 欠損エラーだった場合 -> 箇所にはもう一方の値が入る
    }else if(which == 1){ // secにエラーがあった場合-----------------------------------------------------
      for(j=1; j<=search; j++){  // エラー箇所から先SEARCH個を探索
        if(fir_pos+j-1 >=fir_len || sec_pos+j >=sec_null_cnt) // 配列の端であれば終わり
          break;
        // fir[pos]~fir[pos+SEARCH-1]
        // sec[pos+1]~sec[pos+SEARCH]を検索
        if(fir[fir_pos+j-1]==ERROR && sec[sec_pos+j] ==ERROR){ // 両方エラーだった場合-----
          fir[fir_pos+j-1] = err_2(fir, sec, pos+j);
          if(fir[fir_pos+j-1] ==UNJUDGE){
            sec[sec_pos+j] = UNJUDGE; double_null_cnt++;
          }else if(fir[fir_pos+j-1]<=1) sec[sec_pos+j] = DELETE; // 5はnull
          else                          sec[sec_pos+j] = fir[fir_pos+j-1+1]; // ----------
        }else if(fir[fir_pos+j-1] == ERROR){ // 途中でfirにエラーがみつかった時
          fir[fir_pos+j-1] = err(fir, sec, pos+j-1, 0);
        }else if(sec[sec_pos+j] == ERROR){   // 途中でsecにエラーがみつかった時
          sec[sec_pos+j] = err(fir, sec, pos+j, 1);
        }
        if(fir[fir_pos+j-1] != sec[sec_pos+j]){ // 挿入エラーが発見された場合
          search_flg = 1; break;
        }
        //if(sec[sec_pos+j+1] != fir[fir_pos+j-1-1]){
        //  search_flg = 1; break;
        //}
      }
      if(search_flg==0){ // 挿入エラーだった場合
        sec_null_cnt++;
        fir_pos = pos - sec_null_cnt;  
        sec_null++;              return DELETE; // エラー箇所は削除対象になる
      }else if(search_flg == 1)  return fir[fir_pos]; // 欠損エラーの場合 -> エラー箇所にはもう一方の値が入る 
    }
    return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
/*err_2: firとsecが同じ位置でエラーが生じている場合に対応する関数--------------------------------------------------------*/
int err_2(int fir[], int sec[], int pos){
  // pos: エラー発生箇所のindex   which: エラーがfirなら0 secなら1 
  // dec関数上では、firに返却値が格納される
    printf("double error!\n");
    int search = SEARCH;
    int j=0, k=0;
    int search_flg=0; // サーチ中に比較要素が異なれば1になる
    int sub_flg = 0;
    int fir_pos = pos - sec_null_cnt;
    int sec_pos = pos - fir_null_cnt;
    for(j=1; j<=search;j++){
      if(fir_pos+j >=fir_len || sec_pos+j >=sec_null_cnt) // 配列の端であれば終わり
        break;
      if(fir[fir_pos+j] != sec[sec_pos+j]){ //どちらかが挿入エラーでもう一方が欠損エラーの可能性が高いと判定
        search_flg = 1; break;
      }
    }
    if(search_flg==0){ // search個先全部が一緒だったらUNJUDGEとして登録  (ここはSEARCHの値を大きくすると、発生確率を下げられる。)
      printf("UNJUDGE!\n");                 // 誤差発生の最大の原因箇所の1つ
      return UNJUDGE; 
    }else if(search_flg==1){ // search個先で異なる箇所があれば、どちらかが値を持ち、どちらかがnullとなるはず
      k=0;
      sub_flg=0; // firが欠損エラー、secが挿入エラーであれば1, 逆であれば0
      for(k=1; k<=search; k++){
        if(fir[fir_pos+j] != sec[sec_pos+j+1]){ // firが欠損エラー、secが挿入エラーの可能性が高い
          sub_flg=1; break;
        }
      }
      if(sub_flg == 1)      return sec[sec_pos+1]; 
      else if(sub_flg == 0) return DELETE; // 5はnull
    }
    return 0;
}
/*-----------------------------------------------------------------------------------------------------------------------*/

void adjust_null(int pos){
  if(fir_null_cnt >0 && sec_null_cnt > 0){ // nullの数に応じてiとnull_countの値を調節する
      co_null_cnt = fir_null_cnt;
      if(sec_null_cnt < fir_null_cnt) co_null_cnt = sec_null_cnt;
      pos -= co_null_cnt;
      fir_null_cnt -= co_null_cnt;
      sec_null_cnt -= co_null_cnt;
      co_null_cnt = 0;
    }
}

/*revise: エラーに対処する関数--------------------------------------------------------------------------------------------*/
void revise(int fir[], int sec[]){
  int i=0;
  int fir_len=0, sec_len=0;
  fir_len = fir_len - fir_null;
  sec_len = sec_len - sec_null;
  int data_len = fir_len;
  if(sec_len < fir_len) data_len = sec_len;
  for(i=0; i<data_len; i++){ 
    if(fir[i-sec_null_cnt] != ERROR && sec[i-fir_null_cnt] != ERROR) continue; // 両方エラーで無ければ次へ
    if(fir[i-sec_null_cnt]==ERROR && sec[i-fir_null_cnt] == ERROR){ // 両方ともエラーであるとき--------------------------------
      fir[i-sec_null_cnt] = err_2(fir, sec, i); // firにエラー対応結果を返す
      if(fir[i-sec_null_cnt] ==UNJUDGE){ // firがUNJUDGEと判定された場合
        sec[i-fir_null_cnt] = UNJUDGE; double_null_cnt++; // secもUNJUDGEDにする
      }else if(fir[i-sec_null_cnt]<=1)      sec[i-fir_null_cnt] = DELETE; // firにsecの次の値が入るなら、secは除去
      else  if(fir[i-sec_null_cnt]==DELETE) sec[i-fir_null_cnt] = fir[i-sec_null_cnt+1]; // firが除去ならsecにはfirの次の値が入る
      
    }else if(fir[i-sec_null_cnt] == ERROR){ // firがエラーだったとき----------------------------------------------------------
      fir[i-sec_null_cnt] = err(fir, sec, i, 0);
    }else if(sec[i-fir_null_cnt] == ERROR){ // secがエラーだったとき-----------------------------------------------------------
      sec[i-fir_null_cnt] = err(fir, sec, i, 1); 
    }else if(fir[i-sec_null_cnt] != sec[i-fir_null_cnt]){ // firとsecが01で一致しない時->挿入または欠損エラー--------------------
      fir[i-sec_null_cnt] = err_2(fir, sec, i); // firにエラー対応結果を返す
      if(fir[i-sec_null_cnt] ==UNJUDGE){ // firがUNJUDGEと判定された場合
        sec[i-fir_null_cnt] = UNJUDGE; double_null_cnt++; // secもUNJUDGEDにする
      }else if(fir[i-sec_null_cnt]<=1)      sec[i-fir_null_cnt] = DELETE; // firにsecの次の値が入るなら、secは除去
      else  if(fir[i-sec_null_cnt]==DELETE) sec[i-fir_null_cnt] = fir[i-sec_null_cnt+1]; // firが除去ならsecにはfirの次の値が入る
    }
    adjust_null(i); 
  }
}
/* dec: デコーダ------------------------------------------------------------------------*/
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

  unsigned char c1, c2, res;
  int flg = 0; // 前の入力が共に正常と判断すれば0 エラーが確認できれば1
  int cnt=0;
  int i=0;
  while(1){
    if(flg == 0){
      c1 = getc(sfp);
      if(c1 == '\n') break;
      c2 = getc(sfp);
      if(c2 == '\n') break;
    }else if(flg == 1){
      c1 = c2;
      c2 = getc(sfp);
      if(c2 == '\n') break;
    }
    
    if(c1 == 'A' && c2 == 'T'){
      fir[cnt] = 0;
      cnt++; flg = 0;
    }else if(c1 == 'G' && c2 == 'C'){
      fir[cnt] = 1;
      cnt++; flg = 0;
    }else if(((c1=='A'&& c2 !='T') || (c1=='G' && c2!='C')) || (c1=='T' || c1=='C')){
      fir[cnt] = ERROR; // 3はエラー発生箇所
      cnt++; flg = 1;
    }   
  }
  fir_len = cnt;
  printf("fir_len: %d\n", fir_len);
  cnt = 0;
  while(1){
    if(flg == 0){
      c1 = getc(sfp);
      if(c1 == '\n') break;
      c2 = getc(sfp);
      if(c2 == '\n') break;
    }else if(flg == 1){
      c1 = c2;
      c2 = getc(sfp);
      if(c2 == '\n') break;
    }
    
    if(c1 == 'A' && c2 == 'T'){
      sec[cnt] = 0;
      cnt++; flg = 0;
    }else if(c1 == 'G' && c2 == 'C'){
      sec[cnt] = 1;
      cnt++; flg = 0;
    }else if(((c1=='A'&&c2 !='T') || (c1=='G' && c2!='C')) || (c1=='T' || c1=='C')){
      sec[cnt] = ERROR; // 3はエラー発生箇所
      cnt++; flg = 1;
    }   
  }
  sec_len = cnt;
  printf("sec_len: %d\n", sec_len);
  int data_len=fir_len;

  if(sec_len < fir_len) data_len = sec_len; 
  int co_null_cnt=0;
  // 2本のデータを互いに参照し、エラーに対応する------------------------------------------
  revise(fir, sec);
  printf("double_null_count: %d\n", double_null_cnt);
  fir_len = fir_len - fir_null;
  sec_len = sec_len - sec_null;
  data_len = fir_len;
  if(sec_len < fir_len) data_len = sec_len;
  int dis = data_len -ORGDATA_LEN;
  int data_cnt = 0;
  for(i=0; i<data_len; i++){
    if(fir[i]==0) res = '0';
    else if(fir[i]==1) res = '1';
    else if(fir[i]==DELETE) continue;
    else if(fir[i]==UNJUDGE){
      if(double_null_cnt>0 && dis>0){
        res = '0';
        double_null_cnt--;
        dis--;
      }else{
        continue;
      }
    }
    data_cnt++;
    fputc(res, dfp);
  }
  printf("revised fir length: %d\n", fir_len);
  printf("revised sec length: %d\n", sec_len);
  printf("data_cnt: %d\n", data_cnt);
  int nokori = ORGDATA_LEN - data_cnt;
  for(i=0; i<nokori; i++){
    fputc('0', dfp);
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
