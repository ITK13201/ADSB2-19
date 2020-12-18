* kohs_BS_ver0.1 ベンチマーク用

ブロック(25文字)の通し番号をindexと表記します。indexごとにデータを専用の配列に振り分けます。各ブロックにおけるindexエラーは40%以上なので、各indexには複数のデータが集まる場合もあり、その逆もあります。そのため、一旦全てのデータを振り分けたのち、あたりには印をつけた上で、重み付きの多数決を取ります
<br>
ITERATE: 生成する行数, hamming distance: ハミング距離
<br>
ITERATE = 3  hamming distance = 30000
<br>
ITERATE = 4  hamming distance = 17000
<br>
ITERATE = 5  hamming distance = 11000
<br>
ITERATE = 10 hamming distance = 1342
<br>
ITERATE = 20 hamming distance = 59
<br>
ITERATE = 25 hamming distance = 19 実行時間 0.918s
<br>
ITERATE = 30 hamming distance = 5 実行時間 1.343s
<br>
ITERATE = 40 hamming distance = 2
<br>
ITERATE = 45 hamming distance = 0 実行時間 2.505s
<br>
ベンチマーク用でもコスト捨てるとハミング距離0になります()


* BS_ver0.2 改良版
<br>
ブロック(25文字)の通し番号をindexと表記します。indexごとにデータを専用の配列に振り分けます。各ブロックにおけるindexエラーは40%以上なので、各indexには複数のデータが集まる場合もあり、その逆もあります。そのため、一旦全てのデータを振り分けたのち、各index内のデータ(18文字)のハミング距離を比較し、正しく振り分けられたデータが存在する場合、そのデータに印をつけます。印がついたindexの中で印がつかなかったデータは、indexの7文字をそれぞれ一文字ずつ変えた有効なindexに再度振り分けます。その後、もう一度ハミング距離で比較し印をつけます。最後に、印がついたデータには一定の重み(WEIGHT)
を付けた多数決を行い、最終的なデータを決定します。

性能：
プログラムの都合上、生成できる行数が最大で17でした(配列を全部静的なまま実装してたのでmalloc使えばだいぶ解決するはずです(調整中))
<br>
(行数3)  cost: 84   hamming distance = 34000 実行時間： 0.196s, 0.125s, 0.031s
<br>
(行数10) cost:280   hamming distance = 1462  実行時間: 0.758s, 0.422s, 0.297s
<br>
(行数17) cost: 476  hamming distance = 119  (改善) 実行時間: 0.1284s 0.703s 0.547s

実行：
ITERATEで引き出す行数を指定できます。  ./test.sh ITERATE 0 0 0 0 0


ハミング距離を0にする場合ver0.1のベンチマーク用でしかできないのですが、どのみちコストが削減できてないので...
