#ifndef LEXER__H_
#define LEXER__H_


#define FOREACH_TOKEN(F) \
    F(Illegal) \
    F(EndOfFile) \
    F(Ident) \
    F(Integer) \
    F(Assign) \
    F(Plus) \
    F(Minus) \
    F(Bang) \
    F(Asterisk) \
    F(Slash) \
    F(LowerThan) \
    F(GreaterThan) \
    F(Equal) \
    F(NotEqual) \
    F(Comma) \
    F(Semicolon) \
    F(LeftParenthesis) \
    F(RightParenthesis) \
    F(LeftBrace) \
    F(RightBrace) \
    F(Function) \
    F(Let) \
    F(True) \
    F(False) \
    F(If) \
    F(Else) \
    F(Return) \

enum TokenType {
    #define F(token) token,
    FOREACH_TOKEN(F)
    #undef F
};

extern const char *token_to_string(enum TokenType token);

struct Token {
    enum TokenType type;
    const char *beg;
    const char *end;
};

struct Lexer {
    const char *cur;
    const char *end;
};
int lexer_create(struct Lexer *lex, const char *beg, const char *end);
int lexer_destroy(struct Lexer *lex);
int lexer_next_token(struct Lexer *lex, struct Token *token);

#endif // LEXER__H_
