fusefs: fusefs.c labhelper.c simpleFS.c
	gcc fusefs.c labhelper.c simpleFS.c -o fusefs `pkg-config fuse --cflags --libs` -g -std=gnu99

clean: 
	rm fusefs
