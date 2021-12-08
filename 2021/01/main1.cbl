      *-----------------------------------------------------------------
      * $ cobc -Wall -Wextra -x -o a.out main.cbl
      * $ cat inputs/example.in.txt | ./a.out
      *-----------------------------------------------------------------
       IDENTIFICATION DIVISION.
       PROGRAM-ID.  AOC-2021-D01-1.
      * AUTHOR. HBOBENICIO.
      * DATE-WRITTEN. 07/12/2021.
      * SECURITY. NON-CONFIDENTIAL.
      *-----------------------------------------------------------------
       ENVIRONMENT DIVISION.
         INPUT-OUTPUT SECTION.
         FILE-CONTROL.
      *    SELECT EXAMPLE-FILE ASSIGN TO 'example.txt'
           SELECT EXAMPLE-FILE ASSIGN TO KEYBOARD
           ORGANIZATION IS LINE SEQUENTIAL.
      *-----------------------------------------------------------------
       DATA DIVISION.

       FILE SECTION.
       FD  EXAMPLE-FILE.
       01  EXAMPLE-RECORD.
           05 X PIC 9(4).
       
       WORKING-STORAGE SECTION.
       01  WS-XS           PIC  9(4) OCCURS 2048 TIMES.
       01  WS-XS-LEN       PIC  9(4) VALUE 0.
       01  WS-EOF          PIC S9(1) VALUE 0.
           88 WS-EOF-TRUE            VALUE 1.
           88 WS-EOF-FALSE           VALUE 0.
       01  WS-I            PIC  9(4) VALUE 1.
       01  WS-COUNT        PIC  9(4) VALUE 0.
       01  WS-PREV         PIC  9(4) VALUE 9999.
       01  WS-SOLUTION     PIC  9(4) VALUE 0.
      *-----------------------------------------------------------------
       PROCEDURE DIVISION.
       MAIN.
           PERFORM READ-AND-PARSE-INPUT.
           PERFORM SOLVE.
           DISPLAY WS-SOLUTION.
           GOBACK.

       READ-AND-PARSE-INPUT.
           OPEN INPUT EXAMPLE-FILE.
           PERFORM UNTIL WS-EOF = 1
              READ EXAMPLE-FILE
                 AT END
                    MOVE 1 TO WS-EOF
                 NOT AT END
                    MOVE X TO WS-XS(WS-XS-LEN + 1)
                    SET WS-XS-LEN UP BY 1
              END-READ
           END-PERFORM.
           CLOSE EXAMPLE-FILE.

       SOLVE.
           MOVE 0 TO WS-COUNT.
           MOVE 1 TO WS-I.
           PERFORM UNTIL WS-I > WS-XS-LEN
              IF WS-XS(WS-I) > WS-PREV
                 SET WS-COUNT UP BY 1
              END-IF
              MOVE WS-XS(WS-I) TO WS-PREV
              SET WS-I UP BY 1
           END-PERFORM.
           MOVE WS-COUNT TO WS-SOLUTION.
