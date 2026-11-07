#include "Elektronicky_index.h"
#include "CsvReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
int main()
{
	ifstream file(
		R"(D:\Skola\Elektronicky_index\Elektronicky_index\data\studenti.csv)"
	);
	if (!file.is_open()) {
		printf("file can't be opened");
		return 1;
	}

	vector<vector<string>> students;

	string line;

	while (getline(file, line)) // nacte radek a ulozi do promenne line
	{
		stringstream ss(line); 
		string value;

		vector<string> row;	// da nam radek do pole

		while (getline(ss, value, ';'))	// ss - odkud cteme text, value - kam ulozime nactenou hodnotu, ; - znak, u ktereho se cteni zastavi
		{
			row.push_back(value); // prida hodnotu na konec vektoru
		}

		students.push_back(row); // cely vektor row se ulozi do students
	}


}
