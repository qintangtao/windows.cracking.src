

global cs
global ip

global ss
#global sp

global ds
global bp

global tab
global out
cs='++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.'
ip = 0

ss = []
#sp = 0

ds = []
bp = 0

tab = 0
out = []

def tab_():
	i = tab
	stab = ''
	while i > 0:
		stab += '\t'
		i -= 1
	return stab

def push(var):
	global ss
	ss.append(var)

def pop():
	global ss
	return ss.pop()

def op_inc_val():
	global ip
	count = 0
	while True:
		op = cs[ip]			
		ip = ip + 1
		if op == '+':
			count = count + 1
		else:
			break
	print tab_()+'ds[%d] += %d'%(bp, count)

def op_dec_val():
	global ip	
	count = 0
	while True:
		op = cs[ip]			
		ip = ip + 1
		if op == '-':
			count = count + 1
		else:
			break
	print tab_()+'ds[%d] -= %d'%(bp, count)

def op_inc_dp():
	global bp
	bp = bp + 1

def op_dec_dp():
	global bp
	bp = bp - 1

def op_jmp_fwd():
	global tab
	print tab_()+'while ds[%d]:'%(bp)
	tab = tab + 1

def op_jmp_bck():
	global tab
	tab = tab - 1

def op_out():
	print tab_()+'putchar(ds[%d])'%(bp)

def op_in():
	print tab_()+'getchar'

end = len(cs)
while ip < end:
	op = cs[ip]
	ip = ip + 1
	if op == '+':
		ip = ip - 1
		op_inc_val()
		ip = ip - 1
	elif op == '-':
		ip = ip - 1
		op_dec_val()
		ip = ip - 1
	elif op == '>':
		op_inc_dp()
	elif op == '<':
		op_dec_dp()
	elif op == '[':
		op_jmp_fwd()
	elif op == ']':
		op_jmp_bck()
	elif op == '.':
		op_out()
	elif op == ',':
		op_in()
	else:
		print 'invalid opcode'
		break