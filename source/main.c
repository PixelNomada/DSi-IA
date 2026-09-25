#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 48
#define MAX_HISTORIAL 8

typedef struct{
 const char *pregunta,*respuesta,*explicacion,*categoria;
}Pregunta;

typedef struct{
 const char *clave,*respuesta;
}Tema;

Pregunta banco[]={
{"2+2","4","2+2=4.","Matematicas"},
{"5x6","30","5 por 6=30.","Matematicas"},
{"10/2","5","10 entre 2=5.","Matematicas"},
{"3^2","9","3 al cuadrado=9.","Matematicas"},
{"Raiz de 25","5","5x5=25.","Matematicas"},
{"15% de 100","15","15 por ciento de 100=15.","Matematicas"},
{"Triangulo: suma de angulos","180","Todo triangulo suma 180 grados.","Matematicas"},
{"Formula de Pitagoras","a2+b2=c2","Sirve para triangulos rectangulos.","Matematicas"},
{"Perimetro de cuadrado lado 4","16","4+4+4+4=16.","Matematicas"},
{"Area de cuadrado lado 5","25","5x5=25.","Matematicas"},

{"Sustantivo","Nombre","Nombra personas, animales, cosas o lugares.","Espanol"},
{"Verbo","Accion","Expresa accion, estado o proceso.","Espanol"},
{"Adjetivo","Cualidad","Describe un sustantivo.","Espanol"},
{"Sinonimo de feliz","Contento","Son palabras de significado parecido.","Espanol"},
{"Antonimo de grande","Pequeno","Son palabras de significado contrario.","Espanol"},
{"Que es una oracion","Enunciado","Expresa una idea completa.","Espanol"},
{"Que es un texto","Conjunto de ideas","Comunica un mensaje.","Espanol"},
{"Que es un parrafo","Grupo de oraciones","Desarrolla una idea.","Espanol"},

{"Que es un atomo","Unidad de materia","Tiene protones, neutrones y electrones.","Quimica"},
{"Que es una molecula","Union de atomos","Los atomos se unen quimicamente.","Quimica"},
{"Que es el agua","H2O","Tiene dos H y un O.","Quimica"},
{"Que es el oxigeno","Elemento","Su simbolo es O.","Quimica"},
{"Que es el hidrogeno","Elemento","Su simbolo es H.","Quimica"},
{"Que mide el pH","Acidez","7 es neutro.","Quimica"},
{"pH menor que 7","Acido","Los acidos tienen pH menor que 7.","Quimica"},
{"pH mayor que 7","Base","Las bases tienen pH mayor que 7.","Quimica"},
{"Que es una reaccion","Cambio quimico","Se forman nuevas sustancias.","Quimica"},
{"Que es un elemento","Sustancia pura","Tiene un tipo de atomo.","Quimica"},

{"Que es la fuerza","Empuje o jalon","Puede cambiar el movimiento.","Fisica"},
{"Que es velocidad","Distancia/tiempo","Indica que tan rapido se mueve algo.","Fisica"},
{"Que es gravedad","Atraccion","La masa atrae a otra masa.","Fisica"},
{"Que es energia","Capacidad de cambio","Puede producir trabajo o transformaciones.","Fisica"},
{"Que es masa","Cantidad de materia","Se mide normalmente en kg.","Fisica"},
{"Que es temperatura","Medida termica","Indica que tan caliente o frio esta algo.","Fisica"},

{"Que es una celula","Unidad de vida","Forma la base de los seres vivos.","Biologia"},
{"Que es el ADN","Material genetico","Contiene informacion hereditaria.","Biologia"},
{"Que es un ecosistema","Seres vivos y ambiente","Ambos interactuan.","Biologia"},
{"Que hacen las plantas","Fotosintesis","Usan luz para producir alimento.","Biologia"},
{"Que es un mamifero","Animal con glandulas mamarias","La mayoria nace del vientre materno.","Biologia"},

{"Capital de Mexico","Ciudad de Mexico","Es la capital del pais.","General"},
{"Planeta rojo","Marte","Su superficie contiene mucho oxido de hierro.","General"},
{"Satelite natural de la Tierra","Luna","Orbita nuestro planeta.","General"},
{"Estrella del sistema solar","Sol","Es la estrella central.","General"},
{"Continente de Mexico","America","Mexico esta en America del Norte.","Geografia"},
{"Planeta mas grande","Jupiter","Es el mayor del sistema solar.","General"},
{"Oceano mas grande","Pacifico","Es el mayor oceano de la Tierra.","Geografia"},
{"Que estudia la historia","El pasado","Analiza hechos y procesos humanos.","Historia"},
{"Que fue Roma","Civilizacion antigua","Domino gran parte de Europa y el Mediterraneo.","Historia"},
{"Independencia de Mexico","1810","El movimiento inicio en 1810.","Historia"},

{"Que es internet","Red mundial","Conecta dispositivos y redes.","Tecnologia"},
{"Que es un programa","Software","Son instrucciones para una computadora.","Tecnologia"},
{"Que es hardware","Parte fisica","Son los componentes de un dispositivo.","Tecnologia"},
{"Que es software","Programas","Permite realizar tareas digitales.","Tecnologia"},
{"Que es un algoritmo","Pasos ordenados","Sirve para resolver problemas.","Tecnologia"},

{"hello","hola","Hello significa hola.","Ingles"},
{"goodbye","adios","Goodbye significa adios.","Ingles"},
{"computer","computadora","Computer significa computadora.","Ingles"},
{"water","agua","Water significa agua.","Ingles"},
{"book","libro","Book significa libro.","Ingles"}
};

