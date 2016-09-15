/*
 * File:   main16f.c
 * Author: Esteban
 *
 * Created on 15 de septiembre de 2016, 09:30 AM
 */
#include <xc.h>
#include <pic16f1827.h>
#include <stdlib.h>
#define _XTAL_FREQ 160000000
#pragma config FOSC = INTOSC
#pragma config WDTE = SWDTEN       // Watchdog Timer Enable (WDT disabled)
#pragma config CLKOUTEN = ON    // Clock Out Enable (CLKOUT function is enabled on the CLKOUT pin)
#include "USART.h"
void main(void) 
{
    WDTCONbits.WDTPS4=0;
    WDTCONbits.WDTPS3=1;
    WDTCONbits.WDTPS2=1;
    WDTCONbits.WDTPS1=1;
    WDTCONbits.WDTPS0=1;
    WDTCONbits.SWDTEN=0;
    CLRWDT();
    
    APFCON0bits.RXDTSEL=0;
    APFCON1bits.TXCKSEL=0;
    TRISBbits.TRISB1=1;
    TRISBbits.TRISB2=0;
    TRISBbits.TRISB3=1;
    
    TRISAbits.TRISA2=0;
    OSCCONbits.SCS1=1;
    OSCCONbits.IRCF3=1;
    OSCCONbits.IRCF2=1;
    OSCCONbits.IRCF1=1;
    OSCCONbits.IRCF0=1; 
    T2CONbits.T2OUTPS3=1;
    T2CONbits.T2OUTPS2=1;
    T2CONbits.T2OUTPS1=1;
    T2CONbits.T2OUTPS0=1;
    T2CONbits.T2CKPS1=1;
    T2CONbits.T2CKPS0=1;
    PR2=0xff;
    T2CONbits.TMR2ON=1;
    int t=0;
//    T2CON=0x7F;
    //BSR=0x00;
    TMR2IF=0;
    //BSR=0x00;
    PORTAbits.RA2=0;
    ANSA2=0;
    ANSELB=0x00;
    PIR1=0x00;
    OSFIF=0;
    unsigned long int c=0;
    unsigned char config=0x3C;
    char h;
    int task=0;
    int r=0;
    int c1=0;
    char g;
    int signal=1;
    int signala=1;
    OpenUSART(config,34);
    while(1)
    {
        if(TMR2IF==1)
        {
            TMR2IF=0;
            if(task==0)
            {
                cleanUSART();
                putsUSART("AT+CSQ\r\n");
                while(BusyUSART());
                ReadInicial:
                if( DataRdyUSART)
                {   
                    rt:
                    r=0;

                    c=ReadUSART();
                    //c=',';
                    if(c==' ')
                    {
                        /*if(RC0==1)
                        {
                            RC0=0;
                        }
                        else if(RC0==0)
                        {
                            RC0=1;
                        }*/
                        Read:
                        if(DataRdyUSART==1)
                        {
                            h=ReadUSART();
                            if(h=='1')
                            {
                                NOP();
                                Read2:
                                if(DataRdyUSART==1)
                                {
                                    g=ReadUSART();
                                    if(g==','|g=='0'|g=='1'|g=='2'|g=='3'|g=='4')
                                    {
                                        signala=1;
                                    }
                                    else
                                    {
                                        signala=2;
                                    }
                                }
                                else
                                {
                                    goto Read2;
                                }
                            }
                            else if(h=='2')
                            {
                                Read3:
                                if(DataRdyUSART==1)
                                {
                                    g=ReadUSART();
                                    if(g==',')
                                    {
                                        signala=1;
                                    }
                                    else if(g=='0'|g=='1'|g=='2'|g=='3'|g=='4')
                                    {
                                        signala=2;
                                    }
                                    else
                                    {
                                        signala=3;
                                    }
                                }
                                else
                                {
                                    goto Read3;
                                }
                            }
                            else if(h=='3')
                            {
                                Read4:
                                if(DataRdyUSART==1)
                                {
                                    g=ReadUSART();
                                    if(g==',')
                                    {
                                        signala=1;
                                    }
                                    else
                                    {
                                        signala=3;
                                    }
                                }
                                else
                                {
                                    goto Read4;
                                }
                            }
                            else
                            {
                                signala=1;
                                //RC0=1;
                            }

                        }
                        else
                        {
                            goto Read;
                        }
                        


                    }
                    else
                    {
                        goto ReadInicial;
                    }



                }
                else
                {
                    /*if(TMR3IF==1)
                    {
                        TMR3IF=0;
                        r=r+1;
                        if(r>10)
                        {
                            goto sen;
                        }
                        
                    }*/
                    
                    goto ReadInicial;
                }
                
            }
            sen:
            if(signal==1)
            {
                    TMR2IF=0;
                    if(c1==0)
                    {
                        RA2=1;
                        c1=c1+1;
                    }

                    else if(c1<9 & c1!=0)
                    {
                        RA2=0;
                        
                        c1=c1+1;
                    }
                    else if (c1==9)
                    {
                        c1=0;
                        signal=signala;
                    }

                

            }

            else if(signal==2)
            {
                
                    TMR2IF=0;
                    if(c1==0 | c1==2)
                    {
                        RA2=1;
                        c1=c1+1;
                    }
                    else if(c1==1 | c1<9)
                    {
                        RA2=0;
                        c1=c1+1;
                    }
                    else
                    {
                        RA2=0;
                        c1=0;
                        signal=signala;
                    }
                
            }
            else if(signal==3)
            {
                    TMR2IF=0;
                    if(c1==0 | c1==2 | c1==4)
                    {
                        RA2=1;
                        c1=c1+1;
                    }
                    else if(c1==1 |c1==3| c1<9)
                    {
                        RA2=0;
                        c1=c1+1;
                    }
                    else
                    {
                        RA2=0;
                        c1=0;
                        signal=signala;
                    }
                
            }
            task=task+1;
            if(task==8)
            {
                task=0;
            }
        }
        /*if(TMR2IF==1)
        {
            
            t=1;
            TMR2IF=0;
            TMR2IF=0;
            PIR1bits.TMR2IF=0;
            PIR1=0x00;
            if(PORTAbits.RA2==0)
            {
                PORTAbits.RA2=1;
                WriteUSART(0x31);
            }
            else if(PORTAbits.RA2==1)
            {
                PORTAbits.RA2=0;
            }
            
        }
        else if(TMR2IF==0)
        {
            t=0;
            c=c+1;
            t=0;
        }*/
    }   
}
