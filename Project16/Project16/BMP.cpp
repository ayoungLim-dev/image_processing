#include <stdio.h>
#include <windows.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4) // ������ �������� 4����Ʈ�� ���
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

	// BMP��� ������ �Է�
	BITMAPFILEHEADER hf; // "�����������" ���� ����
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // ����������� ����
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // ���� Ÿ���� "BM"(0x4D42)���� �˻�

	BITMAPINFOHEADER hInfo; // "�����������" ���� ����
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // ����������� ����
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	if (hInfo.biBitCount != 8) { // ���
		printf("Bad File format!"); exit(1);
	}

	// �ȷ�Ʈ ������ �Է�
	RGBQUAD hRGB[256]; // �ȷ�Ʈ ������ ���� �迭(�������)
	fread(hRGB, sizeof(RGBQUAD), 256, infile_s); //�ȷ�Ʈ �Է�

	// (���� �����͸� ������) �޸� �Ҵ�
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // ������ ����޸� �Ҵ�
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // ���� ������ ����
	fclose(infile_s); // �����ߴ� ���� ����

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	// ������ �̹��� ���ϱ�
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * rwsize + j] = 255 - lpImg[i * rwsize + j];
		}
	}

	// ���� ���
	err = fopen_s(&outfile_s, "Gray_Invert_OutImg.bmp", "wb"); // ����� ���� ����
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // ������� ���
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // ������� ���
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile_s); // �ȷ�Ʈ ���
	fwrite(lpImg, sizeof(char), hInfo.biSizeImage, outfile_s); // �������� ���
	fclose(outfile_s); // ���� �ݱ�

	//�޸� ����
	delete[]lpImg;
}

