一の箇所を読み取ったデータであると判定するハミング距離の閾値
#define OVER  1000 // 1つのindexに重複できるデータの個数
#define OVER_sub 1000 // check関数用
#define WEIGHT 10  // 多数決決定法において当たりと判定されたものに付ける重み

typedef struct{
  int count;
  int dist_plus;
  int flg[OVER]; // そのcount値が当たりであれば1、はずれであれば0
  unsigned char data[OVER][18];  
}BLOCK;

// check関数用
typedef struct{
  int left[ITERATE];
  int ham[ITERATE];
  int flg[ITERATE];  // hamが8以下であれば1 そうで無ければ0
}CHECK_SUB;

BLOCK block[ITERATE][BLOCK_LEN]; // インデックスに応じた文字列を格納
CHECK_SUB check_sub[ITERATE];
unsigned char id_char[7]; // distribute用
int atari[BLOCK_LEN] = {0}; //指定のindexが当たりであれば1, はずれであれば0
unsigned char mdres[20]; // 多数決結果を一時的に格納 majority decision result


/*init_block: blockの初期化-------------------------------------------------------------------*/
void init_block(){
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<BLOCK_LEN; j++){ 
      block[i][j].count = 0;
      block[i][j].dist_plus = 0;
      // for(int k=0; k<ITERATE-1; k++)
      for(int k=0;k<OVER;k++){
        block[i][j].flg[k] = 0;
        block[i][j].data[k][0] = 'P'; // Pであれば空
      }
    }
  }
  return;
}

/*init_check_sub: check_subの初期化-----------------------------------------------------------*/
void init_check_sub(){
  for(int i=0; i<ITERATE;i++){
    for(int j=0; j<ITERATE; j++){
      check_sub[i].left[j] = -1;
      // check_sub[i].right[j] = -1;
      check_sub[i].ham[j] = 18;
      check_sub[i].flg[j] = 0;
    }
  }
  return;
}
/* quad_sub: ATGCを数値に変換 (index用)-------------------------------------------------------*/
int quad_sub(unsigned char ATGC){
  switch(ATGC){
    case 'A': return 0;
    case 'C': return 1;
    case 'G': return 2;
    case 'T': return 3;
  }
  return 0;
}
/* quad2num: 4進数を10進数に変換 (index用)----------------------------------------------------*/
int quad2num(unsigned char quad[]){
  int num=0;
  int tmp = 0;
  //int dis = 0;
  int i = 4096;
  for(int j=0; j<7; j++){
    tmp = quad_sub(quad[j]);
    num += tmp * i;
    if(j!=6) i = i/4;
  }
  i = 1;
  if(num > 5555){ // 置換エラーで明らかにアウトなインデックスが出た時 GGCTGGA以上
    num = rand() % 5555;
  }
  return num;
}

/* conv_sub: num2quadの補助関数---------------------------------------------------------------*/
unsigned char conv_sub(int a){
  switch(a){
    case 0: return 'A';
    case 1: return 'C';
    case 2: return 'G';
    case 3: return 'T';
  }
  return 'A';
}

/*num2quad 10進数をATGC4進数に変換------------------------------------------------------------*/
void num2quad(int num){
  int i = 4096; // 4^6
  int tmp = num; // 各桁の値
  int j=0; 
  for(j=0; j<7; j++){
    tmp = num / i;
    id_char[j] = conv_sub(tmp);
    num = num % i;
    i = i / 4;
  }
  return;
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
}
/* ATGC2c2: 文字cから右側を取り出す---------------------------------------------------------*/
unsigned char ATGC2c2(unsigned char c){
  // unsigned char res = '0';
  switch(c){
    case 'A': return '0';
    case 'C': return '1';
    case 'G': return '0';
    case 'T': return '1';
  }
  return '0';
}

/* hamming: 二つのデータのハミング距離を計算 (文字が異なれば+1)-------------------------------*/
int  hamming(unsigned char v1[18], unsigned char v2[18]){
  int distance = 0;
  for(int i=0; i<18; i++){
    if(v1[i] != v2[i]) distance++;
  }
  return distance;
}

