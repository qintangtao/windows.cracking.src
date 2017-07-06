import re

def sym2cal(s):
	if '>' in s:
		return len(s)
	else:
		return -len(s)

def cal(s):
	if '+' in s:
		return '+= %d'%len(s)
	else:
		return '-= %d'%len(s)

def bf2asm(s,ptr,tab):
	p = 0
	l = len(s)
	while(p<l):
		pattern = re.compile(r'([><]*)\[-([><]*)\[-\]([><]+)\[-\]([><]+)\[-([><]+)\+([><]+)\+([><]+)\]([><]+)\[-([><]+)\+([><]+)\]([><]*)\[-([><]+)\+([><]+)\]([><]*)\]')
		match = pattern.match(s[p:])
		if match:		
			p += len(match.group())

			groups = match.groups()
			ptr1 = ptr + sym2cal(groups[0])
			ptr2 = ptr1
			for i in xrange(1,4):
				ptr2 += sym2cal(groups[i])
			ptr3 = ptr2
			for i in xrange(4,12):
				ptr3 += sym2cal(groups[i])
			print tab+'mem[%d] += mem[%d]*mem[%d]'%(ptr3,ptr2,ptr1)

			for v in groups:
				ptr += sym2cal(v)
			continue
		
		pattern = re.compile(r'([><]*)\[-\]([><]+)\[-\]([><]+)\[-([><]+)\+([><]+)\+([><]+)\]([><]+)\[-([><]+)\+([><]+)\]([><]*)\[-([><]+)\+([><]+)\]')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())

			groups = match.groups()
			ptr1 = ptr
			for i in xrange(3):
				ptr1 += sym2cal(groups[i])
			ptr2 = ptr1
			for i in xrange(3,11):
				ptr2 += sym2cal(groups[i])
			print tab+'mem[%d] += mem[%d]'%(ptr2,ptr1)

			for v in groups:
				ptr += sym2cal(v)
			continue

		pattern = re.compile(r'([><]*)\[-\]([><]+)\[-\]([><]+)\[-([><]+)\+([><]+)\+([><]+)\]([><]+)\[-([><]+)\+([><]+)\]([><]+)')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())

			groups = match.groups()
			ptr1 = ptr + sym2cal(groups[0])
			ptr2 = ptr1 + sym2cal(groups[1])
			ptr3 = ptr2 + sym2cal(groups[2])
			print tab+'mem[%d] = mem[%d]'%(ptr1,ptr3)

			for v in groups:
				ptr += sym2cal(v)
			continue
			
		pattern = re.compile(r'\[-\]')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())
			print tab+'mem[%d] = 0'%(ptr)
			continue

		pattern = re.compile(r'>+')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())
			ptr += len(match.group())
			continue

		pattern = re.compile(r'<+')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())
			ptr -= len(match.group())
			continue

		pattern = re.compile(r'\++')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())
			print tab+'mem[%d] %s'%(ptr,cal(match.group()))
			continue

		pattern = re.compile(r'-+')
		match = pattern.match(s[p:])
		if match:
			p += len(match.group())
			print tab+'mem[%d] %s'%(ptr,cal(match.group()))
			continue

		c = s[p]
		if c == '[':
			stk = 1
			for i,v in enumerate(s[p+1:]):
				if v == '[':
					stk += 1
				elif v == ']':
					stk -= 1
				else:
					continue
				if stk == 0:
					print tab+'while mem[%d]:'%ptr
					ptr = bf2asm(s[p+1:p+1+i],ptr,tab+'\t')
					p += i+1
					break
			continue

		elif c == ',':
			if input_ptr < 96:
				print tab+'mov mem[%d] input[input_ptr]'%ptr
			else:
				if bit_add >= 3600:
					print tab+'mov mem[%d] 0x30'%ptr
				else:
					print tab+'mov mem[%d] 1'%ptr
		elif c == '.':
			print tab+'cmp mem[%d] data[data_ptr]'%ptr
		p += 1
	return ptr

s = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>[-]<<<<<<<<<<<<<<<<<<<<<<<[-]>>>>>>>>>>>>>>>>>>>>>>[->+<<<<<<<<<<<<<<<<<<<<<<<+>>>>>>>>>>>>>>>>>>>>>>]<<<<<<<<<<<<<<<<<<<<<<[->>>>>>>>>>>>>>>>>>>>>>+<<<<<<<<<<<<<<<<<<<<<<]>>>>>>>>>>>>>>>>>>>>>>,>>>>>>[-]<<<<<<<<<<<<<<<<<<<<<<<<<<<<[-]>>>>>>>>>>>>>>>>>>>>>>[->>>>>>+<<<<<<<<<<<<<<<<<<<<<<<<<<<<+>>>>>>>>>>>>>>>>>>>>>>]"
input_ptr = 0
bit_add = 0
bf2asm(s,0,'')