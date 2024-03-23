### Considere o manual de programação de sistema da arquitetura AMD64 disponível em:
https://www.amd.com/system/files/TechDocs/24593.pdf (Secções 5.3.3, 5.4 e 5.6)
#### Explique como se aplicam as proteções de Read/Write, User/Supervisor e No Execute quando, na tradução de um endereço, a definição dos respectivos bits de proteção não é igual nas entradas das tabelas em todos os 4 níveis de tradução (por exemplo, as entradas das tabelas PML4 e PDP proíbem acessos de escrita, mas as entradas das tabelas PD e PT permitem-nos).

> Na tradução de um endereço, os bits de proteção não estão nas mesmas posições entre tabelas, no entanto, a partir da tabela
> de mais alto nivel, conseguimos saber a do nivel inferior consequente através do "Translation-Table Base Address Field"
> que mapeia todos os bits acima de do bit 11 , os últimos 12 bits são passados pelo "Physical-Page Offset" á ultima camada,
> o "Physical Address"
>
> Os bits que que representam o Read/Write, User/Supervisor e No Execute, são o 1, 2 e 63 respetivamente.

