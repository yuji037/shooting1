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
	int x;             /* x�����̃s�N�Z���� */
	int y;             /* y�����̃s�N�Z���� */
	unsigned char *r;  /* R�v�f�̋P�x(0�`255) */
	unsigned char *g;  /* G�v�f�̋P�x(0�`255) */
	unsigned char *b;  /* B�v�f�̋P�x(0�`255) */
	unsigned char *a;  /* ���l(�����x)(0�`255) */
					   /* �s�N�Z���̃f�[�^�͍��ォ��E���։������ɑ��삵�����Ŋi�[ */
}Picture;

/* �\���̃������m�ہ��������֐� */
Picture* newPicture() {
	Picture *pPic;
	pPic = (Picture*)malloc(sizeof(Picture));
	if( pPic == NULL ) {
		return NULL;                  /* �������m�ۂł��Ȃ� */
	}
	pPic->x = 0;
	pPic->y = 0;
	pPic->r = NULL;
	pPic->g = NULL;
	pPic->b = NULL;
	pPic->a = NULL;
	return pPic;
}
/* �\���̃���������֐� */
void deletePicture(Picture *pPic) {
	free(pPic->r);
	free(pPic->g);
	free(pPic->b);
	free(pPic->a);
	free(pPic);
	pPic = NULL;
}

