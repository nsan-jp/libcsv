#include <csv/csv.hpp>
#include <iostream>

using namespace std;

struct row_proc {
  void operator()(int lines, vector<string> *row) {
    cout << "  line " << lines + 1 << endl;
    unsigned i;
    for (i = 0; i < row->size(); i++) {
      cout << "    [" << i << "] " << (*row)[i] << endl;
    }
  }
};

int main(void) {
  // read from char* csv-data
  csv csv_text = csv(new csv_memory_datasource("a,b,c\r\nx,y,z"));
  if (!csv_text.read_all(row_proc())) {
    cout << "io error.\n";
    return -1;
  } 

  cout << "-------------------------------------------------------\n";

  // read from file csv-data
  csv csv_file = csv(new csv_file_datasource("example.csv"));
  if (!csv_file.read_all(row_proc())) {
    cout << "io error.\n";
    return -1;
  } 
  return 0;
}
