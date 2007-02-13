/*
  Copyright (C) 2006 Andreas Schwarz <andreas@andreas-s.net>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include "Board.h"
#include "systime.h"
#include "serial.h"

#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>

#include "control.h"
#include "ff.h"
#include "diskio.h"
#include "fileinfo.h"
#include "player.h"
#include "profile.h"
#include "dac.h"
#include "aacdec.h"

#include "raw_aac_data.h"

#define TCK  1000                           /* Timer Clock  */
#define PIV  ((MCK/TCK/16)-1)               /* Periodic Interval Value */

FATFS fs;
static FIL file;
static DIR dir;
static FILINFO fileinfo;

void write_benchmark(void)
{
	assert(f_open(&file, "testdata.raw", FA_WRITE|FA_CREATE_ALWAYS) == FR_OK);
	puts("opened");
	
	{
		WORD BytesWritten;
		FRESULT res;
		
		static const BYTE dummy_ff_block[512] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
			0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
		
		PROFILE_START("writing 512 kB");
		for(int i=0; i<1000; i++) {
			res = f_write(&file, dummy_ff_block, sizeof(dummy_ff_block), &BytesWritten);
			if (res)
				iprintf("result: %i\n", res);
		}
		PROFILE_END();
	}
	
	assert(f_close(&file) == FR_OK);
}

void record(void)
{
	int writeable_buffer, readable_buffer;
	WORD BytesWritten;

	dac_init();
	
	dac_set_srate(44100);
	puts("deleting file");
	assert(f_unlink("testdata.raw") == FR_OK);
	puts("opening file");
	assert(f_open(&file, "testdata.raw", FA_WRITE|FA_CREATE_ALWAYS) == FR_OK);
	puts("opened");
	
	*AT91C_SSC_PTCR = AT91C_PDC_RXTEN;
	
	for(int i=0; i<500;) {
		if ( (readable_buffer = dac_get_readable_buffer()) != -1 ) {
			i++;
			iprintf("write: %i\n", readable_buffer);
			assert(f_write(&file, (BYTE *)dac_buffer[readable_buffer], 2*dac_buffer_size[readable_buffer], &BytesWritten) == FR_OK);
		}

		if (*AT91C_SSC_RCR == 0) {
			writeable_buffer = dac_get_writeable_buffer();
			if (writeable_buffer == -1) {
				continue;
			}
			*AT91C_SSC_RPR = (unsigned int)dac_buffer[writeable_buffer];
			*AT91C_SSC_RCR = DAC_BUFFER_MAX_SIZE;
			dac_buffer_size[writeable_buffer] = DAC_BUFFER_MAX_SIZE;
		}
		
		if (*AT91C_SSC_RNCR == 0) {
			writeable_buffer = dac_get_writeable_buffer();
			if (writeable_buffer == -1) {
				continue;
			}
			*AT91C_SSC_RNPR = (unsigned int)dac_buffer[writeable_buffer];
			*AT91C_SSC_RNCR = DAC_BUFFER_MAX_SIZE;
			dac_buffer_size[writeable_buffer] = DAC_BUFFER_MAX_SIZE;
		}

	}
	assert(f_close(&file) == FR_OK);
	puts("finished");
}

void codec_bypass(void)
{
	short x[3] = {0, 0, 0};
	short y;
	short h[3] = {32767, 0, 0};
	
	dac_init();
	dac_set_srate(8000);

	*AT91C_SSC_THR = 0;
	while(1)
	{
		//iprintf("endrx: %i, txempty: %i", *AT91C_SSC_PTCR & AT91C_SSC_ENDRX)
		x[2] = x[1];
		x[1] = x[0];
		while(!(*AT91C_SSC_SR & AT91C_SSC_ENDRX));
		x[0] = *(short *)(AT91C_SSC_RHR);
		//x[0] = rand();
		y = ((x[0]*h[0]) >> 16) + ((x[1]*h[1]) >> 16) + ((x[2]*h[2]) >> 16);
		while(!(*AT91C_SSC_SR & AT91C_SSC_TXEMPTY));
		*AT91C_SSC_THR = *(unsigned short *)(&y);
		//iprintf("y: %i\n", y);
	}
}

