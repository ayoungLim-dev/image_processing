#include <stdio.h>
#include <windows.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 영상의 가로줄은 4바이트의 배수
#define BYTE unsigned char

void GrayInvert();
void ColorInvert();
void GreenInvert();

void main() {
	//GrayInvert();
	//ColorInvert();
	GreenInvert();
}

void GrayInvert() {
	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	err = fopen_s(&infile_s, "talent.bmp", "rb");
	if (err) { printf("There is no file!\n"); exit(1); }

	// BMP헤드 정보의 입력
	BITMAPFILEHEADER hf; // "파일정보헤드" 변수 선언
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // 파일정보헤드 읽음
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // 파일 타입이 "BM"(0x4D42)인지 검사

	BITMAPINFOHEADER hInfo; // "영상정보헤드" 변수 선언
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // 영상정보헤드 읽음
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	if (hInfo.biBitCount != 8) { // 흑백
		printf("Bad File format!"); exit(1);
	}

	// 팔레트 정보의 입력
	RGBQUAD hRGB[256]; // 팔레트 정보를 위한 배열(흑백파일)
	fread(hRGB, sizeof(RGBQUAD), 256, infile_s); //팔레트 입력

	// (영상 데이터를 저장할) 메모리 할당
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // 저장할 영상메모리 할당
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // 영상 데이터 읽음
	fclose(infile_s); // 오픈했던 파일 닫음

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	// 역상의 이미지 구하기
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * rwsize + j] = 255 - lpImg[i * rwsize + j];
		}
	}

	// 영상 출력
	err = fopen_s(&outfile_s, "Gray_Invert_OutImg.bmp", "wb"); // 출력할 파일 오픈
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // 파일헤드 출력
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // 영상헤드 출력
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile_s); // 팔레트 출력
	fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile_s); // 영상데이터 출력
	fclose(outfile_s); // 파일 닫기

	//메모리 해제
	delete[]lpImg;
}

void ColorInvert() {
	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	err = fopen_s(&infile_s, "pshop256.bmp", "rb");
	if (err) { printf("There is no file!\n"); exit(1); }

	// BMP헤드 정보의 입력
	BITMAPFILEHEADER hf; // "파일정보헤드" 변수 선언
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // 파일정보헤드 읽음
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // 파일 타입이 "BM"(0x4D42)인지 검사

	BITMAPINFOHEADER hInfo; // "영상정보헤드" 변수 선언
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // 영상정보헤드 읽음
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	// 256 컬러 이하의 경우는 취급하지 않음
	if (hInfo.biBitCount < 8) {
		printf("Bad File format!"); exit(1);
	}

	// 팔레트 정보의 입력
	RGBQUAD *pRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		pRGB = new RGBQUAD[hInfo.biClrUsed]; // 팔레트의 크기만큼 메모리를 할당함
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile_s); // 팔레트를 파일에서 읽음
	}

	// (영상 데이터를 저장할) 메모리 할당
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // 저장할 영상메모리 할당
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // 영상 데이터 읽음
	fclose(infile_s); // 오픈했던 파일 닫음

	// 크기 계산, 메모리 할당
	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int rwsize2 = WIDTHBYTES(24*hInfo.biWidth);
	BYTE* lpOutImg = new BYTE [rwsize2 * hInfo.biHeight];

	int index, R, G, B;

	if(hInfo.biBitCount==24) // 만일 입력 영상이 트루(24비트) 컬러인 경우엔 팔레트 없음
		for (int i = 0; i < hInfo.biHeight; i++) {	// 역상의 이미지 구하기
			for (int j = 0; j < hInfo.biWidth; j++) {
				// 팔레트가 없으므로 영상데이터가 바로 컬러값
				lpOutImg[i * rwsize2 + 3*j + 2] = 255 - lpImg[i * rwsize + 3*j + 2];
				lpOutImg[i * rwsize2 + 3*j + 1] = 255 - lpImg[i * rwsize + 3*j + 1];
				lpOutImg[i * rwsize2 + 3*j] = 255 - lpImg[i * rwsize + 3*j];
			}
		}
	else // 트루컬러가 아닌 경우에는 팔레트 있음

		for (int i = 0; i < hInfo.biHeight; i++) {	// 역상의 이미지 구하기
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = pRGB[index].rgbRed; // 팔레트에서 실제 영상 데이터를 가져옴(R)
				G = pRGB[index].rgbGreen; // 팔레트에서 실제 영상 데이터를 가져옴(G)
				B = pRGB[index].rgbBlue; // 팔레트에서 실제 영상 데이터를 가져옴(B)
				R = 255 - R; G = 255 - G; B = 255 - B; // 역상 계산
				lpOutImg[i * rwsize2 + 3 * j + 2] = (BYTE)R;
				lpOutImg[i * rwsize2 + 3 * j + 1] = (BYTE)G;
				lpOutImg[i * rwsize2 + 3 * j] = (BYTE)B;
				
			}
		}
	
	// 계산된 역상 이미지를 하드 디스크에 출력 (24비트인 트루컬러로 출력)
	hInfo.biBitCount = 24;
	hInfo.biSizeImage = rwsize2 * hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant = 0;
	hf.bfOffBits = 54; // 팔레트가 없으므로 파일의 시작부에서 영상데이터까지의 오프셋은 고정크기임
	hf.bfSize = hf.bfOffBits + hInfo.biSizeImage;

	// 영상 출력
	err = fopen_s(&outfile_s, "Color_Invert_OutImg.bmp", "wb"); // 출력할 파일 오픈
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // 파일헤드 출력
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // 영상헤드 출력
	fwrite(lpOutImg, sizeof(char), rwsize2*hInfo.biHeight, outfile_s); // 영상데이터 출력
	fclose(outfile_s); // 파일 닫기

	//메모리 해제
	if (hInfo.biClrUsed != 0) delete[]pRGB;
	delete[]lpOutImg;
	delete[]lpImg;
}

