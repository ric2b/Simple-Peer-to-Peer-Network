from subprocess import call
import sys

if len(sys.argv) != 2:
	print ("(E) Usage: python " + sys.argv[0] + " <p2p or echo>\n")
	exit()

if sys.argv[1] == "echo":
	call(["nc tejo.tecnico.ulisboa.pt 58001"], shell=True)
	exit()

elif sys.argv[1] == "p2p": # forcar UCP
	call(["nc tejo.tecnico.ulisboa.pt 58000 -u"], shell=True)
	exit()

else: 
	print "argumento e 'echo' ou 'p2p'"

