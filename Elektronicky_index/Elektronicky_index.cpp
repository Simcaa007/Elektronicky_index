#include "Elektronicky_index.h"
#include "CsvReader.h"
#include <stdio.h>
#include <sstream> 
#include <string>

FILE* VyberKurzu(int kurz);

int PridatStudenta(int kurz);

void SmazatStudenta(int kurz);

void PridaniBodu(int kurz);

static void SpoctiHashHesla(const char* heslo, char* vystupniHash, size_t velikostBufferu);

bool Prihlaseni(const char* cestaKUzivatelum);

using namespace std;
int main()
{
	int kurz;
	printf("Zadejte kurz pro upravu: kom-1, zkr-2:");
	scanf("%d", &kurz);

	FILE* f = VyberKurzu(1);
	if (f == NULL) {
		return 1;
	}

	printf("\nID|JMENO|CVICENI|TESTY|ZKOUSKA|ZNAMKA\n");

	char radek[100];
	while (fgets(radek, sizeof(radek), f) != NULL) {
		printf("%s", radek);
	}


	int akce;

	do {
		printf("\nVyberte akci:\n");
		printf("1 - Pridat studenta\n");
		printf("2 - Smazat studenta\n");
		printf("3 - Upravit body studenta\n");
		printf("4 - Konec programu\n");
		printf("Vase volba: ");
		if (scanf("%d", &akce) != 1) {
			printf("Neplatna volba.\n");
			fclose(f);
			return 1;
		}

		switch (akce) {
		case 1: {
			PridejStudenta(kurz);
		}break;

		case 2: {
			SmazatStudenta(kurz);
		}break;

		case 3: {
			PridaniBodu(kurz);
		}
		}
	} while (akce != 4);
	return 0;
}

// VYBER KURZU

FILE* VyberKurzu(int kurz)
{
	const char* cesta = NULL; 

	switch (kurz) {

	case 1:
		cesta = "D:\\Skola\\jedeme\\Elektronicky_index\\data\\kom.txt";	
		break;

	case 2:
		cesta = "D:\\Skola\\jedeme\\Elektronicky_index\\data\\zkr.txt";
		break;

	default:
		printf("Neplatny vyber kurzu.\n");
		return NULL;
	}

	FILE* f = fopen(cesta, "r");

	if (f == NULL) {
		perror("Chyba pri otevirani souboru pro cteni"); // Overini, zda se soubor podarilo otevrit
		return NULL;
	}

	return f;
}

// PRIDANI STUDENTA

int PridejStudenta(int kurz)
{
	FILE* f = VyberKurzu(kurz);
	char radek[100];
	int nejvyssiID = -1;
	while (fgets(radek, sizeof(radek), f) != NULL) {
		stringstream ss(radek);
		string id;
		getline(ss, id, '|');
		int aktualniID = stoi(id);
		if (aktualniID > nejvyssiID) {
			nejvyssiID = aktualniID;
		}
	}
	if (ferror(f)) {
		perror("Chyba pri cteni souboru");
		fclose(f);
		return 1;
	}
	fclose(f);

	char jmeno[100];
	printf("Zadejte jmeno studenta: ");
	if (scanf(" %99[^|\n]", jmeno) != 1) {
		printf("Neplatne jmeno studenta.\n");
		return 1;
	}

	FILE* zapis;
	switch (kurz) {
	case 1:
		zapis = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\kom.txt", "a");
		break;
	case 2:
		zapis = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\zkr.txt", "a");
		break;
	default:
		printf("Neplatny vyber kurzu.\n");
		return 1;
	}
	if (zapis == NULL) {
		perror("Chyba pri otevirani souboru pro zapis");
		return 1;
	}

	if (fprintf(zapis, "%d|%s|0|0|0|F\n", nejvyssiID + 1, jmeno) < 0) {
		perror("Chyba pri zapisu studenta");
		fclose(zapis);
		return 1;
	}
	if (fclose(zapis) != 0) {
		perror("Chyba pri ukladani souboru");
		return 1;
	}

	printf("Student byl uspesne pridan s ID %d.\n", nejvyssiID + 1);
}

// SMAZANI STUDENTA

