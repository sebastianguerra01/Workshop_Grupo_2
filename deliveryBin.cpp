#include <iostream>		    // entradas salidas en consola y string cout cin ...
#include <fstream> 		    // manejo de archivos
#include <vector>           // gestiona las listas / arreglos / buffers
#include <string>
//#include <cstring>
#include <sstream> 
#include "../lib/global.h"

using namespace std;
struct provincia
{
    unsigned int Id;    //(autoincremental)
    char sNombre[50];
    int iPoblacion;
    float fTemperatura;
    char lstPlatoTipico[100];
};
vector <provincia> lstProvincia;
const string pathfilebinary = "../resorce/database/delivery.dat";

void eliminarProvincia();
bool leerArchivoBinario();
void escribirArchivoBinario();
void acerdaDe();
void presentarProvincias();    
void buscarProvincia();
void buscarProvincia_A();
bool compararProvincia(string p, string prov);
void presentarProvincia(provincia p);

void agregarProvincia();         
bool existRepetido(string elemento);
void seekg();
void seekp();

int menu();
string tolowerStr(string str);
void segmentarCadena( const string  str, const char delim,  vector <string> &lst ); 


int main()
{
    leerArchivoBinario();
    do{
        switch (menu())
        {
            case AGREGAR:
                agregarProvincia();
                break;
            case PRESENTAR:
                presentarProvincias();
                break;
            case BUSCAR_P:
                //buscarProvincia();
                buscarProvincia();
                break;
            case BUSCAR_P_A:
                buscarProvincia_A();
                break;
            case GUARDAR_BINARIO:
                escribirArchivoBinario();
                break;
            case ELIMINAR:
                eliminarProvincia();
                break;
            case SEEKG:
                seekg();
                break;
            case SEEKP:
                seekp();
                break;
            case ACERCA_DE:
                acerdaDe();
                break;
            case SALIR:
                cout << "bye bye ...";
                exit(0);
            default:
            cout << "Selecci�n no valida \n";
                break;
        }
    }while (true);
} 

void seekg()
{
    setColor(15,1);
    ifstream f(pathfilebinary, ios::in | ios::binary);
    cout << "Abrir archivo:" << pathfilebinary << endl;
    if(!f) 
    {
        cout << " << error >>" << endl;
    } 
    cout << "------ Posicionando Puntero -------" << endl;
    int n;
    cout << "Indique la Id de la provincia: ";
    cin >> n;
    int length = f.tellg();
    f.seekg((n - 1) * sizeof(provincia));
    provincia tp[1];
      f.read((char *) &tp[0], sizeof(provincia));
    presentarProvincia(tp[0]);
    f.close();
    setColor(0,15);
}

void seekp()
{
    ofstream f(pathfilebinary, ios::in | ios::binary);
    cout << "Abrir archivo:" << pathfilebinary << endl;
    if(!f) 
    {
        cout << " << error >>" << endl;
    } 
    cout << "------ Posicionando Puntero Para Reescribir -------" << endl;
    int n;
    cout << "Indique la Id de la provincia: ";
    cin >> n;
    int length = f.tellp();
    f.seekp((n - 1) * sizeof(provincia));
    provincia tp[1];
    f.write((char *) &tp[0], sizeof(provincia));
    presentarProvincia(tp[0]);
    f.close();
}

bool leerArchivoBinario()
{
    ifstream rf(pathfilebinary, ios::in | ios::binary);
    cout << "Abrir archivo:" << pathfilebinary << endl;
    if(!rf) 
    {
        cout << " << error >>" << endl;
        return false;
    } 
    provincia tp[1];
    while(rf.read((char *) &tp[0], sizeof(provincia)))
    {
        tp[0].Id = (lstProvincia.size()+1);
        lstProvincia.push_back(tp[0]);
    }
    rf.close();
    return true;
}
void escribirArchivoBinario()
{
    ofstream f(pathfilebinary, ios::out | ios_base::binary );
    cout << endl << "Escribir archivo: " << pathfilebinary << endl;
    if(f.good())
    {
        for (auto &&prov : lstProvincia)  // { [*],[*],[*]...}
            f.write((char *) &prov, sizeof(provincia));
        //f.write((char*)&lstProvincia[0], lstProvincia.size() * sizeof(provincia));
    }
    f.close();
}

void agregarProvincia()
{
    setColor(15,1);
    string str= "s";
    do
    {
        struct provincia prov;
        cout <<endl << "-- Ingresar datos de provincia  -- " << endl ;
        prov.Id = lstProvincia.size()+1;
        cout <<"<+> Nombre: ";
        getline (cin, str);
        strcpy(prov.sNombre ,str.c_str());

        cout <<"<+> Poblacion: ";
        cin >> prov.iPoblacion;
        
        cout <<"<+> Temperatura: ";
        cin >> prov.fTemperatura;

        cin.clear();
        fflush(stdin);

        cout <<"<+> PlatoTipico (separados por coma): ";
        //cin >> str;
        getline (cin, str);
        strcpy(prov.lstPlatoTipico ,str.c_str());

        //cout << "Esta registrando la provincia " <<listarProvincias()<< endl;
        
        //system ("pause");

        if (!existRepetido(prov.sNombre))
            lstProvincia.push_back(prov);
        else
            cout << "Ya existe en el archivo" <<endl;
        
        cout << "Deseas salir (s): " ;
        cin >> str;
        cin.clear();
        //fflush(stdin);
        //cin.ignore();
    } while (str != "s");
    setColor(0,15);
}

bool existRepetido(const string elemento)
{
    for (auto &&p : lstProvincia)
        if (tolowerStr(p.sNombre) == tolowerStr(elemento))
            return true;
    //bool coicidencias()
    return false;
}