void codec_buffered_bypass(void)
{
	int writeable_buffer;

	dac_init();
	
	dac_set_srate(44100);
	*AT91C_SSC_PTCR = AT91C_PDC_RXTEN;
	dac_enable_dma();
	while(1) {
		while(dac_fill_dma() == 0);

		if (*AT91C_SSC_RCR == 0) {
			writeable_buffer = dac_get_writeable_buffer();
			if (writeable_buffer == -1) {
				continue;
			}
			*AT91C_SSC_RPR = (unsigned int)dac_buffer[writeable_buffer];
			*AT91C_SSC_RCR = DAC_BUFFER_MAX_SIZE;
			dac_buffer_size[writeable_buffer] = DAC_BUFFER_MAX_SIZE;
		}
		
		if (*AT91C_SSC_RNCR == 0) {
			writeable_buffer = dac_get_writeable_buffer();
			if (writeable_buffer == -1) {
				continue;
			}
			*AT91C_SSC_RNPR = (unsigned int)dac_buffer[writeable_buffer];
			*AT91C_SSC_RNCR = DAC_BUFFER_MAX_SIZE;
			dac_buffer_size[writeable_buffer] = DAC_BUFFER_MAX_SIZE;
		}

	}
}

void test_raw_aac(void)
{
	static HAACDecoder hAACDecoder;
	static AACFrameInfo aacFrameInfo;
	static unsigned char *readPtr;
	static short outBuf[2300];
	static int bytesLeft;
	int res;
	
	bytesLeft = sizeof(raw_aac_data);
	readPtr = (unsigned char *)raw_aac_data;
	
	assert(hAACDecoder = AACInitDecoder());
	puts("decoder initialized");
		
	memset(&aacFrameInfo, 0, sizeof(AACFrameInfo));
	aacFrameInfo.nChans = 2;
	aacFrameInfo.sampRateCore = 44100;
	aacFrameInfo.profile = AAC_PROFILE_LC;
	assert(AACSetRawBlockParams(hAACDecoder, 0, &aacFrameInfo) == 0);
	
	for(int i=0; i<100; i++) {
		res = AACDecode(hAACDecoder, &readPtr, &bytesLeft, outBuf);
		iprintf("AACDecode: %i\nbytesLeft: %i\n", res, bytesLeft);
		AACGetLastFrameInfo(hAACDecoder, &aacFrameInfo);
		iprintf("Bitrate: %i\n", aacFrameInfo.bitRate);
		iprintf("%i samples\n", aacFrameInfo.outputSamps);
	}
	
	
	while(1);
	
}

int main(void)
{
	AT91PS_PMC  pPMC  = AT91C_BASE_PMC;
	AT91PS_PIO  pPIOA = AT91C_BASE_PIOA;
	AT91PS_RSTC pRSTC = AT91C_BASE_RSTC;
	
	// Enable the clock for PIO and UART0
	pPMC->PMC_PCER = ( ( 1 << AT91C_ID_PIOA ) | ( 1 << AT91C_ID_US0 ) ); // n.b. IDs are just bit-numbers
	
	// Configure the PIO Lines corresponding to LED1 to LED4
	//pPIOA->PIO_PER = LED_MASK; // pins controlled by PIO (GPIO)
	//pPIOA->PIO_OER = LED_MASK; // pins outputs
	
	// Turn off the LEDs. Low Active: set bits to turn off 
	//pPIOA->PIO_SODR = LED_MASK;
	
	// enable reset-key on demo-board 
	pRSTC->RSTC_RMR = (0xA5000000 | AT91C_RSTC_URSTEN);

	systime_init();
	key_init();
	uart0_init();
	
	memset(&fs, 0, sizeof(FATFS));
	FatFs = &fs;

	iprintf("f_mountdrv: %i\n", f_mountdrv());
	
	memset(&dir, 0, sizeof(DIR));
	assert(f_opendir(&dir, "/") == FR_OK);

	puts("\nDirectory of 'root':");
	while ( f_readdir( &dir, &fileinfo ) == FR_OK && fileinfo.fname[0] ) {
		iprintf( "%s ( %li bytes )\n" ,
			fileinfo.fname,
			fileinfo.fsize ) ;
	}
	
	{
		DWORD clust;

	    // Get free clusters
	    assert(f_getfree(&clust) == FR_OK);

	    // Get free bytes
	    iprintf("%lu bytes available on the disk.\n", clust * FatFs->sects_clust * 512);
	}

	//codec_bypass();
	//record();
	player_init();
	play();
	//test_raw_aac();
	while(1);

	return 0; /* never reached */
}
