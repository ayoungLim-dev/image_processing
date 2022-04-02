#include <stdio.h>

int Negative_Image();
int GrayScale_Image();
int Color_Image();

int main() {
	Negative_Image();
	//GrayScale_Image();
	//Color_Image();
}

/* ����1 negative image */
int Negative_Image() {
	int i, j;
	unsigned char OrgImg[256][256];
	//unsigned char = 8bits = 256

	errno_t err;
	FILE* infile_s = NULL;
	FILE* outfile_s = NULL;


	//��ũ���� �������� �б�

	err = fopen_s(&infile_s, "Coin.raw", "rb");

	if (err != 0)
	{
		printf("File open error!"); return -1;
	}

	fread(OrgImg, sizeof(unsigned char), 256 * 256, infile_s);
	fclose(infile_s);

	//�������� ���� ����ó��
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			OrgImg[i][j] = 255 - OrgImg[i][j];

		}

	}

	//�ϵ��ũ�� �������� ����
	err = fopen_s(&outfile_s, "Coin_inv.raw", "wb");
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s);
	fclose(outfile_s);

}


/* ����2 | gray-scale ���� ���� */
int GrayScale_Image(){
	int i, j;
	unsigned char OrgImg[256][256];
	//unsigned char = 8bits = 256

	errno_t err;
	//FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	//�о� �� ������ ������ �����Ƿ� fopen_s, fread �Լ� �ۼ� X

	// ������ �ٸ� ��
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) // �������� 40 �̸��� ��� - �� �� 0 *black
				OrgImg[i][j] = 0;
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) // �������� 40 �̻�, 80 �̸��� ��� - �� �� 64 *dark gray
				OrgImg[i][j] = 64;
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) // �������� 80 �̻�, 120 �̸��� ��� - �� �� 128 *light gray
				OrgImg[i][j] = 128;
			else // �������� 120 �̻��� ��� - �� �� 255 *white
				OrgImg[i][j] = 255;
		}

	}

	//���� ��� ���� ���簢�� 32x32
	for (i = 0; i < 32; i++) {

		for (j = 0; j < 32; j++) {

			OrgImg[i][j] = 0;

		}

	}


	//�ϵ��ũ�� �������� ����
	err = fopen_s(&outfile_s, "sample gray image.raw", "wb");
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s);
	fclose(outfile_s);

	//��ũ���� �������� �б�
	if (err != 0)
	{
		printf("File open error!"); return -1;
	}

}

/* ���� 3 | color ���� ���� */

int Color_Image(){
	int i, j;
	unsigned char OrgImg[256][256]; // C0 // (���� �����͸� ������� �ʾ�����) R ä�η� ����
	unsigned char C1[256][256]; // G ä�η� ����
	unsigned char C2[256][256]; // B ä�η� ����
	//unsigned char = 8bits = 256

	errno_t err;
	//FILE* infile_s = NULL;
	FILE* outfile_s = NULL;

	//�о� �� ������ ������ �����Ƿ� fopen_s, fread �Լ� �ۼ� X



	// ������ �ٸ� ��
	for (i = 0; i < 256; i++) {

		for (j = 0; j < 256; j++) {

			if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 1600) { // �������� 40 �̸��� ��� - R, G ä�� �� 0 *blue
				OrgImg[i][j] = 0;
				C1[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 6400) { // �������� 40 �̻�, 80 �̸��� ��� - R, B ä�� �� 0 *green
				OrgImg[i][j] = 0;
				C2[i][j] = 0;
			}
			else if ((j - 128) * (j - 128) + (i - 128) * (i - 128) < 14400) { // �������� 80 �̻�, 120 �̸��� ��� - G, B ä�� �� 0 *red
				C1[i][j] = 0;
				C2[i][j] = 0;
			}
			else{ // �������� 120 �̻��� ��� - R, G, B ä�� �� 255 *white
				OrgImg[i][j] = 255;
				C1[i][j] = 255;
				C2[i][j] = 255;
			}

		}

	}

	//���� ��� ���� ���簢�� 32x32
	for (i = 0; i < 32; i++) {

		for (j = 0; j < 32; j++) {

			OrgImg[i][j] = 0;
			C1[i][j] = 0;
			C2[i][j] = 0;
			// RGB ä�� ��� 0�̾�� ����

		}

	}


	//�ϵ��ũ�� �������� ����
	err = fopen_s(&outfile_s, "sample color image.raw", "wb");

	// �̸� �����س��� ä�� ������� ������ �б�
	fwrite(OrgImg, sizeof(unsigned char), 256 * 256, outfile_s); // R channel
	fwrite(C1, sizeof(unsigned char), 256 * 256, outfile_s); // G channel
	fwrite(C2, sizeof(unsigned char), 256 * 256, outfile_s); // B channel

	fclose(outfile_s);

	//��ũ���� �������� �б�
	if (err != 0)
	{
		printf("File open error!"); return -1;
	}
}