void eliminarProvincia()
{
    cout << "------ Eliminar Provincia -------" << endl; 
    string prov = "";
    int indexP=0;

    cout << endl << " Ingresa la provincia a eliminar: ";
    getline(cin, prov);

    for (auto &&p : lstProvincia) 
    {
        if (tolowerStr(p.sNombre) == tolowerStr(prov))
        {
            cout <<" Se elimino: " << prov;
            lstProvincia.erase(lstProvincia.begin()+indexP);
            break;
        }
        indexP++;
    }
}

void buscarProvincia()
{
    setColor(15,1);
    cout << "------ Buscar Provincia -------" << endl;

    bool encondrado = false;
    string prov = "";
    cout <<"Ingresa el Nombre de la provincia: ";
    getline (cin, prov);

    for (auto &&pro : lstProvincia)
    {
        if (tolowerStr(pro.sNombre) == tolowerStr(prov))
        {
            presentarProvincia(pro);
            encondrado = true;
        }
    }
    if (!encondrado) // if(ban == 0 )
    cout <<"No se encontro la provincia " << prov << endl; 
    setColor(0,15);
}
void presentarProvincia(provincia p)
{
    setColor(15,1);
    setColor(15,4);
    cout<<"Id"              << p.Id << endl;
    setColor(15,1);
    cout<<"Nombre: "        << p.sNombre<<endl;
    cout<<"Poblacion: "     << p.iPoblacion<<endl;
    cout<<"Temperatura: "   << p.fTemperatura<<endl;
    cout<<"Platos-Tipicos: "   << p.lstPlatoTipico<<endl;

    vector <string> lstPlatoTipico; 
    segmentarCadena (p.lstPlatoTipico, ',', lstPlatoTipico); 
    for (const auto &pt: lstPlatoTipico)  
        cout <<"\t" << pt << endl ;  
    setColor(0,15);
}
void presentarProvincias()
{
    cout<<endl<<"-------    Provincias   ----------"<<endl;
    for(auto &&prov : lstProvincia)
        presentarProvincia(prov);
}

bool compararProvincia(string p, string prov)
{
    int count = 0, fact = 2;
    for (int i = 0; i < p.length(); i++)
        if (p[i] == prov[i])
            count++;
    if((p.length()-fact) <= count)                   //pichincha =  9 - 2 = 7  --> count = 6 coincidencias
        return true;
    return false;
}

void buscarProvincia_A()
{
    setColor(15,1);
    cout << "------ Buscar Provincia Avazando -------" << endl;
    int i = 0;
    string prov;
    cout <<"Ingresa el Nombre de la provincia: ";
    getline(cin, prov);

    for (auto &&p : lstProvincia)
    {
        if (compararProvincia(tolowerStr(p.sNombre), tolowerStr(prov)))
            presentarProvincia(p);
    }
    setColor(0,15);
}

int menu()
{
    setColor(15,1);
    int eleccion;
    do
    {
        cout << endl
        << "()---Provincias del Ecuador---()\n"         << endl
        << "Ingresa una opcion: \n"                  << endl
        << AGREGAR      << " | Agregar provincias    |"  << endl
        << PRESENTAR    << " | Listado de provincias |"  << endl
        << BUSCAR_P    << " | Buscar provincia      |"  << endl
        << BUSCAR_P_A    << " | Buscar p. avanzado    |"  << endl
        << GUARDAR_BINARIO      << " | Guardar provincias    |"  << endl
        << ELIMINAR    << " | Eliminar provincia    |"  << endl
        << SEEKG    << " | Posicionarse provincia|"  << endl
        << SEEKP    << " | Reescribir provincia  |"  << endl
        << ACERCA_DE    << " | Acerda de             |"  << endl
        << SALIR        << " | Salir del sistema     |"  << endl
        << "\nSu seleccion:";
        cin >> eleccion;
        cin.clear();
        fflush(stdin);
    }while (eleccion < 0 || eleccion > 10); 
    return eleccion;
    setColor(0,15);
}
string tolowerStr(string str)  
{
    for (int x=0; x < str.length(); x++)
        str[x] = tolower(str[x]);
    return str;
}
void segmentarCadena( const string  str, const char delim,  vector<string> &outlst )  
{  
    string strDiv;  
    stringstream s(str);  
    while ( getline (s, strDiv, delim) )  
        outlst.push_back(strDiv);  
}

void acerdaDe()
{
    setColor(15,4);
    cout << "------ Grupo 3 -------" << endl<< endl;
    cout << "estefano.proano@epn.edu.ec" << endl<< endl;
    cout << "josue.cuenca@epn.edu.ec" << endl<< endl;
    cout << "sebastian.guerra01@epn.edu.ec" << endl<< endl;
    cout << "angel.anguaya02@epn.edu.ec" << endl<< endl;
    cout << "cristian.hernandez@epn.edu.ec" << endl<< endl;
    cout << "josting.valeriano@epn.edu.ec" << endl;
    setColor(0,15);
}

/* 

TAREA: deliberyBin
- 0.5 por item resuelto
- Equipos de 5 o 6 personas

1| Agregar provincias      |  x << arreglar, evitar repetidos, Agregar a la estructura unsigned int Id (autoincremental)
2| Presentar provincias    |  x << listar todas las provincias
3| Buscar provincia        |   << pedir nombre de provincia ... buscar y presentar : LOja 
4| Buscar Avanzada         |   << pedir nombre de provincia ... buscar y presentar las que coindidan : loxa, --> loja
5| Guardar provincias      |  x
6| Eliminar provincias     |
0| Salir del sistema      |   x

>> captuas de pantalla del trabajo colaborativo
<< investigar como recupera directamente del archivo la estructura de datos >>
>> agregar color 0,1

    seekp reescribir
    seekg leer
*/