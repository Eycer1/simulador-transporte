
# Compilation Command
COMPILE = cc -c

# Link Command
LINK  = cc

# Source directory
SRCDIR  = src

# Temporary directory
TEMPDIR = bin

# Binary output name
BINARY = simutransusb

# Backend output name
BACKEND = backend.o

# Backend temporary files
BACKENDFILES =	$(TEMPDIR)/List.o \
				$(TEMPDIR)/Hours.o \
				$(TEMPDIR)/Utils.o \
				$(TEMPDIR)/Charge.o \
				$(TEMPDIR)/Parade.o \
				$(TEMPDIR)/Bus.o \
				$(TEMPDIR)/Service.o\
				$(TEMPDIR)/Route.o\
				$(TEMPDIR)/ParentProcess.o\
				$(TEMPDIR)/ChildProcess.o\

# Frontend temporary files
FRONTENDFILES =	$(TEMPDIR)/MainFrontend.o

# Objective temporary files
FILES =	$(BACKENDFILES)\
		$(FRONTENDFILES)\
		$(TEMPDIR)/Main.o


# Arguments to makefile
.PHONY: all clean backend test


all: $(BINARY)

backend-test: $(BACKEND)

backend: $(BACKENDFILES)

clean:
	rm $(BINARY) $(TEMPDIR)/$(BACKEND) $(BACKEND) $(FILES);\
	rmdir $(TEMPDIR)


# Rules of make...

# Create Temporary folder if not exists
$(TEMPDIR):
	mkdir $(TEMPDIR)


############# Backend files ##############
# Utils
$(TEMPDIR)/List.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/utils/list.h \
					$(SRCDIR)/backend/utils/List.c
	$(COMPILE) $(SRCDIR)/backend/utils/List.c -o $(TEMPDIR)/List.o

$(TEMPDIR)/Hours.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/utils/hours.h \
					$(SRCDIR)/backend/utils/hours.c
	$(COMPILE) $(SRCDIR)/backend/utils/hours.c -o $(TEMPDIR)/Hours.o

$(TEMPDIR)/Utils.o: $(TEMPDIR) \
					$(SRCDIR)/backend/utils/utils.h \
					$(SRCDIR)/backend/utils/utils.c
	$(COMPILE) $(SRCDIR)/backend/utils/utils.c -o $(TEMPDIR)/Utils.o

# Charge
$(TEMPDIR)/Parade.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/charge/parade.h \
					$(SRCDIR)/backend/charge/Parade.c
	$(COMPILE) $(SRCDIR)/backend/charge/Parade.c -o $(TEMPDIR)/Parade.o

$(TEMPDIR)/Charge.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/charge/charge.h \
					$(SRCDIR)/backend/charge/Charge.c
	$(COMPILE) $(SRCDIR)/backend/charge/Charge.c -o $(TEMPDIR)/Charge.o
	

#Service
$(TEMPDIR)/Bus.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/service/bus.h \
					$(SRCDIR)/backend/service/Bus.c
	$(COMPILE) $(SRCDIR)/backend/service/Bus.c -o $(TEMPDIR)/Bus.o

$(TEMPDIR)/Route.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/service/route.h \
					$(SRCDIR)/backend/service/Route.c
	$(COMPILE) $(SRCDIR)/backend/service/Route.c -o $(TEMPDIR)/Route.o

$(TEMPDIR)/Service.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/service/service.h \
					$(SRCDIR)/backend/service/Service.c
	$(COMPILE) $(SRCDIR)/backend/service/Service.c -o $(TEMPDIR)/Service.o

#Process
$(TEMPDIR)/ParentProcess.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/process/parent.h \
					$(SRCDIR)/backend/process/parent.c
	$(COMPILE) $(SRCDIR)/backend/process/parent.c -o $(TEMPDIR)/ParentProcess.o

$(TEMPDIR)/ChildProcess.o: 	$(TEMPDIR) \
					$(SRCDIR)/backend/process/child.h \
					$(SRCDIR)/backend/process/child.c
	$(COMPILE) $(SRCDIR)/backend/process/child.c -o $(TEMPDIR)/ChildProcess.o


################## Frontend files ####################
$(TEMPDIR)/MainFrontend.o:	$(TEMPDIR) \
					$(SRCDIR)/frontend/main.h \
					$(SRCDIR)/frontend/main.c
	$(COMPILE) $(SRCDIR)/frontend/main.c -o $(TEMPDIR)/MainFrontend.o


############## Main Function #################
# The main Function
$(TEMPDIR)/Main.o: 	$(TEMPDIR)\
					$(SRCDIR)/main.c
	$(COMPILE) $(SRCDIR)/main.c -o $(TEMPDIR)/Main.o

# The final binary depends of all temporary files
$(BINARY): $(FILES)
	$(LINK) $(FILES) -o $(BINARY) -pthread 


############ JUST FOR TEST SECTION ###########

# The backend.o is the main file just for testing
$(TEMPDIR)/backend.o:	$(TEMPDIR) \
						$(SRCDIR)/backend/backend.c
	$(COMPILE) $(SRCDIR)/backend/backend.c -o $(TEMPDIR)/backend.o

# The backend binary depends of backend files just for test
$(BACKEND): $(BACKENDFILES) \
			$(TEMPDIR)/backend.o
	$(LINK) $(BACKENDFILES) $(TEMPDIR)/backend.o -o $(BACKEND)