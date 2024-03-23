	.text
	.global xwrite
xwrite:
	movq $1, %rax
	syscall
	ret
	.end
