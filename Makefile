CC=gcc
TARGET1=createkn01
TARGET2=bruteforce
TARGET3=dynpro
TARGET4=backtrack
all:
	$(CC) UDAWANT_OMKAR_pa5.c -lm -o $(TARGET1)
	$(CC) UDAWANT_OMKAR_pa5.c -lm -o $(TARGET2)
	$(CC) UDAWANT_OMKAR_pa5.c -lm -o $(TARGET3)
	$(CC) UDAWANT_OMKAR_pa5.c -lm -o $(TARGET4)
clean:
	rm -f *.o *.d *~ $(TARGET1)
	rm -f *.o *.d *~ $(TARGET2)
	rm -f *.o *.d *~ $(TARGET3)
	rm -f *.o *.d *~ $(TARGET4)
	rm -rf output1.txt
	rm -rf output2.txt
	rm -rf output3.txt
	rm -rf entries2.txt
	rm -rf entries3.txt