#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct s2
{
	int signum;
	int *mag;
	int length;
} bignum;

int max(int , int);

bignum add(bignum , bignum);
bignum sub(bignum , bignum);
bignum mul(bignum , bignum);
bignum mulbyint(bignum , int);
bignum mulby10(bignum , int);
bignum reminder(bignum , bignum);
int comparemag(bignum , bignum);
void bypass(int** , int*);
bignum *createcopy(bignum *);
bignum append(bignum , int);
bignum subnum(bignum , int );
bignum addmod(bignum , bignum , bignum);
bignum multmod(bignum, bignum, bignum);
bignum divby2(bignum);
bignum expmod(bignum , bignum , bignum);
bignum genrandom(int);
bignum inverse(bignum , bignum);
void copy(bignum* , bignum*);
void print(bignum ); 


int max(int x , int y){
	return x > y ? x : y ;
}

// 0045454
void bypass(int **magg , int *len){
	int curr  = 0;
	int length = *len;
	int *mag = *magg;
	while(curr<length && mag[curr]==0)
		curr++;

	int i ;
	for(i = 0 ; (i+curr) < length ; i++ )
		mag[i] = mag[curr+i];

	length = length - curr;
	*len = length;
	*magg = mag;

}

int comparemag(bignum a , bignum b){
	if(a.signum==0 && b.signum==0)
		return 0;

	if(a.length > b.length)
		return 1;
	if(b.length > a.length)
		return -1;

	int i = 0;
	for(i = 0 ; i < a.length ; i++)
	{
		if (a.mag[i] > b.mag[i])
			return 1;
		if (a.mag[i] < b.mag[i])
			return -1;

	}
	return 0;
}

bignum *createcopy(bignum *x){
	bignum *b = (bignum*)malloc(sizeof(bignum));
	b->signum = x->signum;
	b->length = x->length;
	b->mag = (int*)malloc(sizeof(int)*b->length);
	int i;
	for(i = 0 ; i < b->length ; i++)
		b->mag[i]=x->mag[i];

	return b;
}
bignum create(char *val){
	bignum *b = (bignum*)malloc(sizeof(bignum));
	int sign;
	int *mag;
	int length;

	int curr = 0;
	
	if (val[0]=='-')
		sign=-1;
	else
		sign=1;

	if(val[0]=='-' || val[0]=='+')
		curr++;

	while(val[curr]=='0' && curr<strlen(val))
		curr++;

	length = strlen(val) - curr;

	if(length==0)
		sign=0;

	mag = (int*) malloc(sizeof(int)*length);

	int i , j;
	for(i=0,j=curr ; j<strlen(val) ; i++,j++)
		mag[i] = val[j]-'0';

	b->signum = sign;
	b->mag = mag;
	b->length = length;
	return *b;
}

// a = 1234
// b =   14
bignum add(bignum a , bignum b){
	// a = createcopy(a);
	// b = createcopy(b);
	if(b.signum==0)
		return a;
	if(a.signum==0)
		return b;

	if(a.signum!=b.signum)
		return sub(a,b);

	int maxlen = max(a.length , b.length);
	int lena = a.length;
	int lenb = b.length;
	bignum *c = (bignum*)malloc(sizeof(bignum));
	int sign;
	int *mag;
	int length = maxlen+1;
	mag = (int*)malloc(sizeof(int)*length);

	int carry = 0, len=length;
	while (lena>0 || lenb>0){

		int x = lena>0 ? a.mag[--lena] : 0 ;
		int y = lenb>0 ? b.mag[--lenb] : 0 ;
		int z = (x + y + carry)%10;
		carry = (x + y + carry)/10;
		mag[--len] = z;

	}
	mag[--len]=carry;

	if (mag[0]==0)
		bypass(&mag , &length);

	c->mag = mag;
	c->signum = a.signum;
	c->length = length;
	return *c;
}

bignum sub(bignum a , bignum b){
	 
	if(b.signum==0)
		return a;

	b.signum = b.signum == -1 ? 1:-1 ;

	if(a.signum==0){
		//b.signum = b.signum == -1 ? 1:-1 ;
		return b;
	}
	
	if(a.signum==b.signum){
		return add(a,b);
	}

	bignum *c = (bignum*) malloc (sizeof(bignum));
	int length = 0;
	int *mag ;
	int signum = 0;
	if(comparemag(a,b)==0){
		c->signum=0;
		c->mag = mag;
		c->length=0;
		return *c;
	}

	bignum x , y ;
	x = comparemag(a,b)==1 ? a:b;
	y = comparemag(a,b)==1 ? b:a;
	length = max(a.length , b.length);
	mag = (int*)malloc(sizeof(int)*length);
	int len = length;

	int lenx = x.length;
	int leny = y.length;

	int borrow = 0;
	while(lenx>0 || leny>0){
		int a = lenx>0 ? x.mag[--lenx] : 0;
		int b = leny>0 ? y.mag[--leny] : 0;
		int c;
		// (a-borrow-b)>=0 ? c = (a-borrow-b), borrow=0 : c = 10+(a-borrow-b),borrow=1 ;

		if ( (a-borrow-b)>=0 ){
			c = (a-borrow-b); borrow=0;
		}
		else{
			c = 10+(a-borrow-b) ; borrow=1;
		}

		mag[--len] = c;
	}
	c->mag = mag;
	c->length = length;
	c->signum = x.signum;
	bypass(&c->mag, &c->length);
	return *c;
}

