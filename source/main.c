#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IN 48
#define HIST 8
#define ARCH "DSiIA_KNOWLEDGE.DAT"

typedef struct{const char*q,*r,*e,*c;}Pregunta;
Keyboard*kbd=NULL;
char hist[HIST][64];
int nh=0,puntos=0,aciertos=0,hechas=0,sd=0;

Pregunta banco[]={
{"Cuanto es (-5) x 6?","-30","Negativo por positivo = negativo.","Matematicas"},
{"Cuanto es 3/4 + 1/4?","1","3/4 + 1/4 = 1.","Matematicas"},
{"Resuelve: 3x - 7 = 8","5","3x = 15, x = 5.","Matematicas"},
{"Area de triangulo base 12 altura 5?","30","(12x5)/2 = 30.","Matematicas"},
{"Perimetro de cuadrado lado 9?","36","4x9 = 36.","Matematicas"},
{"Cuanto es 20% de 150?","30","0.20x150 = 30.","Matematicas"},
{"Cuanto es 2 elevado a 6?","64","2^6 = 64.","Matematicas"},
{"Raiz cuadrada de 121?","11","11x11 = 121.","Matematicas"},
{"Simplifica 16/24","2/3","Dividir entre 8.","Matematicas"},
{"Cuanto es 3/5 de 50?","30","3/5x50 = 30.","Matematicas"},

{"Que es un verbo?","accion","Expresa acciones, estados o procesos.","Espanol"},
{"Que es un sustantivo?","nombre","Nombra personas, animales, cosas o ideas.","Espanol"},
{"Que es un adjetivo?","cualidad","Describe o califica al sustantivo.","Espanol"},
{"Que es un adverbio?","modifica","Modifica al verbo, adjetivo u otro adverbio.","Espanol"},
{"Que es el sujeto?","quien","Quien realiza la accion.","Espanol"},
{"Que es el predicado?","verbo","Lo que se dice del sujeto.","Espanol"},
{"Que es una metafora?","figura","Relaciona una cosa con otra sin usar como.","Espanol"},
{"Que es un diptongo?","vocales","Dos vocales en una silaba.","Espanol"},
{"Que es un hiato?","separacion","Dos vocales en silabas distintas.","Espanol"},
{"Participio de escribir?","escrito","Escribir -> escrito.","Espanol"},

{"Que es la quimica?","materia","Estudia la materia y sus transformaciones.","Quimica"},
{"Que es un atomo?","particula","Unidad basica de un elemento.","Quimica"},
{"Que es una molecula?","union","Union de dos o mas atomos.","Quimica"},
{"Formula del agua?","h2o","Dos hidrogenos y un oxigeno.","Quimica"},
{"Formula del dioxido de carbono?","co2","Un carbono y dos oxigenos.","Quimica"},
{"Que es el pH?","acidez","Mide acidez o basicidad.","Quimica"},
{"Particula con carga positiva?","proton","El proton tiene carga positiva.","Quimica"},
{"Particula con carga negativa?","electron","El electron tiene carga negativa.","Quimica"},
{"Particula sin carga?","neutron","El neutron es neutro.","Quimica"},
{"Simbolo del hierro?","fe","Fe viene de ferrum.","Quimica"},

{"Capital de Francia?","paris","Paris.","General"},
{"Capital de Japon?","tokio","Tokio.","General"},
{"Quien escribio Don Quijote?","cervantes","Miguel de Cervantes.","General"},
{"Cuantos planetas hay?","8","Hay ocho planetas.","General"},
{"Oceano mas grande?","pacifico","Oceano Pacifico.","General"},
{"Quien pinto la Mona Lisa?","leonardo","Leonardo da Vinci.","General"},
{"En que ano llego el hombre a la Luna?","1969","Fue en 1969.","General"},
{"Metal liquido a temperatura ambiente?","mercurio","Mercurio.","General"}
};

int total=sizeof(banco)/sizeof(banco[0]);

void minus(char*s){
 while(*s){*s=tolower((unsigned char)*s);s++;}
}

int tiene(const char*a,const char*b){
 return strstr(a,b)!=NULL;
}

void histadd(const char*a,const char*b){
 if(nh>=HIST){
  for(int i=0;i<HIST-1;i++)strcpy(hist[i],hist[i+1]);
  nh=HIST-1;
 }
 snprintf(hist[nh++],64,"%s: %s",a,b);
}

void histshow(){
 printf("\x1b[2J====== DSi IA - Chat ======\n\n");
 for(int i=0;i<nh;i++)printf("%s\n\n",hist[i]);
}

void teclado(){
 kbd=keyboardInit(NULL,3,BgType_Text4bpp,
                  BgSize_T_256x512,20,0,false,true);
 keyboardShow();
}

void ocultar(){
 if(kbd)keyboardHide();
}

