#define LINHAS 8
#define COLUNAS 10
#define RAIO 20
#define LADO (2*RAIO+10)
#define MARGEM LADO/2
#define LARGURA COLUNAS*LADO
#define ALTURA LINHAS*LADO

#define NUMTIPOS 6
const Color CORES[] = {RED, GREEN, BLUE, YELLOW, PURPLE, WHITE};
const int QUANTCORES = (sizeof(CORES) / sizeof(Color));