CC = gcc

all : consumer producer main clean

consumer:
	$(CC) consumer.c -pthread -lrt -o consumer.out

producer:
	$(CC) producer.c -pthread -lrt -o producer.out

main:
	./producer.out & ./consumer.out &

clean:
	rm -f *.out