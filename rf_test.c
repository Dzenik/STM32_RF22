#include <stdbool.h>
#include "rfm22.h"
#include "rfm22reg.h"

bool test_read_write(void)
{
	uint8_t val;
	spiWrite(RF22_REG_42_CHECK_HEADER0, 10);
	val = spiRead(RF22_REG_42_CHECK_HEADER0);
	if(val == 10)
	{
		spiWrite(RF22_REG_42_CHECK_HEADER0, 5);
		val = spiRead(RF22_REG_42_CHECK_HEADER0);
		if(val == 5) return true;
		else return false;
	}
	else return false;
}

bool test_burst_read_write(void)
{
	uint8_t buf[] = "Hello";
	uint8_t data[sizeof(buf)];
	uint8_t i;

	spiBurstWrite(RF22_REG_7F_FIFO_ACCESS, buf, sizeof(buf));

	spiBurstRead(0, data, sizeof(data));

	for(i=0; i<sizeof(data); i++)
	{
		if(data[i] != buf[i]) return false;
	}

	return true;
}

bool test_wut(void)
{
	setWutPeriod(10000, 0, 0); // 10000, 0, 0 -> 1 secs
	spiWrite(RF22_REG_07_OPERATING_MODE1, RF22_ENWT);

	while (1)
	{
		uint8_t val = spiRead(RF22_REG_04_INTERRUPT_STATUS2);
		if (val & RF22_IWUT)
			return false;
	}
	spiWrite(RF22_REG_07_OPERATING_MODE1, 0);
	return true;
}

