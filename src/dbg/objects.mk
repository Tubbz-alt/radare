# Stupid trick
ifeq ($(CPU),i686)
CPU=i386
endif
ifeq ($(CPU),i586)
CPU=i386
endif


ifeq ($(W32),1)
# Windows 32 bits
psOBJ= dbg/win32/utils.o dbg/win32/debug.o
psOBJ+= dbg/win32/fd.o dbg/win32/signal.o
psOBJ+=dbg/arch/${CPU}-hack.o
psOBJ+=dbg/arch/${CPU}.o dbg/arch/${CPU}-bp.o
psOBJ+=dbg/close.o dbg/debug.o dbg/lseek.o dbg/open.o
psOBJ+=dbg/system.o dbg/signal.o
psOBJ+=dbg/mem.o dbg/wp.o dbg/events.o
psOBJ+=dbg/thread.o dbg/parser.o
else

ifeq ($(DARWIN),1)
# MacOSX
psOBJ=dbg/arch/${CPU}.o dbg/arch/${CPU}-hack.o dbg/arch/${CPU}-bp.o
psOBJ+=dbg/close.o dbg/debug.o dbg/lseek.o dbg/open.o
psOBJ+=dbg/system.o dbg/signal.o
psOBJ+=dbg/mem.o dbg/wp.o dbg/darwin/fd.o dbg/events.o
#psOBJ+=dbg/darwin/debug.o dbg/darwin/fd.o dbg/darwin/signal.o
psOBJ+=dbg/darwin/signal.o dbg/darwin/debug.o
psOBJ+=dbg/procs.o dbg/thread.o dbg/parser.o

else
# GNU/Linux, NetBSD, FreeBSD, OpenBSD
psOBJ=dbg/arch/${CPU}.o dbg/arch/${CPU}-hack.o dbg/arch/${CPU}-bp.o
psOBJ+=dbg/close.o dbg/debug.o dbg/lseek.o dbg/open.o
psOBJ+=dbg/system.o dbg/signal.o
psOBJ+=dbg/mem.o dbg/wp.o dbg/unix/fd.o dbg/events.o
#psOBJ+=dbg/unix/debug.o dbg/unix/fd.o dbg/unix/signal.o
psOBJ+=dbg/unix/signal.o dbg/unix/debug.o
psOBJ+=dbg/procs.o dbg/thread.o dbg/parser.o
endif

endif
