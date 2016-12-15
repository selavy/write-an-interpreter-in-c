#include "lexer.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const char *token_to_string(enum TokenType token) {
    switch (token) {
#define F(T) case T: return #T;
        FOREACH_TOKEN(F)
#undef F
        default: return "unknown";
    }
}

int lexer_create(struct Lexer *lex, const char *beg, const char *end) {
    lex->cur = beg;
    lex->end = end;
    return 0;
}

int lexer_destroy(struct Lexer *lex) {
    return 0;
}

int is_letter(char c) {
    return isalpha(c) || c == '_';
}

enum TokenType lookup_ident(const char *beg, const char *end) {
    int len = end - beg;
    if (beg >= end) {
        return Illegal;
    }
    //printf("Checking ident: '%.*s'\n", len, beg);

    // REVISIT(plesslie): would ideally use a perfect hash generator like gperf here
    if (strncmp(beg, "fn", len) == 0) {
        return Function;
    } else if (strncmp(beg, "let", len) == 0) {
        return Let;
    } else if (strncmp(beg, "true", len) == 0) {
        return True;
    } else if (strncmp(beg, "false", len) == 0) {
        return False;
    } else if (strncmp(beg, "if", len) == 0) {
        return If;
    } else if (strncmp(beg, "else", len) == 0) {
        return Else;
    } else if (strncmp(beg, "return", len) == 0) {
        return Return;
    } else {
        return Ident;
    }
}

int lexer_next_token(struct Lexer *lex, struct Token *token) {
    char c;

    // skip whitespace
    while (lex->cur < lex->end && isspace(*lex->cur)) {
        ++lex->cur;
    }

    if (lex->cur >= lex->end) {
        token->type = EndOfFile;
        return 0;
    }

    c = *lex->cur++;
    if (c == '=') {
        if (lex->cur < lex->end && *lex->cur == '=') {
            ++lex->cur;
            token->type = Equal;
        } else {
            token->type = Assign;
        }
    } else if (c == '!') {
        if (lex->cur < lex->end && *lex->cur == '=') {
            ++lex->cur;
            token->type = NotEqual;
        } else {
            token->type = Bang;
        }
    } else if (isdigit(c)) { // should this be checking for '.' as well?
        token->type = Integer;
        // read_number()
        token->beg = lex->cur - 1;
        while (lex->cur < lex->end && isdigit(*lex->cur)) {
            ++lex->cur;
        }
        token->end = lex->cur;
    } else if (is_letter(c)) {
        // read_ident()
        token->beg = lex->cur - 1;
        while (lex->cur < lex->end && is_letter(*lex->cur)) {
            ++lex->cur;
        }
        token->end = lex->cur;
        token->type = lookup_ident(token->beg, token->end);
    } else {
        switch (c) {
            case '+': token->type = Plus; break;
            case '-': token->type = Minus; break;
            case '/': token->type = Slash; break;
            case '*': token->type = Asterisk; break;
            case '<': token->type = LowerThan; break;
            case '>': token->type = GreaterThan; break;
            case ';': token->type = Semicolon; break;
            case ',': token->type = Comma; break;
            case '{': token->type = LeftBrace; break;
            case '}': token->type = RightBrace; break;
            case '(': token->type = LeftParenthesis; break;
            case ')': token->type = RightParenthesis; break;
            default:
                      // case '=' or '=='
                      // case '!' or '!='
                      // is_letter
                      // is_numeric
                      return 1;
        }
    }

    return 0;
}
