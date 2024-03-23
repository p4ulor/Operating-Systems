	.text
	.global xclose
xclose:
	movq $3, %rax
	movq $0,%rdi
	syscall
	ret
	.end
