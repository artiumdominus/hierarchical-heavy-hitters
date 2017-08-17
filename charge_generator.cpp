#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "lattice.cpp"

using namespace std;

void spreader(int total, int array[], int array_size, int restriction) {
	int i, x, sum;
	int seeds = total;
	const float leveler = 0.5;

	srand(time(NULL));

	for(i = 0; i < array_size; ++i) {
		array[i] = 0 ;
	}

	do {
		sum = 0;
		for(i = 0; i < array_size; ++i) {
			if(seeds > 0) {
				x = rand() % ((seeds / (int)(array_size * leveler)) + 2); 
			} else {
				x = -( rand() % ((-seeds / array_size) + 2) );
			}
			if(array[i] + x >= 0 && i != restriction) {
				array[i] += x;
				sum += x;
			}
		}
		seeds -= sum;
	} while(seeds != 0);
}

void network_generator(ip network[], int network_size) {
	int a[4];
	srand(time(NULL));
	for(int i = 0; i < network_size; ++i) {
		if(i % 8 == 0) {
			a[0] = rand() % 256;
		}
		if(i % 4 == 0) {
			a[1] = rand() % 256;
		}
		if(i % 2 == 0) {
			a[2] = rand() % 256;
		}
		a[3] = rand() % 256;
		
		for(int j = 0; j <= 3; ++j){
			network[i].byte[j] = a[j];
		}
	}
}

list<Tuple> trace_generator(ip network[], int network_size, int volume) {
	list<Tuple> trace;
	
	int semidistribuition[network_size], sum = 0;
	
	spreader(volume, semidistribuition, network_size, -1);
	
	int distribuition[network_size][network_size];
	
	for(int i = 0; i < network_size; ++i) {
		spreader(semidistribuition[i], distribuition[i], network_size, i);
	}
	
	for(int i = 0; i < network_size; ++i) {
		for(int j = 0; j < network_size; ++j) {
			if(i != j && distribuition[i][j] > 0) {
				trace.push_back(Tuple(network[i], network[j], Label(4,4), distribuition[i][j]));
			}
		}
	}
	
	return trace;
}

void print_list(list<Tuple> l) {
	list<Tuple>::iterator tuple;
	int i, x;
	
	for(tuple = l.begin(); tuple != l.end(); ++tuple) {
		cout << "(";
		for(i = 0; i < tuple->label.source_esp; ++i) {
			x = tuple->source.byte[i];
			if(x >= 0) {
				cout << x;
			} else {
				cout << x+256;
			}
			if(i < 3) {
				cout << ".";
			}
		}
		if(tuple->label.source_esp != 4) {
			cout << "*";
		}
		cout << ", ";
		for(i = 0; i < tuple->label.destination_esp; ++i) {
			x = tuple->destination.byte[i];
			if(x >= 0) {
				cout << x;
			} else {
				cout << x+256;
			}
			if(i < 3) {
				cout << ".";
			}
		}
		if(tuple->label.destination_esp != 4) {
			cout << "*";
		}
		cout << ") : " << tuple->count << endl;
	}
}

int main() {
	int network_size, N = 1000;
	float phi;
	char filename[30];
	
	do {
		cout << "Entre com o tamanho da rede: ";
		cin >> network_size;
		if(network_size < 2) {
			cout << "O tamanho da rede deve ser de pelo menos duas maquinas." << endl;
		}
	} while(network_size < 2);
	
	do {
		cout << "Entre com um valor para phi: ";
		cin >> phi;
		if(phi <= 0 || phi > 1) {
			cout << "O valor de phi deve estar entre 0 e 1." << endl;
		}
	} while(phi <= 0 ||phi > 1);
	
	cout << "Entre com nome do arquivo: ";
	cin >> filename;
	
	ofstream file;
	file.open(filename);
	if(file.is_open()) {
		N = 1000 * network_size * network_size;
		int phiN = phi * N;
		
		srand(time(NULL));
		
		ip network[network_size];
		network_generator(network, network_size);	
		list<Tuple> trace = trace_generator(network, network_size, N);
		
		//cout << "- Trace -" << endl;
		//print_list(trace);
		
		list<Tuple>::iterator e, f;
		
		Level level;
		init_level(level);
		list<Label>::iterator label;
		
		// Contagem repassada para as lattices
		list<Tuple> lattices;
		Tuple tuple;
		
		for(int l = 8; l >= 0; --l) {
			for(label = level[l].begin(); label != level[l].end(); ++label) {
				for(e = trace.begin(); e != trace.end(); ++e) {
					tuple = e->generalizeTo(*label);
					tuple.count = e->count;
					lattices.push_back(tuple);
				}
			}
		}
		
		//cout << "- Lattices -" << endl;
		//print_list(lattices);
		
		// Reducao
		list<Tuple> hierarchy;
		
		bool found;
		for(e = lattices.begin(); e != lattices.end(); ++e) {
			found = false;
			for(f = hierarchy.begin(); f != hierarchy.end(); ++f) {
				if(e->equals(*f)) {
					f->count += e->count;
					found = true;
					break;
				}
			}
			if(!found) {
				hierarchy.push_back(*e);
			}
		}
		
		//cout << "- Hierarchy -" << endl;
		//print_list(hierarchy);
		
		// Heavy Hitters
		list<Tuple> heavy_hitters;
		for(e = hierarchy.begin(); e != hierarchy.end(); ++e) {
			if(e->count > phiN) {
				heavy_hitters.push_back(*e);
			}
		}
		
		//cout << "- Heavy Hitters -" << endl;
		//print_list(heavy_hitters);
		
		// Hierarchical Heavy Hitters
		list<Tuple> hierarchical_heavy_hitters;
		
		for(int l = 8; l >= 0; --l) {
			for(e = heavy_hitters.begin(); e != heavy_hitters.end(); ++e) {
				if(e->label.source_esp + e->label.destination_esp == l) {
					for(f = trace.begin(); f != trace.end(); ++f) {
						if(e->iamthekidyouknowwhatimean(*f, &hierarchical_heavy_hitters)) {
							e->count -= f->count;
						}
					}
					if(e->count > phiN) {
						hierarchical_heavy_hitters.push_back(*e);
					}
				}
			}
		}
		
		cout << "- Hierarchical Heavy Hitters -" << endl;
		print_list(hierarchical_heavy_hitters);
		
		
		// Produção do trace
		int volume, x;
		for(e = trace.begin(); e != trace.end(); ++e) {
			do {
				if(e->count >= 1460) {
					volume = 1460;
				} else {
					volume = e->count;
				}
				e->count -= volume;
				file << "IP ";
				for(int i = 0; i < 4; ++i) {
					x = e->source.byte[i];
					if(x >= 0) {
						file << x;
					} else {
						file << x+256;
					}
					file << ".";
				}
				file << "194 > ";
				for(int i = 0; i < 4; ++i) {
					x = e->destination.byte[i];
					if(x >= 0) {
						file << x;
					} else {
						file << x+256;
					}
					file << ".";
				}
				file << "194: tcp " << volume << endl;
			} while (e->count != 0);
		}
	
		cout << "Trace gerado em " << filename << endl;
		file.close();
	
	} else {
		cout << "Unable to open file :C";
	}
	return 0;
}
