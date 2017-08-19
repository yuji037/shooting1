#pragma once
#include <stdlib.h>
#include <stdio.h>

#pragma pack(1)
typedef struct BMPFILEHEADER {
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
}BMPFILEHEADER;
typedef struct BMPINFOHEADER {
	DWORD biSize;
	LONG  biWidth;
	LONG  biHeight;
	WORD  biPlanes;
	WORD  biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG  biXPelsPerMeter;
	LONG  biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
}BMPINFOHEADER;
/*
typedef struct RGBQUAD {
	BYTE  rgbBlue;
	BYTE  rgbGreen;
	BYTE  rgbRed;
	BYTE  rgbReserved;
}RGBQUAD;
*/
#pragma pack()

typedef struct _Picture
{
	int x;             /* x方向のピクセル数 */
	int y;             /* y方向のピクセル数 */
	unsigned char *r;  /* R要素の輝度(0～255) */
	unsigned char *g;  /* G要素の輝度(0～255) */
	unsigned char *b;  /* B要素の輝度(0～255) */
	unsigned char *a;  /* α値(透明度)(0～255) */
					   /* ピクセルのデータは左上から右下へ横方向に操作した順で格納 */
}Picture;

/* 構造体メモリ確保＆初期化関数 */
Picture* newPicture() {
	Picture *pPic;
	pPic = (Picture*)malloc(sizeof(Picture));
	if( pPic == NULL ) {
		return NULL;                  /* メモリ確保できない */
	}
	pPic->x = 0;
	pPic->y = 0;
	pPic->r = NULL;
	pPic->g = NULL;
	pPic->b = NULL;
	pPic->a = NULL;
	return pPic;
}
/* 構造体メモリ解放関数 */
void deletePicture(Picture *pPic) {
	free(pPic->r);
	free(pPic->g);
	free(pPic->b);
	free(pPic->a);
	free(pPic);
	pPic = NULL;
}

