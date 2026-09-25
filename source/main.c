#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IN 48
#define HIST 10
#define ARCH "DSiIA_KNOWLEDGE.DAT"

typedef struct{const char*q,*r,*e,*c;}P;
typedef struct{const char*k,*r;}I;

Keyboard*kbd;
PrintConsole top,bot;
char hist[HIST][64];
int nh,pts,ok,done,sd;

void low(char*s){
 while(*s){*s=tolower((unsigned char)*s);s++;}
}

void normal(char*s){
 char*t=s,*d=s;
 while(*t){
  if(!strchr("¿?!¡.,;:\"'",*t))*d++=*t;
  t++;
 }
 *d=0;
 low(s);
}

void topc(){consoleSelect(&top);}
void botc(){consoleSelect(&bot);}

void addh(const char*a,const char*b){
 if(nh>=HIST){
  for(int i=0;i<HIST-1;i++)strcpy(hist[i],hist[i+1]);
  nh=HIST-1;
 }
 snprintf(hist[nh++],64,"%s: %s",a,b);
}

void showh(){
 topc();
 printf("\x1b[2J=========== DSi IA ===========\n\n");
 for(int i=0;i<nh;i++)printf("%s\n\n",hist[i]);
}

void key(){
 botc();
 printf("\x1b[2J");
 kbd=keyboardInit(NULL,3,BgType_Text4bpp,
                  BgSize_T_256x512,20,0,false,true);
 keyboardShow();
}

void hidekey(){
 if(kbd)keyboardHide();
}

int readt(char*s,int max){
 int p=0;
 while(1){
  swiWaitForVBlank();
  scanKeys();
  int k=keyboardUpdate();

  if(k>0){
   if(k==DVK_ENTER||k=='\n'){
    s[p]=0;
    return 1;
   }

   if((k==DVK_BACKSPACE||k==8)&&p)
    s[--p]=0;
   else if(k>=32&&k<127&&p<max-1){
    s[p++]=k;
    s[p]=0;
   }
  }

  if(keysDown()&KEY_A){
   s[p]=0;
   return 1;
  }

  if(keysDown()&KEY_B){
   s[0]=0;
   return 0;
  }
 }
}

void waitA(){
 while(1){
  swiWaitForVBlank();
  scanKeys();
  if(keysDown()&KEY_A)return;
 }
}

/* ===== BANCO DE EXAMEN ===== */

