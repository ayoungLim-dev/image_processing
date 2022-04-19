#include <stdio.h>
#include <windows.h>  // BITMAPFILEHEADER, etc.

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 4바이트의 배수여야 함
#define BYTE unsigned char

void gray_Image();
void color_Image();

void main() {
	//gray_Image();
	color_Image();
}

void gray_Image() {

	errno_t err;
	FILE* outfile_s;

	BITMAPFILEHEADER hf; // "파일정보헤드" 변수 선언
	hf.bfType = (WORD) 0x4D42;
	hf.bfSize = (DWORD)326;
	hf.bfReserved1 = (WORD)0;
	hf.bfReserved2 = (WORD)0;
	hf.bfOffBits = (DWORD)310;

	if (hf.bfType != 0x4D42) exit(1); // 파일타입이 BM인지 검사

	BITMAPINFOHEADER hInfo; // "영상정보헤드" 변수 선언
	hInfo.biSize = (DWORD)40;
	hInfo.biWidth = (LONG)256;
	hInfo.biHeight = (LONG)256;
	hInfo.biPlanes = (WORD)1;
	hInfo.biBitCount = (WORD)8;
	hInfo.biCompression = (DWORD)0;
	hInfo.biSizeImage = (DWORD)256*256;
	hInfo.biXPelsPerMeter = (LONG)2816;
	hInfo.biYPelsPerMeter = (LONG)2816;
	hInfo.biClrUsed = (DWORD)256;
	hInfo.biClrImportant = (DWORD)256;

	printf("Pallet Type : %dbit", hInfo.biBitCount); // 8bit


	// 팔레트 정보의 입력
	RGBQUAD hRGB[256];
	for (int i = 0; i < 256; i++) { //for문을 이용해 hRGB 배열 원소에 rgbRED,GREEN,BLUE 값 할당
		hRGB[i].rgbRed = (BYTE)i;
		hRGB[i].rgbGreen = (BYTE)i;
		hRGB[i].rgbBlue = (BYTE)i;
		hRGB[i].rgbReserved = (BYTE)0;

	}
	// 메모리 할당
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // 저장할 영상메모리 할당

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	unsigned char OrgImg[256][256]; // 원, 사각형 그릴 배열 선언

	// 명도값이 다른 원
	for (int i = 0; i < 256; i++) {

		for (int j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // 반지름이 40 미만일 경우 - 명도 값 0 *black
				OrgImg[i][j] = 0;	
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // 반지름이 40 이상, 80 미만일 경우 - 명도 값 64 *dark gray
				OrgImg[i][j] = 64;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // 반지름이 80 이상, 120 미만일 경우 - 명도 값 128 *light gray
				OrgImg[i][j] = 128;
			}
			else {// 반지름이 120 이상일 경우 - 명도 값 255 *white
				OrgImg[i][j] = 200;
			}
		}
	}

	//좌측 상단 검은 정사각형 32x32
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			OrgImg[i][j] = 0;
		}
	}

	// 이미지 반전
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * hInfo.biWidth + j] = OrgImg[hInfo.biHeight-i-1][j];
		}
	}


	// 영상 출력
	err = fopen_s(&outfile_s, "sample gray image.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER) ,outfile_s);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile_s);
	fwrite(lpImg, sizeof(char),256*256, outfile_s);
	fclose(outfile_s);

	// 메모리 해제
	delete[]lpImg;

}//void

