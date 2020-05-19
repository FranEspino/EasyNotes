#include <windows.h>
#include <string.h>
#include <mysql.h>
#include <stdio.h>
#define BTN_SALIR 100
#define EDIT_1 102
#define EDIT_2 103
#define AYUDA_PES 104
#define BTN_ABRIR 108
#define ID_PROGRAMADOR 105
#define USER 106
#define CLAVE 107
#define BTN_SALIR2 109
#define ID_INFO 110
#define ID_BTNSAVE 111
MYSQL* conn;
MYSQL_ROW row;
MYSQL_RES *res;

HWND ventana,hedit;
HMENU hMenu;
HWND hiconUser,hiconEntrar,hiconSalir,hiconLogo,lbl_1,lbl_2,lbl_3;

char usuario[20],clave[20];
void AddMenus(HWND);
void Ventana_Inicio(HINSTANCE);
void mostrarInicio(HWND);
char respuesta[20];


void ocultarConsola(){
     HWND Stealth;
     AllocConsole();
     Stealth = FindWindowA("ConsoleWindowClass", NULL);
     ShowWindow(Stealth,0);
}
int login=0;
LRESULT CALLBACK procLogin(HWND ventana,UINT msg, WPARAM wParam,LPARAM lParam){

   static HINSTANCE Instancia;
    switch(msg){

        case WM_CREATE:{
                Instancia =((LPCREATESTRUCT)lParam)->hInstance;

        break;
        }

        case WM_COMMAND:{

            switch(wParam){


            }
            if((HWND)lParam==hiconEntrar){

                   GetDlgItemText(ventana,USER,usuario,20);
                   GetDlgItemText(ventana,CLAVE,clave,20);
                   char consulta1[1024];
                   char consulta2[1024];

                   sprintf(consulta1,"SELECT *FROM users");
                   mostrarDatos(conn,consulta1,row,res);
                   sprintf(consulta2,"SELECT *FROM users WHERE nombre_usuario='%s' AND clave_usuario='%s'",usuario,clave);
                   int r = validarLogin(conn,consulta2,row,res);
                   if(r==1){
                   ShowWindow(GetForegroundWindow(),SW_HIDE);

                   mostrarInicio(ventana);
                   }else{
                   MessageBox(ventana,"Usuario o contraseña incorrecta","Usuario no resgistrado",MB_DEFBUTTON1|MB_OKCANCEL|MB_ICONQUESTION);
                   }
            }
            if((HWND)lParam==hiconSalir){
                if(MessageBox(ventana,"Estas seguro que desea salir?","Salir",MB_DEFBUTTON1|MB_OKCANCEL|MB_ICONQUESTION)==IDOK){
                    SendMessage(ventana,WM_DESTROY,0,0);
                }
            }

            if((HWND)lParam==lbl_3){
             system("start http://localhost/easynotes/");
            }

        break;
        }

        case WM_CTLCOLORSTATIC:{
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkColor(hdcStatic,RGB(105,214,244));
            return (INT_PTR)CreateSolidBrush(RGB(105,214,244));
        }

        case WM_CTLCOLORBTN:{
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkColor(hdcStatic,RGB(105,214,244));
            return (INT_PTR)CreateSolidBrush(RGB(105,214,244));
        }

        case WM_DESTROY:{
            PostQuitMessage(0); //este cero pasará al bucle para que acabe el programa
            break;
        }

        default:{
        return DefWindowProc(ventana, msg,wParam,lParam);
        }

    }
    return 0;

}


LRESULT CALLBACK procInicio(HWND ventana,UINT msg, WPARAM wParam,LPARAM lParam){

    switch(msg){

        case WM_CLOSE:
            DestroyWindow(ventana);
            break;
        case WM_DESTROY:{
            PostQuitMessage(0); //este cero pasará al bucle para que acabe el programa
            break;
        }

         case WM_CTLCOLOREDIT:{
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0,0,0));
            SetBkColor(hdcStatic,RGB(202,240,251));
            return (INT_PTR)CreateSolidBrush(RGB(202,240,251));
            break;
         }
        case WM_COMMAND:
            switch(wParam){
                case BTN_ABRIR:
                    abrirArchivo(ventana);
                    break;
                case BTN_SALIR2:
                     PostQuitMessage(0);
                     break;

                case ID_PROGRAMADOR:
                //ShowWindow(GetForegroundWindow(),SW_HIDE);
                system("start https://github.com/FrapoDeveloper");
                break;
                case ID_INFO:
                MessageBox(ventana,"Este programa te servirá cuando quieras guardar algo importante, organizarte mejor, etc. ","Información del programa.",MB_DEFBUTTON1|MB_OK|MB_ICONINFORMATION);
                break;

                case ID_BTNSAVE:
                guardarArchivo(ventana);
                break;

            }



        default:
            return DefWindowProc(ventana, msg,wParam,lParam);
    }

}

