#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef enum {} 型名
typedef enum {
    TKN_RESERVED, //記号
    TKN_NUM, // 整数
    TKN_EOF, // 入力の終わり
} TokenKind;

// 通常struct Tokenと宣言しなければならないものをTokenだけで宣言できるようにtypedefを用いている
typedef struct Token Token;

struct Token {
    TokenKind kind; //トークンの型
    Token *next;    // 次（リスト型構造
    int val;        // 数値
    char *str;      // 文字列
};

// 現在着目しているトークン
Token *token;

void error(char *fmt, ...){
    va_list ap; // 可変長のリストを格納する型
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char op){
    if (token->kind != TKN_RESERVED || token->str[0] != op)
        return false;
    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char op){
    if (token->kind != TKN_RESERVED || token->str[0] != op)
        error("'%c'ではありません。", op);
    token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number(){
    if (token->kind != TKN_NUM)
        error("数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(){
    return token->kind == TKN_EOF;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str){
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p){
        if (isspace(*p)){
            p++;
            continue;
        }

        if (*p == '+' || *p == '-'){
            cur = new_token(TKN_RESERVED, cur, p++);
            continue;
        }

        if (isdigit(*p)){
            cur = new_token(TKN_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }

        error("トークナイズできません");
    }

    new_token(TKN_EOF, cur, p);
    return head.next;
}

int main(int argc, char **argv){
    if (argc != 2){
        error("引数の個数が正しくありません\n");
        return 1;
    }

    token = tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("    mov rax, %d\n", expect_number());

    while (!at_eof()){
        if (consume('+')){
            printf("    add rax, %d\n", expect_number());
            continue;
        }

        expect('-');
        printf("    sub rax, %d\n", expect_number());
    }

    printf("    ret\n");
    return 0;
}