CFLAGS=-std=c11 -g -static

9cc: 9cc.c 
# Makefileの最初のターゲットは省略できる
# かつ、Makefileは.cのファイルをコンパイルして同名のファイル（拡張子なし）を作るルールが組まれている
# よってmakeだけで「cc 9cc.c -o 9cc」となる

test: 9cc
		./test.sh

clean:
		rm -f 9cc *.o *~ tmp*

.PHONY: test clean