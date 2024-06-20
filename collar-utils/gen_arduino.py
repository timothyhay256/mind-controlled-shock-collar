bitstream = "1111111111111111000000000000000100100011000100100010010010010001000110100100011000100101101100010001001001001001011000100010010010010010010010"
bitstream = [*bitstream]
#print(bitstream)
for i in range(len(bitstream)):
	if bitstream[i] == "0":
		print("digitalWrite(COLLAR_IN_PIN, 0);\ndelayMicroseconds(270);")
	elif bitstream[i] == "1":
		print("digitalWrite(COLLAR_IN_PIN, 1);\ndelayMicroseconds(270);")
