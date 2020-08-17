#ifndef RECORDS_H
#define RECORDS_H

#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct Record {
	vector<double> data;
	string label;
	int id; // the corresponding index to the label_map
	vector<double> output;
	Record() {}
	Record(vector<double> data, string label, int id): data(data), label(label), id(id) {}
};

struct LabelMap {
	string label;
	vector<double> output;
	LabelMap() {}
	LabelMap(string label, vector<double> output): label(label), output(output) {}
};

class Records {
public:
	Records() {}
	// col == -1: no label
	Records(const string filename, const string delim, const string label, bool line_trim = true);
	Records(const string filename, const string delim, int col = -1, bool line_trim = true);
	vector<Record>& read_csv(const string filename, const string delim, int col = -1, bool line_trim = true);
	vector<Record>& read_csv(const string filename, const string delim, const string label, bool line_trim = true);
	vector<LabelMap>& read_label(const string filename);
	vector<LabelMap>& setLabel(vector<string> labels);
	vector<LabelMap> getLabelMap() const { return label_map; }
	void setLabelMap(vector<LabelMap>& lm) { label_map = lm; onehot(); }
	void setLabelMap(Records& recs) { label_map = recs.getLabelMap(); onehot(); }
	void normalization(double M, double SD); // normalization
	void onehot(); // one-hot encoding
	void shuffle();
	int size() const { return records.size(); }
	Record operator[](int i) const;
	void push_back(Record rec) { records.push_back(rec); }
	friend ostream& operator<<(ostream& os, const Records& records);
	friend istream& operator>>(istream& is, Records& records);
private:
	vector<Record>& read_csv(const string filename, const string delim, int col, const string label, int header, bool line_trim);
	vector<Record> records;
	vector<LabelMap> label_map;
};

#endif