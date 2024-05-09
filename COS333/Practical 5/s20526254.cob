        IDENTIFICATION DIVISION. 
        PROGRAM-ID. SIMPLESTATS. 

        DATA DIVISION.
          WORKING-STORAGE SECTION.
            *> Comment
            01 NUM-ELEMENTS PIC 9(2).
            01 ARR PIC 9(5) OCCURS 100 TIMES.
            01 SMALLEST PIC 9(5).
            01 LARGEST PIC 9(5).
            01 MODE-VAL PIC 9(5). 
            01 I PIC 9(5).
            01 J PIC 9(5).
            01 COUNTER PIC 9(5). 
            01 MAX-COUNT PIC 9(5).
            01 CANDIDATE PIC 9(5). 

        PROCEDURE DIVISION.
            DISPLAY "How many integers would you like? "
            ACCEPT NUM-ELEMENTS
            
            PERFORM READDATA.

            PERFORM FINDSMALLEST.
            PERFORM FINDLARGEST.
            PERFORM MODEARR.

            *> Adding a comment to test if I can upload from local

            DISPLAY "Smallest value: " SMALLEST.
            DISPLAY "Largest value: " LARGEST.
            DISPLAY "Mode value: " MODE-VAL.
            STOP RUN.

      *> This is a comment
        READDATA.
            PERFORM VARYING I FROM 1 BY 1 UNTIL I > NUM-ELEMENTS
                DISPLAY "Enter value " I ": " 
                ACCEPT ARR(I)
            END-PERFORM.

        FINDSMALLEST.
            MOVE ARR(1) TO SMALLEST.
            PERFORM VARYING I FROM 2 BY 1 UNTIL I > NUM-ELEMENTS
                IF ARR(I) < SMALLEST THEN
                    MOVE ARR(I) TO SMALLEST
                END-IF
            END-PERFORM. 

        FINDLARGEST.
            MOVE ARR(1) TO LARGEST. 
            PERFORM VARYING I FROM 2 BY 1 UNTIL I > NUM-ELEMENTS
                IF ARR(I) > LARGEST THEN
                    MOVE ARR(I) TO LARGEST
                END-IF
            END-PERFORM. 

        MODEARR.
            MOVE 0 TO MAX-COUNT.
            PERFORM VARYING I FROM 1 BY 1 UNTIL I > NUM-ELEMENTS
                MOVE 0 TO COUNTER
                PERFORM VARYING J FROM 1 BY 1 UNTIL J > NUM-ELEMENTS
                    IF ARR(I) = ARR(J) THEN
                        ADD 1 TO COUNTER
                    END-IF
                END-PERFORM 
                IF COUNTER > MAX-COUNT THEN
                    MOVE COUNTER TO MAX-COUNT
                    MOVE ARR(I) TO CANDIDATE 
                END-IF
            END-PERFORM.
            MOVE CANDIDATE TO MODE-VAL.
