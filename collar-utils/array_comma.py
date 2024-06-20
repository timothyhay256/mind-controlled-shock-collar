bitstream = input("Enter your bitstream")
bitstream = [*bitstream]
#print(bitstream)
for i in range(len(bitstream)):
	print(bitstream[i]+", ", end="")
print(len(bitstream))
