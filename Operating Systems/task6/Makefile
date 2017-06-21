# Makefile for the hello driver.
PROG=	helloN test_helloN
SRCS=	helloN.c test_helloN.c

DPADD+=	${LIBCHARDRIVER} ${LIBSYS}
LDADD+=	-lchardriver -lsys

.include <minix.service.mk>
