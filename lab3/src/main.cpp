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
	vector<vector<complex_number>> A;
	vector<vector<complex_number>> B;
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
	/*vector<vector<complex_number>> A(n1, vector<complex_number>(m1));
	vector<vector<complex_number>> B(n2, vector<complex_number>(m2));
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			A[i][j] = arguments->A[i][j];
		}
	}
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			B[i][j] = arguments->B[i][j];
		}
	}*/
	for (int i = 0; i < partition; i++) {
		//cout << "считается ячейка :" << cell << endl;
		int I = num_of_thread * partition + i;
		for (int J = 0; J < m_ans; J++) {
			for (int k = 0; k < m1; k++) {
				complex_number temp = complex_multiplication((arguments->A)[I][k], (arguments->B)[k][J]);
				answer[I][J] = complex_adition(answer[I][J], temp);
			}
			cell++;
		}
	}
	return 0;
}
 
int main(int argc, char const *argv[])
{
	string command;
	int n1, n2, m1, m2;
	vector<vector<complex_number>> A;
	vector<vector<complex_number>> B;
 	cout << "1) Напишите work, если хотите переумножить матрицы" << endl << "2) Введите test, если хотите провести тест скорости" << endl;
 	cin >> command;
 	if (command == "work") {
 		cout << "Ведите размер первой матрицы" << endl;
		cin >> n1 >> m1;
 		A.resize(n1, vector<complex_number>(m1));
		cout << "Ведите первую матрицу" << endl;
		for (int i = 0; i < n1; i++) {
			for (int j = 0; j < m1; j++) {
				A[i][j] = read_complex_number();
			}	
		}
		cout << "Ведите размер второй матрицы" << endl;
		cin >> n2 >> m2;
		B.resize(n2, vector<complex_number>(m2,complex_number{}));
		cout << "Введите вторую матрицу" << endl;
		for (int i = 0; i < n2; i++) {
			for (int j = 0; j < m2; j++) {
				B[i][j] = read_complex_number();
			}
		}
 	}
 	else if (command == "test"){
 		cout << "Ведите размер первой матрицы" << endl;
		cin >> n1 >> m1;
		cout << "Ведите размер второй матрицы" << endl;
		cin >> n2 >> m2;
 		A.resize(n1, vector<complex_number>(m1,complex_number{}));
 		B.resize(n2, vector<complex_number>(m2,complex_number{}));
 	}
	/*cout << "Ведите размер первой матрицы" << endl;
	cin >> n1 >> m1;
	vector<vector<complex_number>> A(n1, vector<complex_number>(m1,complex_number{}));
	vector<vector<complex_number>> A(n1, vector<complex_number>(m1));
	cout << "Ведите первую матрицу" << endl;
	for (int i = 0; i < n1; i++) {
		for (int j = 0; j < m1; j++) {
			A[i][j] = read_complex_number();
		}	
	}
	cout << "Ведите размер второй матрицы" << endl;
	cin >> n2 >> m2;*/

	if (m1 != n2) {
		cout << "Матрицы таких размеров нельзя перемножить" << endl;
		return 0;
	}

	int n_ans = n1;
	int m_ans = m2;

	/*vector<vector<complex_number>> B(n2, vector<complex_number>(m2,complex_number{}));
	vector<vector<complex_number>> B(n2, vector<complex_number>(m2));
	cout << "Введите вторую матрицу" << endl;
	for (int i = 0; i < n2; i++) {
		for (int j = 0; j < m2; j++) {
			B[i][j] = read_complex_number();
		}
	}*/
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

	cout << "введите колличество потоков" << endl;
	int count_threads;
	cin >> count_threads;

	pthread_t threads[count_threads];
	pthread_mutex_init(&mutex, NULL);
	int partition = n_ans / count_threads;

	if (count_threads > n_ans*m_ans) {
		cout << "Введено слишком много потоков" << endl;
		count_threads = n_ans;
	}
 	
	struct arg_to_thread arg;
	arg.partition = partition;
	arg.count_threads = count_threads;
	arg.n1 = n1;
	arg.m1 = m1;
	arg.n2 = n2;
	arg.m2 = m2;
	arg.n_ans = n_ans;
	arg.m_ans = m_ans;

	arg.A.resize(n1, vector<complex_number>(m1));
	arg.B.resize(n2, vector<complex_number>(m2));

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

    chrono::high_resolution_clock::time_point begin = chrono::high_resolution_clock::now();
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
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
 
	pthread_mutex_destroy(&mutex);
	if (command == "work"){
		cout << "Получившаяся матрица:" << endl;
		for (int i = 0; i < n_ans; i++) {
			for (int j = 0; j < m_ans; j++) {
				write_complex_number(answer[i][j]);
				cout << " ";
			}
			cout << endl;
		}
	}
    cout<< "программа выполнена за :" << chrono::duration_cast<chrono::milliseconds>(end-begin).count() << " миллисекунды" << endl;
	return 0;
 
}
