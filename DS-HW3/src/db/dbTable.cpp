/****************************************************************************
  FileName     [ dbTable.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Table member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <string>
#include <cctype>
#include <cassert>
#include <set>
#include <algorithm>
#include "dbTable.h"
#include "util.h"

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
ostream& operator << (ostream& os, const DBRow& r)
{
   // TODO: to print out a row.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
  
  for(size_t i=0;i<r.size();++i){
    if(r[i]==INT_MAX)
      os<<'.';
    else
      os<<r[i];
    if(i!=r.size()-1)
      os<<' ';
  }
   return os;
}

ostream& operator << (ostream& os, const DBTable& t)
{
   // TODO: to print out a table
   // - Data are seperated by setw(6) and aligned right.
   // - Null cells are printed as '.'
   for(size_t i = 0,n = t.nRows();i < n;++i){
    for(size_t j = 0,m = t.nCols();j < m;++j){
      // cout<<i<<" "<<j<<endl;
      // cout<<&(t._table[i][j]);
      // int d;
      // cin>>d;
      if(t._table[i][j] == INT_MAX)
        os<<setw(6)<<right<<'.';
      else
        os<<setw(6)<<right<<t._table[i][j];
    }
    if(i != n-1)
      os<<endl;
   }
   return os;
}

ifstream& operator >> (ifstream& ifs, DBTable& t)
{
   // TODO: to read in data from csv file and store them in a table
   // - You can assume the input file is with correct csv file format
   // - NO NEED to handle error file format
   
   while(!ifs.eof()){
    DBRow temp ;
    size_t i=0;
    int j;
    // bool finish = false;
    string k,s ;
    getline(ifs,k);
    string buf;
    // if(s[4]=='\r')cout<<"YES"<<endl;
    if(k[k.size()-1]=='\r')
      s = k.substr(0,k.size()-1);
    else
      s = k;
    // cout<<s[3]<<endl;
    // cout<<s.size()<<endl;
    // cout<<"A";
    if(s[0]==','){
      temp.addData(INT_MAX);
    }
    while(i != string::npos){

      // i=s.find(',');
      i = myStrGetTok(s,buf,i,',');
      // cout<<i<<'X'<<endl;
      // cout<<buf<<"A"<<i<<endl;  ///
      // if(i == string::npos)
      //   finish = true;
      //   break;
      // buf = s.substr(0,i);
      // cout<<"A"<<buf.size()<<endl;
      if(buf == ""){
        temp.addData(INT_MAX);
        // cout<<"D"<<endl;
      }
      else{
        if(myStr2Int(buf,j)){
          // cout<<"S";
          temp.addData(j);
        }
        // cout<<"C"<<endl;
      }
      // s.erase(0,i+1);
      // buf = buf+"X";
      // cout<<buf<<endl;  //
      if(s[i+1]==','){
        if(i != string::npos)
          temp.addData(INT_MAX);
      }
    }
    t.addRow(temp);
    // cout<<"R"<<t.nCols()<<endl;
    // cout<<"A"<<endl;
    // cout<<temp.size();
    // cout<<t._table[0][0]<<endl;

   }

   return ifs;
}

/*****************************************/
/*   Member Functions for class DBRow    */
/*****************************************/
void
DBRow::removeCell(size_t c)
{
   // TODO
  _data.erase(_data.begin()+c);
  //erase the (c+1)th element c=0 is the first one
}

/*****************************************/
/*   Member Functions for struct DBSort  */
/*****************************************/
bool
DBSort::operator() (const DBRow& r1, const DBRow& r2) const
{
   // TODO: called as a functional object that compares the data in r1 and r2
   //       based on the order defined in _sortOrder
   for(size_t i = 0 , n = _sortOrder.size();i < n;++i){

   if(r1[_sortOrder[i]] > r2[_sortOrder[i]])
    return false ;
   else if(r1[_sortOrder[i]] < r2[_sortOrder[i]])
    return true;
  }
  return false;
}

/*****************************************/
/*   Member Functions for class DBTable  */
/*****************************************/
void
DBTable::reset()
{
  vector<DBRow> temp;
  temp.swap(_table);
   // TODO
}

void
DBTable::addCol(const vector<int>& d)
{
   // TODO: add a column to the right of the table. Data are in 'd'.
   for(size_t i = 0,n = _table.size();i < n;++i)
    _table[i].addData(d[i]);
}

