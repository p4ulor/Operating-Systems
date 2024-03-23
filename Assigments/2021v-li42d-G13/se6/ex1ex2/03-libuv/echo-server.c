#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 


#include <uv.h>

#define PORT1 54321
#define PORT2 56789
#define PORT3 54345
//sudo apt install libuv1-dev
//gcc -Wall -pedantic -o echo-srv echo-server.c -luv
//./echo-srv 54321
//telnet localhost 54321
typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *wreq) {
	write_req_t * req = (write_req_t *)wreq;
	free(req->buf.base);
	free(req);
}

void error(int err, const char * msg) {
	fprintf(stderr, "%s: %s\n", msg, uv_strerror(err));
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t * handle) {
	puts("** on close **");
	free(handle);
}

void after_write(uv_write_t *req, int status) {
	if (status) {
		error(status, "write error");
	}
	free_write_req(req);
}

char *toLower(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}
char *toUpper(char *str, size_t len)
{
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = toupper((unsigned char)str[i]);
    }
    return str_l;
}
void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	//ja chegaram os dados
	//socket(client) tamanho(nread) e (buf)(suposto estar já os dados)
	printf("** read: %d **\n", (int)nread);
	if (nread > 0) {
		buf->base[nread] = 0;
    	size_t len = strlen(buf->base);
    	char *lower = toLower(buf->base, len);
		printf("server received %d bytes: %s\n", (int)nread, lower);
		write_req_t * req = (write_req_t *)malloc(sizeof (write_req_t)); 
		req->buf = uv_buf_init(lower, nread);
		uv_write((uv_write_t *)req, client, &(req->buf), 1, after_write);
		free(lower);
		return;
	}
	if (nread < 0) {
		if (nread == UV_EOF) {
			puts("** eof **");
		} else {
			error(nread, "read error");
		}
		uv_close((uv_handle_t *)client, on_close);
	}
}
void on_read2(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
	printf("** read: %d **\n", (int)nread);
	if (nread > 0) {
		buf->base[nread] = 0;
    	size_t len = strlen(buf->base);
    	char *upper = toUpper(buf->base, len);
		printf("server received %d bytes: %s\n", (int)nread, upper);
		write_req_t * req = (write_req_t *)malloc(sizeof (write_req_t)); 
		req->buf = uv_buf_init(upper, nread);
		uv_write((uv_write_t *)req, client, &(req->buf), 1, after_write);
		free(upper);
		return;
	}
	if (nread < 0) {
		if (nread == UV_EOF) {
			puts("** eof **");
		} else {
			error(nread, "read error");
		}
		uv_close((uv_handle_t *)client, on_close);
	}
}
void prepare_client(uv_tcp_t * client) {
	struct sockaddr_in cli_addr;
	int cli_addr_len = sizeof (struct sockaddr_in);
	uv_tcp_getpeername(client, (struct sockaddr *)&cli_addr, &cli_addr_len);
	//ir buscar o endereco preenche o cli_addr
	char cli_addr_str[INET_ADDRSTRLEN];
	uv_ip4_name(&cli_addr, cli_addr_str, INET_ADDRSTRLEN);
	//conversao
	printf("** server established connection with %s in port %d**\n", cli_addr_str, ntohs(cli_addr.sin_port));
	
	uv_read_start((uv_stream_t *)client, alloc_buffer, on_read);
	//nao bloqueia só serve para configurar
	//iniciar uma leitura dizer qual o socket(client) e qual a leitura(on_read)
	//on_read nao fica bloqueado vamos configurar o epool e vamos deixar registado nas estruturas internas do loop 
	//o que e que e para fazer quando houver um evento de read neste socket
	//alloc_buffer quando houver um read os dados vao ser transferido do kernel para user mode
	//já passava o buffer ou alocar o vuffer so quando os dados estao disponiveis
}
void prepare_client2(uv_tcp_t * client) {//Repetido
	struct sockaddr_in cli_addr;
	int cli_addr_len = sizeof (struct sockaddr_in);
	uv_tcp_getpeername(client, (struct sockaddr *)&cli_addr, &cli_addr_len);
	char cli_addr_str[INET_ADDRSTRLEN];
	uv_ip4_name(&cli_addr, cli_addr_str, INET_ADDRSTRLEN);
	printf("** server established connection with %s in port %d**\n", cli_addr_str, ntohs(cli_addr.sin_port));
	uv_read_start((uv_stream_t *)client, alloc_buffer, on_read2);
}

