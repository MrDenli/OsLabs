#include<pthread.h>
#include<iostream>
#include<ctime> 
#include<vector>
#include<fstream>
#include<chrono>
 
using namespace std;
 
pthread_mutex_t mutex;
int flag = 0;
int cell = 0;
 
struct complex_number {
	int a;
	int b;
};
 
vector<vector<complex_number>> answer;
 
complex_number complex_multiplication(complex_number X, complex_number Y) {
	complex_number ans;
	ans.a = X.a * Y.a + (-1) * X.b * Y.b;
	ans.b = X.a * Y.b + X.b * Y.a;
	return ans;
}
 
complex_number complex_adition(complex_number X, complex_number Y) {
	complex_number ans;
	ans.a = X.a + Y.a;
	ans.b = X.b + Y.b;
	return ans;
}
 
void write_complex_number(complex_number X) {
	cout << X.a;
	if (X.b == 0) {
		return;
	}
	if (X.b > 0) {
		cout << "+";
	}
	cout << X.b << "i";
	return;
}
 
complex_number read_complex_number() {
	complex_number ans;
	cin >> ans.a;
	cin >> ans.b;
	char temp;
	cin >> temp;
	return ans;
}
 
 
struct arg_to_thread {
	complex_number A[100][100];
	complex_number B[100][100];
	int partition;
	int num_of_thread;
	int count_threads;
	int n1;
	int m1;
	int n2;
	int m2;
	int n_ans;
	int m_ans;
};
 
void* thread_func(void *args)
{	
	arg_to_thread* arguments = (arg_to_thread*) args;
	int num_of_thread = arguments->num_of_thread;
	int partition = arguments->partition;
	flag = 1;
	int count_threads = arguments->count_threads;
	int n1 = arguments->n1;
	int m1 = arguments->m1;
	int n2 = arguments->n2;
	int m2 = arguments->m2;
	int n_ans =  arguments->n_ans;
	int m_ans = arguments->m_ans;
	complex_number A[100][100];
	complex_number B[100][100];
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			A[i][j] = arguments->A[i][j];
		}
	}
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			B[i][j] = arguments->B[i][j];
		}
	}
	for (int i = 0; i < partition; i++) {
		//cout << "считается ячейка :" << cell << endl;
		int I = cell / m_ans;
		int J = cell % m_ans;
		for (int k = 0; k < m1; k++) {
			complex_number temp = complex_multiplication(A[I][k], B[k][J]);
			answer[I][J] = complex_adition(answer[I][J], temp);
		}
		cell++;
	}
	return 0;
}
 
int main(int argc, char const *argv[])
{
 
	int count_threads;
	cout<<"Введите количество потоков:" << endl;
	cin >>count_threads;
	int n1, n2, m1, m2;
	cout << "Ведите размер первой матрицы" << endl;
	cin >> n1 >> m1;
	vector<vector<complex_number>> A(n1, vector<complex_number>(m1));
	cout << "Ведите первую матрицу" << endl;
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			A[i][j] = read_complex_number();
		}
	}
	cout << "Ведите размер второй матрицы" << endl;
	cin >> n2 >> m2;
	if (m1 != n2) {
		cout << "Матрицы таких размеров нельзя перемножить" << endl;
		return 0;
	}
	vector<vector<complex_number>> B(n2, vector<complex_number>(m2));
	cout << "Введите вторую матрицу" << endl;
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			B[i][j] = read_complex_number();
		}
	}
	answer.resize(n1);
	for (int i = 0; i < n1; i++) {
		answer[i].resize(m2);
	}
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m2; j++) {
			answer[i][j].a = 0;
			answer[i][j].b = 0;
		}
	}
 
	pthread_t threads[count_threads];
	pthread_mutex_init(&mutex, NULL);
	int partition = n1 * m2 / count_threads;
 
	int n_ans = n1;
	int m_ans = m2;
 
	struct arg_to_thread arg;
	arg.partition = partition;
	arg.count_threads = count_threads;
	arg.n1 = n1;
	arg.m1 = m1;
	arg.n2 = n2;
	arg.m2 = m2;
	arg.n_ans = n_ans;
	arg.m_ans = m_ans;
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			arg.A[i][j].a = A[i][j].a;
			arg.A[i][j].b = A[i][j].b;
		}
	}
 
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			arg.B[i][j].a = B[i][j].a;
			arg.B[i][j].b = B[i][j].b;
		}
	}
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	for (int i = 0; i < count_threads; i++) {
		arg.num_of_thread = i;
		if (i == count_threads - 1) {
			partition += (n1 * m2 % count_threads);
			arg.partition = partition;
		}
		int status = pthread_create(&threads[i], NULL, thread_func, (void*)&arg);
		flag = 0;
		if (status != 0) {
			cout<<"Create thread error"<<endl;
		}
	}
 
	for (int i = 0; i < count_threads; ++i) {
		pthread_join(threads[i], NULL);
	}
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
 
	pthread_mutex_destroy(&mutex);
	cout << "Получившаяся матрица:" << endl;
	for (int i = 0; i < n_ans; i++) {
		for (int j = 0; j < m_ans; j++) {
			write_complex_number(answer[i][j]);
			cout << " ";
		}
		cout << endl;
	}
    cout<<chrono::duration_cast<chrono::microseconds>(end-begin).count() << endl;
	return 0;
 
}
