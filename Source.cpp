#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <omp.h>
#include <locale.h>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <time.h>

using std::cout;
using std::cin;

double problem_1(int N) {
	srand(time(NULL));
	double** M1 = new double* [N];
	for (int i = 0; i < N; i++) {
		M1[i] = new double[N];
	}
	printf("Matrix M1\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			M1[i][j] = (double)rand() / RAND_MAX * (12 - 10) - 10;
			cout << M1[i][j] << " ";
		}
		cout << "\n";
	}
	double** E = new double* [N];
	for (int i = 0; i < N; i++) {
		E[i] = new double[N];
	}
	printf("Matrix E\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			E[i][j] = (double)rand() / RAND_MAX * (10 - 0) + 0;
			cout << E[i][j] << " ";
		}
		cout << "\n";
	}
	double** M2 = new double* [N];
	for (int i = 0; i < N; i++) {
		M2[i] = new double[N];
	}
	printf("Matrix M2\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			M2[i][j] = M1[i][j] - E[i][j];
			cout << M2[i][j] << " ";
		}
		cout << "\n";
	}
	double** M3 = new double* [N];
	for (int i = 0; i < N; i++) {
		M3[i] = new double[N];
	}
	//double start = clock();

	printf("Matrix M3\n");
	clock_t start, end;
	start = clock();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			M3[i][j] = 0.0;
			for (int k = 0; k < N; k++) {
				M3[i][j] += (M1[i][k] * M2[k][j]);
			}
			cout << M3[i][j] << "\t";
		}
		cout << "\n";
	}
	end = clock();
	printf("Время %lf \n", ((double)end - start) / ((double)CLOCKS_PER_SEC));

	for (int i = 0; i < N; i++)
	{
		delete[]M1[i];
		delete[]E[i];
		delete[]M2[i];
		delete[]M3[i];
	}
	delete[]M1;
	delete[]E;
	delete[]M2;
	delete[]M3;
	return ((double)end - start) / ((double)CLOCKS_PER_SEC);
}

double problem_2(int N, int num_threads) {
	srand(time(NULL));
	double** M1 = new double* [N];
	for (int i = 0; i < N; i++) {
		M1[i] = new double[N];
	}
	printf("Matrix M1\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			M1[i][j] = (double)rand() / RAND_MAX * (12 - 10) - 10;
			cout << M1[i][j] << " ";
		}
		cout << "\n";
	}
	double** E = new double* [N];
	for (int i = 0; i < N; i++) {
		E[i] = new double[N];
	}
	printf("Matrix E\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			E[i][j] = (double)rand() / RAND_MAX * (10 - 0) + 0;
			cout << E[i][j] << " ";
		}
		cout << "\n";
	}
	double** M2 = new double* [N];
	for (int i = 0; i < N; i++) {
		M2[i] = new double[N];
	}
	printf("Matrix M2\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			M2[i][j] = M1[i][j] - E[i][j];
			cout << M2[i][j] << " ";
		}
		cout << "\n";
	}
	double** M3 = new double* [N];
	for (int i = 0; i < N; i++) {
		M3[i] = new double[N];
	}
	omp_set_num_threads(num_threads);
	int i, j, k;
	printf("Matrix M3\n");
	double start_time = omp_get_wtime();
#pragma omp parallel for shared(M1, M2, M3) private(i,j,k)
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			M3[i][j] = 0.0;
			for (k = 0; k < N; k++) {
				M3[i][j] += (M1[i][k] * M2[k][j]);

			}
		}
	}
	double end_time = omp_get_wtime();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%f\t", M3[i][j]);
		}
		printf("\n");
	}
	cout << "----------------------------\n";
	printf("Время %lf \n", end_time - start_time);
	//printf("Точность таймера %lf ", omp_get_wtick());
	for (int i = 0; i < N; i++)
	{
		delete[]M1[i];
		delete[]E[i];
		delete[]M2[i];
		delete[]M3[i];
	}
	delete[]M1;
	delete[]E;
	delete[]M2;
	delete[]M3;
	return end_time - start_time;
}

int main()
{
	setlocale(LC_ALL, "RU");
	int N;
	int num_threads;
	/*scanf("%d", N);
	scanf("%d", num_threads);*/
	cin >> N;
	cin >> num_threads;

	double time1 = problem_1(N);

	cout << "Parallel solution\n";
	double time2 = problem_2(N, num_threads);

	printf("----------Расчёт ускорения и эффетиквности----------\n");
	float boost = time1 / time2;
	printf("Ускорение: %f\n", boost);
	printf("Эффективность: %f", boost/num_threads);
}