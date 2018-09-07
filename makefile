CC=gcc
CFLAGS=-Iinclude

ODIR = obj

LIBS=-lpthread

_OBJ = main.o server.o client_list.o client_thread.o messages.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

chatserv: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ && make