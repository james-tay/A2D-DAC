/*
  Operation :

  The MCP4921 expects 16-bits of input, starting from MSB. Where :

    bit15   : A/B - 0=DAC(a), 1=DAC(b)
    bit14   : BUF - Vref input buffer 0=unbuffered, 1=bufferd
    bit13   : GA - output gain select 0=2x gain, 1=1x gain
    bit12   : SHDN - output power down control bit
    bit11-0 : DAC Data

  Note that for correct operation, bit 15 and 14 are set to 0 while bit 13
  and bit 12 must be 1.

  Compile :
    % gcc -o mcp4921 mcp4921.c -lwiringPi
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
  unsigned char data[2] ;

  if (argc != 3)
  {
    printf ("Usage: %s <channel 0|1> <value 0-4095>\n", argv[0]) ;
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

  data[0] = (0x30 + ((value >> 8) & 0x0f)) ;
  data[1] = (value & 0xff) ;
  result = wiringPiSPIDataRW (channel, data, 2) ;
  close (fd) ;

  if (result == 2)
    return (0) ;	/* success */
  else
  {
    printf ("WARNING: wiringPiSPIDataRW() failed. %s\n", strerror(errno)) ;
    return (1) ;	/* fault */
  }
}