int leer(char*b,int max){
 int p=0;
 b[0]=0;
 printf("Tu: ");

 while(1){
  swiWaitForVBlank();
  scanKeys();
  int k=keyboardUpdate();

  if(k>0){
   if(k==DVK_ENTER||k=='\n'){
    b[p]=0;
    return 1;
   }

   if((k==DVK_BACKSPACE||k==8)&&p){
    b[--p]=0;
    printf("\b \b");
   }
   else if(k>=32&&k<127&&p<max-1){
    b[p++]=(char)k;
    b[p]=0;
    printf("%c",k);
   }
  }

  if(keysDown()&KEY_A){
   b[p]=0;
   return 1;
  }

  if(keysDown()&KEY_B){
   b[0]=0;
   return 0;
  }
 }
}

void espera(){
 while(1){
  swiWaitForVBlank();
  scanKeys();
  if(keysDown()&KEY_A)return;
 }
}

/* ===== APRENDIZAJE ILIMITADO =====
   Formato:
   [2 bytes pregunta][2 bytes respuesta]
   [pregunta][respuesta]
*/

void guardar(const char*q,const char*r){
 FILE*f;
 unsigned short a=strlen(q),b=strlen(r);

 if(!sd)return;
 if(a>63)a=63;
 if(b>127)b=127;

 f=fopen(ARCH,"ab");
 if(!f)return;

 fwrite(&a,2,1,f);
 fwrite(&b,2,1,f);
 fwrite(q,1,a,f);
 fwrite(r,1,b,f);
 fclose(f);
}

int buscar(const char*preg,char*resp){
 FILE*f;
 unsigned short a,b;
 char q[64],r[128],bus[64];
 int encontrado=0;

 if(!sd)return 0;

 strncpy(bus,preg,63);
 bus[63]=0;
 minus(bus);

 f=fopen(ARCH,"rb");
 if(!f)return 0;

 while(fread(&a,2,1,f)==1&&fread(&b,2,1,f)==1){

  if(a>63||b>127)break;

  if(fread(q,1,a,f)!=a)break;
  if(fread(r,1,b,f)!=b)break;

  q[a]=0;
  r[b]=0;

  {
   char x[64];
   strncpy(x,q,63);
   x[63]=0;
   minus(x);

   if(!strcmp(bus,x)||tiene(bus,x)||tiene(x,bus)){
    strcpy(resp,r);
    encontrado=1;
   }
  }
 }

 fclose(f);
 return encontrado;
}

int aprender_detectado(const char*s){
 char x[IN];
 strncpy(x,s,IN-1);
 x[IN-1]=0;
 minus(x);
 return tiene(x,"aprender");
}

void aprender(){
 char q[64],r[128];

 printf("\x1b[2J");
 printf("======= MODO APRENDER =======\n\n");
 printf("Escribe la pregunta:\n\n");

 if(!leer(q,64)||!q[0])return;

 printf("\n\nEscribe la respuesta:\n\n");

 if(!leer(r,128)||!r[0])return;

 guardar(q,r);

 printf("\x1b[2J");
 printf("===== APRENDIDO CON EXITO =====\n\n");
 printf("Se agrego a mi biblioteca.\n");
 printf("Guardado automaticamente.\n\n");
 printf("La biblioteca puede crecer\n");
 printf("hasta llenar el espacio disponible.\n\n");
 printf("Pulsa A...");
 espera();
}

/* ===== CALCULADORA ===== */

int calcular(const char*e,int*res){
 char b[64],l[64],a[32]={0},c[32]={0};
 char op=0;
 int p=-1,j=0;

 strncpy(b,e,63);
 b[63]=0;
 minus(b);

 for(int i=0;b[i]&&j<63;i++)
  if(b[i]!=' ')l[j++]=b[i];
 l[j]=0;

 for(int i=1;l[i];i++)
  if(l[i]=='+'||l[i]=='-'||l[i]=='*'||
     l[i]=='x'||l[i]=='/'||l[i]=='^'){
   op=l[i]=='x'?'*':l[i];
   p=i;
   break;
  }

 if(p<0)return 0;

 strncpy(a,l,p);
 strcpy(c,l+p+1);

 int x=atoi(a),y=atoi(c);

 switch(op){
  case '+':*res=x+y;break;
  case '-':*res=x-y;break;
  case '*':*res=x*y;break;
  case '/':if(!y)return 0;*res=x/y;break;
  case '^':
   *res=1;
   for(int i=0;i<y;i++)*res*=x;
   break;
  default:return 0;
 }

 return 1;
}

/* ===== CHAT ===== */

