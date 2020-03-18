#ifndef DS18_H_
#define DS18_H_

#include "OneWire.h"

typedef uint64_t rom_t;

typedef struct scratchpad_t
{
	volatile uint8_t temp_lsb;
	volatile uint8_t temp_msb;
	volatile uint8_t th;
	volatile uint8_t tl;
	volatile uint8_t config;
	volatile uint8_t reserved1;
	volatile uint8_t reserved2;
	volatile uint8_t reserved3;
	volatile uint8_t crc;
} scratchpad_t;

// ROM Commmands
#define SEARCH_ROM		0xF0
#define READ_ROM		0x33
#define MATCH_ROM		0x55
#define SKIP_ROM		0xCC
#define ALARM_SEARCH	0xEC

// Function Commmands
#define COVERT_T			0x44
#define WRITE_SCRATCHPAD	0x4E
#define READ_SCRATCHPAD		0xBE
#define COPY_SCRATCHPAD		0x48
#define RECALL_E2			0xB8
#define READ_POWER_SUPPLY	0xB4

// Helper Functions
uint8_t ds18b20_checkcrc(uint8_t data[], uint8_t length);
uint8_t ds18b20_crc_scratchpad(scratchpad_t *sp);
uint8_t ds18b20_crc_rom	(rom_t rom);
//uint8_t ds18b20_


// ROM Commmands
void	ds18b20_search_rom(onewire_t *ow);
rom_t	ds18b20_read_rom(onewire_t *ow);
void	ds18b20_match_rom(onewire_t *ow, rom_t rom);
void	ds18b20_skip_rom(onewire_t *ow);
void	ds18b20_alarm_search(onewire_t *ow);


// Function Commands
void			ds18b20_convert_t(onewire_t *ow);
void			ds18b20_write_scratchpad(onewire_t *ow, scratchpad_t *sp);
scratchpad_t	ds18b20_read_scratchpad(onewire_t *ow);
void			ds18b20_copy_scratchpad(onewire_t *ow);
void			ds18b20_recall_eeprom(onewire_t *ow);
void			ds18b20_read_power_supply(onewire_t *ow);




#endif /* DS18_H_ */