/*check: 指定したインデックスの各文字列に対して、当たりがあるかを探索、あれば印をつける
　　　　　返却値は当たりがあれば1 無ければ0---------------------------------------------------*/
int check(int index){
  init_check_sub();
  int res =0;
  int search_cnt = 0;
  int tmp = 0;
  int tmp_flg = 0;
  int iter_sub[OVER_sub] = {-1};
  int count_sub[OVER_sub] = {-1};
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<block[i][index].count; j++){
      iter_sub[search_cnt] = i; // iterateの番号を入れる
      count_sub[search_cnt] = j; // countの番号を入れる
      search_cnt++;
    }
  }
  // indexにある要素のハミング距離を比較する
  for(int i=0; i<search_cnt; i++){
    for(int j=i+1; j<search_cnt; j++){
      if(iter_sub[i] == iter_sub[j]) continue; // もし同じITERATEを比較する時は飛ばす
      tmp = hamming(block[iter_sub[i]][index].data[count_sub[i]], block[iter_sub[j]][index].data[count_sub[j]]);
      if(tmp < check_sub[iter_sub[i]].ham[iter_sub[j]]){
        check_sub[iter_sub[i]].ham[iter_sub[j]] = tmp;
        if(tmp <= JUDGE){
          res = 1;
          atari[index] = 1;
          check_sub[iter_sub[i]].flg[iter_sub[j]] = 1;
          check_sub[iter_sub[i]].left[iter_sub[j]] = count_sub[i];
          check_sub[iter_sub[j]].flg[iter_sub[i]] = 1;
          check_sub[iter_sub[j]].left[iter_sub[i]] = count_sub[j];
        }
      }
    }
  }
  // int flg_cnt = 0;
  for(int i=0; i<ITERATE; i++){
    tmp_flg = 0;
    // flg_cnt = 0;
    for(int j=0;j<ITERATE;j++){
      if(check_sub[i].flg[j] == 1){ // 別のITERATEとの当たりがあった
        tmp_flg = 1; break;
      }
    }
    if(tmp_flg == 1){ // 今見ているITERATEに当たりが1つでもあった場合
      for(int j=0; j<ITERATE; j++)
        if(check_sub[i].left[j] != -1){ // 当たったときの自分のcount数が判明したとき
          block[i][index].flg[check_sub[i].left[j]] = 1; // 当たりだったcount値は1
        }
    }
  }
  return res;
}

/*dist_each_sub: 指定されたiterate, indexのbitをcに変換し、新たなindexの箇所に挿入する------------------*/
void dist_each_sub(int iterate, int index, int bit, unsigned char c, unsigned char from[]){
  int new_index = 0;
  int data_count = 0;
  id_char[bit] = c;
  new_index = quad2num(id_char);
  if(atari[new_index] == 0 && new_index <= 5555){
    for(int i=0; i<18; i++){
      data_count = block[iterate][new_index].count;
      block[iterate][new_index].data[data_count][i] = from[i];
    }
    block[iterate][new_index].dist_plus++;
  }else{
    return;
  }
  return;
}

/*dist_each: 分配指せる箇所のiterate, index, とbitの桁を指定し、分配を行う--------------------------------*/
void dist_each(int iterate, int index, unsigned char data[]){
  num2quad(index);
  for(int bit =0; bit<7; bit++){
    if(id_char[bit] == 'A'){
      dist_each_sub(iterate, index, bit, 'C', data);
      dist_each_sub(iterate, index, bit, 'G', data);
      dist_each_sub(iterate, index, bit, 'T', data);
    }else if(id_char[bit] == 'C'){
      dist_each_sub(iterate, index, bit, 'A', data);
      dist_each_sub(iterate, index, bit, 'G', data);
      dist_each_sub(iterate, index, bit, 'T', data);
    }else if(id_char[bit] == 'G'){
      dist_each_sub(iterate, index, bit, 'A', data);
      dist_each_sub(iterate, index, bit, 'C', data);
      dist_each_sub(iterate, index, bit, 'T', data);
    }else if(id_char[bit] == 'T'){
      dist_each_sub(iterate, index, bit, 'A', data);
      dist_each_sub(iterate, index, bit, 'C', data);
      dist_each_sub(iterate, index, bit, 'G', data);
  } 
  }
  return;
}

/*distribute_sub: 指定されたindexにおける余ったデータを分配する----------------------------------------------*/
void distribute_sub(int index){
  // int flg_check = 0; // 探索中のcount値が当たりだったときに1
  int data_count = 0;
  for(int i=0; i<ITERATE; i++){ // 各iterateを探索
  data_count = block[i][index].count;
    for(int j=0; j< data_count; j++){ // 指定したindexの各count値が当たりであるか探索
      if(block[i][index].flg[j] == 1) continue; // 当たりであればスルー
      else{ // 外れであれば分配する必要あり
        dist_each(i, index, block[i][index].data[j]);
      }
    }
  }
  return;
}

