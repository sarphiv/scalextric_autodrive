;Interrupt service routine module
    
ISR_INIT
    ISR_WCopy	RES 1	;Variable for temporarily storing working register
    BSF	    INTCON, GIE		;Enable general interrupts (master interrupt switch)
    
    RETURN

ISR
    MOVWF   ISR_WCopy		;Create copy of working register to not interfere with main loop

    
    BTFSC   INTCON, INTF	;Check if triggered by hall effect sensor
    CALL    HallEffect_TRIG
    
    BTFSC   PIR1, C2IF		;Check if triggered by turn switch
    CALL    LDR_TRIG

    
    MOVF    ISR_WCopy, W	;Restore copy of working register

    RETFIE