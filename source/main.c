#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 48
#define MAX_HISTORIAL 8

typedef struct {
    const char *pregunta;
    const char *respuesta;
    const char *explicacion;
    const char *categoria;
} Pregunta;


/* =========================================================
   BANCO DE PREGUNTAS
   ========================================================= */

Pregunta banco[] = {

    /* ================= MATEMATICAS ================= */

    {"Cuanto es (-5) x 6?",
     "-30",
     "Negativo por positivo = negativo.",
     "Matematicas"},

    {"Cuanto es 3/4 + 1/4?",
     "1",
     "3/4 + 1/4 = 4/4 = 1.",
     "Matematicas"},

    {"Resuelve: 3x - 7 = 8",
     "5",
     "3x = 15 -> x = 5.",
     "Matematicas"},

    {"Resuelve: 2x + 4 = 18",
     "7",
     "2x = 14 -> x = 7.",
     "Matematicas"},

    {"Area de un triangulo base 12 altura 5?",
     "30",
     "(base x altura)/2 = 30.",
     "Matematicas"},

    {"Perimetro de un cuadrado de lado 9?",
     "36",
     "4 x lado = 36.",
     "Matematicas"},

    {"Cuanto es 20% de 150?",
     "30",
     "0.20 x 150 = 30.",
     "Matematicas"},

    {"Cuanto es 2 elevado a 6?",
     "64",
     "2^6 = 64.",
     "Matematicas"},

    {"Cuanto es la raiz cuadrada de 121?",
     "11",
     "11 x 11 = 121.",
     "Matematicas"},

    {"Cuanto es 7! ?",
     "5040",
     "7x6x5x4x3x2x1 = 5040.",
     "Matematicas"},

    {"Simplifica 16/24",
     "2/3",
     "Dividir entre 8: 2/3.",
     "Matematicas"},

    {"Cuanto es 3/5 de 50?",
     "30",
     "3/5 x 50 = 30.",
     "Matematicas"},


    /* ================= ESPANOL ================= */

    {"Que es un verbo?",
     "accion",
     "Expresa acciones, estados o procesos.",
     "Espanol"},

    {"Que es un sustantivo?",
     "nombre",
     "Nombra personas, animales, cosas o ideas.",
     "Espanol"},

    {"Que es un adjetivo?",
     "cualidad",
     "Describe o califica al sustantivo.",
     "Espanol"},

    {"Que es un adverbio?",
     "modifica",
     "Modifica al verbo, adjetivo u otro adverbio.",
     "Espanol"},

    {"Que es el sujeto?",
     "quien",
     "Quien realiza la accion o de quien se habla.",
     "Espanol"},

    {"Que es el predicado?",
     "verbo",
     "Lo que se dice del sujeto. Contiene el verbo.",
     "Espanol"},

    {"Que es una metafora?",
     "figura",
     "Identifica una cosa con otra sin usar 'como'.",
     "Espanol"},

    {"Que es un diptongo?",
     "vocales",
     "Dos vocales en la misma silaba.",
     "Espanol"},

    {"Que es un hiato?",
     "separacion",
     "Dos vocales que se separan en silabas distintas.",
     "Espanol"},

    {"Cual es el participio de escribir?",
     "escrito",
     "Escribir -> escrito.",
     "Espanol"},

    {"Cual es el antonimo de escaso?",
     "abundante",
     "Escaso <-> abundante.",
     "Espanol"},

    {"Que es el sujeto tacito?",
     "sobreentendido",
     "No se escribe pero se entiende por el verbo.",
     "Espanol"},


    /* ================= QUIMICA ================= */

    {"Que es la quimica?",
     "materia",
     "Estudia la materia, su composicion y transformaciones.",
     "Quimica"},

    {"Que es un atomo?",
     "particula",
     "Unidad basica de un elemento.",
     "Quimica"},

    {"Que es una molecula?",
     "union",
     "Union de dos o mas atomos.",
     "Quimica"},

    {"Cual es la formula del agua?",
     "h2o",
     "Dos hidrogenos y un oxigeno.",
     "Quimica"},

    {"Cual es la formula del dioxido de carbono?",
     "co2",
     "Un carbono y dos oxigenos.",
     "Quimica"},

    {"Que es el pH?",
     "acidez",
     "Mide la acidez o basicidad (0 a 14).",
     "Quimica"},

    {"Que particula tiene carga positiva?",
     "proton",
     "El proton tiene carga +.",
     "Quimica"},

    {"Que particula tiene carga negativa?",
     "electron",
     "El electron tiene carga -.",
     "Quimica"},

    {"Que particula no tiene carga?",
     "neutron",
     "El neutron es neutro.",
     "Quimica"},

    {"Que es un enlace ionico?",
     "transferencia",
     "Se transfieren electrones (metal + no metal).",
     "Quimica"},

    {"Que es un enlace covalente?",
     "compartir",
     "Se comparten electrones.",
     "Quimica"},

    {"Simbolo del hierro?",
     "fe",
     "Del latin ferrum.",
     "Quimica"},

    {"Simbolo del sodio?",
     "na",
     "Del latin natrium.",
     "Quimica"},

    {"Simbolo del potasio?",
     "k",
     "Del latin kalium.",
     "Quimica"},


    /* ================= CULTURA GENERAL ================= */

    {"Capital de Francia?",
     "paris",
     "Paris.",
     "General"},

    {"Capital de Japon?",
     "tokio",
     "Tokio.",
     "General"},

    {"Quien escribio Don Quijote?",
     "cervantes",
     "Miguel de Cervantes.",
     "General"},

    {"Cuantos planetas hay en el sistema solar?",
     "8",
     "Ocho planetas.",
     "General"},

    {"Oceano mas grande?",
     "pacifico",
     "Oceano Pacifico.",
     "General"},

    {"Quien pinto la Mona Lisa?",
     "leonardo",
     "Leonardo da Vinci.",
     "General"},

    {"En que ano llego el hombre a la Luna?",
     "1969",
     "1969.",
     "General"},

    {"Metal liquido a temperatura ambiente?",
     "mercurio",
     "Mercurio.",
     "General"},
};


