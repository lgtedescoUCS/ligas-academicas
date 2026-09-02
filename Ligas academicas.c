#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estruturas de Dados
typedef struct Estudante {
    int codigo;
    char nome[100];
    char curso[100];
    struct Estudante* prox;
} Estudante;

typedef struct ListaEstudantes {
    Estudante* inicio;
    int quantidade;
} ListaEstudantes;

typedef struct Participante {
    Estudante* estudante;
    struct Participante* prox;
} Participante;

typedef struct ListaParticipantes {
    Participante* inicio;
    int quantidade;
} ListaParticipantes;

typedef struct Atividade {
    char nome_liga[100];
    char local[100];
    ListaParticipantes* participantes;
    struct Atividade* prox;
} Atividade;

typedef struct ListaAtividades {
    char data[11];
    char horario[6];
    Atividade* inicio;
    int quantidade;
} ListaAtividades;

// Protótipos de Funções
int estudante_ja_inscrito(ListaAtividades* agenda, int codigo_estudante);

// Funções para Lista de Estudantes (TAD Cadastro)
ListaEstudantes* criar_lista_estudantes() {
    ListaEstudantes* lista = (ListaEstudantes*)malloc(sizeof(ListaEstudantes));
    lista->inicio = NULL;
    lista->quantidade = 0;
    return lista;
}

Estudante* criar_estudante(int codigo, const char* nome, const char* curso) {
    Estudante* novo = (Estudante*)malloc(sizeof(Estudante));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    strcpy(novo->curso, curso);
    novo->prox = NULL;
    return novo;
}

void insere_estudante(ListaEstudantes* lista, Estudante* novo_estudante) {
    Estudante* atual;
    if (lista->inicio == NULL || strcmp(lista->inicio->nome, novo_estudante->nome) >= 0) {
        novo_estudante->prox = lista->inicio;
        lista->inicio = novo_estudante;
    } else {
        atual = lista->inicio;
        while (atual->prox != NULL && strcmp(atual->prox->nome, novo_estudante->nome) < 0) {
            atual = atual->prox;
        }
        novo_estudante->prox = atual->prox;
        atual->prox = novo_estudante;
    }
    lista->quantidade++;
}

void remove_estudante_por_codigo(ListaEstudantes* lista, int codigo, ListaAtividades* agenda) {
    if (estudante_ja_inscrito(agenda, codigo)) {
        printf("Erro: Nao e possivel remover o estudante (codigo %d) pois ele esta inscrito em uma atividade.\n", codigo);
        return;
    }

    Estudante* temp = lista->inicio;
    Estudante* anterior = NULL;

    if (temp != NULL && temp->codigo == codigo) {
        lista->inicio = temp->prox;
        free(temp);
        lista->quantidade--;
        printf("Estudante com codigo %d removido com sucesso.\n", codigo);
        return;
    }

    while (temp != NULL && temp->codigo != codigo) {
        anterior = temp;
        temp = temp->prox;
    }

    if (temp == NULL) {
        printf("Erro: Estudante com codigo %d nao encontrado para remocao.\n", codigo);
        return;
    }

    anterior->prox = temp->prox;
    free(temp);
    lista->quantidade--;
    printf("Estudante com codigo %d removido com sucesso.\n", codigo);
}

Estudante* busca_estudante_por_codigo(ListaEstudantes* lista, int codigo) {
    Estudante* atual = lista->inicio;
    while (atual != NULL) {
        if (atual->codigo == codigo) return atual;
        atual = atual->prox;
    }
    return NULL;
}

Estudante* busca_estudante_por_nome(ListaEstudantes* lista, const char* nome) {
    Estudante* atual = lista->inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) return atual;
        atual = atual->prox;
    }
    return NULL;
}

void consulta_todos_estudantes(ListaEstudantes* lista) {
    Estudante* atual = lista->inicio;
    printf("--- Lista de Estudantes Cadastrados (%d) ---\n", lista->quantidade);
    while (atual != NULL) {
        printf("Codigo: %d, Nome: %s, Curso: %s\n", atual->codigo, atual->nome, atual->curso);
        atual = atual->prox;
    }
    printf("-------------------------------------------\n");
}

// Funções para Lista de Atividades e Participantes (TAD AgendaLigas)
ListaAtividades* criar_agenda(const char* data, const char* horario) {
    ListaAtividades* agenda = (ListaAtividades*)malloc(sizeof(ListaAtividades));
    strcpy(agenda->data, data);
    strcpy(agenda->horario, horario);
    agenda->inicio = NULL;
    agenda->quantidade = 0;
    return agenda;
}

ListaParticipantes* criar_lista_participantes() {
    ListaParticipantes* lista = (ListaParticipantes*)malloc(sizeof(ListaParticipantes));
    lista->inicio = NULL;
    lista->quantidade = 0;
    return lista;
}

