* kohs_BS_ver0.1 ベンチマーク用

ブロック(25文字)の通し番号をindexと表記します。indexごとにデータを専用の配列に振り分けます。各ブロックにおけるindexエラーは40%以上なので、各indexには複数のデータが集まる場合もあり、その逆もあります。そのため、一旦全てのデータを振り分けたのち、あたりには印をつけた上で、重み付きの多数決を取ります
<br>
評価：コストとハミング距離だけ
<br>
ITERATE: 生成する行数, hamming distance: ハミング距離
<br>
ITERATE = 3  hamming distance = 30000
<br>
ITERATE = 4  hamming distance = 22000
<br>
ITERATE = 5  hamming distance = 17585
<br>
ITERATE = 10 hamming distance = 4395
<br>
ITERATE = 15 hamming distance = 1400
<br>
ITERATE = 20 hamming distance = 439
<br>
ITERATE = 25 hamming distance = 121
<br>
ITERATE = 30 hamming distance = 39
<br>
ITERATE = 35 hamming distance = 19
<br>
ITERATE = 40 hamming distance = 2
<br>
ITERATE = 45 hamming distance = 2
<br>
ITERATE = 50 hamming distance = 0
<br>
ベンチマーク用でもコスト捨てるとハミング距離0になります()


* BS_ver0.2 改良版
<br>
ブロック(25文字)の通し番号をindexと表記します。indexごとにデータを専用の配列に振り分けます。各ブロックにおけるindexエラーは40%以上なので、各indexには複数のデータが集まる場合もあり、その逆もあります。そのため、一旦全てのデータを振り分けたのち、各index内のデータ(18文字)のハミング距離を比較し、正しく振り分けられたデータが存在する場合、そのデータに印をつけます。印がついたindexの中で印がつかなかったデータは、indexの7文字をそれぞれ一文字ずつ変えた有効なindexに再度振り分けます。その後、もう一度ハミング距離で比較し印をつけます。最後に、印がついたデータには一定の重み(WEIGHT)
を付けた多数決を行い、最終的なデータを決定します。

性能：
プログラムの都合上、生成できる行数が最大で17でした(アルゴリズムをうまいこと変えればこの問題は解決できそうですが、、)
<br>
(行数3)  cost: 84   hamming distance = 34000
<br>
(行数10) cost:280   hamming distance = 1462 (改善)
<br>
(行数17) cost: 476  hamming distance = 119  (改善)

実行：
ITERATEで引き出す行数を指定できます。  ./test.sh ITERATE 0 0 0 0 0


ハミング距離を0にする場合ver0.1のベンチマーク用でしかできないのですが、どのみちコストが削減できてないので...
