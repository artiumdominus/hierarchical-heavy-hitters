#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <ctime>

using namespace std;

union ip {
	long full;
	char byte[4];
};

class Label {
	public:
		unsigned short origin_esp : 3;
		unsigned short destiny_esp : 3;
		
		Label(int origin, int destiny) {
			this->origin_esp = origin;
			this->destiny_esp = destiny;
		}
		
		Label(){}
		
		bool equals(Label label){
			return this->origin_esp == label.origin_esp && this->destiny_esp == label.destiny_esp;
		}
};

class Tuple {
	public:
		Label label;
		ip origin;
		ip destiny;
		long count;
		
		Tuple(ip origin, ip destiny, Label label, int count = 0) {
			this->count = count;
			this->origin = origin;
			this->destiny = destiny;
			this->label = label;
		}
		
		Tuple(){}
		
		Tuple generalizeTo(Label label) {
			ip general_origin, general_destiny;
			int i;
			general_origin = this->origin;
			general_destiny = this->destiny;
			for(i = label.origin_esp; i < 4; ++i){
				general_origin.byte[i] = 0;
			}
			for(i = label.destiny_esp; i < 4; ++i){
				general_destiny.byte[i] = 0;
			}
			return Tuple(general_origin, general_destiny, label);
		}
		
		bool isAGeneralizationOf(Tuple x) {
			if(x.label.origin_esp < this->label.origin_esp || x.label.destiny_esp < this->label.destiny_esp) {
				return false;
			}
			Tuple y = x.generalizeTo(this->label);
			return this->origin.full == y.origin.full && this->destiny.full == y.destiny.full;
		}
		
		bool iamthekidyouknowwhatimean(Tuple e, list<Tuple> *hhh) {
			list<Tuple>::iterator h;
			for(h = hhh->begin(); h != hhh->end(); ++h) {
				if (h->isAGeneralizationOf(e) && this->isAGeneralizationOf(*h)) {
					return true;
				}
			}
			return false;
		}
		
		bool equals(Tuple tuple) {
			return this->label.equals(tuple.label) && this->origin.full == tuple.origin.full && this->destiny.full == tuple.destiny.full;
		}
		
		void print(){
			int i;
			cout << "(origin: ";
			for(i = 0; i < label.origin_esp; ++i) {
				cout << (int)origin.byte[i];
				if(i < 3) {
					cout << ".";
				}
			}
			if(label.origin_esp != 4) {
				cout << "*";
			}
			cout << ", destiny: ";
			for(i = 0; i < label.destiny_esp; ++i) {
				cout << (int)destiny.byte[i];
				if(i < 3) {
					cout << ".";
				}
			}
			if(label.destiny_esp != 4) {
				cout << "*";
			}
			cout << ")" << endl;
		}
};

void init_level(list<Label> level[]) {
	level[0].insert(level[0].end(), Label(0,0));
	level[1].insert(level[1].end(), Label(0,1));
	level[1].insert(level[1].end(), Label(1,0));
	level[2].insert(level[2].end(), Label(0,2));
	level[2].insert(level[2].end(), Label(1,1));
	level[2].insert(level[2].end(), Label(2,0));
	level[3].insert(level[3].end(), Label(0,3));
	level[3].insert(level[3].end(), Label(1,2));
	level[3].insert(level[3].end(), Label(2,1));
	level[3].insert(level[3].end(), Label(3,0));
	level[4].insert(level[4].end(), Label(0,4));
	level[4].insert(level[4].end(), Label(1,3));
	level[4].insert(level[4].end(), Label(2,2));
	level[4].insert(level[4].end(), Label(3,1));
	level[4].insert(level[4].end(), Label(4,0));
	level[5].insert(level[5].end(), Label(1,4));
	level[5].insert(level[5].end(), Label(2,3));
	level[5].insert(level[5].end(), Label(3,2));
	level[5].insert(level[5].end(), Label(4,1));
	level[6].insert(level[6].end(), Label(2,4));
	level[6].insert(level[6].end(), Label(3,3));
	level[6].insert(level[6].end(), Label(4,2));
	level[7].insert(level[7].end(), Label(3,4));
	level[7].insert(level[7].end(), Label(4,3));
	level[8].insert(level[8].end(), Label(4,4));
}

list<Tuple> overlap(list<Tuple> S, float phi) {
	const int L = 8; // L = h1 + h2 + ... + hd
	long N = 0;
	list<Tuple> hhh;
	hhh.clear(); // hhh <- empty
	list<Label> level[9];
	init_level(level);
	list<Label>::iterator label;
	list<Tuple> interlist;
	list<Tuple>::iterator e, f;
	Tuple p;
	bool found;
	
	for(e = S.begin(); e != S.end(); ++e) { // Computa o volume de dados
		N += e->count;
	}
	int phiN = phi * N;
	
	for(int l = L; l > 0; --l) { // For all levels
		for(label = level[l].begin(); label != level[l].end(); ++label) { // For all labels in level(l)
			interlist.clear(); //list <- empty
			for(e = S.begin(); e != S.end(); ++e) { // for all (e element of S)
				p = e->generalizeTo(*label);
				if(!p.iamthekidyouknowwhatimean(*e, &hhh)){ // if ¬(Exist an h element of hhh : (isAGeneralizationOf(h,e) and isAGeneralizationOf(p,h)
					found = false;
					for(f = interlist.begin(); f != interlist.end(); ++f) { 
						if(p.equals(*f)){                  // if p belongs to list
							f->count += e->count; //fp += fe
							found = true;
							break;
						}
					}
					if(!found) {          // else
						p.count = e->count;  // fp <- fe
						interlist.insert(interlist.end(), p); // list <- list union {p} 
					}
				}
			}
			for(e = interlist.begin(); e != interlist.end(); ++e) { // for all e that belongs to list
				if(e->count > phiN) {  // if (fe > floor(phi * N))
					hhh.insert(hhh.end(), *e); // hhh <- hhh union {e}
				}
			}
		}
	}
	
	return hhh;
}

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
		int byte[8], count;
		char dot;
		ip origin, destiny;
		list<Tuple> S;
		
		while(!file.eof()) {
			file >> byte[0] >> dot >> byte[1] >> dot >> byte[2] >> dot >> byte[3]
			>> byte[4] >> dot >> byte[5] >> dot >> byte[6] >> dot >> byte[7] >> count;
			origin.byte[0] = byte[0];
			origin.byte[1] = byte[1];
			origin.byte[2] = byte[2];
			origin.byte[3] = byte[3];
			destiny.byte[0] = byte[4];
			destiny.byte[1] = byte[5];
			destiny.byte[2] = byte[6];
			destiny.byte[3] = byte[7];
			
			S.insert(S.end(), Tuple(origin, destiny, Label(4,4), count));
		}
		
		float phi;
		if(argc < 3){
			cout << "Entre com um valor para phi: ";
			cin >> phi;
		} else {
			phi = atof(argv[2]);
		}
		
		time_t beginning, end;
		time(&beginning);
		list<Tuple> result = overlap(S, phi);
		time(&end);
		
		list<Tuple>::iterator i;
		for(i = result.begin(); i != result.end(); ++i) {
			i->print();
		}
		
		cout << "Duration: " << difftime(end, beginning) << " seconds" << endl;
		
	} else {
		cout << "Unable to open file :C";
	}
	return 0;
}
