/**
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

void sendWord(char* word);

int main(void){
    int contador = 0, antirrebote = 0, antirrebote2 = 0;
    char* datos;

    //Establece el reloj del microcontrolador
    SysCtlClockSet(SYSCTL_SYSDIV_4|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //Habilita el periferico GPIOF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    //Habilita pines de salida (LEDS R, B, G respectivamente)
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Habilita pines de entrada (Boton SW1 del pad)
    HWREG (GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0X1;
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    /*Enable the peripheral UART Module 0*/
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);


    /* Make the UART pins be peripheral controlled. */
    GPIOPinConfigure( GPIO_PB0_U1RX );
    GPIOPinConfigure( GPIO_PB1_U1TX );
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    /* Sets the configuration of a UART. */
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
    (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

    sendWord("AT\r\n");
    SysCtlDelay(30000000);

    sendWord("ATE0\r\n");
    SysCtlDelay(30000000);

    sendWord("AT+CWMODE=1\r\n");
    SysCtlDelay(30000000);

    sendWord("AT+CWJAP=");
    UARTCharPut(UART1_BASE,'"');
    sendWord("The End Is Near");
    UARTCharPut(UART1_BASE,'"');
    UARTCharPut(UART1_BASE,',');
    UARTCharPut(UART1_BASE,'"');
    sendWord("SanFer2494!");
    UARTCharPut(UART1_BASE,'"');
    sendWord("\r\n");
    SysCtlDelay(30000000);

    while(1){
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0 && antirrebote2 == 0){

            antirrebote2 = 1;
            sendWord("AT+CIPSTART=");
            UARTCharPut(UART1_BASE,'"');
            sendWord("TCP");
            UARTCharPut(UART1_BASE,'"');
            UARTCharPut(UART1_BASE,',');
            UARTCharPut(UART1_BASE,'"');
            sendWord("192.168.1.22");
            UARTCharPut(UART1_BASE,'"');
            sendWord(",80\r\n");
            SysCtlDelay(30000000);

            sendWord("AT+CIPSEND=170\r\n");
            SysCtlDelay(30000000);

            sendWord(datos);
            SysCtlDelay(30000000);

            sendWord("\nAT+CIPCLOSE\n");


        }else if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) != 0){
            antirrebote2 = 0;
        }
        //Condición si lee un cero en el boton SW1
        if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) == 0 && antirrebote == 0){
            contador = contador + 1;
            antirrebote = 1;
        }else if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) != 0){
            antirrebote = 0;
        }
        if (contador > 7){
            contador = 0;
        }
        switch (contador){
        case 0:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=0&color=Negro   ";
            break;
        case 1:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=1&color=Azul    ";
            break;
        case 2:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=2&color=Verde   ";
            break;
        case 3:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=3&color=Turquesa";
            break;
        case 4:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=4&color=Rojo    ";
            break;
        case 5:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=5&color=Violeta ";
            break;
        case 6:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=6&color=Amarillo";
            break;
        case 7:
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
            datos = "POST /index.php HTTP/1.0\r\nHost:  192.168.1.22\r\nAccept: */*\r\nContent-Length: 37\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\ncarne=17302&id_color=7&color=Blanco  ";
            break;
        default :
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x00);
            break;
        }
    }
}

void sendWord (char* word){
    int x;
    for (x = 0; x<strlen(word); ++x){
        UARTCharPut(UART1_BASE,word[x]);
    }
}
