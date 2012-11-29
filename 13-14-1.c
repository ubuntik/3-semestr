#include <unistd.h>
int main(void){ 
	(void)fork();
	while(1);
	return 0;
}
