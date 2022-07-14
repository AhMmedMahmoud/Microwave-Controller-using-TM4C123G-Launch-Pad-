#include "Delay.h"

void delay_milli(int n)
{
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<3180;j++){}
}
 
void delay_micro(int n)
{
	int i,j;
	for(i=0;i<n;i++)
	for(j=0;j<3;j++){}
}