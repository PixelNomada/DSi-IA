#include <nds.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

PrintConsole top,bottom;
Keyboard *kbd;

#define PI 3.14159265358979323846

double valor=0,mem=0,ans=0;
double pendiente=0;
char op=0;
char entrada[32]="0";
int nuevo=1;
int grado=1;
int error=0;

void T(){consoleSelect(&top);}
void B(){consoleSelect(&bottom);}
void C(int c){consoleSetColor(NULL,(ConsoleColor)c);}

void mostrar(){
    T();
    consoleClear();

    C(CONSOLE_CYAN);
    printf("================================\n");
    printf("       CALCULADORA CIENTIFICA\n");
    printf("================================\n\n");

    C(CONSOLE_LIGHT_GRAY);
    printf("Modo: %s\n\n",grado?"DEG":"RAD");

    C(CONSOLE_WHITE);
    printf("  %s\n\n",entrada);

    C(CONSOLE_GREEN);
    printf("------------------------------\n");
    printf("  ");

    if(error) printf("ERROR");
    else printf("%.12g",valor);

    printf("\n");
    printf("------------------------------\n");

    C(CONSOLE_WHITE);
}

void pantalla(){
    B();
    consoleClear();

    C(CONSOLE_CYAN);
    printf("  SHIFT ALPHA   MODE\n");
    printf("  ------------------------\n");

    C(CONSOLE_YELLOW);
    printf("  x!   nPr   nCr   POL\n");
    printf("  a/b  √     x²    x^y\n");
    printf("  log  ln    sin   cos\n");
    printf("  tan  ENG   Ans   DRG\n");
    printf("  1/x  ABS   (     )\n");

    C(CONSOLE_WHITE);
    printf("  ------------------------\n");

    C(CONSOLE_GREEN);
    printf("  DEL  AC   M+   M-\n");

    C(CONSOLE_BLUE);
    printf("  ------------------------\n");

    C(CONSOLE_LIGHT_BLUE);
    printf("  7    8    9    ÷\n");
    printf("  4    5    6    ×\n");
    printf("  1    2    3    -\n");
    printf("  0    .    =    +\n");

    C(CONSOLE_WHITE);
    printf("  ------------------------\n");

    C(CONSOLE_YELLOW);
    printf("  PI   e    +/-  %c\n",'%');

    C(CONSOLE_WHITE);
    printf("\n B = salir\n");
}

void numero(char x){
    if(nuevo){
        entrada[0]=x;
        entrada[1]=0;
        nuevo=0;
    }else if(strlen(entrada)<20){
        int n=strlen(entrada);
        entrada[n]=x;
        entrada[n+1]=0;
    }

    valor=strtod(entrada,NULL);
    error=0;
}

void punto(){
    if(nuevo){
        strcpy(entrada,"0.");
        valor=0;
        nuevo=0;
        return;
    }

    if(!strchr(entrada,'.')){
        strcat(entrada,".");
        valor=strtod(entrada,NULL);
    }
}

void borrar(){
    int n;

    if(nuevo){
        strcpy(entrada,"0");
        valor=0;
        return;
    }

    n=strlen(entrada);

    if(n>1){
        entrada[n-1]=0;
        valor=strtod(entrada,NULL);
    }else{
        strcpy(entrada,"0");
        valor=0;
        nuevo=1;
    }
}

void operar(char x){
    if(op){
        double b=valor;

        if(op=='+')pendiente+=b;
        if(op=='-')pendiente-=b;
        if(op=='*')pendiente*=b;
        if(op=='/'&&b!=0)pendiente/=b;
        if(op=='/'&&b==0){
            error=1;
            strcpy(entrada,"ERROR");
            return;
        }
        if(op=='^')pendiente=pow(pendiente,b);

        valor=pendiente;
    }else{
        pendiente=valor;
    }

    op=x;
    nuevo=1;
}

void igual(){
    if(!op)return;

    operar(0);

    valor=pendiente;
    ans=valor;
    op=0;
    nuevo=1;

    sprintf(entrada,"%.12g",valor);
}