P b[]={
{"Cuanto es 2+2","4","2+2=4.","Matematicas"},
{"Cuanto es 5x6","30","5x6=30.","Matematicas"},
{"Cuanto es 100/4","25","100/4=25.","Matematicas"},
{"Cuanto es 15-8","7","15-8=7.","Matematicas"},
{"Cuanto es 2 elevado a 5","32","2^5=32.","Matematicas"},
{"Raiz cuadrada de 144","12","12x12=144.","Matematicas"},
{"Que es una fraccion","parte de un todo","Representa partes iguales de un todo.","Matematicas"},
{"Que es un porcentaje","parte de cien","Representa una cantidad por cada 100.","Matematicas"},
{"Que es una variable","valor que cambia","Representa un valor que puede cambiar.","Matematicas"},
{"Que es una constante","valor fijo","Es un valor que permanece igual.","Matematicas"},
{"Que es un numero primo","dos divisores","Solo tiene como divisores positivos al 1 y a si mismo.","Matematicas"},
{"Que es un numero par","divisible entre dos","Es divisible exactamente entre 2.","Matematicas"},
{"Que es un numero impar","no divisible entre dos","No es divisible exactamente entre 2.","Matematicas"},
{"Que es un numero entero","numero sin parte decimal","Incluye numeros negativos, cero y positivos.","Matematicas"},
{"Que es un numero racional","fraccion","Puede escribirse como cociente de dos enteros.","Matematicas"},
{"Que es un numero irracional","no fraccion exacta","No puede expresarse como fraccion de dos enteros.","Matematicas"},
{"Que es un decimal","numero decimal","Representa cantidades con decimas, centesimas y mas.","Matematicas"},
{"Que es una razon","comparacion","Compara dos cantidades mediante una division.","Matematicas"},
{"Que es una proporcion","igualdad de razones","Relaciona dos razones que tienen el mismo valor.","Matematicas"},
{"Que es la regla de tres","proporcion","Permite encontrar un valor desconocido entre cantidades relacionadas.","Matematicas"},
{"Que es la media","promedio","Se suman los datos y se divide entre su cantidad.","Matematicas"},
{"Que es la mediana","valor central","Es el valor central de datos ordenados.","Matematicas"},
{"Que es la moda","valor frecuente","Es el dato que aparece mas veces.","Matematicas"},
{"Que es probabilidad","posibilidad","Mide que tan posible es que ocurra un evento.","Matematicas"},
{"Que es estadistica","analisis de datos","Recopila, organiza y analiza datos.","Matematicas"},
{"Que es algebra","matematicas con letras","Usa letras y numeros para representar relaciones.","Matematicas"},
{"Que es una potencia","multiplicacion repetida","Una base se multiplica varias veces por si misma.","Matematicas"},
{"Que es una raiz cuadrada","numero","Es el numero que multiplicado por si mismo produce otro.","Matematicas"},
{"Que es la jerarquia de operaciones","orden","Indica el orden correcto para resolver operaciones.","Matematicas"},
{"Que es una ecuacion","igualdad","Es una igualdad que contiene uno o mas valores desconocidos.","Matematicas"},
{"Que es una desigualdad","comparacion","Compara cantidades usando signos como < y >.","Matematicas"},
{"Que es una expresion algebraica","expresion con letras","Combina numeros, letras y operaciones.","Matematicas"},
{"Que es un termino algebraico","termino","Puede contener coeficiente, variable y exponentes.","Matematicas"},
{"Que es un monomio","un termino","Es una expresion algebraica de un solo termino.","Matematicas"},
{"Que es un polinomio","varios terminos","Es una expresion algebraica formada por varios terminos.","Matematicas"},
{"Que es factorizar","descomponer","Transforma una expresion en producto de factores.","Matematicas"},
{"Que es una ecuacion lineal","primer grado","La variable tiene exponente uno.","Matematicas"},
{"Que es una funcion","relacion","Relaciona valores de entrada con valores de salida.","Matematicas"},
{"Que es una coordenada","posicion","Indica la posicion de un punto.","Matematicas"},
{"Que es el plano cartesiano","ejes","Usa los ejes X y Y para ubicar puntos.","Matematicas"},
{"Que es una pendiente","inclinacion","Mide el cambio de Y respecto al cambio de X.","Matematicas"},
{"Que es una sucesion","secuencia","Es una lista ordenada de numeros siguiendo una regla.","Matematicas"},
{"Que es una sucesion aritmetica","diferencia constante","Cada termino cambia por una cantidad constante.","Matematicas"},
{"Que es una sucesion geometrica","razon constante","Cada termino se obtiene multiplicando por una razon fija.","Matematicas"},
{"Que es el valor absoluto","distancia al cero","Es la distancia de un numero al cero.","Matematicas"},
{"Que es la notacion cientifica","potencias de diez","Representa numeros usando potencias de 10.","Matematicas"},
{"Que es un sistema de ecuaciones","varias ecuaciones","Conjunto de ecuaciones que deben cumplirse simultaneamente.","Matematicas"},
{"Que es una variable independiente","entrada","Es la variable cuyo valor se elige o controla.","Matematicas"},
{"Que es una variable dependiente","salida","Su valor depende de otra variable.","Matematicas"},
{"Que es una identidad algebraica","igualdad siempre cierta","Es una igualdad verdadera para todos los valores permitidos.","Matematicas"},
{"Que es un binomio","dos terminos","Expresion algebraica formada por dos terminos.","Matematicas"},
{"Que es un trinomio","tres terminos","Expresion algebraica formada por tres terminos.","Matematicas"},
{"Que es el discriminante","parte de una ecuacion","En una cuadratica indica informacion sobre sus soluciones.","Matematicas"},
{"Que es una ecuacion cuadratica","segundo grado","Tiene una variable elevada al cuadrado como mayor exponente.","Matematicas"},
{"Que es una raiz de una ecuacion","solucion","Es un valor que hace verdadera la ecuacion.","Matematicas"},
{"Que es una funcion lineal","funcion de primer grado","Puede representarse mediante una recta.","Matematicas"},
{"Que es una funcion cuadratica","parabola","Su grafica suele ser una parabola.","Matematicas"},
{"Que es el perimetro","suma de lados","Es la longitud del contorno de una figura.","Geometria"},
{"Que es el area","superficie","Mide la superficie de una figura.","Geometria"},
{"Que es el volumen","espacio","Mide el espacio que ocupa un cuerpo.","Geometria"},
{"Que es un triangulo","tres lados","Es un poligono de tres lados.","Geometria"},
{"Que es un cuadrado","cuatro lados iguales","Tiene cuatro lados iguales y cuatro angulos rectos.","Geometria"},
{"Que es un rectangulo","cuatro angulos rectos","Sus lados opuestos son iguales.","Geometria"},
{"Que es un rombo","cuatro lados iguales","Es un cuadrilatero con cuatro lados iguales.","Geometria"},
{"Que es un trapecio","cuadrilatero","Tiene al menos un par de lados paralelos.","Geometria"},
{"Que es un circulo","figura circular","Es la region interior de una circunferencia.","Geometria"},
{"Que es una circunferencia","linea curva cerrada","Es el borde de un circulo.","Geometria"},
{"Que es pi","3.14159","Pi relaciona la longitud de una circunferencia con su diametro.","Geometria"},
{"Area de un cuadrado","lado por lado","A=l*l.","Geometria"},
{"Perimetro de un cuadrado","cuatro veces el lado","P=4l.","Geometria"},
{"Area de un rectangulo","base por altura","A=b*h.","Geometria"},
{"Area de un triangulo","base por altura entre dos","A=b*h/2.","Geometria"},
{"Area de un circulo","pi por radio al cuadrado","A=pi*r*r.","Geometria"},
{"Longitud de una circunferencia","dos pi por radio","C=2*pi*r.","Geometria"},
{"Que es un angulo","abertura","Es la abertura formada por dos semirrectas.","Geometria"},
{"Que es un angulo recto","90 grados","Mide exactamente 90 grados.","Geometria"},
{"Que es un angulo llano","180 grados","Mide exactamente 180 grados.","Geometria"},
{"Que es un angulo completo","360 grados","Una vuelta completa mide 360 grados.","Geometria"},
{"Que es un angulo agudo","menos de 90","Mide mas de 0 y menos de 90 grados.","Geometria"},
{"Que es un angulo obtuso","entre 90 y 180","Es mayor que 90 y menor que 180 grados.","Geometria"},
{"Que es el teorema de pitagoras","a2+b2=c2","En un triangulo rectangulo relaciona sus lados.","Geometria"},
{"Que es seno","razon trigonometrica","En un triangulo rectangulo es cateto opuesto entre hipotenusa.","Geometria"},
{"Que es coseno","razon trigonometrica","Es cateto adyacente entre hipotenusa.","Geometria"},
{"Que es tangente","razon trigonometrica","Es cateto opuesto entre cateto adyacente.","Geometria"},
{"Que es la quimica","ciencia de la materia","Estudia la materia, sus propiedades, composicion y transformaciones.","Quimica"},
{"Que es la materia","masa y espacio","Tiene masa y ocupa un lugar en el espacio.","Quimica"},
{"Que es un atomo","unidad de un elemento","Es la unidad basica de un elemento quimico.","Quimica"},
{"Que es un proton","particula positiva","Se encuentra en el nucleo y tiene carga positiva.","Quimica"},
{"Que es un neutron","particula sin carga","Se encuentra en el nucleo y no tiene carga.","Quimica"},
{"Que es un electron","particula negativa","Tiene carga negativa y se encuentra alrededor del nucleo.","Quimica"},
{"Que es el nucleo atomico","centro del atomo","Contiene protones y neutrones.","Quimica"},
{"Que es un elemento quimico","tipo de atomo","Sus atomos tienen el mismo numero de protones.","Quimica"},
{"Que es un compuesto","union de elementos","Esta formado por diferentes elementos unidos quimicamente.","Quimica"},
{"Que es una molecula","union de atomos","Es un conjunto de atomos unidos.","Quimica"},
{"Formula del agua","h2o","Tiene dos atomos de hidrogeno y uno de oxigeno.","Quimica"},
{"Formula del dioxido de carbono","co2","Tiene un carbono y dos oxigenos.","Quimica"},
{"Formula de la sal","nacl","El cloruro de sodio tiene formula NaCl.","Quimica"},
{"Simbolo del hidrogeno","h","H representa al hidrogeno.","Quimica"},
{"Simbolo del oxigeno","o","O representa al oxigeno.","Quimica"},
{"Simbolo del carbono","c","C representa al carbono.","Quimica"},
{"Simbolo del sodio","na","Na representa al sodio.","Quimica"},
{"Simbolo del hierro","fe","Fe representa al hierro.","Quimica"},
{"Simbolo del oro","au","Au representa al oro.","Quimica"},
{"Simbolo de la plata","ag","Ag representa a la plata.","Quimica"},
{"Que es la tabla periodica","organizacion de elementos","Organiza los elementos principalmente por numero atomico.","Quimica"},
{"Que es el numero atomico","numero de protones","Indica cuantos protones tiene un atomo.","Quimica"},
{"Que es la masa atomica","masa promedio","Representa la masa promedio de los atomos de un elemento.","Quimica"},
{"Que es un isotopo","mismo elemento","Tiene el mismo numero de protones pero distinto numero de neutrones.","Quimica"},
{"Que es un ion","atomo con carga","Es un atomo o grupo de atomos con carga electrica.","Quimica"},
{"Que es un cation","ion positivo","Se forma cuando se pierden electrones.","Quimica"},
{"Que es un anion","ion negativo","Se forma cuando se ganan electrones.","Quimica"},
{"Que es un enlace ionico","transferencia de electrones","Se produce por atraccion entre iones de cargas opuestas.","Quimica"},
{"Que es un enlace covalente","compartir electrones","Los atomos comparten electrones.","Quimica"},
{"Que es un enlace metalico","electrones compartidos","Es caracteristico de los metales.","Quimica"},
{"Que es una reaccion quimica","transformacion de sustancias","Los reactivos se transforman en productos.","Quimica"},
{"Que es un reactivo","sustancia inicial","Participa al inicio de una reaccion.","Quimica"},
{"Que es un producto quimico","sustancia final","Se forma como resultado de una reaccion.","Quimica"},
{"Que es combustion","reaccion con oxigeno","Suele liberar calor y luz.","Quimica"},
{"Que es oxidacion","perdida de electrones","En el modelo electronico implica perdida de electrones.","Quimica"},
{"Que es reduccion","ganancia de electrones","En el modelo electronico implica ganancia de electrones.","Quimica"},
{"Que es ph","acidez","Indica la acidez o basicidad de una disolucion.","Quimica"},
{"Que es un acido","sustancia acida","Tiene propiedades acidas; en agua puede aumentar H+.","Quimica"},
{"Que es una base","sustancia basica","Tiene propiedades basicas; muchas bases producen OH- en agua.","Quimica"},
{"Que es una mezcla","combinacion fisica","Sus componentes no estan unidos quimicamente.","Quimica"},
{"Que es una mezcla homogenea","mezcla uniforme","Presenta una composicion uniforme.","Quimica"},
{"Que es una mezcla heterogenea","mezcla no uniforme","Sus componentes pueden distinguirse.","Quimica"},
{"Que es una solucion","mezcla homogenea","Es una mezcla de soluto y solvente.","Quimica"},
{"Que es el soluto","sustancia disuelta","Es la sustancia que se disuelve.","Quimica"},
{"Que es el solvente","medio que disuelve","Es la sustancia que disuelve al soluto.","Quimica"},
{"Que es evaporacion","liquido a gas","Es el cambio de liquido a gas.","Quimica"},
{"Que es fusion","solido a liquido","Es el cambio de solido a liquido.","Quimica"},
{"Que es condensacion","gas a liquido","Es el cambio de gas a liquido.","Quimica"},
{"Que es solidificacion","liquido a solido","Es el cambio de liquido a solido.","Quimica"},
{"Que es sublimacion","solido a gas","Es el cambio directo de solido a gas.","Quimica"},
{"Que es un catalizador","acelera reacciones","Aumenta la velocidad de una reaccion sin consumirse netamente.","Quimica"},
{"Que es una reaccion exotermica","libera calor","Libera energia hacia el entorno.","Quimica"},
{"Que es una reaccion endotermica","absorbe calor","Absorbe energia del entorno.","Quimica"},
{"Que es concentracion","cantidad de soluto","Indica cuanto soluto hay en una cantidad de solucion.","Quimica"},
{"Que es un sustantivo","nombre","Nombra personas, animales, objetos, lugares, ideas o conceptos.","Espanol"},
{"Que es un verbo","accion o estado","Expresa acciones, estados o procesos.","Espanol"},
{"Que es un adjetivo","cualidad","Describe o caracteriza a un sustantivo.","Espanol"},
{"Que es un adverbio","modificador","Modifica principalmente un verbo, adjetivo u otro adverbio.","Espanol"},
{"Que es un pronombre","sustituye al nombre","Puede sustituir a un sustantivo.","Espanol"},
{"Que es un articulo","acompanante","Acompana al sustantivo y ayuda a determinarlo.","Espanol"},
{"Que es una oracion","enunciado","Expresa una idea completa y normalmente contiene un verbo.","Espanol"},
{"Que es el sujeto","quien realiza","Es de quien se habla o quien realiza la accion.","Espanol"},
{"Que es el predicado","lo que se dice","Es lo que se afirma acerca del sujeto.","Espanol"},
{"Que es el nucleo del sujeto","sustantivo","Es la palabra principal del sujeto.","Espanol"},
{"Que es el nucleo del predicado","verbo","Generalmente es el verbo principal.","Espanol"},
{"Que es un sinonimo","significado parecido","Palabra con significado igual o parecido.","Espanol"},
{"Que es un antonimo","significado contrario","Palabra que expresa un significado opuesto.","Espanol"},
{"Que es una palabra homofona","mismo sonido","Suena igual que otra pero puede tener distinto significado o escritura.","Espanol"},
{"Que es una silaba","unidad de voz","Es una unidad de pronunciacion de una palabra.","Espanol"},
{"Que es una palabra aguda","ultima silaba","Tiene la mayor fuerza de voz en la ultima silaba.","Espanol"},
{"Que es una palabra grave","penultima silaba","Tiene la mayor fuerza de voz en la penultima silaba.","Espanol"},
{"Que es una palabra esdrujula","antepenultima silaba","Tiene la mayor fuerza de voz en la antepenultima silaba.","Espanol"},
{"Que es un diptongo","dos vocales","Dos vocales se pronuncian en una misma silaba.","Espanol"},
{"Que es un hiato","vocales separadas","Dos vocales juntas pertenecen a silabas diferentes.","Espanol"},
{"Que es un texto narrativo","relato","Cuenta hechos reales o ficticios mediante una narracion.","Espanol"},
{"Que es un texto descriptivo","descripcion","Presenta caracteristicas de personas, lugares, objetos o situaciones.","Espanol"},
{"Que es un texto argumentativo","argumentos","Defiende una idea mediante razones o argumentos.","Espanol"},
{"Que es un texto expositivo","informacion","Explica un tema de manera clara y organizada.","Espanol"},
{"Que es una metafora","comparacion implicita","Relaciona dos ideas sin usar necesariamente como.","Espanol"},
{"Que es un poema","obra literaria","Texto literario que puede usar ritmo, versos e imagenes poeticas.","Espanol"},
{"Que es una fabula","relato con moraleja","Relato breve que suele transmitir una ensenanza.","Espanol"},
{"Que es una novela","narracion extensa","Obra narrativa extensa con personajes y acontecimientos.","Espanol"},
{"Que es un cuento","narracion breve","Relato generalmente breve con personajes y acontecimientos.","Espanol"},
{"Que es la fisica","ciencia de la naturaleza","Estudia materia, energia, movimiento, fuerzas y sus interacciones.","Fisica"},
{"Que es la velocidad","distancia entre tiempo","Indica cuanto espacio se recorre por unidad de tiempo.","Fisica"},
{"Que es la aceleracion","cambio de velocidad","Mide como cambia la velocidad con el tiempo.","Fisica"},
{"Que es una fuerza","interaccion","Puede cambiar el movimiento o deformar un objeto.","Fisica"},
{"Que es la gravedad","atraccion","Es la interaccion que atrae masas entre si.","Fisica"},
{"Que es la energia","capacidad de producir cambios","Puede transferirse o transformarse.","Fisica"},
{"Que es el trabajo mecanico","fuerza por distancia","En un caso simple, W=F*d.","Fisica"},
{"Que es la potencia","trabajo por tiempo","Indica la rapidez con que se realiza trabajo.","Fisica"},
{"Que es la electricidad","fenomeno de cargas","Involucra cargas electricas y sus interacciones.","Fisica"},
{"Que es la corriente electrica","movimiento de carga","Es el flujo ordenado de carga electrica.","Fisica"},
{"Que es un circuito","camino electrico","Permite el paso de corriente mediante un recorrido cerrado.","Fisica"},

{"Que es una celula","unidad de vida","Es la unidad estructural y funcional basica de los seres vivos.","Biologia"},
{"Que es el ADN","material genetico","Contiene informacion genetica en los seres vivos.","Biologia"},
{"Que es un gen","segmento de ADN","Es una unidad de informacion genetica.","Biologia"},
{"Que es la fotosintesis","produccion de alimento","Las plantas usan luz para producir materia organica a partir de sustancias simples.","Biologia"},
{"Que es un ecosistema","seres vivos y ambiente","Incluye organismos y factores no vivos que interactuan.","Biologia"},
{"Que es una especie","grupo de organismos","Grupo de organismos con caracteristicas y reproduccion compatibles segun la definicion biologica.","Biologia"},
{"Que es la respiracion celular","obtencion de energia","Proceso celular que obtiene energia util a partir de nutrientes.","Biologia"},

{"Capital de Mexico","ciudad de mexico","La capital de Mexico es Ciudad de Mexico.","General"},
{"Capital de Francia","paris","La capital de Francia es Paris.","General"},
{"Capital de Japon","tokio","La capital de Japon es Tokio.","General"},
{"Capital de Espana","madrid","La capital de Espana es Madrid.","General"},
{"Cuantos planetas hay","8","El sistema solar tiene ocho planetas.","General"},
{"Planeta mas cercano al sol","mercurio","Mercurio es el planeta mas cercano al Sol.","General"},
{"Planeta mas grande","jupiter","Jupiter es el planeta mas grande del sistema solar.","General"},
{"Satelite natural de la tierra","luna","La Luna es el satelite natural de la Tierra.","General"},
{"Que es la democracia","forma de gobierno","Es un sistema donde la ciudadania participa en las decisiones politicas.","Civica"},
{"Que es un derecho humano","derecho","Son derechos inherentes a todas las personas.","Civica"},
};