Tema temas[]={
{"hola","Hola bro 😎 ¿Como estas?"},
{"buenas","¡Buenas! ¿Que hacemos?"},
{"hey","¡Hey! ¿Que tal?"},
{"como estas","Todo bien 😎 ¿Y tu?"},
{"quien eres","Soy DSi IA, tu asistente."},
{"que haces","Aqui estoy, listo para ayudarte."},
{"aburrido","Podemos hablar o aprender algo."},
{"gracias","¡De nada bro! 😎"},
{"adios","¡Nos vemos! 👋"},
{"escuela","¿Que materia quieres estudiar?"},
{"tarea","Dime la materia y te ayudo."},
{"matematicas","Puedo ayudarte con numeros, algebra y geometria."},
{"espanol","Podemos practicar gramatica y lectura."},
{"quimica","Puedo explicar atomos, elementos y reacciones."},
{"fisica","Podemos hablar de fuerza, energia y movimiento."},
{"historia","Puedo explicar hechos y procesos historicos."},
{"geografia","Podemos hablar de paises, mapas y planetas."},
{"biologia","Podemos hablar de celulas, animales y ecosistemas."},
{"tecnologia","Podemos hablar de computadoras, internet y programacion."},
{"juegos","¿Que juego estas jugando?"},
{"musica","¿Que tipo de musica te gusta?"},
{"peliculas","¿Que pelicula te gusta?"},
{"futbol","¿Cual es tu equipo favorito?"},
{"comida","¿Cual es tu comida favorita?"},
{"mexico","Mexico esta en America del Norte."},
{"planetas","El sistema solar tiene ocho planetas."},
{"sol","El Sol es la estrella de nuestro sistema."},
{"luna","La Luna es el satelite natural de la Tierra."},
{"tierra","La Tierra es nuestro planeta."},
{"marte","Marte es conocido como el planeta rojo."},
{"jupiter","Jupiter es el planeta mas grande."},
{"espacio","El espacio contiene estrellas, planetas y galaxias."},
{"universo","El universo contiene toda la materia y energia conocidas."},
{"robot","Un robot es una maquina programable."},
{"computadora","Una computadora procesa informacion mediante programas."},
{"internet","Internet conecta millones de dispositivos."},
{"programacion","Programar es crear instrucciones para una computadora."},
{"codigo","El codigo contiene instrucciones de un programa."},
{"html","HTML estructura el contenido de una pagina web."},
{"css","CSS controla el aspecto visual de una pagina."},
{"javascript","JavaScript permite agregar logica e interactividad web."},
{"c","C es un lenguaje de programacion muy usado."},
{"nintendo","Nintendo es una empresa conocida por sus videojuegos."},
{"ps4","PS4 es una consola de videojuegos de Sony."},
{"roblox","Roblox permite crear y jugar experiencias."},
{"minecraft","Minecraft permite construir y explorar mundos."},
{"sonic","Sonic es un personaje famoso de videojuegos."},
{"mario","Mario es un personaje clasico de Nintendo."},
{"dragon ball","Dragon Ball es una serie de manga y anime."},
{"anime","Anime es animacion producida principalmente en Japon."},
{"libros","Un libro puede enseñar, contar historias o informar."},
{"leer","Leer ayuda a comprender informacion e historias."},
{"ciencia","La ciencia estudia el mundo mediante observacion y pruebas."},
{"experimento","Un experimento prueba una idea bajo ciertas condiciones."},
{"atomo","Un atomo es una unidad basica de la materia."},
{"molecula","Una molecula esta formada por atomos unidos."},
{"agua","El agua tiene formula H2O."},
{"oxigeno","El oxigeno tiene simbolo O."},
{"hidrogeno","El hidrogeno tiene simbolo H."},
{"ph","El pH indica acidez o basicidad."},
{"acido","Una sustancia acida tiene pH menor que 7."},
{"base","Una base suele tener pH mayor que 7."},
{"fuerza","Una fuerza puede cambiar el movimiento."},
{"gravedad","La gravedad atrae masas entre si."},
{"energia","La energia puede producir cambios."},
{"velocidad","La velocidad relaciona distancia y tiempo."},
{"masa","La masa mide cantidad de materia."},
{"celula","La celula es la unidad basica de la vida."},
{"adn","El ADN contiene informacion genetica."},
{"animal","Los animales son organismos vivos."},
{"planta","Las plantas producen alimento mediante fotosintesis."},
{"ecosistema","Un ecosistema incluye seres vivos y ambiente."},
{"historia","La historia estudia procesos y hechos del pasado."},
{"geografia","La geografia estudia lugares y relaciones espaciales."},
{"mexico","Mexico tiene una gran diversidad geografica y cultural."},
{"independencia","La Independencia de Mexico comenzo en 1810."},
{"roma","Roma fue una importante civilizacion antigua."},
{"francia","Francia esta en Europa occidental."},
{"segunda guerra","La Segunda Guerra Mundial ocurrio entre 1939 y 1945."},
{"tierra plana","La Tierra tiene forma aproximadamente esferica."},
{"sol sistema","El Sol es el centro del sistema solar."},
{"galaxia","Una galaxia contiene estrellas, gas, polvo y otros objetos."},
{"estrella","Una estrella produce energia y luz."},
{"agujero negro","Es una region con gravedad extremadamente intensa."},
{"tiempo","El tiempo permite ordenar la duracion de eventos."},
{"espacio tiempo","En fisica, espacio y tiempo forman parte del espacio-tiempo."},
{"matematicas","Las matematicas estudian numeros, formas y relaciones."},
{"fraccion","Una fraccion representa partes de un todo."},
{"porcentaje","Un porcentaje expresa una cantidad sobre 100."},
{"ecuacion","Una ecuacion contiene una igualdad y valores desconocidos."},
{"pitagoras","En un triangulo rectangulo: a2+b2=c2."},
{"area","El area mide una superficie."},
{"perimetro","El perimetro mide el borde de una figura."},
{"pi","Pi es aproximadamente 3.14159."},
{"numero primo","Un primo tiene exactamente dos divisores positivos."},
{"algebra","El algebra usa letras y simbolos para representar cantidades."},
{"ingles","Puedo ayudarte con palabras y frases en ingles."},
{"hello","Hello significa hola."},
{"goodbye","Goodbye significa adios."},
{"computer","Computer significa computadora."},
{"water","Water significa agua."},
{"book","Book significa libro."},
{"porque","Puedo explicar el tema paso a paso."},
{"por que","Dime el tema y te explico el motivo."},
{"explica","Claro. Dime que quieres que explique."},
{"otro dato","Dato: un pulpo tiene tres corazones."},
{"dato","Dato: la luz del Sol tarda unos 8 minutos en llegar."},
{"sueño","Los sueños pueden incluir imagenes, sonidos y emociones."},
{"miedo","El miedo es una respuesta ante algo percibido como amenaza."},
{"feliz","¡Que bueno! 😎"},
{"triste","Aqui estoy bro. Podemos hablar."},
{"no se","No pasa nada. Podemos descubrirlo juntos."},
{"ayuda","Claro. Preguntame lo que quieras."}
};

