#include <nds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

PrintConsole consolaTop;
PrintConsole consolaBottom;
Keyboard *kbd;

/* ---------------- ESTADO ---------------- */

char entrada[64] = "0";
char expresion[96] = "";

double izquierda = 0.0;
double ans = 0.0;
double memoria = 0.0;

char operacion = 0;
int nuevaEntrada = 1;

int angulo = 0;
/* 0 = DEG, 1 = RAD, 2 = GRAD */

int modoSCI = 0;
int errorCalc = 0;

/* ---------------- COLORES ---------------- */

void color(int c)
{
    consoleSetColor(NULL, (ConsoleColor)c);
}

/* ---------------- VALOR ACTUAL ---------------- */

double valorActual(void)
{
    return strtod(entrada, NULL);
}

void ponerNumero(double n)
{
    if (isnan(n) || isinf(n)) {
        strcpy(entrada, "ERROR");
        errorCalc = 1;
        return;
    }

    if (modoSCI)
        sprintf(entrada, "%.8e", n);
    else
        sprintf(entrada, "%.10g", n);

    nuevaEntrada = 0;
}

void limpiar(void)
{
    strcpy(entrada, "0");
    expresion[0] = '\0';
    izquierda = 0;
    operacion = 0;
    nuevaEntrada = 1;
    errorCalc = 0;
}

/* ---------------- ANGULOS ---------------- */

double convertirAngulo(double x)
{
    if (angulo == 0)
        return x * M_PI / 180.0;

    if (angulo == 2)
        return x * M_PI / 200.0;

    return x;
}

const char *nombreAngulo(void)
{
    if (angulo == 0) return "DEG";
    if (angulo == 1) return "RAD";
    return "GRAD";
}

void cambiarAngulo(void)
{
    angulo++;

    if (angulo > 2)
        angulo = 0;
}

/* ---------------- ENTRADA ---------------- */

void digito(char d)
{
    if (errorCalc)
        limpiar();

    if (nuevaEntrada) {
        entrada[0] = d;
        entrada[1] = '\0';
        nuevaEntrada = 0;
        return;
    }

    if (strlen(entrada) < 28)
        strncat(entrada, &d, 1);
}

void dobleCero(void)
{
    if (nuevaEntrada) {
        strcpy(entrada, "0");
        nuevaEntrada = 0;
        return;
    }

    if (strlen(entrada) < 26)
        strcat(entrada, "00");
}

void tripleCero(void)
{
    if (nuevaEntrada) {
        strcpy(entrada, "0");
        nuevaEntrada = 0;
        return;
    }

    if (strlen(entrada) < 25)
        strcat(entrada, "000");
}

void decimal(void)
{
    if (errorCalc)
        limpiar();

    if (nuevaEntrada) {
        strcpy(entrada, "0.");
        nuevaEntrada = 0;
        return;
    }

    if (!strchr(entrada, '.'))
        strcat(entrada, ".");
}

void borrar(void)
{
    int n;

    if (errorCalc) {
        limpiar();
        return;
    }

    n = strlen(entrada);

    if (nuevaEntrada || n <= 1) {
        strcpy(entrada, "0");
        nuevaEntrada = 0;
        return;
    }

    entrada[n - 1] = '\0';

    if (!strcmp(entrada, "-"))
        strcpy(entrada, "0");
}

void cambiarSigno(void)
{
    double x;

    if (errorCalc)
        return;

    x = valorActual();

    if (x == 0)
        return;

    ponerNumero(-x);
}

/* ---------------- OPERACIONES ---------------- */

double calcular(double a, double b, char op)
{
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;

    if (op == '/') {
        if (b == 0) return NAN;
        return a / b;
    }

    if (op == '^')
        return pow(a, b);

    if (op == 'P') {
        int ia = (int)a;
        int ib = (int)b;
        double r = 1;
        int i;

        if (ia < 0 || ib < 0 || ib > ia)
            return NAN;

        for (i = 0; i < ib; i++)
            r *= (ia - i);

        return r;
    }

    if (op == 'C') {
        int ia = (int)a;
        int ib = (int)b;
        double r = 1;
        int i;

        if (ia < 0 || ib < 0 || ib > ia)
            return NAN;

        for (i = 1; i <= ib; i++)
            r *= (double)(ia - ib + i) / i;

        return r;
    }

    return b;
}

