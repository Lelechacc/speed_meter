#ifndef KEY_H
#define KEY_H

#define KEY_SCAN_INTERVAL 5
#define KEY_DEBOUNCE_INTERVAL 20

typedef enum
{
    KEY_POWER,
    KEY_1,
    KEY_2, 
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_MAX,
} Key_ID_t;

typedef void (*Key_Function_t)(void *);

void Key_Init();

void Key_RegisterCallback(Key_ID_t key, Key_Function_t callback, void* arg);

#endif