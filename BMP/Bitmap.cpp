#include <stdio.h>
#include <windows.h>  // BITMAPFILEHEADER, etc.

#define WIDTHBYTES(bits) (((bits)+31)/32*4) // 4����Ʈ�� ������� ��
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

	BITMAPFILEHEADER hf; // "�����������" ���� ����
	hf.bfType = (WORD) 0x4D42;
	hf.bfSize = (DWORD)326;
	hf.bfReserved1 = (WORD)0;
	hf.bfReserved2 = (WORD)0;
	hf.bfOffBits = (DWORD)310;

	if (hf.bfType != 0x4D42) exit(1); // ����Ÿ���� BM���� �˻�

	BITMAPINFOHEADER hInfo; // "�����������" ���� ����
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


	// �ȷ�Ʈ ������ �Է�
	RGBQUAD hRGB[256];
	for (int i = 0; i < 256; i++) { //for���� �̿��� hRGB �迭 ���ҿ� rgbRED,GREEN,BLUE �� �Ҵ�
		hRGB[i].rgbRed = (BYTE)i;
		hRGB[i].rgbGreen = (BYTE)i;
		hRGB[i].rgbBlue = (BYTE)i;
		hRGB[i].rgbReserved = (BYTE)0;

	}
	// �޸� �Ҵ�
	BYTE* lpImg = new BYTE[hInfo.biSizeImage]; // ������ ����޸� �Ҵ�

	int rwsize = WIDTHBYTES(hInfo.biBitCount * hInfo.biWidth);

	unsigned char OrgImg[256][256]; // ��, �簢�� �׸� �迭 ����

	// ������ �ٸ� ��
	for (int i = 0; i < 256; i++) {

		for (int j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // �������� 40 �̸��� ��� - �� �� 0 *black
				OrgImg[i][j] = 0;	
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // �������� 40 �̻�, 80 �̸��� ��� - �� �� 64 *dark gray
				OrgImg[i][j] = 64;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // �������� 80 �̻�, 120 �̸��� ��� - �� �� 128 *light gray
				OrgImg[i][j] = 128;
			}
			else {// �������� 120 �̻��� ��� - �� �� 255 *white
				OrgImg[i][j] = 200;
			}
		}
	}

	//���� ��� ���� ���簢�� 32x32
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			OrgImg[i][j] = 0;
		}
	}

	// �̹��� ����
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * hInfo.biWidth + j] = OrgImg[hInfo.biHeight-i-1][j];
		}
	}


	// ���� ���
	err = fopen_s(&outfile_s, "sample gray image.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER) ,outfile_s);
	fwrite(hRGB, sizeof(RGBQUAD), 256, outfile_s);
	fwrite(lpImg, sizeof(char),256*256, outfile_s);
	fclose(outfile_s);

	// �޸� ����
	delete[]lpImg;

}//void

void color_Image() {

	errno_t err;
	FILE* outfile_s;

	BITMAPFILEHEADER hf; // "�����������" ���� ����
	hf.bfType = (WORD)0x4D42;
	hf.bfSize = (DWORD)1572888;
	hf.bfReserved1 = (WORD)0;
	hf.bfReserved2 = (WORD)0;
	hf.bfOffBits = (DWORD)54;

	if (hf.bfType != 0x4D42) exit(1); // ����Ÿ���� BM���� �˻�

	BITMAPINFOHEADER hInfo; // "�����������" ���� ����
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
	// �޸� �Ҵ�
	BYTE* lpImg = new BYTE[rwsize * hInfo.biHeight]; // ������ ����޸� �Ҵ�

	unsigned char C0[256][256]; // C0 // (���� �����͸� ������� �ʾ�����) R ä�η� ����
	unsigned char C1[256][256]; // G ä�η� ����
	unsigned char C2[256][256]; // B ä�η� ����

	// �ȷ�Ʈ ������ �Է�
	RGBQUAD* hRGB = NULL;
	if (hInfo.biClrUsed != 0) {
		hRGB = new RGBQUAD[hInfo.biClrUsed]; // �ȷ�Ʈ�� ũ�⸸ŭ �޸𸮸� �Ҵ���
	}
	int index, R, G, B;

	// �÷����� �ٸ� ��
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // �������� 40 �̸��� ��� - B
				C0[i][j] = 0;
				C1[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // �������� 40 �̻�, 80 �̸��� ��� - G
				C0[i][j] = 0;
				C2[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // �������� 80 �̻�, 120 �̸��� ��� - R
				C1[i][j] = 0;
				C2[i][j] = 0;
			}
			else {// �������� 120 �̻��� ��� - �� �� 200
				C0[i][j] = 200;
				C1[i][j] = 200;
				C2[i][j] = 200;
			}
		}
	}


	//���� ��� ���� ���簢�� 32x32
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 32; j++) {
			C0[i][j] = 0;
			C1[i][j] = 0;
			C2[i][j] = 0;
		}
	}

	if (hInfo.biBitCount == 24) // ���� �Է� ������ Ʈ��(24��Ʈ) �÷��� ��쿣 �ȷ�Ʈ ����
		for (int i = 0; i < hInfo.biHeight; i++) {
			for (int j = 0; j < hInfo.biWidth; j++) {
				// �ȷ�Ʈ�� �����Ƿ� �������Ͱ� �ٷ� �÷���
				lpImg[i * rwsize + 3 * j + 2] = C0[i][j];
				lpImg[i * rwsize + 3 * j + 1] = C1[i][j];
				lpImg[i * rwsize + 3 * j] = C2[i][j];
			}
		}
	else // Ʈ���÷��� �ƴ� ��쿡�� �ȷ�Ʈ ����

		for (int i = 0; i < hInfo.biHeight; i++) {	// ������ �̹��� ���ϱ�
			for (int j = 0; j < hInfo.biWidth; j++) {
				index = lpImg[i * rwsize + j];
				R = hRGB[index].rgbRed; // �ȷ�Ʈ���� ���� ���� �����͸� ������(R)
				G = hRGB[index].rgbGreen; // �ȷ�Ʈ���� ���� ���� �����͸� ������(G)
				B = hRGB[index].rgbBlue; // �ȷ�Ʈ���� ���� ���� �����͸� ������(B)

				C0[i][j] = (BYTE)R;
				C1[i][j] = (BYTE)G;
				C2[i][j] = (BYTE)B;

			}
		}

	// �̹��� ����
	for (int i = 0; i < hInfo.biHeight; i++) {
		for (int j = 0; j < hInfo.biWidth; j++) {
			lpImg[i * rwsize + 3 * j + 2] = C0[hInfo.biHeight - 1 - i][j];
			lpImg[i * rwsize + 3 * j + 1] = C1[hInfo.biHeight - 1 - i][j];
			lpImg[i * rwsize + 3 * j] = C2[hInfo.biHeight - 1 - i][j];
		}
	}


	// ���� ���
	err = fopen_s(&outfile_s, "sample color image.bmp", "wb");
	fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile_s);
	fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile_s);
	fwrite(lpImg, sizeof(char), rwsize*hInfo.biHeight, outfile_s);
	fclose(outfile_s);

	// �޸� ����
	if (hInfo.biClrUsed != 0) delete[]hRGB;
	delete[]lpImg;
 }