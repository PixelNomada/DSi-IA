#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 48

typedef struct {
    const char *pregunta;
    const char *respuesta;
    const char *explicacion;
    const char *categoria;
    int dificultad;
} Pregunta;

Pregunta banco[] = {
    {"Cuanto es (-3) x 7?", "-21", "Negativo por positivo = negativo.", "Matematicas", 1},
    {"Cuanto es 2/5 + 1/5?", "3/5", "Misma denominacion: 3/5.", "Matematicas", 1},
    {"Resuelve: 2x + 5 = 13", "4", "2x = 8 → x = 4.", "Matematicas", 2},
    {"Area de un triangulo base 10 altura 6?", "30", "(base x altura)/2 = 30.", "Matematicas", 1},
    {"Cuanto es 5! ?", "120", "5x4x3x2x1 = 120.", "Matematicas", 2},
    {"Cuanto es 15% de 80?", "12", "0.15 x 80 = 12.", "Matematicas", 1},
    {"Que es un verbo?", "accion", "Expresa acciones o estados.", "Espanol", 1},
    {"Que es un sustantivo?", "nombre", "Nombra personas, cosas o ideas.", "Espanol", 1},
    {"Que es un adjetivo?", "cualidad", "Describe al sustantivo.", "Espanol", 1},
    {"Que es una metafora?", "figura", "Identifica una cosa con otra sin usar 'como'.", "Espanol", 2},
    {"Cual es la formula del agua?", "h2o", "Dos hidrogenos y un oxigeno.", "Quimica", 1},
    {"Que es un atomo?", "particula", "Unidad basica de la materia.", "Quimica", 1},
    {"Que es el pH?", "acidez", "Mide que tan acida o basica es una sustancia.", "Quimica", 1},
    {"Simbolo del hierro?", "fe", "Viene del latin ferrum.", "Quimica", 1},
    {"Capital de Francia?", "paris", "Paris es la capital de Francia.", "General", 1},
    {"Quien escribio Don Quijote?", "cervantes", "Miguel de Cervantes.", "General", 1},
    {"Cuantos planetas hay en el sistema solar?", "8", "Ocho planetas.", "General", 1},
};

int total_preguntas = sizeof(banco) / sizeof(banco[0]);
int puntuacion = 0, aciertos = 0, preguntas_hechas = 0;
Keyboard *kbd = NULL;

void a_minusculas(char *s) {
    while (*s) { *s = tolower((unsigned char)*s); s++; }
}

int contiene(const char *h, const char *n) {
    return strstr(h, n) != NULL;
}

int es_correcta(const char *user, const char *correcta) {
    char u[MAX_INPUT], c[MAX_INPUT];
    strncpy(u, user, MAX_INPUT-1); u[MAX_INPUT-1] = 0;
    strncpy(c, correcta, MAX_INPUT-1); c[MAX_INPUT-1] = 0;
    a_minusculas(u); a_minusculas(c);
    if (strcmp(u, c) == 0) return 1;
    if (contiene(u, c)) return 1;
    return 0;
}

void esperar_A(void) {
    while (1) {
        swiWaitForVBlank();
        scanKeys();
        if (keysDown() & KEY_A) break;
    }
}

void iniciar_teclado(void) {
    kbd = keyboardInit(NULL, 3, BgType_Text4bpp, BgSize_T_256x512, 20, 0, false, true);
    keyboardShow();
}

void ocultar_teclado(void) {
    if (kbd) keyboardHide();
}

int leer_texto(char *buf, int max) {
    int pos = 0;
    buf[0] = 0;
    iprintf("> ");
    while (1) {
        swiWaitForVBlank();
        scanKeys();
        int key = keyboardUpdate();
        if (key > 0) {
            if (key == DVK_ENTER || key == '\n') {
                buf[pos] = 0;
                return 1;
            }
            if ((key == DVK_BACKSPACE || key == 8) && pos > 0) {
                pos--;
                buf[pos] = 0;
                iprintf("\b \b");
            } else if (key >= 32 && key < 127 && pos < max-1) {
                buf[pos++] = (char)key;
                buf[pos] = 0;
                iprintf("%c", key);
            }
        }
        if (keysDown() & KEY_A) { buf[pos] = 0; return 1; }
        if (keysDown() & KEY_B) { buf[0] = 0; return 0; }
    }
}

