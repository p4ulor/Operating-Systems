a)

Sabendo que temos 3 níveis, que cada PTE ocupa 4 bytes e que no nível 1
temos 8 bits (2^8=256), no nível 2 temos 10 bits (2^10 = 1024) e que
no nível 3 temos 10 bits (2^10 = 1024), podemos concluir que para
representar o endereço virtual, temos de ter em conta a soma de cada
tabela de páginas de cada nível bem como os bits de deslocamento
(offset) , ou seja, nesse caso temos que o Bits Endereço Virtual = Bits
tabela de nível 1 + Bits tabela de nível 2 + Bits tabela de nível 3 +
Bits de deslocamento (offset) logo, Endereço Virtual = 8+10+10+offset =
28+offset bits. Para obter o offset, multiplicamos a tabela de nível 3
com o valor de cada PTE, 2^10 x  2^2 = 2^12 e temos 12 bits. Logo,
para o endereço virtual temos 28+12=40 bits.

b)

Espaço de endereçamento físico = Espaço de endereçamento virtual / 4 =
2^40 / 4 = 2^40 / 2^2 = 2^38 = 38 bits.
Se a PTE possui 4 bytes = 32 bits

38-32 = 6 bits.

c)

Apenas a tabela de nível 1 pode crescer para 1024, aumentando de 2^40
para 2^42.

d)

Endereço virtual 0x701053CC
Endereço base página física 0x400000

Assumindo um espaço de endereçamento virtual de 32 bits, 2^32 bytes = 4GB.
Assumindo um tamanaho de uma página de 2^12 bytes
Offset de 12 bits
e cada PTE a 4 bytes
As tabelas de nível 1 têm 2^8 bytes = 8 bits (256)
As tabelas de nível 2 e 3 têm 2^10 bytes = 10 bits (1024)


Tamanho total das tabela de páginas de nível 1 é de 256*4 bytes = 1024 bytes.

Tamanho total das tabela de páginas de nível 2 e nível 3 é de 1024*4 bytes = 4096 bytes = 4 KBytes.

0x701053CC

0x70 índice da Page Table Entry da tabela nível 1
0x10 índice da Page Table Entry tabela nível 2
0x53 índice da Page Table Entry tabela nível 3
0xCC offset (12 bits)

endereço fisico final = endereço base página fisica + offset = 0x400000 + 0xCC = 0x4000CC



e)

Nesta arquitetura, cada vez que um processo emite um endereço, a MMU vai ter de passar pelas tabelas existentes, todas elas presentes na memória física.
Para chegar até à memória física, ocorre a tradução de endereços lógicos para endereços físicos, desta forma, aceder a memória física, ou seja, temos de ir a Page Table Entry da tabela de nivel 1
para obter a page frame number, que a MMU foi buscar a RAM (memória física), e que contém o endereço da tabela de nivel 2. De seguida, vamos a Page Table Entry da tabela de nivel 2
obter a page frame number que contém o endereço da tabela de nível 3. Por fim, vamos a Page Table Entry da tabela de nível 3 obter o número da page frame que
contém exatamente a página física a que se quer aceder no endereço emitido pelo processo. Com o offset do mesmo acedemos ao endereço físico efetivo.
Em suma, para um acesso a RAM temos que fazer 4 acessos, 1º a Tabela Nivel 1, em seguida a Tabela Nivel 2, em 3º aceder a Tabela Nivel 3 e por fim adecemos aos dados efetivamente pretendidos 
