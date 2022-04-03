#include <stdio.h>
#include <stdlib.h>
// 메모리 동적할당을 하는 malloc()이 포함돼있는 라이브러리 추가
#include <time.h>
// time()(UCT 기준 1970년1월1일0시0분0초로부터 경과된 시간을 초(sec)로 반환하는 함수)이 포함돼있는 라이브러리 추가

/* 함수의 원형 선언 */
int** create_matrix(int row, int col); // 행렬 생성
void print_matrix(int** matrix, int row, int col); // 행렬 출력
int free_matrix(int** matrix, int row, int col); // 생성된 행렬 소멸
int fill_data(int** matrix, int row, int col); // 행렬에 성분 삽입
int addition_matrix(int** matrix_a, int** matrix_b, int row, int col); // 두 행렬의 덧셈
int subtraction_matrix(int** matrix_a, int** matrix_b, int row, int col); // 두 행렬의 뺄셈
int transpose_matrix(int** matrix, int** matrix_t, int row, int col); // 전치행렬
int multiply_matrix(int** matrix_a, int** matrix_t, int row, int col); // 두 행렬의 곱셈

int main()
{
    char command; // 사용자의 입력값을 저장할 변수 선언
    printf("[----- [허원일]  [2019036068] -----]\n");

	int row, col; // 행과 열의 값을 저장할 변수 선언
	srand(time(NULL)); // time()를 seed로 갖는 srand() 호출

	printf("Input row and col : ");
	scanf("%d %d", &row, &col);

    // 이중포인터 matrix_a, matrix_b 각각에 create_matrix()로부터 (입력된 행과 열 크기를 갖는 공간을 가리키는)주소를 받아와 대입
    int** matrix_a = create_matrix(row, col);
	int** matrix_b = create_matrix(row, col);

    // 이중포인터 matrix_a에 create_matrix()로부터 (입력된 행과 열 크기를 갖는 공간을 가리키는)주소를 받아와 대입 (전치행렬: 행과 열을 교환하여 얻는 행렬)
    int** matrix_a_t = create_matrix(col, row);

	printf("Matrix Created.\n");

    // 각 변수에서 메모리 할당이 제대로 이루어지지 않은 경우 -1 반환
	if (matrix_a == NULL || matrix_b == NULL) {return -1;}

    // do.. while 문을 사용하여 사용자의 입력이 Q나q(quit)가 아닐 경우 계속해서 메뉴선택창을 출력
	do{
		printf("----------------------------------------------------------------\n");
		printf("                     Matrix Manipulation                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Matrix   = z           Print Matrix        = p \n");
		printf(" Add Matrix          = a           Subtract Matrix     = s \n");
		printf(" Transpose matrix_a  = t           Multiply Matrix     = m \n");
		printf(" Quit                = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command); 

        /* 사용자의 입력값을 case로 갖는 switch */
		switch(command) { 
        // case 'z' - fill_data() 호출 후 각 행렬에 데이터 대입
		case 'z': case 'Z':
		    printf("Matrix Initialized\n");
			fill_data(matrix_a, row, col);
			fill_data(matrix_b, row, col);
			break;
        // case 'p' - print_matrix() 호출하여 각 행렬을 출력
        case 'p': case 'P':
            printf("Print matrix\n");
            printf("matrix_a\n");
			print_matrix(matrix_a, row, col);
			printf("matrix_b\n");
			print_matrix(matrix_b, row, col);
			break;
        // case 'a' - addtion_matrix() 호출하여 두 행렬을 더함
        case 'a': case 'A':
			printf("Add two matrices\n");
			addition_matrix(matrix_a, matrix_b, row, col);
			break;
        // case 's' - subtraction_matrix() 호출하여 행렬a에서 행렬b를 뺌
        case 's': case 'S':
			printf("Subtract two matrices \n");
            subtraction_matrix(matrix_a, matrix_b, row, col);
			break;
        // case 't' - transpose_matrix() 호출하여 행렬a의 전치행렬 출력
        case 't': case 'T':
			printf("Transpose matrix_a \n");
			printf("matrix_a\n");
            transpose_matrix(matrix_a, matrix_a_t, col, row);
            print_matrix(matrix_a_t, col, row);
			break;
        // case 'm' - multiply_matrix() 호출하여 두 행렬을 곱함. 이때, 행렬과 그것의 전치행렬을 곱함
        case 'm': case 'M':
			printf("Multiply matrix_a with transposed matrix_a \n");
			transpose_matrix(matrix_a, matrix_a_t, col, row);
            multiply_matrix(matrix_a, matrix_a_t, row, col);
			break;
        // case 'q' - free_matrix() 호출하여 각 행렬들에 할당된 메모리들을 해제(반납)
        case 'q': case 'Q':
            printf("Free all matrices..\n");
            free_matrix(matrix_a_t, col, row);
            free_matrix(matrix_a, row, col);
            free_matrix(matrix_b, row, col);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

/* malloc()을 사용하여 입력된 행과 열 크기로 행렬(2차원 배열)을 생성하는 함수 */
int** create_matrix(int row, int col)
{
	/* 전처리 - 행과 열의 값이 음수일 경우 NULL 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return NULL;
	}

	int** matrix = (int**)malloc(sizeof(int*) * row);
    // int형 이중포인터 변수 matrix 선언 : 입력된 행 크기를 갖는 포인터배열을 가리키는 이중포인터
    // 이중 포인터에 행 크기만큼의 메모리를 할당
	for (int r = 0; r < row; r++) { 
        // for반복문을 행크기 만큼 돌며 포인터배열의 원소에 차례대로 열 크기만큼의 메모리를 할당
		matrix[r] = (int*)malloc(sizeof(int) * col);
	}

	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 NULL 반환하며 함수 종료 */
	if (matrix == NULL) {
		printf("Memory Allocation Failed.\n");
		return NULL;
	}

	return matrix;
}

/* row x col 크기의 행렬을 출력하는 함수 */
void print_matrix(int** matrix, int row, int col)
{
	/* 전처리 - 행과 열의 값이 음수일 경우 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return;
	}

    for (int matrix_row = 0; matrix_row < row; matrix_row++) {
		for (int matrix_col = 0; matrix_col < col; matrix_col++)
        // 이중 for문을 돌며 행렬의 모든 성분를 출력. 이때, matrix[i][j] = **(matrix[i] + j)
			printf("%3d ", matrix[matrix_row][matrix_col]);
		printf("\n");
	}
	printf("\n");
	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 함수 종료 */
	if (matrix == NULL) {
		printf("Memory Allocation Failed.\n");
		return;
	}

	return;
}


/* create_matrix() 로 할당받은 메모리 공간을 해제(반납)하는 함수 */
int free_matrix(int** matrix, int row, int col)
{
	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}

	for (int matrix_row = 0; matrix_row < row; matrix_row++) {
		free(matrix[matrix_row]); 
        // for 반복문을 행의 크기만큼 돌며 free() 를 사용하여 각 행을 가리키는 포인터가 가리키는 공간을 반납
	}

	free(matrix);
    // 행의 크기만큼의 배열을 담는 이중포인터가 가리키는 공간을 반납
	return 1;
}


/* 주어진 행렬의 각 성분에 1~19사이의 난수를 대입하는 함수 */
int fill_data(int** matrix, int row, int col)
{
	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}
    for (int matrix_row = 0; matrix_row < row; matrix_row++)
		for (int matrix_col = 0; matrix_col < col; matrix_col++)
        // rand()와 %(나머지 연산자)를 이용해서 1~19 사이의 값을 행렬성분에 대입
			matrix[matrix_row][matrix_col] = rand() % 20;

	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 -1 반환하며 함수 종료 */
    if (matrix == NULL) {
		printf("Memory Allocation Failed.\n");
		return -1;
	}
	return 1;
}

/* 두 행렬의 덧셈을 수행하는 함수 */
int addition_matrix(int** matrix_a, int** matrix_b, int row, int col)
{
    int** matrix_sum = create_matrix(row, col);
    // 두 행렬을 더한 결과행렬을 저장할 공간을 담는 주소를 create_matrix()로부터 받아와 이중포인터 matrix_sum에 대입

	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}

	for (int matrix_row = 0; matrix_row < row; matrix_row++) {
		for (int matrix_col = 0; matrix_col < col; matrix_col++)
        // 이중 for문을 행과 열의 크기만큼 돌며 matrix_a와 matrix_b의 같은 위치의 성분들을 더한 값을 matrix_sum의 같은 위치에 대입
			matrix_sum[matrix_row][matrix_col] = matrix_a[matrix_row][matrix_col] + matrix_b[matrix_row][matrix_col];
	}
	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 -1 반환하며 함수 종료 */
	if (matrix_a == NULL || matrix_b == NULL || matrix_sum == NULL) {
		printf("Memory Allocation Failed.\n");
		return -1;
	}

	print_matrix(matrix_sum, row, col); 
	free_matrix(matrix_sum, row, col); // matrix_sum에 할당된 메모리 반납
	return 1;
}

/* 두 행렬의 뺄셈을 수행하는 함수 */
int subtraction_matrix(int** matrix_a, int** matrix_b, int row, int col)
{
    int** matrix_sub = create_matrix(row, col);
	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}
	for (int matrix_row = 0; matrix_row < row; matrix_row++) {
		for (int matrix_col = 0; matrix_col < col; matrix_col++)
        // 이중 for문을 행과 열의 크기만큼 돌며 matrix_a와 matrix_b의 같은 위치의 성분들을 뺀 값(matrix_a의 값에서 matrix_b값을 뺌)을 matrix_sum의 같은 위치에 대입
			matrix_sub[matrix_row][matrix_col] = matrix_a[matrix_row][matrix_col] - matrix_b[matrix_row][matrix_col];
	}
	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 -1 반환하며 함수 종료 */
    if (matrix_a == NULL || matrix_b == NULL || matrix_sub == NULL) {
		printf("Memory Allocation Failed.\n");
		return -1;
	}
	print_matrix(matrix_sub, row, col);
	free_matrix(matrix_sub, row, col); // matrix_sum에 할당된 메모리 반납

	return 1;
}

