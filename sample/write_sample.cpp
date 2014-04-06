#include <csv/csv.hpp>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(void) {
  csv _csv = csv();

  // vector<string> to csv-text
  vector<string> row;
  row.push_back(string("a"));
  row.push_back(string("b"));
  row.push_back(string("c"));

  string csv_text;
  _csv.to_csv(row, csv_text);
  cout << csv_text;


  cout << "--------------------------------" << endl;
  csv_text.assign("");

  // any types to csv-text
  _csv.put_field(100);
  _csv.put_field(123.123);
  _csv.put_field("xyz");
  _csv.to_csv(csv_text);
  cout << csv_text;

  return 0;
}