void SmazatStudenta(int kurz)
{
	FILE* f = VyberKurzu(kurz);
	char radek[100];
	string pole[10];
	int i = 0;

	printf("ID|JMENO|CVICENI|TESTY|ZKOUSKA|ZNAMKA\n");
	while (fgets(radek, sizeof(radek), f) != NULL) {
		if (i >= 10) {
			printf("Chyba: Soubor obsahuje vice nez 10 studentu.\n");
			fclose(f);
			return;
		}
		pole[i] = radek;
		printf("%s", radek);
		i++;
	}
	fclose(f);

	int studentID;
	printf("Zadejte ID studenta ke smazani: ");
	if (scanf("%d", &studentID) != 1) {
		printf("Neplatne ID.\n");
		return;
	}

	int pozice = -1;
	for (int j = 0; j < i; j++) {
		stringstream ss(pole[j]);
		string id;
		getline(ss, id, '|');

		if (id == to_string(studentID)) {
			pozice = j;
			break;
		}
	}

	if (pozice == -1) {
		printf("Student s timto ID nebyl nalezen.\n");
		return;
	}

	switch (kurz) {
	case 1:
		f = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\kom.txt", "w");
		break;
	case 2:
		f = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\zkr.txt", "w");
		break;
	}

	if (f == NULL) {
		perror("Chyba pri otevirani souboru pro zapis");
		return;
	}

	for (int j = 0; j < i; j++) {
		if (j != pozice) {
			fprintf(f, "%s", pole[j].c_str());
		}
	}

	fclose(f);
	printf("Student byl smazan.\n");
}

// PRIDANI BODU

void PridaniBodu(int kurz)
{
	FILE* f = VyberKurzu(kurz);
	char radek[100];
	string pole[10];
	int i = 0;

	printf("ID|JMENO|CVICENI|TESTY|ZKOUSKA|ZNAMKA\n");
	while (fgets(radek, sizeof(radek), f) != NULL)	//fgets cte dokud nenarazi na konec radku \n
	{
		if (i >= 10) {
			printf("Chyba: Soubor obsahuje vice nez 10 studentu.\n");
			fclose(f);
			return;
		}
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

	int pozice = -1;
	for (int j = 0; j < i; j++) {
		stringstream hledani(pole[j]);
		string idRadku;
		getline(hledani, idRadku, '|');

		if (idRadku == to_string(studentID)) {
			pozice = j;
			break;
		}
	}

	if (pozice == -1) {
		printf("Chyba: Zadane ID studenta neexistuje!\n");
		return;
	}

	stringstream ss(pole[pozice]);

	string id, jmeno, cviceniStr, testStr, zkouskaStr, znamkaStr;

	getline(ss, id, '|');
	getline(ss, jmeno, '|');
	getline(ss, cviceniStr, '|');
	getline(ss, testStr, '|');
	getline(ss, zkouskaStr, '|');
	getline(ss, znamkaStr, '|');

	int cviceni = stoi(cviceniStr);
	int test = stoi(testStr);
	int zkouska = stoi(zkouskaStr);

	switch (vyber) {
	case 1:
		cviceni += body; 
		break;

	case 2:
		test += body;   
		break;

	case 3:
		zkouska += body;
		break;

	default:
		printf("Neplatny vyber.\n");
		return;
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

	pole[pozice] = id + "|" + jmeno + "|" + to_string(cviceni) + "|" + to_string(test) + "|" + to_string(zkouska) + "|" + znamka + "\n";


	switch (kurz) {

	case 1:
		f = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\kom.txt", "w");
		if (f == NULL) {
			perror("Chyba pri otevirani souboru pro zapis");
			return;
		}
		break;

	case 2:
		f = fopen("D:\\Skola\\jedeme\\Elektronicky_index\\data\\zkr.txt", "w");
		if (f == NULL) {
			perror("Chyba pri otevirani souboru pro zapis");
			return;
		}
		break;
	}

	for (int j = 0; j < i; j++) {
		fprintf(f, "%s", pole[j].c_str());
	}

	fclose(f); // uvolni systemove prostredky a zajisti, ze se data v poradku uvolni z pameti
	printf("Soubor byl uspesne aktualizovan!\n");

}