int total=sizeof(b)/sizeof(b[0]);

/* ===== APRENDIZAJE ===== */

void saveknow(const char*q,const char*r){
 FILE*f;
 unsigned short a=strlen(q),z=strlen(r);

 if(!sd)return;
 if(a>63)a=63;
 if(z>127)z=127;

 f=fopen(ARCH,"ab");
 if(!f)return;

 fwrite(&a,2,1,f);
 fwrite(&z,2,1,f);
 fwrite(q,1,a,f);
 fwrite(r,1,z,f);

 fclose(f);
}

int findknow(const char*msg,char*r){
 FILE*f;
 unsigned short a,z;
 char q[64],x[128],m[64];
 int found=0;

 if(!sd)return 0;

 strncpy(m,msg,63);
 m[63]=0;
 normal(m);

 f=fopen(ARCH,"rb");
 if(!f)return 0;

 while(fread(&a,2,1,f)==1&&fread(&z,2,1,f)==1){
  if(a>63||z>127)break;
  if(fread(q,1,a,f)!=a)break;
  if(fread(x,1,z,f)!=z)break;

  q[a]=0;
  x[z]=0;
  normal(q);

  if(!strcmp(m,q)){
   strcpy(r,x);
   found=1;
  }
 }

 fclose(f);
 return found;
}