Atividade* criar_atividade(const char* nome_liga, const char* local) {
    Atividade* nova = (Atividade*)malloc(sizeof(Atividade));
    strcpy(nova->nome_liga, nome_liga);
    strcpy(nova->local, local);
    nova->participantes = criar_lista_participantes();
    nova->prox = NULL;
    return nova;
}

void insere_atividade(ListaAtividades* agenda, Atividade* nova_atividade) {
    if (agenda->inicio == NULL) {
        agenda->inicio = nova_atividade;
    } else {
        Atividade* atual = agenda->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = nova_atividade;
    }
    agenda->quantidade++;
}

void liberar_participantes(ListaParticipantes* lista) {
    Participante* atual = lista->inicio;
    Participante* proximo;
    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
    }
    free(lista);
}

void remove_atividade(ListaAtividades* agenda, const char* nome_atividade) {
    Atividade* temp = agenda->inicio;
    Atividade* anterior = NULL;

    if (temp != NULL && strcmp(temp->nome_liga, nome_atividade) == 0) {
        agenda->inicio = temp->prox;
        liberar_participantes(temp->participantes);
        free(temp);
        agenda->quantidade--;
        printf("Atividade '%s' removida com sucesso.\n", nome_atividade);
        return;
    }

    while (temp != NULL && strcmp(temp->nome_liga, nome_atividade) != 0) {
        anterior = temp;
        temp = temp->prox;
    }

    if (temp == NULL) {
        printf("Erro: Atividade '%s' nao encontrada para remocao.\n", nome_atividade);
        return;
    }

    anterior->prox = temp->prox;
    liberar_participantes(temp->participantes);
    free(temp);
    agenda->quantidade--;
    printf("Atividade '%s' removida com sucesso.\n", nome_atividade);
}

Atividade* busca_atividade(ListaAtividades* agenda, const char* nome_atividade) {
    Atividade* atual = agenda->inicio;
    while (atual != NULL) {
        if (strcmp(atual->nome_liga, nome_atividade) == 0) return atual;
        atual = atual->prox;
    }
    return NULL;
}

int estudante_ja_inscrito(ListaAtividades* agenda, int codigo_estudante) {
    Atividade* atividade_atual = agenda->inicio;
    while (atividade_atual != NULL) {
        Participante* participante_atual = atividade_atual->participantes->inicio;
        while (participante_atual != NULL) {
            if (participante_atual->estudante->codigo == codigo_estudante) {
                return 1;
            }
            participante_atual = participante_atual->prox;
        }
        atividade_atual = atividade_atual->prox;
    }
    return 0;
}

void insere_participante(ListaAtividades* agenda, ListaEstudantes* cadastro, const char* nome_atividade, int codigo_estudante) {
    Estudante* estudante = busca_estudante_por_codigo(cadastro, codigo_estudante);
    if (!estudante) {
        printf("Erro: Estudante com codigo %d nao cadastrado.\n", codigo_estudante);
        return;
    }
    if (estudante_ja_inscrito(agenda, codigo_estudante)) {
        printf("Erro: Estudante %s (codigo %d) ja esta inscrito em outra atividade.\n", estudante->nome, codigo_estudante);
        return;
    }
    Atividade* atividade = busca_atividade(agenda, nome_atividade);
    if (!atividade) {
        printf("Erro: Atividade '%s' nao encontrada.\n", nome_atividade);
        return;
    }

    Participante* novo_participante = (Participante*)malloc(sizeof(Participante));
    novo_participante->estudante = estudante;
    novo_participante->prox = NULL;

    if (atividade->participantes->inicio == NULL) {
        atividade->participantes->inicio = novo_participante;
    } else {
        Participante* atual = atividade->participantes->inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo_participante;
    }
    atividade->participantes->quantidade++;
    printf("Estudante %s inscrito na atividade '%s' com sucesso.\n", estudante->nome, nome_atividade);
}

void consulta_atividade(ListaAtividades* agenda, const char* nome_atividade) {
    Atividade* atividade = busca_atividade(agenda, nome_atividade);
    if (!atividade) {
        printf("Atividade '%s' nao encontrada.\n", nome_atividade);
        return;
    }
    printf("\n--- Detalhes da Atividade ---\n");
    printf("Nome da Liga: %s\n", atividade->nome_liga);
    printf("Local/Link: %s\n", atividade->local);
    printf("Participantes Inscritos (%d):\n", atividade->participantes->quantidade);
    if (atividade->participantes->inicio == NULL) {
        printf("  Nenhum participante inscrito.\n");
    } else {
        Participante* atual = atividade->participantes->inicio;
        while (atual != NULL) {
            printf("  - %s (Codigo: %d)\n", atual->estudante->nome, atual->estudante->codigo);
            atual = atual->prox;
        }
    }
    printf("-----------------------------\n");
}

