
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

void cat(int f, char *s)
{
	char buf[1024];
	long n;

	while((n=read(f, buf, (long)sizeof buf))>0)
		if(write(1, buf, n)!=n)
			printf("write error copying %s: \n", s);
	if(n < 0)
		printf("error reading %s: \n", s);
}

int main(int argc, char *argv[])
{
	int f, i;

	argv[0] = "cat";
	if(argc == 1)
		cat(0, "<stdin>");
	else 
        for(i=1; i<argc; i++)
        {
		    f = open(argv[i], O_RDONLY);
		    if(f < 0)
			    printf("can't open %s: \n", argv[i]);
		    else
            {
			    cat(f, argv[i]);
			    close(f);
		    }
	    }
	return 0;
}