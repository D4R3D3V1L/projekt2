// Kacper Raczyński - grupa 9:15 poniedziałek
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int MAX_INT = 2147483647;



// SCIEZKA //



void path(int *prev, int *cost, int vert)
{
    int *display = new int [vert];  

    int n = 0;
    int m;

    for(int c = 0; c < vert; c++)    
    {
        cout << c <<": ";
        for(m = c; m != -1 && n < vert; m = prev[m]){
            display[n++] = m;
        }
        while(n){
            cout << display[--n] << " ";
        }
        cout << "$" << cost[c] << endl;
    }
    delete [] display;             
}



//  MACIERZ //



void display_M(int **graph, int vert) {
    for(int i=0; i<vert; ++i){
        for(int j=0; j<vert; ++j){
            cout<<graph[i][j]<<" ";
        }
        cout<<endl;
    }
}



void **graph_M(int &vert, int &edge, int&start, int **graph, int **graphm){
    int v1, v2, w;
    int p = 0;

    for(int i=0; i<vert; ++i){
        for(int j=p; j<vert; ++j){
            graphm[i][j]=0;
            graphm[j][i]=0;
        }
        ++p;
    }
    for(int i=0; i<edge; ++i){
        v1=graph[i][0];
        v2=graph[i][1];
        w=graph[i][2];
        graphm[v1][v2]=w;
    }
}



void Matrix_BF(int** graph, int vert, int edge, int start) {
    int* cost;
    cost = new int[vert];
    int* prev;
    prev = new int[vert];

    for(int i=0; i<vert; ++i){
        prev[i] = -1;
        cost[i] = MAX_INT;
    }
    cost[start] = 0;

    for(int j=1; j<vert; ++j){
        for(int k=0; k<vert; ++k){
            for(int l=0; l<vert; ++l){
                if( cost[k]!=MAX_INT   &&   graph[l][k]!=0   &&   cost[l]>cost[k]+graph[l][k] ){
                    cost[l] = cost[k] + graph[l][k];
                    prev[l] = k;
                }
            }
        }
    }
    //cout <<"Macierz sasiedztwa:\n";
    //path(prev, cost, vert);
}



void path_M(int **graph, int &vert, int &edge, int &start, fstream &out)
{
    double t; //czas trwania
    clock_t t1, t2; // pukty czasu

    int **graphm;  
    graphm = new int *[vert];

    for(int i=0; i<vert; ++i)
    {
        graphm[i] = new int[vert];
    }

    graph_M(vert, edge, start, graph, graphm);   // funkcja odpowiedzialna za tworzenie macierzy sasiedztwa

    t1 = clock(); 

    Matrix_BF(graphm, vert, edge, start); // najkrotsza sciezka BF macierzy

    t2 = clock() - t1;        

    for(int i = 0; i < vert; ++i) // usuwamy macierz
    {
        delete [] graphm[i];
    }
    delete [] graphm;

    t = (double) (t2) / CLOCKS_PER_SEC; // konwertujemy czas trwania na zmienna double

    out << "a dla macierzy: " << t <<"s"<<endl;
}



//  LISTA   //



struct list  // lista sasiędztwa dla jednego wierzchołka
{
    int v,w;    // v -> sasiedzi , w -> waga
    list* next;
};



void displayL(list** tab_list, int vert)
{
    list* List;
    cout<< "lista:"<<endl;
    for(int i = 0; i < vert; i++)
      {
        cout << "tab[" << i << "] =";
        List = tab_list[i];
        while(List)
        {
          cout << List->v <<" ";
          List = List->next;
        }
        cout << endl;
    }
    delete [] List; // usuwamy liste ddo wyswietlania
}



