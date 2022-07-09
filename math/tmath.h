#define MAXLEN 1001
int prime[MAXLEN] = { 0, };

unsigned long gcd(unsigned long x, unsigned long y){
	while(x % y){
		unsigned long tmp = x;
		x = y;
		y = tmp % y;
	}

	return y;
}

unsigned long mpow(long x, int d, unsigned long N){
	if(d == 0) return 1;
	if(d == 1) return x;
	
	//if d is odd
	if(d & 1) return (x * mpow(x, d-1, N)) % N;

	unsigned long half = mpow(x, d >> 1, N);
	
	return (half * half) % N;
}

void get_prime(){
	int len = 0;
	int i = 2;
	
	prime[len++] = 2;
	
	while(len < MAXLEN){
		i ++;
		for(int j = 2; j < i; j ++){
			if( (j*j) % i == 1 ){ 
				if(j != i-1) goto NEXT;

				for(int k = 0; k < len; k ++)
					if(i % prime[k] == 0) goto NEXT;

				prime[len ++] = i;
				goto NEXT;
			}
		}
		NEXT:
		continue;
	}

	return ;
}