int calcular_expresion(const char *expr, int *resultado) {
    char buffer[64];
    strncpy(buffer, expr, 63);
    buffer[63] = 0;
    a_minusculas(buffer);

    char limpio[64];
    int j = 0;
    for (int i = 0; buffer[i] && j < 63; i++) {
        if (buffer[i] != ' ') limpio[j++] = buffer[i];
    }
    limpio[j] = 0;

    char op = 0;
    int pos_op = -1;
    for (int i = 1; limpio[i]; i++) {
        if (limpio[i] == '+' || limpio[i] == '-' || limpio[i] == '*' ||
            limpio[i] == 'x' || limpio[i] == '/' || limpio[i] == '^') {
            op = limpio[i];
            if (op == 'x') op = '*';
            pos_op = i;
            break;
        }
    }
    if (pos_op == -1) return 0;

    char izq[32] = {0}, der[32] = {0};
    strncpy(izq, limpio, pos_op);
    strcpy(der, limpio + pos_op + 1);

    int a = atoi(izq);
    int b = atoi(der);

    switch (op) {
        case '+': *resultado = a + b; break;
        case '-': *resultado = a - b; break;
        case '*': *resultado = a * b; break;
        case '/': if (b == 0) return 0; *resultado = a / b; break;
        case '^': {
            *resultado = 1;
            for (int i = 0; i < b; i++) *resultado *= a;
            break;
        }
        default: return 0;
    }
    return 1;
}

void responder_charla(const char *msg) {
    char m[MAX_INPUT];
    strncpy(m, msg, MAX_INPUT-1);
    m[MAX_INPUT-1] = 0;
    a_minusculas(m);

    iprintf("\n\nDSi IA: ");

    // Calculadora
    if (contiene(m, "cuanto es") || contiene(m, "cuánto es") || contiene(m, "calcula") ||
        strchr(m, '+') || strchr(m, '-') || strchr(m, '*') || strchr(m, 'x') || strchr(m, '/') || strchr(m, '^')) {
        int res = 0;
        if (calcular_expresion(m, &res)) {
            iprintf("El resultado es: %d", res);
            return;
        }
    }

    if (contiene(m, "hola") || contiene(m, "buenos") || contiene(m, "buenas"))
        iprintf("Hola! Soy DSi IA, tu asistente escolar. En que te puedo ayudar?");
    else if (contiene(m, "como estas") || contiene(m, "cómo estás") || contiene(m, "que tal"))
        iprintf("Estoy muy bien, gracias. Lista para ayudarte a estudiar.");
    else if (contiene(m, "quien eres") || contiene(m, "quién eres"))
        iprintf("Soy DSi IA, tu asistente educativa para DSi XL. Se de Matematicas, Espanol y Quimica.");
    else if (contiene(m, "que es la quimica") || contiene(m, "qué es la química"))
        iprintf("La Quimica estudia la materia, su composicion y las transformaciones que sufre.");
    else if (contiene(m, "que es un atomo") || contiene(m, "qué es un átomo"))
        iprintf("El atomo es la unidad mas pequena de un elemento. Tiene protones, neutrones y electrones.");
    else if (contiene(m, "que es un verbo"))
        iprintf("Un verbo expresa acciones, estados o procesos. Ejemplo: correr, estudiar, ser.");
    else if (contiene(m, "que es un sustantivo"))
        iprintf("Un sustantivo nombra personas, animales, cosas o ideas.");
    else if (contiene(m, "que es una fraccion") || contiene(m, "qué es una fracción"))
        iprintf("Una fraccion representa partes de un todo. Tiene numerador y denominador.");
    else if (contiene(m, "que es una ecuacion") || contiene(m, "qué es una ecuación"))
        iprintf("Una ecuacion es una igualdad con una incognita (x). Hay que encontrar el valor de x.");
    else if (contiene(m, "gracias"))
        iprintf("De nada! Sigue estudiando, tu puedes.");
    else if (contiene(m, "adios") || contiene(m, "adiós") || contiene(m, "hasta luego"))
        iprintf("Hasta luego! Que te vaya muy bien en tus estudios.");
    else
        iprintf("No estoy segura de entenderte. Prueba preguntar: 'Que es la quimica?', 'Cuanto es 56+7' o usa el modo Leccion.");
}

