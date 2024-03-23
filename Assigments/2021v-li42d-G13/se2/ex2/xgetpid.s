	.text
	.global xgetpid
xgetpid:movq $39, %rax
	syscall
	ret
	.end
	