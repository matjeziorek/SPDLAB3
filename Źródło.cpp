#include <vector>      
#include <fstream>      
#include <iostream>
#include <queue>       
#include <string>


using namespace std;

class Zadanie {
public:
	int terminDostepnosci;      //r
	int czasWykonania;		    //p
	int czasDostarczenia;       //q
};

struct PorownajCzasDostarczenia {
	bool operator ()(const Zadanie & zadanie1, const Zadanie & zadanie2) {
		return zadanie1.czasDostarczenia < zadanie2.czasDostarczenia;
	}
};

struct PorownajTerminDostepnosci {
	bool operator ()(const Zadanie & zadanie1, const Zadanie & zadanie2) {
		return zadanie1.terminDostepnosci > zadanie2.terminDostepnosci;
	}
};


int main() {

	vector<Zadanie> dane;
	fstream plik;

	//tworzymy kolejke
	priority_queue < Zadanie, vector < Zadanie >, PorownajTerminDostepnosci > kolejkaTermDost; //kolejka R
	priority_queue < Zadanie, vector < Zadanie >, PorownajCzasDostarczenia > kolejkaCzasDost;  //kolejka Q

	string nazwaPliku;

	nazwaPliku = "dane.txt";
	plik.open(nazwaPliku, ios::in);

	unsigned int liczbaZadan;

	if (plik.good()) {
		Zadanie tmpObiekt;
		plik >> liczbaZadan;

		for (int i = 0; i<liczbaZadan; i++) {
			plik >> tmpObiekt.terminDostepnosci;    //wczytujemy r
			plik >> tmpObiekt.czasWykonania;        //wczytujemy p
			plik >> tmpObiekt.czasDostarczenia;     //wczytujemy q
			dane.push_back(tmpObiekt);			    //wrzucenie calego obiektu na koniec
		}

		for (int i = 0; i < liczbaZadan; i++)
		{
			kolejkaTermDost.push(dane[i]); //wrzucenie do kolejki
		}
		plik.close();

	}
	else {
		std::cout << "Nie mozna wczytac pliku" << std::endl;
	}

	// schrage z podzialem
	Zadanie e, l;
	l.czasDostarczenia = 10000; //nieskonczenie wielkie
	l.czasWykonania = 0;
	l.terminDostepnosci = 0;

	int t = 0;      //chwila czasowa
	int cMax = 0;   //maksymalny z terminow dostarczenia zadan

	while (!(kolejkaCzasDost.empty()) || !(kolejkaTermDost.empty())) {
		while (!(kolejkaTermDost.empty()) && (kolejkaTermDost.top().terminDostepnosci <= t)) {
			e = kolejkaTermDost.top();

			kolejkaCzasDost.push(e);      //wrzucenie do kolejki cale zadanie
			kolejkaTermDost.pop();        //wyrzucamy element ze zbioru N

										  //tu sie dzieje podzial! pierwsza iteracja tu nie wejdzie 😉
			if (e.czasDostarczenia > l.czasDostarczenia) {
				l.czasWykonania = t - e.terminDostepnosci; //p1=t-r
				t = e.terminDostepnosci;  //czas r=t

				if (l.czasWykonania > 0) {
					kolejkaCzasDost.push(l);
				}
			}
		}
		if (kolejkaCzasDost.empty()) {
			t = kolejkaTermDost.top().terminDostepnosci;
			continue;
		}
		e = kolejkaCzasDost.top();
		kolejkaCzasDost.pop();
		l = e;
		t = t + e.czasWykonania;
		cMax = std::max(cMax, t + e.czasDostarczenia);
		cout << (int)e.terminDostepnosci << " : " << (int)e.czasWykonania << " : " << (int)e.czasDostarczenia << endl;
	}

	cout << "C Max: " << cMax;
	getchar();
}