void color_Image() {

	errno_t err;
	FILE* outfile_s;

	BITMAPFILEHEADER hf; // "파일정보헤드" 변수 선언
	hf.bfType = (WORD)0x4D42;
	hf.bfSize = (DWORD)1572888;
	hf.bfReserved1 = (WORD)0;
	hf.bfReserved2 = (WORD)0;
	hf.bfOffBits = (DWORD)54;

	if (hf.bfType != 0x4D42) exit(1); // 파일타입이 BM인지 검사

	BITMAPINFOHEADER hInfo; // "영상정보헤드" 변수 선언
	hInfo.biSize = (DWORD)40;
	hInfo.biWidth = (LONG)256;
	hInfo.biHeight = (LONG)256;
	hInfo.biPlanes = (WORD)1;
	hInfo.biBitCount = (WORD)24;
	hInfo.biCompression = (DWORD)0;
	hInfo.biSizeImage = (DWORD)24 * 256 * 256;
	hInfo.biXPelsPerMeter = (LONG)0;
	hInfo.biYPelsPerMeter = (LONG)0;
	hInfo.biClrUsed = (DWORD)0;
	hInfo.biClrImportant = (DWORD)0;

	printf("Pallet Type : %dbit", hInfo.biBitCount);

	int rwsize = WIDTHBYTES(24 * hInfo.biWidth);
	//int rwsize2 = WIDTHBYTES(24 * hInfo.biWidth);
	// 
	// 메모리 할당
	BYTE* lpImg = new BYTE[rwsize * hInfo.biHeight]; // 저장할 영상메모리 할당

	unsigned char C0[256][256]; // C0 // (아직 데이터를 집어넣진 않았지만) R 채널로 고정
	unsigned char C1[256][256]; // G 채널로 고정
	unsigned char C2[256][256]; // B 채널로 고정

	// 팔레트 정보의 입력
	RGBQUAD* hRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		hRGB = new RGBQUAD[hInfo.biClrUsed]; // 팔레트의 크기만큼 메모리를 할당함
	}
	int index, R, G, B;

	// 컬러값이 다른 원
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // 반지름이 40 미만일 경우 - B
				C0[i][j] = 0;
				C1[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // 반지름이 40 이상, 80 미만일 경우 - G
				C0[i][j] = 0;
				C2[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // 반지름이 80 이상, 120 미만일 경우 - R
				C1[i][j] = 0;
				C2[i][j] = 0;
			}
			else {// 반지름이 120 이상일 경우 - 명도 값 200
				C0[i][j] = 200;
				C1[i][j] = 200;
				C2[i][j] = 200;
			}
		}
	}


	//좌측 상단 검은 정사각형 32x32
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			C0[i][j] = 0;
			C1[i][j] = 0;
			C2[i][j] = 0;
		}
	}

	if (hInfo.biBitCount == 24) // 만일 입력 영상이 트루(24비트) 컬러인 경우엔 팔레트 없음
		for (int i = 0; i < hInfo.biHeight; i++) {
			for (int j = 0; j < hInfo.biWidth; j++) {
				// 팔레트가 없으므로 영상데이터가 바로 컬러값
				lpImg[i * rwsize + 3 * j + 2] = C0[i][j];
				lpImg[i * rwsize + 3 * j + 1] = C1[i][j];
				lpImg[i * rwsize + 3 * j] = C2[i][j];
			}
		}
	else // 트루컬러가 아닌 경우에는 팔레트 있음

		for (int i = 0; i < hInfo.biHeight; i++) {	// 역상의 이미지 구하기
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = hRGB[index].rgbRed; // 팔레트에서 실제 영상 데이터를 가져옴(R)
				G = hRGB[index].rgbGreen; // 팔레트에서 실제 영상 데이터를 가져옴(G)
				B = hRGB[index].rgbBlue; // 팔레트에서 실제 영상 데이터를 가져옴(B)

				C0[i][j] = (BYTE)R;
				C1[i][j] = (BYTE)G;
				C2[i][j] = (BYTE)B;

			}
		}

	// 이미지 반전
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * rwsize + 3 * j + 2] = C0[hInfo.biHeight - 1 - i][j];
			lpImg[i * rwsize + 3 * j + 1] = C1[hInfo.biHeight - 1 - i][j];
			lpImg[i * rwsize + 3 * j] = C2[hInfo.biHeight - 1 - i][j];
		}
	}


	// 영상 출력
	err = fopen_s(&outfile_s, "sample color image.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s);
	fwrite(lpImg, sizeof(char), rwsize*hInfo.biHeight, outfile_s);
	fclose(outfile_s);

	// 메모리 해제
	if (hInfo.biClrUsed != 0) delete[]hRGB;
	delete[]lpImg;
 }