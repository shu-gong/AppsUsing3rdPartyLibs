#include <mpi.h>
#include<fstream>
#include <sstream> 
#include <iostream>
#include<vector>
#include <time.h>
//#include <sys/time.h>
//#define N 100
//#define myarysize N / 4

using namespace std;
int main(int argc, char* argv[])
{

	// myid defines the NO of each process, while numprocs marks the total number of processors
	int n, myid, numprocs;
	// the border process ranked 0 and (numproc - 1)
	int up, down;
	int i, j;

	// determine the size of the array needing itering
	int N;

	// know the size of the array each process has
	int myarysize;
	int myarysizeExtra;

	// define the num of iterations 
	int nsteps;

	MPI_Datatype  onerow;
	MPI_Status  status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	// judge the args
	if (argc < 5)
	{
		cout << "Invalid parameters, please check your values." << endl;
		return -1;
	}
	else
	{
		if (atoi(argv[2]) <= 0 || atoi(argv[4]) <= 0)
		{
			cout << "Invalid parameters, please check your values." << endl;
			return 0;
		}
		if (atoi(argv[2]) > 256 || atoi(argv[4]) > 10000)
		{
			cout << " the first param is up to 256 and the second up to 10000." << endl;
			return 0;
		}
	}

	// initialize all arrays claimed above	
	N = atoi(argv[2]);
	//N = 256;

	//nsteps = 10000;
	nsteps = atoi(argv[4]);

	myarysize = N / numprocs;
	myarysizeExtra = N / numprocs + N % numprocs;

	vector<vector<double>> prev(myarysize + 2, vector<double>(N + 2));
	vector<vector<double>> curr(myarysize + 2, vector<double>(N + 2));

	vector<vector<double>> prevExtra(myarysizeExtra + 2, vector<double>(N + 2));
	vector<vector<double>> currExtra(myarysizeExtra + 2, vector<double>(N + 2));


	// define the temp_1d designed for saving the outcome of iterations
	double* temp_1d = (double*)malloc(sizeof(double) * myarysize * (N + 2));
	double* temp_1d_extra = (double*)malloc(sizeof(double) * myarysizeExtra * (N + 2));

	// this 1d array is used for collecting data coming from all processes
	double* gather_arr = (double*)malloc(sizeof(double) * N * (N + 2));
	double* gather_arr_all = (double*)malloc(sizeof(double) * N * (N + 2));

	clock_t start, end;
	start = clock();

	for (size_t i = 0; i < N * (N + 2); i++)	gather_arr[i] = 20.00;
	for (size_t i = 0; i < N * (N + 2); i++)	gather_arr_all[i] = 20.00;


	MPI_Type_contiguous(N + 2, MPI_DOUBLE, &onerow);
	MPI_Type_commit(&onerow);

	up = myid - 1;
	if (up < 0) up = MPI_PROC_NULL;// when the current process is the number 0 process, it needn't transfer data to the number -1 process,and hence set NULL_process
	down = myid + 1;
	if (down > (numprocs - 1)) down = MPI_PROC_NULL;// when the current process is the NO (numproc - 1) process, it needn't transfer data to the number (numproc) process,and therefore set NULL_process
	int begin_row = 1;

	int end_row = myarysize;
	int end_row_extra = myarysizeExtra;


	if (myid == 0)
	{

		for (int i = 0; i < myarysizeExtra + 2; i++)for (int j = 0; j < N + 2; j++)	prevExtra[i][j] = 20.00;

		for (int j = 0; j < N + 2; j++)
			if (j > 0.3 * (N + 1) && j < 0.7 * (N + 1))
				prevExtra[0][j] = 100.00;

		// this is a kind of new datatype assembling the concrete length of data we want to transfer from process to process



		// timestamp 1 - marking the start time of iterations
		//struct timeval t1, t2;
		//gettimeofday(&t1, 0);

		for (n = 0; n < nsteps; n++)
		{
			MPI_Sendrecv(&prevExtra[1][0], 1, onerow, up, 1000, &prevExtra[myarysizeExtra + 1][0], 1, onerow, down, 1000, MPI_COMM_WORLD, &status);
			MPI_Sendrecv(&prevExtra[myarysizeExtra][0], 1, onerow, down, 1000, &prevExtra[0][0], 1, onerow, up, 1000, MPI_COMM_WORLD, &status);
			for (i = begin_row; i <= end_row_extra; i++)
			{
				for (j = 1; j < N + 1; j++)
				{
					currExtra[i][j] = (prevExtra[i][j + 1] + prevExtra[i][j - 1] + prevExtra[i - 1][j] + prevExtra[i + 1][j]) * 0.25;

				}
			}
			for (i = begin_row; i <= end_row_extra; i++)
				for (j = 1; j < N + 1; j++)
					prevExtra[i][j] = currExtra[i][j];


		}/* iteration over*/

		// use a 1d temporary array to save these data that need removing the first and the last line（because these two lines are borders used in exchange with other processes）

		int k = 0;
		for (int i = begin_row; i <= end_row_extra; i++)
		{
			for (int j = 0; j < N + 2; j++)
			{
				temp_1d_extra[k] = prevExtra[i][j];
				k++;
			}
		}

		k = 0;
		for (int i = begin_row; i <= end_row; i++)
		{
			for (int j = 0; j < N + 2; j++)
			{
				temp_1d[k] = prevExtra[i][j];
				k++;
			}
		}


	}
	else
	{

		for (int i = 0; i < myarysize + 2; i++)for (int j = 0; j < N + 2; j++)	prev[i][j] = 20.00;



		for (n = 0; n < nsteps; n++)
		{
			MPI_Sendrecv(&prev[1][0], 1, onerow, up, 1000, &prev[myarysize + 1][0], 1, onerow, down, 1000, MPI_COMM_WORLD, &status);
			MPI_Sendrecv(&prev[myarysize][0], 1, onerow, down, 1000, &prev[0][0], 1, onerow, up, 1000, MPI_COMM_WORLD, &status);
			for (i = begin_row; i <= end_row; i++)
			{
				for (j = 1; j < N + 1; j++)
				{
					curr[i][j] = (prev[i][j + 1] + prev[i][j - 1] + prev[i - 1][j] + prev[i + 1][j]) * 0.25;

				}
			}
			for (i = begin_row; i <= end_row; i++)
				for (j = 1; j < N + 1; j++)
					prev[i][j] = curr[i][j];


		}/* iteration over*/

		// use a 1d temporary array to save these data that need removing the first and the last line（because these two lines are borders used in exchange with other processes）
		int k = 0;
		for (int i = begin_row; i <= end_row; i++)
		{
			for (int j = 0; j < N + 2; j++)
			{
				temp_1d[k] = prev[i][j];
				k++;
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (myid == 0)
	{
		// print the time
		end = clock();
		double interval = (double)(end - start) / CLOCKS_PER_SEC;
		// timestamp2 - marking the end of iterations
		//gettimeofday(&t2, 0);
		//double time = (1000000.0 * (t2.tv_sec - t1.tv_sec) + t2.tv_usec - t1.tv_usec) / 1000.0;
		printf("Time to generate:  %4.2f ms \n", interval * 1000);
	}


	MPI_Gather(temp_1d, myarysize * (N + 2), MPI_DOUBLE, gather_arr, myarysize * (N + 2), MPI_DOUBLE, 0, MPI_COMM_WORLD);

	if (myid == 0)
	{
		int k = 0;
		for (int i = 1; i < myarysizeExtra * (N + 2); i++)
		{
			gather_arr_all[i] = temp_1d_extra[i];
			//gather_arr_all[i] = 1.11;
			k++;
		}
		int j = 0;
		for (int i = myarysize * (N + 2); i < N * (N + 2); i++)
		{
			gather_arr_all[i + (myarysizeExtra - myarysize) * (N + 2)] = gather_arr[i];
			//gather_arr_all[i + (myarysizeExtra - myarysize) * (N + 2)] = 2.22;
			
		}

	}


	MPI_Type_free(&onerow);
	MPI_Finalize();

	// back to the root process
	if (myid == 0)
	{
		// output the result to the csv file
		std::ofstream myfile;
		myfile.open("finalTemperatures.csv");
		for (int p = 0; p < (N + 2) * (N + 2); p++)
		{

			std::ostringstream out;
			out.precision(8);
			if (p % (N + 2) == 0 && p != 0)
			{
				myfile << "\n";
			}
			if (p < (N + 2))
			{
				if (p > 0.3 * (N + 1) && p < 0.7 * (N + 1))
					myfile << "100" << ", ";
				else
					myfile << "20" << ", ";
			}
			else if (p >= (N + 2) * (N + 1))
				myfile << "20" << ", ";
			else
			{
				out << gather_arr_all[p - (N + 2)];
				std::string str = out.str(); //format
				myfile << str << ", ";
			}
		}
		myfile.close();
	}

	free(gather_arr);
	free(gather_arr_all);
	free(temp_1d);
	free(temp_1d_extra);
	std::vector<vector<double>>().swap(prev);
	std::vector<vector<double>>().swap(curr);

	return 1;
}
