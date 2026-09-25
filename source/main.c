#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

PrintConsole top,bottom;
Keyboard *kbd;

void T(){consoleSelect(&top);}
void B(){consoleSelect(&bottom);}
void col(int c){consoleEnhancedColorHandler(c);}

typedef struct{
 const char*q,*r,*e,*c;
} P;

P p[]={
{"Cuanto es 7 + 8?","15","Siete mas ocho son quince.","Matematicas"},
{"Cuanto es 9 x 6?","54","Nueve por seis son cincuenta y cuatro.","Matematicas"},
{"Cuanto es 144 / 12?","12","144 dividido entre 12 da 12.","Matematicas"},
{"Cuanto es 15 - 8?","7","Quince menos ocho son siete.","Matematicas"},
{"Cuanto es 5 al cuadrado?","25","Cinco por cinco son veinticinco.","Matematicas"},
{"Raiz cuadrada de 81?","9","La raiz de 81 es nueve.","Matematicas"},
{"Cuanto es 2 elevado a 5?","32","2 por 2 por 2 por 2 por 2 son 32.","Matematicas"},
{"Cuanto es 13 + 29?","42","Trece mas veintinueve son cuarenta y dos.","Matematicas"},
{"Cuanto es 10 x 10?","100","Diez por diez son cien.","Matematicas"},
{"Cuanto es 50 - 23?","27","Cincuenta menos veintitres son veintisiete.","Matematicas"},
{"Cuanto es 3 al cubo?","27","Tres al cubo es veintisiete.","Matematicas"},

{"Simbolo del oxigeno?","O","El oxigeno usa el simbolo O.","Quimica"},
{"Simbolo del hidrogeno?","H","El hidrogeno usa H.","Quimica"},
{"Simbolo del carbono?","C","El carbono usa C.","Quimica"},
{"Que elemento es Fe?","Hierro","Fe procede de ferrum.","Quimica"},
{"Que elemento es Na?","Sodio","Na corresponde al sodio.","Quimica"},
{"Que elemento es Au?","Oro","Au procede de aurum.","Quimica"},
{"Que elemento es Ag?","Plata","Ag procede de argentum.","Quimica"},
{"Particula con carga negativa?","Electron","El electron tiene carga negativa.","Quimica"},
{"Particula con carga positiva?","Proton","El proton tiene carga positiva.","Quimica"},
{"Particula sin carga?","Neutron","El neutron no tiene carga.","Quimica"},
{"Formula del agua?","H2O","Dos H y un O forman H2O.","Quimica"},
{"Formula del dioxido de carbono?","CO2","Un carbono y dos oxigenos forman CO2.","Quimica"},

{"Que es un sustantivo?","Nombre","Nombra personas, animales, cosas o ideas.","Espanol"},
{"Que es un verbo?","Accion","Expresa accion, estado o proceso.","Espanol"},
{"Antonimo de grande?","Pequeno","Pequeno es contrario de grande.","Espanol"},
{"Sinonimo de rapido?","Veloz","Rapido y veloz tienen significado parecido.","Espanol"},
{"Signo que abre una pregunta?","¿","En espanol se usa ¿ al comenzar.","Espanol"},
{"Signo que abre una exclamacion?","¡","Las exclamaciones empiezan con ¡.","Espanol"},
{"Plural de lapiz?","Lapices","La z cambia por c.","Espanol"},
{"Sujeto en 'Ana corre'?","Ana","Ana realiza la accion.","Espanol"},
{"Que es un adjetivo?","Descripcion","Describe o califica al sustantivo.","Espanol"},
{"Donde tiene fuerza una palabra aguda?","Ultima","La silaba tonica es la ultima.","Espanol"},
{"Donde tiene fuerza una palabra llana?","Penultima","La silaba tonica es la penultima.","Espanol"},
{"Donde tiene fuerza una esdrujula?","Antepenultima","La silaba tonica es la antepenultima.","Espanol"}
};

#define NP (sizeof(p)/sizeof(p[0]))

char hist[7][70];
int nh=0;

void add(const char*s){
 if(nh<7){strncpy(hist[nh],s,69);hist[nh][69]=0;nh++;}
 else{int i;for(i=0;i<6;i++)strcpy(hist[i],hist[i+1]);strcpy(hist[6],s);}
}

void historia(){
 int i;T();consoleClear();
 col(6);printf("================================\n  AULA DE GERMAN\n================================\n");col(5);
 for(i=0;i<nh;i++)printf("%s\n",hist[i]);
}

