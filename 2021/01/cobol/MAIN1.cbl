      *-----------------------------------------------------------------
      * $ cobc -Wall -Wextra -std=ibm-strict -fixed -g -O0 -debug \
      *        -x -o a.out cobol/MAIN1.cbl
      * $ cat example.txt | ./a.out
      *-----------------------------------------------------------------
       IDENTIFICATION DIVISION.
       PROGRAM-ID.  AOC-2021-D01-1.
      * AUTHOR. HBOBENICIO.
      * DATE-WRITTEN. 09/12/2021.
      * SECURITY. NON-CONFIDENTIAL.
      *-----------------------------------------------------------------
       ENVIRONMENT DIVISION.
         INPUT-OUTPUT SECTION.
         FILE-CONTROL.
           SELECT INPUT-FILE ASSIGN TO '/dev/stdin'
           ORGANIZATION IS LINE SEQUENTIAL.
      *-----------------------------------------------------------------
       DATA DIVISION.

       FILE SECTION.
       FD  INPUT-FILE.
       01  INPUT-FILE-RECORD.
           05 NUM PIC X(4).
       
       WORKING-STORAGE SECTION.
       01  WS-XS.
         05  XS-ARR         PIC 9(4) OCCURS 2048 TIMES.
         05  XS-LEN         PIC 9(4) VALUE 0.
       01  WS-LOOP          PIC A(1).
         88  LOOP-BREAK              VALUE 'B'.
         88  LOOP-CONTINUE           VALUE 'C'.
       01  WS-AUX.
         05  WS-I           PIC 9(4) VALUE 1.
         05  WS-COUNT       PIC 9(4) VALUE 0.
         05  WS-PREV        PIC 9(4) VALUE 9999.
         05  WS-SOLUTION    PIC 9(4) VALUE 0.
      *-----------------------------------------------------------------
       PROCEDURE DIVISION.
       MAIN.
           PERFORM READ-AND-PARSE-INPUT.
           PERFORM SOLVE.
           DISPLAY WS-SOLUTION.
           MOVE 0 TO RETURN-CODE.
           GOBACK.

       READ-AND-PARSE-INPUT.
           OPEN INPUT INPUT-FILE.
           PERFORM UNTIL LOOP-BREAK
              READ INPUT-FILE AT END SET LOOP-BREAK TO TRUE
              NOT AT END
                 MOVE NUM TO XS-ARR(XS-LEN + 1)
                 SET XS-LEN UP BY 1
              END-READ
           END-PERFORM.
           CLOSE INPUT-FILE.

       SOLVE.
           MOVE 0 TO WS-COUNT.
           MOVE 1 TO WS-I.
           PERFORM UNTIL WS-I > XS-LEN
              IF XS-ARR(WS-I) > WS-PREV
                 SET WS-COUNT UP BY 1
              END-IF
              MOVE XS-ARR(WS-I) TO WS-PREV
              SET WS-I UP BY 1
           END-PERFORM.
           MOVE WS-COUNT TO WS-SOLUTION.