void prepararOperacion(char op)
{
    if (errorCalc)
        return;

    if (operacion != 0 && !nuevaEntrada) {
        double resultado;

        resultado = calcular(izquierda, valorActual(), operacion);

        if (isnan(resultado) || isinf(resultado)) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            operacion = 0;
            return;
        }

        izquierda = resultado;
        ponerNumero(resultado);
    } else {
        izquierda = valorActual();
    }

    operacion = op;
    nuevaEntrada = 1;

    sprintf(expresion, "%.10g %c", izquierda,
            op == 'P' ? 'P' :
            op == 'C' ? 'C' : op);
}

void igual(void)
{
    double resultado;

    if (!operacion || errorCalc)
        return;

    resultado = calcular(izquierda, valorActual(), operacion);

    if (isnan(resultado) || isinf(resultado)) {
        strcpy(entrada, "ERROR");
        errorCalc = 1;
        operacion = 0;
        return;
    }

    ans = resultado;
    ponerNumero(resultado);

    expresion[0] = '\0';
    operacion = 0;
    nuevaEntrada = 1;
}

/* ---------------- FUNCIONES CIENTIFICAS ---------------- */

void funcionUnaria(int f)
{
    double x = valorActual();
    double r = x;
    int i;

    if (errorCalc)
        return;

    if (f == 1) {
        r = sin(convertirAngulo(x));
    }

    if (f == 2) {
        r = cos(convertirAngulo(x));
    }

    if (f == 3) {
        double c = cos(convertirAngulo(x));

        if (fabs(c) < 0.000000001) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = tan(convertirAngulo(x));
    }

    if (f == 4) {
        if (x <= 0) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = log10(x);
    }

    if (f == 5) {
        if (x <= 0) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = log(x);
    }

    if (f == 6) {
        if (x < 0) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = sqrt(x);
    }

    if (f == 7)
        r = x * x;

    if (f == 8) {
        if (x == 0) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = 1.0 / x;
    }

    if (f == 9)
        r = fabs(x);

    if (f == 10) {
        if (x < 0 || x != floor(x) || x > 170) {
            strcpy(entrada, "ERROR");
            errorCalc = 1;
            return;
        }

        r = 1;

        for (i = 1; i <= (int)x; i++)
            r *= i;
    }

    if (f == 11)
        r = x / 100.0;

    if (f == 12)
        r = M_PI;

    if (f == 13)
        r = exp(1.0);

    if (f == 14)
        r = pow(10.0, x);

    if (f == 15)
        r = exp(x);

    if (f == 16)
        r = floor(x);

    if (isnan(r) || isinf(r)) {
        strcpy(entrada, "ERROR");
        errorCalc = 1;
        return;
    }

    ans = r;
    ponerNumero(r);
}

/* ---------------- MEMORIA ---------------- */

void memoriaMas(void)
{
    if (!errorCalc)
        memoria += valorActual();
}

void memoriaMenos(void)
{
    if (!errorCalc)
        memoria -= valorActual();
}

void memoriaClear(void)
{
    memoria = 0;
}

void memoriaRecall(void)
{
    ponerNumero(memoria);
}

/* ---------------- PANTALLA SUPERIOR ---------------- */

void mostrarTop(void)
{
    seleccionar_top();

    consoleClear();

    color(CONSOLE_CYAN);
    iprintf("     CALCULADORA CIENTIFICA\n");

    color(CONSOLE_WHITE);
    iprintf("-------------------------------\n");

    color(CONSOLE_YELLOW);
    iprintf("EXP: ");

    color(CONSOLE_WHITE);

    if (expresion[0])
        iprintf("%s\n", expresion);
    else
        iprintf("-\n");

    color(CONSOLE_GREEN);
    iprintf("\nRESULTADO\n");

    color(CONSOLE_WHITE);
    iprintf("> %s\n", entrada);

    iprintf("\n");

    color(CONSOLE_LIGHT_BLUE);
    iprintf("ANGULO: %s\n", nombreAngulo());

    color(CONSOLE_LIGHT_BLUE);
    iprintf("MEMORIA: %.8g\n", memoria);

    color(CONSOLE_WHITE);
    iprintf("\n");

    color(CONSOLE_YELLOW);
    iprintf("SIN COS TAN = trigonometria\n");
    iprintf("LOG/LN = logaritmos\n");
    iprintf("XY = x elevado a y\n");
    iprintf("NPR/NCR = combinaciones\n");

    color(CONSOLE_WHITE);
    iprintf("\n");

    if (errorCalc) {
        color(CONSOLE_RED);
        iprintf("ERROR: pulsa AC\n");
    } else {
        color(CONSOLE_WHITE);
        iprintf("Toca los botones de abajo.");
    }
}

