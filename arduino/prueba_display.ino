//se declaran los pines a usar para el display
int LEDs[] = {25,16,5,18,21,3,1};
//25 g, 16 f, 5 e, 18 d, 3 c, 21 b, 1 a    

//se declaran los arreglos que forman los dígitos
int zero[] = {0, 1, 1, 1, 1, 1, 1};   // cero
int one[] = {0, 0, 0, 0, 1, 1, 0};   // uno
int two[] = {1, 0, 1, 1, 0, 1, 1};   // dos
int three[] = {1, 0, 0, 1, 1, 1, 1};   // tres
int four[] = {1, 1, 0, 0, 1, 1, 0};   // cuatro 
int five[] = {1, 1, 0, 1, 1, 0, 1};   // cinco
int six[] = {1, 1, 1, 1, 1, 0, 1};   // seis
int seven[] = {0, 0, 0, 0, 1, 1, 1};   // siete
int eight[] = {1, 1, 1, 1, 1, 1, 1}; // ocho
int nine[] = {1, 1, 0, 1, 1, 1, 1};   // nueve
int ten[] = {1, 1, 1, 0, 1, 1, 1};   // diez, A
int eleven[] = {1, 1, 1, 1, 1, 0, 0};   // once, b
int twelve[] = {0, 1, 1, 1, 0, 0, 1};   // doce, C
int thirteen[] = {1, 0, 1, 1, 1, 1, 0};   // trece, d
int fourteen[] = {1, 1, 1, 1, 0, 0, 1};   // catorce, E
int fifteen[] = {1, 1, 1, 0, 0, 0, 1};   // quince, F

//se variable declara contador
unsigned char contador = 0;

//función que despliega del 0 al F
void segment_display(unsigned char valor){
    switch(valor){
        case 0:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], zero[i]); break;
        case 1:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], one[i]); break;
        case 2:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], two[i]); break;
        case 3:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], three[i]); break;
        case 4:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], four[i]); break;
        case 5:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], five[i]); break;
        case 6:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], six[i]); break;
        case 7:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], seven[i]); break;
        case 8:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], eight[i]); break;
        case 9:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], nine[i]); break;
        case 10:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], ten[i]); break;
        case 11:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], eleven[i]); break;
        case 12:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], twelve[i]); break;
        case 13:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], thirteen[i]); break;
        case 14:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], fourteen[i]); break;
        case 15:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], fifteen[i]); break; 
        default:
            for (int i = 0; i<7; i++) digitalWrite(LEDs[i], zero[i]); break;          
    }
}

void setup() 
{
  //se inicializan los pines como salida
  for (int i = 0; i<7; i++) pinMode(LEDs[i], OUTPUT);
}

void loop() 
{
    //se muestra contador, retardo y se incrementa contador
    segment_display(contador);
    delay(1000);
    if(contador < 15) contador++;
    else contador = 0;
}