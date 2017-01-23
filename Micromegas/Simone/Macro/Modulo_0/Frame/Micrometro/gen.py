import random

f = open("numeri.txt","w")
for i in range(1,50):
#       a = random.random()
        f.write("%s     %s\n" %(random.random(), i))
#       out_file.write("       ")
#       out_file.write( i )
#       out_file.write("\n")
f.close()
