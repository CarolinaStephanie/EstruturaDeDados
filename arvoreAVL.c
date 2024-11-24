// Adaptação de: https://github.com/fabricioandradesantos/estruturaDeDados/blob/master/Linguagem%20C/Arvores/arvoreAVL.c
#include <stdio.h>
#include <stdlib.h>
#define tab 10

typedef struct ITEM
{
    int valor;
} tipoItem;

typedef struct No
{
    tipoItem item;
    struct No *esquerda;
    struct No *direita;
    int fatorBalanceamento;
    int altura;
} tipoNo;

void imprimirOrdem(tipoNo *raiz, int espaco)
{
    if (raiz == NULL)
    {
        return;
    }
    else
    {
        espaco += tab;
        imprimirOrdem(raiz->direita, espaco);
        printf("\n");
        for (int i = tab; i < espaco; i++)
        {
            printf(" ");
        }
        printf("%d\n", raiz->item.valor);
        imprimirOrdem(raiz->esquerda, espaco);
    }
}

void apagarArvore(tipoNo *raiz)
{
    if (raiz != NULL)
    {
        apagarArvore(raiz->esquerda);
        apagarArvore(raiz->direita);
        free(raiz);
        raiz = NULL;
    }
}

tipoNo *pesquisar(tipoNo *raiz, int valor)
{

    if (raiz != NULL)
    {
        if (raiz->item.valor == valor)
        {
            return raiz;
        }
        else
        {
            if (valor > raiz->item.valor)
            {
                return pesquisar(raiz->direita, valor);
            }
            else
            {
                return pesquisar(raiz->esquerda, valor);
            }
        }
    }

    return NULL;
}

tipoItem criarItem(int valor)
{

    tipoItem item;
    item.valor = valor;
    return item;
}

tipoNo *alocar(int valor)
{

    tipoNo *novoNo = (tipoNo *)malloc(sizeof(tipoNo));
    if (novoNo != NULL)
    {
        novoNo->item.valor = valor;
        novoNo->altura = 0;
        novoNo->esquerda = NULL;
        novoNo->direita = NULL;
        return novoNo;
    }
    else
    {
        printf("Erro ao alocar memória\n");
        exit(1);
    }
}

int alturaNo(tipoNo *raiz)
{
    if (raiz == NULL)
        return -1;
    else
        return raiz->altura;
}

int fatorBalanceamento(tipoNo *raiz)
{
    return labs(alturaNo(raiz->esquerda) - alturaNo(raiz->direita));
}

int alturaArvore(tipoNo *raiz)
{

    int alturaDireita = 0;
    int alturaEsquerda = 0;

    if (raiz == NULL)
    {
        return -1;
    }

    alturaEsquerda = alturaArvore(raiz->esquerda);
    alturaDireita = alturaArvore(raiz->direita);

    if (alturaEsquerda > alturaDireita)
    {
        return alturaEsquerda + 1;
    }
    else
    {
        return alturaDireita + 1;
    }
}

int contarNos(tipoNo *raiz)
{

    if (raiz == NULL)
    {
        return 0;
    }
    else
    {
        return contarNos(raiz->esquerda) + contarNos(raiz->direita) + 1;
    }
}

int maior(int x, int y)
{

    if (x > y)
    {
        return x;
    }
    else
    {
        return y;
    }
}

void rotacaoDireita(tipoNo **raiz)
{

    tipoNo *NoAuxiliar;

    NoAuxiliar = (*raiz)->esquerda;
    (*raiz)->esquerda = NoAuxiliar->direita;
    NoAuxiliar->direita = *raiz;

    (*raiz)->altura = maior(alturaNo((*raiz)->esquerda), alturaNo((*raiz)->direita)) + 1;
    NoAuxiliar->altura = maior(alturaNo(NoAuxiliar->esquerda), (*raiz)->altura) + 1;

    *raiz = NoAuxiliar;
}

void rotacaoEsquerda(tipoNo **raiz)
{
    tipoNo *NoAuxiliar;

    NoAuxiliar = (*raiz)->direita;
    (*raiz)->direita = NoAuxiliar->esquerda;
    NoAuxiliar->esquerda = *raiz;

    (*raiz)->altura = maior(alturaNo((*raiz)->esquerda), alturaNo((*raiz)->direita)) + 1;
    NoAuxiliar->altura = maior(alturaNo(NoAuxiliar->esquerda), (*raiz)->altura) + 1;

    *raiz = NoAuxiliar;
}

void rotacaoEsquerdaDireita(tipoNo **raiz)
{

    rotacaoEsquerda(&(*raiz)->esquerda);
    rotacaoDireita(raiz);
}

void rotacaoDireitaEsquerda(tipoNo **raiz)
{

    rotacaoDireita(&(*raiz)->direita);
    rotacaoEsquerda(raiz);
}

