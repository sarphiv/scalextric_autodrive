;Keeps track of laps
    
LAP_INIT
    CALL    LDR_INIT
    
    BCF	    STATUS, RP0		;Change to bank 0
    MOVLW   h'00'
    MOVWF   LAP_Laps
    
    BCF	    RSTAT, RUN		;Set state to stopped
    
    RETURN

LAP_UPDATE
    BCF	    STATUS, RP0		;Change to bank 0
    BTFSS   RSTAT, MODE		;Figure out update type
    GOTO    LAP_UPDATE_RACE
    ;else   
    GOTO    LAP_UPDATE_RECON
    

    RETURN			;This line should never be reached

LAP_UPDATE_RECON
    BCF	    STATUS, RP0		;Change to bank 0
    
    INCF    LAP_Laps, F		;Increment laps by one
    
    MOVF    LAP_Laps, W		;Calculate if Laps <= 2 (one run on the track)
    SUBLW   h'01'
    
    GOTO    LAP_UPDATE_RUN
    
    RETURN			;This line should never be reached
    
LAP_UPDATE_RACE
    BCF	    STATUS, RP0		;Change to bank 0
    INCF    LAP_Laps, F		;Increment laps by one
    
    MOVF    LAP_Laps, W		;Calculate if Laps <= TotalLaps
    SUBLW   TOTALLAPS		
    
    GOTO    LAP_UPDATE_RUN
    
    RETURN			;This line should never be reached
    
LAP_UPDATE_RUN
    BTFSC   STATUS, 0		;If under or equal
    CALL    MOTOR_RESTART	;stop stop, this line might be wrong
    BTFSS   STATUS, 0		;If over
    CALL    MOTOR_STOP
    
    RETURN