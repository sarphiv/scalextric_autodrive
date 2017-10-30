; ****** forbindelse af ICSP *********************  forbindelse af kredsløb ***************
;	Programmet forventes uploadet med en PICkit2 
;	eller en PICkit3 pilen på PICkit angiver pin1 
;	Navn		PICkit	PIC16F684					Funktion		PIC16F684
;	VPP/MCLR	1		4										
;	VDD/TARGET	2		1							LED				PORTC,5	
;	VSS/GND		3		14							SWITCH			PORTA,0		
;	ICSPDAT		4		13										
;	ICSPCLK		5		12
; ******* PROCESSOR DEFINITIONER *******************************************************
    list        p=16F684    
 	INCLUDE "p16f684.inc"
    ;errorlevel  -302    ; no "register not in bank 0" warnings 
    ;errorlevel  -305    ; no "page or bank selection not needed for this device" messages
; ******* COMPILER configuration bits *****************************************************
                ; ext reset, no code or data protect, no brownout detect,
                ; no watchdog, power-up timer, int clock with I/O,
                ; no failsafe clock monitor, two-speed start-up disabled 
 __CONFIG _FCMEN_OFF & _IESO_OFF & _BOD_OFF & _CPD_OFF &    _CP_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT
; ******* OPSÆTNING AF VARIABLE ***********************************************************
; Skriv variable og konstanter ind her

; ******* OPSÆTNING AF PROGRAM POINTERE ***************************************************
	ORG 	0x0000          ; processor reset vector
	goto	INIT
; ******* INITIALISERING AF CONTROLER *****************************************************
INIT
	MOVLW	b'00000111'		;Disable comparators
	MOVWF	CMCON0

	MOVLW	b'00001100'		;Enable PWM on PORTC5
	MOVWF	CCP1CON

	BSF 	STATUS, RP0		;Change to bank 1

	MOVLW	b'00000000'		;Change analog ports to digital
	MOVWF	ANSEL

	MOVLW	b'11011111'		;Set up pins
	MOVWF	TRISC

	BCF 	STATUS, RP0		;Change to bank 0
	
loop
	
  	goto 	loop				
; ******* PROGRAM SLUT *******************************************************************
 	end