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
bignum mult(bignum , bignum);
bignum mulbyint(bignum , int);
bignum mulby10(bignum , int);
bignum mulby5(bignum , int);
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

/**
   * This method is used to find maximum number between two integers. 
   * @param x This is the first paramter to max method
   * @param y  This is the second parameter to max method
   * @return int This returns maximum of x and y.
   */

int max(int x , int y){
    return x > y ? x : y ;
}

/**
   * This method is used to remove preindexed zero from number. 
   * @param mag This is the first paramter to bypass method
   * @param len This is the second parameter to bypass method
   * @return void This function just removes leading zero from number.
   * like 0045454 to 45454
   */

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

 /**
   * This method is used to compare two bignum variable. 
   * @param a This is the first paramter to comparemag method
   * @param b This is the second parameter to comparemag method
   * @return int This function compare two bignum variable according to the sign of the variable and return 0,1,-1
   * @return 0:- if both the variable is having signum 0
   * @return 1:- if a is greater than b
   * @return -1:- if a is less than b
   */

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

 /**
   * This method is used to create copy of one bignum variable to another bignum variable. 
   * @param x This is the first paramter to createcopy method
   * @return bignum This function creates copy of variable
   */ 

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

/**
   * This method is used to create structure variable according to the passed value. 
   * @param val This is the first paramter to create method
   * @return bignum This function creates structure variable according to the sign of passed value and converts character to integer
   */