/***********************************************************************/
/*** BMP�t�@�C���擾�֐�                                             ***/
/*** �Ή�����`���FWindows Bit Map�`���A                             ***/
/*** 24bit�J���[����уC���f�b�N�X�J���[�`���A                       ***/
/*** �g�b�v�_�E���ɑΉ��ARLE���k�ɂ͖��Ή�                           ***/
/*** �t�@�C���|�C���^�������ɗ^���ēǂݏo���B                        ***/
/*** �摜�f�[�^�͓����Ń������m�ۂ��A�߂�l�Ƃ��Ă��̃|�C���^��Ԃ��B***/
/*** ���炩�̃G���[�����������ꍇNULL��Ԃ��B                        ***/
/***********************************************************************/
Picture* getBmp(FILE* fp) {
	int          i, j;                  /* ���[�v�p�ϐ�                   */
	int          p;                    /* �J���[�p���b�g�ԍ�             */
	int          x, y;                  /* x���Ay�������̉�f��           */
	int          start_y, stop_y, sign_y;/* ���������̈Ⴂ���L�^           */
	int          pad;                  /* 32bit���E�̂��߂̃p�f�B���O    */
	int          mask, shift;           /* 8bit�ȉ��̒l�����o�����ɗ��p */
	int          palsize;              /* �J���[�p���b�g�̃T�C�Y         */
	int          imgsize;              /* �C���[�W�̃T�C�Y               */
	int          colbit;               /* 1��f������̃r�b�g��          */
	unsigned char *buf, *buf_top;       /* �摜�f�[�^�Ƃ��̐擪�|�C���^   */
	Picture       *pPic;               /* �߂�l                         */
	BMPFILEHEADER bf;                  /* �t�@�C���w�b�_                 */
	BMPINFOHEADER bi;                  /* �t�@�C���w�b�_                 */
	RGBQUAD       *rgbq;               /* �J���[�p���b�g                 */
	if( fp == NULL )                     /* �����ُ�                       */
		return NULL;
	/* ------------------------- �w�b�_�擾 ---------------------------- */
	if( fread((void*)&bf, sizeof(BMPFILEHEADER), 1, fp) != 1 )
		return NULL;                     /* �ǂݍ��ݎ��s                   */
	if( fread((void*)&bi, sizeof(BMPINFOHEADER), 1, fp) != 1 )
		return NULL;                     /* �ǂݍ��ݎ��s                   */
	if( bf.bfType != *( WORD* )"BM" )
		return NULL;                     /* �t�@�C���`�����Ⴄ             */
	if( bi.biSize != sizeof(BMPINFOHEADER) )
		return NULL;                     /* �Ή����Ă��Ȃ��`��             */
	if( bi.biCompression != 0 )
		return NULL;                     /* ���k����Ă���(���Ή�)         */
	palsize = bf.bfOffBits             /* �p���b�g�̃T�C�Y               */
		- sizeof(BMPFILEHEADER)
		- sizeof(BMPINFOHEADER);
	imgsize = bi.biSizeImage;          /* �C���[�W�T�C�Y                 */
	x = bi.biWidth;              /* �C���[�W�̕�                   */
	y = bi.biHeight;             /* �C���[�W�̍���                 */
	colbit = bi.biBitCount;           /* �P�F������̃r�b�g��           */
	if( y < 0 ) {                         /* y�̐����ɂ���đ���������ݒ�  */
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
	pad = ( x * colbit + 31 ) / 32 * 4   /* 32bit���E�����ɂ����          */
		- ( x * colbit + 7 ) / 8;       /* �p�f�B���O�����o�C�g�����v�Z */
	mask = ( 1 << colbit ) - 1;          /* �w��r�b�g���̃}�X�N           */
										 /* ------------------------- �w�b�_�̖������m�F -------------------- */
	if( colbit != 24 &&
		palsize != sizeof(RGBQUAD) * ( 1 << colbit ) )
		return NULL;                     /* �p���b�g�T�C�Y�ԈႢ           */
	if( colbit == 0 )
		return NULL;                     /* �r�b�g�����ُ�                 */
	if( x <= 0 || y <= 0 )
		return NULL;                     /* �T�C�Y���ُ�                   */
	if( imgsize != bf.bfSize - bf.bfOffBits )
		return NULL;                     /* �C���[�W�T�C�Y�ԈႢ           */
	if( imgsize != ( x * colbit + 31 ) / 32 * 4 * y )
		return NULL;                     /* �C���[�W�T�C�Y�ԈႢ           */
										 /* ------------------------- �J���[�p���b�g�擾 -------------------- */
	if( colbit == 24 )
		rgbq = NULL;
	else {
		rgbq = (RGBQUAD*)malloc(palsize);
		if( rgbq == NULL )
			return NULL;                   /* �������m�ێ��s                 */
		if( fread((void*)rgbq, 1, palsize, fp) != palsize ) {
			free(rgbq);
			return NULL;                   /* �ǂݍ��ݎ��s                   */
		}
	}
	/* ------------------------- �C���[�W�f�[�^�擾 -------------------- */
	buf_top = buf = (unsigned char*)malloc(imgsize);
	if( buf == NULL ) {
		free(rgbq);
		return NULL;                     /* �������m�ێ��s                 */
	}
	if( fread((void*)buf, 1, imgsize, fp) != imgsize ) {
		free(rgbq);
		free(buf_top);
		return NULL;                     /* �ǂݍ��ݎ��s                   */
	}
	/* ------------------------- �߂�l�̃������m�� -------------------- */
	pPic = newPicture();
	if( pPic == NULL ) {
		free(rgbq);
		free(buf_top);
		return NULL;                     /* �������m�ێ��s                 */
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
		return NULL;                     /* �������m�ێ��s                 */
	}
	/* ------------------------- �f�[�^�ϊ� ---------------------------- */
	if( colbit == 24 ) {                 /* �J���[�p���b�g���g�p���Ȃ��ꍇ */
		for( i = start_y ; i < stop_y ; i++ ) {
			for( j = 0 ; j < x ; j++ ) {
				pPic->b[ j + i * x * sign_y ] = *( buf++ );
				pPic->g[ j + i * x * sign_y ] = *( buf++ );
				pPic->r[ j + i * x * sign_y ] = *( buf++ );
			}
			buf += pad;                    /* �p�f�B���O��ǂݔ�΂�         */
		}
	}
	else {                             /* �J���[�p���b�g���g�p����ꍇ   */
		for( i = start_y ; i < stop_y ; i++ ) {
			shift = 8;                     /* �V�t�g�ʒu�̏�����             */
			for( j = 0 ; j < x ; j++ ) {
				shift -= colbit;
				p = *buf >> shift & mask;
				if( shift <= 0 ) {
					shift = 8;
					buf++;
				}                            /* �p���b�g����F����ǂݏo��   */
				pPic->b[ j + i * x * sign_y ] = rgbq[ p ].rgbBlue;
				pPic->g[ j + i * x * sign_y ] = rgbq[ p ].rgbGreen;
				pPic->r[ j + i * x * sign_y ] = rgbq[ p ].rgbRed;
			}
			if( shift != 8 )                 /* �p�f�B���O��ǂݔ�΂�         */
				buf++;
			buf += pad;                    /* �p�f�B���O��ǂݔ�΂�         */
		}
	}
	/* ------------------------- ���ׂĐ��� ---------------------------- */
	free(rgbq);
	free(buf_top);
	return pPic;
}