/***********************************************************************/
/*** BMPファイル取得関数                                             ***/
/*** 対応する形式：Windows Bit Map形式、                             ***/
/*** 24bitカラーおよびインデックスカラー形式、                       ***/
/*** トップダウンに対応、RLE圧縮には未対応                           ***/
/*** ファイルポインタを引数に与えて読み出す。                        ***/
/*** 画像データは内部でメモリ確保し、戻り値としてそのポインタを返す。***/
/*** 何らかのエラーが発生した場合NULLを返す。                        ***/
/***********************************************************************/
Picture* getBmp(FILE* fp) {
	int          i, j;                  /* ループ用変数                   */
	int          p;                    /* カラーパレット番号             */
	int          x, y;                  /* x軸、y軸方向の画素数           */
	int          start_y, stop_y, sign_y;/* 走査方向の違いを記録           */
	int          pad;                  /* 32bit境界のためのパディング    */
	int          mask, shift;           /* 8bit以下の値を取り出す時に利用 */
	int          palsize;              /* カラーパレットのサイズ         */
	int          imgsize;              /* イメージのサイズ               */
	int          colbit;               /* 1画素あたりのビット数          */
	unsigned char *buf, *buf_top;       /* 画像データとその先頭ポインタ   */
	Picture       *pPic;               /* 戻り値                         */
	BMPFILEHEADER bf;                  /* ファイルヘッダ                 */
	BMPINFOHEADER bi;                  /* ファイルヘッダ                 */
	RGBQUAD       *rgbq;               /* カラーパレット                 */
	if( fp == NULL )                     /* 引数異常                       */
		return NULL;
	/* ------------------------- ヘッダ取得 ---------------------------- */
	if( fread((void*)&bf, sizeof(BMPFILEHEADER), 1, fp) != 1 )
		return NULL;                     /* 読み込み失敗                   */
	if( fread((void*)&bi, sizeof(BMPINFOHEADER), 1, fp) != 1 )
		return NULL;                     /* 読み込み失敗                   */
	if( bf.bfType != *( WORD* )"BM" )
		return NULL;                     /* ファイル形式が違う             */
	if( bi.biSize != sizeof(BMPINFOHEADER) )
		return NULL;                     /* 対応していない形式             */
	if( bi.biCompression != 0 )
		return NULL;                     /* 圧縮されている(未対応)         */
	palsize = bf.bfOffBits             /* パレットのサイズ               */
		- sizeof(BMPFILEHEADER)
		- sizeof(BMPINFOHEADER);
	imgsize = bi.biSizeImage;          /* イメージサイズ                 */
	x = bi.biWidth;              /* イメージの幅                   */
	y = bi.biHeight;             /* イメージの高さ                 */
	colbit = bi.biBitCount;           /* １色あたりのビット数           */
	if( y < 0 ) {                         /* yの正負によって走査方向を設定  */
		y = -y;
		start_y = 0;
		stop_y = y;
		sign_y = 1;
	}
	else {
		start_y = -y + 1;
		stop_y = 1;
		sign_y = -1;
	}
	pad = ( x * colbit + 31 ) / 32 * 4   /* 32bit境界条件によって          */
		- ( x * colbit + 7 ) / 8;       /* パディングされるバイト数を計算 */
	mask = ( 1 << colbit ) - 1;          /* 指定ビット数のマスク           */
										 /* ------------------------- ヘッダの矛盾を確認 -------------------- */
	if( colbit != 24 &&
		palsize != sizeof(RGBQUAD) * ( 1 << colbit ) )
		return NULL;                     /* パレットサイズ間違い           */
	if( colbit == 0 )
		return NULL;                     /* ビット数が異常                 */
	if( x <= 0 || y <= 0 )
		return NULL;                     /* サイズが異常                   */
	if( imgsize != bf.bfSize - bf.bfOffBits )
		return NULL;                     /* イメージサイズ間違い           */
	if( imgsize != ( x * colbit + 31 ) / 32 * 4 * y )
		return NULL;                     /* イメージサイズ間違い           */
										 /* ------------------------- カラーパレット取得 -------------------- */
	if( colbit == 24 )
		rgbq = NULL;
	else {
		rgbq = (RGBQUAD*)malloc(palsize);
		if( rgbq == NULL )
			return NULL;                   /* メモリ確保失敗                 */
		if( fread((void*)rgbq, 1, palsize, fp) != palsize ) {
			free(rgbq);
			return NULL;                   /* 読み込み失敗                   */
		}
	}
	/* ------------------------- イメージデータ取得 -------------------- */
	buf_top = buf = (unsigned char*)malloc(imgsize);
	if( buf == NULL ) {
		free(rgbq);
		return NULL;                     /* メモリ確保失敗                 */
	}
	if( fread((void*)buf, 1, imgsize, fp) != imgsize ) {
		free(rgbq);
		free(buf_top);
		return NULL;                     /* 読み込み失敗                   */
	}
	/* ------------------------- 戻り値のメモリ確保 -------------------- */
	pPic = newPicture();
	if( pPic == NULL ) {
		free(rgbq);
		free(buf_top);
		return NULL;                     /* メモリ確保失敗                 */
	}
	pPic->x = x;
	pPic->y = y;
	pPic->r = (unsigned char*)malloc(sizeof(unsigned char) * x * y);
	pPic->g = (unsigned char*)malloc(sizeof(unsigned char) * x * y);
	pPic->b = (unsigned char*)malloc(sizeof(unsigned char) * x * y);
	if( pPic->r == NULL || pPic->b == NULL || pPic->g == NULL ) {
		deletePicture(pPic);
		free(rgbq);
		free(buf_top);
		return NULL;                     /* メモリ確保失敗                 */
	}
	/* ------------------------- データ変換 ---------------------------- */
	if( colbit == 24 ) {                 /* カラーパレットを使用しない場合 */
		for( i = start_y ; i < stop_y ; i++ ) {
			for( j = 0 ; j < x ; j++ ) {
				pPic->b[ j + i * x * sign_y ] = *( buf++ );
				pPic->g[ j + i * x * sign_y ] = *( buf++ );
				pPic->r[ j + i * x * sign_y ] = *( buf++ );
			}
			buf += pad;                    /* パディングを読み飛ばす         */
		}
	}
	else {                             /* カラーパレットを使用する場合   */
		for( i = start_y ; i < stop_y ; i++ ) {
			shift = 8;                     /* シフト位置の初期化             */
			for( j = 0 ; j < x ; j++ ) {
				shift -= colbit;
				p = *buf >> shift & mask;
				if( shift <= 0 ) {
					shift = 8;
					buf++;
				}                            /* パレットから色情報を読み出す   */
				pPic->b[ j + i * x * sign_y ] = rgbq[ p ].rgbBlue;
				pPic->g[ j + i * x * sign_y ] = rgbq[ p ].rgbGreen;
				pPic->r[ j + i * x * sign_y ] = rgbq[ p ].rgbRed;
			}
			if( shift != 8 )                 /* パディングを読み飛ばす         */
				buf++;
			buf += pad;                    /* パディングを読み飛ばす         */
		}
	}
	/* ------------------------- すべて成功 ---------------------------- */
	free(rgbq);
	free(buf_top);
	return pPic;
}

