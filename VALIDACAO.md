# Validação da atualização UTF-8

**Data:** 21/09/2026.

**Base:** `14a8befe0a03283e87e91603e8ced70e9a537888`, do repositório [pauloregismsuva/Clic](https://github.com/pauloregismsuva/Clic).

**Ambiente:** Linux x86_64, GCC 13.3.0, glibc 2.39, Python 3 e localidade UTF-8.

**Escopo:** largura visual, armazenamento das células, truncamento, alinhamento, centralização e liberação da memória alocada para tabelas.

## Como reproduzir

Na raiz do repositório:

```bash
make test
make test-allocations
make examples
```

`make test` requer uma localidade UTF-8 instalada e Python 3 para os cenários de saída e pseudoterminal. O teste tenta a localidade do ambiente e alternativas comuns. `make test-allocations` usa a opção `--wrap` do linker GNU/LLVM; essa verificação adicional foi executada em Linux.

Os testes de entradas inválidas imprimem mensagens esperadas em `stderr`. O resultado final deve incluir `text width: OK`, `cell storage: OK` e `rendering: 14 scenarios OK`, com código de saída zero.

## Largura visual

`tests/test_text.c` verifica strings vazias, ASCII, acentos, cedilha, caracteres de duas colunas, marcas combinantes, reticências, sequências incompletas/inválidas, controles e troca entre localidade `C` e UTF-8. A função retorna `-1` para textos inválidos/não imprimíveis e não altera a localidade.

## Armazenamento e liberação

`tests/test_rows.c` verifica:

- Preservação de um texto com 802 bytes, incluindo um caractere multibyte que atravessa o antigo limite de 299 bytes.
- Formatação de inteiros e números com casas decimais.
- Cópia independente da string de origem.
- Rejeição de uma segunda célula inválida sem adicionar uma linha parcial.
- Strings nulas, controles e precisão de `%s` que corta um caractere.
- Rejeição de formatos não suportados antes de consumir argumentos incompatíveis.
- Criação e liberação repetidas de tabelas.

O alvo `test-allocations` intercepta `malloc`, `calloc` e `free` chamados pelos objetos compilados no teste. O saldo final foi **zero**, inclusive nas falhas parciais de construção de linhas. Esse contador não monitora todas as alocações internas da biblioteca C do sistema e não substitui um detector geral de vazamentos.

## Renderização

`tests/test_render.py` compara a saída UTF-8 e os espaços emitidos com layouts esperados, usando o programa `tests/render_fixture.c`. Não se limita a repetir a função de medição para decidir se a borda está correta.

| Cenário | Verificação |
| --- | --- |
| Acentos | `João`, `Ação` e `Saúde`, com alinhamentos diferentes. |
| Truncamento | Reticências após caracteres completos e preservação de acentos combinantes. |
| Caractere largo próximo ao limite | Espaço restante preenchido corretamente quando o próximo caractere ocupa duas colunas. |
| Largura personalizada | `%12s` preenchido na exibição, sem espaços artificiais armazenados. |
| Todas as colunas personalizadas | Desenho sem divisão por zero. |
| Larguras personalizadas excessivas | Redução das colunas para caber na largura total. |
| Tabela pequena demais | Nenhum desenho quando nem bordas e separadores cabem. |
| Célula de uma coluna | Somente o marcador de truncamento quando necessário. |
| Célula de largura zero | Nenhum byte de conteúdo impresso. |
| Localidade `C` | Truncamento de texto ASCII com `.`. |
| Célula longa | Texto com 800 bytes e coluna de largura superior a 300. |
| Centralização com entrada inválida | Retorno zero e ausência de saída. |
| Centralização com acentos | Coluna inicial 9 para `João` em um terminal de 20 colunas. |
| Título maior que a tela | Truncamento na largura da tela e início na coluna 1. |

**Resultado:** 14 cenários aprovados. Os dois casos de centralização válida usam pseudoterminal; os demais comparam a saída emitida em uma largura fixada pelo teste.

## Compilação e exemplos

Os testes e os oito exemplos foram compilados com:

```text
-std=gnu11 -Wall -Wextra -Werror
```

**Resultado:** compilação concluída sem avisos com essas opções. A antiga comparação entre `strlen` e a largura da coluna foi removida. Isso não implica ausência de avisos em outras combinações de otimização e opções: com `-O2`, o código preexistente de `Clic_getCursorPosition()` ainda pode emitir aviso por ignorar o retorno de `fscanf`.

Também foram executados em pseudoterminal de 80 colunas por 24 linhas:

- `05_tabela.c`, conferindo o conteúdo das três linhas.
- `06_selecao_tabela.c`, selecionando `Instrucoes` com seta para baixo e Enter.
- `08_tabela_utf8.c`, conferindo acentos, caracteres largos e reticências.

Todos encerraram com código zero e mantiveram os modos canônico e de eco ativos. A aparência exata das cores e fontes não foi inspecionada em diferentes emuladores.

## Verificação de acessos à memória

A suíte `make test` também passou com AddressSanitizer e UndefinedBehaviorSanitizer:

```bash
ASAN_OPTIONS=detect_leaks=0 make BUILD_DIR=build/sanitize \
  CFLAGS='-std=gnu11 -Wall -Wextra -Werror -g -fsanitize=address,undefined -fno-omit-frame-pointer' test
```

LeakSanitizer não pôde funcionar neste ambiente porque a execução é supervisionada por `ptrace`. Por isso, a verificação de vazamentos foi limitada ao contador de alocações descrito acima. AddressSanitizer e UndefinedBehaviorSanitizer não relataram erros nos cenários executados.

## Limites da atualização

A atualização usa a soma das larguras de `wcwidth`, não uma segmentação completa de grafemas. Emojis compostos, seletores de apresentação, fontes e caracteres de largura ambígua podem exigir tratamento específico do terminal.

As limitações anteriores do analisador de formatos, da navegação de `Table_select()` e da captura de teclas permanecem documentadas em `README.md`. Os testes não cobrem Windows nativo, macOS, todas as falhas possíveis de alocação, todas as entradas inválidas ou todos os emuladores.
