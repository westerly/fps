#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <time.h>
#include <algorithm>

static class Helper
{
public:

	// On suppose a<b, Permet d'obtenir un nombre entier random compris entre [a;b[ 
	static inline int rand_a_b(int a, int b){ return rand() % (b - a) + a; }
	// On suppose a<b, Permet d'obtenir un nombre floatant random compris entre [a;b[ 
	static inline double frand_a_b(double a, double b){ return (rand() / (double)RAND_MAX) * (b - a) + a; }

	// retourne la somme des n premiers nombres naturels
	static inline int sum_n(int n){
		if (n == 0){
			return 0;
		}
		else{
			return n + sum_n(n-1);
		}

	}
};

#endif

