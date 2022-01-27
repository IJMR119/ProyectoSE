#include "funciones.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#
#define _BV(bit) (1 << (bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))
#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))

// Variables a usar
int salida = 0; // variable que permitira quedarse en el bucle
int estadoLuz =0; // variable que  representa el estado del reflector (encendido/apagado)
int estadoPuerta= 0; // variable que  representa el estado de la puerta (apierta/cerrada)
int alarma = 0; // variable que representa si se va a activar o desactivar la alarma en el bucle
int alarmaActivada = 0; // variable que representa si se quiere que la alarma funcione o no
int alarmaActivada2 = 0; // variable que representa si se quiere que la alarma funcione o no
int detenerseT = 0; // variable que permite detener el motor de la puerta cuando este haya llegado al techo
int detenerseP = 0; // variable que permite detener el motor de la puerta cuando este haya llegado al piso
int contador = 5000;
int alarma2 =0;
// recibir datos del rapi

void lecturaDatos()
{
   serial_begin();
 if(is_data_ready())
   {
	if (get_RX_buffer()[0]=='A' && get_RX_buffer()[1]==0 )
	 {
	    estadoLuz = 1;
	 }
	if (get_RX_buffer()[0]=='B' && get_RX_buffer()[1]==0)
	 {
	    estadoLuz = 0;
         }
	 if (get_RX_buffer()[0]=='C' && get_RX_buffer()[1]==0 )
	 {
	    estadoPuerta = 1;
	    
	 }
	 if (get_RX_buffer()[0]=='D' && get_RX_buffer()[1]==0)
	 {
	    estadoPuerta = 0;
	    
         }
	 if (get_RX_buffer()[0]=='P' && get_RX_buffer()[1]==0)
	 {
	    alarmaActivada = 1;
	    
         }
	 if (get_RX_buffer()[0]=='Q' && get_RX_buffer()[1]==0)
	 {
	    alarmaActivada = 0;
	    contador=0;
	    
         }
	 if (get_RX_buffer()[0]=='E' && get_RX_buffer()[1]==0)
	 {
	    alarmaActivada2 = 1;
	    
         }
	 if (get_RX_buffer()[0]=='F' && get_RX_buffer()[1]==0)
	 {
	    alarmaActivada2 = 0;
	    contador=0;
	    
         }
   }
}







// llamdo de interrpcion por timer 
void interrupt_TIMER1_Init(){
 cli(); //deshabilitar momentáneamente las interrupciones
 TCCR1B = 0b101; //preescalador 1024
 TIMSK1= 1; // habilitar interrupcion por desbordamiento
 sei(); //habilitar las interrupciones
 TCNT1= 65047; // 0.5 s  tiempo de la interrupcion
}



// funcion para modificar alarma
int a = 1; //varible que activa el reflector en la alarma
void confAlarma(a)
{
   if(a==1)
   {
      PORTC = ((1<<PC3)); // activa el reflector
   }
}
int main()
 {
     
    //comunicacion serial
     serial_begin(); // activa la comunicacion serial 
     i2c_init(); // inica la comunicacion i2c
     lcd_init(); // activa el lcd
     lcd_msg("  ****Bienvenido/a****    "); // se presenta mensaje en el lcd
     lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
     lcd_msg("  a Smarter Guard   "); // se presenta mensaje en el lcd
     _delay_ms(1000); // retraso de 1 segundo
    
    // Configuracion de los pines
    PORTD = (1<<PD2)|(1<<PD3)|(1<<PD4)|(1<<PD5)|(1<<PD6); //Activar resistencia de PULLUP desde el PD2 al PD6 del puerto D
    DDRC = 0b00001111; // Puerto PC0 al PC3 como salida
    DDRB = 0b00011100; // Puerto PC0 al PC3 como salida
    // Interrupciones
    interrupt_TIMER1_Init();
     while (1)
      {
	 
	 lcd_cursor(0,0); // se especifica que se quiere colocar el siguiente mensaje en la primera linea del lcd
	 lcd_msg("Selecc. dispositivo:               "); // se presenta mensaje en el lcd
	 lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
	 lcd_msg("Reflector     Puerta                  "); // se presenta mensaje en el lcd
	 
	 salida = 0;
	 //  cuando se selecciona REFLECTOR
	  if((PIND&(1<<PIND2))==0) //si está presionado entra al if 
	     { 
		while((PIND&(1<<PIND2))==0){} //espera a que se suelte la botonera
		while(!salida)
		{   
		   // presentacion en el lcd
		   if(estadoLuz) // reflector encendido
		      {
			 lcd_cursor(0,0); // se especifica que se quiere colocar el siguiente mensaje en la primera linea del lcd
			 lcd_msg("Reflector:  Activado         "); // se presenta mensaje en el lcd
			 lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
			 lcd_msg("Enc.   Apag.   Listo"); // se presenta mensaje en el lcd
		      }
		   
		   if(!estadoLuz) // refelctor apagado
		   {
			lcd_cursor(0,0); // se especifica que se quiere colocar el siguiente mensaje en la primera linea del lcd
			lcd_msg("Reflector: Apagado         "); // se presenta mensaje en el lcd
			lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
			lcd_msg("Enc.   Apag.   Listo"); // se presenta mensaje en el lcd
		   }
		   
		    // cambio de estado del dispositivo
		   if((PIND&(1<<PIND4))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND4))==0){} //espera a que se suelte la botonera
			 estadoLuz = 1; 
			 
		      }
		      
		   if((PIND&(1<<PIND5))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND5))==0){} //espera a que se suelte la botonera
			 estadoLuz = 0; 
		      }
		   if((PIND&(1<<PIND6))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND6))==0){} //espera a que se suelte la botonera
			 salida = 1; 
		      }
		      
		}
	     }
	     
	   //  cuando se selecciona PUERTA
	  if((PIND&(1<<PIND3))==0) //si está presionado entra al if 
	     { 
		while((PIND&(1<<PIND3))==0){} //espera a que se suelte la botonera
		while(!salida)
		{   
		   // presentacion en el lcd
		   if(estadoPuerta) // Puerta abierta
		      {
			 lcd_cursor(0,0); // se especifica que se quiere colocar el siguiente mensaje en la primera linea del lcd
			 lcd_msg("Puerta:  Abierta         "); // se presenta mensaje en el lcd
			 lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
			 lcd_msg("Abrir  Cerrar  Listo       "); // se presenta mensaje en el lcd
		      }
		   
		   if(!estadoPuerta) // Puerta cerrada
		   {
			lcd_cursor(0,0); // se especifica que se quiere colocar el siguiente mensaje en la primera linea del lcd
			lcd_msg("Puerta: Cerrada         "); // se presenta mensaje en el lcd
			lcd_cursor(1,0); // se especifica que se quiere colocar el siguiente mensaje en la segunda linea del lcd
			lcd_msg("Abrir  Cerrar  Listo       "); // se presenta mensaje en el lcd
		   }
		   
		    // cambio de estado del dispositivo
		   if((PIND&(1<<PIND4))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND4))==0){} //espera a que se suelte la botonera
			 estadoPuerta = 1; 
		      }
		      
		   if((PIND&(1<<PIND5))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND5))==0){} //espera a que se suelte la botonera
			 estadoPuerta = 0; 
		     }
		   if((PIND&(1<<PIND6))==0) //si está presionado entra al if 
		      { 
			 while((PIND&(1<<PIND6))==0){} //espera a que se suelte la botonera
			 salida = 1; 
		      }
		}
	     }
           
      }
      return 0;
      
 }
 
 