void mostrarDatos(MYSQL *conn,char * consulta,MYSQL_ROW row,MYSQL_RES *res){
    if(mysql_query(conn,consulta)==0){
        res = mysql_use_result(conn);

        while((row=mysql_fetch_row(res))){
            printf("\n************************************");
            printf("\nId: %s\n",row[0]);
            printf("Nombre: %s\n",row[1]);
            printf("Clave: %s\n",row[2]);
        }
    }

}
int validarLogin(MYSQL *conn,char * consulta,MYSQL_ROW row,MYSQL_RES *res){
    if(mysql_query(conn,consulta)==0){
        res = mysql_use_result(conn);
        if(row=mysql_fetch_row(res)!=0){
           return 1;
        }else{
            return 0;
        }
    }
}

int WINAPI WinMain(HINSTANCE hIns,HINSTANCE hPrev,LPSTR lpCmdLinea,int nCmdShow){

    char consulta0[1024];
    conn = mysql_init(0);

    conn = mysql_real_connect(conn,"localhost","root","","easynotes",NULL,NULL,0);
      if(conn){
            system("color a");
        printf("Base de datos conectada.");
    }else{
        printf("Error de conexion");
    }
    ocultarConsola();
    Ventana_Inicio(hIns);
    Ventana_Login(ventana,hIns,nCmdShow);


    MSG mensaje;
    while(GetMessage(&mensaje,NULL,0,0)>0){
            TranslateMessage(&mensaje);
            DispatchMessage(&mensaje);
    }
    return mensaje.wParam;
}


void Ventana_Login(HWND ventana,HINSTANCE hIns,int nCmdShow ){
    WNDCLASSEX clase;
    clase.cbSize = sizeof(WNDCLASSEX);
    clase.cbWndExtra = 0;
    clase.cbClsExtra = 0;
    clase.style = CS_HREDRAW|CS_VREDRAW;
    clase.lpfnWndProc = procLogin;
    clase.hInstance = procLogin;
    clase.hIcon = LoadIcon(NULL,IDI_HAND);
    clase.hIconSm =LoadIcon(NULL,IDI_ASTERISK);
    clase.hCursor = LoadCursor(NULL,IDC_ARROW);
    clase.lpszClassName = "id_login";
    clase.lpszMenuName = NULL;
    clase.hbrBackground = CreateSolidBrush(RGB(105,214,244));
    if(!RegisterClassEx(&clase)){
        MessageBox( NULL,"No se pudo ejecutar la aplicacion","Error",MB_ICONERROR);
        return EXIT_FAILURE;
    }
    HBITMAP img_user = (HBITMAP)LoadImageW(NULL,L"programmer.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HBITMAP img_entrar = (HBITMAP)LoadImageW(NULL,L"boton.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HBITMAP img_salir = (HBITMAP)LoadImageW(NULL,L"btn_salir.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    ventana = CreateWindowEx(0,"id_login","EASY NOTES",WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU,350,0,600,650,HWND_DESKTOP,NULL,hIns,NULL);
    CreateWindowEx(NULL, TEXT("Static"), TEXT("Usuario: "), WS_CHILD | WS_VISIBLE , 170, 300, 155, 20,ventana,NULL,hIns, NULL);
    CreateWindowEx(0,"EDIT","",ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE,290,300,150,20,ventana,(HMENU)USER,hIns,NULL);
    CreateWindowEx(NULL, TEXT("Static"), TEXT("Contraseña: "), WS_CHILD | WS_VISIBLE , 170, 350, 155, 20,ventana,NULL,hIns, NULL);
    CreateWindowEx(0,"EDIT","",ES_AUTOHSCROLL|ES_LEFT|WS_CHILD|WS_VISIBLE|ES_PASSWORD,290,350,150,20,ventana,(HMENU)CLAVE,hIns,NULL);

    hiconUser = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP , 240,90,0,0,ventana,NULL,hIns,NULL);
    SendMessageW(hiconUser,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) img_user);
    hiconEntrar = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP |SS_NOTIFY , 240,400,0,0,ventana,NULL,hIns,NULL);
    SendMessageW(hiconEntrar,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) img_entrar);
    hiconSalir = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP |SS_NOTIFY, 520,540,0,0,ventana,(HMENU)BTN_SALIR,hIns,NULL);
    lbl_3 = CreateWindowEx(NULL, TEXT("Static"), TEXT("CREA UNA CUENTA AQUÍ"), WS_CHILD | WS_VISIBLE |SS_NOTIFY , 230,490, 170, 20,ventana,NULL,hIns, NULL);
    SendMessageW(hiconSalir,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) img_salir);
    //CreateWindow("STATIC",respuesta,ES_CENTER|WS_CHILD|WS_VISIBLE,450,123,80,25,ventana,(HMENU)USER_1,hIns,NULL); //unidades[itemIndice]
    if(ventana == NULL){
        MessageBox( NULL,"No se pudo crear la ventana","Error",MB_ICONERROR);
        return EXIT_FAILURE;
    }
    ShowWindow(ventana,nCmdShow);
    UpdateWindow(ventana);
}