/* ---------------- BOTONES ---------------- */

const char *botones[7][8] = {
    {"SIN", "COS", "TAN", "LOG", "7", "8", "9", "/"},
    {"LN", "SQRT", "X2", "XY", "4", "5", "6", "*"},
    {"1/X", "ABS", "N!", "%", "1", "2", "3", "-"},
    {"PI", "E", "DRG", "ANS", "0", ".", "+/-", "+"},
    {"NPR", "NCR", "10X", "EX", "00", "000", "=", "+"},
    {"AC", "DEL", "M+", "M-", "MC", "MR", "", ""},
    {"MODE", "CLR", "", "", "", "", "", ""}
};

void dibujarBoton(const char *texto, int fila, int col)
{
    if (col < 4) {
        color(CONSOLE_YELLOW);
    } else {
        if (col >= 4 && col <= 6)
            color(CONSOLE_LIGHT_BLUE);
        else
            color(CONSOLE_WHITE);
    }

    iprintf("%-4s", texto);

    if (col == 3)
        iprintf(" ");
}

void mostrarBottom(void)
{
    int f, c;

    seleccionar_bottom();

    consoleClear();

    for (f = 0; f < 7; f++) {
        for (c = 0; c < 8; c++) {
            dibujarBoton(botones[f][c], f, c);
        }

        iprintf("\n");
    }

    color(CONSOLE_WHITE);
    iprintf("\n");
    iprintf("DRG: DEG/RAD/GRAD\n");
    iprintf("MODE: normal/cientifica");
}

/* ---------------- AYUDA ---------------- */

void ayuda(void)
{
    seleccionar_top();

    consoleClear();

    color(CONSOLE_CYAN);
    iprintf("       AYUDA RAPIDA\n");
    iprintf("-------------------------------\n\n");

    color(CONSOLE_WHITE);
    iprintf("1. Toca un numero.\n\n");
    iprintf("2. Toca + - * / o XY.\n\n");
    iprintf("3. Escribe el segundo numero.\n\n");
    iprintf("4. Pulsa =.\n\n");

    color(CONSOLE_YELLOW);
    iprintf("Ejemplo:\n");
    color(CONSOLE_WHITE);
    iprintf("2  XY  3  =  8\n\n");

    color(CONSOLE_YELLOW);
    iprintf("Cientifica:\n");
    color(CONSOLE_WHITE);
    iprintf("SIN COS TAN LOG LN SQRT\n");
    iprintf("1/X ABS N! 10X EX %%\n");

    color(CONSOLE_LIGHT_BLUE);
    iprintf("\nNPR/NCR: permutaciones\n");
    iprintf("DRG: grados/radianes/grads\n");

    color(CONSOLE_WHITE);
    iprintf("\nToca CLR para regresar.");
}

/* ---------------- ACCIONES TACTILES ---------------- */