void List_BF(int vert, int edge, int start, list** tab_list)
{
    list* n;  //lista sasiadow

    int* cost;                 //inicjujemy wskaznik na tablice dynamiczna kosztow dojscia
    cost = new int [vert];
    int* prev;                    //inicjujemy wskaznik na tablice dynamiczna poprzednikow
    prev = new int [vert];

    for(int i = 0; i < vert; i++)
    {
        prev[i] = -1;             //przypisujemy wartosci tablicy poprzednikow -1 nie ma wierzcholka ujmenego
        cost[i] = MAX_INT;     //koszty dojscia ustawiamy na nieskonczonosc, tutaj na maksymalna wartosc inta
    }

    cost[start] = 0;           //zerujemy koszt dojscia do naszego wierzcholka startowego

    for(int i = 1; i < vert; i++)
    {
        for(int j = 0; j < vert; j++)    //przeszukujemy liste sasiedztwa w poszukiwaniu krawedzi dla danego wierzcholka relaksujac kolejne krawedzie
        {
            for(n = tab_list[j]; n; n = n -> next)
            {
                if(cost[j] != MAX_INT && cost[n -> v] > cost[j] + n -> w)
                {
                    cost[n -> v] = cost[j] + n -> w;
                    prev[n -> v] = j;
                }
            }
        }
    }
    //cout <<"Lista sasiedztwa:\n";
    //path(prev, cost, vert);
}



void graph_L(int &vert, int &edge, int &start, int **graph, list** tab_list, list* List)
{
    int v1, v2, w;

    for(int i = 0; i < vert; i++) 
    {
        tab_list[i] = NULL;
    }

    for(int i = 0; i < edge; i++)   //tworzymy listy dla grafu skierowanego
    {
        v1 = graph[i][0];    //wczytujemy dane z reprezentacji grafu z pliku
        v2 = graph[i][1];
        w = graph[i][2];

        List = new list;

        List->v = v2;
        List->w = w;
        List->next = tab_list[v1];
        tab_list[v1] = List;

    }
}



void path_L(int **graph, int &vert, int &edge, int &start, fstream& out)
{
    double t;
    clock_t t1, t2;           
    list** tab_list;
    list* List, *del;      // struktury dynamiczne odpowiadajace za reprezentacje grafu w postaci listy sasiedztwa

    tab_list = new list * [vert]; //alokujemy pamiec dla tablicy list sasiedztwa

    graph_L(vert, edge, start, graph, tab_list, List);    //funkcja odpowiedzialna za tworzenie tablicy dynamicznej list sasiedztwa

    t1 = clock();                

    List_BF(vert, edge, start, tab_list);   //funkcja odpowiedzialna za szukanie najkrotszej sciezki algorytmem Bellmana-Forda dla listy sasiedztwa

    t2 = clock() - t1;       

      for(int i = 0; i < vert; i++) //usuwamy listy z tablicy list
      {
        List = tab_list[i];
        while(List)
        {
          del = List;
          List = List->next;
          delete del;
        }
      }
        delete [] tab_list;   //usuwamy tablice list

    t = (double) (t2) / CLOCKS_PER_SEC;       //konwertujemy czas trwania na zmienna double i obliczamy

    out << "Czas trwania dla listy przy "<<vert<<" wierzcholkach i "<<edge*100/(vert * (vert - 1) /2)<<"% wypelnienia wynosi "<<t<<"s";
}



// DANE //



void write_data(int vert, int edg, fstream& str) ///funkcja wypisujaca wygenerowane grafy do pliku
{
    int rest = edg - vert + 1;  //liczba wykonan pozostalych po stworzeniu sciezki laczacej wszystkie wierzcholki grafu
    int tmp = 0;
    int nvert = vert;
    int count = 0;

    str.open("dane.txt", ios_base::app);           //otwieramy strumien danych zapisu do pliku

    str << vert<<" "<<edg<<" "<<rand()%vert<<endl; //wypisujemy ilosc wierzcholkow, krawedzi i wierzcholek startowy wierzcholek startowy oraz wagi krawedzi sa pseudolosowe
    
    for(int i = 0; i < vert - 1 && i < edg -1 ; i++)   //wypisujemy glowny cykl laczacy wszystkie wierzcholki po kolei
    {
        str << i<< " "<<i+1<<" "<<rand()%100+1<<endl;
    }

    while(count < rest) //wypisujemy pozostale krawdzie w zaleznosci od wypelnienia
    {
        tmp = 0;

        while(count < rest && tmp < nvert - 2)
        {
            str << nvert - 1<<" "<< tmp <<" "<<rand()%10+1<<endl;

            count++;
            tmp++;
        }
        nvert--;
    }
    str.close();           //zamykamy strumien do pliku

}