int leer(char*s,int n){
 int x=0;keyboardShow();s[0]=0;
 while(1){
  swiWaitForVBlank();scanKeys();int k=keyboardUpdate();
  if(k==DVK_ENTER||k=='\n'){s[x]=0;return 1;}
  if((k==DVK_BACKSPACE||k==8)&&x){s[--x]=0;}
  else if(k>=32&&k<127&&x<n-1)s[x++]=k,s[x]=0;
  if(keysDown()&KEY_B)return 0;
 }
}

void germ_responde(char*s){
 char*r;
 if(strstr(s,"hola")||strstr(s,"Hola"))
  r="¡Buenas! Soy German. Venga, dime que estudiamos hoy.";
 else if(strstr(s,"mate")||strstr(s,"Mate"))
  r="Vale, vamos con matematicas. Ojo con los signos y sin miedo.";
 else if(strstr(s,"quim")||strstr(s,"Quim"))
  r="Perfecto. Abrimos quimica: atomos, elementos y formulas.";
 else if(strstr(s,"español")||strstr(s,"espanol")||strstr(s,"lengua"))
  r="Muy bien. Vamos con lengua, gramatica y ortografia.";
 else if(strstr(s,"gracias")||strstr(s,"Gracias"))
  r="De nada, hombre. Para eso estoy. ¡Venga, seguimos!";
 else if(strstr(s,"quien")||strstr(s,"Quien"))
  r="Soy German, tu profesor virtual. Un profesor español, claro.";
 else if(strstr(s,"2+2")||strstr(s,"2 + 2"))
  r="Eso es facil: 2 + 2 = 4. No me pongas a prueba tan pronto.";
 else if(strstr(s,"pi")||strstr(s,"PI"))
  r="Pi vale aproximadamente 3,141592. Aparece mucho en geometria.";
 else
  r="Hmm... buena pregunta. No pasa nada: podemos estudiarla juntos.";
 add("German:");
 add(r);
}

void charla(){
 char s[80];nh=0;
 add("German:");
 add("¡Buenas! Soy el profesor German.");
 add("Venga, dime que quieres estudiar.");
 while(1){
  historia();B();consoleClear();
  col(3);printf("HABLA CON GERMAN\n\n");col(5);
  printf("Escribe aqui:\n> ");
  if(!leer(s,80))return;
  if(!s[0])continue;
  if(!strcasecmp(s,"salir"))return;
  add("Tu:");add(s);germ_responde(s);
 }
}

void examen(char*cat){
 int a[50],n=0,i,j,sc=0,lim;char s[60];
 for(i=0;i<NP;i++)
  if(!cat||!strcmp(p[i].c,cat))a[n++]=i;
 for(i=n-1;i>0;i--){j=rand()%(i+1);int z=a[i];a[i]=a[j];a[j]=z;}
 lim=n<7?n:7;
 for(i=0;i<lim;i++){
  T();consoleClear();col(6);
  printf("================================\n PROFESOR GERMAN\n================================\n\n");
  col(5);printf("%s\n\n%s\n",p[a[i]].c,p[a[i]].q);
  B();consoleClear();printf("Pregunta %d/%d\n\nRespuesta:\n> ",i+1,lim);
  if(!leer(s,60))return;
  T();
  if(!strcasecmp(s,p[a[i]].r)){sc++;col(2);printf("\n\n¡MUY BIEN!\n");}
  else{col(1);printf("\n\nCasi, hombre.\n");col(5);printf("Respuesta: %s\n",p[a[i]].r);}
  col(5);printf("%s\n",p[a[i]].e);
  B();printf("\nPulsa A para continuar.");
  while(1){swiWaitForVBlank();scanKeys();if(keysDown()&KEY_A)break;if(keysDown()&KEY_B)return;}
 }
 T();consoleClear();col(6);printf("========== RESULTADO ==========\n\n");col(5);
 printf("%d de %d correctas.\n\n",sc,lim);
 if(sc==lim){col(2);printf("¡Perfecto! Menudo nivel.\n");}
 else if(sc>=lim/2){col(3);printf("Muy bien. Sigue practicando.\n");}
 else{col(1);printf("No pasa nada. Repasamos.\n");}
 col(5);B();printf("\nPulsa A.");
 while(1){swiWaitForVBlank();scanKeys();if(keysDown()&KEY_A)break;}
}

double calc(char*s){
 double a,b;
 char o;
 if(sscanf(s,"%lf %c %lf",&a,&o,&b)==3){
  if(o=='+')return a+b;if(o=='-')return a-b;
  if(o=='*'||o=='x'||o=='X')return a*b;
  if(o=='/'&&b!=0)return a/b;
  if(o=='^')return pow(a,b);
 }
 if(sscanf(s,"sqrt %lf",&a)==1)return sqrt(a);
 if(sscanf(s,"sin %lf",&a)==1)return sin(a);
 if(sscanf(s,"cos %lf",&a)==1)return cos(a);
 if(sscanf(s,"tan %lf",&a)==1)return tan(a);
 if(sscanf(s,"log %lf",&a)==1)return log10(a);
 if(sscanf(s,"ln %lf",&a)==1)return log(a);
 return sscanf(s,"%lf",&a)==1?a:0;
}

