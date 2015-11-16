main:main.c cpu.c fadd.c finv.c fsqrt.c
	gcc -Wall -o rin main.c cpu.c fadd.c finv.c fsqrt.c fmul.c float.c -lm
