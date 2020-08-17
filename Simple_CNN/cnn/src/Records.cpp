#include "Records.hpp"
#include "BinaryStream.hpp"
#include "util.hpp"
#include <algorithm> // std::random_shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
using namespace std;

Records::Records(const string filename, const string delim, int col, bool line_trim) {
	read_csv(filename, delim, col, line_trim);
}

Records::Records(const string filename, const string delim, const string label, bool line_trim) {
	read_csv(filename, delim, label, line_trim);
}

vector<Record>& Records::read_csv(const string filename, const string delim, int col, const string label, int header, bool line_trim) {
	fstream fin;
	string line;
	fin.open(filename.c_str(), ios::in);
	if (header) {
		getline(fin, line);
		if (line_trim) trim(line);
		vector<string> row = split(line, delim);
		if (label != "") for (int i = 0; i < row.size(); i++) if (row[i] == label) { col = i; break; }
	}
	while (getline(fin, line)) {
		Record rec;
		if (line_trim) trim(line);
		vector<string> row = split(line, delim);
		for (int i = 0; i < row.size(); i++) {
			if (i == col) rec.label = row[i];
			else rec.data.push_back(stod(row[i]));
		}
		records.push_back(rec);
	}
	fin.close();

	return records;
}

vector<Record>& Records::read_csv(const string filename, const string delim, int col, bool line_trim) {
	return read_csv(filename, delim, col, "", false, line_trim);
}

vector<Record>& Records::read_csv(const string filename, const string delim, const string label, bool line_trim) {
	return read_csv(filename, delim, 0, label, true, line_trim);
}

vector<LabelMap>& Records::read_label(const string filename) {
	vector<string> labels;
	fstream fin(filename.c_str(), ios::in);
	string s;
	while (getline(fin, s)) {
		trim(s);
		bool is_found = false;
		for (int i = 0; i < labels.size(); i++) {
			if (s == labels[i]) {
				is_found = true;
				break;
			}
		}
		if (!is_found) labels.push_back(s);
	}
	fin.close();

	label_map.clear();

	for (int i = 0; i < labels.size(); i++) {
		LabelMap lm;
		lm.label = labels[i];
		for (int j = 0; j < labels.size(); j++) {
			if (i == j) lm.output.push_back(1);
			else lm.output.push_back(0);
		}
		label_map.push_back(lm);
	}

	onehot();
	return label_map;
}

vector<LabelMap>& Records::setLabel(vector<string> labels) {
	vector<string> label_set;
	
	for (int i = 0; i < labels.size(); i++) {
		bool is_found = false;
		for (int j = 0; j < label_set.size(); j++) {
			if (labels[i] == label_set[j]) {
				is_found = true;
				break;
			}
		}
		if (!is_found) label_set.push_back(labels[i]);
	}

	label_map.clear();

	for (int i = 0; i < label_set.size(); i++) {
		LabelMap lm;
		lm.label = label_set[i];
		for (int j = 0; j < label_set.size(); j++) {
			if (i == j) lm.output.push_back(1);
			else lm.output.push_back(0);
		}
		label_map.push_back(lm);
	}

	onehot();
	return label_map;
}

void Records::normalization(double M, double SD) {
	for (int i = 0; i < records.size(); i++) {
		for (int j = 0; j < records[i].data.size(); j++) {
			records[i].data[j] = (records[i].data[j] - M) / SD;
		}
	}
}

void Records::onehot() { // one-hot encoding
	for (int i = 0; i < records.size(); i++) {
		for (int j = 0; j < label_map.size(); j++) {
			if (records[i].label == label_map[j].label) {
				records[i].id = j;
				break;
			}
		}
	}
}

void Records::shuffle() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(records.begin(), records.end(), std::default_random_engine(seed));
}

Record Records::operator[](int i) const {
	if (i >= records.size()) throw "Records index out of range";
	Record rec = records[i];
	if (label_map.size() > 0 && rec.id < label_map.size()) rec.output = label_map[rec.id].output;
	return rec;
}

ostream& operator<<(ostream& os, const Records& records) {
	// { int size, int data_size, { double data[data_size], int id }[size], label_size, string labels[label_size] }
	BinaryStream bs;
	// write obj to stream
	int size = records.size();
	bs.writeInt(os, size);
	int data_size = (size > 0)? records[0].data.size(): 0;
	bs.writeInt(os, data_size);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < data_size; j++) bs.writeDouble(os, records[i].data[j]);
		bs.writeInt(os, records[i].id);
	}
	bs.writeInt(os, records.label_map.size());
	for (int i = 0; i < records.label_map.size(); i++) {
		os << records.label_map[i].label << endl;
	}
	return os;
}

istream& operator>>(istream& is, Records& records) {
	// { int size, int data_size, { double data[data_size], int id }[size], label_size, string labels[label_size] }
	BinaryStream bs;
	// read obj from stream
	int size = bs.readInt(is);
	int data_size = bs.readInt(is);
	records.records.clear();
	for (int i = 0; i < size; i++) {
		Record rec;
		for (int j = 0; j < data_size; j++) rec.data.push_back(bs.readDouble(is));
		rec.id = bs.readInt(is);
		records.push_back(rec);
	}
	int label_size = bs.readInt(is);
	vector<string> labels;
	for (int i = 0; i < label_size; i++) {
		string s;
		is >> s;
		labels.push_back(s);
	}
	
	for (int i = 0; i < records.size(); i++) {
		records[i].label = labels[records[i].id];
	}
	records.setLabel(labels);
	return is;
}