ISR(TIMER1_OVF_vect)  //INTERRUPCIONES 
 {
    lecturaDatos();

    if(bit_is_set(PINB,PB1))
      {
	 alarma = 1;
      }
    if(bit_is_set(PINB,PB0))
      {
	 alarma2 = 1;
	  
      }
     if (!bit_is_set(PINB,PB0)&&!bit_is_set(PINB,PB1))
     {
	alarma=0;
	alarma2=0;
     }
     
      // ALARMA EXTERIOR
    while(alarma2 && alarmaActivada2)
    {   
       
       if(bit_is_set(PINB,PB1)&& alarmaActivada)
      {
	 PORTC = ((1<<PC2));
	 if (contador ==5000)
       {
	  serial_println_str("0");
	  contador =0;
       }
       else if (contador<5000)
       {
	  contador +=1;
       }
	 
      }
       confAlarma(1);
       
       	    
       if((PIND&(1<<PIND2))==0)
       {
	  while((PIND&(1<<PIND2))==0){} //espera a que se suelte la botonera
	  alarma2 = 0;
	    alarma =0;
	    alarmaActivada2 = 0;
	  contador=5000;
	  PORTC = ((0<<PC2));
	  
       }
       
       if (contador ==5000)
       {
	  serial_println_str("1");
	  contador =0;
       }
       else if (contador<5000)
       {
	  contador +=1;
       }
       serial_begin(); // activa la comunicacion serial 
       if(is_data_ready())
       {
	   if (get_RX_buffer()[0]=='F' && get_RX_buffer()[1]==0 )
	 {
	    alarma2 = 0;
	    alarma =0;
	    alarmaActivada2 = 0;
	    contador=5000;
	    PORTC = ((0<<PC2));
	 }
       }
    }
    // alarma interna
    while(alarma && alarmaActivada)
    {
       PORTC = ((1<<PC2));
       confAlarma(1);
            	    
       if((PIND&(1<<PIND2))==0)
       {
	  while((PIND&(1<<PIND2))==0){} //espera a que se suelte la botonera
	  alarma2 = 0;
	    alarma =5000;
	    alarmaActivada = 0;
	  PORTC = ((0<<PC2));
       }
       
       if (contador ==5000)
       {
	  serial_println_str("0");
	  contador =0;
       }
       else if (contador<5000)
       {
	  contador +=1;
       }
       serial_begin(); // activa la comunicacion serial 
       if(is_data_ready())
       {
	   if (get_RX_buffer()[0]=='Q' && get_RX_buffer()[1]==0 )
	 {
	    alarma2 = 0;
	    alarma =0;
	    
	    alarmaActivada = 0;
	    contador=5000;
	    PORTC = ((0<<PC2));
	 }
       }
       
       
    }
    
    // prendido de luz
    if(estadoLuz)
    {
       PORTC = ((1<<PC3)); // activa el reflector
       
    }
    if(!estadoLuz)
    {
       PORTC = ((0<<PC3)); // desctiva el reflector 
    }
    
    // final de carrera
    if(bit_is_set(PINB,PB5) )
    {
       detenerseP = 0;
       detenerseT = 1; 
    }
    if(bit_is_set(PIND,PD7) )
    {
       detenerseP = 1;
       detenerseT = 0;
    }
    if(!bit_is_set(PIND,PD7) && !bit_is_set(PINB,PB5) )
    {
       detenerseP = 0;
       detenerseT = 0;  
    }

    // puerta
     if(estadoPuerta && !detenerseT) // puerta abriendose
    {
       PORTB = ((1<<PB2))|((0<<PB3))|((1<<PB4)); 
    }
    else if(!estadoPuerta && !detenerseP) // puerta cerrandose
    {
       PORTB = ((0<<PB2))|((1<<PB3))|((1<<PB4)); 
    }
    else
    {
       PORTB = 0;
    }
    
 
    
    TCNT1= 65047; 
 }
   
 
 