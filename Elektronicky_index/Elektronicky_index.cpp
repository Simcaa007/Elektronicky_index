#include "Elektronicky_index.h"
#include "CsvReader.h"
#include <stdio.h>
#include <sstream> 
#include <string>
#include <fstream>
using namespace std;
int main()
{
	int kurz;
	printf("Zadejte kurz pro upravu: kom-1, zkr-2:");
	scanf("%d", &kurz);

	char radek[100]; // pole pro ulozeni radku
	FILE* f; // ukazatel na soubor
	switch (kurz) {

	case 1:
		f = fopen("C:\\Users\\Pepa\\Documents\\PC1T\\Elektronicky_index\\Elektronicky_index\\data\\kom.txt", "r");	// otevreni souboru pro cteni
		break;

	case 2:
		f = fopen("C:\\Users\\Pepa\\Documents\\PC1T\\Elektronicky_index\\Elektronicky_index\\data\\zkr.txt", "r");
		break;

	default:
		printf("Neplatny vyber kurzu.\n");
		return 1;
	}

	if (f == NULL) {
		perror("Chyba pri otevirani souboru pro cteni");
		return 1;
	}

	string pole[10];
	int i = 0;

	printf("ID|JMENO|CVICENI|TESTY|ZKOUSKA|ZNAMKA\n");
	while (fgets(radek, sizeof(radek), f) != NULL)	//fgets cte dokud nenarazi na konec radku \n
	{
		printf("%s", radek);
		pole[i] = radek;
		i++;
	}

	fclose(f);

	int studentID, vyber, body;
	printf("Zadejte ID studenta, kteremu chcete pridat body: ");
	scanf("%d", &studentID);
	printf("Zadejte, kam pridat body: cviceni-1, test-2, zkouska-3: ");
	scanf("%d", &vyber);
	printf("A nyni zadejte pocet bodu, kolik chcete pridat: ");
	scanf("%d", &body);

	if (studentID < 0 || studentID >= i) {
		printf("Chyba: Zadane ID studenta neexistuje!\n");
		return 1;
	}

	stringstream ss(pole[studentID]);

	string id, jmeno, cviceniStr, testStr, zkouskaStr;

	getline(ss, id, '|');
	getline(ss, jmeno, '|'); 
	getline(ss, cviceniStr, '|'); 
	getline(ss, testStr, '|');
	getline(ss, zkouskaStr, '|');

	int cviceni = stoi(cviceniStr);
	int test = stoi(testStr);
	int zkouska = stoi(zkouskaStr);

	if (vyber == 1) {
		cviceni += body; // Přičteme body ke cvičení
	}
	else if (vyber == 2) {
		test += body;    // Přičteme body k testu
	}
	else if (vyber == 3) {
		zkouska += body;
	}
	else {
		printf("Neplatny vyber.\n");
		return 1;
	}

	int celkemBodu = cviceni + test + zkouska;
	string znamka;
	if (celkemBodu >= 90) {
		znamka = "A";
	}
	else if (celkemBodu >= 80) {
		znamka = "B";
	}
	else if (celkemBodu >= 70) {
		znamka = "C";
	}
	else if (celkemBodu >= 60) {
		znamka = "D";
	}
	else if (celkemBodu >= 50) {
		znamka = "E";
	}
	else {
		znamka = "F";
	}

	pole[studentID] = id + "|" + jmeno + "|" + to_string(cviceni) + "|" + to_string(test) + "|" + to_string(zkouska) + "|" + znamka + "\n";


	switch (kurz) {

	case 1:
		f = fopen("C:\\Users\\Pepa\\Documents\\PC1T\\Elektronicky_index\\Elektronicky_index\\data\\kom.txt", "w");
		if (f == NULL) {
			perror("Chyba pri otevirani souboru pro zapis");
			return 1;
		}
		break;

	case 2:
		f = fopen("C:\\Users\\Pepa\\Documents\\PC1T\\Elektronicky_index\\Elektronicky_index\\data\\zkr.txt", "w");
		if (f == NULL) {
			perror("Chyba pri otevirani souboru pro zapis");
			return 1;
		}
		break;
	}

	for (int j = 0; j < i; j++) {
		fprintf(f, "%s", pole[j].c_str());
	}

	fclose(f); // uvolni systemove prostredky a zajisti, ze se data v poradku uvolni z pameti
	printf("Soubor byl uspesne aktualizovan!\n");

	return 0;
}