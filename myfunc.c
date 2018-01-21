#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdbool.h>
#include<conio.h>
typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef signed char CHAR;
typedef signed short SHORT;
typedef signed long LONG;
#define NUMOF(ary) (sizeof(ary) / sizeof(ary[0]))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
/*==================================================*/
// キー入力関数
// key_input()
// 引数 - dst(out):     受信データ出力先
//      - siz(in):      dstのサイズ
// 戻り値 - true:１行入力終了 or サイズ最大まで入力, false:入力未完了
static bool key_input(char* dst, long siz)
{
    bool ret = false;
    static int cnt = 0;
    static char line[1000] = { 0 };

    // キー入力確認
    while (_kbhit() != 0)
    {
        // 入力キー取得
        line[cnt] = _getche(); 
        int min_siz = min(sizeof(line), siz);
        // キーボードからの改行[CR] or バッファオーバーなら入力完了とする
        if ((line[cnt] == '\r') || ((cnt + 2) >= min_siz))
        {
            line[cnt] = '\0';
            memcpy(dst, &line[0], min_siz);
            memset(&line[0], 0x00, sizeof(line));
            cnt = 0;
            ret = true;
        }
        else
        {
            cnt++;
        }
    }
    return ret;
}
/*==================================================*/
// バイナリデータ出力関数
// key_input()
// 引数 - dt(in):     出力するデータ
//      - siz(in):    dtのサイズ
// 戻り値 - なし
void put_binary(const UCHAR* dt, long siz)
{

    // データ長出力
    printf("Length=%dbyte.\n", siz);

    // 16(0x10)文字ずつを１行に出力
    for (int i = 0; i < siz; i += 0x10)
    {

        // 現在出力バイト数
        printf("%04X | ", i);
        // binaryデータ出力
        for (int k = 0; k < 0x10; k++)
        {
            // データありなら
            if (i + k < siz)
            {
                printf("%02X ", dt[i + k]);
            }
            // データなしなら空白
            else
            {
                     // 012 
                printf("   ");
            }
        }
        printf("|");

        // 表示文字出力
        for (int k = 0; k < 0x10; k++)
        {
            UCHAR ch = 
                (i + k >= siz)      ? ' ' :        // データなしなら空白出力
                isprint(dt[i + k])  ? dt[i + k] :  // 表示文字出力、 
                                     '.';         // 表示文字以外は.と出力
            printf("%c", ch); 

        }
    }
}




/******************************************************************************/
/*! @brief  配列を逆に並び替える
 *
 *  配列を逆向きに並び替える。
 *
 *  @param[in]  dat          配列の先頭アドレス
 *  @param[in]  num          並び替える配列数
 *  @param[in]  item_siz     配列1要素のサイズ
 */
void reverse_array(UCHAR *dat, ULONG num, ULONG item_siz)
{
    UCHAR temp;
    ULONG i, frontcnt, backcnt;
    UCHAR *front, *back;

    for(frontcnt = 0, backcnt = num - 1; frontcnt < backcnt; frontcnt++, backcnt--)
    {
        front = &dat[frontcnt * item_siz];
        back  = &dat[backcnt  * item_siz];
        for(i=0; i < item_siz; i++)
        {
            temp     = front[i];
            front[i] = back[i];
            back[i]  = temp;
        }
    }
}
/******************************************************************************/
/*! @brief  リングバッファ配列ソート
 *
 *  [x+1件目, …, 最新N件名, 最古1件名, …, x件目]の並びである配列をソートする。
 *  降順ソート例:4th 5th 1st 2nd 3rd -> 5th 4th 1st 2nd 3rd -> 5th 4th 3rd 2nd 1st 
 *  昇順ソート例:降順ソートされた並びを逆に並べる
 *
 *  @param[in]  dat          配列の先頭アドレス
 *  @param[in]  idx_old      最古データのインデックス
 *  @param[in]  num          配列の数
 *  @param[in]  siz          配列のサイズ
 *  @param[in]  asc          ON:昇順(新しい順), OFF:降順(古い順)
 */
void sort_ringbuf(UCHAR *dat, ULONG idx_old, ULONG num, ULONG siz, bool asc)
{
    ULONG item_siz;
    
    /* 配列1要素のサイズ計算 */
    item_siz = siz / num;
    
    /* 降順に並び替える */
    reverse_array((UCHAR*)&dat[0],                  idx_old,       item_siz);
    reverse_array((UCHAR*)&dat[item_siz * idx_old], num - idx_old, item_siz);
    if(asc)
    {
        /* 昇順に並び替える */
        reverse_array((UCHAR*)&dat[0], num, item_siz);
    }
    else
    {
        /* 何もしない */
    }
}
