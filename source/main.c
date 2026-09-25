#include <nds.h>
#include <fat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define IN 80
#define HIST 12
#define ARCH "DSiIA_KNOWLEDGE.DAT"

typedef struct{const char*q,*r,*e,*c;}P;

Keyboard*kbd;
PrintConsole top,bot;

char hist[HIST][96];
int nh,pts,ok,done,sd;

char cexpr[96];
int cpos;

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
 snprintf(hist[nh++],96,"%s|%s",a,b);
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
 s[0]=0;

 while(1){
  swiWaitForVBlank();
  scanKeys();

  int k=keyboardUpdate();

  if(k>0){
   if(k==DVK_ENTER||k=='\n'){
    s[p]=0;
    return 1;
   }

   if((k==DVK_BACKSPACE||k==8)&&p){
    s[--p]=0;
   }
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
void bubble(const char*who,const char*msg,int right){
 int len=strlen(msg);
 int i=0;

 if(right)
  printf("                    %s\n",who);
 else
  printf("%s\n",who);

 printf("+--------------------------+\n");

 while(i<len){
  printf("| %-26.26s |\n",msg+i);
  i+=26;
 }

 if(len==0)
  printf("| %-26s |\n","");

 printf("+--------------------------+\n");
}

void showh(){
 int start=nh>4?nh-4:0;

 topc();
 printf("\x1b[2J");
 printf("          DSi IA CHAT\n");
 printf("------------------------------\n");

 for(int i=start;i<nh;i++){
  char x[96],*p;
  strcpy(x,hist[i]);

  p=strchr(x,'|');

  if(p){
   *p=0;
   p++;

   if(!strcmp(x,"TU"))
    bubble("TU",p,1);
   else
    bubble("DSi IA",p,0);
  }
 }
}

void chatInput(const char*s){
 botc();
 printf("\x1b[2J");

 printf("================================\n");
 printf(" Escribir:\n");
 printf("> %-28.28s\n",s);
 printf("================================\n");
 printf("   ENTER = ENVIAR   B = SALIR\n\n");
}

int chatRead(char*s,int max){
 int p=0;
 s[0]=0;

 chatInput(s);

 while(1){
  swiWaitForVBlank();
  scanKeys();

  int k=keyboardUpdate();

  if(k>0){
   if(k==DVK_ENTER||k=='\n'){
    s[p]=0;
    return 1;
   }

   if((k==DVK_BACKSPACE||k==8)&&p){
    s[--p]=0;
   }
   else if(k>=32&&k<127&&p<max-1){
    s[p++]=k;
    s[p]=0;
   }

   chatInput(s);
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

 return 0;
}
P b[]={
{"Cuanto es 2+2","4","2+2 es igual a 4.","Matematicas"},
{"Que es una fraccion","Una fraccion representa una parte de un todo.","El numerador esta arriba y el denominador abajo.","Matematicas"},
{"Que es un numero primo","Es un numero mayor que 1 que solo tiene como divisores positivos al 1 y a si mismo.","Ejemplos: 2, 3, 5, 7 y 11.","Matematicas"},
{"Que es el numero pi","Pi es la constante que relaciona la circunferencia con su diametro. Aproximadamente vale 3.14159265.","Se representa con la letra griega pi.","Matematicas"},
{"Teorema de Pitagoras","En un triangulo rectangulo, a2+b2=c2.","c es la hipotenusa.","Matematicas"},
{"Que es un porcentaje","Es una forma de expresar una cantidad como una parte de cien.","25 por ciento equivale a 25 de cada 100.","Matematicas"},
{"Que es un promedio","Es la suma de varios valores dividida entre la cantidad de valores.","Tambien se llama media aritmetica.","Matematicas"},
{"Que es una ecuacion","Es una igualdad matematica que contiene una o mas cantidades desconocidas.","Resolverla significa encontrar los valores que hacen verdadera la igualdad.","Matematicas"},
{"Que es el area","Es la medida de la superficie de una figura plana.","Se expresa en unidades cuadradas.","Matematicas"},
{"Que es el perimetro","Es la longitud total del borde de una figura.","Se obtiene sumando las longitudes de sus lados.","Matematicas"},
{"Que es la probabilidad","Es una medida de que tan posible es que ocurra un evento.","Puede expresarse entre 0 y 1 o entre 0 y 100 por ciento.","Matematicas"},
{"Que es una potencia","Es una multiplicacion repetida de un numero por si mismo.","Por ejemplo, 2 elevado a 3 es 8.","Matematicas"},
{"Que es una raiz cuadrada","Es un numero que multiplicado por si mismo produce otro numero.","La raiz cuadrada de 25 es 5.","Matematicas"},
{"Que es un angulo","Es la abertura formada por dos semirrectas que parten de un mismo punto.","Se mide normalmente en grados.","Matematicas"},
{"Que es un triangulo","Es un poligono de tres lados.","La suma de sus angulos interiores es 180 grados.","Matematicas"},
{"Que es la fisica","Es la ciencia que estudia la materia, la energia, el movimiento y sus interacciones.","Busca describir la naturaleza mediante modelos y leyes.","Fisica"},
{"Que es una fuerza","Es una interaccion capaz de cambiar el movimiento o deformar un objeto.","Se mide en newtons.","Fisica"},
{"Que es la gravedad","Es una interaccion por la cual los cuerpos con masa se atraen.","En la Tierra produce la aceleracion de la gravedad.","Fisica"},
{"Que es la velocidad","Es la relacion entre desplazamiento y tiempo.","Indica que tan rapido cambia la posicion de un objeto.","Fisica"},
{"Que es la aceleracion","Es el cambio de velocidad con respecto al tiempo.","Puede aumentar, disminuir o cambiar la direccion del movimiento.","Fisica"},
{"Primera ley de Newton","Un objeto mantiene su estado de movimiento si la fuerza neta sobre el es cero.","Tambien se conoce como ley de la inercia.","Fisica"},
{"Segunda ley de Newton","La fuerza neta es igual a la masa multiplicada por la aceleracion.","Se expresa como F=m*a.","Fisica"},
{"Tercera ley de Newton","Las interacciones entre cuerpos producen fuerzas de igual magnitud y sentido contrario.","Es conocida como accion y reaccion.","Fisica"},
{"Que es la energia","Es una magnitud asociada con la capacidad de producir cambios o realizar trabajo.","Puede presentarse de muchas formas.","Fisica"},
{"Que es la energia cinetica","Es la energia asociada al movimiento de un objeto.","Depende de su masa y de su velocidad.","Fisica"},
{"Que es la energia potencial","Es energia asociada con la posicion o configuracion de un sistema.","La energia potencial gravitatoria depende de la altura.","Fisica"},
{"Que es la luz","Es radiacion electromagnetica que puede propagarse por el espacio.","La luz visible es una pequena parte del espectro electromagnetico.","Fisica"},
{"Que es el sonido","Es una onda mecanica producida por vibraciones y que necesita un medio material para propagarse.","No se propaga por el vacio.","Fisica"},
{"Que es la electricidad","Es el conjunto de fenomenos relacionados con las cargas electricas.","Puede producir corrientes, campos y efectos energeticos.","Fisica"},
{"Que es el voltaje","Es una diferencia de potencial electrico entre dos puntos.","Se mide en voltios.","Fisica"},
{"Que es la corriente electrica","Es el flujo de carga electrica a traves de un material.","Se mide en amperios.","Fisica"},
{"Que es la resistencia electrica","Es la oposicion al paso de corriente electrica.","Se mide en ohmios.","Fisica"},
{"Ley de Ohm","La relacion basica es V=I*R.","V es voltaje, I corriente y R resistencia.","Fisica"},
{"Que es un atomo","Es la unidad fundamental de un elemento quimico que conserva sus propiedades quimicas.","Tiene un nucleo y electrones alrededor.","Quimica"},
{"Que es un proton","Es una particula con carga electrica positiva que se encuentra en el nucleo atomico.","El numero de protones determina el elemento.","Quimica"},
{"Que es un neutron","Es una particula sin carga electrica neta que se encuentra en el nucleo atomico.","Contribuye a la masa del atomo.","Quimica"},
{"Que es un electron","Es una particula con carga negativa que forma parte de los atomos.","Los electrones participan en los enlaces quimicos.","Quimica"},
{"Que es un elemento quimico","Es una sustancia cuyos atomos tienen el mismo numero de protones.","Cada elemento tiene un numero atomico propio.","Quimica"},
{"Que es la tabla periodica","Es una organizacion de los elementos quimicos segun sus propiedades y numero atomico.","Las filas se llaman periodos y las columnas grupos.","Quimica"},
{"Que es una molecula","Es una agrupacion de atomos unidos mediante enlaces quimicos.","Puede estar formada por atomos iguales o diferentes.","Quimica"},
{"Que es un enlace quimico","Es una interaccion que mantiene unidos atomos o iones en una sustancia.","Entre los tipos estan ionico, covalente y metalico.","Quimica"},
{"Que es un acido","Es una sustancia que puede donar protones en ciertos modelos quimicos.","Las propiedades acidas dependen del medio y de la sustancia.","Quimica"},
{"Que es una base","Es una sustancia que puede aceptar protones o producir iones hidroxido segun el modelo utilizado.","Las bases pueden reaccionar con acidos.","Quimica"},
{"Que es el pH","Es una medida relacionada con la actividad de los iones hidrogeno de una disolucion.","La escala se usa habitualmente para describir acidez y basicidad.","Quimica"},
{"Que es una reaccion quimica","Es un proceso donde unas sustancias se transforman en otras mediante reorganizacion de atomos.","La materia se conserva en las reacciones ordinarias.","Quimica"},
{"Que es un catalizador","Es una sustancia que modifica la velocidad de una reaccion sin consumirse de forma neta en ella.","Reduce la barrera energetica de la reaccion.","Quimica"},
{"Que es una mezcla","Es una combinacion de sustancias donde cada componente conserva su identidad quimica.","Puede ser homogenea o heterogenea.","Quimica"},
{"Que es una disolucion","Es una mezcla homogenea formada por uno o mas solutos distribuidos en un disolvente.","Por ejemplo, sal disuelta en agua.","Quimica"},
{"Que es un mol","Es una unidad del SI para cantidad de sustancia.","Un mol contiene exactamente 6.02214076 por 10 elevado a 23 entidades elementales.","Quimica"},
{"Que es una celula","Es la unidad estructural y funcional basica de los seres vivos.","Los organismos pueden tener una o muchas celulas.","Biologia"},
{"Que es el ADN","Es una molecula que almacena informacion genetica en los seres vivos y algunos virus.","Su estructura contiene dos cadenas complementarias.","Biologia"},
{"Que es un gen","Es una region del material genetico que contiene informacion funcional heredable.","Muchos genes participan en la produccion de ARN o proteinas.","Biologia"},
{"Que es un cromosoma","Es una estructura organizada de ADN asociada con proteinas.","Los cromosomas contienen genes.","Biologia"},
{"Que es la fotosintesis","Es el proceso mediante el cual plantas, algas y algunos microorganismos transforman energia luminosa en energia quimica.","En la fotosintesis oxigenica se libera oxigeno.","Biologia"},
{"Que es la respiracion celular","Es un conjunto de procesos mediante los cuales las celulas obtienen energia util a partir de moleculas organicas.","En muchos organismos utiliza oxigeno.","Biologia"},
{"Que es la mitosis","Es un proceso de division celular que normalmente produce dos celulas hijas con informacion genetica equivalente a la de la celula inicial.","Es importante para crecimiento y reparacion.","Biologia"},
{"Que es la meiosis","Es una division celular especializada que reduce a la mitad el numero de cromosomas.","Participa en la formacion de gametos en muchos organismos.","Biologia"},
{"Que es un ecosistema","Es un sistema formado por organismos vivos y los componentes no vivos de su ambiente junto con sus interacciones.","Incluye flujos de energia y ciclos de materia.","Biologia"},
{"Que es una cadena alimentaria","Es una representacion simplificada de como la energia y la materia pasan entre organismos mediante relaciones de alimentacion.","Comienza frecuentemente con productores.","Biologia"},
{"Que es la evolucion","Es el cambio de las caracteristicas heredables de poblaciones a traves de generaciones.","Es uno de los conceptos centrales de la biologia.","Biologia"},
{"Que es la seleccion natural","Es un proceso mediante el cual las diferencias heredables que afectan la supervivencia o reproduccion pueden cambiar su frecuencia en una poblacion.","Es un mecanismo de evolucion.","Biologia"},
{"Que es una especie","En biologia, es una categoria utilizada para agrupar organismos con caracteristicas y relaciones reproductivas determinadas por el concepto utilizado.","La definicion puede variar segun el grupo biologico.","Biologia"},
{"Que es una bacteria","Es un organismo unicelular procariota.","Existen bacterias beneficiosas, neutras y causantes de enfermedades.","Biologia"},
{"Que es un virus","Es un agente infeccioso compuesto por material genetico y estructuras que le permiten entrar en celulas y utilizar su maquinaria.","No se considera una celula.","Biologia"},
{"Que fue la escritura","Fue un conjunto de sistemas desarrollados para representar informacion mediante signos.","Las primeras escrituras conocidas aparecieron en sociedades antiguas de diferentes regiones.","Historia"},
{"Que fue Mesopotamia","Fue una region historica situada entre los rios Tigris y Eufrates.","Alli se desarrollaron importantes ciudades y sistemas de escritura.","Historia"},
{"Que fue el antiguo Egipto","Fue una civilizacion desarrollada principalmente a lo largo del rio Nilo.","Es conocida por sus monumentos, escritura jeroglifica y larga historia politica.","Historia"},
{"Que fue la antigua Grecia","Fue una civilizacion formada por numerosas ciudades-estado y comunidades del Mediterraneo.","Influyo profundamente en filosofia, ciencia, arte y politica.","Historia"},
{"Que fue Roma","Fue una civilizacion que paso de monarquia a republica y posteriormente a imperio.","Su legado incluye derecho, arquitectura, lenguas y sistemas administrativos.","Historia"},
{"Que fue la Edad Media","Es el nombre utilizado para un amplio periodo de la historia europea situado entre la Antiguedad y la Edad Moderna.","El periodo abarco aproximadamente un milenio, aunque sus limites varian segun el historiador.","Historia"},
{"Que fue el Renacimiento","Fue un amplio movimiento cultural europeo asociado con renovado interes por las culturas clasicas, las artes, la ciencia y el humanismo.","Se desarrollo principalmente entre los siglos XV y XVI.","Historia"},
{"Quien invento la imprenta moderna","Johannes Gutenberg desarrollo en Europa en el siglo XV una imprenta de tipos moviles que facilito enormemente la reproduccion de textos.","Ya existian tecnicas de impresion anteriores en otras regiones.","Historia"},
{"Que fue la Revolucion Industrial","Fue un proceso de transformacion economica y tecnologica marcado por mecanizacion, nuevas fuentes de energia y cambios en la produccion.","Comenzo en Gran Bretana durante el siglo XVIII y se extendio posteriormente.","Historia"},
{"Cuando inicio la Independencia de Mexico","El proceso inicio en 1810 con el levantamiento encabezado por Miguel Hidalgo y otros insurgentes.","La independencia se consumo en 1821.","Historia"},
{"Que fue la Constitucion Mexicana de 1917","Es la constitucion promulgada el 5 de febrero de 1917 durante el proceso revolucionario mexicano.","Establecio derechos y principios politicos y sociales fundamentales.","Historia"},
{"Que fue la Primera Guerra Mundial","Fue un conflicto internacional desarrollado principalmente entre 1914 y 1918.","Involucro a numerosas potencias y produjo enormes perdidas humanas y cambios geopoliticos.","Historia"},
{"Que fue la Segunda Guerra Mundial","Fue un conflicto global desarrollado principalmente entre 1939 y 1945.","Involucro a gran parte de las principales potencias del mundo.","Historia"},
{"Que es la ONU","La Organizacion de las Naciones Unidas fue creada en 1945 para fomentar la cooperacion internacional y trabajar en asuntos de paz y seguridad, entre otros objetivos.","Actualmente agrupa a la mayoria de los Estados del mundo.","Historia"},
{"Que fue la Guerra Fria","Fue un periodo de rivalidad politica, militar, economica y tecnologica entre Estados Unidos, la Union Sovietica y sus respectivos bloques.","Se desarrollo principalmente despues de la Segunda Guerra Mundial hasta la disolucion de la URSS.","Historia"},
{"Que es una computadora","Es una maquina capaz de procesar informacion mediante instrucciones.","Puede ejecutar programas y almacenar datos.","Computacion"},
{"Que es un procesador","Es el componente que ejecuta instrucciones y realiza operaciones de un sistema informatico.","Tambien se conoce como CPU.","Computacion"},
{"Que es la RAM","Es memoria de acceso rapido utilizada temporalmente por los programas y el sistema operativo.","Su contenido normalmente se pierde al apagar el equipo.","Computacion"},
{"Que es un SSD","Es un dispositivo de almacenamiento que utiliza memoria no volatil, normalmente memoria flash.","No tiene platos mecanicos como un disco duro tradicional.","Computacion"},
{"Que es un disco duro","Es un dispositivo de almacenamiento magnetico que utiliza platos giratorios y cabezales.","Los discos duros tradicionales suelen ser mas lentos que los SSD.","Computacion"},
{"Que es un sistema operativo","Es el software principal que administra recursos del hardware y proporciona servicios para los programas.","Ejemplos son Windows, Linux, Android y otros.","Computacion"},
{"Que es un algoritmo","Es un conjunto de pasos definidos para resolver un problema o realizar una tarea.","Los algoritmos son fundamentales en programacion.","Computacion"},
{"Que es un compilador","Es un programa que transforma codigo fuente de un lenguaje de programacion en otra representacion ejecutable o intermedia.","Los compiladores realizan diferentes etapas de analisis y traduccion.","Computacion"},
{"Que es HTML","Es un lenguaje de marcado utilizado para estructurar documentos de la web.","HTML no es un lenguaje de programacion general.","Computacion"},
{"Que es una GPU","Es un procesador especializado en operaciones paralelas, especialmente util para graficos y ciertas cargas de computacion.","Las GPU modernas tambien se usan en inteligencia artificial.","Computacion"},
{"Que es inteligencia artificial","Es un campo de la informatica que estudia y desarrolla sistemas capaces de realizar tareas que requieren capacidades asociadas con la inteligencia.","Incluye aprendizaje automatico, razonamiento y percepcion.","Computacion"},
{"Que es aprendizaje automatico","Es una rama de la inteligencia artificial donde los sistemas aprenden patrones a partir de datos para realizar tareas.","Puede utilizar modelos estadisticos y redes neuronales.","Computacion"},
{"Que es internet","Es una red mundial de redes informaticas que utilizan protocolos comunes para comunicarse.","La web es uno de los servicios que funcionan sobre internet.","Computacion"},
{"Que es una base de datos","Es un sistema organizado para almacenar, consultar y administrar informacion.","Puede utilizar diferentes modelos y tecnologias.","Computacion"},
{"Que es un pixel","Es un elemento de imagen utilizado para representar informacion visual en una imagen digital o pantalla.","Las pantallas forman imagenes mediante muchos pixeles.","Computacion"},
{"Que es el codigo binario","Es una representacion basada en dos simbolos, normalmente 0 y 1.","Los sistemas digitales utilizan representaciones binarias de diversas formas.","Computacion"},
{"Que es un navegador web","Es un programa utilizado para acceder e interpretar recursos de la web.","Ejemplos conocidos son Firefox, Chrome, Edge y Safari.","Computacion"},
{"Que es un videojuego","Es un programa interactivo que presenta reglas, contenido y sistemas con los que el jugador puede interactuar.","Puede ejecutarse en consolas, computadoras, telefonos y otros dispositivos.","Cultura"},
{"Que es un libro","Es una obra escrita, impresa o digital que contiene informacion, literatura u otro contenido organizado.","Los libros pueden pertenecer a muchos generos y disciplinas.","Cultura"},
{"Que es una enciclopedia","Es una obra de referencia que organiza conocimientos sobre temas diversos o sobre un campo especifico.","Puede existir en formato impreso o digital.","Cultura"},
{"Que es el metodo cientifico","Es una forma sistematica de investigar fenomenos mediante observacion, preguntas, hipotesis, pruebas, analisis y comunicacion de resultados.","Su aplicacion concreta varia segun la disciplina.","Ciencia"},
{"Que es una hipotesis","Es una explicacion propuesta que puede ser sometida a pruebas o contrastacion.","Una hipotesis cientifica debe poder evaluarse mediante evidencia.","Ciencia"},
{"Que es una teoria cientifica","Es una explicacion amplia y fundamentada de fenomenos naturales respaldada por multiples lineas de evidencia.","En ciencia, teoria no significa simplemente una ocurrencia.","Ciencia"},
{"Que es una ley cientifica","Es una descripcion general de un patron observado en la naturaleza.","Una ley y una teoria cumplen funciones diferentes.","Ciencia"},
{"Que es el agua","Es un compuesto quimico formado por hidrogeno y oxigeno, con formula H2O.","Es esencial para todos los organismos conocidos y participa en numerosos procesos naturales.","Ciencia"},
{"Que es el oxigeno","Es un elemento quimico de numero atomico 8.","Forma aproximadamente una quinta parte del aire seco de la atmosfera terrestre por volumen.","Ciencia"},
{"Que es el carbono","Es un elemento quimico de numero atomico 6.","Es fundamental para la quimica organica y para la vida conocida.","Ciencia"},
{"Que es la materia","Es todo aquello que posee masa y ocupa un lugar en el espacio, segun la definicion escolar habitual.","Puede presentarse en diferentes estados y formas.","Ciencia"},
{"Que es el espacio","Es la extension en la que se encuentran objetos y ocurren fenomenos fisicos.","En fisica moderna se estudia junto con el tiempo como espacio-tiempo.","Ciencia"},
{"Que es el tiempo","Es una magnitud utilizada para ordenar acontecimientos y describir su duracion.","Forma parte del espacio-tiempo en la relatividad.","Ciencia"},
{"Que es la democracia","Es una forma de organizacion politica en la que el poder se relaciona con la participacion de la ciudadania y mecanismos de decision colectiva.","Existen diferentes modelos democraticos.","Cultura"},
{"Que es una constitucion","Es la norma fundamental de un Estado que establece principios de organizacion del poder y derechos, segun el sistema juridico correspondiente.","Su contenido varia entre paises.","Cultura"},
{"Que es una biblioteca","Es una institucion o espacio que conserva, organiza y facilita el acceso a colecciones de informacion y obras.","Muchas bibliotecas tambien ofrecen servicios digitales.","Cultura"},
{"Que es una universidad","Es una institucion de educacion superior dedicada a la ensenanza, investigacion y otras actividades academicas.","Sus estructuras y funciones varian segun el pais.","Cultura"},
{"Que es una fotografia","Es una imagen producida mediante el registro de luz sobre un soporte fisico o digital.","Las camaras modernas utilizan sensores electronicos.","Cultura"},
{"Que es un mapa","Es una representacion simbolica y generalmente reducida de una parte de la superficie terrestre u otro espacio.","Puede mostrar informacion geografica especifica.","Geografia"},
{"Que es un continente","Es una gran extension continua de tierra firme utilizada como unidad geografica.","La cantidad de continentes depende del modelo geografico.","Geografia"},
{"Que es un rio","Es una corriente natural de agua que fluye hacia otro cuerpo de agua o hacia una zona de acumulacion.","Puede tener afluentes y formar cuencas hidrograficas.","Geografia"},
{"Que es una montana","Es una elevacion natural importante del terreno.","Las montanas pueden originarse por procesos tectonicos, volcanicos y erosivos.","Geografia"},
{"Que es el clima","Es el conjunto de condiciones atmosfericas estadisticamente caracteristicas de una region durante periodos prolongados.","No es lo mismo que el estado del tiempo de un solo dia.","Geografia"},
};

int total=sizeof(b)/sizeof(b[0]);
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
 char q[64],x[128],m[80];

 int found=0;

 if(!sd)return 0;

 strncpy(m,msg,79);
 m[79]=0;
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

int stopword(const char*w){
 const char*s[]={
  "que","como","cual","cuales","una","uno","los","las",
  "del","para","por","con","una","ese","esa","es","son",
  "de","el","la","un","y","o","en","se","me","te"
 };

 int n=sizeof(s)/sizeof(s[0]);

 for(int i=0;i<n;i++)
  if(!strcmp(w,s[i]))return 1;

 return 0;
}

int scoreq(const char*m,const char*q){
 char a[80],b[80];
 int score=0;

 strncpy(a,m,79);
 a[79]=0;
 normal(a);

 strncpy(b,q,79);
 b[79]=0;
 normal(b);

 char*pa=strtok(a," ");
 while(pa){

  if(strlen(pa)>=3&&!stopword(pa)){
   char*pb=strtok(b," ");
   while(pb){
    if(strlen(pb)>=3&&!stopword(pb)&&!strcmp(pa,pb)){
     score++;
     break;
    }
    pb=strtok(NULL," ");
   }

   strncpy(b,q,79);
   b[79]=0;
   normal(b);
  }

  pa=strtok(NULL," ");
 }

 return score;
}

void learn(){
 char q[80],r[128];

 topc();
 printf("\x1b[2J");
 printf("========== APRENDER ==========\n\n");
 printf("Escribe la pregunta.\n");

 if(!chatRead(q,80)||!q[0])return;

 topc();
 printf("\x1b[2J");
 printf("Ahora escribe la respuesta.\n");

 if(!chatRead(r,128)||!r[0])return;

 normal(q);
 saveknow(q,r);

 addh("DSi IA","Aprendido y guardado en la SD.");
 showh();

 topc();
 printf("\nPulsa A...");
 waitA();
}

void answer(const char*msg){
 char m[IN],r[128],q[64];
 int best=-1,bs=0,n;

 if(findknow(msg,r)){
  addh("DSi IA",r);
  showh();
  return;
 }

 strncpy(m,msg,IN-1);
 m[IN-1]=0;
 normal(m);

 if(strstr(m,"hola")||strstr(m,"buenas")||
    strstr(m,"hey")||strstr(m,"hello")){
  strcpy(r,"Hola! Soy DSi IA. Puedes preguntarme sobre ciencia, historia, matematicas, computacion y muchos temas mas.");
  addh("DSi IA",r);
  showh();
  return;
 }

 if(strstr(m,"gracias")){
  strcpy(r,"De nada! Sigue preguntando.");
  addh("DSi IA",r);
  showh();
  return;
 }

 if(strstr(m,"quien eres")){
  strcpy(r,"Soy DSi IA, un asistente educativo creado para Nintendo DSi. Tengo conocimientos guardados y tambien puedo aprender respuestas nuevas.");
  addh("DSi IA",r);
  showh();
  return;
 }

 if(strstr(m,"que puedes hacer")){
  strcpy(r,"Puedo responder preguntas de muchas materias, hacer calculos, guardar conocimientos en la SD y ayudarte a estudiar.");
  addh("DSi IA",r);
  showh();
  return;
 }

 for(int i=0;i<total;i++){
  int s=scoreq(m,b[i].q);

  if(strstr(m,b[i].q))s+=5;

  if(s>bs){
   bs=s;
   best=i;
  }
 }

 if(best>=0&&bs>=1){
  strncpy(r,b[best].r,127);
  r[127]=0;
 }
 else{
  strcpy(r,"No tengo una respuesta exacta para eso. Puedes decir APRENDER para ensenarme una respuesta y guardarla en la SD.");
 }

 if((strstr(m,"cuanto es")||strstr(m,"calcula"))&&
   (strchr(m,'+')||strchr(m,'-')||strchr(m,'*')||
    strchr(m,'x')||strchr(m,'/')||strchr(m,'^'))){

  /* El calculo se maneja abajo con la calculadora y el parser. */
 }

 addh("DSi IA",r);
 showh();
}
const char*cptr;
int cerr;

double cexprParse(void);

double cFactor(){
 double v;
 char*end;

 while(*cptr==' ')cptr++;

 if(*cptr=='+'){
  cptr++;
  return cFactor();
 }

 if(*cptr=='-'){
  cptr++;
  return -cFactor();
 }

 if(*cptr=='('){
  cptr++;

  v=cexprParse();

  while(*cptr==' ')cptr++;

  if(*cptr!=')'){
   cerr=1;
   return 0;
  }

  cptr++;
  return v;
 }

 v=strtod(cptr,&end);

 if(end==cptr){
  cerr=1;
  return 0;
 }

 cptr=end;
 return v;
}

double cTerm(){
 double v= cFactor();

 while(!cerr){
  while(*cptr==' ')cptr++;

  if(*cptr=='*'){
   cptr++;
   v*=cFactor();
  }
  else if(*cptr=='/'){
   double z;
   cptr++;
   z=cFactor();

   if(z==0){
    cerr=1;
    return 0;
   }

   v/=z;
  }
  else break;
 }

 return v;
}

double cexprParse(){
 double v=cTerm();

 while(!cerr){
  while(*cptr==' ')cptr++;

  if(*cptr=='+'){
   cptr++;
   v+=cTerm();
  }
  else if(*cptr=='-'){
   cptr++;
   v-=cTerm();
  }
  else break;
 }

 return v;
}

int calcEval(double*out){
 cptr=cexpr;
 cerr=0;

 *out=cexprParse();

 while(*cptr==' ')cptr++;

 if(cerr||*cptr)return 0;

 return 1;
}

int cadd(char c){
 if(cpos>=95)return 0;

 cexpr[cpos++]=c;
 cexpr[cpos]=0;

 return 1;
}

void cclear(){
 cpos=0;
 cexpr[0]=0;
}

void cback(){
 if(cpos){
  cexpr[--cpos]=0;
 }
}

void calcTop(){
 char x[32];
 int n=strlen(cexpr);

 if(n>28){
  strcpy(x,cexpr+n-28);
 }
 else{
  strcpy(x,cexpr);
 }

 topc();

 printf("\x1b[2J");
 printf("         CALCULADORA\n\n");
 printf("+----------------------------+\n");
 printf("| %-28s |\n",x);
 printf("+----------------------------+\n\n");

 printf("Pulsa B para volver.");
}

void calcBottom(){
 botc();

 printf("\x1b[2J");

 printf(" ( C ) (DEL) ( ( ) ( ) )\n\n");
 printf(" ( 7 ) ( 8 ) ( 9 ) ( / )\n\n");
 printf(" ( 4 ) ( 5 ) ( 6 ) ( * )\n\n");
 printf(" ( 1 ) ( 2 ) ( 3 ) ( - )\n\n");
 printf(" ( 0 ) ( . ) ( = ) ( + )\n");
}

void calcEqual(){
 double v;

 if(!cexpr[0])return;

 if(calcEval(&v)){
  snprintf(cexpr,96,"%.10g",v);
  cpos=strlen(cexpr);
 }
 else{
  strcpy(cexpr,"ERROR");
  cpos=5;
 }
}

void calcButton(int row,int col){
 char k[5][4]={
  {'C','D','(',')'},
  {'7','8','9','/'},
  {'4','5','6','*'},
  {'1','2','3','-'},
  {'0','.','=','+'}
 };

 char ch=k[row][col];

 if(ch=='C'){
  cclear();
  return;
 }

 if(ch=='D'){
  if(!strcmp(cexpr,"ERROR"))cclear();
  else cback();
  return;
 }

 if(ch=='='){
  calcEqual();
  return;
 }

 if(!strcmp(cexpr,"ERROR"))
  cclear();

 cadd(ch);
}

void calculator(){
 touchPosition t;

 int bx[4]={4,68,132,196};
 int by[5]={12,47,82,117,152};

 cclear();

 hidekey();

 while(1){

  calcTop();
  calcBottom();

  swiWaitForVBlank();
  scanKeys();

  u16 k=keysDown();

  if(k&KEY_B)return;

  if(k&KEY_START)return;

  if(k&KEY_A){
   calcEqual();
   continue;
  }

  if(k&KEY_TOUCH){

   touchRead(&t);

   for(int row=0;row<5;row++){
    for(int col=0;col<4;col++){

     if(t.px>=bx[col]&&
        t.px<bx[col]+56&&
        t.py>=by[row]&&
        t.py<by[row]+28){

      calcButton(row,col);

      while(1){
       scanKeys();

       if(!(keysHeld()&KEY_TOUCH))
        break;

       swiWaitForVBlank();
      }

      row=5;
      break;
     }
    }
   }
  }
 }
}
void chat(){
 char m[IN],x[IN];

 nh=0;

 key();

 addh("DSi IA","Hola! Soy DSi IA.");
 addh("DSi IA","Pregunta lo que quieras.");

 while(1){

  showh();

  if(!chatRead(m,IN))
   break;

  if(!m[0])
   continue;

  strncpy(x,m,IN-1);
  x[IN-1]=0;
  normal(x);

  if(strstr(x,"salir")||strstr(x,"menu"))
   break;

  addh("TU",m);
  showh();

  if(!strcmp(x,"aprender")||strstr(x," aprender")){
   learn();
   continue;
  }

  answer(m);
 }

 hidekey();
}

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
 int ix[150],n=0;

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
  }
  else{
   printf("Incorrecto.\n\n");
   printf("Respuesta: %s\n\n",p->r);
   printf("%s\n",p->e);
  }

  printf("\nPulsa A...");
  waitA();
 }

 hidekey();
}

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
 printf("SELECT - Chat IA\n");
 printf("START - Calculadora\n\n");

 printf("Puntos: %d\n",pts);
 printf("Aciertos: %d/%d\n",ok,done);
}

int main(void){

 videoSetMode(MODE_0_2D);
 videoSetModeSub(MODE_0_2D);

 vramSetBankA(VRAM_A_MAIN_BG);
 vramSetBankC(VRAM_C_SUB_BG);

 consoleInit(
  &top,0,
  BgType_Text4bpp,
  BgSize_T_256x256,
  22,3,
  true,true
 );

 consoleInit(
  &bot,0,
  BgType_Text4bpp,
  BgSize_T_256x256,
  22,3,
  false,true
 );

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
    play("Cultura",1);
    break;
   }

   if(k&KEY_SELECT){
    chat();
    break;
   }

   if(k&KEY_START){
    calculator();
    break;
   }
  }
 }

 return 0;
}
