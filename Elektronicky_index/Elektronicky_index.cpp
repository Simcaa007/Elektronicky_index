#include "Elektronicky_index.h"
#include "CsvReader.h"
#include <stdio.h>
#include <sstream> 
#include <string>

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
		fopen_s(&f, "D:\\Skola\\p\\Elektronicky_index\\data\\kom.txt", "r");	// otevreni souboru pro cteni
		break;

	case 2:
		fopen_s(&f, "D:\\Skola\\p\\Elektronicky_index\\data\\zkr.txt", "r");
		break;
	}

	
	string pole[10];
	int i = 0;

	printf("ID|JMENO|CVICENI|TESTY|ZNAMKA\n");
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
	printf("Nyni zadejte zda chcete pridat body za cviceni-1 nebo za test-2: ");
	scanf("%d", &vyber);
	printf("A nyni zadejte pocet bodu, kolik chcete pridat: ");
	scanf("%d", &body);

	if (studentID < 0 || studentID >= i) {
		printf("Chyba: Zadane ID studenta neexistuje!\n");
		return 1;
	}

	stringstream ss(pole[studentID]);

	string id, jmeno, cviceniStr, testStr, znamkaStr;

	getline(ss, id, '|');
	getline(ss, jmeno, '|'); 
	getline(ss, cviceniStr, '|'); 
	getline(ss, testStr, '|');
	getline(ss, znamkaStr, '|');

	int cviceni = stoi(cviceniStr);
	int test = stoi(testStr);
	int znamka = stoi(znamkaStr);

	if (vyber == 1) {
		cviceni += body; // Přičteme body ke cvičení
	}
	else if (vyber == 2) {
		test += body;    // Přičteme body k testu
	}

	pole[studentID] = id + "|" +jmeno + "|" + to_string(cviceni) + "|" + to_string(test) + "|" + to_string(znamka) + "\n";


	switch (kurz) {

	case 1:
		if (fopen_s(&f, "D:\\Skola\\p\\Elektronicky_index\\data\\kom.txt", "w") != 0 || f == NULL) {
			printf("Chyba pri otevirani souboru pro zapis.\n");
			return 1;
		}
		break;

	case 2:
		if (fopen_s(&f, "D:\\Skola\\p\\Elektronicky_index\\data\\zkr.txt", "w") != 0 || f == NULL) {
			printf("Chyba pri otevirani souboru pro zapis.\n");
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