void
DBTable::delRow(int c)
{
   // TODO: delete row #c. Note #0 is the first row.
   _table.erase(_table.begin()+c);
}

void
DBTable::delCol(int c)
{
   // delete col #c. Note #0 is the first row.
   for (size_t i = 0, n = _table.size(); i < n; ++i)
      _table[i].removeCell(c);
}

// For the following getXXX() functions...  (except for getCount())
// - Ignore null cells
// - If all the cells in column #c are null, return NAN
// - Return "float" because NAN is a float.
float
DBTable::getMax(size_t c) const
{
   // TODO: get the max data in column #c
  size_t j=0;
  while(_table[j][c]==INT_MAX && j< nRows())
   j++;
  if(j == nRows())
    return NAN;
  int max = _table[j][c];
  for (j=j+1;j< nRows();++j){
    if(_table[j][c] != INT_MAX)
      if(max < _table[j][c])
        max = _table[j][c];
  }
   return (float)max;    //0.0;
}

float
DBTable::getMin(size_t c) const
{
   // TODO: get the min data in column #c
  size_t j=0;
  while(_table[j][c]==INT_MAX && j< nRows())
   j++;
  if(j ==  nRows())
    return NAN;
  int min = _table[j][c];
  for (j=j+1;j< nRows();++j)
    if(_table[j][c] != INT_MAX)
      if(min > _table[j][c])
        min = _table[j][c];
   return (float)min;
   // return 0.0;
}

float 
DBTable::getSum(size_t c) const
{
   // TODO: compute the sum of data in column #c
  size_t j=0;
  while(_table[j][c]==INT_MAX && j< nRows())
   j++;
  if(j ==  nRows())
    return NAN;
  int sum = _table[j][c];
  for (j=j+1;j< nRows();++j)
    if(_table[j][c] != INT_MAX)
        sum += _table[j][c];
   return (float)sum;
   // return 0.0;
}

int
DBTable::getCount(size_t c) const
{
   // TODO: compute the number of distinct data in column #c
   // - Ignore null cells
  size_t j=0;
  set <int> count;
  while(_table[j][c]==INT_MAX && j< nRows())
   j++;
  if(j ==  nRows())
    return NAN;
  for (;j< nRows();++j)
    if(_table[j][c] != INT_MAX)
        count.insert(_table[j][c]);
   return (float)count.size();
   // return 0;
}

float
DBTable::getAve(size_t c) const
{
   // TODO: compute the average of data in column #c
   size_t j=0;
   float count =0;
  while(_table[j][c]==INT_MAX && j< nRows())
   ++j;
  if(j ==  nRows())
    return NAN;
  int sum = _table[j][c];++count;
  for (j=j+1;j< nRows();++j)
    if(_table[j][c] != INT_MAX){
        sum += _table[j][c];
        ++count;
      }
   return sum/count; //cout<<fixed<<setprecision(2)
   // return 0.0;
}

void
DBTable::sort(const struct DBSort& s)
{
   // TODO: sort the data according to the order of columns in 's'
  
  std::sort(_table.begin(),_table.end(),s);
}
// for(size_t i = 0, n = nRows();i < n-1;++i){
  //   for (size_t j = i+1; j < n;++j){
  //     if(s(_table[i],_table[j])){
  //       vector <int> temp;
  //       size_t m = nCols();
  //       for(size_t k=0;k<m;++k)
  //         temp.push_back(_table[i][k]);
  //       _table[i].reset();
  //       for(size_t k=0;k<m;++k)
  //         _table[i].addData(_table[j][k]);
  //       _table[j].reset();
  //       for(size_t k=0;k<m;++k)
  //         _table[j].addData(temp[k]);
  //     }
  //   }
    
  // }
void
DBTable::printCol(size_t c) const
{
   // TODO: to print out a column.
   // - Data are seperated by a space. No trailing space at the end.
   // - Null cells are printed as '.'
   for(size_t i = 0,n =  nRows();i < n;++i){
    if(_table[i][c] == INT_MAX)
      cout<<".";
    else
      cout<<_table[i][c];
    if(i != n-1)
      cout<<' ';
   }
}

void
DBTable::printSummary() const
{
   size_t nr = nRows(), nc = nCols(), nv = 0;
   for (size_t i = 0; i < nr; ++i)
      for (size_t j = 0; j < nc; ++j)
         if (_table[i][j] != INT_MAX) ++nv;
   cout << "(#rows, #cols, #data) = (" << nr << ", " << nc << ", "
        << nv << ")" << endl;
}

