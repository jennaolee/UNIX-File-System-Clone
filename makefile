UnixLs:
	gcc -Werror -Wall main.c infodemo.c UnixLs.c -o UnixLs

clean: 
	rm -f UnixLs