bignum mulby10(bignum a, int times){
	bignum *c = (bignum*)malloc(sizeof(bignum));
	int *mag = (int*)malloc(sizeof(int)*(a.length+times));
	int len = a.length+times;
	int i = 0;
	for(i = 0 ; i < a.length ; i++)
		mag[i] = a.mag[i];

	while(times--)
		mag[i++] = 0;

	c->mag = mag;
	c->length = len;
	c->signum = a.signum;
	return *c;

}
// 134
//   9
bignum mulbyint(bignum a , int x){
	bignum *b = (bignum*)malloc(sizeof(bignum));
	if(x==0){
		b->signum=0;
		b->length=0;
		return *b;
	}
	
	int *mag = (int*)malloc(sizeof(int)*(a.length+1));
	int len = a.length;

	int carry=0 , i ;

	for(i = len ; i>0 ; i--){
		int m = a.mag[i-1]*x + carry ;
		carry = m/10;
		m = m%10;
		mag[i]=m;
	}

	mag[0]=carry;
	b->mag=mag;
	b->length = a.length+1;
	b->signum = a.signum;
	bypass(&b->mag,&b->length);
	return *b;
}
bignum mul(bignum a , bignum b){
	// a = createcopy(a);
	// b = createcopy(b);

	if(a.signum==0)
		return a;
	if(b.signum==0)
		return b;

	bignum *x = (bignum*)malloc(sizeof(bignum));
	int i = 0;
	for(i = 0 ; i < b.length ; i++){
		bignum c = mulbyint(a,b.mag[i]);
		c = mulby10(c,b.length-i-1);
		*x = add(*x,c);
	}
	bypass(&x->mag, &x->length);
	return *x;

}

bignum append(bignum b , int x){
	
	int *mag = (int*)malloc(sizeof(int)*(b.length+1));
	int i;
	for(i = 0 ; i < b.length ; i++)
		mag[i] = b.mag[i];
	mag[i] = x;
	b.length++;
	free(b.mag);
	b.mag = mag;
	return b;
	
}

bignum subnum(bignum a , int l){
	bignum *c = (bignum*)malloc(sizeof(bignum));
	c->length = l;
	c->signum = a.signum;
	c->mag = (int*) malloc(sizeof(int)*l);
	int i;
	for(i = 0 ; i < l ; i++)
		c->mag[i] = a.mag[i];
	return *c;
}

// bignum divident(bignum a , bignum b){

// 	bignum divi = create("0") ;
	

// 	if(b.signum==0)
// 	{
// 		printf("Exception case of 0 divisor\n");
// 		exit(0);
// 	}

// 	if(a.signum==0)
// 	{
// 		return divi ;
// 	}

// 	if (comparemag(a,b)==-1){

// 		return divi;
// 	}
// 	if(comparemag(a,b)==0){

// 		divi = add(divi , create("1"));
// 		return divi;
// 	}

// 	int len = 1 ;
// 	bignum t;
// 	// a is 10500001
// 	// b is 70
// 	while(comparemag(a,b)>=0){
// 		divi = mulby10(divi,1);
// 		len=1;
// 		t = subnum(a,len);
// 		while(comparemag(t,b)==-1){
// 			len++;
// 			t = subnum(a,len);
// 		}
// 		if(len>2)
// 			divi = mulby10(divi , len-2);

// 		print(t);
// 		int num = 0;
// 		bignum tmpb = *createcopy(&b);
// 		print(tmpb);
// 		while(comparemag(t,tmpb)>=0){
// 			print(tmpb);
// 			num++;
// 			tmpb = mulbyint(b,num);
// 		}
// 		num--;
// 		// 
// 		char ch[2];
// 		ch[0]=(char)(num+48);
// 		ch[1]='\0';
// 		divi = add(divi,create(ch));
// 		print(divi);
// 		// free(tmpb);
// 		print(b);
// 		tmpb = mulbyint(b,num);
// 		tmpb = mulby10(tmpb , a.length - len);
// 		print(tmpb);
// 		a = sub(a,tmpb);
// 		print(a);
// 		print(b);
// 	}

// 	return divi;

// }

