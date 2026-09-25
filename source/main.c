#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IN 48
#define HIST 8
#define ARCH "DSiIA_KNOWLEDGE.DAT"

typedef struct{const char*q,*r,*e,*c;}P;
Keyboard*kbd;
PrintConsole top,bot;
char hist[HIST][64];
int nh,pts,ok,done,sd;

/* ===== BANCO ===== */

P b[]={
{"Cuanto es 2+2?","4","2+2=4.","Matematicas"},
{"Cuanto es 5x6?","30","5x6=30.","Matematicas"},
{"Cuanto es 100/4?","25","100/4=25.","Matematicas"},
{"Cuanto es 15-8?","7","15-8=7.","Matematicas"},
{"Cuanto es 2 elevado a 5?","32","2^5=32.","Matematicas"},
{"Raiz cuadrada de 144?","12","12x12=144.","Matematicas"},
{"Que es una fraccion?","parte","Representa partes de un todo.","Matematicas"},
{"Que es una ecuacion?","igualdad","Es una igualdad con valores desconocidos.","Matematicas"},
{"Que es el porcentaje?","parte de cien","Indica una cantidad de cada 100.","Matematicas"},
{"Area de un cuadrado lado 5?","25","5x5=25.","Matematicas"},
{"Perimetro de un cuadrado lado 6?","24","4x6=24.","Matematicas"},
{"Area de un triangulo base 10 altura 4?","20","(10x4)/2=20.","Matematicas"},
{"Cuanto es 20% de 200?","40","0.2x200=40.","Matematicas"},
{"Cuanto es 3/4 de 20?","15","20x3/4=15.","Matematicas"},
{"Que es un numero primo?","divisible entre uno y si mismo","Tiene exactamente dos divisores positivos.","Matematicas"},
{"Que es una variable?","valor desconocido","Representa un valor que puede cambiar.","Matematicas"},
{"Que es una potencia?","multiplicacion","Multiplica una base varias veces.","Matematicas"},
{"Que es la media?","promedio","Se obtiene sumando y dividiendo entre la cantidad.","Matematicas"},
{"Que es el perimetro?","suma de lados","Es la longitud alrededor de una figura.","Matematicas"},
{"Que es el area?","superficie","Mide la superficie de una figura.","Matematicas"},

{"Que es la quimica?","materia","Estudia la materia, sus propiedades y transformaciones.","Quimica"},
{"Que es un atomo?","particula","Es la unidad basica de un elemento.","Quimica"},
{"Que es una molecula?","union de atomos","Es un conjunto de dos o mas atomos unidos.","Quimica"},
{"Que es un elemento quimico?","sustancia","Sustancia formada por atomos del mismo tipo.","Quimica"},
{"Que es un compuesto?","union de elementos","Sustancia formada por distintos elementos unidos quimicamente.","Quimica"},
{"Formula del agua?","h2o","Tiene dos hidrogenos y un oxigeno.","Quimica"},
{"Formula del dioxido de carbono?","co2","Tiene un carbono y dos oxigenos.","Quimica"},
{"Formula de la sal?","nacl","Cloruro de sodio.","Quimica"},
{"Que es el oxigeno?","elemento","Elemento quimico de simbolo O.","Quimica"},
{"Que es el hidrogeno?","elemento","Elemento quimico de simbolo H.","Quimica"},
{"Que es el carbono?","elemento","Elemento quimico de simbolo C.","Quimica"},
{"Que es el pH?","acidez","Indica acidez o basicidad de una sustancia.","Quimica"},
{"Que es un acido?","sustancia acida","Una sustancia con pH menor que 7.","Quimica"},
{"Que es una base?","sustancia basica","Una sustancia con pH mayor que 7.","Quimica"},
{"Que particula tiene carga positiva?","proton","El proton tiene carga positiva.","Quimica"},
{"Que particula tiene carga negativa?","electron","El electron tiene carga negativa.","Quimica"},
{"Que particula no tiene carga?","neutron","El neutron no tiene carga electrica.","Quimica"},
{"Que es un enlace ionico?","transferencia","Implica transferencia de electrones.","Quimica"},
{"Que es un enlace covalente?","compartir","Los atomos comparten electrones.","Quimica"},
{"Simbolo del hierro?","fe","Fe.","Quimica"},
{"Simbolo del oro?","au","Au.","Quimica"},
{"Simbolo de la plata?","ag","Ag.","Quimica"},
{"Simbolo del sodio?","na","Na.","Quimica"},
{"Simbolo del potasio?","k","K.","Quimica"},
{"Que es una reaccion quimica?","transformacion","Transforma unas sustancias en otras.","Quimica"},
{"Que es la tabla periodica?","elementos","Organiza los elementos quimicos.","Quimica"},
{"Que es una mezcla?","sustancias","Combinacion fisica de sustancias.","Quimica"},
{"Que es la materia?","todo lo que tiene masa","La materia tiene masa y ocupa espacio.","Quimica"},
{"Que es la combustion?","reaccion","Reaccion rapida con oxigeno que libera energia.","Quimica"},
{"Que es la evaporacion?","cambio de estado","Paso de liquido a gas.","Quimica"},

{"Que es un verbo?","accion","Expresa acciones, estados o procesos.","Espanol"},
{"Que es un sustantivo?","nombre","Nombra personas, animales, cosas o ideas.","Espanol"},
{"Que es un adjetivo?","cualidad","Describe al sustantivo.","Espanol"},
{"Capital de Francia?","paris","Paris.","General"},
{"Capital de Japon?","tokio","Tokio.","General"},
{"Cuantos planetas hay?","8","Hay ocho planetas.","General"}
};