int total_preguntas=sizeof(banco)/sizeof(banco[0]);
int total_temas=sizeof(temas)/sizeof(temas[0]);
int puntuacion=0,aciertos=0,preguntas_hechas=0;
Keyboard *kbd=NULL;
PrintConsole consolaTop,consolaBottom;
char historial[MAX_HISTORIAL][64];
int num_historial=0;
char ultimo_tema[64]="";

void seleccionar_top(void){consoleSelect(&consolaTop);}
void seleccionar_bottom(void){consoleSelect(&consolaBottom);}

void a_minusculas(char *s){
 int i;
 for(i=0;s[i];i++)s[i]=(char)tolower((unsigned char)s[i]);
}

int contiene(const char *a,const char *b){
 char x[MAX_INPUT];
 strncpy(x,a,MAX_INPUT-1);x[MAX_INPUT-1]=0;
 a_minusculas(x);
 return strstr(x,b)!=NULL;
}

void agregar_historial(const char *quien,const char *msg){
 if(num_historial<MAX_HISTORIAL){
  snprintf(historial[num_historial],64,"%s: %.55s",quien,msg);
  num_historial++;
 }else{
  int i;
  for(i=1;i<MAX_HISTORIAL;i++)strcpy(historial[i-1],historial[i]);
  snprintf(historial[MAX_HISTORIAL-1],64,"%s: %.55s",quien,msg);
 }
}

