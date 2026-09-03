# Ligas Acadêmicas — listas encadeadas em C

Sistema de cadastro de estudantes e agenda de atividades de ligas acadêmicas, com todas as estruturas de dados implementadas do zero em C, sem biblioteca de coleções.

---

## O problema

Uma liga acadêmica é uma associação estudantil sem fins lucrativos. As atividades de todas as ligas acontecem **em paralelo, no mesmo dia e horário** — o que cria a restrição central do sistema: um estudante inscrito em uma atividade não pode se inscrever em outra.

O sistema gerencia dois domínios:
- **Cadastro** — estudantes mantidos em ordem alfabética permanente
- **Agenda** — atividades do dia, cada uma com seu local e sua lista de inscritos

---

## Decisões de implementação

**Três listas encadeadas simples, todas com header.**
`ListaEstudantes`, `ListaAtividades` e `ListaParticipantes` são estruturas de cabeçalho que guardam o ponteiro para o primeiro nodo e a quantidade de elementos. O contador no header transforma a consulta de quantidade em O(1) em vez de percorrer a lista.

`ListaAtividades` carrega também data e horário do encontro, funcionando como header de agenda além de header de lista.

**Ordem alfabética mantida na inserção, não na consulta.**
`insere_estudante` percorre a lista comparando com `strcmp` e insere o nodo na posição correta — inclusive tratando o caso de inserção no início, que exige atualizar o ponteiro do header. A lista nunca precisa ser ordenada depois: ela nunca está desordenada.

**Participantes referenciam estudantes, não os copiam.**
```c
typedef struct Participante {
    Estudante* estudante;      // ponteiro, não cópia
    struct Participante* prox;
} Participante;
```
Um estudante existe em um único lugar na memória. Se os dados dele mudarem, a mudança aparece em toda atividade em que está inscrito. Copiar a struct criaria cópias que divergiriam silenciosamente.

**Integridade referencial em duas direções.**
- `insere_participante` recusa a inscrição se o estudante não estiver cadastrado, ou se já estiver inscrito em outra atividade — a regra do horário paralelo
- `remove_estudante_por_codigo` consulta `estudante_ja_inscrito` antes de remover. Não é possível deletar um estudante que tem inscrição ativa, o que evitaria ponteiro pendurado na lista de participantes

**Liberação de memória em cascata.**
Remover uma atividade chama `liberar_participantes`, que percorre a lista de inscritos dando `free` em cada nodo antes de liberar o header e a própria atividade. Nenhum nodo fica órfão.

---

## Estruturas

```
ListaEstudantes (header)          ListaAtividades (header + data/horário)
  └── Estudante                     └── Atividade
        codigo, nome, curso               nome_liga, local
        prox ─────────┐                   participantes ──► ListaParticipantes
                      ▼                   prox ──────────┐        └── Participante
                  Estudante                              ▼              estudante ──┐
                                                     Atividade          prox         │
                                                                                     ▼
                                                                                 Estudante
```

---

## Operações

**Cadastro**
| Operação | Comportamento |
|---|---|
| `insere_estudante` | Inserção ordenada alfabeticamente |
| `remove_estudante_por_codigo` | Bloqueada se o estudante tiver inscrição ativa |
| `busca_estudante_por_codigo` / `por_nome` | Busca linear |
| `consulta_todos_estudantes` | Lista em ordem alfabética |

**Agenda**
| Operação | Comportamento |
|---|---|
| `insere_atividade` | Insere no fim da agenda |
| `remove_atividade` | Libera em cascata a lista de participantes |
| `insere_participante` | Valida cadastro e inscrição única |
| `consulta_atividade` | Detalhes da atividade e seus inscritos |
| `consulta_quantidades` | Total de participantes por atividade |
| `consulta_todas_atividades` | Agenda completa do dia |
| `listar_atividades_do_estudante` | Busca reversa: em que atividade o estudante está |

`listar_atividades_do_estudante` é uma operação adicional, fora do escopo mínimo do enunciado.

---

## Como executar

```bash
gcc ligas.c -o ligas
./ligas
```

A `main` popula cadastro e agenda com dados de exemplo e executa uma bateria de testes que inclui os dois casos de falha esperados: remoção de estudante inscrito (deve ser recusada) e remoção de estudante livre (deve funcionar).

---

## Restrição do trabalho

O enunciado proíbe explicitamente o uso de classes ou bibliotecas prontas que mantenham encadeamento automático. Todos os ponteiros entre nodos são manipulados diretamente pelo código, incluindo os casos de borda de inserção e remoção na cabeça da lista.

---

Trabalho de implementação — Algoritmos e Estruturas de Dados I, Universidade de Caxias do Sul (UCS).