bignum str2bignum(char *val){
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

/**
   * This method is used to add two structure variable according to the sign of the variable. 
   * @param a This is the first paramter to add method
   * @param b This is the second paramter to add method
   * @return bignum This function is used to add two structure variable according to the sign.
   */

bignum add(bignum a , bignum b){
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

/**
   * This method is used to substract two structure variable according to the sign of the variable. 
   * @param a This is the first paramter to sub method
   * @param b This is the second paramter to sub method
   * @return bignum This function is used to substract two structure variable according to the sign.
   */
bignum sub(bignum a , bignum b){
     
    if(b.signum==0)
        return a;

    b.signum = b.signum == -1 ? 1:-1 ;

    if(a.signum==0){
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

/**
   * This method is used to multiply structure variable with 10 based on the times. 
   * @param a This is the first paramter to 
  method
   * @param times This is the second paramter to mulby10 method
   * @return bignum This function is used to multiply structure variable by 10 according to the times.
   * @like a=18 and times =2 result= 1800
   */
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

/**
   * This method is used to multiply structure variable with 10 based on the times. 
   * @param a This is the first paramter to 
  method
   * @param times This is the second paramter to mulby5 method
   * @return bignum This function is used to multiply structure variable by 10 according to the times.
   * @like a=18 and times =2 result= 1800
   */
bignum mulby5(bignum a, int times){
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

/**
   * This method is used to multiply structure variable with integer variable. 
   * @param a This is the first paramter to mulbyint method
   * @param x This is the second paramter to mulbyint method
   * @return bignum This function is used to multiply structure variable by passed integer variable.
   */

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

 /**
   * This method is used to multiply two structure variable. 
   * @param a This is the first paramter to mul method
   * @param b This is the second paramter to mul method
   * @return bignum This function is used to multiply two structure variable.
   */
bignum mult(bignum a , bignum b){
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

/**
   * This method is used to append one integer value to structure variable. 
   * @param b This is the first paramter to append method
   * @param x This is the second paramter to append method
   * @return bignum This function is used to append integer value to bignum variable.
   */
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

/**
   * This method is used to keep l length value in bignum variable. 
   * @param a This is the first paramter to subnum method
   * @param l This is the second paramter to subnum method
   * @return bignum here in this function l is the length which we have to keep in bignum variable 
   *  and the remaining part we have to remove from bignum.
   */
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

/**
   * This method is used to get divident from bignum divisor and bignum div
   * @param divisor This is the first paramter to divident method
   * @param div This is the second paramter to divident method
   * @return bignum this function used to divide two bignum variable
   */
bignum divident(bignum divisor , bignum div ){

    bignum l = str2bignum("1");
    bignum h = *createcopy(&divisor);
    bignum rem = reminder(divisor , div);
    divisor = sub(divisor,rem);
    bignum ONE = str2bignum("1");

    while(1){
        bignum mid = divby2(add(l,h));
        bignum tmp = mult(div , mid);
        int cmp = comparemag( divisor , tmp ) ;
        if (cmp==0)
            return mid;
        else if ( cmp == 1)
            l = add(mid,ONE);
        else
            h = sub(mid,ONE);
    }
    
}

 /**
   * This method is used to get reminder from bignum 
   * @param a This is the first paramter to reminder method
   * @param b This is the second paramter to reminder method
   * @return bignum this function used to get reminder from two bignum variable
   */

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

/**
   * This method is used to divide bignum by 2
   * @param a This is the first paramter to reminder method
   * @return bignum this function used to divide bignum by 2
   */
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

/**
   * This method is used to get mod of bignum by 2
   * @param a This is the first paramter to mod2 method
   * @return int this function used to get mod of 2 of a bignum variable
   */
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

/**
   * This method is used to get m^e mod n
   * @param m This is the first paramter to expmod method
   * @param e This is the second paramter to expmod method
   * @param n This is the third paramter to expmod method
   * @return bignum this function used to get exponent mod by the efficient way
   */
bignum expmod(bignum m , bignum e , bignum n){
    bignum ZERO = str2bignum("0");
    bignum ONE = str2bignum("1");

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


/**
   * This method is used to generate random number based on bits size
   * @param bits This is the first paramter to genrandom method
   * @return bignum this function used to generate random number based on bit size.
   */
bignum genrandom(int bits){

    bignum TWO = str2bignum("2");
    bignum ONE = str2bignum("1");
    bignum ZERO = str2bignum("0");
    bignum pow2 = str2bignum("0");

    bignum sum = str2bignum("0");
    while(bits--)
    {
        int bit = rand()%2;

        if(comparemag(pow2,ZERO)==0)
            pow2=ONE;

        else if(comparemag(pow2,ONE)==0)
            pow2=TWO;

        else pow2 = mult(pow2,TWO);

        
        if(bit==1){
            sum = add(sum , pow2);
        }

    }
    return sum;
}

/**
   * This method is used to return 0 or 1 based on the bignum p is prime or not
   * @param p This is the first paramter to millerrabin method
   * @param it This is the second paramter to millerrabin method
   * @return int from this function checking the bignum p is prime or not according to the it which is the accuracy of the test
   */
int millerrabin(bignum p, int it){

    bignum TWO = str2bignum("2");
    bignum ONE = str2bignum("1");

    if(comparemag(p, TWO)==-1)
        return 0;

    if( ( comparemag(p,TWO)==0 ) || ( mod2(p)==0 ) )
        return 0;

    bignum d = sub(p,ONE);
    int s=0;
    while(mod2(d)==0){
        d = divby2(d);
        s++;
    }

 witnessloop: do{
            bignum a = add(reminder( genrandom(100) , sub(p,str2bignum("4")) ), TWO);
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

/**
   * This method is used to convert bignum variable to long long integer
   * @param b This is the first paramter to to_int method
   * @return long long int from this function just converting bignum variable to long int
   */
long long int to_int(bignum b){
    long long int sum = 0;
    int i ;
    for (i = 0; i < b.length ; ++i)
    {
        sum = sum*10 + b.mag[i] ;
    }
    return sum;
}

/**
   * @param len This is the first paramter to genrandomprime method
   * @return bignum this function is used to generate random prime number based on the given length
   */
bignum genrandomprime(int len){
    bignum PRIME_PRODUCT = str2bignum("152125131763605");
    bignum ONE = str2bignum("1");
    bignum TWO = str2bignum("2");
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
         continue;
         }
         if(millerrabin(ran,2)==1)
            return ran;

         ran = add(ran,PRIME_PRODUCT);
                        
    }
    return ran;
}

 /**
   * This method is used to generate addition mod (a+b mod n)
   * @param a This is the first paramter to addmod method
   * @param b This is the second paramter to addmod method
   * @param n This is the third paramter to addmod method
   * @return bignum 
   */
bignum addmod(bignum a , bignum b , bignum n)
{
    return reminder(add(a,b),n);
}

 /**
   * This method is used to generate multiplication mod (a*b mod n)
   * @param a This is the first paramter to multmod method
   * @param b This is the second paramter to multmod method
   * @param n This is the third paramter to multmod method
   * @return bignum 
   */
bignum multmod(bignum a, bignum b, bignum n){
    return reminder(mult(a,b),n);
}

/**
   * print method to print any bignum variable
   * @param c This is the first paramter to print method
   * @return void 
   */
void print(bignum c){
    int i , j , k;
    if(c.signum==-1)
    printf("-" );
    if(c.signum==0)
        printf("0");
    for(i = 0 ; i < c.length ; i++)
        printf("%d",c.mag[i] );

    printf("\n");
}

/**
   * This method is used to get co-prime of two bignum variable
   * @param a This is the first paramter to multmod method
   * @param b This is the second paramter to multmod method
   * @return int either 0 or 1 
   */
int coprime(bignum a , bignum b){
    while(b.signum!=0){
        bignum t = b;
        b = reminder(a,b);
        a = t;
    }
    bignum ONE = str2bignum("1");
    if (comparemag(a,ONE)==0)
        return 1;
    else
        return 0;
}

/**
   * This method is used to generate key which is being used by the rsa algorith.
   * @param n This is the first paramter to keygen method
   * @param e This is the second paramter to keygen method
   * @param d This is the third paramter to keygen method
   * @param length This is the fourth paramter to keygen method
   * @return void 
   */
void keygen(bignum *n,bignum *e, bignum *d,int length){
    bignum ONE = str2bignum("1");
    bignum p = genrandomprime(length/2);
    bignum q = genrandomprime(length/2);
    *n = mult(p,q);
    bignum phi = mult(sub(p,ONE), sub(q,ONE));
    do{
        *e = reminder(genrandom(256),phi);
    }while( coprime (phi,*e)!=1);
    *d = inverse(*e , phi);

}

/**
   * This method is used to copy one bignum to another bignum
   * @param to This is the first paramter to copy method
   * @param from This is the second paramter to copy method
   * @return void
   */
void copy(bignum *to , bignum *from){
    to->length = from->length;
    to->signum = from->signum;
    free(to->mag);
    to->mag = (int*)malloc(sizeof(int)*to->length);
    int i;
    for(i = 0 ; i < to->length ; i++)
        to->mag[i] = from->mag[i];

}

/**
   * This method is used to inverse of a such that a * inverse(a) = 1 mod(n)
   * @param a This is the first paramter to inverse method
   * @param n This is the second paramter to inverse method
   * @return bignum
   */
bignum inverse(bignum a , bignum n){
    bignum ONE = str2bignum("1");
    bignum ZERO = str2bignum("0");
    bignum t = str2bignum("0");
    bignum newt = str2bignum("1");
    bignum newn = *createcopy(&n);
    bignum newa = *createcopy(&a);
    
    bignum p_i_2 = str2bignum("0");
    bignum p_i_1 = str2bignum("1");
    do{
        bignum q_i_2 = divident(newn , newa);
        bignum rem = reminder(newn , newa);
        bignum p = sub(p_i_2 , reminder( mult(p_i_1,q_i_2), n));
        if(p.signum==-1){
            p.signum = 1;
            bignum tmp = sub( n , p);
            copy(&p , &tmp);

        }

        copy(&p_i_2 ,&p_i_1 );
        copy(&p_i_1 , &p );
        copy(&newn ,&newa );
        copy(&newa , &rem);
        if(comparemag(rem,ONE)==0)
            return p;
    }while(1);
}

/**
   * This method is used encrypt the data
   * @param m This is the first paramter to RSAEncrypt method
   * @param e This is the second paramter to RSAEncrypt method
   * @param n This is the third paramter to RSAEncrypt method
   * @return bignum
   */
bignum RSAEncrypt(bignum m , bignum e , bignum n){
    return expmod(m , e, n);
}

/**
   * This method is used decrypt the data
   * @param c This is the first paramter to RSADecrypt method
   * @param d This is the second paramter to RSADecrypt method
   * @param n This is the third paramter to RSADecrypt method
   * @return bignum
   */
bignum RSADecrypt(bignum c , bignum d , bignum n){
    return expmod(c , d, n);
}


/**
   * testRSA is used to generate public and private key and then generating the random message 
   * and then encrypt and decrypt that generated random message
   * @param length This is the first paramter to testRSA method
   * @return void
   */

void testRSA(int length){
    bignum n , e , d ;
    printf("generating primes P and Q\n");
    keygen(&n , &e , &d, length);

    printf("\n\nValue of public key (e , n ) \n");
    print(e);
    print(n);
    printf("\n\nValue of Private key ( d , n ) \n");
    print(d);
    print(n);

    //bignum message = reminder(genrandom(512),n);
    bignum message = str2bignum("123456789");

    printf("\n\nRandom generated messege \n");
    print(message);

    bignum cipher=RSAEncrypt(message , e , n);
    printf("\n\nCipher text \n");
    print(cipher);

    message = RSADecrypt(cipher , d , n);
    printf("\n\nCipher Decrypted\n");
    print(message);
}

// main function to call the above utility functions.

int main(){

    //testRSA is the main function by which the RSA algorithm and all the above utility function is called
    testRSA(512);
    

    return 0;
}
