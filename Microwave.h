#ifndef Microwave_H_INCLUDED
#define Microwave_H_INCLUDED

void setup();
void selectMode(char *read);
void getWeight(char* food,int* mins,int* secs);
void getTime(int* mins,int* secs);
void startCooking(int mins,int secs);
void finishCooking();


#endif 