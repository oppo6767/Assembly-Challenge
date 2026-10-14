000 S_203 START 1300h
010 VAR1  DD    10
020 VAR2  DD    20
030 VAR3  RD    3
040       DW    4,5,'A'
050       DB    6,7,'B','C'
060       RB    10
070       DB    "ABC"
080 MAIN  LDA   VAR1
090       ADD   VAR2
100       STA   VAR3
110       HALT
120       END   MAIN