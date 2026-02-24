#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <time.h>

//Variaveis globais
#define QtdTerritorios 5
#define TamStrNome 30
#define TamStrCor 10

//struct
struct Territorio
{
     char nome[TamStrNome];
     char cor[TamStrCor];
     int tropas;
};

struct Territorio *Mapa[QtdTerritorios];


#pragma region Funções de apoio

//função para limpar o buffer de entrada
void limparBufferEntrada(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

//função para alocar memória para o mapa
void AlocarMapa(){
    for (int i = 0; i < QtdTerritorios; i++)
    {
        Mapa[i] = (struct Territorio*) malloc(sizeof(struct Territorio));
    }
}

//função para verificar se a alocação de memória foi bem sucedida
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

//função para liberar a memória alocada para o mapa
void ClearMapa(){
    for (int i = 0; i < QtdTerritorios; i++)
    {
        free(Mapa[i]);
    }
}
#pragma endregion

#pragma region Funções do menu

//função para exibir o menu inicial para cadastro dos territórios
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

//função para exibir o mapa atualizado
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
            
            ShowAtualMapa(); //exibe o mapa atualizado

        } while (atacante != 0);
        
    }
    
#pragma endregion

int main(){

    srand(time(NULL)); //inicializa o gerador de números aleatórios
    
    AlocarMapa(); //aloca memória para o mapa

    if(CheckAlocacao() == -1) {
        return -1; //sair do programa em caso de erro na alocação
    } 
    
    ShowMenuInicial(); //exibe o menu inicial para cadastro dos territórios

    printf("\nCadastro inicial concluido com sucesso!\n\n");

    ShowAtualMapa(); //exibe o mapa atualizado

    FaseAtaque(); //chama a fase de ataque

    ClearMapa(); // liberar memória
}
