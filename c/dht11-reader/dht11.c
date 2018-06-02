 /*
 * butdht11ton.c:
 * Simple test program to test the wiringPi functions 
 * Author: Andrew Didinchuk <adidinchuk@gmail.com>
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************

*/

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAXTIMINGS	85
// DHT data pin
#define DHT		1
/* 8bit integral humidity data
 * 8bit decimal humidity data
 * 8bit integral temp data
 * 8bit decimal temp data
 * 8bit check sum
*/
int dht11_dat[5] = { 0, 0, 0, 0, 0 };

void read_dht11_dat()
{
	uint8_t laststate	= HIGH;
	uint8_t counter		= 0;
	uint8_t j		= 0, i;
	float	fahrenheit;

	// clear data storage
	dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

	// pull pin down for 18 milliseconds to request data
	pinMode( DHT, OUTPUT );
	digitalWrite( DHT, LOW );
	delay( 18 );
    // pull pin high for response data
	digitalWrite( DHT, HIGH );
	delayMicroseconds( 40 );
	// read pin
	pinMode( DHT, INPUT );
	
	for ( i = 0; i < MAXTIMINGS; i++ ){
		counter = 0;
		// detect high signal duration length ( if > 50um --> 1, else --> 0)
		while ( digitalRead( DHT ) == laststate ){
			counter++;
			delayMicroseconds( 1 );
			// break on constant high
			if ( counter == 255 )
				break;
		}
		// update current signal state
		laststate = digitalRead( DHT );
		// break on constant high
		if ( counter == 255 )
			break;

		// DHT start signal is 80us low, 80us down
		// every bit starts with 50us low 
		// ignore first 3 transitions and every start bit read signal (50um low)
		if ( (i >= 4) && (i % 2 == 0) ){
			// left shift bit and update padded 0 with a 1 if signal > 50um
			dht11_dat[j / 8] <<= 1;
			if ( counter > 50 )
				dht11_dat[j / 8] = 1;
			j++;
		}
	}

	/*
	 * Ensure that a total of 40 bits were processed
	 * Validate checksum 
	 */
	if ( (j >= 40) && 
	     (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) ){
		fahrenheit = dht11_dat[2] * 9. / 5. + 32;
		printf( "Humidity = %d.%d %% Temperature = %d.%d *C (%.1f *F)\n",
			dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], fahrenheit );
	}else
		printf( "Data not good, skip\n" );
}

int main( void ){
	printf( "Raspberry Pi wiringPi DHT11 Temperature test program\n" );
	if ( wiringPiSetup() == -1 )
		exit( 1 );
	while ( 1 )	{
		read_dht11_dat();
		delay( 1000 ); /* wait 1sec to refresh */
	}
	return(0);
}