void learn(){
 char q[64],r[128];

 topc();
 printf("\x1b[2J========== APRENDER ==========\n\n");
 printf("Pulsa A y escribe la pregunta.");
 waitA();

 if(!readt(q,64)||!q[0])return;

 topc();
 printf("\x1b[2JAhora escribe la respuesta.");
 waitA();

 if(!readt(r,128)||!r[0])return;

 normal(q);
 saveknow(q,r);

 addh("DSi IA","Aprendido y guardado en la SD.");
 showh();

 topc();
 printf("\nPulsa A...");
 waitA();
}

/* ===== CALCULADORA ===== */

int calc(const char*e,int*r){
 char b[64],l[64],a[32]={0},c[32]={0},op=0;
 int p=-1,j=0,x,y;

 strncpy(b,e,63);
 b[63]=0;
 normal(b);

 for(int i=0;b[i]&&j<63;i++)
  if(b[i]!=' ')l[j++]=b[i];

 l[j]=0;

 for(int i=1;l[i];i++)
  if(strchr("+-*x/^",l[i])){
   op=l[i]=='x'?'*':l[i];
   p=i;
   break;
  }

 if(p<0)return 0;

 strncpy(a,l,p);
 a[p]=0;
 strcpy(c,l+p+1);

 x=atoi(a);
 y=atoi(c);

 switch(op){
  case '+':*r=x+y;break;
  case '-':*r=x-y;break;
  case '*':*r=x*y;break;
  case '/':if(!y)return 0;*r=x/y;break;
  case '^':
   *r=1;
   for(int i=0;i<y;i++)*r*=x;
   break;
  default:return 0;
 }

 return 1;
}

