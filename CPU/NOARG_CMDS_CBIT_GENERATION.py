commands = []
commands_j = []
with open("commands.h", "r") as file:
	for line in file.readlines():
		splitted = line.split(",")
		if int(splitted[2]) == 0: commands.append(splitted[0][8:])
		if splitted[5] == " JMP_NON_KEYWORD_PROCESSING_INSTRUCTION)\n": commands_j.append(splitted[0][8:])

print(*commands)
print(*commands_j)

with open("GENERATED_CMDS_CBIT_DISASSEMBLER.h","w") as file:
	for command in commands:
		file.write("\n\n#define CBITS_0_"+command+"\ \n    CBITS_0("+command+")\ \n"+'    fprintf(fp, "\\n");\ \n\n')
		file.write("#define CBITS_1_"+command+"\ \n    printf(\"Error: code corrupted, wrong use of "+command+"\\n\");\ \n    exit(-1);\ \n \n")
		for i in range(2,8):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_1_"+command+"\n")
	
	for command in commands_j:
		file.write("\n\n#define CBITS_4_"+command+"\ \n    DISASM_JMP("+command+")\ \n\n")
		file.write("#define CBITS_0_"+command+"\ \n    printf(\"Error: code corrupted, wrong use of "+command+"\\n\");\ \n    exit(-1);\ \n \n")
		for i in range(1,4):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_0_"+command+"\n")
		for i in range(5,8):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_0_"+command+"\n")

with open("GENERATED_CMDS_CBIT_CPU.h","w") as file:
	for command in commands:
		file.write("\n\n#define CBITS_0_"+command+"\ \n    DO_"+command+"\ \n"+'    CPU_TRACE \n\n')
		file.write("#define CBITS_1_"+command+"\ \n    printf(\"Error: code corrupted, wrong use of "+command+"\\n\");\ \n    exit(-1);\ \n \n")
		for i in range(2,8):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_1_"+command+"\n")
	for command in commands_j:
		file.write("\n\n#define CBITS_4_"+command+"\ \n    DO_"+command+"\ \n\n")
		file.write("#define CBITS_0_"+command+"\ \n    printf(\"Error: code corrupted, wrong use of "+command+"\\n\");\ \n    exit(-1);\ \n \n")
		for i in range(1,4):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_0_"+command+"\n")
		for i in range(5,8):
			file.write("#define CBITS_"+str(i)+"_"+command+" CBITS_0_"+command+"\n")
