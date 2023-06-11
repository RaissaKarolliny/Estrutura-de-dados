#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct {
    char nome[50];
    char classe[50];
    char papel[50];
    char tier[50];
    float porcentagemVitorias;
    float porcentagemEscolha;
    float porcentagemBanimento;
    float kda;
} Campeao;

typedef struct No {
    Campeao campeao;
    struct No* proximo;
} NoLista;

typedef struct {
    NoLista* inicio;
    int tamanho;
} ListaCampeoes;

ListaCampeoes* criarLista() {
    ListaCampeoes* lista = (ListaCampeoes*)malloc(sizeof(ListaCampeoes));
    lista->inicio = NULL;
    lista->tamanho = 0;
    return lista;
}

void destruirLista(ListaCampeoes* lista) {
    NoLista* atual = lista->inicio;
    while (atual != NULL) {
        NoLista* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(lista);
}

void inserirCampeao(ListaCampeoes* lista, Campeao campeao) {
    NoLista* novoNo = (NoLista*)malloc(sizeof(NoLista));
    novoNo->campeao = campeao;
    novoNo->proximo = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novoNo;
    } else {
        NoLista* atual = lista->inicio;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }

    lista->tamanho++;
}

void lerCampeoes(ListaCampeoes* lista, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(1);
    }
    char linha[256];
    fgets(linha, sizeof(linha), arquivo); // Ignorar a primeira linha (cabe�alho)
    while (fgets(linha, sizeof(linha), arquivo)) {
        Campeao campeao;
        sscanf(linha, "%[^,],%[^,],%[^,],%[^,],%f,%f,%f,%f",
               campeao.nome, campeao.classe, campeao.papel, campeao.tier,
               &campeao.porcentagemVitorias, &campeao.porcentagemEscolha,
               &campeao.porcentagemBanimento, &campeao.kda);
        inserirCampeao(lista, campeao);
    }

    fclose(arquivo);
}

void imprimirCampeao(Campeao* campeao) {
    system("cls");
    printf("Nome: %s\n", campeao->nome);
    printf("Classe: %s\n", campeao->classe);
    printf("Papel: %s\n", campeao->papel);
    printf("Tier: %s\n", campeao->tier);
    printf("Porcentagem de vit�rias: %.2f\n", campeao->porcentagemVitorias);
    printf("Porcentagem de escolha: %.2f\n", campeao->porcentagemEscolha);
    printf("Porcentagem de banimento: %.2f\n", campeao->porcentagemBanimento);
    printf("KDA: %.2f\n", campeao->kda);
}

void imprimirCampeoes(ListaCampeoes* lista) {
    NoLista* atual = lista->inicio;
    int i = 1;
    while (atual != NULL) {
        printf("%d. %s\n", i, atual->campeao.nome);
        atual = atual->proximo;
        i++;
    }
}