/* =========================================================
   VARIABLES
   ========================================================= */

int total_preguntas =
    sizeof(banco) / sizeof(banco[0]);

int puntuacion = 0;
int aciertos = 0;
int preguntas_hechas = 0;

Keyboard *kbd = NULL;


/* =========================================================
   CONSOLAS DE LAS DOS PANTALLAS
   ========================================================= */

PrintConsole consola_arriba;
PrintConsole consola_abajo;


/* =========================================================
   HISTORIAL
   ========================================================= */

char historial[MAX_HISTORIAL][64];
int num_historial = 0;


/* =========================================================
   TEXTO A MINUSCULAS
   ========================================================= */

void a_minusculas(char *s) {

    while (*s) {
        *s = tolower((unsigned char)*s);
        s++;
    }
}


/* =========================================================
   CONTIENE
   ========================================================= */

int contiene(const char *h, const char *n) {

    return strstr(h, n) != NULL;
}


/* =========================================================
   RESPUESTA CORRECTA
   ========================================================= */

int es_correcta(
    const char *user,
    const char *correcta
) {

    char u[MAX_INPUT];
    char c[MAX_INPUT];

    strncpy(u, user, MAX_INPUT - 1);
    u[MAX_INPUT - 1] = 0;

    strncpy(c, correcta, MAX_INPUT - 1);
    c[MAX_INPUT - 1] = 0;

    a_minusculas(u);
    a_minusculas(c);

    if (
        strcmp(u, c) == 0 ||
        contiene(u, c)
    )
        return 1;

    return 0;
}


/* =========================================================
   ESPERAR A
   ========================================================= */

void esperar_A(void) {

    while (1) {

        swiWaitForVBlank();

        scanKeys();

        if (keysDown() & KEY_A)
            break;
    }
}


