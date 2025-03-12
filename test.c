
/* Example application of Columbo Simple Serial Library
 * Copyright 2003 Marcin Siennicki <m.siennicki@cloos.pl>
 * see COPYING file for details */

#include <stdio.h>
#include <unistd.h>

#include "cssl.h"

#define LOGGING 0

/* if it is time to finish */
static int finished=0;


/* example callback, it gets its id, buffer, and buffer length */
static void callback(int id,
		     uint8_t *buf,
		     int length)
{
    printf("Callback\n");
    int i;

    for(i=0;i<length;i++) {

	switch (buf[i]) {

	case 0x04:  /* Ctrl-D */
	    finished=1;
	    return;

	case '\r': /* replace \r with \n */
	    buf[i]='\n';

	}

	putchar(buf[i]);
    }

    fflush(stdout);
}


int main()
{
    cssl_t *serial;

    cssl_start(LOGGING);
    
    serial=cssl_open("/dev/ttyACM0",callback,0,
		     115200,8,0,1);

    if (!serial) {
	printf("%s\n",cssl_geterrormsg());
	return -1;
    }

    uint8_t data[4] = {'F', '0', '0', 'B'};

    cssl_putdata(serial, &data[0], 4);
    //cssl_putstring(serial, );

    while (!finished)
	pause();

    printf("\n^D - we exit\n");

    cssl_close(serial);
    cssl_stop();

    return 0;
}
