# Validação da documentação e dos exemplos

**Data:** 17/09/2026.  
**Fonte:** [pauloregismsuva/Clic](https://github.com/pauloregismsuva/Clic), commit `e697e0cb72976f45df7bcacf8c24cfd1e690d9d3`.  
**Compilador:** GCC 13.3.0.  
**Ambiente:** Linux x86_64, glibc 2.39.  
**Execução interativa:** pseudoterminal POSIX com 80 colunas e 24 linhas, `TERM=xterm-256color`.

## Compilação

Os sete arquivos de `exemplos/` foram compilados individualmente, vinculados ao `Clic.c` original, com as opções:

```bash
gcc -std=gnu11 -Wall -Wextra -I. exemplos/01_cores.c Clic.c -o cores
```

O nome do exemplo e o do executável foram ajustados para cada programa.

**Resultado:** sete compilações concluídas com sucesso. Nenhum diagnóstico foi atribuído ao código dos exemplos. O compilador emitiu um aviso na implementação original, em `Table_printHighlight()`, pela comparação entre `strlen(cell.content)` (`size_t`, sem sinal) e `cellWidth` (`int`, com sinal):

```text
warning: comparison of integer expressions of different signedness
[-Wsign-compare]
```

O aviso permanece na biblioteca consultada e não impediu a compilação com as opções usadas. Os comandos não utilizam `-Werror`.

## Execução

Foram verificados o término dos programas, os textos esperados, a recepção de sequências usuais das setas e a preservação dos modos canônico e de eco no encerramento normal.

| Exemplo | Cenário verificado | Resultado |
| --- | --- | --- |
| `01_cores.c` | Impressão das mensagens e encerramento. | Concluído, código de saída 0. |
| `02_caixa.c` | Emissão do desenho e dos textos posicionados. | Concluído, código de saída 0. |
| `03_teclado.c` | Cima, baixo, direita, esquerda e saída com `q`. | As quatro direções foram identificadas. |
| `04_menu.c` | Baixo e Enter. | Retornou a opção `Instrucoes`. |
| `04_menu.c` | Cima a partir da primeira opção e Enter. | A navegação circular selecionou `Sair`. |
| `04_menu.c` | Cancelamento com `q`. | Exibiu `Menu cancelado`. |
| `05_tabela.c` | Desenho de três linhas com texto, inteiro e decimal. | Conteúdos esperados presentes; saída 0. |
| `06_selecao_tabela.c` | Baixo e Enter. | Selecionou `Instrucoes`. |
| `06_selecao_tabela.c` | Cima a partir da primeira linha e Enter. | O retorno 0 foi tratado como seleção inválida. |
| `06_selecao_tabela.c` | Envio do byte 127 na primeira linha. | Retorno negativo tratado como solicitação; nenhuma exclusão automática. |
| `07_animacao.c` | Execução sem entrada inicial e saída posterior com `q`. | Produziu seis quadros antes de encerrar; saída 0. |

**Resultado:** os 11 cenários concluíram com sucesso. Os modos de eco e entrada canônica estavam ativos após o encerramento normal de cada execução.

## Correspondência com a API

Foram conferidos os nomes e as assinaturas das **39 funções públicas** declaradas em `Clic.h`. Todos aparecem na referência. As tabelas de cores, símbolos, teclas e estruturas foram produzidas a partir desse cabeçalho.

As observações sobre o índice zero de `Table_select()` foram verificadas também em execução. As demais particularidades descritas, incluindo a liberação incompleta de memória, foram identificadas pela leitura de `Clic.c`; os exemplos não corrigem essas funções.

## Alcance da verificação

A execução em pseudoterminal verificou entrada, saída e encerramento. Não equivale a uma inspeção visual em todos os emuladores de terminal. A aparência exata das cores, a largura de emojis e o desenho de caracteres Unicode dependem do terminal e da fonte.

Não foram realizados testes de Windows nativo, macOS, todas as sequências possíveis de teclado, tratamento de sinais, falhas forçadas de alocação, carga prolongada ou todas as entradas inválidas. A consulta `Clic_getCursorPosition()` não foi exercitada, pois exige uma resposta de posição fornecida por um emulador de terminal. Os resultados não constituem uma certificação geral da biblioteca nem eliminam as limitações descritas em `README.md`.
