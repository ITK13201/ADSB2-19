# Description

## Encode

0bit目から見ていって，偶数bitと奇数bitでエンコード後の文字列を分ける．

- 偶数bit
  - 0: A
  - 1: G
- 奇数bit
  - 0: T
  - 1: C

最後の1つにだけ2bit分の情報を持たせ，DNAのsizeを下げる．
最後の2bitを小さい方からc1, c2とすると，

- (c1, c2)
  - (0, 0): A
  - (0, 1): T
  - (1, 0): G
  - (1, 1): C


## Decode