/* ===== RESPUESTA ===== */

void answer(const char*msg){
 char m[IN],r[128];
 int n;

 if(findknow(msg,r)){
  addh("DSi IA",r);
  showh();
  return;
 }

 strncpy(m,msg,IN-1);
 m[IN-1]=0;
 normal(m);

 strcpy(r,"No tengo una respuesta para eso. Puedes usar APRENDER.");

 /* Busqueda por palabras clave */
 for(int i=0;i<total;i++){
  char q[64];

  strncpy(q,b[i].q,63);
  q[63]=0;
  normal(q);

  if(strstr(m,q)){
   strncpy(r,b[i].r,127);
   r[127]=0;
   break;
  }
 }

 /* Operaciones */
 if((strstr(m,"cuanto es")||strstr(m,"calcula"))&&
    (strchr(m,'+')||strchr(m,'-')||strchr(m,'*')||
     strchr(m,'x')||strchr(m,'/')||strchr(m,'^'))){
  if(calc(m,&n))
   snprintf(r,128,"El resultado es: %d",n);
 }

 addh("DSi IA",r);
 showh();
}
/* ===== CHAT ===== */

void chat(){
 char m[IN],x[IN];

 nh=0;
 key();

 addh("DSi IA","Hola! Soy DSi IA.");
 addh("DSi IA","Pregunta lo que quieras.");

 while(1){

  showh();

  if(!readt(m,IN))continue;
  if(!m[0])continue;

  strncpy(x,m,IN-1);
  x[IN-1]=0;
  normal(x);

  if(strstr(x,"salir")||strstr(x,"menu"))
   break;

  addh("Tu",m);

  if(!strcmp(x,"aprender")||strstr(x," aprender")){
   learn();
   continue;
  }

  answer(m);
 }

 hidekey();
}

