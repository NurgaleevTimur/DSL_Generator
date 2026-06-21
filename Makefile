# ======================== #
#         PROJECT          #
# ======================== #

GENDIR		:= appendix
TARGET		:= $(GENDIR)/dsl_gen

# ======================== #
#       DIRECTORIES        #
# ======================== #

# Директории исходников
VPATH		:= src

# Директории заголовков
IPATH		:= include
INCPATH		:= $(addprefix -I,$(IPATH))

# ======================== #
#      DEFINES / FLAGS     #
# ======================== #

CFLAGS		:= -std=c++11 -Wall -Wextra $(INCPATH)
LDLIBS		:= -lpugixml

# Список исходников и объектных файлов
SRCS		:= main.cpp $(wildcard $(VPATH)/*.cpp)
OBJS		:= $(addprefix $(GENDIR)/, $(notdir $(SRCS:.cpp=.o)))

# ======================== #
#           TOOLS          #
# ======================== #

CPP		:= g++

# ======================== #
#         TARGETS          #
# ======================== #


.PHONY: all clean mkdirs

all: mkdirs $(TARGET)

$(TARGET): $(OBJS)
	@echo "[LINKING]: $^ -> $@"
	$(CPP) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(GENDIR)/%.o: %.cpp
	@echo "[COMPILING]: $< -> $@"
	$(CPP) $(CFLAGS) -c $< -o $@


# ======================== #
#         SERVICE          #
# ======================== #


mkdirs:
	@mkdir -p $(GENDIR)

clean:
	@echo "[CLEANING] Removing $(GENDIR)/* ..."
	@rm -rvf $(GENDIR)/*