/* =========================================================
   TECLADO
   ========================================================= */

void iniciar_teclado(void) {

    kbd = keyboardInit(
        NULL,
        3,
        BgType_Text4bpp,
        BgSize_T_256x512,
        20,
        0,
        false,
        true
    );

    keyboardShow();
}


void ocultar_teclado(void) {

    if (kbd)
        keyboardHide();
}


/* =========================================================
   AGREGAR AL HISTORIAL
   ========================================================= */

void agregar_historial(
    const char *quien,
    const char *texto
) {

    if (num_historial >= MAX_HISTORIAL) {

        for (
            int i = 0;
            i < MAX_HISTORIAL - 1;
            i++
        ) {
            strcpy(
                historial[i],
                historial[i + 1]
            );
        }

        num_historial =
            MAX_HISTORIAL - 1;
    }

    snprintf(
        historial[num_historial],
        63,
        "%s: %s",
        quien,
        texto
    );

    num_historial++;
}


/* =========================================================
   MOSTRAR CONVERSACION
   PANTALLA SUPERIOR
   ========================================================= */

void mostrar_historial(void) {

    consoleSelect(&consola_arriba);

    printf("\x1b[2J");
    printf("\x1b[1;1H");

    printf("==============================\n");
    printf("          DSi IA\n");
    printf("==============================\n\n");


    /*
       Mostramos los mensajes del historial.
       Los ultimos mensajes son los mas importantes.
    */

    int inicio = 0;

    if (num_historial > 7)
        inicio = num_historial - 7;


    for (
        int i = inicio;
        i < num_historial;
        i++
    ) {

        printf("%s\n\n", historial[i]);
    }


    printf("------------------------------\n");
    printf("DSi IA conectado\n");
}


/* =========================================================
   LEER TEXTO
   ========================================================= */

int leer_texto(
    char *buf,
    int max
) {

    int pos = 0;

    buf[0] = 0;


    /*
       La escritura se muestra abajo.
    */

    consoleSelect(&consola_abajo);

    printf("\x1b[2J");
    printf("\x1b[1;1H");

    printf("==============================\n");
    printf("        NUEVO MENSAJE\n");
    printf("==============================\n\n");

    printf("Tu: ");


    while (1) {

        swiWaitForVBlank();

        scanKeys();

        int key = keyboardUpdate();


        if (key > 0) {

            /*
               ENTER
            */

            if (
                key == DVK_ENTER ||
                key == '\n'
            ) {

                buf[pos] = 0;

                return 1;
            }


            /*
               BORRAR
            */

            if (
                (
                    key == DVK_BACKSPACE ||
                    key == 8
                ) &&
                pos > 0
            ) {

                pos--;

                buf[pos] = 0;

                printf("\b \b");
            }


            /*
               CARACTER
            */

            else if (
                key >= 32 &&
                key < 127 &&
                pos < max - 1
            ) {

                buf[pos++] =
                    (char)key;

                buf[pos] = 0;

                printf(
                    "%c",
                    key
                );
            }
        }


        /*
           A = ENVIAR
        */

        if (keysDown() & KEY_A) {

            buf[pos] = 0;

            return 1;
        }


        /*
           B = CANCELAR
        */

        if (keysDown() & KEY_B) {

            buf[0] = 0;

            return 0;
        }
    }
}


/* =========================================================
   CALCULADORA
   ========================================================= */