bignum divident(bignum divisor , bignum div ){

	bignum l = create("1");
	bignum h = *createcopy(&divisor);
	bignum rem = reminder(divisor , div);
	divisor = sub(divisor,rem);
	bignum ONE = create("1");

	while(1){
		bignum mid = divby2(add(l,h));
		bignum tmp = mul(div , mid);
		int cmp = comparemag( divisor , tmp ) ;
		if (cmp==0)
			return mid;
		else if ( cmp == 1)
			l = add(mid,ONE);
		else
			h = sub(mid,ONE);
	}
	
}


bignum reminder(bignum a , bignum b){


	if(b.signum==0)
	{
		printf("Exception case of 0 divisor\n");
		exit(0);
	}
	if(a.signum==0)
	{
		return a;
	}

	if (comparemag(a,b)==-1){

		return a;
	}
	if(comparemag(a,b)==0){

		bignum *c = (bignum*)malloc(sizeof(bignum));
		c->length=0;
		c->signum =  0 ;
		
		return *c;
	}

	// a is 1543
	// b is 9
	int len = b.length;

	bignum t=  subnum(a,len);
	while(comparemag(t,b)==-1){
		len++;
		t = subnum(a,len);
	}
	int appendzeroes = a.length - len ;
	t = mulby10(b , appendzeroes);
	a = sub(a,t);
	return reminder(a,b);
}

//8685080500001
bignum divby2(bignum a){
	bignum *c = (bignum*)malloc(sizeof(bignum));

	int len = a.length;
	int *mag = (int*)malloc(sizeof(int)*len);
	int signum = a.signum;
	int carry = 0;
	int i;
	for(i = 0 ; i < len ; i++){
		int x = carry*10 + a.mag[i];
		int v = x/2;
		carry = x%2;
		mag[i]=v;
	}
	c->mag = mag;
	c->signum = signum;
	c->length = len;
	bypass(&c->mag , &c->length);
	return *c;
}

int mod2(bignum a){

	int len = a.length;
	int signum = a.signum;
	int carry = 0;
	int i;
	for(i = 0 ; i < len ; i++){
		int x = carry*10 + a.mag[i];
		carry = x%2;
	}
	return carry;
}

bignum expmod(bignum m , bignum e , bignum n){
	bignum ZERO = create("0");
	bignum ONE = create("1");

	if(comparemag(e,ONE)==0)
		return m;
	if(comparemag(e,ZERO)==0)
		return ONE;

	int r = mod2(e);
	e = divby2(e);

	bignum tmp = expmod(m , e , n);
	tmp = multmod(tmp,tmp,n);

	if(r==1)
		tmp = multmod(tmp,m,n);
	return tmp;



}

bignum genrandom(int bits){

	bignum TWO = create("2");
	bignum ONE = create("1");
	bignum ZERO = create("0");
	bignum pow2 = create("0");

	bignum sum = create("0");
	while(bits--)
	{
		int bit = rand()%2;

		if(comparemag(pow2,ZERO)==0)
			pow2=ONE;

		else if(comparemag(pow2,ONE)==0)
			pow2=TWO;

		else pow2 = mul(pow2,TWO);

		
		if(bit==1){
			sum = add(sum , pow2);
		}

	}
	return sum;
}
int millerrabin(bignum p, int it){

	bignum TWO = create("2");
	bignum ONE = create("1");

	if(comparemag(p, TWO)==-1)
		return 0;

	if( ( comparemag(p,TWO)==2 ) || ( mod2(p)==0 ) )
		return 0;

	bignum d = sub(p,ONE);
	int s=0;
	while(mod2(d)==0){
		d = divby2(d);
		s++;
	}

 witnessloop: do{
			bignum a = add(reminder( genrandom(100) , sub(p,create("4")) ), TWO);
			bignum x = expmod(a , d , p);
			if( (comparemag(x , ONE)==0) || ( comparemag(x, sub(p,ONE))==0 ))
				continue;
			int i;
			for(i = 0 ; i < s-1 ; i++){
				x = multmod(x,x,p);
				if(comparemag(x,ONE)==0)
					return 0;
				if(comparemag(x,sub(p,ONE))==0)
					goto witnessloop;
			}
			return 0;

	}while(it--);
	return 1;
}

long long int to_int(bignum b){
	long long int sum = 0;
	int i ;
	for (i = 0; i < b.length ; ++i)
	{
		sum = sum*10 + b.mag[i] ;
	}
	return sum;
}

