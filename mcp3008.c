/*
  Operation :

  The MCP3008 reads a total of 5 bits, waits 2 clock cycles and then starts
  returning a 10-bit result. Thus, our 3 byte "data" buffer is layed out as
  follows :

         1 2 3 4 5 6 7 8   1 2 3 4 5 6 7 8   1 2 3 4 5 6 7 8
         ---------------   ---------------   ---------------
   xmit                s   m x y z - -
   recv                                a b   c d e f g h i j

  Where:

   s     : start bit, must be 1.
   m     : mode, 0=single 1=differential
   x/x/y : 3-bits to indicate input channel to sample
   a-j   : 10-bits containing the result

  Compile :
    % gcc -o mcp3008 mcp3008.c -lwiringPi
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringPiSPI.h>

#define SPI_SPEED_HZ 500000

int main (int argc, char **argv)
{
  int fd ;
  int result ;
  int channel ;
  int value ;
  unsigned char data[3] ;

  if (argc != 3)
  {
    printf ("Usage: %s <channel 0|1> <input 0-7>\n", argv[0]) ;
    return (1) ;
  }

  channel = atoi (argv[1]) ;
  value = atoi (argv[2]) ;

  fd = wiringPiSPISetup (channel, SPI_SPEED_HZ) ;
  if (fd < 0)
  {
    fprintf (stderr, "FATAL! wiringPiSPISetup() failed for channel '%s'.\n",
             argv[1]) ;
    return (1) ;
  }

  data[0] = 1 ;
  data[1] = (8 + value) << 4 ;
  data[2] = 0 ;
  result = wiringPiSPIDataRW (channel, data, 3) ;
  close (fd) ;

  if (result == 3)
  {
    printf ("%d\n", ((data[1] & 3) << 8) + data[2]) ;
    return (0) ;	/* success */
  }
  else
  {
    printf ("WARNING: wiringPiSPIDataRW() failed. %s\n", strerror(errno)) ;
    return (1) ;	/* fault */
  }
}