void mostrar_historial(void){
 int i;
 seleccionar_top();
 printf("\x1b[2J");
 printf("====== DSi IA ======\n\n");
 for(i=0;i<num_historial;i++)printf("%s\n\n",historial[i]);
}

void iniciar_teclado(void){
 kbd=keyboardInit(NULL,3,BgType_Text4bpp,BgSize_T_256x512,20,0,false,true);
 keyboardShow();
 seleccionar_bottom();
}

void ocultar_teclado(void){
 keyboardHide();
}

int leer_texto(char *out,int max){
 int pos=0;
 out[0]=0;
 seleccionar_bottom();
 printf("\x1b[2J");
 printf("Escribe tu mensaje:\n\n");
 printf("Tu: ");
 while(1){
  swiWaitForVBlank();
  scanKeys();
  int key=keyboardUpdate();
  if(key>0){
   if(key==DVK_ENTER||key=='\n'){
    out[pos]=0;
    return 1;
   }
   if((key==DVK_BACKSPACE||key==8)&&pos>0){
    pos--;out[pos]=0;
   }else if(key>=32&&key<127&&pos<max-1){
    out[pos++]=(char)key;
    out[pos]=0;
   }
   seleccionar_bottom();
   printf("\x1b[2J");
   printf("Escribe tu mensaje:\n\nTu: %s",out);
  }
  if(keysDown()&KEY_B)return 0;
 }
}

int calcular_expresion(const char *s,int *ok){
 int a,b;char op;
 *ok=0;
 if(sscanf(s,"%d %c %d",&a,&op,&b)==3){
  *ok=1;
  if(op=='+')return a+b;
  if(op=='-')return a-b;
  if(op=='*'||op=='x')return a*b;
  if(op=='/'&&b)return a/b;
  *ok=0;
 }
 return 0;
}

void responder_charla(const char *msg){
 char m[MAX_INPUT],resp[64];
 int i,ok,r;
 strncpy(m,msg,MAX_INPUT-1);m[MAX_INPUT-1]=0;
 a_minusculas(m);

 if(contiene(m,"cuanto es")||isdigit((unsigned char)m[0])){
  r=calcular_expresion(m,&ok);
  if(ok){
   sprintf(resp,"Resultado: %d",r);
   agregar_historial("DSi IA",resp);
   mostrar_historial();
   return;
  }
 }

 for(i=0;i<total_temas;i++){
  if(strstr(m,temas[i].clave)){
   strncpy(ultimo_tema,temas[i].clave,63);
   ultimo_tema[63]=0;
   agregar_historial("DSi IA",temas[i].respuesta);
   mostrar_historial();
   return;
  }
 }

 if(contiene(m,"por que")||contiene(m,"porque")){
  if(ultimo_tema[0]){
   sprintf(resp,"Sobre %s: puedo explicarlo mas si quieres.",ultimo_tema);
   agregar_historial("DSi IA",resp);
  }else agregar_historial("DSi IA","Dime el tema y te explico por que.");
 }else if(contiene(m,"explica")||contiene(m,"mas")){
  agregar_historial("DSi IA","Claro. Preguntame algo mas concreto.");
 }else if(contiene(m,"otro dato")){
  agregar_historial("DSi IA","Dato: los pulpos tienen tres corazones.");
 }else{
  agregar_historial("DSi IA","Interesante 😎. Cuéntame mas.");
 }
 mostrar_historial();
}

