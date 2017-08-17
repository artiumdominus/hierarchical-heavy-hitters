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
#include <list>
#include "lattice.cpp"

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
		
		list<Tuple> S, R;
		ip source, destination;
		double count;
		Tuple tuple;
		list<Tuple>::iterator e;
		bool found;
		
		while(getline(file, s)) {
			line << s;
			line >> IP >> number[0] >> dot >> number[1] >> dot >> number[2] >> dot >> number[3] >> dot >> number[4] >> arrow
			>> number[5] >> dot >> number[6] >> dot >> number[7] >> dot >> number[8] >> dot >> number[9] >> colon >> tcp >> number[10];
			
			if(!strcmp(IP, "IP") && !strcmp(tcp, "tcp") && number[10] > 0) {
				source.byte[0] = number[0];
				source.byte[1] = number[1];
				source.byte[2] = number[2];
				source.byte[3] = number[3];
				destination.byte[0] = number[5];
				destination.byte[1] = number[6];
				destination.byte[2] = number[7];
				destination.byte[3] = number[8];
				count = number[10];
				tuple = Tuple(source, destination, Label(4,4),count);
				found = false;
				for(e = S.begin(); e != S.end(); ++e) {
					if(tuple.equals(*e)) {
						e->count += tuple.count;
						found = true;
						break;
					}
				}
				if(!found) {
					S.push_back(tuple);
				}
			}
			line.clear();
		}
		file.close();
		
		int x;
		for(e = S.begin(); e != S.end(); ++e) {
			for(int i = 0; i < 4; ++i) {
				x = e->source.byte[i];
				if(x >= 0) {
					cout << x;
				} else {
					cout << x+256;
				}
				if(i < 3) {
					cout << ".";
				}
			}
			cout << " ";
			for(int i = 0; i < 4; ++i) {
				x =  e->destination.byte[i];
				if(x >= 0) {
					cout << x;
				} else {
					cout << x+256;
				}
				if(i < 3) {
					cout << ".";
				}
			}
			cout << " " << e->count << endl;
		}
		
	} else {
		cout << "Unable to open file :C";
	}
	
	return 0;
}