;Interrupt service routine module
    
ISR_INIT
    ISR_WCopy	RES 1		;Variable for temporarily storing working register
    
    ISR_BCopy	RES 1		;Variable for temporarily storing bits
    BANKBIT	EQU 1		;Status bank bit
	
    BSF	    INTCON, GIE		;Enable general interrupts (master interrupt switch)
    
    RETURN

ISR
    CALL    ISR_BACKUP		;Backup to registers to not interfere with main loop
    
    BCF	    STATUS, RP0		;Change to bank 0
    
    BTFSC   INTCON, INTF	;Check if triggered by hall effect sensor
    CALL    HallEffect_TRIG
    
    BTFSC   PIR1, C2IF		;Check if triggered by LDR change
    CALL    LDR_TRIG

    
    CALL    ISR_RESTORE

    RETFIE
    
ISR_BACKUP
    MOVWF   ISR_WCopy		;Backup working register
    
    BTFSS   STATUS, RP0		;Backup bank select bit
    BCF	    ISR_BCopy, BANKBIT
    BTFSC   STATUS, RP0		
    BSF	    ISR_BCopy, BANKBIT
    
    RETURN
    
ISR_RESTORE
    MOVF    ISR_WCopy, W	;Restore copy of working register
    
    BTFSS   ISR_BCopy, BANKBIT	;Restore bank select bit
    BCF	    STATUS, RP0
    BTFSC   ISR_BCopy, BANKBIT
    BSF	    STATUS, RP0
    
    RETURN