void calculadora(){
 char s[70];double r;
 while(1){
  T();consoleClear();col(6);
  printf("================================\n CALCULADORA CIENTIFICA\n================================\n\n");
  col(5);printf("+ - x / ^\n");printf("sqrt sin cos tan\nlog ln pi x^2\n\n");
  printf("Ejemplo: 12 + 8\n> ");
  if(!leer(s,70))return;
  if(!s[0])continue;
  r=calc(s);printf("\n");
  col(2);printf("RESULTADO: %.6f\n",r);col(5);
  B();printf("\nA = otra cuenta\nB = volver");
  while(1){swiWaitForVBlank();scanKeys();if(keysDown()&KEY_A)break;if(keysDown()&KEY_B)return;}
 }
}

void reloj(){
 while(1){
  time_t t=time(NULL);struct tm*d=localtime(&t);
  T();consoleClear();col(6);
  printf("================================\n          RELOJ\n================================\n\n");col(5);
  if(d)printf("%02d:%02d:%02d\n\n%02d/%02d/%04d\n",d->tm_hour,d->tm_min,d->tm_sec,d->tm_mday,d->tm_mon+1,d->tm_year+1900);
  printf("\nPulsa B para volver.");
  B();consoleClear();printf("RELOJ DE GERMAN\n\n");printf("Hora y fecha del sistema.");
  scanKeys();if(keysDown()&KEY_B)return;
  swiWaitForVBlank();
 }
}

void aula(){
 while(1){
  T();consoleClear();col(6);
  printf("================================\n          AULA 3-B\n================================\n\n");
  col(5);
  printf("       +----------------+\n");
  printf("       |    PIZARRON     |\n");
  printf("       | x + y = 20      |\n");
  printf("       | H2O = AGUA      |\n");
  printf("       +----------------+\n\n");
  printf("          .-''''-.\n");
  printf("         /  o  o  \\\n");
  printf("        |    ^     |\n");
  printf("        |  \\___/   |\n");
  printf("         \\_________/\n");
  printf("          PROF. GERMAN\n\n");
  col(3);printf("\"Venga, vamos al lio.\"\n");col(5);
  B();consoleClear();printf("A Todas\nX Matematicas\nY Espanol\nL Quimica\nSELECT Hablar\nB Volver");
  while(1){
   swiWaitForVBlank();scanKeys();uint32_t k=keysDown();
   if(k&KEY_A){examen(NULL);break;}
   if(k&KEY_X){examen("Matematicas");break;}
   if(k&KEY_Y){examen("Espanol");break;}
   if(k&KEY_L){examen("Quimica");break;}
   if(k&KEY_SELECT){charla();break;}
   if(k&KEY_B)return;
  }
 }
}

void menu(){
 while(1){
  T();consoleClear();col(6);
  printf("================================\n      PROFESOR GERMAN\n================================\n\n");
  col(5);printf("       ¡VENGA, AL LIO!\n\n");
  printf("       [ AULA 3-B ]\n\n");
  B();consoleClear();col(3);
  printf("========= MENU =========\n\n");col(5);
  printf("TOCA LA PANTALLA\n\n");
  printf("A  AULA DE GERMAN\n");
  printf("X  CALCULADORA\n");
  printf("Y  RELOJ\n");
  printf("START  SALIR\n");
  while(1){
   swiWaitForVBlank();scanKeys();uint32_t k=keysDown();
   if(k&KEY_A){aula();break;}
   if(k&KEY_X){calculadora();break;}
   if(k&KEY_Y){reloj();break;}
   if(k&KEY_START)return;
   if(k&KEY_TOUCH){
    touchPosition t;touchRead(&t);
    if(t.py<70){reloj();break;}
    if(t.py<140){calculadora();break;}
    aula();break;
   }
  }
 }
}

int main(int argc,char**argv){
 videoSetMode(MODE_0_2D);videoSetModeSub(MODE_0_2D);
 vramSetBankA(VRAM_A_MAIN_BG);vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(&top,0,BgType_Text4bpp,BgSize_T_256x256,22,3,true,true);
 consoleInit(&bottom,0,BgType_Text4bpp,BgSize_T_256x256,22,3,false,true);

 kbd=keyboardInit(NULL,3,BgType_Text4bpp,BgSize_T_256x512,20,0,false,true);
 keyboardShow();
 srand((unsigned)time(NULL));
 menu();
 keyboardHide();
 return 0;
}