/* 행렬의 전치행렬을 구하는 함수 */
int transpose_matrix(int** matrix, int** matrix_t, int row, int col)
{
	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}
	for (int matrix_row = 0; matrix_row < row; matrix_row++) {
		for (int matrix_col = 0; matrix_col < col; matrix_col++)
        // 이중 for문을 행과 열의 크기만큼 돌며 해당 matrix의 열에 해당하는 모든 성분들을 차례대로 matrix_t의 해당 행에 대입
			matrix_t[matrix_row][matrix_col] = matrix[matrix_col][matrix_row];
	}
	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 -1 반환하며 함수 종료 */
    if (matrix == NULL || matrix_t ==NULL) {
		printf("Memory Allocation Failed.\n");
		return -1;
	}

	return 1;
}

/* 두 행렬의 곱셈을 수행하는 함수 */
int multiply_matrix(int** matrix_a, int** matrix_t, int row, int col)
{
    int** matrix_axt = create_matrix(row, row);
    // 두 행렬의 곱의 결과를 저장할 row x row 크기의 행렬 matrix_axt 선언
	// (m x n 크기 행렬과 n x m 크기 행렬의 곱은 m x m 크기의 행렬로 표현됨)

	/* 전처리 - 행과 열의 값이 음수일 경우 -1 반환하며 함수종료 */
	if (row <= 0 || col <= 0) {
		printf("Check the size of row and col!\n");
		return -1;
	}

	for (int matrix_a_row = 0; matrix_a_row < row; matrix_a_row++) {
		for (int matrix_t_row = 0; matrix_t_row < row; matrix_t_row++) {
		// 이중 for문을 matrix_a의 row(행의 크기)와 matrix_t의 row(열의 크기, 전치행렬은 col x row 크기의 행렬로 정의함)만큼 돌면서 계산한 값을 temp에 저장한 후 matrix_axt에 그 값을 대응되는 위치에 대입
			int temp = 0;
			for (int matrix_col = 0; matrix_col < col; matrix_col++)
				temp += matrix_a[matrix_a_row][matrix_col] * matrix_t[matrix_col][matrix_t_row];
			matrix_axt[matrix_a_row][matrix_t_row] = temp;
			// 가령, matrix_axt의 1행 1열의 성분은 matrix_a의 1행의 성분들과 matrix_t의 1열의 성분들을 각각 곱한 값이 matrix_t의 행 개수만큼 반복하며 더해진 값으로, 나머지 성분들도 같은 과정을 통해 계산됨

		}
	}

	/* 후처리 - 메모리 할당이 정상적으로 이루어지지 않은 경우 -1 반환하며 함수 종료 */
    if (matrix_t == NULL || matrix_axt ==NULL) {
		printf("Memory Allocation Failed.\n");
		return -1;
	}
	print_matrix(matrix_axt, row, row);
	free_matrix(matrix_axt, row, col);
	// matrix_axt에 할당된 메모리 반납
	return 1;
}

