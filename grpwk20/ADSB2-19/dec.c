#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "grpwk20.h"
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#define rep(i, n) for (register int(i) = 0; (i) < (n); (i)++)
#define max(a, b) a > b ? a : b
#define N 4
#define M 12

int dp[M + 1][M + 1];

// 編集距離をもとめる関数
inline static int edis(unsigned char *S, unsigned char *T)
{
    bool chk = true;
    for (register int i = M - 1; i >= 0; i--)
    {
        if (S[i] != T[i])
        {
            chk = false;
            break;
        }
    }
    if (chk)
        return 0;
    for (register int i = 1; i <= M; i++)
    {
        for (register int j = 1; j <= M; j++)
        {
            int min = dp[i - 1][j - 1] + (S[i - 1] == T[j - 1] ? 0 : 1);
            if (dp[i - 1][j] + 1 < min)
                min = dp[i - 1][j] + 1;
            if (dp[i][j - 1] + 1 < min)
                min = dp[i][j - 1] + 1;
            dp[i][j] = min;
        }
    }
    return dp[M][M];
}

// 空列がある場合の編集距離を求める関数
inline static int edis2(unsigned char *S, unsigned char *T, int m)
{
    bool chk = true;
    for (register int i = m - 1; i >= 0; i--)
    {
        if (S[i] != T[i])
        {
            chk = false;
            break;
        }
    }
    if (chk)
        return 0;
    for (register int i = 1; i <= m; i++)
    {
        for (register int j = 1; j <= m; j++)
        {
            int min = dp[i - 1][j - 1] + (S[i - 1] == T[j - 1] ? 0 : 1);
            if (dp[i - 1][j] + 1 < min)
                min = dp[i - 1][j] + 1;
            if (dp[i][j - 1] + 1 < min)
                min = dp[i][j - 1] + 1;
            dp[i][j] = min;
        }
    }
    return dp[m][m];
}

