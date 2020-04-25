#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK 10 
#define MAX_CHAR 256
//priprema terena za dinamičku alokaciju 

//funkcije za proveru validnosti alokacije 
void allocationValidityCheckLine(char* line) {
	if (!line) {
		printf("MEM_GRESKA");
		exit(0);
	}
}

void allocationValidityCheckInt(int* n) {
	if (!n) {
		printf("MEM_GRESKA");
		exit(0);
	}
}

//funkcija koja čita liniju teksta (do \n) nepoznate dužine u niz char-ova (string) 
char* readLine() {
	char* line = NULL, c; //pokazivač na niz karaktera u čije elemente smeštamo karaktere 
	int cnt = 0; //brojač pozicije u nizu 

	while ((c = getchar()) != '\n') { //sve dok nismo stigli do kraja reda
		if (cnt % CHUNK == 0) { //ako smo ispunili kvotu slobodnog rezervirsanog prostora (0,10,20, ..., 100, ...)
			line = realloc(line, sizeof(*line) * (cnt + CHUNK)); 
			//proširi kvotu slobodnog prostora na koji ukazuje pokazivač line za vrednost countera (npr. 0+10 u prvom prolazu)
			//tj. dodaj novi chunk prostora i počni da ga trošiš

			//provera ispravnosti dodele memorije (not null== true => is null, alokacija nije uspela)
			allocationValidityCheckLine(line);
		}
		//na tekuću poziciju niza na koji ukazuje line upisuje pripadajući karakter koji je pročitao
		line[cnt++] = c;
	}
	
	//detekcija praznog reda (!0 => 1; ako se vrednost countera nije promenila, nismo upisali ništa)
	if (!cnt) return NULL;

	//rešavanje problema eventualnog viška alocirane memorije u CHUNK-u 
	line = realloc(line, sizeof(*line) * (cnt + 1)); //realokacija za tačan broj char-ova (*line - na lokaciji na koju pokazuje line nalazi se char)
													//pohranjen u counteru, i još jedan, za kraj stringa
	
	//provera ispravnosti dodele memorije
	allocationValidityCheckLine(line);
	line[cnt] = '\0';

	//povratak tačno sačinjene linije posle svih navedenih operacija
	return line;
}

char** readLines(int* n) {
	char** lines = NULL, * line;
	int cnt = 0;


	while (1) {
		line = readLine();
		if (!line)
			break;
		if (cnt % CHUNK == 0) {
			lines = realloc(lines, sizeof(*lines) * (cnt + CHUNK));
			allocationValidityCheckLine(lines);
		}
		lines[cnt++] = line;
	}

	*n = cnt;

	return lines;
}

//int areAnagrams(char* string0, char* string1) {
//	int flag=0;
//
//
//	//ako nisu jednaki, ne mogu biti ni anagrami (potreban uslov)
//	if (strlen(string0) != strlen(string1)) return flag; 
//
//	//ako su jednaki, dužine su im jednake
//	int length = strlen(string0);
//
//	//pomoćna promenljiva za sortiranje
//	char t;
//	
//	//alociramo memoriju za 2 nova niza, da se sortiranje ne bi odrazilo na originale, radi ispisa
//	char* str0=NULL, * str1=NULL;
//	str0 = (char*)malloc(sizeof(char) * (strlen(string0)+1));
//	allocationValidityCheckLine(str0);
//	strcpy(str0, string0);
//	
//	str1 = (char*)malloc(sizeof(char) * (strlen(string1)+1));
//	allocationValidityCheckLine(str1);
//	strcpy(str1, string1);
//
//	//sortiramo oba stringa
//	for (int i = 0; i < length - 1; i++) {
//		for (int j = i + 1; j < length; j++) {
//			if (str0[i] > str0[j]) {
//				t = str0[i];
//				str0[i] = str0[j];
//				str0[j] = t;
//			}
//			if (str1[i] > str1[j]) {
//				t = str1[i];
//				str1[i] = str1[j];
//				str1[j] = t;
//			}
//		}
//	}
//
//	
//	//poredimo sortirane stringove na jednakost, tj tražimo ima li pozicije na kojoj nisu jednaki
//	//logika kontrapozicije
//	for (int i = 0; i < length; i++) {
//		if (str0[i] != str1[i]) {
//			free(str0);
//			free(str1);
//			return flag; //ako postoji bar jedna pozicija na kojoj u sortiranim stringovima nisu isti 
//						//karakteri, onda to nisu anagrami
//		}
//	}
//
//	//ako nisu ispali iz provere, anagrami su
//	flag = 1;
//	free(str0);
//	free(str1);
//	return flag;
//}

//bolje rešenje
int areAnagrams(char* string0, char* string1)
{
	int flag = 0;
	//ako nisu jednaki, nisu ni anagrami (potreban uslov)
	if (strlen(string0) != strlen(string1)) return flag;
	int cnt[MAX_CHAR] = { 0 };

	for (int i = 0; string0[i] && string1[i]; i++) {
		cnt[string0[i]]++;
		cnt[string1[i]]--;
	}

	//ako se ++ i -- nisu poništili do nule, onda nisu anagrami;
	for (int i = 0; i < MAX_CHAR; i++)
		if (cnt[i])
			return flag;

	//u suprotnom jesu anagrami
	flag = 1;
	return flag;
}

int main() {
	int* n = (int*)malloc(sizeof(int));
	allocationValidityCheckInt(n);

	char** lines = readLines(n);

	//nema unosa - greška
	if (*n == 0) {
		printf("GRESKA");
		exit(0);
	}

	//proveri za svaki u nizu da li u ostatku niza ima anagrame
	//tako automatski eliminišemo ponavljanja ispisa istih parova anagrama
	int flag = 0;
	for (int i = 0; i < ((*n) - 1); i++) {
		for (int j = i + 1; j < *n; j++) {
			flag = areAnagrams(lines[i], lines[j]);
			if (flag) printf("%s|%s\n", lines[i], lines[j]);
		}

	}

	//dealokacija
	for (int i = 0; i < *n; i++) {
		free(lines[i]);
	}
	free(lines);
	free(n);
	return 0;
}

