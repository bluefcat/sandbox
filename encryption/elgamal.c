#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../math/tmath.h"

int main(){
	srand((unsigned int)time(NULL));
	get_prime();

	unsigned long long p, g, Y;			//public keys
	unsigned long long x;				//private keys
	unsigned long long k;				//tool's value
	unsigned long long M, D, Ca, Cb;	//messages

	//get public & private key
	p = prime[rand() % MAXLEN];
	g = (rand() % (p-1))+1;
	x = (rand() % (p-1))+1;
	Y = mpow(g, x, p);

	//calculate tool's value k
	for(int i = 2; i < p; i ++){
		if(gcd(p-1, i) == 1){
			k = i;
			break;
		}
	}

	
	//Message confirm
	//M = rand() % n;
	M = 240;
	Ca = mpow(g, k, p);
	Cb = (mpow(Y, k, p) * M) % p;
	D = (Cb * mpow(Ca, p-1-x, p)) % p;

	//print line
	printf("p: %lld, g: %lld, Y: %lld\n", p, g, Y);
	printf("x: %lld\n", x);

	printf("===Elgamal===\n");
	printf("Message: %lld\n", M);
	printf("Crypto: %lld, %lld\n", Ca, Cb);
	printf("Decrypt: %lld\n", D);

	printf("this right?: %s\n", (M==D)? "Yes": "NO");

	//Testing
	for(int c = 0; c <= 100; c ++){
		M = rand() % p;
		Ca = mpow(g, k, p);
		Cb = (mpow(Y, k, p) * M) % p;
		D = (Cb * mpow(Ca, p-1-x, p)) % p;

		if((c != 0) && (c % 10 == 0)){
			printf("[%4d] M: %lld\tC: (%lld, %lld)\tD: %lld\n", c, M, Ca, Cb, D);
		}

		if(M != D){
			printf("Wrong Decoding\n");
			printf("M: %lld\nC: (%lld, %lld)\nD: %lld\n", M, Ca, Cb, D);
		}
	}

	return 0;
}