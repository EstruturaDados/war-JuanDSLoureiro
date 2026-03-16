#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Variaveis globais
#define QtdTerritorios 5
#define QtdMissoes 5
#define TamStrNome 30
#define TamStrCor 10

const char *Missoes[] = {
    "Conquistar o mundo",
    "Dominar 3 territórios",
    "Eliminar um adversário",
    "Eliminar todas as tropas da cor vermelha",
    "Dominar o território Azul"
};
char Missao[TamStrNome];

#define TotalMissoes (sizeof(Missoes) / sizeof(Missoes[0]))
//struct
struct Territorio
{
     char nome[TamStrNome];
     char cor[TamStrCor];
     int tropas;
};

struct Territorio *Mapa[QtdTerritorios];

//  --- PROTOTIPO DAS FUNÇÕES ---
void limparBufferEntrada();
void AlocarMapa();
int CheckAlocacao();
void ClearMapa();
void ShowMenuInicial();
void ShowAtualMapa();
void Atacar(struct Territorio* atacante, struct Territorio* defensor);
void FaseAtaque();
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho);

int main(){
    
    srand(time(NULL)); //inicializa o gerador de números aleatórios
    
    AlocarMapa(); //aloca memória para o mapa

    if(CheckAlocacao() == -1) {
        return -1; //sair do programa em caso de erro na alocação
    } 
    
    atribuirMissao(Missao, Missoes, TotalMissoes); //atribui uma missão aleatória ao jogador

    ShowMenuInicial(); //exibe o menu inicial para cadastro dos territórios

    printf("\nCadastro inicial concluido com sucesso!\n\n");

    ShowAtualMapa(); //exibe o mapa atualizado

    FaseAtaque(); //chama a fase de ataque

    ClearMapa(); // liberar memória
}

#pragma region Funções de apoio

/**
 * @brief Limpa o buffer de entrada para evitar problemas com fgets após scanf.
 */
