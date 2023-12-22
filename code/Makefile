steganographie_exe: steganographie.c
	gcc -std=c99 -Wall -pedantic-errors steganographie.c hide.c extract.c -o steganographie_exe
run : steganographie_exe
	./steganographie_exe papillon6136.bmp secret secret2 imageOutput.bmp imageUpToPosition.bmp

clean:
	rm -f steganographie_exe *~