bignum genrandomprime(int len){
	bignum PRIME_PRODUCT = create("152125131763605");
	bignum ONE = create("1");
	bignum TWO = create("2");
	bignum ran = genrandom(len);
	
	if(mod2(ran)==0)
		ran = add(ran,ONE);

	while(1){
		 bignum rem = reminder(ran ,PRIME_PRODUCT);
		 long long int r = to_int(rem);
		 if ((r%3==0)  || (r%5==0)  || (r%7==0)  || (r%11==0) ||
                        (r%13==0) || (r%17==0) || (r%19==0) || (r%23==0) ||
                        (r%29==0) || (r%31==0) || (r%37==0) || (r%41==0)) {

		 	ran = add(ran , TWO);
		 // printf("continueing\n");
		 continue;
		 }
		 if(millerrabin(ran,10)==1)
		 	return ran;

		 ran = add(ran,PRIME_PRODUCT);
                        
	}
	return ran;
}
bignum addmod(bignum a , bignum b , bignum n){
	return reminder(add(a,b),n);
}

bignum multmod(bignum a, bignum b, bignum n){
	return reminder(mul(a,b),n);
}
void print(bignum c){
	int i , j , k;
	printf("(%d)",c.signum );
	for(i = 0 ; i < c.length ; i++)
		printf("%d",c.mag[i] );

	printf("\n\n");
}

int coprime(bignum a , bignum b){
	while(b.signum!=0){
		bignum t = b;
		b = reminder(a,b);
		a = t;
	}
	bignum ONE = create("1");
	if (comparemag(a,ONE)==0)
		return 1;
	else
		return 0;
}
void keygen(bignum *n,bignum *e, bignum *d,int length){
	bignum ONE = create("1");
	bignum p = genrandomprime(length/2);
	printf("genrated p \n");
	print(p);
	bignum q = genrandomprime(length/2);
	printf("genrated q \n");
	print(q);
	*n = mul(p,q);
	printf("N generated\n");
	print(*n);
	bignum phi = mul(sub(p,ONE), sub(q,ONE));
	printf("Phi generated\n");
	print(phi);
	// *d = reminder(genrandom(length), *n);
	do{
		//*e = reminder(genrandomprime(15),phi);
		*e = genrandomprime(10);
		
	}while( coprime (phi,*e)!=1);
	printf("genrated e \n");
	print(*e);

	*d = inverse(*e , phi);
	printf("d generated\n");
	print(*d);


	// while(comparemag(multmod(*e , *d , phi),ONE)!=0){
	// 	printf("genrated d \n");
	// 	*d = add(*d, ONE);
	// 	if (comparemag(*d , phi)<=0)
	// 		*d = reminder(*d , phi);
	// }

}
void copy(bignum *to , bignum *from){
	to->length = from->length;
	to->signum = from->signum;
	free(to->mag);
	to->mag = (int*)malloc(sizeof(int)*to->length);
	int i;
	for(i = 0 ; i < to->length ; i++)
		to->mag[i] = from->mag[i];

}
bignum inverse(bignum a , bignum n){
	bignum ONE = create("1");
	bignum ZERO = create("0");
	bignum t = create("0");
	bignum newt = create("1");
	bignum newn = *createcopy(&n);
	bignum newa = *createcopy(&a);
	
	bignum p_i_2 = create("0");
	bignum p_i_1 = create("1");
	do{
		// print(newn);
		// print(newa);
		bignum q_i_2 = divident(newn , newa);
		bignum rem = reminder(newn , newa);
		// print(rem);
		bignum p = sub(p_i_2 , reminder( mul(p_i_1,q_i_2), n));
		// print(p);
		if(p.signum==-1){
			p.signum = 1;
			bignum tmp = sub( n , p);
			copy(&p , &tmp);

		}

		// print(q_i_2);
		// print(p);
		// printf("============================================\n");
		copy(&p_i_2 ,&p_i_1 );
		copy(&p_i_1 , &p );
		copy(&newn ,&newa );
		copy(&newa , &rem);
		if(comparemag(rem,ONE)==0)
			return p;
	}while(1);

}

bignum RSAEncrypt(bignum m , bignum e , bignum n){
	return expmod(m , e, n);
}
bignum RSADecrypt(bignum c , bignum d , bignum n){
	return expmod(c , d, n);
}

void testRSA(int length){
	// bignum n = create("3233"), e=create("17") , d=create("2753") ;
	bignum n , e , d ;
	keygen(&n , &e , &d, length);

	printf("====================\n");
	print(n);
	printf("====================\n");
	print(e);
	printf("====================\n");
	print(d);
	printf("====================\n");
	bignum message = genrandom(10);

	printf("Random generated messege \n");
	print(message);

	bignum cipher=RSAEncrypt(message , e , n);
	printf("CIpher text generation\n");
	print(cipher);

	message = RSADecrypt(cipher , d , n);
	printf("Cipher Decrypted\n");
	print(message);
}
int main(){
	bignum ONE = create("1");
	bignum a = create("17");
	bignum b = create("3120");
	bignum z = inverse(a , b );
	bignum n , e , d ;
	testRSA(512);
	// print(z);
	// print(e);
	return 0;
}