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
	string name; string course;
	cout << "Zadejte sve jmeno: "; cin >> name;
	cout << "Zadejte premet (MA,ZKR, KOM, PCT, PNA): "; cin >> course;
	int CountOfStudents = 0;

	string path = R"(D:\Skola\Elektronicky_index\Elektronicky_index\data\)" + course + ".csv";

	ifstream file(path);
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
		CountOfStudents++;
	}

	// VYPIS STUDENTU DANEHO KURZU

	cout << students[0][0] << " | " << students[0][1] << " | " << students[0][2] << " | " << students[0][3] << endl;
	cout << "-------------------------------------" << endl;
	for (int i = 1; i < CountOfStudents; i++) {
		cout << students[i][0] << "\t | " << students[i][1] << " | " << students[i][2] << " | " << students[i][3] << endl;
	}

	string student;
	cout << "Zadejte prijmeni studenta, kteremu chcete pridavat body: "; cin >> student;
	int id = -1;
	for (int i = 1; i < CountOfStudents; i++) {
		if (student == students[i][0]) {
			id = i;
		}
	}


	// ZAPIS

	int change = -1; int add = 0;
	cout << "Chcete pridavat body do cviceni nebo zkousky? (1-cviceni, 2-zkouska)"; cin >> change;
	cout << "Zadjte kolik bodu chcete studentovi pridat: "; cin >> add;
	int value = stoi(students[id][change]); // stoi() prevede ze string na int

	students[id][change] = to_string(value + add);
	file.close();

	string tempPath = path + ".tmp"; // vytvoreni docasneho souboru
	ofstream output(path);

	for (const auto& row : students) // postupne prochazime radky ve vektoru students
	{
		for (int i = 0; i < row.size(); i++) // dokud nedojdeme na konec radku
		{
			if (i > 0)
				output << ";"; // vypise se strednik za kazdou dalsi hodnotu

			output << row[i]; // zapis aktualni hodnoty do souboru
		}

		output << "\n"; // dalsi radek csv
	}

}