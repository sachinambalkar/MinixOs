myshell: main.o exit_sig.o  run.o error_handler.o init.o pipe_exe.o interpretor_services.o
	     clang -o myshell main.o exit_sig.o  run.o error_handler.o init.o pipe_exe.o interpretor_services.o
main.o: main.c 
	    clang -c main.c
exit_sig.o: exit_sig.c 
	        clang -c exit_sig.c
run.o: run.c 
	   clang -c run.c 
error_handler.o: error_handler.c 
	         clang -c error_handler.c 
init.o: init.c 
	    clang -c init.c 
pipe_exe.o: pipe_exe.c
	        clang -c pipe_exe.c
interpretor_services.: interpretor_services.c
			clang -c interpretor_services.c
clean:
	  rm myshell main.o exit_sig.o run.o exception.o init.o pipe_exe.o interpretor_services.o
	    