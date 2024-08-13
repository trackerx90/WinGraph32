
!include wingraph32.mak

#-------------------------- Flags ------------------------------

# lex-compatible scanner generator
LEX	= flex

# yacc-compatible parser generator. Currently, it does not work 
# with yacc because some bison features are used directly.
YACC     = bison -ydvt

#-------------------------- Rules ------------------------------

y.tab.c \
y.tab.h : grammar.y
	$(YACC) grammar.y

lex.yy.c: grammar.l
	$(LEX) grammar.l