int total=sizeof(b)/sizeof(b[0]);

void low(char*s){while(*s){*s=tolower((unsigned char)*s);s++;}}

void normal(char*s){
 char*t=s,*d=s;
 while(*t){
  if(!strchr("¿?!¡.,;:",*t)&&*t!='\''&&*t!='\"')*d++=*t;
  t++;
 }
 *d=0;low(s);
}

void topc(){consoleSelect(&top);}
void botc(){consoleSelect(&bot);}

void addh(const char*a,const char*c){
 if(nh>=HIST){for(int i=0;i<HIST-1;i++)strcpy(hist[i],hist[i+1]);nh=HIST-1;}
 snprintf(hist[nh++],64,"%s: %s",a,c);
}

void showh(){
 topc();
 printf("\x1b[2J====== DSi IA ======\n\n");
 for(int i=0;i<nh;i++)printf("%s\n\n",hist[i]);
}

void key(){
 botc();
 printf("\x1b[2J");
 kbd=keyboardInit(NULL,3,BgType_Text4bpp,BgSize_T_256x512,20,0,false,true);
 keyboardShow();
}

void hidekey(){if(kbd)keyboardHide();}

int readt(char*s,int max){
 int p=0;
 botc();
 printf("Tu: ");
 while(1){
  swiWaitForVBlank();scanKeys();
  int k=keyboardUpdate();
  if(k>0){
   if(k==DVK_ENTER||k=='\n'){s[p]=0;return 1;}
   if((k==DVK_BACKSPACE||k==8)&&p){s[--p]=0;printf("\b \b");}
   else if(k>=32&&k<127&&p<max-1){s[p++]=k;s[p]=0;printf("%c",k);}
  }
  if(keysDown()&KEY_A){s[p]=0;return 1;}
  if(keysDown()&KEY_B){s[0]=0;return 0;}
 }
}

void waitA(){
 while(1){swiWaitForVBlank();scanKeys();if(keysDown()&KEY_A)return;}
}

/* ===== APRENDIZAJE ILIMITADO ===== */

void saveknow(const char*q,const char*r){
 FILE*f;
 unsigned short a=strlen(q),z=strlen(r);
 if(!sd)return;
 if(a>63)a=63;if(z>127)z=127;
 f=fopen(ARCH,"ab");
 if(!f)return;
 fwrite(&a,2,1,f);fwrite(&z,2,1,f);
 fwrite(q,1,a,f);fwrite(r,1,z,f);
 fclose(f);
}