void on_new_connection(uv_stream_t * server, int status) {//tem um cliente
	puts("** on new connection **");
	if (status < 0) {
		error(status, "new connection failed");
		return;
	}

	uv_tcp_t * client = (uv_tcp_t *)malloc(sizeof (uv_tcp_t));
	//tem de ser associado ao mesmo loop do servidor mesmo epoolwait
	uv_tcp_init(server->loop, client);

	int res = uv_accept(server, (uv_stream_t *)client);
	//quando temos uma connection queremos accept
	//socket de entrada(server) socket de saida(client)
	if (res == 0) {
		prepare_client(client);
	} else {
		uv_close((uv_handle_t *)client, on_close);
	}
}
void on_new_connection2(uv_stream_t * server, int status) {//Repetido
	puts("** on new connection **");
	if (status < 0) {
		error(status, "new connection failed");
		return;
	}
	uv_tcp_t * client = (uv_tcp_t *)malloc(sizeof (uv_tcp_t));
	uv_tcp_init(server->loop, client);
	int res = uv_accept(server, (uv_stream_t *)client);
	if (res == 0) {
		prepare_client2(client);
	} else {
		uv_close((uv_handle_t *)client, on_close);
	}
}


void prepare_server(uv_tcp_t * server, int port) {
	struct sockaddr_in srv_addr;
	uv_ip4_addr("0.0.0.0", port, &srv_addr);
	//criar o endereco para ficar a escuta em todas as interfaces no porto indicado
	
	int res = uv_tcp_bind(server, (const struct sockaddr *)&srv_addr, 0);
	//
	if (res) {
		error(res, "bind failed");
		exit(1);
	}
	
	res = uv_listen((uv_stream_t *)server, 5, on_new_connection);
	//
	//on_new_connection uma funcao q devera ser chamada sempre q for estabelecida ligaco com este servidor
	//colocar numa funcao a parte
	if (res) {
		error(res, "listen failed");
		exit(1);
	}
}

void prepare_server2(uv_tcp_t * server, int port) {//Repetido
	struct sockaddr_in srv_addr;
	uv_ip4_addr("0.0.0.0", port, &srv_addr);
	int res = uv_tcp_bind(server, (const struct sockaddr *)&srv_addr,0);
	if (res) {
		error(res, "bind failed");
		exit(1);
	}
	res = uv_listen((uv_stream_t *)server, 5, on_new_connection2);
	if (res) {
		error(res, "listen failed");
		exit(1);
	}
}

int main(int argc, char * argv[]) {
	if (argc != 3) {
		fprintf(stderr, "use: %s <port>\n", argv[0]);
		exit(1);
	}
	int port1 = atoi(argv[1]);
	int port2 = atoi(argv[2]);
	//int port3 = atoi(argv[3]);
	
	puts(":: START ::");

	uv_loop_t loop;
	uv_loop_init(&loop);//capacidade de processamento da libuv

	/*uv_tcp_t server;//socket tcp
	uv_tcp_t server2;//socket tcp
	uv_tcp_init(&loop, &server);//Represents a TCP stream or TCP server.
	uv_tcp_init(&loop, &server2);//Represents a TCP stream or TCP server.
	*/
	uv_tcp_t *server = malloc(sizeof(uv_tcp_t));
	uv_tcp_t *server2 = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(&loop, server);
	uv_tcp_init(&loop, server2);
    uv_tcp_keepalive(server, 1, 60);
	uv_tcp_keepalive(server2, 1, 60);
	
	prepare_server(server, port1);
	prepare_server2(server2, port2);
	//Trata texto ASCII e devolvido com os caracteres na gama A-Z convertidos para MAISCULA
	//Trata texto ASCII e devolvido com os caracteres na gama A-Z convertidos para MINUSCULA

	//prepare_server(&server, port2);
	//vou fazer configuracoes ao kernel nada disso bloqueante, a uma operacao bloqueante que para nos acontecimentos
	uv_run(&loop, UV_RUN_DEFAULT);// e aqui que o programa fica bloqueado
	
	uv_loop_close(&loop);

	puts(":: END ::");

	return 0;
}