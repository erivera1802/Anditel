#include <xc.h>
void OpenUSART(unsigned char config,unsigned int spbrg)
{
    TXSTA = 0;           // Reset USART registers to POR state
    RCSTA = 0;
    
    if(config&0x01)      // Sync or async operation
    TXSTAbits.SYNC = 1;
    
    if(config&0x02)      // 8- or 9-bit mode
    {
        TXSTAbits.TX9 = 1;
        RCSTAbits.RX9 = 1;
    }
    
    if(config&0x04)      // Master or Slave (sync only)
        TXSTAbits.CSRC = 1;
    if(config&0x04)
        BAUDCONbits.BRG16=1; //!6 or 8 bits
    else
        BAUDCONbits.BRG16=0;

    if(config&0x08)      // Continuous or single reception
        RCSTAbits.CREN = 1;
    else
        RCSTAbits.SREN = 1;
    
    if(config&0x10)      // Baud rate select (asychronous mode only)
        TXSTAbits.BRGH = 1;
  
    PIR1bits.TXIF = 0;
    
    if(config&0x20)  // Address Detect Enable
        RCSTAbits.ADDEN = 1;
    
	if(config&0x40)      // Interrupt on receipt
        PIE1bits.RCIE = 1;
    else
        PIE1bits.RCIE = 0;
    /*if(config&0x04)
        BRG16=1;
    else
        BRG16=0;*/
    
    PIR1bits.RCIF = 0;
    
    if(config&0x80)      // Interrupt on transmission
        PIE1bits.TXIE = 1;
    else
        PIE1bits.TXIE = 0;
    
    SPBRG = spbrg;       // Write baudrate to SPBRG1
    SPBRGH = spbrg >> 8; // For 16-bit baud rate generation
    TXSTAbits.TXEN = 1;  // Enable transmitter
    RCSTAbits.SPEN = 1;  // Enable receiver

}

void WriteUSART(char data)
{
    TXREG = data;      // Write the data byte to the USART
}
char ReadUSART(void)
{
    char data=RCREG;
    return data;
}
char BusyUSART(void)
{
  if(!TXSTAbits.TRMT)  // Is the transmit shift register empty
    return 1;          // No, return FALSE
  return 0;            // Return TRUE
}
void putsUSART( char *data)
{
  do
  {  // Transmit a byte
    while(BusyUSART());
    WriteUSART(*data);
  } while( *data++ );
}

void cleanUSART (void)
{	char data;
    RCSTAbits.CREN = 0;    // reset UART continuous receive to avoid overrun state
	RCSTAbits.CREN = 1;	
	data = ReadUSART(); //read data
	data = ReadUSART(); //read data
	data = ReadUSART(); //read data
	data=0;
	RCSTAbits.CREN = 0;    // reset UART continuous receive to avoid overrun state
	RCSTAbits.CREN = 1;	
	data = ReadUSART(); //read data
	data = ReadUSART(); //read data
	data = ReadUSART(); //read data
	data=0;
}



