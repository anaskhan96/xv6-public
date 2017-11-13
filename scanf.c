#include "types.h"
#include "stat.h"
#include "user.h"

static void
getc(int fd, char* c)
{
  read(fd, c, 1);
}

int my_atoi(char *str, int base)
{
    int res = 0;  // Initialize result
    int sign = 1;  // Initialize sign as positive
    int i = 0, j;  // Initialize index of first digit
    static char digits[] = "0123456789ABCDEF";
    int b = 0;
    if(base==10){  
    	// If number is negative, then update sign
    	if (str[0] == '-')
    	{
        	sign = -1;  
        	i++;  // Also update index of first digit
    	}
      
    	// Iterate through all digits and update the result
    	for (; str[i] != '\0'; ++i)
        	res = res*base + str[i] - '0';
    
    	// Return result with sign
    	return sign*res;
    }
    else{
    	for(;str[i]!='\0';++i){
    		if(str[i]-'0'<=9)
    			b=str[i]-'0';
    		else{
    			for(j=10;j<=16;j++)
    				if(digits[j]==str[i] || (digits[j]+32)==str[i])
    					b=j;
    		}
    		res = res*base + b;
    	}
    	return res;
    }
}

static void
scanint(int fd, uint xx, int base, int sgn)
{
  char *buf = (char*)malloc(sizeof(char)*16);
  char k;
  int digits = 0;
  do{
    getc(fd, &k);
    *(buf+digits) = k;
    digits++;
  }while(k != '\n');
  // Making the last '\n' character a null one 
  *(buf+digits-1) = '\0';
  int number = my_atoi(buf, base);
  if(base == 10||base == 16){
  	int *p = (int*)xx;
  	*p = number;
  }
}

void
scanf(int fd, char *fmt, ...)
{
  char *s;
  int c, i, state;
  uint *ap;

  state = 0;
  ap = (uint*)(void*)&fmt + 1;
  for(i = 0; fmt[i]; i++){
    c = fmt[i] & 0xff;
    if(state == 0){
      if(c == '%'){
        state = '%';
      } else {
        getc(fd, (char*)c);
      }
    } else if(state == '%'){
      if(c == 'd'){
        scanint(fd, *ap, 10, 1);
        ap++;
      } else if(c == 'x' || c == 'p'){
        scanint(fd, *ap, 16, 0);
        ap++;
      } else if(c == 's'){
        s = (char*)*ap;
        //ap++;
        if(s == 0)
          s = "(null)";
        char k;
        int chars;
        do{
          getc(fd, &k);
          *(s+chars) = k;
          chars++;
        }while(k != '\n');
        // Making the last '\n' character a null one 
        *(s+chars-1) = '\0';
      } else if(c == 'c'){
        getc(fd, (char*)*ap);
        ap++;
      } else if(c == '%'){
        getc(fd, (char*)c);
      }
      state = 0;
    }
  }
}
