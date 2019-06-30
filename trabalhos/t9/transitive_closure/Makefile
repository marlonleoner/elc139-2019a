NAME		=	transitive_closure
NVCC		=	nvcc

EXE			=	$(NAME)
CUDAFILE 	=	$(NAME).cu

all : $(EXE)

$(EXE) :
   $(NVCC) $(CUDAFILE) -o $@

clean :
   rm $(EXE)

run: $(EXE)
   ./$(EXE) < transitive_closure.in