int calcular_expresion(
    const char *expr,
    int *resultado
) {

    char buffer[64];

    strncpy(
        buffer,
        expr,
        63
    );

    buffer[63] = 0;

    a_minusculas(buffer);


    char limpio[64];

    int j = 0;


    for (
        int i = 0;
        buffer[i] && j < 63;
        i++
    ) {

        if (buffer[i] != ' ')
            limpio[j++] =
                buffer[i];
    }

    limpio[j] = 0;


    char op = 0;

    int pos_op = -1;


    for (
        int i = 1;
        limpio[i];
        i++
    ) {

        if (
            limpio[i] == '+' ||
            limpio[i] == '-' ||
            limpio[i] == '*' ||
            limpio[i] == 'x' ||
            limpio[i] == '/' ||
            limpio[i] == '^'
        ) {

            op =
                (limpio[i] == 'x')
                ? '*'
                : limpio[i];

            pos_op = i;

            break;
        }
    }


    if (pos_op == -1)
        return 0;


    char izq[32] = {0};
    char der[32] = {0};


    strncpy(
        izq,
        limpio,
        pos_op
    );

    strcpy(
        der,
        limpio + pos_op + 1
    );


    int a = atoi(izq);
    int b = atoi(der);


    switch (op) {

        case '+':

            *resultado =
                a + b;

            break;


        case '-':

            *resultado =
                a - b;

            break;


        case '*':

            *resultado =
                a * b;

            break;


        case '/':

            if (b == 0)
                return 0;

            *resultado =
                a / b;

            break;


        case '^':

            *resultado = 1;

            for (
                int i = 0;
                i < b;
                i++
            )
                *resultado *= a;

            break;


        default:

            return 0;
    }


    return 1;
}


/* =========================================================
   RESPONDER AL CHAT
   ========================================================= */

