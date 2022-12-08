#ifndef led_H
#define led_H
#include <Arduino.h>

class LED {
    public:
        LED();
        ~LED();
        void led_merah(int x);
        void led_biru(int x);
        void led_hijau(int x);
};
#endif
