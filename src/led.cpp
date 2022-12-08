#include "led.h"
const int led[3] = {15, 14, 0}; // biru, hijau, merah

led::led(){
    for(int i = 0; i <=2; i++){
        pinMode(led[i], OUTPUT);
    }
}

led::~led(){}

void led::led_merah(int x){
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], x);
    digitalWrite(led[2], HIGH);
}

void led::led_hijau(int x){
    digitalWrite(led[0], HIGH);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], x);
}
void led::led_biru(int x){
    digitalWrite(led[0], x);
    digitalWrite(led[1], HIGH);
    digitalWrite(led[2], HIGH);
}