void limparBufferEntrada(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Aloca memória para cada território no mapa.
 */
void AlocarMapa(){
    for (int i = 0; i < QtdTerritorios; i++)
    {
        Mapa[i] = (struct Territorio*) malloc(sizeof(struct Territorio));
    }
}

/**
 * @brief Verifica se a alocação de memória para o mapa foi bem sucedida.
 * @return Retorna 0 se a alocação foi bem sucedida, ou -1 se houve um erro na alocação.
 */
int CheckAlocacao(){
    for (int i = 0; i < QtdTerritorios; i++)
    {
        if (Mapa[i] == NULL)
        {
            printf("Erro ao alocar memória para o mapa!\n");
            return -1; //retorna -1 para indicar erro
        }else{
            return 0; //retorna 0 para indicar sucesso
        }
    }
}

/**
 * @brief Libera a memória alocada para cada território no mapa.
 */
void ClearMapa(){
    for (int i = 0; i < QtdTerritorios; i++)
    {
        free(Mapa[i]);
    }
}
#pragma endregion

#pragma region Funções do menu

/**
 * @brief Exibe o menu inicial para cadastro dos territórios, permitindo ao usuário inserir o nome, cor do exército e número de tropas para cada território.
 */
void ShowMenuInicial(){
    printf("======================================================== \n");
    printf("Vamos cadastrar os 5 territórios iniciais do nosso mundo \n\n");

    //laço de repetição entrada de dados
    for (int i = 0; i < QtdTerritorios; i++)
    {
        printf("\n--- Cadastrando Território %d --- \n",i+1);
        
        printf("Nome do Território: ");
        fgets(Mapa[i]->nome,TamStrNome,stdin);
        
        printf("Cor do Exercito (ex Azul, Verde): ");
        fgets(Mapa[i]->cor,TamStrCor,stdin);
        
        printf("Numero de Tropas: ");
        scanf("%d",&Mapa[i]->tropas);
        limparBufferEntrada();
    }
}

/**
 * @brief Exibe o estado atual do mapa, mostrando o nome, cor do exército dominante e número de tropas para cada território.
 */
void ShowAtualMapa(){
    printf("======================================================== \n");
    printf("    MAPA DO MUNDO - ESTADO ATUAL   \n");
    printf("======================================================== \n");

    //laço de repetição saida de dados
    for (int i = 0; i < QtdTerritorios; i++)
    {
        printf("\nTERRITÓRIO %d\n", i+1);
        printf(" - Nome: %s", Mapa[i]->nome);
        printf(" - Dominada por: Exercito %s", Mapa[i]->cor);
        printf(" - Tropas: %d\n\n", Mapa[i]->tropas);
    }
}
#pragma endregion

#pragma region Funcionalidades do jogo
/**
 * @brief Realiza um ataque entre um território atacante e um território defensor, rolando dados para determinar o resultado da batalha e atualizando o número de tropas e a cor do território defensor conforme necessário.
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void Atacar(struct Territorio* atacante, struct Territorio* defensor){
    int DadoAtacante = (rand() % 6) + 1;
    int DadoDefensor = (rand() % 6) + 1;

    printf("\n--- Resultado da Batalha ---\n");
    printf("O atacante %s rolou o dado e tirou: %d\n", atacante->nome, DadoAtacante);
    printf("O defensor %s rolou o dado e tirou: %d\n", defensor->nome, DadoDefensor);

    if (DadoAtacante > DadoDefensor){//atacante venceu
        printf("VITORIA DO ATACANTE! O defensor perde 1 tropa.\n");
        defensor->tropas -= 1;
        if (defensor->tropas <= 0) {//verifica se o defensor perdeu todas as tropas
            printf("Território conquistado!\n");
            strcpy(defensor->cor, atacante->cor); //atualiza a cor do defensor para a cor do atacante
            defensor->tropas = 1; // pelo menos 1 tropa ocupando
        }
    }else if (DadoAtacante < DadoDefensor){//defensor venceu
        printf("VITORIA DO DEFENSOR! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;            
    }else{//empate
        printf("Empate! Nenhuma tropa é perdida.\n");
    }
    printf("Precione enter para continuar para o próximo turno...\n\n");
    getchar();
}

/**
 * @brief Gerencia a fase de ataque do jogo, permitindo ao usuário escolher territórios atacantes e defensores, validando as escolhas e chamando a função de ataque para calcular os resultados das batalhas, até que o usuário decida encerrar a fase de ataque.
 */
void FaseAtaque(){
    int atacante, defensor;
    int TerritoriosValidos = 0;

    do
    {
        do
        {        
            printf("--- Fase de Ataque ---\n");
            printf("Escolha o território atacante (1 a 5, ou 0 para sair): ");
            scanf("%d", &atacante);
            limparBufferEntrada();
            if (atacante == 0) {
                printf("Encerrando a fase de ataque.\n");
                return; //sair da função
            }
            printf("Escolha o território defensor: ");
            scanf("%d", &defensor);
            limparBufferEntrada();
            
            //validação de entradas válidas para os territórios
            if (atacante < 1 || atacante > QtdTerritorios ||
            defensor < 1 || defensor > QtdTerritorios) {
                printf("Território inválido!\n");
                TerritoriosValidos = 0;
            }else if (Mapa[atacante-1]->tropas <= 0) {
                printf("O território atacante não tem tropas suficientes para atacar!\n");
                TerritoriosValidos = 0;
            } else if (Mapa[defensor-1]->tropas <= 0) {
                printf("O território defensor já foi conquistado! Escolha outro defensor.\n");
                TerritoriosValidos = 0;
            } else if (strcmp(Mapa[atacante-1]->cor, Mapa[defensor-1]->cor) == 0) {
                printf("O território defensor já foi conquistado! Escolha outro defensor.\n");
                TerritoriosValidos = 0;
            } else if (atacante == defensor) {
                printf("O território atacante e defensor não podem ser o mesmo!\n");
                TerritoriosValidos = 0;
            } else {
                TerritoriosValidos = 1; //entrar na fase de ataque
            }

        } while (TerritoriosValidos != 1);

        Atacar(Mapa[atacante-1], Mapa[defensor-1]);//calcular o resultado do ataque
        
        int missaoCumprida = verificarMissao(Missao, Mapa, QtdTerritorios); //verificar se a missão foi cumprida

        if (missaoCumprida) {
            printf("\nPARABENS! Você cumpriu a missão: %s\n", Missao);
            return; //sair da função
        }

        ShowAtualMapa(); //exibe o mapa atualizado

    } while (atacante != 0);
    
}

/**
 * @brief Atribui uma missão aleatória a um destino específico, selecionando uma missão de um array de missões e copiando-a para o destino fornecido.
 * @param destino Ponteiro para a string onde a missão selecionada será armazenada.
 * @param missoes Array de strings contendo as missões disponíveis.
 * @param totalMissoes Número total de missões disponíveis no array.
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes){
    int indice = rand() % totalMissoes; //gera um índice aleatório para selecionar uma missão
    strcpy(destino, missoes[indice]); //atribui a missão selecionada ao destino
};

/**
 * @brief Verifica se uma missão específica está presente no mapa, comparando a missão fornecida com as missões associadas a cada território no mapa.
 * @param missao Ponteiro para a string contendo a missão a ser verificada.
 * @param mapa Array de ponteiros para os territórios do mapa.
 * @param tamanho Número total de territórios no mapa.
 * @return Retorna 1 se a missão for encontrada no mapa, ou 0 se
 */
int verificarMissao(char* missao, struct Territorio* mapa, int tamanho){
    if (strcmp(missao, "Conquistar o mundo") == 0) {
        for (int i = 0; i < tamanho-1; i++) {
            if (strcmp(mapa[i].cor,mapa[i+1].cor) != 0)
                return 0;
        }
        return 1;
    }
    else if (strcmp(missao, "Dominar 3 territórios") == 0) {
        int contagem = 0;
        for (int i = 0; i < tamanho; i++) {
            contagem = 0;
            for (int j = 0; j < tamanho; j++) {
                if (strcmp(mapa[i].cor, mapa[j].cor) == 0) {
                    contagem++;
                }
                if (contagem >= 3) {
                    return 1;
                }
            }
        }
        return 0;
    }
    else if (strcmp(missao, "Eliminar um adversário") == 0) {
        for (int i = 0; i < tamanho-1; i++) {
            if (strcmp(mapa[i].cor,mapa[i+1].cor) == 0)
                return 1;
        }
        return 0;
    }
    else if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {
        for (int i = 0; i < tamanho-1; i++) {
            if (strcmp(mapa[i].cor,"Vermelho") == 0)
                return 0;
        }
        return 1;
    }
    else if (strcmp(missao, "Dominar o território Azul") == 0) {
        for (int i = 0; i < tamanho-1; i++) {
            if (strcmp(mapa[i].cor,"Azul") == 0)
                return 0;
        }
        return 1;
    }
    return 0;
};
#pragma endregion