tipoNo *procuraMenor(tipoNo **raiz)
{
    tipoNo *NoAuxiliar1;
    tipoNo *NoAuxiliar2;

    NoAuxiliar1 = (*raiz);
    NoAuxiliar2 = (*raiz)->esquerda;

    while (NoAuxiliar2 != NULL)
    {
        NoAuxiliar1 = NoAuxiliar2;
        NoAuxiliar2 = NoAuxiliar2->esquerda;
    }
    return NoAuxiliar1;
}

int inserir(tipoNo **raiz, int valor)
{

    int controle;

    if (*raiz == NULL)
    {
        tipoNo *NoAuxiliar = alocar(valor);
        *raiz = NoAuxiliar;
        return 1;
    }

    tipoNo *NoAtual;

    NoAtual = *raiz;

    if (valor < NoAtual->item.valor)
    {
        if ((controle = inserir(&(NoAtual->esquerda), valor)) == 1)
        {
            if (fatorBalanceamento(NoAtual) >= 2)
            {
                if (valor < (*raiz)->esquerda->item.valor)
                    rotacaoDireita(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
    }

    else
    {
        if (valor > NoAtual->item.valor)
        {
            if ((controle = inserir(&(NoAtual->direita), valor)) == 1)
            {
                if (fatorBalanceamento(NoAtual) >= 2)
                {
                    if (valor > (*raiz)->direita->item.valor)
                        rotacaoEsquerda(raiz);
                    else
                        rotacaoDireitaEsquerda(raiz);
                }
            }
        }
        else
        {
            printf("\nValor Duplicado!\n");
            return 0;
        }
    }

    NoAtual->altura = maior(alturaNo(NoAtual->esquerda), alturaNo(NoAtual->direita)) + 1;
    return controle;
}

int remover(tipoNo **raiz, int valor)
{
    int controle;

    if (*raiz == NULL)
    {
        return 0;
    }
    if (valor < (*raiz)->item.valor)
    {
        if ((controle = remover(&((*raiz)->esquerda), valor)) == 1)
        {
            if (fatorBalanceamento(*raiz) >= 2)
            {
                if (alturaNo((*raiz)->direita->esquerda) <= alturaNo((*raiz)->direita->direita))
                    rotacaoEsquerda(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
    }
    if (valor > (*raiz)->item.valor)
    {
        if ((controle = remover(&((*raiz)->direita), valor)) == 1)
        {

            if (fatorBalanceamento(*raiz) >= 2)
            {
                if (alturaNo((*raiz)->esquerda->direita) <= alturaNo((*raiz)->esquerda->esquerda))
                    rotacaoDireita(raiz);
                else
                    rotacaoDireitaEsquerda(raiz);
            }
        }
    }
    if ((*raiz)->item.valor == valor)
    {
        if (((*raiz)->esquerda == NULL) || ((*raiz)->direita == NULL))
        {
            tipoNo *NoAuxiliar = *raiz;

            if ((*raiz)->esquerda != NULL)
                *raiz = (*raiz)->esquerda;
            else
                *raiz = (*raiz)->direita;
            free(NoAuxiliar);
            NoAuxiliar = NULL;
            printf("\nNó removido com sucesso!\n");
        }
        else
        {
            tipoNo *NoAuxiliar = procuraMenor(&(*raiz)->direita);
            (*raiz)->item.valor = NoAuxiliar->item.valor;
            remover(&(*raiz)->direita, (*raiz)->item.valor);
            if (fatorBalanceamento(*raiz) >= 2)
            {
                if (alturaNo((*raiz)->esquerda->direita) <= alturaNo((*raiz)->esquerda->esquerda))
                    rotacaoDireita(raiz);
                else
                    rotacaoEsquerdaDireita(raiz);
            }
        }
        return 1;
    }
    return controle;
}

int main()
{
    tipoNo *raiz = NULL;
    int opcao = -1;
    int valor = 0;

    while (opcao != 0)
    {
        printf("\n---Arvore atual---\n");
        imprimirOrdem(raiz, tab);

        valor = 0;
        printf("\n---------------MENU---------------\n");
        printf("1: Inserir \n");
        printf("2: Remover \n");
        printf("3: Pesquisar \n");
        printf("0: SAIR \n");
        scanf("%d", &opcao);

        switch (opcao)
        {

        case 1:
            printf("\n------INSERINDO------\n");
            printf("Digite um valor\n");
            scanf("%d", &valor);
            inserir(&raiz, valor);
            break;

        case 2:
            printf("\n------REMOVENDO------\n");
            printf("Digite um valor\n");
            scanf("%d", &valor);
            remover(&raiz, valor);
            break;

        case 3:
            printf("\n------PESQUISANDO------\n");
            printf("Digite um valor\n");
            scanf("%d", &valor);

            tipoNo *NoAuxiliar = pesquisar(raiz, valor);
            if (NoAuxiliar == NULL)
            {
                printf("\nElemento nao encontrado!\n");
            }
            else
            {
                printf("\nElemento encontrado com sucesso! \n");
            }
            break;

        case 0:
            printf("\ninalizou\n");
            break;

        default:
            printf("\nOpcao inválida! \n");
            break;
        }
    }
    apagarArvore(raiz);

    return 0;
}