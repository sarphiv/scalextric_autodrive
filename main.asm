list        p=16F684    
INCLUDE "p16f684.inc"
;errorlevel  -302    ; no "register not in bank 0" warnings 
;errorlevel  -305    ; no "page or bank selection not needed for this device" messages
; ******* COMPILER configuration bits *****************************************************
; ext reset, no code or data protect, no brownout detect,
; no watchdog, power-up timer, int clock with I/O,
; no failsafe clock monitor, two-speed start-up disabled 
 __CONFIG _FCMEN_OFF & _IESO_OFF & _BOD_OFF & _CPD_OFF &  _CP_OFF & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT

; ******* Vectors ***************************************************	
    ORG	    0x0000	;Processor reset vector
    goto    INIT

    ORG	    0x0004	;Interrupt service routine vector
    goto    ISR

 ; ****** Memory and constants ***********************************
    ;RSTAT bit definitions
    RUN			EQU	0	;Running - The state of motor. 1 = Race, 0 = Stop
    MODE		EQU	1	;Mode - Toggle reading/write to EEPROM. 1 = Recon, 0 = Race
    SLOW		EQU	2	;Slow - State of the motor speed
    ;ISR constants
    BANKBIT		EQU	0	;Status bank bit
    ;LDR constants
    TOTALLAPS		EQU	3	;Total number of laps
    ;Motor constants
<<<<<<< HEAD
    SPEED_RACE_TURN	EQU	d'70'	;Speed while turning in race mode
    SPEED_RACE_STRAIGHT	EQU	d'127'	;Speed driving straight in race mode
=======
    SPEED_RACE_TURN	EQU	d'75'	;Speed while turning in race mode
    SPEED_RACE_STRAIGHT	EQU	d'125'	;Speed driving straight in race mode
>>>>>>> 2ff347a599d9087fe76798e4650190985565112d
    SPEED_RECON_NORMAL	EQU	d'85'	;Speed in recon mode
    SPEED_RACE_ACCEL	EQU	d'220'	;Speed while accelerating in race mode
    ACCEL_RACE_LENGTH	EQU	d'6'	;Cycles to accelerate in (8 is around 25 cm)
    EE_LOOK		EQU	d'2'	;Look-ahead distance in bytes

    
    cblock  0x20
    RSTAT		    ;Status register for car
    ISR_WCopy		    ;Variable for temporarily storing working register
    ISR_BCopy		    ;Variable for temporarily storing bits
    EE_DATA
    EE_ADR
    EE_MASK
    EE_BUFF
    EE_LEN		    ;EOF for map in EEPROM
    LAP_Laps
    MOTOR_ACCEL_COUNT	    ;Counter describing how long
    endc

; ******* MODULES *******************************************************
#INCLUDE    "PWM.inc"
#INCLUDE    "HallEffect.inc"
#INCLUDE    "Motor.inc"
#INCLUDE    "LDR.inc"
#INCLUDE    "LapCounter.inc"
#INCLUDE    "EEPROM.inc"
#INCLUDE    "ISR.inc"
	
; ******* SETUP *****************************************************
INIT
    ;Zero RSTAT address
    BCF	    STATUS, RP0	    ;Change to bank 0
    MOVLW   h'00'
    MOVWF   RSTAT
    
    ;Switch pin setup (this is only temporarily left here)
    BSF	    STATUS, RP0	    ;Change to bank 1
    MOVLW   b'10111111'	    ;Change C2 (turn switch) to digital
    ANDWF   ANSEL, F
    MOVLW   b'00000100'	    ;Set C2 (turn switch) to input
    IORWF   TRISC, F			
    
    ;Brake light (C3)
    MOVLW   b'01111111'	    ;Change C3 (Brake light) to digital
    ANDWF   ANSEL, F
    MOVLW   b'11110111'	    ;Set C3 (Brake light) to output
    ANDWF   TRISC, F
    
    ;Debug light (A5)
    BCF	    TRISA, 5	    ;Set A5 (debug light) to digital output
    
    ;State switch (A3)
    MOVLW   b'00001000'	    ;Set A3 (State switch) to input
    IORWF   TRISA, F
    
    CALL    MOTOR_INIT
    CALL    LAP_INIT
    CALL    EE_INIT
    CALL    ISR_INIT
    
    ;Check if recon or race mode is selected
    BCF	    STATUS, RP0
    
    BTFSC   PORTA, 3
    BSF	    RSTAT, MODE	    ;Recon mode
    BTFSS   PORTA, 3
    BCF	    RSTAT, MODE	    ;Race mode
    
    ;C5 (PWM signal), C4 (Comparator out), C3 UNUSED, C2 (Turn switch), C1 C0 (Comparator -, +)
    ;A2 (Hall effect interrupt) input, A3 (State switch) (These are both input by default)
    
; ******* MAIN LOOP *****************************************************	
LOOP
    BCF	    STATUS, RP0
    
;    BTFSS   PORTC, 2		;Turn on debug light
;    BSF	    PORTA, 5
;    BTFSC   PORTC, 2		;Turn off debug light
;    BCF	    PORTA, 5
    
;    BTFSS   PORTC, 2
;    MOVLW   SPEED_RACE_TURN
;    BTFSC   PORTC, 2
;    MOVLW   SPEED_RACE_STRAIGHT
;    
;    BTFSS   RSTAT, RUN
;    MOVLW   d'0'
;    
;    CALL    PWM_SET

    GOTO    LOOP

    END
