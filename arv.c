//CIENCIA DA COMPUTACAO - UFMT - CUA
//KEVYN MENEZES CARVALHO
//ESTRUTURAS DE DADOS 1
//TRABALHO 7


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//DEFININDO ESTRUTURAS
typedef struct FILA {
	char letra;
	struct FILA *prox;
}tipoFila;

typedef struct ARVORE {
	char morse[6];
	char letra;
	struct ARVORE *esq;
	struct ARVORE *dir;
}tipoArvore;


void insereFila(tipoFila**, char); //Funcao que insere elementos em uma fila encadeada simples
char removeFila(tipoFila**); //Funcao que remove um elemento da fila retornando qual elemento foi removido
void insereArvore(tipoArvore**, char*, int, char); //Funcao para inserir uma letra junto com o codigo correspondente
char achaLetra(tipoArvore*, tipoFila**); //Funcao que retorna uma letra da arvore ja construida, com base em uma fila
void preOrdem(tipoArvore*); //Funcao que imprime a arvore em pre-ordem





//FUNCAO PRINCIPAL
int main(){
	tipoFila *fila = NULL;
	tipoArvore *arvore = NULL;
	FILE *codmorse, *arvmorse;
	char letra, c, palavra[200], pontoTraco[1000], morse[6];
	int i = 0, j = 0, k, opcao;

	//ABERTURA DE ARQUIVO
	arvmorse = fopen("morse.txt", "r"); //abro o arquivo com a arvore e volto o endereco para "arvmorse"
	codmorse = fopen("entrada.txt", "r"); //abro o arquivo com o codigo morse e volto o endereco para "codmorse"
	if ((arvmorse == NULL) || (codmorse == NULL)) { //verifico se ocorreu algum erro na abertura do arquivo
		printf("deu ero\n");
		return 0; // se ocorreu eu encerro o programa
	}



	//MONTANDO A ARVORE
	while((c = getc(arvmorse)) != EOF){ //enquanto nao chegar no fim do arquivo
		k = 0; // variavel controladora para adicionar elementos na minha string "morse"
		while(c != ' '){ //enquanto nao encontrar o espaco que difere o codigo da letra que o codigo corresponde
			morse[k++] = c; //adiciona este elemento que foi pegado na minha string "morse" e somo a variavel controladora "k"
			c = getc(arvmorse); //pego o proximo elemento para ser verificado na proxima iteracao
		}
		morse[k++] = '\0'; //quando achar o espaco, fecho minha string com "\0"
		letra = getc(arvmorse); //pego a letra que esta presente depois do espaco
		insereArvore(&arvore, morse, 0, letra); //insiro o codigo com sua letra correspondente na arvore
		getc(arvmorse); //uso este getc para ignorar o proximo caractere que tenho certeza que e um "\n"
	}
	fclose(arvmorse);
	//TERMINO DO PROCESSO DE MONTAGEM DA ARVORE




	//INSERINDO CODIGO MORSE A SER TRADUZIDO EM UMA STRING
	while((c = getc(codmorse)) != EOF){
		pontoTraco[j++] = c;
	}
	pontoTraco[j++] = '\0'; //fecho minha string com "\0"
	fclose(codmorse);


	//COM BASE NO CODIGO MORSE LIDO, MONTO MEU TEXTO TRADUZIDO
	j = 0; // variavel controladora da string
	while (pontoTraco[j] != '\0'){ //enquanto nao chegar no ultimo elemento da string
		if (pontoTraco[j] == '/'){ //verifico se o caractere na posicao e "/"
			palavra[i++] = ' ';//    se sim adiciono um espaco em meu texto
			j += 2;//				 somo +2 pois sei que depois de "/" sempre havera um espaco
		} else { // se nao for "/"
			if (pontoTraco[j] != ' ') { // verifico se e diferente de um espaco
				insereFila(&fila, pontoTraco[j]);// se for diferente, entao quer dizer que ainda nao posso traduzir para uma letra
			} else { // mas se for igual a espaco, entao tenho que traduzir para uma letra
				palavra[i++] = achaLetra(arvore, &fila); // entao chamo minha funcao achaLetra, que volta a letra desejada e guardo em palavra
			}
			j++; // somo j para a proxima iteracao
		}
	}
	//como eu so chamo a funcao achaLetra quando o caractere for um espaco, ele vai chegar no ultimo caractere da string e nao ira chamar achaLetra
	palavra[i++] = achaLetra(arvore, &fila); //por isso tenho que chamar a funcao pela ultima vez fora do while
	palavra[i++] = '\0'; //e fecho meu texto com o finalizador de string



	//UMA MINI INTERFACE
	printf("\nEscolha:\n0. Sair do programa\n1. Imprimir a arvore em pre-ordem\n2. Traduzir o codigo morse do arquivo de entrada\n");
	while (1){
		printf("\nSua opcao: ");
		scanf("%d", &opcao);
		if (opcao == 0){
			break;
		}
		switch (opcao){
			case(1):
				preOrdem(arvore);
				break;
			case(2):
				printf("\nCodigo Morse:\n");
				printf("%s\n", pontoTraco);
				printf("\nTraducao:\n%s\n", palavra);
				break;
		}
	}

	return 1;
}





