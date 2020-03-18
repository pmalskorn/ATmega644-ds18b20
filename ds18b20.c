#include <inttypes.h>
#include <util/crc16.h>
#include <util/delay.h>

#include "OneWire.h"
#include "ds18b20.h"


// Helper Functions
uint8_t ds18b20_checkcrc(uint8_t data[], uint8_t length)
{
	uint8_t crc = 0;
	for (uint8_t i = 0; i < length; i++){
		crc = _crc_ibutton_update(crc, data[i]);
	}
	return crc;
}


uint8_t ds18b20_crc_scratchpad(scratchpad_t *sp)
{
	uint8_t data[] = {
		sp->temp_lsb,
		sp->temp_msb,
		sp->th,
		sp->tl,
		sp->config,
		sp->reserved1,
		sp->reserved2,
		sp->reserved3,
		sp->crc
	};
	return ds18b20_checkcrc(data,9);
}

uint8_t ds18b20_crc_rom(rom_t rom)
{
	uint8_t data[8];
	for (uint8_t i = 0; i<8;i++)
	{
		data[i] = rom >> 8*i;
	}
	return ds18b20_checkcrc(data,8);
}


// ROM Commmands
rom_t ds18b20_read_rom(onewire_t *ow)
{
	onewire_reset(ow);
	onewire_write_byte(ow, READ_ROM);
	
	rom_t rom = 0;
	for(uint8_t i = 0; i < 8; i++){
		uint8_t byte = onewire_read_byte(ow);
		rom |= ((uint64_t)byte) << 8*i; 
	}

	return rom;
}


void ds18b20_match_rom(onewire_t *ow, rom_t rom){
	onewire_reset(ow);
	onewire_write_byte(ow, MATCH_ROM);
	for(uint8_t i = 0; i < 8; i++){
		onewire_write_byte(ow, rom >> 8*i);
	}
}


void ds18b20_skip_rom(onewire_t *ow){
	onewire_reset(ow);
	onewire_write_byte(ow, SKIP_ROM);
}


// Function Commands
void ds18b20_convert_t(onewire_t *ow){
	onewire_write_byte(ow, COVERT_T);
	while(!onewire_read_bit(ow));
}

void ds18b20_write_scratchpad(onewire_t *ow, scratchpad_t *sp){
	onewire_write_byte(ow, WRITE_SCRATCHPAD);
	
	onewire_write_byte(ow, sp->th);
	onewire_write_byte(ow, sp->tl);
	onewire_write_byte(ow, sp->config);
}


scratchpad_t ds18b20_read_scratchpad(onewire_t *ow){

	onewire_write_byte(ow, READ_SCRATCHPAD);
	scratchpad_t sp;

	sp.temp_lsb = onewire_read_byte(ow);
	sp.temp_msb = onewire_read_byte(ow);
	sp.th = onewire_read_byte(ow);
	sp.tl = onewire_read_byte(ow);
	sp.config = onewire_read_byte(ow);
	sp.reserved1 = onewire_read_byte(ow);
	sp.reserved2 = onewire_read_byte(ow);
	sp.reserved3 = onewire_read_byte(ow);
	sp.crc = onewire_read_byte(ow);
	
	return sp;
}


void ds18b20_copy_scratchpad(onewire_t *ow){
	onewire_write_byte(ow,COPY_SCRATCHPAD);
}


void ds18b20_recall_eeprom(onewire_t *ow){
	onewire_write_byte(ow,RECALL_E2);
	while(!onewire_read_bit(ow));
}