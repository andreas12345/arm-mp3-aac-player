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

#ifndef _DAC_H_
#define _DAC_H_

#define AIC23B_REG_LINVOL		0x00
#define AIC23B_REG_RINVOL		0x01
#define AIC23B_REG_LOUTVOL		0x02
#define AIC23B_REG_ROUTVOL		0x03
#define AIC23B_REG_AN_PATH		0x04
#define AIC23B_REG_DIG_PATH		0x05
#define AIC23B_REG_POWER		0x06
#define AIC23B_REG_DIG_FORMAT	0x07
#define AIC23B_REG_SRATE		0x08
#define AIC23B_REG_DIG_ACT		0x09
#define AIC23B_REG_RESET		0x0F

#define USB (1 << 0)
#define BOSR (1 << 1)
#define SR0 (1 << 2)
#define SR1 (1 << 3)
#define SR2 (1 << 4)
#define SR3 (1 << 5)

#define MAX_BUFFERS 3
#define DAC_BUFFER_MAX_SIZE 2400
extern short dac_buffer[MAX_BUFFERS][DAC_BUFFER_MAX_SIZE];
extern int dac_buffer_size[MAX_BUFFERS];

void dac_reset();
int dac_get_writeable_buffer();
int dac_get_readable_buffer();
int dac_readable_buffers();
int dac_writeable_buffers();
int dac_busy_buffers();
int dac_fill_dma();

void dac_enable_dma();
void dac_disable_dma();
int next_dma_empty();
int first_dma_empty();
void set_first_dma(short *buffer, int n);
void set_next_dma(short *buffer, int n);
int dma_endtx(void);
void dac_init(void);

#endif /* _DAC_H_ */