void insereFila(tipoFila** fila, char letra){
	tipoFila *novoNo, *aux;
	aux = (*fila); //ponteiro auxiliar aponta para o ponteiro fila da funcao principal
	novoNo = malloc(sizeof(tipoFila)); //aloca um novo no na memoria
	novoNo->letra = letra; //atribui a letra para esse no
	novoNo->prox = NULL; //atualiza o ponteiro "prox" do novo no para nulo

	if (aux == NULL){ //se a fila estiver vazia 
		(*fila) = novoNo; //atualizo o ponteiro da funcao principal para apontar para esse no alocado
	} else { //se a fila estiver com elementos
		while(aux->prox != NULL){ // enquanto nao chegar no ultimo elemento da fila
			aux = aux->prox; //vai iterar
		}
		//quando chegar no ultimo elemento da lista
		aux->prox = novoNo; //o ponteiro "prox" do ultimo elemento vai apontar para o no alocado
	}
}

char removeFila(tipoFila** fila){
	tipoFila *aux; 
	char letra;
	if ((*fila) != NULL){ //se a fila estiver com elementos
		letra = (*fila)->letra; //guardo a letra do primeiro elemento
		aux = (*fila); //aux recebe o endereco de fila da funcao principal
		(*fila) = (*fila)->prox; //atualiza a cabeca da fila para ser o prox elemento
		free(aux); //desaloca o antigo primeiro elemento da memoria
		return letra; //retorna a letra que foi retirada da fila
	} 
}

void insereArvore(tipoArvore** arvore, char *morse, int pos, char letra){
	if ((*arvore) == NULL){
		(*arvore) = malloc(sizeof(tipoArvore));
		(*arvore)->dir = NULL;
		(*arvore)->esq = NULL;
		(*arvore)->letra = letra;
		strcpy((*arvore)->morse, morse);
	} else {
		if (morse[pos++] == '.'){
			insereArvore(&((*arvore)->esq), morse, pos, letra);
		} else {
			insereArvore(&((*arvore)->dir), morse, pos, letra);
		}
	}
}

char achaLetra(tipoArvore* arvore, tipoFila **fila){
	if ((*fila) == NULL){ // se nao tenho nenhum elemento a remover, entao estou no no desejado
		return arvore->letra; // retorno a letra desse no
	} else { //se ainda ha elementos na fila
		if (removeFila(fila) == '.'){ //verifico se esse elemento e um ponto ja o removendo
			return achaLetra(arvore->esq, fila); // se sim desco a esquerda
		} else { // se nao
			return achaLetra(arvore->dir, fila); // desco a direita
		}
	}
}

void preOrdem(tipoArvore* arvore){
	if (arvore != NULL){
		printf("%s ",arvore->morse);
		printf("%c\n", arvore->letra);
		preOrdem(arvore->esq);
		preOrdem(arvore->dir);
	}
}