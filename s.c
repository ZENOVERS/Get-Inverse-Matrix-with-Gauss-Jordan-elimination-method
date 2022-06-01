#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma warning(disable:4996)

#define debug_mode_view_matrix
//활성화하면 연산 수행마다 (입력받은 행렬)\n\n(역행렬) 형태로 시각적으로 출력


//단위행렬인지 확인(단위행렬이면 1, 아니면 0 반환) 
int is_I(float x[][100], int dim) {
	int cnt = 0;

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if ((i == j && x[i][j] != 1.0f) || (i != j && x[i][j] != 0.0f)) //단위행렬 조건 - 주대각성분이 모두 1, 나머지 성분은 모두 0
				return 0;

	return 1;
}


//대각행렬 및 역행렬이 존재하는지 확인
int is_Diagonal(float x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			if (((i == j) && x[i][j] == 0) || ((i != j) && x[i][j] != 0)) //역행렬 존재 여부(주대각성분 != 0) && 대각행렬 조건(주대각성분 이외 == 0)
				return 0;

	return 1;
}


//역행렬 계산
int get_inverse(float x[][100], float des[][100], int dim)
{
	float mul = 0.0f;

	//대각행렬로 만듦
	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			for (int k = 0; k < dim; k++)
			{
				if (i == j || x[i][i] == 0)
					break;

				if (k == 0)
					mul = -x[j][i] / x[i][i];

				x[j][k] += mul * x[i][k];
				des[j][k] += mul * des[i][k];
			}
			
#ifdef debug_mode_view_matrix
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); //글씨 색 초록색

			puts("\n");

			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
					printf("%g ", x[i][j]);

				puts("");
			}

			puts("");

			for (int i = 0; i < dim; i++)
			{
				for (int j = 0; j < dim; j++)
					printf("%g ", des[i][j]);

				puts("");
			}
#endif

		}
	}

	//위의 연산을 끝낸 상태에서 대각행렬이 아니면 역행렬은 존재하지 않음
	if (is_Diagonal(x, dim) == 0)
		return 0;

	//대각행렬이 1이 되게 변환 -> 최종적으로 역행렬 계산
	for (int i = 0; i < dim; i++)
	{
		mul = 1 / x[i][i];

		for (int j = 0; j < dim; j++)
		{
			des[i][j] *= mul;
			x[i][j] *= mul; //사실상 불필요한 코드. 하지만 debug_mode_view_matrix를 통한 디버그 모드에서 행렬의 계산 과정을 보려면 필요
		}

#ifdef debug_mode_view_matrix
		puts("\n");

		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
				printf("%g ", x[i][j]);

			puts("");
		}

		puts("");

		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
				printf("%g ", des[i][j]);

			puts("");
		}

		puts("\n");
#endif

	}

	return 1;
}


//단위행렬 구현
void make_I(float x[][100], int dim)
{
	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			x[i][j] = (i == j) ? 1.0f : 0.0f; //대각선에 1, 나머지에 0 
}


int main()
{
	/* === 조건 및 주의사항 ===
	- 입력되는 행렬의 차원은 100보다 작거나 같다.
	- 각 행렬의 요소는 float 범위를 벗어나지 않는다.
	- 분수 형태가 아닌 실수 형태로 계산되므로 소수점 이하 자리에서 오차가 발생한다.



	   === 순서도 ===
	   1. dim차원 정방행렬 arr에 입력
	   2. inv 배열을 단위행렬로 변환
	   3. arr배열을 가우스-요르단 소거법을 통해 각 열을 항등행렬로 변환
	   4. inv배열에 arr에 적용한 연산 그대로 수행
	   5. dim번 만큼 3~4과정 반복
	   6. arr이 대각행렬인지 확인 -> 위의 연산 결과에서 대각행렬이 돼야만 역행렬이 존재
	   7. arr을 단위행렬로 변환
	   8. arr에 적용한 연산 inv에 그대로 수행
	   9. dim번 만큼 7~8과정 반복
	*/

	
	system("title Gauss-Jordan elimination method - made by https://blog.naver.com/gmail2012");
	int dim; //입력될 차원 수
	float arr[100][100], inv[100][100]; //각각 입력받을 행렬, 역행렬을 구하고 저장할 행렬
	scanf("%d", &dim);

	for (int i = 0; i < dim; i++)
		for (int j = 0; j < dim; j++)
			scanf("%f", &arr[i][j]);


	//inv배열을 단위행렬로 만듦 - 가우스 요르단 소거법을 사용하기 위해
	make_I(inv, dim);

	//역행렬 계산
	get_inverse(arr, inv, dim);

	puts("");


	//역행렬이 존재한다면 출력
	if (get_inverse(arr, inv, dim) == 1)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6); //글씨 색 주황
		
		for (int i = 0; i < dim; i++)
		{
			for (int j = 0; j < dim; j++)
				printf("%10.5f ", inv[i][j]);

			puts("");
		}
	}
	else
		printf("inverse matrix is not exist.");

	system("pause"); //콘솔 창 종료되는 현상 방지

	return 0;
}