CXX = g++ #compilador
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 #diretivas para máximo de avisos, travamento de versão e otimização do executável

# Diretórios e artefato final
SRC_DIR = src
TARGET = dedup

SRCS = $(wildcard $(SRC_DIR)/*.cpp) #mapeia arquivos .cpp
OBJS = $(SRCS:.cpp=.o) #tranforma eles em .o

# Regra primária
all: $(TARGET)

$(TARGET): $(OBJS) #target depende dos objetos
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) #comando para gerar o executável a partir dos objetos

# Etapa de compilação individual dos objetos
%.o: %.cpp #para qualquer .o, depende do .cpp correspondente
	$(CXX) $(CXXFLAGS) -c $< -o $@ #instrui para compilar .cpp em .o mas SEM LINKAR com os outros, com é o arquivo de origem e o arquivo de destino

# Regra de limpeza da árvore de trabalho
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) storage.dat