void consulta_quantidades(ListaAtividades* agenda) {
    Atividade* atividade_atual = agenda->inicio;
    printf("\n--- Quantidade de Participantes por Atividade ---\n");
    while (atividade_atual != NULL) {
        printf("Atividade '%s': %d participante(s)\n", atividade_atual->nome_liga, atividade_atual->participantes->quantidade);
        atividade_atual = atividade_atual->prox;
    }
    printf("-------------------------------------------------\n");
}

void consulta_todas_atividades(ListaAtividades* agenda) {
    Atividade* atual = agenda->inicio;
    printf("\n--- Todas as Atividades Agendadas para %s as %s (%d) ---\n", agenda->data, agenda->horario, agenda->quantidade);
    while (atual != NULL) {
        printf("Liga: %s, Local/Link: %s\n", atual->nome_liga, atual->local);
        atual = atual->prox;
    }
    printf("---------------------------------------------------------------\n");
}

void listar_atividades_do_estudante(ListaAtividades* agenda, ListaEstudantes* cadastro, int codigo_estudante) {
    Estudante* estudante = busca_estudante_por_codigo(cadastro, codigo_estudante);
    if (!estudante) {
        printf("Erro: Estudante com codigo %d nao cadastrado.\n", codigo_estudante);
        return;
    }

    printf("\n--- Atividades de %s (Codigo: %d) ---\n", estudante->nome, estudante->codigo);
    int encontrou = 0;
    Atividade* atividade_atual = agenda->inicio;
    while (atividade_atual != NULL) {
        Participante* participante_atual = atividade_atual->participantes->inicio;
        while (participante_atual != NULL) {
            if (participante_atual->estudante->codigo == codigo_estudante) {
                printf("- %s (Local: %s)\n", atividade_atual->nome_liga, atividade_atual->local);
                encontrou = 1;
                break; 
            }
            participante_atual = participante_atual->prox;
        }
        atividade_atual = atividade_atual->prox;
    }

    if (!encontrou) {
        printf("Nenhuma atividade encontrada para este estudante.\n");
    }
    printf("-----------------------------------------\n");
}

int main() {
    ListaEstudantes* cadastro = criar_lista_estudantes();
    ListaAtividades* agenda = criar_agenda("03/10/2025", "10h-12h");

    insere_estudante(cadastro, criar_estudante(1, "Andre", "EngComputacao"));
    insere_estudante(cadastro, criar_estudante(2, "Beatriz", "CienciaComputacao"));
    insere_estudante(cadastro, criar_estudante(3, "Luiz", "ADS"));
    insere_estudante(cadastro, criar_estudante(4, "Joaquim", "EngSoftware"));
    insere_estudante(cadastro, criar_estudante(6, "Saulo", "CriacaoDigital"));
    insere_estudante(cadastro, criar_estudante(5, "Carla", "Design"));
    
    insere_atividade(agenda, criar_atividade("jogos de cartas", "liga.com/001"));
    insere_atividade(agenda, criar_atividade("livros de ficcao", "liga.com/010"));
    insere_atividade(agenda, criar_atividade("tenis de mesa", "ginasio"));

    insere_participante(agenda, cadastro, "jogos de cartas", 3);
    insere_participante(agenda, cadastro, "jogos de cartas", 2);
    insere_participante(agenda, cadastro, "livros de ficcao", 4);
    insere_participante(agenda, cadastro, "tenis de mesa", 1);
    insere_participante(agenda, cadastro, "tenis de mesa", 6);
    
    printf("\n--- Estado Inicial ---\n");
    consulta_todos_estudantes(cadastro);
    consulta_todas_atividades(agenda);
    consulta_quantidades(agenda);

    printf("\n--- Testando Novas Operacoes ---\n");
    
    // Teste de busca por nome
    Estudante* e = busca_estudante_por_nome(cadastro, "Joaquim");
    if (e) printf("Busca por nome 'Joaquim' encontrou: Codigo %d, Curso %s\n", e->codigo, e->curso);

    // Teste de remoção de estudante (deve falhar, pois está inscrito)
    remove_estudante_por_codigo(cadastro, 1, agenda); 

    // Teste de remoção de estudante (deve funcionar)
    remove_estudante_por_codigo(cadastro, 5, agenda); 
    consulta_todos_estudantes(cadastro);

    // Teste de remoção de atividade
    remove_atividade(agenda, "livros de ficcao");
    consulta_todas_atividades(agenda);

    // Teste de listagem de atividades por estudante
    listar_atividades_do_estudante(agenda, cadastro, 2);
    listar_atividades_do_estudante(agenda, cadastro, 1);
    listar_atividades_do_estudante(agenda, cadastro, 4); // Deve mostrar nenhuma

    return 0;
}