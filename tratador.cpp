/*
Trata um arquivo gerado por tcpdump filtrando as linhas que se encaixam no padrao:
IP x.y.z.w.v > m.n.o.p.q: tcp s
e as resumindo para o padrao:
x.y.z.w m.n.o.p s
Assim gerando uma entrada formatada para o algoritimo Overlap_Offline.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <sstream>

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

	if(file.is_open()) {
		char linha[100];
		string s;
		stringstream line;
		char IP[5], dot, arrow, colon, tcp[5];
		int number[11];

		while(getline(file, s)) {
			line << s;
            line >> IP >> number[0] >> dot >> number[1] >> dot >> number[2] >> dot >> number[3] >> dot >> number[4] >> arrow
            >> number[5] >> dot >> number[6] >> dot >> number[7] >> dot >> number[8] >> dot >> number[9] >> colon >> tcp >> number[10];
            if(!strcmp(IP, "IP") && !strcmp(tcp, "tcp") && number[10] > 0) {
            	cout << number[0] << '.' << number[1] << '.' << number[2] << '.' << number[3] << ' ' << number[5] << '.' << number[6]
                << '.' << number[7] << '.' << number[8] << ' ' << number[10] << endl;
			}
			line.clear();
		}
		file.close();
		
	} else {
		cout << "Unable to open file :C";
	}
	return 0;
}
