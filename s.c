#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <float.h>
#include <math.h>
#pragma warning(disable:4996)

#define debug_mode_view_matrix
//활성화하면('NOT_' 제거) 연산 수행마다 (현 연산 단계)\n\n(입력받은 행렬)\n\n(역행렬) 형태로 시각적으로 출력


//모든 주대각성분이 0인지 확인
int is_Diagonal_all_zero(double x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if (((i == j) && x[i][j] != 0.0)) //주대각성분에 0이 없다면
				return 0;

	return 1;
}

//주대각성분이 0이 하나라도 있는지 확인
int is_Diagonal_zero(double x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if (((i == j) && x[i][j] == 0.0)) //주대각성분에 0이 없다면
				return 1;

	return 0;
}

//대각행렬인지 확인
int is_Diagonal(double x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if ((i != j) && x[i][j] != 0.0) //대각행렬 조건에 어긋나면 (주대각성분 이외 == 0)
				return 0;

	return 1;
}

//주대각행렬이 모두 0인 경우 계산이 불가능하기에 행 교체
int swap(double x[][100], double des[][100], int dim)
{
	for (int i = 0; i < dim; i++)
	{
		for (int j = 1; j < dim; j++)
		{
			if (x[i][i] != x[j][i])
			{
				for (int k = 0; k < dim; k++)
				{
					double temp = x[j][k];
					x[j][k] = x[i][k];
					x[i][k] = temp;

					temp = des[j][k];
					des[j][k] = des[i][k];
					des[i][k] = temp;
				}

				return 1;
			}
		}
	}

	return 0;
}

//역행렬 계산
int get_inverse(double x[][100], double des[][100], int dim)
{
	double mul = 0.0;
	int modified_row = 0; //수정된 행의 색을 변화시키기 위해 수정된 행의 수 저장

	do
	{
		if (is_Diagonal_all_zero(x, dim) == 1) //모든 주대각성분이 0일 때
			if (swap(x, des, dim) == 0) //행을 교체할 수 없다면
				return 0; //역행렬 없음

		//대각행렬로 만듦
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++) //j - 행렬의 행 수
			{
				if (i == j || x[i][i] == 0.0 || x[j][i] == 0.0) //분모, 분자가 0인 경우 continue
					continue;

				for (int k = 0; k < dim; k++) //k - 행렬의 열 수
				{
					modified_row = j;

					if (k == 0) //처음 한 번만 실행
						mul = -x[j][i] / x[i][i];

					x[j][k] += mul * x[i][k];
					des[j][k] += mul * des[i][k];

					//실수 계산의 부동소수점 반올림 오차를 해결하기 위해 너무 작은 값은 0으로 처리
					if (fabs(x[j][k]) <= DBL_EPSILON)
						x[j][k] = 0.0;

					if (fabs(des[j][k]) <= DBL_EPSILON)
						des[j][k] = 0.0;
				}



#ifdef debug_mode_view_matrix
				puts("\n\n\n\nSt.1 - Making Diagonal Matrix\n\n"); //대각행렬 만들기

				for (int i = 0; i < dim; i++)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (modified_row == i) ? 12 : 10); //일반적 - 연두색(10), 수정된 행 - 주황색(12)

					for (int j = 0; j < dim; j++)
						printf("%11lg ", x[i][j]);

					puts("");
				}

				puts("");

				for (int i = 0; i < dim; i++)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (modified_row == i) ? 12 : 10);

					for (int j = 0; j < dim; j++)
						printf("%11lg ", des[i][j]);

					puts("");
				}

				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
#endif
			}
		}
	} while ((is_Diagonal(x, dim) == 0));
	//대각행렬이 나올 때 까지 반복

	//위의 연산을 끝낸 상태에서 주대각성분이 하나라도 0이면 역행렬은 존재하지 않음
	if (is_Diagonal_zero(x, dim) == 1)
		return 0;

	//주대각성분이 1이 되게 변환 -> 최종적으로 역행렬 계산
	for (int i = 0; i < dim; i++)
	{
		mul = 1 / x[i][i];

		if (mul == 1) //주대각성분이 1이면 나눌 필요 없음
			continue;

		modified_row = i;

		for (int j = 0; j < dim; j++)
		{
			des[i][j] *= mul;
			x[i][j] *= mul; //사실상 불필요한 코드. 하지만 debug_mode_view_matrix를 통한 디버그 모드에서 행렬의 계산 과정을 보려면 필요
		}

#ifdef debug_mode_view_matrix
		puts("\n\n\n\nSt.2 - Making Identity Matrix\n\n"); //단위행렬 만들기

		for (int i = 0; i < dim; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (modified_row == i) ? 12 : 10);

			for (int j = 0; j < dim; j++)
				printf("%11lg ", x[i][j]);

			puts("");
		}

		puts("");

		for (int i = 0; i < dim; i++)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (modified_row == i) ? 12 : 10);

			for (int j = 0; j < dim; j++)
				printf("%11lg ", des[i][j]);

			puts("");
		}

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
#endif
	}

	return 1;
}

//단위행렬 구현
void make_I(double x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			x[i][j] = (i == j) ? 1.0 : 0.0; //대각선에 1, 나머지에 0 
}


int main()
{
	system("title Gauss-Jordan elimination method - made by https://blog.naver.com/gmail2012");
	int dim; //입력될 차원 수
	double arr[100][100], inv[100][100]; //각각 입력받을 행렬, 역행렬을 구하고 저장할 행렬
	scanf("%d", &dim);

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			scanf("%lf", &arr[i][j]);


	//inv배열을 단위행렬로 만듦 - 가우스 요르단 소거법을 사용하기 위해
	make_I(inv, dim);

	puts("\n");

	//역행렬이 존재한다면 출력
	if (get_inverse(arr, inv, dim) == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		puts("\n\n\n");

		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
				printf("%11lg ", inv[i][j]);

			puts("");
		}
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		printf("\n\n\ninverse matrix does not exist.");
	}

	puts("");

	system("pause"); //콘솔 창 종료되는 현상 방지

	return 0;
}