Campeao* buscarCampeao(ListaCampeoes* lista, const char* nome) {
    NoLista* atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->campeao.nome, nome) == 0) {
            return &atual->campeao;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void adicionarCampeao(ListaCampeoes* lista) {
    Campeao campeao;
    printf("Digite o nome do campe�o: ");
    scanf("%s", campeao.nome);
    printf("Digite a classe do campe�o: ");
    scanf("%s", campeao.classe);
    printf("Digite o papel do campe�o: ");
    scanf("%s", campeao.papel);
    printf("Digite o tier do campe�o: ");
    scanf("%s", campeao.tier);
    printf("Digite a porcentagem de vit�rias do campe�o: ");
    scanf("%f", &campeao.porcentagemVitorias);
    printf("Digite a porcentagem de escolha do campe�o: ");
    scanf("%f", &campeao.porcentagemEscolha);
    printf("Digite a porcentagem de banimento do campe�o: ");
    scanf("%f", &campeao.porcentagemBanimento);
    printf("Digite o KDA do campe�o: ");
    scanf("%f", &campeao.kda);
    system("cls");
    inserirCampeao(lista, campeao);
    printf("Campe�o adicionado com sucesso.\n");
}

void removerCampeao(ListaCampeoes* lista) {
    char nome[50];
    printf("Digite o nome do campe�o a ser removido: ");
    scanf("%s", nome);

    NoLista* anterior = NULL;
    NoLista* atual = lista->inicio;

    while (atual != NULL) {
        if (strcmp(atual->campeao.nome, nome) == 0) {
            if (anterior == NULL) {
                lista->inicio = atual->proximo;
            } else {
                anterior->proximo = atual->proximo;
            }

            free(atual);
            lista->tamanho--;
            printf("Campe�o removido com sucesso.\n");
            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("Campe�o n�o encontrado.\n");
}

void ordenarCampeoes(ListaCampeoes* lista, int opcao) {
    if (lista->tamanho <= 1) {
        return;
    }

    NoLista*atual = lista->inicio;
    NoLista*proximo = atual->proximo;

    switch (opcao) {
        case 1: // Ordenar por nome
            while (atual != NULL) {
                proximo = atual->proximo;
                while (proximo != NULL) {
                    if (strcmp(atual->campeao.nome, proximo->campeao.nome) > 0) {
                        Campeao temp = atual->campeao;
                        atual->campeao = proximo->campeao;
                        proximo->campeao = temp;
                    }
                    proximo = proximo->proximo;
                }
                atual = atual->proximo;
            }
            break;

        case 2: // Ordenar por porcentagem de vit�rias
            while (atual != NULL) {
                proximo = atual->proximo;
                while (proximo != NULL) {
                    if (atual->campeao.porcentagemVitorias < proximo->campeao.porcentagemVitorias) {
                        Campeao temp = atual->campeao;
                        atual->campeao = proximo->campeao;
                        proximo->campeao = temp;
                    }
                    proximo = proximo->proximo;
                }
                atual = atual->proximo;
            }
            break;

        case 3: // Ordenar por porcentagem de escolha
            while (atual != NULL) {
                proximo = atual->proximo;
                while (proximo != NULL) {
                    if (atual->campeao.porcentagemEscolha < proximo->campeao.porcentagemEscolha) {
                        Campeao temp = atual->campeao;
                        atual->campeao = proximo->campeao;
                        proximo->campeao = temp;
                    }
                    proximo = proximo->proximo;
                }
                atual = atual->proximo;
            }
            break;

        case 4: // Ordenar por porcentagem de banimento
            while (atual != NULL) {
                proximo = atual->proximo;
                while (proximo != NULL) {
                    if (atual->campeao.porcentagemBanimento < proximo->campeao.porcentagemBanimento) {
                        Campeao temp = atual->campeao;
                        atual->campeao = proximo->campeao;
                        proximo->campeao = temp;
                    }
                    proximo = proximo->proximo;
                }
                atual = atual->proximo;
            }
            break;

        default:
            printf("Op��o inv�lida.\n");
            return;
    }

    printf("Campe�es ordenados com sucesso.\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    ListaCampeoes* lista = criarLista();
    lerCampeoes(lista, "campeoes.csv");

    int opcao;
    do {
        printf("==== Menu ====\n");
        printf("1. Buscar campe�o\n");
        printf("2. Adicionar campe�o\n");
        printf("3. Remover campe�o\n");
        printf("4. Ordenar campe�es por nome\n");
        printf("5. Ordenar campe�es por porcentagem de vit�rias\n");
        printf("6. Ordenar campe�es por porcentagem de escolha\n");
        printf("7. Ordenar campe�es por porcentagem de banimento\n");
        printf("8. Imprimir a lista de campe�es\n");
        printf("0. Sair\n");
        printf("Digite a op��o desejada: ");
        scanf("%d", &opcao);
        switch (opcao) {
            case 1: {
                system("cls");
                char nomeBusca[50];
                printf("Digite o nome do campe�o: ");
                scanf("%s", nomeBusca);
                system("cls");
                Campeao* campeaoEncontrado = buscarCampeao(lista, nomeBusca);
                if (campeaoEncontrado != NULL) {
                    system("cls");
                    printf("=== Informa��es do Campe�o ===\n");
                    imprimirCampeao(campeaoEncontrado);
                } else {
                    printf("Campe�o n�o encontrado.\n");
                }
                break;
            }

            case 2:
                system("cls");
                adicionarCampeao(lista);
                break;

            case 3:
                system("cls");
                removerCampeao(lista);
                break;

            case 4:
                system("cls");
                ordenarCampeoes(lista, 1); // Ordenar por nome
                break;

            case 5:
                system("cls");
                ordenarCampeoes(lista, 2); // Ordenar por porcentagem de vit�rias
                break;

            case 6:
                system("cls");
                ordenarCampeoes(lista, 3); // Ordenar por porcentagem de escolha
                break;

            case 7:
                system("cls");
                ordenarCampeoes(lista, 4); // Ordenar por porcentagem de banimento
                break;
            case 8:
                system("cls");
                imprimirCampeoes(lista);
                break;

            case 0:
                system("cls");
                printf("Encerrando o programa.\n");
                break;

            default:
                printf("Op��o inv�lida.\n");
                break;
        }

        printf("\n");
    } while (opcao != 0);

    destruirLista(lista);

    return 0;
}