int findknow(const char*msg,char*r){
 FILE*f;
 unsigned short a,z;
 char q[64],x[128],m[64];
 int found=0;

 if(!sd)return 0;
 strncpy(m,msg,63);m[63]=0;normal(m);
 f=fopen(ARCH,"rb");
 if(!f)return 0;

 while(fread(&a,2,1,f)==1&&fread(&z,2,1,f)==1){
  if(a>63||z>127)break;
  if(fread(q,1,a,f)!=a||fread(x,1,z,f)!=z)break;
  q[a]=0;x[z]=0;normal(q);

  if(!strcmp(m,q)||strstr(m,q)||strstr(q,m)){
   strcpy(r,x);found=1;
  }
 }
 fclose(f);
 return found;
}

void learn(){
 char q[64],r[128];
 botc();
 printf("\x1b[2J=== APRENDER ===\n\nPregunta:\n\n");
 if(!readt(q,64)||!q[0])return;
 printf("\n\nRespuesta:\n\n");
 if(!readt(r,128)||!r[0])return;
 saveknow(q,r);
 botc();
 printf("\x1b[2JAPRENDIDO CON EXITO!\n\n");
 printf("Se agrego a la biblioteca.\n");
 printf("Guardado automaticamente.\n\n");
 printf("Pulsa A...");
 waitA();
}

/* ===== CALCULADORA ===== */

int calc(const char*e,int*r){
 char b[64],l[64],a[32]={0},c[32]={0},op=0;
 int p=-1,j=0;
 strncpy(b,e,63);b[63]=0;normal(b);

 for(int i=0;b[i]&&j<63;i++)if(b[i]!=' ')l[j++]=b[i];
 l[j]=0;

 for(int i=1;l[i];i++)
  if(strchr("+-*x/^",l[i])){
   op=l[i]=='x'?'*':l[i];p=i;break;
  }

 if(p<0)return 0;
 strncpy(a,l,p);strcpy(c,l+p+1);
 int x=atoi(a),y=atoi(c);

 switch(op){
  case '+':*r=x+y;break;
  case '-':*r=x-y;break;
  case '*':*r=x*y;break;
  case '/':if(!y)return 0;*r=x/y;break;
  case '^':*r=1;for(int i=0;i<y;i++)*r*=x;break;
  default:return 0;
 }
 return 1;
}

/* ===== RESPUESTA ===== */

void answer(const char*msg){
 char m[IN],r[128];
 int n;

 if(findknow(msg,r)){
  addh("DSi IA",r);showh();topc();printf("\n%s\n",r);return;
 }

 strncpy(m,msg,IN-1);m[IN-1]=0;normal(m);
 strcpy(r,"No lo se todavia. Puedes ensenarme escribiendo APRENDER.");

 if(strstr(m,"cuanto es")||strstr(m,"calcula")||
    strchr(m,'+')||strchr(m,'-')||strchr(m,'*')||
    strchr(m,'x')||strchr(m,'/')||strchr(m,'^')){
  if(calc(m,&n))snprintf(r,128,"El resultado es: %d",n);
 }
 else if(strstr(m,"quimica")||strstr(m,"que estudia la quimica"))
  strcpy(r,"La Quimica estudia la materia, sus propiedades y transformaciones.");
 else if(strstr(m,"atomo"))
  strcpy(r,"Un atomo tiene protones, neutrones y electrones.");
 else if(strstr(m,"molecula"))
  strcpy(r,"Una molecula esta formada por dos o mas atomos unidos.");
 else if(strstr(m,"ph"))
  strcpy(r,"El pH indica la acidez o basicidad. 7 es neutro.");
 else if(strstr(m,"verbo"))
  strcpy(r,"Un verbo expresa acciones, estados o procesos.");
 else if(strstr(m,"sustantivo"))
  strcpy(r,"Un sustantivo nombra personas, animales, lugares, cosas o ideas.");
 else if(strstr(m,"hola")||strstr(m,"buenas")||strstr(m,"buenos"))
  strcpy(r,"Hola! Soy DSi IA. Preguntame lo que quieras.");
 else if(strstr(m,"quien eres"))
  strcpy(r,"Soy DSi IA, tu asistente escolar para DSi XL.");
 else if(strstr(m,"gracias"))
  strcpy(r,"De nada! Sigue aprendiendo.");

 addh("DSi IA",r);
 showh();
 topc();
 printf("\n%s\n",r);
}

