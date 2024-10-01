/*
 * MCP4725.c
 *
 * Created: 8/23/2024 3:58:46 PM
 * Author : ADMIN
 */ 
#define F_CPU 8000000UL
#define SCL_clock 100000UL
#define MCP4725_BASEADDRESS (0xC0) //device base address

//i2c settings


#define I2C_READ    1

/** defines the data direction (writing to I2C device) in i2c_start(),i2c_rep_start() */
#define I2C_WRITE   0

//registers
#define MCP4725_WRITEDAC 0x40
#define MCP4725_WRITEDACEEPROM 0x60
#define MCP4725_READDAC 0x03

//set the power down mode
#define MCP4725_POWERDOWN1K 1
#define MCP4725_POWERDOWN100K 100
#define MCP4725_POWERDOWN500K 500
#define MCP4725_POWERDOWNMODE MCP4725_POWERDOWN1K

#include <compat/twi.h>
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

		
#define SCL_CLOCK  100000L


/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void i2c_init(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}/* i2c_init */


/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char i2c_start(unsigned char address)
{
    uint8_t   twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;

}/* i2c_start */


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
void i2c_start_wait(unsigned char address)
{
    uint8_t   twst;


    while ( 1 )
    {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    	// wait until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);
    
    	// wail until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
    	{    	    
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        
	        // wait until stop condition is executed and bus released
	        while(TWCR & (1<<TWSTO));
	        
    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

}/* i2c_start_wait */


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 

 Input:   address and transfer direction of I2C device
 
 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start( address );

}/* i2c_rep_start */


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}/* i2c_stop */


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   0 write successful 
            1 write failed
*************************************************************************/
unsigned char i2c_write( unsigned char data )
{	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}/* i2c_readAck */


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}/* i2c_readNak */

void mcp4725_init(uint8_t address, uint8_t resetic) {
	#if MCP4725_I2CINIT == 1
	//init i2c
	i2c_init();
	_delay_us(10);
	#endif

	//set power down mode
	i2c_start_wait(I2C_WRITE + MCP4725_BASEADDRESS + address);
#if MCP4725_POWERDOWNMODE == MCP4725_POWERDOWN1K
	i2c_write(0b01000010);
#elif MCP4725_POWERDOWNMODE == MCP4725_POWERDOWN100K
	i2c_write(0b01000100);
#elif MCP4725_POWERDOWNMODE == MCP4725_POWERDOWN500K
	i2c_write(0b01000110);
#endif
	if(resetic) {
		i2c_write(0b00000000);
		i2c_write(0b00000000);
	}
	i2c_stop();
}

/*
 * set the raw value to output fast mode
 */
void mcp4725_setrawoutputfast(uint8_t address, uint16_t rawoutput) {
	//write raw output value to register
	i2c_start_wait(I2C_WRITE + MCP4725_BASEADDRESS + address);
	i2c_write((uint8_t)(rawoutput>>8));
	i2c_write((uint8_t)(rawoutput));
	i2c_stop();
}

/*
 * set a voltage value to output fast mode
 */
void mcp4725_setvoltagefast(uint8_t address, double voltage, double dacref) {
	uint16_t rawoutput = 0;

	//get the raw output
	rawoutput = voltage*4096/dacref;
	if (rawoutput > 4095)
		rawoutput = 4095;

	//write to chip
	mcp4725_setrawoutputfast(address, rawoutput);
}

/*
 * set the raw value to output
 */
void mcp4725_setrawoutput(uint8_t address, uint16_t rawoutput, uint8_t savetoeeprom) {
	//write raw output value to register
	i2c_start_wait(I2C_WRITE + MCP4725_BASEADDRESS + address);
	if(savetoeeprom) {
		i2c_write(MCP4725_WRITEDACEEPROM);
	} else {
		i2c_write(MCP4725_WRITEDAC);
	}
	i2c_write((uint8_t)(rawoutput>>4));
	rawoutput = (rawoutput<<12);
	i2c_write((uint8_t)(rawoutput>>8));
	i2c_stop();
}

/*
 * set a voltage value to output
 */
void mcp4725_setvoltage(uint8_t address, double voltage, double dacref, uint8_t savetoeeprom) {
	uint16_t rawoutput = 0;

	//get the raw output
	rawoutput = voltage*4096/dacref;
	if (rawoutput > 4095)
		rawoutput = 4095;

	//write to chip
	mcp4725_setrawoutput(address, rawoutput, savetoeeprom);
}

int main(void)
{
	uint8_t address = 0x60;  
	
	mcp4725_init(address, 0);
	double dacref = 3.3;     
	//double voltage = 0.0;   
	double step = 0.01;      
	//uint8_t savetoeeprom = 0; 
	
	
	
	
	
    while (1) 
    {
		for (double voltage = 0.0; voltage <= 3.0; voltage += step) {
			mcp4725_setvoltagefast(address, voltage, dacref);
			
			_delay_ms(10);
		}
		
    }
}

