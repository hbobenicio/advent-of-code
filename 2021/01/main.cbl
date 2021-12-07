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
           05 NUM PIC 9(6).
       
       WORKING-STORAGE SECTION.
       01  WS-EOF          PIC S9(1) VALUE 0.
           88 WS-EOF-TRUE            VALUE 1.
           88 WS-EOF-FALSE           VALUE 0.
      *-----------------------------------------------------------------
       PROCEDURE DIVISION.
       MAIN.
           OPEN INPUT EXAMPLE-FILE.
           PERFORM UNTIL WS-EOF = 1
              READ EXAMPLE-FILE
                 AT END
                    MOVE 1 TO WS-EOF
                 NOT AT END
                    DISPLAY NUM
              END-READ
           END-PERFORM.
           CLOSE EXAMPLE-FILE.

      *    MOVE 0 TO RETURN-CODE.
           GOBACK.
 