void dibujar_titulo(void) {
    iprintf("\x1b[2J");
    iprintf("================================\n");
    iprintf("         DSi  IA\n");
    iprintf("   Asistente escolar DSi XL\n");
    iprintf("================================\n\n");
}

void menu_principal(void) {
    ocultar_teclado();
    dibujar_titulo();
    iprintf("  A - Modo Examen\n");
    iprintf("  B - Modo Leccion\n");
    iprintf("  X - Matematicas\n");
    iprintf("  Y - Espanol\n");
    iprintf("  L - Quimica\n");
    iprintf("  R - Cultura General\n");
    iprintf("  Select - Hablar con DSi IA\n");
    iprintf("  Start - Salir\n\n");
    iprintf("  Puntos: %d | Aciertos: %d/%d\n", puntuacion, aciertos, preguntas_hechas);
}

void jugar(const char *cat, int leccion) {
    int idx[40], n = 0;
    for (int i = 0; i < total_preguntas; i++) {
        if (!cat || strcmp(banco[i].categoria, cat) == 0)
            idx[n++] = i;
    }
    if (n == 0) return;

    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
    }

    int maxq = n < 6 ? n : 6;
    iniciar_teclado();

    for (int q = 0; q < maxq; q++) {
        Pregunta *p = &banco[idx[q]];
        dibujar_titulo();
        iprintf("Categoria: %s\nPregunta %d/%d\n\n%s\n\n", p->categoria, q+1, maxq, p->pregunta);

        char resp[MAX_INPUT];
        leer_texto(resp, MAX_INPUT);
        preguntas_hechas++;

        if (resp[0] && es_correcta(resp, p->respuesta)) {
            iprintf("\n\n*** CORRECTO! ***\n");
            aciertos++;
            puntuacion += 10 * p->dificultad;
            if (leccion) iprintf("\n%s\n", p->explicacion);
        } else {
            iprintf("\n\nIncorrecto.\nRespuesta: %s\n%s\n", p->respuesta, p->explicacion);
        }
        iprintf("\nPulsa A...");
        esperar_A();
    }
    ocultar_teclado();
    dibujar_titulo();
    iprintf("Ronda terminada!\nPuntos: %d\nAciertos: %d/%d\n\nPulsa A...", puntuacion, aciertos, preguntas_hechas);
    esperar_A();
}

void modo_charla(void) {
    iniciar_teclado();
    while (1) {
        dibujar_titulo();
        iprintf("Habla con DSi IA\nEscribe 'salir' para volver\n\n");
        char msg[MAX_INPUT];
        if (!leer_texto(msg, MAX_INPUT) || msg[0] == 0) continue;

        char tmp[MAX_INPUT];
        strncpy(tmp, msg, MAX_INPUT-1);
        a_minusculas(tmp);
        if (contiene(tmp, "salir") || contiene(tmp, "menu")) break;

        responder_charla(msg);
        iprintf("\n\nPulsa A para continuar...");
        esperar_A();
    }
    ocultar_teclado();
}

int main(void) {
    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);
    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, true, true);
    consoleInit(NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 22, 3, false, true);

    srand(2026);

    while (1) {
        menu_principal();
        while (1) {
            swiWaitForVBlank();
            scanKeys();
            u16 k = keysDown();

            if (k & KEY_A) { jugar(NULL, 0); break; }
            if (k & KEY_B) { jugar(NULL, 1); break; }
            if (k & KEY_X) { jugar("Matematicas", 1); break; }
            if (k & KEY_Y) { jugar("Espanol", 1); break; }
            if (k & KEY_L) { jugar("Quimica", 1); break; }
            if (k & KEY_R) { jugar("General", 1); break; }
            if (k & KEY_SELECT) { modo_charla(); break; }
            if (k & KEY_START) {
                dibujar_titulo();
                iprintf("Hasta luego!\nGracias por usar DSi IA.\n");
                while (1) swiWaitForVBlank();
            }
        }
    }
    return 0;
}