void responder_charla(
    const char *msg
) {

    char m[MAX_INPUT];

    strncpy(
        m,
        msg,
        MAX_INPUT - 1
    );

    m[MAX_INPUT - 1] = 0;

    a_minusculas(m);


    char respuesta[128] =
        "No estoy segura. Prueba con: 'Que es la quimica?' o 'Cuanto es 25+17'";


    /* ================= MATEMATICAS ================= */

    if (
        contiene(m, "cuanto es") ||
        contiene(m, "calcula") ||
        strchr(m, '+') ||
        strchr(m, '-') ||
        strchr(m, '*') ||
        strchr(m, 'x') ||
        strchr(m, '/') ||
        strchr(m, '^')
    ) {

        int res = 0;

        if (
            calcular_expresion(
                m,
                &res
            )
        ) {

            snprintf(
                respuesta,
                127,
                "El resultado es: %d",
                res
            );
        }
    }


    /* ================= SALUDOS ================= */

    else if (
        contiene(m, "hola") ||
        contiene(m, "buenos") ||
        contiene(m, "buenas")
    ) {

        strcpy(
            respuesta,
            "Hola! Soy DSi IA. Puedes preguntarme de Matematicas, Espanol, Quimica o pedirme que calcule algo."
        );
    }


    else if (
        contiene(m, "como estas") ||
        contiene(m, "que tal")
    ) {

        strcpy(
            respuesta,
            "Estoy muy bien, lista para ayudarte a estudiar. Y tu?"
        );
    }


    else if (
        contiene(m, "quien eres")
    ) {

        strcpy(
            respuesta,
            "Soy DSi IA, tu asistente escolar para DSi XL. Se de varias materias y puedo hacer calculos."
        );
    }


    /* ================= QUIMICA ================= */

    else if (
        contiene(m, "que es la quimica") ||
        contiene(m, "que es quimica")
    ) {

        strcpy(
            respuesta,
            "La Quimica estudia la materia, su composicion, propiedades y las transformaciones que puede sufrir."
        );
    }


    else if (
        contiene(m, "que es un atomo")
    ) {

        strcpy(
            respuesta,
            "El atomo es la unidad mas pequena de un elemento. Tiene protones, neutrones y electrones."
        );
    }


    else if (
        contiene(m, "que es una molecula")
    ) {

        strcpy(
            respuesta,
            "Una molecula esta formada por dos o mas atomos unidos mediante enlaces quimicos."
        );
    }


    else if (
        contiene(m, "que es el ph")
    ) {

        strcpy(
            respuesta,
            "El pH mide la acidez o basicidad de 0 a 14. 7 es neutro, menos de 7 acido y mas de 7 basico."
        );
    }


    /* ================= ESPANOL ================= */

    else if (
        contiene(m, "que es un verbo")
    ) {

        strcpy(
            respuesta,
            "Un verbo expresa acciones, estados o procesos. Ejemplos: correr, estudiar, ser y estar."
        );
    }


    else if (
        contiene(m, "que es un sustantivo")
    ) {

        strcpy(
            respuesta,
            "Un sustantivo nombra personas, animales, cosas, lugares o ideas."
        );
    }


    else if (
        contiene(m, "que es un adjetivo")
    ) {

        strcpy(
            respuesta,
            "Un adjetivo describe o califica a un sustantivo. Ejemplos: grande, azul, rapido y amable."
        );
    }


    else if (
        contiene(m, "que es una fraccion")
    ) {

        strcpy(
            respuesta,
            "Una fraccion representa partes de un todo. Tiene numerador arriba y denominador abajo."
        );
    }


    else if (
        contiene(m, "que es una ecuacion")
    ) {

        strcpy(
            respuesta,
            "Una ecuacion es una igualdad matematica que contiene una incognita. Hay que encontrar su valor."
        );
    }


    /* ================= CULTURA ================= */

    else if (
        contiene(m, "capital de francia")
    ) {

        strcpy(
            respuesta,
            "La capital de Francia es Paris."
        );
    }


    else if (
        contiene(m, "capital de japon")
    ) {

        strcpy(
            respuesta,
            "La capital de Japon es Tokio."
        );
    }


    else if (
        contiene(m, "don quijote")
    ) {

        strcpy(
            respuesta,
            "Don Quijote fue escrito por Miguel de Cervantes y publicado en dos partes."
        );
    }


    else if (
        contiene(m, "planetas")
    ) {

        strcpy(
            respuesta,
            "El Sistema Solar tiene ocho planetas: Mercurio, Venus, Tierra, Marte, Jupiter, Saturno, Urano y Neptuno."
        );
    }


    else if (
        contiene(m, "oceano mas grande")
    ) {

        strcpy(
            respuesta,
            "El Oceano Pacifico es el oceano mas grande de la Tierra."
        );
    }


    /* ================= DESPEDIDA ================= */

    else if (
        contiene(m, "gracias")
    ) {

        strcpy(
            respuesta,
            "De nada! Sigue preguntando todo lo que necesites."
        );
    }


    else if (
        contiene(m, "adios") ||
        contiene(m, "hasta luego")
    ) {

        strcpy(
            respuesta,
            "Hasta luego! Que te vaya excelente en tus estudios."
        );
    }


    /*
       Guardar solamente la respuesta.
       El mensaje del usuario se guarda
       en modo_charla().
    */

    agregar_historial(
        "DSi IA",
        respuesta
    );
}


/* =========================================================
   MENU PRINCIPAL
   ========================================================= */

void menu_principal(void) {

    ocultar_teclado();

    consoleSelect(
        &consola_arriba
    );

    printf("\x1b[2J");

    printf("==============================\n");
    printf("         DSi  IA\n");
    printf("   Asistente escolar\n");
    printf("==============================\n\n");

    printf("  A - Modo Examen\n");
    printf("  B - Modo Leccion\n");
    printf("  X - Matematicas\n");
    printf("  Y - Espanol\n");
    printf("  L - Quimica\n");
    printf("  R - Cultura General\n");
    printf("  Select - Chat con DSi IA\n");
    printf("  Start - Salir\n\n");

    printf(
        "  Puntos: %d | Aciertos: %d/%d\n",
        puntuacion,
        aciertos,
        preguntas_hechas
    );
}


/* =========================================================
   JUEGO
   ========================================================= */

void jugar(
    const char *cat,
    int leccion
) {

    int idx[50];
    int n = 0;


    for (
        int i = 0;
        i < total_preguntas;
        i++
    ) {

        if (
