# Sistemas Operativos

*  Thayná Pereira
* Fábio Amado

Este projeto prático tem como objetivo: uma maior aproximação e desenvolvimentos de conhecimentos em relação a programação de mecanismos de sincronização e comunicação entre processos, no ambiente Linux. Para a realização deste projeto recorreu-se à linguagem C e ao sistema operativo Linux, no qual se pretende desenvolver uma aplicação cliente/multithreaded server, que implemente um serviço de respostas de acordo com um determinado conjunto de pedidos dos clientes.

## Versões
Versão 1. Implementação do modelo cliente-servidor um para um, i.e., o serviço deve estar a funcionar apenas na modalidade de um programa cliente (com a possibilidade de ser utilizados por vários utilizadores) em comunicação com um programa servidor. Nesta versão, o servidor não tem de gerir utilizadores, apenas os pedidos que recebe de forma sequencial e garantir que dá a resposta certa pela mesma ordem.

Versão 2. Implementação do modelo cliente-servidor vários para um, i.e., o serviço deve estar a funcionar na modalidade de quantos programa cliente quisermos (com a possibilidade de serem utilizados por vários utilizadores cada um) em comunicação com um programa servidor. Nesta versão, o servidor já tem de gerir utilizadores, i.e., tem de gerir múltiplos pedidos independentes e garantir que dá a resposta certa ao utilizador correto. Para tal, é necessário implementar a técnica multithreading para a gestão eficiente dos pedidos e permitir dar uma resposta mais rápida aos utilizadores do serviço.

Versão 3. Implementação do modelo cliente-servidor vários para um, com um limite máximo de ligações em simultâneo, i.e., aplicação cliente/multithreading servidor (Versão 2) com a imposição de um limite máximo de utilizadores ativos em simultâneo. Nesta versão, o servidor terá de controlar também o número total de utilizações (comunicações, i.e., utilizadores ativos) em simultâneo e caso 
$ ./cliente CANAL start Iscac www.iscac.pt open Não existe resposta ao seu pedido -> open end Bom dia Tem de iniciar o serviço introduzindo a palavra “start” start Bom dia Bom dia end atinja o máximo permitido, não pode aceitar mais ligações dos clientes. Isto significa que podemos ter em execução quantas aplicações cliente quisermos, mas apenas vamos ter no máximo o limite estabelecido de utilizadores ativos. 
 