void tocar(int fila, int col)
{
    char *b;

    if (fila < 0 || fila > 6 || col < 0 || col > 7)
        return;

    b = (char *)botones[fila][col];

    if (!strcmp(b, ""))
        return;

    /* NUMEROS */

    if (!strcmp(b, "0")) {
        digito('0');
        return;
    }

    if (!strcmp(b, "1")) {
        digito('1');
        return;
    }

    if (!strcmp(b, "2")) {
        digito('2');
        return;
    }

    if (!strcmp(b, "3")) {
        digito('3');
        return;
    }

    if (!strcmp(b, "4")) {
        digito('4');
        return;
    }

    if (!strcmp(b, "5")) {
        digito('5');
        return;
    }

    if (!strcmp(b, "6")) {
        digito('6');
        return;
    }

    if (!strcmp(b, "7")) {
        digito('7');
        return;
    }

    if (!strcmp(b, "8")) {
        digito('8');
        return;
    }

    if (!strcmp(b, "9")) {
        digito('9');
        return;
    }

    if (!strcmp(b, ".")) {
        decimal();
        return;
    }

    if (!strcmp(b, "00")) {
        dobleCero();
        return;
    }

    if (!strcmp(b, "000")) {
        tripleCero();
        return;
    }

    /* OPERACIONES */

    if (!strcmp(b, "+")) {
        prepararOperacion('+');
        return;
    }

    if (!strcmp(b, "-")) {
        prepararOperacion('-');
        return;
    }

    if (!strcmp(b, "*")) {
        prepararOperacion('*');
        return;
    }

    if (!strcmp(b, "/")) {
        prepararOperacion('/');
        return;
    }

    if (!strcmp(b, "XY")) {
        prepararOperacion('^');
        return;
    }

    if (!strcmp(b, "NPR")) {
        prepararOperacion('P');
        return;
    }

    if (!strcmp(b, "NCR")) {
        prepararOperacion('C');
        return;
    }

    if (!strcmp(b, "=")) {
        igual();
        return;
    }

    /* CIENTIFICAS */

    if (!strcmp(b, "SIN")) {
        funcionUnaria(1);
        return;
    }

    if (!strcmp(b, "COS")) {
        funcionUnaria(2);
        return;
    }

    if (!strcmp(b, "TAN")) {
        funcionUnaria(3);
        return;
    }

    if (!strcmp(b, "LOG")) {
        funcionUnaria(4);
        return;
    }

    if (!strcmp(b, "LN")) {
        funcionUnaria(5);
        return;
    }

    if (!strcmp(b, "SQRT")) {
        funcionUnaria(6);
        return;
    }

    if (!strcmp(b, "X2")) {
        funcionUnaria(7);
        return;
    }

    if (!strcmp(b, "1/X")) {
        funcionUnaria(8);
        return;
    }

    if (!strcmp(b, "ABS")) {
        funcionUnaria(9);
        return;
    }

    if (!strcmp(b, "N!")) {
        funcionUnaria(10);
        return;
    }

    if (!strcmp(b, "%")) {
        funcionUnaria(11);
        return;
    }

    if (!strcmp(b, "PI")) {
        funcionUnaria(12);
        return;
    }

    if (!strcmp(b, "E")) {
        funcionUnaria(13);
        return;
    }

    if (!strcmp(b, "10X")) {
        funcionUnaria(14);
        return;
    }

    if (!strcmp(b, "EX")) {
        funcionUnaria(15);
        return;
    }

    /* OTROS */

    if (!strcmp(b, "+/-")) {
        cambiarSigno();
        return;
    }

    if (!strcmp(b, "DEL")) {
        borrar();
        return;
    }

    if (!strcmp(b, "AC")) {
        limpiar();
        return;
    }

    if (!strcmp(b, "DRG")) {
        cambiarAngulo();
        return;
    }

    if (!strcmp(b, "M+")) {
        memoriaMas();
        return;
    }

    if (!strcmp(b, "M-")) {
        memoriaMenos();
        return;
    }

    if (!strcmp(b, "MC")) {
        memoriaClear();
        return;
    }

    if (!strcmp(b, "MR")) {
        memoriaRecall();
        return;
    }

    if (!strcmp(b, "CLR")) {
        limpiar();
        return;
    }

    if (!strcmp(b, "MODE")) {
        modoSCI = !modoSCI;
        ponerNumero(valorActual());
        return;
    }
}

/* ---------------- MAIN ---------------- */

int main(void)
{
    touchPosition touch;
    int f, c;

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    consoleInit(
        &consolaTop,
        0,
        BgType_Text4bpp,
        BgSize_T_256x256,
        22,
        3,
        true,
        true
    );

    consoleInit(
        &consolaBottom,
        0,
        BgType_Text4bpp,
        BgSize_T_256x256,
        22,
        3,
        false,
        true
    );

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

    keyboardHide();

    seleccionar_top();
    consoleClear();

    seleccionar_bottom();
    consoleClear();

    mostrarTop();
    mostrarBottom();

    while (1) {
        swiWaitForVBlank();

        scanKeys();

        if (keysDown() & KEY_TOUCH) {
            touchRead(&touch);

            c = touch.px / 32;
            f = touch.py / 24;

            if (c > 7) c = 7;
            if (f > 6) f = 6;

            tocar(f, c);

            mostrarTop();
            mostrarBottom();
        }

        if (keysDown() & KEY_START)
            break;
    }

    keyboardShow();

    return 0;
}
