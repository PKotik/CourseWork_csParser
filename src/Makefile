# Компиляторы
CC = gcc
LEX = flex
YACC = bison
CFLAGS = -Wall -g

LDFLAGS = -lfl

# Цели
TARGET = csharp_parser
OBJECTS = lex.yy.o parser.tab.o main.o

# Правила по умолчанию
all: $(TARGET)

# Генерация парсера из .y файла
parser.tab.c parser.tab.h: parser.y
	$(YACC) -d -o parser.tab.c parser.y

# Генерация лексера из .l файла
lex.yy.c: lexer.l parser.tab.h
	$(LEX) -o lex.yy.c lexer.l

# Компиляция объектных файлов
lex.yy.o: lex.yy.c parser.tab.h
	$(CC) $(CFLAGS) -c lex.yy.c -o lex.yy.o

parser.tab.o: parser.tab.c parser.tab.h
	$(CC) $(CFLAGS) -c parser.tab.c -o parser.tab.o

main.o: main.c parser.tab.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Сборка исполняемого файла
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Очистка
clean:
	rm -f $(TARGET) lex.yy.c parser.tab.c parser.tab.h *.o

# Тестовый запуск
test: $(TARGET)
	./$(TARGET) test.cs

# Установка зависимостей (Ubuntu)
setup:
	sudo apt-get update
	sudo apt-get install -y flex bison gcc make

.PHONY: all clean test setup