void ColorInvert() {
	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	err = fopen_s(&infile_s, "pshop256.bmp", "rb");
	if (err) { printf("There is no file!\n"); exit(1); }

	// BMP��� ������ �Է�
	BITMAPFILEHEADER hf; // "�����������" ���� ����
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // ����������� ����
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // ���� Ÿ���� "BM"(0x4D42)���� �˻�

	BITMAPINFOHEADER hInfo; // "�����������" ���� ����
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // ����������� ����
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	// 256 �÷� ������ ���� ������� ����
	if (hInfo.biBitCount < 8) {
		printf("Bad File format!"); exit(1);
	}

	// �ȷ�Ʈ ������ �Է�
	RGBQUAD *pRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		pRGB = new RGBQUAD[hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ���
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile_s); // �ȷ�Ʈ�� ���Ͽ��� ����
	}

	// (���� �����͸� ������) �޸� �Ҵ�
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // ������ ����޸� �Ҵ�
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // ���� ������ ����
	fclose(infile_s); // �����ߴ� ���� ����

	// ũ�� ���, �޸� �Ҵ�
	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int rwsize2 = WIDTHBYTES(24*hInfo.biWidth);
	BYTE* lpOutImg = new BYTE [rwsize2 * hInfo.biHeight];

	int index, R, G, B;

	if(hInfo.biBitCount==24) // ���� �Է� ������ Ʈ��(24��Ʈ) �÷��� ��쿣 �ȷ�Ʈ ����
		for (int i = 0; i < hInfo.biHeight; i++) {	// ������ �̹��� ���ϱ�
			for (int j = 0; j < hInfo.biWidth; j++) {
				// �ȷ�Ʈ�� �����Ƿ� �������Ͱ� �ٷ� �÷���
				lpOutImg[i * rwsize2 + 3*j + 2] = 255 - lpImg[i * rwsize + 3*j + 2];
				lpOutImg[i * rwsize2 + 3*j + 1] = 255 - lpImg[i * rwsize + 3*j + 1];
				lpOutImg[i * rwsize2 + 3*j] = 255 - lpImg[i * rwsize + 3*j];
			}
		}
	else // Ʈ���÷��� �ƴ� ��쿡�� �ȷ�Ʈ ����

		for (int i = 0; i < hInfo.biHeight; i++) {	// ������ �̹��� ���ϱ�
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = pRGB[index].rgbRed; // �ȷ�Ʈ���� ���� ���� �����͸� ������(R)
				G = pRGB[index].rgbGreen; // �ȷ�Ʈ���� ���� ���� �����͸� ������(G)
				B = pRGB[index].rgbBlue; // �ȷ�Ʈ���� ���� ���� �����͸� ������(B)
				R = 255 - R; G = 255 - G; B = 255 - B; // ���� ���
				lpOutImg[i * rwsize2 + 3 * j + 2] = (BYTE)R;
				lpOutImg[i * rwsize2 + 3 * j + 1] = (BYTE)G;
				lpOutImg[i * rwsize2 + 3 * j] = (BYTE)B;
				
			}
		}
	
	// ���� ���� �̹����� �ϵ� ��ũ�� ��� (24��Ʈ�� Ʈ���÷��� ���)
	hInfo.biBitCount = 24;
	hInfo.biSizeImage = rwsize2 * hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant = 0;
	hf.bfOffBits = 54; // �ȷ�Ʈ�� �����Ƿ� ������ ���ۺο��� �������ͱ����� �������� ����ũ����
	hf.bfSize = hf.bfOffBits + hInfo.biSizeImage;

	// ���� ���
	err = fopen_s(&outfile_s, "Color_Invert_OutImg.bmp", "wb"); // ����� ���� ����
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // ������� ���
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // ������� ���
	fwrite(lpOutImg, sizeof(char), rwsize2*hInfo.biHeight, outfile_s); // �������� ���
	fclose(outfile_s); // ���� �ݱ�

	//�޸� ����
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

	// BMP��� ������ �Է�
	BITMAPFILEHEADER hf; // "�����������" ���� ����
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, infile_s); // ����������� ����
	if (hf.bfType != 0x4D42) exit(1); //String "BM" // ���� Ÿ���� "BM"(0x4D42)���� �˻�

	BITMAPINFOHEADER hInfo; // "�����������" ���� ����
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, infile_s); // ����������� ����
	printf("Image Size : (%3dx%3d)\n", hInfo.biWidth, hInfo.biHeight);
	printf("Image Type : %dbit Colors\n", hInfo.biBitCount);

	// 256 �÷� ������ ���� ������� ����
	if (hInfo.biBitCount < 8) {
		printf("Bad File format!"); exit(1);
	}

	// �ȷ�Ʈ ������ �Է�
	RGBQUAD* pRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		pRGB = new RGBQUAD[hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ���
		fread(pRGB, sizeof(RGBQUAD), hInfo.biClrUsed, infile_s); // �ȷ�Ʈ�� ���Ͽ��� ����
	}

	// (���� �����͸� ������) �޸� �Ҵ�
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // ������ ����޸� �Ҵ�
	fread(lpImg, sizeof(char), hInfo.biSizeImage, infile_s); // ���� ������ ����
	fclose(infile_s); // �����ߴ� ���� ����

	// ũ�� ���, �޸� �Ҵ�
	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);
	int rwsize2 = WIDTHBYTES(24 * hInfo.biWidth);
	BYTE* lpOutImg = new BYTE[rwsize2 * hInfo.biHeight];

	int index, R, G, B;

	if (hInfo.biBitCount == 24) // ���� �Է� ������ Ʈ��(24��Ʈ) �÷��� ��쿣 �ȷ�Ʈ ����
		for (int i = 0; i < hInfo.biHeight; i++) {	// ������ �̹��� ���ϱ�
			for (int j = 0; j < hInfo.biWidth; j++) {
				// �ȷ�Ʈ�� �����Ƿ� �������Ͱ� �ٷ� �÷���
				lpOutImg[i * rwsize2 + 3 * j + 2] = 255 - lpImg[i * rwsize + 3 * j + 2];
				lpOutImg[i * rwsize2 + 3 * j + 1] = 255 - lpImg[i * rwsize + 3 * j + 1];
				lpOutImg[i * rwsize2 + 3 * j] = 255 - lpImg[i * rwsize + 3 * j];
			}
		}
	else // Ʈ���÷��� �ƴ� ��쿡�� �ȷ�Ʈ ����

		for (int i = 0; i < hInfo.biHeight; i++) {	// ������ �̹��� ���ϱ�
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = pRGB[index].rgbGreen; // �ȷ�Ʈ���� ���� ���� �����͸� ������(R)
				G = pRGB[index].rgbGreen; // �ȷ�Ʈ���� ���� ���� �����͸� ������(G)
				B = pRGB[index].rgbGreen; // �ȷ�Ʈ���� ���� ���� �����͸� ������(B)
				//R = 255 - R; G = 255 - G; B = 255 - B; // ���� ���
				lpOutImg[i * rwsize2 + 3 * j + 2] = (BYTE)R;
				lpOutImg[i * rwsize2 + 3 * j + 1] = (BYTE)G;
				lpOutImg[i * rwsize2 + 3 * j] = (BYTE)B;

			}
		}

	// ���� ���� �̹����� �ϵ� ��ũ�� ��� (24��Ʈ�� Ʈ���÷��� ���)
	hInfo.biBitCount = 24;
	hInfo.biSizeImage = rwsize2 * hInfo.biHeight;
	hInfo.biClrUsed = hInfo.biClrImportant = 0;
	hf.bfOffBits = 54; // �ȷ�Ʈ�� �����Ƿ� ������ ���ۺο��� �������ͱ����� �������� ����ũ����
	hf.bfSize = hf.bfOffBits + hInfo.biSizeImage;

	// ���� ���
	err = fopen_s(&outfile_s, "pshop256_gray.bmp", "wb"); // ����� ���� ����
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s); // ������� ���
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s); // ������� ���
	fwrite(lpOutImg, sizeof(char), rwsize2 * hInfo.biHeight, outfile_s); // �������� ���
	fclose(outfile_s); // ���� �ݱ�

	//�޸� ����
	if (hInfo.biClrUsed != 0) delete[]pRGB;
	delete[]lpOutImg;
	delete[]lpImg;
}