void generate(fstream& str)
{
    int vert[5] = {10, 50, 100, 300, 500};  // tablica ilosci wierzcholkow zadanych w poleceniu projektu
    int max_edg[5];                            // tablica maksymalnych liczb krawedzi
    int edg[5][4];                            // tablica liczb krawedzi dla kazdego z przypadkow zadanych w projekcie
    int fill[4] = {25, 50, 75, 100};         // tablica % wypelnien zadanych przypadkow

    str.open("dane.txt", ios_base::out);       // otwieramy strumien do zapisu danych do pliku dane.txt
    str.close();                               // zamykamy go, daje to pewnosc, ze plik uprzednio zostal wyczyszczony z poprzednich danych

    for (int i = 0; i < 5; i++) // generujemy kolejne przypadki dla ktorych pozniej rozpoczynamy             5
// wypisywanie grafow do pliku poprzez funkcje wypisz_dane
    {
        max_edg [i] = vert[i] * (vert[i] - 1) /2;
        for (int z = 0; z < 4; z++) // 4
        {
            edg[i][z] = fill[z] * max_edg[i] /100;
            for(int c = 0; c < 3; c++) write_data(vert[i], edg[i][z], str); // 100
        }
    }

}





void** read(int** graph, int& vert, int& edge, int& start, ifstream& inp)
{
    for(int i = 0; i < edge; i++)    //wczytujemy graf z pliku do tablicy dynamicznej 2-wymiarowej
    {
        inp >> graph[i][0];
        inp >> graph[i][1];
        inp >> graph[i][2];
    }
}



// Pomiar //

void find(ifstream& inp)
{
    fstream out;
    int vert, edg, start;
    int** graph;

    out.open("pomiar.txt", ios_base::app);  //otwieramy strumien wyjsciowy do pliku pomiar.txt z prawem do zapisu w formie dopisywania danych

    for(int i = 0; i < 60; i++) //2000
    {


        inp >> vert;             //wczytujemy kolejno: liczbe wierzcholkow, krawedzi i wierzcholek startowy
        inp >> edg;
        inp >> start;

        graph = new int* [edg];    //alokowanie pamieci dla tablicy dynamicznej dwuwymiarowej

        for(int c = 0; c < edg; c++)
        {
            graph[c] = new int [3];
        }

        read(graph, vert, edg, start, inp);          //inicjujemy wczytywanie grafu z pliku
        path_L(graph, vert, edg, start, out);        //inicjujemy szukanie drogi dla listy sasiedztwa
        path_M(graph, vert, edg, start, out);      //inicjujemy szukanie drogi dla macierzy sasiedztwa

        for(int z = 0; z < edg; ++z) // usuwamy tablice dynamiczna przechowujaca dane grafu z pliku
        {
            delete [] graph[z];
        }
        delete [] graph;
    }
    out.close();    // zamykamy strumien wyjsciowy
}

void test_path(ifstream& str)
{

    str.open("dane.txt", ios_base::in); //otwarcie strumienia do pliku z danymi wejsciowymi dla grafow z prawem do odczytu danych

    find(str);           // uruchamiamy proces odnajdywania drogi dla grafow

    str.close();                        //zamykamy strumien danych

}



// MAIN //



int main(){

    fstream data;
    ifstream input;       //zainicowanie zmiennych odpowiedzialnych za strumienie danych

    srand(time(NULL));      //ustawienie pomiaru czasu od czasu zero

    generate(data);     //generujemy dane
    test_path(input);   //szukamy drogi
}