#include <stdio.h>

int Negative_Image();
int GrayScale_Image();
int Color_Image();

int main() {
	Negative_Image();
	//GrayScale_Image();
	//Color_Image();
}

/* 과제1 negative image */
int Negative_Image() {
	int i, j;
	unsigned char OrgImg[256][256];
	//unsigned char = 8bits = 256

	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;


	//디스크에서 영상데이터 읽기

	err = fopen_s(&infile_s, "Coin.raw", "rb");

	if (err != 0)
	{
		printf("File open error!"); return -1;
	}

	fread(OrgImg, sizeof(unsigned char), 256 * 256, infile_s);
	fclose(infile_s);

	//역상계산을 위한 영상처리
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			OrgImg[i][j] = 255 - OrgImg[i][j];

		}

	}

	//하드디스크에 영상데이터 쓰기
	err = fopen_s(&outfile_s, "Coin_inv.raw", "wb");
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s);
	fclose(outfile_s);

}


/* 과제2 | gray-scale 영상 생성 */
int GrayScale_Image(){
	int i, j;
	unsigned char OrgImg[256][256];
	//unsigned char = 8bits = 256

	errno_t err;
	//FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	//읽어 올 데이터 파일이 없으므로 fopen_s, fread 함수 작성 X

	// 명도값이 다른 원
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) // 반지름이 40 미만일 경우 - 명도 값 0 *black
				OrgImg[i][j] = 0;
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) // 반지름이 40 이상, 80 미만일 경우 - 명도 값 64 *dark gray
				OrgImg[i][j] = 64;
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) // 반지름이 80 이상, 120 미만일 경우 - 명도 값 128 *light gray
				OrgImg[i][j] = 128;
			else // 반지름이 120 이상일 경우 - 명도 값 255 *white
				OrgImg[i][j] = 255;
		}

	}

	//좌측 상단 검은 정사각형 32x32
	for (i = 0; i < 32; i++) {

		for (j = 0; j < 32; j++) {

			OrgImg[i][j] = 0;

		}

	}


	//하드디스크에 영상데이터 쓰기
	err = fopen_s(&outfile_s, "sample gray image.raw", "wb");
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s);
	fclose(outfile_s);

	//디스크에서 영상데이터 읽기
	if (err != 0)
	{
		printf("File open error!"); return -1;
	}

}

/* 과제 3 | color 영상 생성 */

int Color_Image(){
	int i, j;
	unsigned char OrgImg[256][256]; // C0 // (아직 데이터를 집어넣진 않았지만) R 채널로 고정
	unsigned char C1[256][256]; // G 채널로 고정
	unsigned char C2[256][256]; // B 채널로 고정
	//unsigned char = 8bits = 256

	errno_t err;
	//FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	//읽어 올 데이터 파일이 없으므로 fopen_s, fread 함수 작성 X



	// 명도값이 다른 원
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // 반지름이 40 미만일 경우 - R, G 채널 값 0 *blue
				OrgImg[i][j] = 0;
				C1[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // 반지름이 40 이상, 80 미만일 경우 - R, B 채널 값 0 *green
				OrgImg[i][j] = 0;
				C2[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // 반지름이 80 이상, 120 미만일 경우 - G, B 채널 값 0 *red
				C1[i][j] = 0;
				C2[i][j] = 0;
			}
			else{ // 반지름이 120 이상일 경우 - R, G, B 채널 값 255 *white
				OrgImg[i][j] = 255;
				C1[i][j] = 255;
				C2[i][j] = 255;
			}

		}

	}

	//좌측 상단 검은 정사각형 32x32
	for (i = 0; i < 32; i++) {

		for (j = 0; j < 32; j++) {

			OrgImg[i][j] = 0;
			C1[i][j] = 0;
			C2[i][j] = 0;
			// RGB 채널 모두 0이어야 검정

		}

	}


	//하드디스크에 영상데이터 쓰기
	err = fopen_s(&outfile_s, "sample color image.raw", "wb");

	// 미리 고정해놓은 채널 순서대로 데이터 읽기
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s); // R channel
	fwrite(C1, sizeof(unsigned char), 256 * 256, outfile_s); // G channel
	fwrite(C2, sizeof(unsigned char), 256 * 256, outfile_s); // B channel

	fclose(outfile_s);

	//디스크에서 영상데이터 읽기
	if (err != 0)
	{
		printf("File open error!"); return -1;
	}
}



