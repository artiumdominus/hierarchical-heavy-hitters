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
	
	ifstream file;
	file.open(filename);
	string s;
	long count = 0;
	
	if(file.is_open()) {
		while(getline(file, s)) {
			count++;
		}
		cout << count << " lines" << endl;
	} else {
		cout << "Unable to open file :C" << endl;
	}
	
	return 0;
}