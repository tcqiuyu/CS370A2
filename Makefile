#CC = clang
CFLAGS += -Wall -std=c99 -D_XOPEN_SOURCE

all: checker coordinator

checker: checker.c
	$(CC) $(CFLAGS)  $< -o Qiu_Yu_Checker.out

coordinator: coordinator.c
	$(CC) $(CFLAGS)  $< -o Qiu_Yu_Coordinator.out

clean:
	rm -f Qiu_Yu_Checker Qiu_Yu_Coordinator *.out*
