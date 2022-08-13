/* 
 * File:   main.cpp
 * Author: Stan
 *
 * Created on 21 Февраль 2021 г., 13:21
 */

#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <conio.h>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <vector>
#include <fstream>

using namespace std;

/*
 * 
 */
//Функция чтения из файла.
void Read(string ipath, vector<string> *sockets){
    vector<string> sockets2;//Второй вектор, чтобы программа не ругалась.
    ifstream read(ipath.c_str());//Поток чтения из файла.
    string r;//Строка для записи из потока.
    
    //Открытие и чтение из потока.
    if (read.is_open()){
        while(getline(read,r,'\n')){
            sockets2.push_back(r);           
        }
    }
    read.close();//Закрытие потока.
    
    *sockets=sockets2;//Передача данных из второго вектора в указатель.
        
    }

//Функция, возвр. слово под номером z из строки x с разделителем y
string SplitFind(string x, char y, int z)
{
    int dcount=0;//Счётчик разделителей
//    int wcount;
    for(int i=0;i<=x.length();i++)
    {
       if (x[i]==y) dcount++;//Когда символ i из строки х равен y 
       //счётчик прибавляется
    }
//    wcount=dcount+1;
    
    if (z>dcount) return "";//Выход из функции в случае слишком большого
    //номера слова
    
    vector <string> vx;//Объявление строкового вектора слов vx 
    
    istringstream iss(x);//Объявление строкового потока iss с входной 
    //строкой x в качестве аргумента
    while(getline(iss,x,y))//Цикл, перебирающий и делящий строку х на слова
    //с разделителем y    
    {
        vx.push_back(x);//Добавление слов из строки x в вектор vx 
    }  
    return vx[z];//Возвращаемое функцией значение в виде слова
    //(элемента вектора vx с номером z)
    }

void Range(string *range){
    stringstream *rs;
    rs=new stringstream;
    vector <int> rv;
        
    *rs<<*range;
    
    while(getline(*rs,*range,'-')){
    stringstream *s2i;
    s2i=new stringstream;
    int *n;
    n=new int;
    *s2i<<*range;
    *s2i>>*n;
    rv.push_back(*n);
    delete n;
    delete s2i;    
    }
    delete rs;
    
    stringstream *css;
    css=new stringstream;
    for(int i=rv[0];i<=rv[1];i++)
    *css<<i<<',';
        
    *css>>*range;
    delete css;
    
}

void ComProc(string port, string ad, string opath){

string cmdl;//Строка с командой.
stringstream cmdls;//Поток с командой.
stringstream ps;//Поток с портами.
int hc;//Счётчик тире.

ps<<port;//Запись порта(ов) в поток.
 
   //Деление переменной портов запятыми через поток портов. 
    while(getline(ps,port,',')){
    
    //Если в переменной с портами есть тире.    
    if (port.find("-")!=-1){ 
    Range(&port);//Преобразование диапазона в значения через запятую.
    ComProc(port,ad,opath);//Рекурсивный вызов функции для обработки полученных
    //значений через запятую.
    }
    
    //Если в переменной портов нет запятых.
    if (port.find(",")==-1){
    //Формирование Powershell-команды для проверки портов.
    cmdls<<"powershell.exe Test-NetConnection"<<" "<<ad<<" "<<"-port"<<" "<<
    port<<">>"<<opath;
    cmdl=cmdls.str();
    system(cmdl.c_str());
    cmdls.str("");
    }
    }

}

int main(int argc, char *argv[]) {
    string cmdl;//Команда.
    string socket;//Пара адрес:порт(ы)(если портов несколько, то через запятую).
    string opath;//Путь.
    string ipath;//Путь для чтения.
    stringstream cmdls;//Поток с командой.
    vector <string> sockets;//вектор для адресов и портов.
if (argc==1){        
    cout<<"PortScanner 1.1, Maxim Badaev, 2021"<<endl;
    cout<<"Press 'T' to input socket from the keyboard, press 'N' to type next "
    "socket while typing sockets from the keyboard, press 'I' to import values "
    "from file. Set the English keyboard layout before pressing."<<endl;
    switch(getch()){
    case 116://Буква T(eng.).Для набора с клавиатуры.
    cout<<"Input socket"<<endl;
    NextSocket:
    getline(cin,socket);
    sockets.push_back(socket);
    if (getch()==110) goto NextSocket;//Буква N(eng.). Следующий адрес.    
    break;
    case 105://Буква I(eng.). Для чтения из файла.
    cout<<"Type import path:"<<endl;
    getline(cin,ipath);
    Read(ipath,&sockets);
    break;
    }
    
    cout<<"Input path"<<endl;//Ввод пути для записи.
    getline(cin,opath);
}
else if(argc>1){
    ipath=argv[1];
    opath=argv[2];
    Read(ipath,&sockets);
}    
    for(int i=0;i<sockets.size();i++){
    string *ad;//Адрес.
    ad=new string;
    string *port;//Порт(ы).
    port=new string;
    
    
    //Присвоение значений временным переменным адреса, порта и потока портов.
    *ad=SplitFind(sockets[i],':',0);
    *port=SplitFind(sockets[i],':',1);
    
    ComProc(*port,*ad,opath); 
   
    //Удаление временных переменных.
    delete ad;
    delete port;
    }
    system("pause");
    return 0;
}