void unaria(int f){
    double x=valor;

    if(f==1){
        if(x<0)error=1;
        else valor=sqrt(x);
    }

    if(f==2)valor=x*x;

    if(f==3)valor=pow(x,valor);

    if(f==4){
        if(grado)valor=sin(x*PI/180.0);
        else valor=sin(x);
    }

    if(f==5){
        if(grado)valor=cos(x*PI/180.0);
        else valor=cos(x);
    }

    if(f==6){
        if(grado)valor=tan(x*PI/180.0);
        else valor=tan(x);
    }

    if(f==7){
        if(x<=0)error=1;
        else valor=log10(x);
    }

    if(f==8){
        if(x<=0)error=1;
        else valor=log(x);
    }

    if(f==9){
        if(x==0)error=1;
        else valor=1.0/x;
    }

    if(f==10)valor=fabs(x);

    if(f==11)valor=-x;

    if(f==12){
        int i;
        double r=1;

        if(x<0||x>12||floor(x)!=x){
            error=1;
        }else{
            for(i=1;i<=(int)x;i++)r*=i;
            valor=r;
        }
    }

    if(f==13)valor=ans;

    if(f==14)valor=PI;

    if(f==15)valor=2.718281828459;

    if(f==16)valor=x/100.0;

    if(error)strcpy(entrada,"ERROR");
    else sprintf(entrada,"%.12g",valor);

    nuevo=1;
}

void tecla(int x,int y){
    int c=x/32;
    int r=y/24;

    /*
       0-3 = funciones
       4-7 = numeros
    */

    if(r==0){
        if(c==0)unaria(12);
        if(c==1){}
        if(c==2)grado=!grado;
        if(c==3){}
        return;
    }

    if(r==1){
        if(c==0)unaria(1);
        if(c==1)unaria(2);
        if(c==2)operar('^');
        if(c==3){}
        return;
    }

    if(r==2){
        if(c==0)unaria(7);
        if(c==1)unaria(8);
        if(c==2)unaria(4);
        if(c==3)unaria(5);
        return;
    }

    if(r==3){
        if(c==0)unaria(6);
        if(c==1){}
        if(c==2)unaria(13);
        if(c==3)grado=!grado;
        return;
    }

    if(r==4){
        if(c==0)unaria(9);
        if(c==1)unaria(10);
        if(c==2){}
        if(c==3){}
        return;
    }

    if(r==5){
        if(c==0)borrar();
        if(c==1){
            strcpy(entrada,"0");
            valor=0;
            pendiente=0;
            op=0;
            nuevo=1;
            error=0;
        }
        if(c==2)mem+=valor;
        if(c==3)mem-=valor;
        return;
    }

    /*
       NUMEROS Y OPERADORES
    */

    if(r==6){
        if(c==4)numero('7');
        if(c==5)numero('8');
        if(c==6)numero('9');
        if(c==7)operar('/');
    }

    if(r==7){
        if(c==4)numero('4');
        if(c==5)numero('5');
        if(c==6)numero('6');
        if(c==7)operar('*');
    }

    if(r==8){
        if(c==4)numero('1');
        if(c==5)numero('2');
        if(c==6)numero('3');
        if(c==7)operar('-');
    }

    if(r==9){
        if(c==4)numero('0');
        if(c==5)punto();

        if(c==6)igual();

        if(c==7)operar('+');
    }

    if(r==10){
        if(c==4)unaria(14);
        if(c==5)unaria(15);
        if(c==6)unaria(11);
        if(c==7)unaria(16);
    }
}

int main(int argc,char**argv){

    /*
       BASE TECNICA DEL PROYECTO
       Se mantiene la misma estructura que ya funciono.
    */

    videoSetMode(MODE_0_2D);
    videoSetModeSub(MODE_0_2D);

    vramSetBankA(VRAM_A_MAIN_BG);
    vramSetBankC(VRAM_C_SUB_BG);

    consoleInit(
        &top,
        0,
        BgType_Text4bpp,
        BgSize_T_256x256,
        22,
        3,
        true,
        true
    );

    consoleInit(
        &bottom,
        0,
        BgType_Text4bpp,
        BgSize_T_256x256,
        22,
        3,
        false,
        true
    );

    /*
       Se conserva la inicializacion del teclado
       del proyecto que ya funcionaba.
    */

    kbd=keyboardInit(
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

    mostrar();
    pantalla();

    while(1){

        swiWaitForVBlank();
        scanKeys();

        if(keysDown()&KEY_B)
            break;

        if(keysDown()&KEY_TOUCH){

            touchPosition t;
            touchRead(&t);

            if(t.px<256&&t.py<192)
                tecla(t.px,t.py);

            mostrar();
            pantalla();
        }
    }

    keyboardShow();
    return 0;
}