void Ventana_Inicio(HINSTANCE hInst){
    WNDCLASSEX clase;
    clase.cbSize = sizeof(WNDCLASSEX);
    clase.cbWndExtra = 0;
    clase.cbClsExtra = 0;
    clase.style = CS_HREDRAW|CS_VREDRAW;
    clase.lpfnWndProc = procInicio;
    clase.hInstance = procInicio;
    clase.hIcon = LoadIcon(NULL,IDI_INFORMATION);
    clase.hIconSm =LoadIcon(NULL,IDI_INFORMATION);
    clase.hCursor = LoadCursor(NULL,IDC_ARROW);
    clase.lpszClassName = "ventanainicio";
    clase.lpszMenuName = NULL;
    clase.hbrBackground = CreateSolidBrush(RGB(105,214,244));
    RegisterClassEx(&clase);
}

void mostrarArchivo(char *path){

    FILE *file;
    file = fopen(path,"rb");
    fseek(file,0,SEEK_END);
    int _size = ftell(file);
    rewind(file);
    char *data [_size+1];
    fread(data,_size,1,file);
    data[_size] = '\0';
    SetWindowText(hedit,data);
    fclose(file);
}


void abrirArchivo(HWND hWnd){
    OPENFILENAME ofn;
    char nombre_archivo[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = nombre_archivo;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    GetOpenFileName(&ofn);
    mostrarArchivo(ofn.lpstrFile);

}

void guardarArchivo(HWND hWnd){
    OPENFILENAME ofn;
    char nombre_archivo[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = nombre_archivo;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = 100;
    ofn.lpstrFilter = "All files\0*.*\0Source Files\0*.CPP\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    GetSaveFileName(&ofn);
    saveArchivo(ofn.lpstrFile);

}

void saveArchivo(char *path){

    FILE *file;
    file = fopen(path,"w");
    int _size = GetWindowTextLength(hedit);
    char *data[_size+1];
    GetWindowText(hedit,data,_size+1);
    fwrite(data,_size+1,1,file);
    fclose(file);

}


void mostrarInicio(HWND ventana){
    HBITMAP img_user = (HBITMAP)LoadImageW(NULL,L"logoo.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HBITMAP img_abrir = (HBITMAP)LoadImageW(NULL,L"btn_abrir.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
    HWND inicio = CreateWindowEx(0,"ventanainicio","EASY NOTES",WS_OVERLAPPED|WS_MINIMIZEBOX|WS_SYSMENU|WS_VISIBLE,350,0,600,720,ventana,NULL,NULL,NULL);
    hiconLogo = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP , 10,0,0,0,inicio,NULL,NULL,NULL);
    SendMessageW(hiconLogo,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) img_user);
    hedit = CreateWindowEx(0,"EDIT","",ES_LEFT|WS_BORDER|WS_CHILD|WS_VISIBLE|ES_MULTILINE| WS_VSCROLL|WS_HSCROLL,10,200,575,400,inicio,NULL,NULL,NULL);
    CreateWindowEx(0,"Button","Guardar Archivo",WS_VISIBLE|WS_CHILD,435,620,120,40,inicio,(HMENU)ID_BTNSAVE,NULL,NULL);

    hiconSalir = CreateWindowW(L"Static",NULL,WS_VISIBLE |WS_CHILD | SS_BITMAP |SS_NOTIFY, 200,100,0,0,inicio,(HMENU)BTN_ABRIR,NULL,NULL);
    SendMessageW(hiconSalir,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM) img_abrir);


      AddMenus(inicio);
}

void AddMenus(HWND hWnd){

    hMenu = CreateMenu();
    HMENU menu_inicio = CreateMenu();
    HMENU menu_info = CreateMenu();
    HMENU menu_ayuda = CreateMenu();
    HMENU menu_acerca= CreateMenu();

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)menu_inicio,"Inicio");
            AppendMenu(menu_inicio,MF_STRING,BTN_ABRIR,"Abrir nota");
            AppendMenu(menu_inicio,MF_STRING,ID_BTNSAVE,"Guardar nota");
            AppendMenu(menu_inicio,MF_STRING,BTN_SALIR2,"Salir");

   /* AppendMenu(hMenu,MF_POPUP,(UINT_PTR)menu_info,"Ver");
            AppendMenu(menu_info,MF_STRING,AYUDA_PES,"Ultima nota");
            AppendMenu(menu_info,MF_STRING,AYUDA_PES,"Favoritos");*/

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)menu_ayuda,"Ayuda");
            AppendMenu(menu_ayuda,MF_STRING,ID_INFO,"Funcionamiento");
            AppendMenu(menu_ayuda,MF_POPUP,(UINT_PTR)menu_acerca,"Acerca de");
                    AppendMenu(menu_acerca,MF_STRING,ID_PROGRAMADOR,"Programador");
                    AppendMenu(menu_acerca,MF_STRING,AYUDA_PES,"Más aplicaciones");
    SetMenu(hWnd,hMenu);
}