/*distribute: 分配を行う関数------------------------------------------------------------------------------*/
void distribute(){
  int check_res = 0;
  for(int i=0; i<BLOCK_LEN; i++){
    check_res = 0;
    check_res = check(i);
    if(check_res == 1) distribute_sub(i); // 当たりが含まれていれば、残りを分配する
  }
  return;
}

/*re_count: countを再集計*/
void re_count(){
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<BLOCK_LEN; j++){
      block[i][j].count = block[i][j].count + block[i][j].dist_plus;
    }
  }
}

/*Toblock_sub: ファイルから25文字文字を読み出し、インデックスの箇所に挿入する--------------------------------*/
void Toblock_sub(int num, int kaisu, FILE *fp){
  unsigned char c1[7], c2[18];
  // unsigned char dis;
  int index = 0;
  int cum_count =0;
  if(num != 0 && kaisu == 0) fgetc(fp); /*改行を退避させる*/
  for(int i=0; i<25; i++){
    if(i<7) c1[i] = fgetc(fp);
    else    c2[i-7] = fgetc(fp);
  }
  index = quad2num(c1); // インデックスを計算
  cum_count = block[num][index].count;
  for(int i=0; i<18; i++){ // 指定箇所にデータの書込み
    block[num][index].data[cum_count][i] = c2[i];
  }
  block[num][index].count++;
  return;
}

/*Toblock: ファイルから全ての文字列を読み出し、インデックスの箇所に挿入する-----------------------------------*/
void Toblock(FILE *fp){
  for(int i=0; i<ITERATE; i++){
    for(int j=0; j<BLOCK_LEN; j++){
      Toblock_sub(i, j, fp);
    }
  }
  return;
}

/*md_sub: 4つの中から最大のものを取り出す------------------------------------------------------------------*/
int md_sub(int a[]){
  int index1 = 0, index2 = 2;
  if(a[0] < a[1]){a[0] = a[1]; index1 = 1;}
  if(a[2] < a[3]){a[2] = a[3]; index2 = 3;}
  if(a[0] < a[2]){a[0] = a[2]; index1 = index2;}
  if(a[0] == 0) index1 = -1;
  return index1;
}

/* Majority: 多数決をとる関数　結果→Mdres----------------------------------------------------------------*/
void Majority(int index){
  int vote[4] = {0,0,0,0}; // 0→A 1→C 2→G 3→T
  int vote_id = 0;
  for(int i=0; i<18; i++){
    vote[0]=0; vote[1]=0; vote[2]=0, vote[3]=0;
    for(int j=0; j<ITERATE; j++){
      for(int k=0; k<block[j][index].count; k++){
        if(block[j][index].flg[k] == 1){
          switch(block[j][index].data[k][i]){
            case 'A': vote[0]+=WEIGHT;
                      break;
            case 'C': vote[1]+=WEIGHT;
                      break;
            case 'G': vote[2]+=WEIGHT;
                      break;
            case 'T': vote[3]+=WEIGHT;
                      break;
            default : break;
          } 
        }else{
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


/* dec: デコーダ---------------------------------------------------------------------------------*/
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
  distribute(); // 当たりがあるか判定し、当たりがあった場所の余りのデータは分配する
  re_count();
  for(int i=0; i<BLOCK_LEN; i++){ // 分配後、前のcheckで外れ判定を受けた箇所に、再度checkを行う
    if(atari[i] == 0)
      check(i);
  }

  // ATGCから01に直していく
  unsigned char c1, c2, res;
  int all_count=0;
  for(int m=0; m<BLOCK_LEN; m++){
    Majority(m); // 重み付き多数決で文字を決定する
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
  int zero_total = 0;
  int block_cnt =0;
  for(int i=0; i<BLOCK_LEN; i++){
    //printf("%d: ", i);
    zero_cnt = 0;
    block_cnt = 0;
    for(int j=0; j<ITERATE; j++){
      //printf("%d ", block[j][i].count);
      block_cnt += block[j][i].count;
      if(block[j][i].count == 0) zero_cnt++;
    }
    if(zero_cnt == ITERATE) zero_total++;
    if(block_cnt > count_max) count_max = block_cnt; 
    //printf("\n");
  }
  printf("count_max: %d\n", count_max);
  printf("zero_total: %d\n", zero_total);

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
