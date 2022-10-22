#include <unistd.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int get_number(string &s) {
	if (s.size() == 0) {
		return -1;
	}
	if (s[s.size() - 1] != ' ') {
		s += ' ';
	}
	int ans = 0;
	int i = 0;
	string temp;
	while (s[i] != ' ') {
		temp += s[i];
		i++;
	}
	s.erase(s.begin(), s.begin() + i + 1);
	ans = stoi(temp);
	return ans;
}

int main(int argc, char *argv[]){
	int pipefd[2];
	string file_name = argv[2];
	pipefd[0] = atoi(argv[0]);
	pipefd[1] = atoi(argv[1]);
	//cout << file_name;
	string s;
	ifstream fin(file_name);
	//fin >> s;
	while(!fin.eof()) {
		getline(fin, s);
		// << s << endl;
		//cout << s << endl;	
		int a = get_number(s);
		bool error = false;
		while(true) {
			int del = get_number(s);
			if (del == -1) {
				break;
			}
			if (del == 0) {
				error = true;
				break;
			}
			a /= del;
		}
		//cout << a << endl;
		if (!error) { 
			write(pipefd[1], &a, sizeof(int));
		}
	}
	fin.close();
	close(pipefd[0]);
	close(pipefd[1]);
	return 0;
}