/* ===== EXAMEN ===== */

int right(const char*u,const char*c){
 char a[IN],b[IN];

 strncpy(a,u,IN-1);
 strncpy(b,c,IN-1);

 a[IN-1]=0;
 b[IN-1]=0;

 normal(a);
 normal(b);

 return !strcmp(a,b)||strstr(a,b);
}

void play(const char*cat,int lesson){
 int ix[100],n=0;

 for(int i=0;i<total;i++)
  if(!cat||!strcmp(b[i].c,cat))
   ix[n++]=i;

 if(!n)return;

 for(int i=n-1;i;i--){
  int j=rand()%(i+1);
  int t=ix[i];
  ix[i]=ix[j];
  ix[j]=t;
 }

 int max=n<7?n:7;

 key();

 for(int q=0;q<max;q++){
  P*p=&b[ix[q]];
  char r[IN];

  topc();

  printf("\x1b[2J");
  printf("=========== %s ===========\n",p->c);
  printf("Pregunta %d/%d\n\n%s\n\n",q+1,max,p->q);

  readt(r,IN);
  done++;

  topc();

  if(r[0]&&right(r,p->r)){
   ok++;
   pts+=10;

   printf("*** CORRECTO! ***\n\n");

   if(lesson)
    printf("%s\n",p->e);

  }else{
   printf("Incorrecto.\n\n");
   printf("Respuesta: %s\n\n",p->r);
   printf("%s\n",p->e);
  }

  printf("\nPulsa A...");
  waitA();
 }

 hidekey();
}