void responder(const char*msg){
 char m[IN],r[128];
 int n;

 if(buscar(msg,r)){
  histadd("DSi IA",r);
  histshow();
  printf("\n%s\n",r);
  return;
 }

 strncpy(m,msg,IN-1);
 m[IN-1]=0;
 minus(m);

 strcpy(r,"No lo se todavia. Puedes ensenarme escribiendo APRENDER.");

 if(tiene(m,"cuanto es")||tiene(m,"calcula")||
    strchr(m,'+')||strchr(m,'-')||strchr(m,'*')||
    strchr(m,'x')||strchr(m,'/')||strchr(m,'^')){
  if(calcular(m,&n))
   snprintf(r,128,"El resultado es: %d",n);
 }
 else if(tiene(m,"hola")||tiene(m,"buenos")||tiene(m,"buenas"))
  strcpy(r,"Hola! Soy DSi IA. Preguntame o ensename algo.");
 else if(tiene(m,"quien eres"))
  strcpy(r,"Soy DSi IA, tu asistente escolar para DSi XL.");
 else if(tiene(m,"que es la quimica")||tiene(m,"que es quimica"))
  strcpy(r,"La Quimica estudia la materia, sus propiedades y transformaciones.");
 else if(tiene(m,"que es un atomo"))
  strcpy(r,"Un atomo es la unidad basica de un elemento. Tiene protones, neutrones y electrones.");
 else if(tiene(m,"que es un verbo"))
  strcpy(r,"Un verbo expresa acciones, estados o procesos.");
 else if(tiene(m,"que es un sustantivo"))
  strcpy(r,"Un sustantivo nombra personas, animales, cosas, lugares o ideas.");
 else if(tiene(m,"gracias"))
  strcpy(r,"De nada! Sigue aprendiendo.");
 else if(tiene(m,"adios")||tiene(m,"hasta luego"))
  strcpy(r,"Hasta luego! Que te vaya excelente.");

 histadd("DSi IA",r);
 histshow();
 printf("\n%s\n",r);
}

void chat(){
 char m[IN],x[IN];
 nh=0;
 teclado();

 histadd("DSi IA","Hola! Escribe una pregunta.");
 histadd("DSi IA","Si no se algo, escribe APRENDER.");

 while(1){
  histshow();
  printf("\n----------------\n");

  if(!leer(m,IN)||!m[0])continue;

  strncpy(x,m,IN-1);
  x[IN-1]=0;
  minus(x);

  if(tiene(x,"salir")||tiene(x,"menu"))break;

  histadd("Tu",m);

  if(aprender_detectado(m)){
   aprender();
   continue;
  }

  responder(m);
  printf("\nPulsa A...");
  espera();
 }

 ocultar();
}

/* ===== EXAMEN ===== */

int correcta(const char*u,const char*c){
 char a[IN],b[IN];

 strncpy(a,u,IN-1);
 strncpy(b,c,IN-1);
 a[IN-1]=b[IN-1]=0;
 minus(a);
 minus(b);

 return !strcmp(a,b)||tiene(a,b);
}

void jugar(const char*cat,int lec){
 int idx[50],n=0;

 for(int i=0;i<total;i++)
  if(!cat||!strcmp(banco[i].c,cat))
   idx[n++]=i;

 if(!n)return;

 for(int i=n-1;i>0;i--){
  int j=rand()%(i+1);
  int t=idx[i];idx[i]=idx[j];idx[j]=t;
 }

 int max=n<7?n:7;
 teclado();

 for(int q=0;q<max;q++){
  Pregunta*p=&banco[idx[q]];
  char r[IN];

  printf("\x1b[2J%s\nPregunta %d/%d\n\n%s\n\n",
         p->c,q+1,max,p->q);

  leer(r,IN);
  hechas++;

  if(r[0]&&correcta(r,p->r)){
   aciertos++;
   puntos+=10;
   printf("\n*** CORRECTO! ***\n");
   if(lec)printf("\n%s\n",p->e);
  }else
   printf("\nIncorrecto.\nRespuesta: %s\n%s\n",p->r,p->e);

  printf("\nPulsa A...");
  espera();
 }

 ocultar();
}

/* ===== MENU ===== */

void menu(){
 ocultar();
 printf("\x1b[2J");
 printf("==============================\n");
 printf("           DSi IA\n");
 printf("      Asistente escolar\n");
 printf("==============================\n\n");
 printf("A - Examen\n");
 printf("B - Leccion\n");
 printf("X - Matematicas\n");
 printf("Y - Espanol\n");
 printf("L - Quimica\n");
 printf("R - Cultura General\n");
 printf("SELECT - Chat / Aprender\n");
 printf("START - Salir\n\n");
 printf("Puntos: %d | Aciertos: %d/%d\n",
        puntos,aciertos,hechas);
}

int main(void){
 videoSetMode(MODE_0_2D);
 videoSetModeSub(MODE_0_2D);
 vramSetBankA(VRAM_A_MAIN_BG);
 vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(NULL,0,BgType_Text4bpp,
            BgSize_T_256x256,22,3,true,true);
 consoleInit(NULL,0,BgType_Text4bpp,
            BgSize_T_256x256,22,3,false,true);

 srand(2026);

 if(fatInitDefault()){
  sd=1;
 }

 while(1){
  menu();

  while(1){
   swiWaitForVBlank();
   scanKeys();
   u16 k=keysDown();

   if(k&KEY_A){jugar(NULL,0);break;}
   if(k&KEY_B){jugar(NULL,1);break;}
   if(k&KEY_X){jugar("Matematicas",1);break;}
   if(k&KEY_Y){jugar("Espanol",1);break;}
   if(k&KEY_L){jugar("Quimica",1);break;}
   if(k&KEY_R){jugar("General",1);break;}
   if(k&KEY_SELECT){chat();break;}

   if(k&KEY_START){
    printf("\x1b[2JHasta luego!\nGracias por usar DSi IA.");
    while(1)swiWaitForVBlank();
   }
  }
 }

 return 0;
}