int es_correcta(const char *respuesta,const char *correcta){
 char a[64],b[64];
 strncpy(a,respuesta,63);a[63]=0;
 strncpy(b,correcta,63);b[63]=0;
 a_minusculas(a);a_minusculas(b);
 return strcmp(a,b)==0||strstr(a,b)!=NULL;
}

void jugar(const char *categoria,int leccion){
 int idx[100],n=0,i,j,t,ok;
 char respuesta[MAX_INPUT];
 for(i=0;i<total_preguntas;i++)
  if(!categoria||strcmp(banco[i].categoria,categoria)==0)idx[n++]=i;
 for(i=n-1;i>0;i--){
  j=rand()%(i+1);t=idx[i];idx[i]=idx[j];idx[j]=t;
 }
 if(n==0)return;
 iniciar_teclado();
 puntuacion=0;aciertos=0;preguntas_hechas=0;
 for(i=0;i<n&&i<7;i++){
  seleccionar_top();
  printf("\x1b[2J");
  printf("Pregunta %d/7\n\n%s\n\n",i+1,banco[idx[i]].pregunta);
  if(leer_texto(respuesta,MAX_INPUT)==0)break;
  seleccionar_top();
  printf("\x1b[2J");
  preguntas_hechas++;
  if(es_correcta(respuesta,banco[idx[i]].respuesta)){
   aciertos++;puntuacion+=10;
   printf("¡Correcto!\n\n");
  }else printf("Incorrecto.\nRespuesta: %s\n\n",banco[idx[i]].respuesta);
  if(leccion)printf("%s\n",banco[idx[i]].explicacion);
  printf("\nPulsa A...");
  while(!(keysDown()&KEY_A)){swiWaitForVBlank();scanKeys();}
 }
 ocultar_teclado();
 seleccionar_top();
 printf("\x1b[2J");
 printf("FIN\n\nAciertos: %d/%d\nPuntos: %d\n",aciertos,preguntas_hechas,puntuacion);
 printf("\nPulsa A...");
 while(!(keysDown()&KEY_A)){swiWaitForVBlank();scanKeys();}
}

void modo_charla(void){
 char msg[MAX_INPUT],tmp[MAX_INPUT];
 num_historial=0;
 iniciar_teclado();
 agregar_historial("DSi IA","Hola bro 😎 Escribe algo.");
 while(1){
  mostrar_historial();
  seleccionar_bottom();
  printf("\x1b[2J");
  printf("Escribe tu mensaje:\n\n");
  if(!leer_texto(msg,MAX_INPUT))break;
  if(msg[0]==0)continue;
  strncpy(tmp,msg,MAX_INPUT-1);tmp[MAX_INPUT-1]=0;
  a_minusculas(tmp);
  if(strstr(tmp,"salir")||strstr(tmp,"menu"))break;
  agregar_historial("Tu",msg);
  responder_charla(msg);
 }
 ocultar_teclado();
 seleccionar_top();
}

void menu_principal(void){
 seleccionar_top();
 printf("\x1b[2J");
 printf("====== DSi IA ======\n\n");
 printf("A: Quiz\n");
 printf("B: Leccion\n");
 printf("X: Matematicas\n");
 printf("Y: Espanol\n");
 printf("L: Quimica\n");
 printf("R: General\n");
 printf("SELECT: Chat\n");
 printf("START: Salir\n");
}

int main(void){
 videoSetMode(MODE_0_2D);
 videoSetModeSub(MODE_0_2D);
 vramSetBankA(VRAM_A_MAIN_BG);
 vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(&consolaTop,0,BgType_Text4bpp,BgSize_T_256x256,22,3,true,true);
 consoleInit(&consolaBottom,0,BgType_Text4bpp,BgSize_T_256x256,22,3,false,true);

 srand(2026);
 seleccionar_top();

 while(1){
  menu_principal();
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
   if(k&KEY_SELECT){modo_charla();break;}
   if(k&KEY_START){
    printf("\x1b[2J\nHasta luego!\nGracias por usar DSi IA.\n");
    while(1)swiWaitForVBlank();
   }
  }
 }
 return 0;
}
