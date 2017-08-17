#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <ctime>
#include "lattice.cpp"

using namespace std;

list<Tuple> overlap(list<Tuple> S, float phi) {
	const int L = 8; // L = h1 + h2 + ... + hd
	long N = 0;
	list<Tuple> hhh;
	hhh.clear(); // hhh <- empty
	Level level;
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
				if(!p.iamthekidyouknowwhatimean(*e, &hhh)){ // if ¬(Exist an h element of hhh : (isAGeneralizationOf(h,e) and isAGeneralizationOf(p,h)))
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
						interlist.push_back(p); // list <- list union {p} 
					}
				}
			}
			for(e = interlist.begin(); e != interlist.end(); ++e) { // for all e that belongs to list
				if(e->count > phiN) {  // if (fe > floor(phi * N))
					hhh.push_back(*e); // hhh <- hhh union {e}
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
		ip source, destination;
		list<Tuple> S;
		
		while(!file.eof()) {
			file >> byte[0] >> dot >> byte[1] >> dot >> byte[2] >> dot >> byte[3]
			>> byte[4] >> dot >> byte[5] >> dot >> byte[6] >> dot >> byte[7] >> count;
			source.byte[0] = byte[0];
			source.byte[1] = byte[1];
			source.byte[2] = byte[2];
			source.byte[3] = byte[3];
			destination.byte[0] = byte[4];
			destination.byte[1] = byte[5];
			destination.byte[2] = byte[6];
			destination.byte[3] = byte[7];
			
			S.insert(S.end(), Tuple(source, destination, Label(4,4), count));
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
