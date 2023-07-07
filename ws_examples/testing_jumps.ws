label.main.0_L
S S 0 0 0 0 L
call.testcall.4_L
S T	0 1	0 0 L
jmp.testjmp.5_L
S L
0 1	0 1	L
label.retone.1_L
S S 0 0 0 1	L
push.0_S S 0 L
jz.testjz.6_L
T	S 0 1	1	0 L
label.rettwo.2_L
S S 0 0 1	0 L
push.0_S S 0 L
push.1_S S 1	L
sub_T	S S T	jn.testjn.7_L
T	T	0 1	1	1	L
label.retthree.3_L
S S 0 0 1	1	L
_end_L
L
L
label.testcall.4_L
S S 0 1	0 0 L
push.1_S S 1	L
puti_T	L
S T	ret_L
T	L
label.testjmp.5_L
S S 0 1	0 1	L
push.2_S S 1	0 L
puti_T	L
S T	jmp.retone.1_L
S L
0 0 0 1	L
label.testjz.6_L
S S 0 1	1	0 L
push.3_S S 1	1	L
puti_T	L
S T	jmp.rettwo.2_L
S L
1	0 L
label.testjn.7_L
S S 0 1	1	1	L
push.4_S S 1	0 0 L
puti_T	L
S T	jmp.retthree.3_L
S L
1	1	L
