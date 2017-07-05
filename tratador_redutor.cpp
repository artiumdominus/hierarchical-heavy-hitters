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

using namespace std;

union ip {
	long full;
	char byte[4];
};

class Tuple {
	public:
		ip origin;
		ip destination;
		long count;
	
		Tuple(ip origin, ip destination, long count) {
			this->origin = origin;
			this->destination = destination;
			this->count = count;
		}
	
		Tuple(){}
	
		bool equals(Tuple tuple) {
			return this->origin.full == tuple.origin.full && this->destination.full == tuple.destination.full;
		}
	
		void print() {
			int i;
			for(i = 0; i < 4; ++i) {
				cout << (int)origin.byte[i];
				if(i < 3) {
					cout << ".";
				}
			}
			cout << " ";
			for(i = 0; i < 4; ++i) {
				cout << (int)destination.byte[i];
				if(i < 3) {
					cout << ".";	
				}
			}
			cout << " " << count << endl;
		}
};

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
		
		list<Tuple> S;
		ip origin, destination;
		double count;
		Tuple tuple;
		list<Tuple>::iterator i;
		bool found;

		while(getline(file, s)) {
			line << s;
            line >> IP >> number[0] >> dot >> number[1] >> dot >> number[2] >> dot >> number[3] >> dot >> number[4] >> arrow
            >> number[5] >> dot >> number[6] >> dot >> number[7] >> dot >> number[8] >> dot >> number[9] >> colon >> tcp >> number[10];
			
            if(!strcmp(IP, "IP") && !strcmp(tcp, "tcp") && number[10] > 0) {
            	cout << number[0] << '.' << number[1] << '.' << number[2] << '.' << number[3] << ' ' << number[5] << '.' << number[6]
                << '.' << number[7] << '.' << number[8] << ' ' << number[10] << endl;
				origin.byte[0] = number[0];
				origin.byte[1] = number[1];
				origin.byte[2] = number[2];
				origin.byte[3] = number[3];
				destination.byte[0] = number[5];
				destination.byte[1] = number[6];
				destination.byte[2] = number[7];
				destination.byte[3] = number[8];
				count = number[10];
				tuple = Tuple(origin, destination, count);
				found = false;
				for(i = S.begin(); i != S.end(); ++i) {
					if(tuple.equals(*i)) {
						i->count += tuple.count;
						found = true;
						break;
					}
				}
				if(!found) {
					S.insert(S.end(), tuple);
				}
			}
			line.clear();
		}
		file.close();
		
		for(i = S.begin(); i != S.end(); ++i) {
			i->print();
		}
		
	} else {
		cout << "Unable to open file :C";
	}
	return 0;
}