int dec()
{
    FILE *sfp;
    if ((sfp = fopen(SEQDATA, "r")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", SEQDATA);
        exit(1);
    }

    FILE *dfp;
    if ((dfp = fopen(DECDATA, "w")) == NULL)
    {
        fprintf(stderr, "cannot open %s\n", DECDATA);
        exit(1);
    }
    rep(i, M + 1) dp[i][0] = i;
    rep(i, M) dp[0][i + 1] = i + 1;

    unsigned char c, res;
    int cur;
    unsigned char S[N][202000];
    char ans[200001];
    // N回読み出したものをdecodeする
    // S[0]: 0回目のdecode
    // ...
    // S[N-1]: N-1回目のdecode
    rep(i, N)
    {
        cur = 0;
        while ((c = getc(sfp)) != '\n')
        {
            // decode 
            // (A, T, G, C) = (0, 1, 2, 3)
            switch (c)
            {
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
            S[i][cur] = res;
            cur++;
        }
        // 余ったところは4で埋める
        for (int j = cur; j < 202000; j++)
            S[i][j] = 4;
    }
    int pt[N] = {};
    unsigned char now[N];
    int zero, one, wrong;
    unsigned char base[M + 2];
    // 誤り訂正
    rep(i, 199998)
    {
        // 読み取り0回目~N-1回目までのpt[j]列目をnowに格納
        rep(j, N) now[j] = S[j][pt[j]];
        
        zero = 0, one = 0;
        rep(j, N)
        {
            if (now[j] == 4)
                continue;
            // now[j]が2 or 3，かつ，i, now[j]が共に偶数 or 奇数の場合
            else if (now[j] & 2 && !((i & 1) ^ (now[j] & 1)))
                one++;
            // now[j]が0 or 1，かつ，i, now[j]が共に偶数 or 奇数の場合
            else if (!(now[j] & 2) && !((i & 1) ^ (now[j] & 1)))
                zero++;
        }
        // 全ての読み取りにおいて，i, now[j]が偶数，奇数が一致しないため，0にして打ち切り
        if (zero + one == 0)
        {
            ans[i] = '0';
            continue;
        }
        // 多数決で0 or 1なのでansは0
        if (zero >= one)
            ans[i] = '0', wrong = 1;
        // 多数決で2 or 3なのでansは1
        else
            ans[i] = '1', wrong = 0;

        // 完全一致の場合，すべてのポイントを上げる
        if (zero == N || one == N)
        {
            rep(j, N) pt[j]++;
            continue;
        }
        int fail[N] = {};
        int cut = 0;
        // ポイント調整
        rep(j, M + 2)
        {
            int n[5] = {};
            rep(k, N)
            {
                if (S[k][pt[k] + j] == 4)
                {
                    n[4]++;
                    continue;
                }
                if (fail[k])
                    continue;
                // 多数決で0 or 1の場合
                if (wrong)
                {
                    if (now[k] >= 2)
                    {
                        fail[k] = 1;
                        continue;
                    }
                    // インデックスとその値の偶奇が一致していなければ足切り
                    if (((S[k][pt[k] + j]) & 1) ^ ((i + j) & 1))
                    {
                        fail[k] = 1;
                        continue;
                    }
                    n[S[k][pt[k] + j]]++;
                }
                // 多数決で2 or 3の場合
                else
                {
                    if (now[k] < 2)
                    {
                        fail[k] = 1;
                        continue;
                    }
                    // インデックスとその値の偶奇が一致していなければ足切り
                    if (((S[k][pt[k] + j]) & 1) ^ ((i + j) & 1))
                    {
                        fail[k] = 1;
                        continue;
                    }
                    n[S[k][pt[k] + j]]++;
                }
            }
            int plc = 0, maxi = 0;
            rep(k, 5) if (n[k] > maxi) plc = k, maxi = n[k];
            // 空列(4)のみの場合，cutに差分を保存して足切り
            if (maxi == 0 && j > 2)
            {
                cut = j;
                break;
            }
            // 最も多い値を保存
            base[j] = plc;
        }
        // 多数決
        rep(j, N)
        {
            if (now[j] == 4)
                continue;
            // 差分多数決で決定した文字列
            unsigned char *p = base;
            // 多数決で0 or 1の場合
            if (wrong)
            {
                // 一致
                if (now[j] < 2 && !((now[j] & 1) ^ (i & 1)))
                    continue;
                unsigned char *p2 = &S[j][pt[j] + 1];
                int diff[4];
                // 差分が0文字以外(cut-3をmとする)
                if (cut)
                {
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j+1 ~ j+m
                    if ((diff[0] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j]++;
                        continue;
                    }
                    p += 1, p2 -= 1;
                    // 比較
                    // 多数決文字列: j+1 ~ j+m
                    // デコード文字列: j ~ j+m-1
                    if ((diff[1] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j]--;
                        continue;
                    }
                    p--;
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j ~ j+m-1
                    if (edis2(p, p2, cut - 3) == 1)
                        continue;
                    p2 += 2;
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j+2 ~ j+m+1
                    if ((diff[2] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j] += 2;
                        continue;
                    }
                    p += 2, p2 -= 2;
                    // 比較
                    // 多数決文字列: j+2 ~ j+m+1
                    // デコード文字列: j ~ j+m-1
                    if ((diff[3] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j] -= 2;
                        continue;
                    }
                }
                // 差分が0文字
                else
                {
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j+1 ~ j+M
                    if ((diff[0] = edis(p, p2)) == 0)
                    {
                        pt[j]++;
                        continue;
                    }
                    p += 1, p2 -= 1;
                    // 比較
                    // 多数決文字列: j+1 ~ j+M
                    // デコード文字列: j ~ j+M-1
                    if ((diff[1] = edis(p, p2)) == 0)
                    {
                        pt[j]--;
                        continue;
                    }
                    p--;
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j ~ j+M-1
                    if (edis(p, p2) == 1)
                        continue;
                    p2 += 2;
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j+2 ~ j+M+1
                    if ((diff[2] = edis(p, p2)) == 0)
                    {
                        pt[j] += 2;
                        continue;
                    }
                    p += 2, p2 -= 2;
                    // 比較
                    // 多数決文字列: j+2 ~ j+M+1
                    // デコード文字列: j ~ j+M-1
                    if ((diff[3] = edis(p, p2)) == 0)
                    {
                        pt[j] -= 2;
                        continue;
                    }
                }
                // 完全に一致しなかった場合，最も小さいものでポイント調整
                int mini = M, use = 0;
                rep(k, 4) if (mini > diff[k]) mini = diff[k], use = k;
                if (use == 0)
                    pt[j]++;
                else if (use == 1)
                    pt[j]--;
                else if (use == 2)
                    pt[j] += 2;
                else
                    pt[j] -= 2;
            }
            // 多数決で2 or 3の場合
            else
            {
                // 一致
                if (now[j] >= 2 && !((now[j] & 1) ^ (i & 1)))
                    continue;
                unsigned char *p2 = &S[j][pt[j] + 1];
                int diff[4];
                // 差分が0文字以外(cut-3をmとする)
                if (cut)
                {
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j+1 ~ j+m
                    if ((diff[0] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j]++;
                        continue;
                    }
                    p += 1, p2 -= 1;
                    // 比較
                    // 多数決文字列: j+1 ~ j+m
                    // デコード文字列: j ~ j+m-1
                    if ((diff[1] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j]--;
                        continue;
                    }
                    p--;
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j ~ j+m-1
                    if (edis2(p, p2, cut - 3) == 1)
                        continue;
                    p2 += 2;
                    // 比較
                    // 多数決文字列: j ~ j+m-1
                    // デコード文字列: j+2 ~ j+m+1
                    if ((diff[2] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j] += 2;
                        continue;
                    }
                    p += 2, p2 -= 2;
                    // 比較
                    // 多数決文字列: j+2 ~ j+m+1
                    // デコード文字列: j ~ j+m-1
                    if ((diff[3] = edis2(p, p2, cut - 3)) == 0)
                    {
                        pt[j] -= 2;
                        continue;
                    }
                }
                // 差分が0文字
                else
                {
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j+1 ~ j+M
                    if ((diff[0] = edis(p, p2)) == 0)
                    {
                        pt[j]++;
                        continue;
                    }
                    p += 1, p2 -= 1;
                    // 比較
                    // 多数決文字列: j+1 ~ j+M
                    // デコード文字列: j ~ j+M-1
                    if ((diff[1] = edis(p, p2)) == 0)
                    {
                        pt[j]--;
                        continue;
                    }
                    p--;
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j ~ j+M-1
                    if (edis(p, p2) == 1)
                        continue;
                    p2 += 2;
                    // 比較
                    // 多数決文字列: j ~ j+M-1
                    // デコード文字列: j+2 ~ j+M+1
                    if ((diff[2] = edis(p, p2)) == 0)
                    {
                        pt[j] += 2;
                        continue;
                    }
                    p += 2, p2 -= 2;
                    // 比較
                    // 多数決文字列: j+2 ~ j+M+1
                    // デコード文字列: j ~ j+M-1
                    if ((diff[3] = edis(p, p2)) == 0)
                    {
                        pt[j] -= 2;
                        continue;
                    }
                    // 完全に一致しなかった場合，最も小さいものでポイント調整
                    int mini = M, use = 0;
                    rep(k, 4) if (mini > diff[k]) mini = diff[k], use = k;
                    if (use == 0)
                        pt[j]++;
                    else if (use == 1)
                        pt[j]--;
                    else if (use == 2)
                        pt[j] += 2;
                    else
                        pt[j] -= 2;
                }
            }
        }
        // 次のインデックスへ
        rep(j, N)
        {
            pt[j]++;
            // 範囲外参照防止用
            if (pt[j] >= 201900)
                pt[j] = 200500;
        }
    }
    // 最後の部分のデコード
    int last[4] = {};
    // 多数決
    rep(i, N) if (S[i][pt[i]] != 4) last[S[i][pt[i]]]++;
    int ls = 0, maxi = 0;
    // 多数決で決定
    rep(i, 4) if (maxi < last[i]) maxi = last[i], ls = i;
    // 偶奇で判定
    char c2 = ls & 1;
    ls /= 2;
    // ls >= 2 or not で判定
    char c1 = ls & 1;
    ans[199998] = '0' + c1;
    ans[199999] = '0' + c2;
    ans[200000] = '\n';
    fputs(ans, dfp);
    fclose(sfp);
    fclose(dfp);
    return (0);
}

int main()
{
    dec();
    return (0);
}
