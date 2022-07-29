#include <iostream>		    // entradas salidas en consola y string cout cin ...
#include <fstream> 		    // manejo de archivos
#include <vector>           // gestiona las listas / arreglos / buffers
#include <string>
#include <cstring>
#include <sstream> 
#include "../lib/global.h"

using namespace std;
struct provincia
{
    unsigned int Id;
    char sNombre[50];
    int iPoblacion;
    float fTemperatura;
    char lstPlatoTipico[100];
};
vector <provincia> lstProvincia;
const string pathfilebinary = "resorce\\database\\delivery.dat";

bool existRepetido(string elemento);
bool leerArchivoBinario();
void escribirArchivoBinario();
void buscarAv();
void presentarProvincias();      
void presentarProvincia();      
void presentarProvincia(provincia p);
void borrarProvincias();
void agregarProvincia(); 
void asignarId();        

int menu();
string tolowerStr(string str);
void segmentarCadena( const string  str, const char delim,  vector <string> &lst ); 


int main()
{
    leerArchivoBinario();
    do{
        SetColor(BLACK,WHITE);
        switch (menu())
        {
            
            case AGREGAR:
                SetColor(BLACK,BLUE);
                agregarProvincia();
                break;
            case PRESENTAR:
                presentarProvincias();
                break;
            case BUSCAR:
                SetColor(BLACK,CYAN);
                presentarProvincia();
                break;
            case BUSCARAV: 
                SetColor(BLACK,MAGENTA);
                buscarAv();
                break;
            case GUARDAR:
                SetColor(BLACK, LGREEN);
                escribirArchivoBinario();
                break;
            case ELIMINAR:
                SetColor(BLACK, LRED);
                borrarProvincias();
                break;
            case SALIR:
                SetColor(BLACK,LGREEN);
                cout << "Gracias por usarnos!";
                exit(0);
            default:
            SetColor(BLACK,RED);
            cout << "Seleccion no valida \n";
                break;
        }
    }while (true);
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
        lstProvincia.push_back(tp[0]);
    rf.close();
    asignarId();
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
    string str= "s";

    do
    {
        struct provincia prov;
        cout <<endl << "-- Ingresar datos de provincia  -- " << endl ;
        
        cout <<"<+> Nombre: ";
        cin.getline(prov.sNombre,sizeof(prov.sNombre));
        if(existRepetido(prov.sNombre))
            break;
        //cin.ignore(10000, '\n');

        cout <<"<+> Poblacion: ";
        cin >> prov.iPoblacion;
        cin.ignore(10000, '\n');

        cout <<"<+> Temperatura: ";
        cin >> prov.fTemperatura;

        cin.ignore(10000, '\n');

        cout <<"<+> PlatoTipico (separados por coma): ";
        cin.getline(prov.lstPlatoTipico,sizeof(prov.lstPlatoTipico));

        lstProvincia.push_back(prov);
        

        cout << "Deseas salir (s): " ;
        cin >> str;
        cin.ignore();
        //fflush(stdin);
        //cin.ignore();
    } while (str != "s");
    asignarId();
}
void presentarProvincia()
{
    cout << "------ Buscar Provincia -------" << endl;
    bool encontrado = false;
    char opcion;
    int inp;
    string prov = "";
    do
    {
        cout <<"Ingrese (i) para usar el Id de la provincia, o (n) para buscar por el nommbre: ";
        cin>>opcion;
        cin.ignore();
    }while(opcion!='n'&&opcion!='N'&&opcion!='i'&&opcion!='I');
    if(opcion=='n'||opcion=='N')
    {
        cout<<"Ingrese el nombre de la provincia"<<endl;
        getline (cin, prov);

        for (auto &&provincia : lstProvincia)
        {

            if (tolowerStr(provincia.sNombre) == tolowerStr(prov))
            {
                presentarProvincia(provincia);
                encontrado = true;
            }
        }
        if (!encontrado) // if(ban == 0 )
            cout <<"No se encontro la provincia " << prov << endl;
    }
    else if(opcion=='i'||opcion=='I')
    {
        cout<<"Ingrese el ID de la provincia: "<<endl;
        cin>>inp;
        cin.clear();
        for (auto &&provincia : lstProvincia)
        {
            if (inp == provincia.Id)
            {
                presentarProvincia(provincia);
                encontrado = true;
            }
        }
        if (!encontrado) // if(ban == 0 )
            cout <<"No se encontro la provincia " << prov << endl;
    }
}
void presentarProvincia(provincia p)
{
    SetColor(BLACK,CYAN);
    cout << "__________________" <<endl;
    SetColor(BLACK,BROWN);
    cout<<"ID: "<<p.Id<<endl;
    cout<<"Nombre: "        << p.sNombre<<endl;
    cout<<"Poblacion: "     << p.iPoblacion<<endl;
    cout<<"Temperatura: "   << p.fTemperatura<<endl;
    cout<<"Platos-Tipicos: "   << p.lstPlatoTipico<<endl;
    
    vector <string> lstPlatoTipico; 
    segmentarCadena (p.lstPlatoTipico, ',', lstPlatoTipico); 
    for (const auto &pt: lstPlatoTipico)  
        cout <<"\t" << pt << endl ;  
}
void presentarProvincias(){
    cout<<endl<<"-------    Provincias   ----------"<<endl;
    for(auto &&prov : lstProvincia)
        presentarProvincia(prov);
}