/* ===== CHAT ===== */

void chat(){
 char m[IN],x[IN];
 nh=0;
 key();

 addh("DSi IA","Hola! Escribe tu pregunta.");
 addh("DSi IA","Si no se algo, escribe APRENDER.");

 while(1){
  showh();
  botc();
  printf("\n----------------\n");

  if(!readt(m,IN)||!m[0])continue;

  strncpy(x,m,IN-1);x[IN-1]=0;normal(x);
  if(strstr(x,"salir")||strstr(x,"menu"))break;

  addh("Tu",m);

  if(strstr(x,"aprender")){
   learn();
   continue;
  }

  answer(m);
  botc();
  printf("\nPulsa A...");
  waitA();
 }

 hidekey();
}

/* ===== EXAMEN ===== */

int right(const char*u,const char*c){
 char a[IN],b[IN];
 strncpy(a,u,IN-1);strncpy(b,c,IN-1);
 a[IN-1]=b[IN-1]=0;normal(a);normal(b);
 return !strcmp(a,b)||strstr(a,b);
}

void play(const char*cat,int lesson){
 int ix[60],n=0;
 for(int i=0;i<total;i++)if(!cat||!strcmp(b[i].c,cat))ix[n++]=i;
 if(!n)return;

 for(int i=n-1;i;i--){
  int j=rand()%(i+1),t=ix[i];ix[i]=ix[j];ix[j]=t;
 }

 int max=n<7?n:7;
 key();

 for(int q=0;q<max;q++){
  P*p=&b[ix[q]];
  char r[IN];

  topc();
  printf("\x1b[2J%s\nPregunta %d/%d\n\n%s\n\n",
         p->c,q+1,max,p->q);

  readt(r,IN);
  done++;

  if(r[0]&&right(r,p->r)){
   ok++;pts+=10;
   topc();printf("\n*** CORRECTO! ***\n");
   if(lesson)printf("\n%s\n",p->e);
  }else{
   topc();
   printf("\nIncorrecto.\nRespuesta: %s\n%s\n",p->r,p->e);
  }

  topc();printf("\nPulsa A...");
  waitA();
 }

 hidekey();
}

/* ===== MENU ===== */

void menu(){
 hidekey();
 topc();
 printf("\x1b[2J==============================\n");
 printf("           DSi IA\n");
 printf("      Asistente escolar\n");
 printf("==============================\n\n");
 printf("A - Examen\nB - Leccion\n");
 printf("X - Matematicas\nY - Espanol\n");
 printf("L - Quimica\nR - General\n");
 printf("SELECT - Chat / Aprender\n");
 printf("START - Salir\n\n");
 printf("Puntos: %d | Aciertos: %d/%d\n",pts,ok,done);
}

int main(void){
 videoSetMode(MODE_0_2D);
 videoSetModeSub(MODE_0_2D);
 vramSetBankA(VRAM_A_MAIN_BG);
 vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(&top,0,BgType_Text4bpp,BgSize_T_256x256,22,3,true,true);
 consoleInit(&bot,0,BgType_Text4bpp,BgSize_T_256x256,22,3,false,true);

 srand(2026);
 sd=fatInitDefault();

 while(1){
  menu();

  while(1){
   swiWaitForVBlank();
   scanKeys();
   u16 k=keysDown();

   if(k&KEY_A){play(NULL,0);break;}
   if(k&KEY_B){play(NULL,1);break;}
   if(k&KEY_X){play("Matematicas",1);break;}
   if(k&KEY_Y){play("Espanol",1);break;}
   if(k&KEY_L){play("Quimica",1);break;}
   if(k&KEY_R){play("General",1);break;}
   if(k&KEY_SELECT){chat();break;}

   if(k&KEY_START){
    topc();
    printf("\x1b[2JHasta luego!\nGracias por usar DSi IA.");
    while(1)swiWaitForVBlank();
   }
  }
 }
 return 0;
}