void GreenInvert() {
	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	err = fopen_s(&infile_s, "pshop256.bmp", "rb");
	if (err) { printf("There is no file!\n"); exit(1); }

	// BMP헤드 정보의 입력
	BITMAPFILEHEADER hf; // "파일정보헤드" 변수 선언
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // 파일정보헤드 읽음
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // 파일 타입이 "BM"(0x4D42)인지 검사

	BITMAPINFOHEADER hInfo; // "영상정보헤드" 변수 선언
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // 영상정보헤드 읽음
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	// 256 컬러 이하의 경우는 취급하지 않음
	if (hInfo.biBitCount < 8) {
		printf("Bad File format!"); exit(1);
	}

	// 팔레트 정보의 입력
	RGBQUAD* pRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		pRGB = new RGBQUAD[hInfo.biClrUsed]; // 팔레트의 크기만큼 메모리를 할당함
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile_s); // 팔레트를 파일에서 읽음
	}

	// (영상 데이터를 저장할) 메모리 할당
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // 저장할 영상메모리 할당
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // 영상 데이터 읽음
	fclose(infile_s); // 오픈했던 파일 닫음

	// 크기 계산, 메모리 할당
	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int rwsize2 = WIDTHBYTES(24 * hInfo.biWidth);
	BYTE* lpOutImg = new BYTE[rwsize2 * hInfo.biHeight];

	int index, R, G, B;

	if (hInfo.biBitCount == 24) // 만일 입력 영상이 트루(24비트) 컬러인 경우엔 팔레트 없음
		for (int i = 0; i < hInfo.biHeight; i++) {	// 역상의 이미지 구하기
			for (int j = 0; j < hInfo.biWidth; j++) {
				// 팔레트가 없으므로 영상데이터가 바로 컬러값
				lpOutImg[i * rwsize2 + 3 * j + 2] = 255 - lpImg[i * rwsize + 3 * j + 2];
				lpOutImg[i * rwsize2 + 3 * j + 1] = 255 - lpImg[i * rwsize + 3 * j + 1];
				lpOutImg[i * rwsize2 + 3 * j] = 255 - lpImg[i * rwsize + 3 * j];
			}
		}
	else // 트루컬러가 아닌 경우에는 팔레트 있음

		for (int i = 0; i < hInfo.biHeight; i++) {	// 역상의 이미지 구하기
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = pRGB[index].rgbGreen; // 팔레트에서 실제 영상 데이터를 가져옴(R)
				G = pRGB[index].rgbGreen; // 팔레트에서 실제 영상 데이터를 가져옴(G)
				B = pRGB[index].rgbGreen; // 팔레트에서 실제 영상 데이터를 가져옴(B)
				//R = 255 - R; G = 255 - G; B = 255 - B; // 역상 계산
				lpOutImg[i * rwsize2 + 3 * j + 2] = (BYTE)R;
				lpOutImg[i * rwsize2 + 3 * j + 1] = (BYTE)G;
				lpOutImg[i * rwsize2 + 3 * j] = (BYTE)B;

			}
		}

	// 계산된 역상 이미지를 하드 디스크에 출력 (24비트인 트루컬러로 출력)
	hInfo.biBitCount = 24;
	hInfo.biSizeImage = rwsize2 * hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant = 0;
	hf.bfOffBits = 54; // 팔레트가 없으므로 파일의 시작부에서 영상데이터까지의 오프셋은 고정크기임
	hf.bfSize = hf.bfOffBits + hInfo.biSizeImage;

	// 영상 출력
	err = fopen_s(&outfile_s, "pshop256_gray.bmp", "wb"); // 출력할 파일 오픈
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // 파일헤드 출력
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // 영상헤드 출력
	fwrite(lpOutImg, sizeof(char), rwsize2 * hInfo.biHeight, outfile_s); // 영상데이터 출력
	fclose(outfile_s); // 파일 닫기

	//메모리 해제
	if (hInfo.biClrUsed != 0) delete[]pRGB;
	delete[]lpOutImg;
	delete[]lpImg;
}

