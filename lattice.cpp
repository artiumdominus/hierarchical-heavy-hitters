#include <iostream>

using namespace std;

union ip {
	short full;
	char byte[4];
};

// class ip {
	// public:
	// char byte[4];

	// ip(){}

	// // ip(int byte0, int byte1, int byte2, int byte3) {
		// // byte[0] = byte0;
		// // byte[1] = byte1;
		// // byte[2] = byte2;
		// // byte[3] = byte3;
	// // }
	
	// bool equals(ip Ip) {
		// for(int i = 0; i < 4; ++i) {
			// if(this->byte[i] != Ip.byte[i]) {
				// return false;
			// }
		// }
		// return true;
	// }
// };

class Label {
	public:
		unsigned short source_esp : 3;
		unsigned short destination_esp : 3;
		// int source_esp;
		// int destination_esp;
		
		Label(int source, int destination) {
			this->source_esp = source;
			this->destination_esp = destination;
		}
		
		Label(){}
		
		bool equals(Label label){
			return this->source_esp == label.source_esp && this->destination_esp == label.destination_esp;
		}
};

class Tuple {
	public:
		Label label;
		ip source;
		ip destination;
		long count;
		
		Tuple(ip source, ip destination, Label label, int count = 0) {
			this->source = source;
			this->destination = destination;
			this->label = label;
			this->count = count;
		}
		
		Tuple(){}
		
		Tuple generalizeTo(Label label) {
			ip general_source, general_destination;
			int i;
			general_source = this->source;
			general_destination = this->destination;
			for(i = label.source_esp; i < 4; ++i){
				general_source.byte[i] = 0;
			}
			for(i = label.destination_esp; i < 4; ++i){
				general_destination.byte[i] = 0;
			}
			return Tuple(general_source, general_destination, label);
		}
		
		bool isAGeneralizationOf(Tuple x) {
			if(x.label.source_esp < this->label.source_esp || x.label.destination_esp < this->label.destination_esp) {
				return false;
			}
			Tuple y = x.generalizeTo(this->label);
			return this->source.full == y.source.full && this->destination.full == y.destination.full;
			// return this->equals(y);
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
			return this->label.equals(tuple.label) && this->source.full == tuple.source.full && this->destination.full == tuple.destination.full;
			// return this->label.equals(tuple.label) && this->source.equals(tuple.source) && this->destination.equals(tuple.destination);
		}
		
		void print(){
			int i, x;
			cout << "(";
			for(i = 0; i < label.source_esp; ++i) {
				x = source.byte[i];
				if(x >= 0) {
					cout << x;
				} else {
					cout << x+256;
				}
				if(i < 3) {
					cout << ".";
				}
			}
			if(label.source_esp != 4) {
				cout << "*";
			}
			cout << ", ";
			for(i = 0; i < label.destination_esp; ++i) {
				x = destination.byte[i];
				if(x >= 0) {
					cout << x;
				} else {
					cout << x+256;
				}
				if(i < 3) {
					cout << ".";
				}
			}
			if(label.destination_esp != 4) {
				cout << "*";
			}
			cout << ")" << endl;
		}
};

typedef list<Label> Level[9];

void init_level(Level level) {
	level[0].push_back(Label(0,0));
	level[1].push_back(Label(0,1));
	level[1].push_back(Label(1,0));
	level[2].push_back(Label(0,2));
	level[2].push_back(Label(1,1));
	level[2].push_back(Label(2,0));
	level[3].push_back(Label(0,3));
	level[3].push_back(Label(1,2));
	level[3].push_back(Label(2,1));
	level[3].push_back(Label(3,0));
	level[4].push_back(Label(0,4));
	level[4].push_back(Label(1,3));
	level[4].push_back(Label(2,2));
	level[4].push_back(Label(3,1));
	level[4].push_back(Label(4,0));
	level[5].push_back(Label(1,4));
	level[5].push_back(Label(2,3));
	level[5].push_back(Label(3,2));
	level[5].push_back(Label(4,1));
	level[6].push_back(Label(2,4));
	level[6].push_back(Label(3,3));
	level[6].push_back(Label(4,2));
	level[7].push_back(Label(3,4));
	level[7].push_back(Label(4,3));
	level[8].push_back(Label(4,4));
}