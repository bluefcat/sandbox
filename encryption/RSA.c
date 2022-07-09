#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../math/tmath.h"

int main(){
	srand((unsigned int)time(NULL));
	get_prime();

	unsigned long long p, q, phi; 		//tool value
	unsigned long long n, e, d;			//keys
	unsigned long long M, D, C;			//message
	
	//prime confirm
	p = prime[rand() % MAXLEN];
	q = prime[rand() % MAXLEN];
	
	phi = (p-1)*(q-1);

	//get public key
	n = p*q;
	
	for(int i = 65537; i > 2; i --){
		if(gcd(i, phi) == 1){
			e = i;
			break;
		}
	}

	//get private key
	for(int i = 2; i < phi; i ++){
		if((i * e) % phi == 1){
			d = i;
			break;
		}
	}

	//Message confirm
	//M = rand() % n;
	M = 240;
	C = mpow(M, e, n);
	D = mpow(C, d, n);

	//print line
	printf("p: %lld, q: %lld\n", p, q);
	printf("phi: %lld\n", phi);
	printf("public key (n, e): (%lld, %lld)\n", n, e);
	printf("private key: %lld\n", d);

	printf("===RSA===\n");
	printf("Message: %lld\n", M);
	printf("Crypto: %lld\n", C);
	printf("Decrypt: %lld\n", D);

	printf("this right?: %s\n", (M==D)? "Yes": "NO");

	//Testing
	for(int c = 0; c <= 100; c ++){
		M = rand() % n;
		C = mpow(M, e, n);
		D = mpow(C, d, n);
		
		if((c != 0) && (c % 10 == 0)){
			printf("[%4d] M: %lld\tC: %lld\tD: %lld\n", c, M, C, D);
		}

		if(M != D){
			printf("Wrong Decoding\n");
			printf("M: %lld\nC: %lld\nD: %lld\n", M, C, D);
		}
	}

	return 0;
}