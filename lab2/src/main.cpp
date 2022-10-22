#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
using namespace std;

int main(){
	string file, name;
	cin >> name;
	int rez;
	char *const child_args[] = { "./child", NULL };
	int pipefd[2];
	int pip = pipe(pipefd);
	if (pip == -1) {
		cout << "error with oppening the pipe";
		return 0;
	} 
	int id = fork();
	if (id == -1) {
		cout << "error with fork";
		return 0;
	} else if (id == 0) {
		execlp(child_args[0], to_string(pipefd[0]).c_str(), to_string(pipefd[1]).c_str(), name.c_str(), NULL);
	} else {
		while (read(pipefd[0], &rez, sizeof(int))) {
			if (rez == -1) {
				break;
			}
			cout << rez << endl;
		}
	}
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;	
}
