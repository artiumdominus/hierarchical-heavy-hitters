// Lista um determinado numero de linhas de um arquivo.

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <string.h>

using namespace std;

int main(int argc, char *argv[]) {
	char filename[30];
	if(argc < 2) {
		cout << "Entre com o nome do arquivo: ";
		cin >> filename;
	} else {
		strcpy(filename, argv[1]);
	}
	
	double limit;
	if(argc < 3){
		cout << "Entre com o numero limite de linhas: ";
		cin >> limit;
	} else {
		limit = atoi(argv[2]);
	}
	
	ifstream file;
	file.open(filename);
	string s;
	
	if(file.is_open()) {
		for(int i = 0; i < limit; ++i) {
			getline(file, s);
			cout << s << endl;
		}
	} else {
		cout << "Unable to open file :C" << endl;
	}
	
	return 0;
}