/* ===== MENU ===== */

void menu(){
 hidekey();

 topc();

 printf("\x1b[2J");
 printf("==============================\n");
 printf("           DSi IA\n");
 printf("      PROFESOR VIRTUAL\n");
 printf("==============================\n\n");

 printf("A - Examen general\n");
 printf("B - Leccion general\n");
 printf("X - Matematicas\n");
 printf("Y - Espanol\n");
 printf("L - Quimica\n");
 printf("R - Cultura general\n");
 printf("SELECT - Chat\n");
 printf("START - Salir\n\n");

 printf("Puntos: %d\n",pts);
 printf("Aciertos: %d/%d\n",ok,done);
}

int main(void){

 videoSetMode(MODE_0_2D);
 videoSetModeSub(MODE_0_2D);

 vramSetBankA(VRAM_A_MAIN_BG);
 vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(&top,0,
             BgType_Text4bpp,
             BgSize_T_256x256,
             22,3,true,true);

 consoleInit(&bot,0,
             BgType_Text4bpp,
             BgSize_T_256x256,
             22,3,false,true);

 srand(2026);

 sd=fatInitDefault();

 while(1){

  menu();

  while(1){

   swiWaitForVBlank();
   scanKeys();

   u16 k=keysDown();

   if(k&KEY_A){
    play(NULL,0);
    break;
   }

   if(k&KEY_B){
    play(NULL,1);
    break;
   }

   if(k&KEY_X){
    play("Matematicas",1);
    break;
   }

   if(k&KEY_Y){
    play("Espanol",1);
    break;
   }

   if(k&KEY_L){
    play("Quimica",1);
    break;
   }

   if(k&KEY_R){
    play("General",1);
    break;
   }

   if(k&KEY_SELECT){
    chat();
    break;
   }

   if(k&KEY_START){
    topc();

    printf("\x1b[2J");
    printf("Hasta luego!\n\n");
    printf("Gracias por usar DSi IA.");

    while(1)
     swiWaitForVBlank();
   }
  }
 }

 return 0;
}