int menu()
{
    int eleccion;
    do
    {
        cout << endl
        << "  ()---Provincias del Ecuador---()\n"         << endl
        << "         Ingresa una opcion: \n"                  << endl
        << AGREGAR      << " |       Agregar provincias       |"  << endl
        << PRESENTAR    << " |      Listado de provincias     |"  << endl
        << BUSCAR       << " |      Buscar una provincia      |"  << endl
        << BUSCARAV     << " | Busqueda avanzada de provincia |"  << endl
        << GUARDAR      << " |      Guardar los cambios       |"  << endl
        << ELIMINAR     << " |     Eliminar una provincia     |"  << endl
        << SALIR        << " |       Salir del programa       |" <<endl
        << "Su seleccion:";
        cin >> eleccion;
        cin.ignore(10000, '\n');
        if(eleccion < 0 || eleccion > 7)
            cout<<"Seleccion no valida, intentelo de nuevo"<<endl;
    }while (eleccion < 0 || eleccion > 7); 
    return eleccion;
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

void borrarProvincias()
{
    string nombreProvincia ;
    int i=0;
    bool encontrado=false;

    cout << endl << " Ingrese la provincia a eliminar: ";
    getline(cin,nombreProvincia);
    cin.clear();

    for (auto &&provincia : lstProvincia) 
    {
        if (tolowerStr(provincia.sNombre)==tolowerStr(nombreProvincia))
        {
            cout <<" Se eliminara: " << nombreProvincia;
            lstProvincia.erase(lstProvincia.begin()+i);
            asignarId();
            encontrado=true;
            break;
        }
        i++;
    }
    if(!encontrado)
        cout<<"No existe esa provincia en la base de datos"<<endl;
}
void buscarAv()
{
    bool encontrado=false, aceptado=true;
    int count;
    char conf;
    string input;
    cout<<"Ingrese la provincia a presentar"<<endl;
    getline(cin, input); 
    cin.clear();
    for (auto &&i : lstProvincia)
    {
        count=0;
        string lower=tolowerStr(i.sNombre);
        string linput=tolowerStr(input);
        for(int x=0;x<=linput.length();x++)
        {
            if(lower[x]==linput[x]&&linput[x]!=' ')
                count++;
            if(count>=3)
            {   
                
                cout<<"Acaso quiso decir "<<i.sNombre<<"? Y/N"<<endl
                    <<"(+)";
                cin>>conf;
                cin.clear();
                if(conf=='Y'||conf=='y')
                {
                    presentarProvincia(i);
                    encontrado=true;
                    break;
                } else if(conf=='N'||conf=='n')
                    break;
                if(conf!='Y'&&conf!='y'&&conf!='n'&&conf!='N')
                    cout<<"Ingreso no valido, intentelo de nuevo"<<endl;
            }
        }
         
    }
    if(!encontrado)
        cout<<"No se ha encontrado ninguna similaridad";
}

bool existRepetido(string elemento)
{
    bool ban=false;
    for (auto &&e : lstProvincia)   //--> pets = {perros ,gatos ,loros ,nutrias, gatos , loros }
        if (tolowerStr(e.sNombre)==tolowerStr(elemento))
        {
            cout <<"La provincia "<< tolowerStr(elemento) << " ya existe!";
            ban=true;
            continue;
        }
    return ban;
}
void asignarId()
{
    int x=1;
    for (auto &&i : lstProvincia)
    {
        i.Id=x;
        x++;
    }
    
}


/* 

TAREA: deliberyBin
- 0.5 por item resuelto
- Equipos de 5 o 6 personas

 | Agregar provincias      |   << arreglar, evitar repetidos, Agregar a la estructura unsigned int Id (autoincremental)
 | Presentar provincias    |   << listar todas las provincias
 | Buscar provincia        |   << pedir nombre de provincia ... buscar y presentar : LOja 
 | Buscar Avanzada         |   << pedir nombre de provincia ... buscar y presentar las que coindidan : loxa, --> loja
 | Guardar provincias      |
 | Eliminar provincias     |
0| Salir del sistema      |

>> captuas de pantalla del trabajo colaborativo
<< investigar como recupera directamente del archivo la estructura de datos >>
>> agregar color 0,1

*/