	.text
	.global xopen
xopen:
	movq $2, %rax
	syscall
	ret
	.end
