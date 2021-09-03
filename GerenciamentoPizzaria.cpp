
#include "stdio.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

struct clientes
{
	char nome[30];
	char cpf[50];
	double fone;
	char endereco[50];
}MatrizClientes[100];

struct produtos 
{
	char descricao[30];
	float custo;
	float preco;
	int codigo;
	int quantidade;
	int total_vendido;
}MatrizProdutos[100];

struct usuarios
{
	char username[30];
	int senha;
	int unidade;
}MatrizUsuarios[100];

struct funcionarios {
	char nome[30];
	char rg[30];
	double cpf;
	float salario;
	char funcao[15];
}MatrizFuncionarios[100];

struct pedidos {
	int numero_pedido;
	float valor_pedido;
	int status;//1=pendente, 2=confirmado, 3=cancelado
	int codigo_produto;
}MatrizPedidos[100];

//prototipos
int busca_funcionario(double cpf);
int busca_cliente(double cpf);
int busca_produto(int codigo);
void telaInicial();
void tela001();
void tela002();
void tela003();
void tela004();
void tela005();
void tela006();
void telaPedidos();
void limpabuffer();
float lucro = 0.0;//Variavel global que carrega o faturamento
int otimo = 0;
int bom = 0;
int medio = 0;
int ruim = 0;
int pessimo = 0;
int semNota = 0;
int uni = 0;
float uni1 = 0.0, uni2 = 0.0, uni3 = 0.0;//Faturamento de cada unidade
int cancelados = 0;//Numero de pedidos cancelados
int confirmados = 0;//Numero de pedidos confirmados

void limpabuffer()
{
	char c;
	while ((c = getchar()) != '\n' && c != EOF); //Comando pra limpar o buffer
}

void inicializa()
{
	int i;
	FILE* arq = NULL;

	//inicializacao da matriz de produtos 
	for (i = 0; i < sizeof(MatrizProdutos) / sizeof(struct produtos); i++)
	{
		if (i == 0)
			MatrizProdutos[i].codigo = 1;
		else
			MatrizProdutos[i].codigo = -1;
		
		MatrizProdutos[i].quantidade = 0;
		MatrizProdutos[i].custo = 0.0;
		MatrizProdutos[i].preco = 0.0;
		strcpy(MatrizProdutos[i].descricao, "");
		MatrizProdutos[i].total_vendido = 0;

	}
	
	arq = fopen("D:\\Arquivos_Pizzaria\\Produtos.txt","rb");
	if (arq != NULL)
	{	i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&MatrizProdutos[i], sizeof(struct produtos), 1, arq);
			i++;
		}
		fclose(arq);
	}
}

void inicializa_cli()
{
	//Inicializacao da matriz de clientes
	int i;
	FILE* arq = NULL;

	for (i = 0; i < sizeof(MatrizClientes) / sizeof(struct clientes); i++)
	{
		if (i == 0)
			MatrizClientes[i].fone = 1;
		else
			MatrizClientes[i].fone = -1;
		
		
		strcpy(MatrizClientes[i].cpf,"");
		strcpy(MatrizClientes[i].nome, "");
		strcpy(MatrizClientes[i].endereco, "");
	}

	arq = fopen("D:\\Arquivos_Pizzaria\\Clientes.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&MatrizClientes[i], sizeof(struct clientes), 1, arq);
			i++;
		}
		fclose(arq);
		
	}
}

void inicializa_func()
{
	//Inicializacao da Matriz de funcionarios
	int i;
	FILE* arq = NULL;
	for (i = 0; i < sizeof(MatrizFuncionarios) / sizeof(struct funcionarios); i++)
	{
		if (i == 0)
			MatrizFuncionarios[i].cpf = 1.0;
		else
			MatrizFuncionarios[i].cpf = -1.0;
		
		
		strcpy(MatrizFuncionarios[i].nome, "");
		strcpy(MatrizFuncionarios[i].rg, "");
		MatrizFuncionarios[i].salario = 0.0;
		strcpy(MatrizFuncionarios[i].funcao, "");
	}

	arq = fopen("D:\\Arquivos_Pizzaria\\Funcionarios.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&MatrizFuncionarios[i], sizeof(struct funcionarios), 1, arq);
			i++;
		}
		fclose(arq);
	}
}

void inicializa_user()
{
	//Inicializacao da Matriz de usuarios
	int i;
	FILE* arq = NULL;
	for (i = 0; i < sizeof(MatrizUsuarios) / sizeof(struct usuarios); i++)
	{
		if (i == 0)
			MatrizUsuarios[i].senha = 1;

		else
			MatrizUsuarios[i].senha = -1;
		
		strcpy(MatrizUsuarios[i].username, "");
		MatrizUsuarios[i].unidade = 0;
	}
	arq= fopen("D:\\Arquivos_Pizzaria\\Usuarios.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&MatrizUsuarios[i], sizeof(struct usuarios), 1, arq);
			i++;
		}
		fclose(arq);
	}
	
}

void inicializa_pedido()
{
	//Inicializacao da matriz de faturamento
	int i;
	FILE* arq = NULL;
	for (i = 0; i < sizeof(MatrizPedidos) / sizeof(struct pedidos); i++)
	{
		if (i == 0)
			MatrizPedidos[i].numero_pedido = 1;
		else
			MatrizPedidos[i].numero_pedido = -1;

		MatrizPedidos[i].codigo_produto = 0;
		MatrizPedidos[i].status = 0;
		MatrizPedidos[i].valor_pedido = 0.0;
	}
		
	
	arq = fopen("D:\\Arquivos_Pizzaria\\Pedidos.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&MatrizPedidos[i], sizeof(struct pedidos), 1, arq);
			i++;
		}
			fclose(arq);
	}
	
}
void inicializa_faturamento()
{
	//Inicializacao do arquivo de faturamento
	int i;
	FILE* arq = NULL;
	FILE* arq2 = NULL;
	FILE* arq3 = NULL;

	arq = fopen("D:\\Arquivos_Pizzaria\\Faturamento1.txt", "rb");//O faturamento é computado de maneira individual, por isso existem tres arquivos sendo inicializidados
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq) && i < 100)
		{
			fread(&uni1, sizeof(uni1), 1, arq);
			i++;
		}
	}
	fclose(arq);
	arq2 = fopen("D:\\Arquivos_Pizzaria\\Faturamento2.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq2) && i < 100)
		{
			fread(&uni2, sizeof(uni2), 1, arq2);
			i++;
		}
	}
	fclose(arq2);
	arq3 = fopen("D:\\Arquivos_Pizzaria\\Faturamento3.txt", "rb");
	if (arq != NULL)
	{
		i = 0;
		while (!feof(arq3) && i < 100)
		{
			fread(&uni3, sizeof(uni3), 1, arq3);
			i++;
		}
	}
	fclose(arq3);
}

int busca_cliente(double fone)
{
	//Funcao q busca os clientes, controlada pelo telefone que o cliente informar
	int i = 0;
	while (i<= sizeof(MatrizClientes) / sizeof(struct clientes) && MatrizClientes[i].fone != -1)
	{
		if (MatrizClientes[i].fone == fone)
		{
			return i;
		}
		i++;
	}
	if (MatrizClientes[i].fone == -1)
		return -i;

	return -32000;
}

int busca_produto(int codigo)
{
	//Funcao q busca os produtos, controlada pelo codigo do produto
	int i = 0;
	while (i<= sizeof(MatrizProdutos) / sizeof(struct produtos) && MatrizProdutos[i].codigo != -1)
	{
		if (MatrizProdutos[i].codigo== codigo)
		{
			return i;
		}
		i++;
	}
	if (MatrizProdutos[i].codigo == -1)
		return -i;

	return -32000;
}

int busca_funcionario(double cpf)
{
	//Funcao que busca os funcionarios, controlada pelo cpf do funcionario
	int i = 0;
	while (i <= sizeof(MatrizFuncionarios) / sizeof(struct funcionarios) && MatrizFuncionarios[i].cpf != -1)
	{
		if (MatrizFuncionarios[i].cpf == cpf)
		{
			return i;
		}
		i++;
	}
	if (MatrizFuncionarios[i].cpf == -1)
		return -i;
	
	return -32000;
}

int busca_usuario(int senha)
{
	//Funcao que busca o usuario, controlado pela senha
	int i = 0;
	while (i <= sizeof(MatrizUsuarios) / sizeof(struct usuarios) && MatrizUsuarios[i].senha != -1)
	{
		if (MatrizUsuarios[i].senha == senha)
		{
			return i;
		}
		i++;
	}
	if (MatrizUsuarios[i].senha == -1)
		return -i;

	return -32000;
}

int busca_pedido(int numero_pedido)
{
	//Funcao que busca o pedido, controlado pelo numero do pedido
	int i = 0;
	while (i <= sizeof(MatrizPedidos) / sizeof(struct pedidos) && MatrizPedidos[i].numero_pedido != -1)
	{
		if (MatrizPedidos[i].numero_pedido == numero_pedido)
		{
			return i;
		}
		i++;
	}
	if (MatrizPedidos[i].numero_pedido == -1)
		return -i;
	return -32000;
}
void login()
{
	//Login do usuario que ira operar o sistema
	char usuario[30];
	int senha;
	int posicao_user;
	int acerto = 0;

	do 
	{
		printf("Digite o nome de usuario: ");
		fgets(usuario, sizeof(usuario) / sizeof(char) - 1, stdin);
		printf("Digite sua senha: ");
		scanf("%d", &senha);
		posicao_user = busca_usuario(senha);//Procura a senha digitada pelo usuario na matriz com a funcao de busca
		if (posicao_user >= 0) 
		{
			if (strcmp(MatrizUsuarios[posicao_user].username, usuario) == 0)//Caso tenha sido encontrada, compara o nome de usuario digitado com o nome de usuario registrado no vetor
			{
				printf("Bem vindo %s", MatrizUsuarios[posicao_user].username);
				uni = MatrizUsuarios[posicao_user].unidade;
				acerto++;
				system("pause");
			}
			else
			{
				printf("Nome de usuario errado, tente novamente\n");
				limpabuffer();
			}
		}
		else
		{
			printf("Senha incorreta, tente novamente\n");
			limpabuffer();
		}

	} while (acerto == 0);
}

int main()
{
	//Carrega todas as funcoes de inicializacao do codigo, junto com login e a tela inicial
	inicializa_user();
	inicializa();
	inicializa_cli();
	inicializa_func();
	inicializa_pedido();
	inicializa_faturamento();
	login();
	telaInicial();
	return 0;
}



void telaInicial()
{
	//Tela do menu inicial, controla todas as funcionalidades do sistema
	int opcao;
	system("cls");
	do
	{
		printf("\n\n========Tela inical da Pizzaria ==========\n\n");
		printf("\n1 - Pedidos");
		printf("\n2 - Produtos");
		printf("\n3 - Usuarios ");
		printf("\n4 - Funcionarios "); 
		printf("\n5 - Feedback");
		printf("\n6 - Relatorios"); 
		printf("\n7 - Confirmacao de pedidos");
		printf("\n0 - Sair");
		printf("\n\nDigite a Opcao Desejada :");
		scanf("%d", &opcao);
		switch (opcao)
		{
		case 1: tela001(); break;
		case 2: tela002(); break;
		case 3: tela003(); break;
		case 4: tela004(); break;
		case 5: tela005(); break;
		case 6: tela006(); break;
		case 7: telaPedidos(); break;
		}
	} while (opcao != 0);
}

void tela001()
{
	//Controle de Pedidos
	double tel_cliente;
	int posicao_cliente;
	int codigo_produto;
	int posicao_produto;
	int posicao_pedido;
	int numero_pedido;
	int confirma;
	int cadastro;
	int nota;
	int add;
	int confirma2;
	int dolly;
	FILE* arq = NULL;
	

	system("cls");
	printf("\n\n======== Geracao de Pedidos ==========\n\n");
	
	printf("Digite o codigo do produto pedido pelo cliente: ");
	scanf("%d", &codigo_produto);
	posicao_produto = busca_produto(codigo_produto);
	if (posicao_produto >= 0)//localizou o produto pedido
	{
		//Caso localizar, imprime a descricao e o preco do produto
		printf("\n\n Descricao: %s", MatrizProdutos[posicao_produto].descricao);
		printf("\t Preco de venda: %.2f", MatrizProdutos[posicao_produto].preco);
		printf("\n\nConfirmar o pedido? (1 p/sim 0 p/nao) ");
		scanf("%d", &confirma);
		if (confirma == 1)
		{
			printf("\nAtribua um numero para o pedido: ");//Apos a confirmacao, solicita o usuario para atribuir um numero qualquer para identificar o pedido
			scanf("%d", &numero_pedido);
			posicao_pedido = busca_pedido(numero_pedido);
			if (posicao_pedido >= 0)//Checa se o numero digito ja foi utilizado pelo usuario e solicita outro
			{
				do {
					printf("Voce ja usou esse numero!!");
					printf("\nAtribua o numero do pedido: ");
					scanf("%d", &numero_pedido);
					posicao_pedido = busca_pedido(numero_pedido);
				} while (posicao_pedido >= 0);
			}
			//Atribui valores na matriz de pedidos com as informacoes das outras matrizes
			MatrizPedidos[-posicao_pedido].numero_pedido = numero_pedido;
			MatrizPedidos[-posicao_pedido].status = 1;
			MatrizPedidos[-posicao_pedido].valor_pedido+=MatrizProdutos[posicao_produto].preco;
			MatrizPedidos[-posicao_pedido].codigo_produto = MatrizProdutos[posicao_produto].codigo;	
			printf("\nPedido confirmado");
			MatrizProdutos[posicao_produto].quantidade--;
			if (codigo_produto < 100)
			{
				MatrizProdutos[posicao_produto].total_vendido++;//Caso seja uma pizza(qualquer produto com codigo menor a 100) somar ao totalizador que controle qual o produto mais pedido
			}
			printf("\nAdicionar mais um item? (1 p/ sim, 0 p/ nao) ");
			scanf("%d", &add);
			while (add == 1)//Condicao que controla quantos items a mais adicionar no pedido
			{
				printf("Digite o codigo do produto pedido pelo cliente:");
				scanf("%d", &codigo_produto);
				posicao_produto = busca_produto(codigo_produto);
				if (posicao_produto >= 0) {
					printf("\n\nDescricao: %s", MatrizProdutos[posicao_produto].descricao);
					printf("\t Preco de venda: %.2f", MatrizProdutos[posicao_produto].preco);
					printf("\n\nConfirmar outro item? (1 p/ sim, 0 p/ nao) ");
					scanf("%d", &confirma2);
					if (confirma2 == 1) {
						MatrizProdutos[posicao_produto].quantidade--;
						MatrizPedidos[-posicao_pedido].valor_pedido += MatrizProdutos[posicao_produto].preco;

						if (codigo_produto < 100)
						{
							MatrizProdutos[posicao_produto].total_vendido++;
							printf("Ativar promocao Dolly?(1 p/sim 0 p/nao) ");//Comando para adicionar um Dolly gratis ao pedido
							scanf("%d", &dolly);
							if (dolly == 1)
							{
								posicao_produto = busca_produto(102);//Joga um Dolly(Codigo 102) na busca de produtos para retornar a posicao na matriz referente a este produto
								MatrizProdutos[posicao_produto].quantidade--;//Subtrai 1 da quantidade de Dolly no sistema
								printf("Dolly gratis adicionado com sucesso!!");
							}
						}
						printf("\nAdicionar outro item? (1 p/sim 0 p/nao) ");
						scanf("%d", &add);
					}
				}
			}
			printf("\nO cliente deseja cadastro? (1 p/sim 0 p/nao): ");
			scanf("%d", &cadastro);//Comando para iniciar o processo de cadastro
			if (cadastro == 1)
			{
				printf("\nDigite o Telefone do cliente: ");
				scanf("%lf", &tel_cliente);
				posicao_cliente = busca_cliente(tel_cliente);//Busca no sistema se ja existe o telefone digitado no vetor de clientes
				if (posicao_cliente >= 0)//Caso o sistema encontre, imprime as informacoes do cliente
				{
					printf("\n\nCliente ja Cadastrado!!");
					printf("\n\nNome: %s", MatrizClientes[posicao_cliente].nome);
					printf("\n\nCPF: %s", MatrizClientes[posicao_cliente].cpf);
					printf("\n\nEndereco: %s", MatrizClientes[posicao_cliente].endereco);
				}
				else //Caso nao encontre, prossegue o processo de cadastro, extraindo as informacoes do cliente
				{
					printf("\nCliente nao cadastrado\n");
					limpabuffer();
					MatrizClientes[-posicao_cliente].fone = tel_cliente;
					printf("Nome: ");
					fgets(MatrizClientes[-posicao_cliente].nome, sizeof(MatrizClientes[0].nome) / sizeof(char) - 1, stdin);
					printf("CPF: ");
					fgets(MatrizClientes[-posicao_cliente].cpf, sizeof(MatrizClientes[0].nome) / sizeof(char) - 1, stdin);
					printf("Endereco: ");
					fgets(MatrizClientes[-posicao_cliente].endereco, sizeof(MatrizClientes[0].endereco) / sizeof(char) - 1, stdin);
					
					arq = fopen("D:\\Arquivos_Pizzaria\\Clientes.txt", "a+b");
					fwrite(&MatrizClientes[-posicao_cliente], sizeof(struct clientes), 1, arq);//Salva as informaçoes do cliente num arquivo
					printf("\nCliente Cadastrado com sucesso\n");
				}
			}
			printf("\nDigite a nota de atendimento que o cliente te informou: ");
			scanf("%d", &nota);//Atribui uma nota de atendimento dada pelo cliente, com escala de 1 a 5, sendo 5=otimo, 4=bom, 3=medio, 2=ruim, 1=pessimo
			if (nota == 5)
				otimo++;
			else if (nota == 4)
				bom++;
			else if (nota == 3)
				medio++;
			else if (nota == 2)
				ruim++;
			else if (nota == 1)
				pessimo++;
			else
				semNota++;

		}
	}
	else
	{
		printf("\nProduto nao registrado, para cadastrar acesse o menu de produtos");//Caso seja digitado um produto nao registrado
	}
}

void telaPedidos()
{
	//Tela responsavel por controlar pedidos entregues e cancelados
	int numero_pedido;
	int posicao_pedido;
	int status_pedido;
	FILE* arq = NULL;
	printf("\nDigite o numero do pedido");//Solicita o usuario a informar o numero do pedido que ele mesmo atribuiu quando o pedido foi feito
	scanf("%d", &numero_pedido);
	posicao_pedido = busca_pedido(numero_pedido);//Busca a posicao da matriz em que se encontra o numero do pedido
	if (posicao_pedido >= 0)//Caso encontre o pedido, mostra o numero e o valor total deste pedido
	{
		printf("\nPedido numero %d", MatrizPedidos[posicao_pedido].numero_pedido);
		printf("\nValor total %.2f", MatrizPedidos[posicao_pedido].valor_pedido);
		if (MatrizPedidos[posicao_pedido].status == 1)
		{
			printf("\nPedido Pendente\n Digite o status atual do pedido? (2 p/ entregue 3 p/ cancelado)");//Se o pedido estiver pendente, o usuario deve digtar se o pedido foi entregue ou cancelado
			scanf("%d", &status_pedido);
			if (status_pedido == 2)//Se o status for 2(Entregue) adciona o valor na variavel lucro, adciona um no numero de pedidos confirmados e salva o pedido num arquivo
			{
				printf("\nPedido Entregue");
				lucro += MatrizPedidos[posicao_pedido].valor_pedido;
				MatrizPedidos[posicao_pedido].status = 2;
				confirmados++;
				arq = fopen("D:\\Arquivos_Pizzaria\\Pedidos.txt", "a+b");
				fwrite(&MatrizPedidos[posicao_pedido], sizeof(struct pedidos), 1, arq);
				fclose(arq);
			}
			else if (status_pedido == 3)//Se o status for 3(Cancelado) adciona um no numero de pedidos cancelados
			{
				printf("\nPedido Cancelado");
				MatrizPedidos[posicao_pedido].status = 3;
				cancelados++;
			}
		}
	}
	else
		printf("\nPedido Inexistente");//Caso nao achar o numero do pedido
}


void tela002()
{
	//Controle de produtos
	FILE* arq = NULL;
	
	int codigo_produto, posicao_produto;
	int confirma;
	system("cls");
	printf("\n\n======== Cadastro de Produtos ==========\n\n");
	printf("\nDigite o Produto a ser procurado: ");
	scanf("%d", &codigo_produto);
	posicao_produto = busca_produto(codigo_produto);
	if (posicao_produto >= 0)//localizou o produto
	{
		//mostra os dados do produtos na tela
		printf("\n\n Descricao: %s", MatrizProdutos[posicao_produto].descricao);
		printf("\t Quantidade em Estoque: %d", MatrizProdutos[posicao_produto].quantidade);
		printf("\t Preco de venda: %.2f", MatrizProdutos[posicao_produto].preco);
		printf("\t Custo de aquisicao: %.2f\n\n", MatrizProdutos[posicao_produto].custo);
		printf("\nDeseja atualizar a quantidade do produto?");
		scanf("%d", &confirma);
		if (confirma == 1)
		{
			printf("Digite a quantidade atual: ");
			scanf("%d", &MatrizProdutos[posicao_produto].quantidade);
			arq = fopen("D:\\Arquivos_Pizzaria\\Produtos.txt", "a");
			fwrite(&MatrizProdutos[posicao_produto].quantidade, sizeof(struct produtos), 1, arq);
			fclose(arq);
			printf("\nQuantidade atualizada com sucesso\n");
		}
	}
	else
	{
		int cadastro;
		//Comando para cadastrar o produto
		printf("produto nao encontrado.Deseja cadastrar (1 p/sim 0 p/nao): ");
		scanf("%d", &cadastro);
		if (cadastro == 1)
		{
			//caso positivo, entre os dados do produto
			limpabuffer();
			MatrizProdutos[-posicao_produto].codigo = codigo_produto;
			printf("Digite a descricao: ");
			fgets(MatrizProdutos[-posicao_produto].descricao, sizeof(MatrizProdutos[0].descricao) / sizeof(char) - 1, stdin);
			printf("Digite a qtde em estoque: ");
			scanf("%d", &MatrizProdutos[-posicao_produto].quantidade);
			printf("Digite o Preco de venda: ");
			scanf("%f", &MatrizProdutos[-posicao_produto].preco);
			printf("Digite o custo de aquisicao: ");
			scanf("%f", &MatrizProdutos[-posicao_produto].custo);

			arq = fopen("D:\\Arquivos_Pizzaria\\Produtos.txt", "a+b");
			fwrite(&MatrizProdutos[-posicao_produto], sizeof(struct produtos), 1, arq);
			fclose(arq);

			printf("\nProduto registrado com sucesso\n");
		
		}

		//caso negativo, retornar para a tela principal
	}

	system("pause");
}



void tela003()
{
	//Criacao de usuarios
	FILE* arq = NULL;
	int senha;
	int posicao_user;
	int confirma;
	printf("Digite sua Senha: ");//Solicita a senha para realizar uma busca
	scanf("%d", &senha);
	posicao_user = busca_usuario(senha);
	
	if (posicao_user >= 0)//Caso o usuario seja encontrado 
	{
		printf("\nUsuario ja cadastrado!!");
		printf("\n%s", MatrizUsuarios[posicao_user].username);
	}
	else
	{
		printf("\nUsuario nao cadastrado, prosseguir para cadastro?(1 p/sim 0 p/nao)");//Caso nao encontrar o usuario, prosseguir para o processo de cadastro
		scanf("%d", &confirma);
		if (confirma == 1)
		{
			MatrizUsuarios[-posicao_user].senha = senha;
			limpabuffer();
			printf("\nDigite o nome de usuario: ");
			fgets(MatrizUsuarios[-posicao_user].username, sizeof(MatrizUsuarios[0].username) / sizeof(char) - 1, stdin);
			printf("\nDigite a unidade onde esta trabalhando: ");
			scanf("%d", &MatrizUsuarios[-posicao_user].unidade);
			arq = fopen("D:\\Arquivos_Pizzaria\\Usuarios.txt", "a+b");
			fwrite(&MatrizUsuarios[-posicao_user], sizeof(struct usuarios), 1, arq);
			fclose(arq);
			printf("\nUsuario cadastrado com sucesso!");
		}
	}
}



void tela004()
{
	//Controle de Funcionarios
	int cpf_func;
	int posicao_func;
	int confirma;
	FILE* arq = NULL;

	printf("\nControle de Funcionarios\n");
	printf("Digite o cpf do Funcionario:");//Realiza a busca do funcionario pelo CPF
	scanf("%d", &cpf_func);

	posicao_func = busca_funcionario(cpf_func);
	if (posicao_func >= 0)//Caso encontre, imprimir as informacoes
	{
		printf("\nNome: %s", MatrizFuncionarios[posicao_func].nome);
		printf("\nRG: %s", MatrizFuncionarios[posicao_func].rg);
		printf("\nSalario: %.2f", MatrizFuncionarios[posicao_func].salario);
		printf("\nFuncao: %s",MatrizFuncionarios[posicao_func].funcao);
		
	}
	else
	{
		printf("Funcionario nao registrado\n Deseja Registra-lo no sistema?(1 p/sim 0 p/nao)");//Caso nao encontre, prosseguir no processo para registar o funcionario
		scanf("%d", &confirma);
		if (confirma == 1)
		{
			limpabuffer();
			MatrizFuncionarios[-posicao_func].cpf= cpf_func;
			printf("\nNome: ");
			fgets(MatrizFuncionarios[-posicao_func].nome, sizeof(MatrizFuncionarios[0].nome) / sizeof(char) - 1, stdin);
			printf("\nRG: ");
			fgets(MatrizFuncionarios[-posicao_func].rg, sizeof(MatrizFuncionarios[0].rg) / sizeof(char) - 1, stdin);
			printf("\nSalario(p/mes): ");
			scanf("%f", &MatrizFuncionarios[-posicao_func].salario);
			limpabuffer();
			printf("Funcao: ");
			fgets(MatrizFuncionarios[-posicao_func].funcao, sizeof(MatrizFuncionarios[0].funcao) / sizeof(char) - 1, stdin);
			arq = fopen("D:\\Arquivos_Pizzaria\\Funcionarios.txt", "a+b");
			fwrite(&MatrizFuncionarios[-posicao_func], sizeof(struct funcionarios), 1, arq);
			fclose(arq);
			
			printf("\nFuncionario registrado com sucesso");

		}
	}


	

}


void tela005()
{
	//Tela de feedback
	printf("Tela de feedback");
	//Apenas mostra o numero de pedidos com as respecticas notas
	printf("\nAtendimentos otimos: %d", otimo);
	printf("\nAtendimentos bons: %d", bom);
	printf("\nAtendimentos medios: %d", medio);
	printf("\nAtendimentos ruins: %d", ruim);
	printf("\nAtendimentos pessimos: %d", pessimo);
	printf("\n");
	system("pause");

}


void tela006()
{
	//Relatorios
	FILE* arq = NULL;
	FILE* arq2 = NULL;
	FILE* arq3 = NULL;
	int i, maior, pos_maior;
	float soma = 0.0;
	int c;
	
	printf("Tela de relatorios");
	if (uni == 1)//Caso o usuario seja da unidade 1, grava o faturamento na variavel uni1, e no arquivo faturamento 1
	{
		uni1 += lucro;
		arq = fopen("D:\\Arquivos_Pizzaria\\Faturamento1.txt", "a+b");
		fwrite(&uni1, sizeof(uni1), 1, arq);
		fclose(arq);
	}
	else if (uni == 2) //Caso o usuario seja da unidade 2, grava o faturamento na variavel uni2, e no arquivo faturamento 1
	{
		uni2 += lucro;
		arq2 = fopen("D:\\Arquivos_Pizzaria\\Faturamento2.txt", "a+b");
		fwrite(&uni2, sizeof(uni2), 1, arq2);
		fclose(arq2);
	}
	else if (uni == 3)//Caso o usuario seja da unidade 3, grava o faturamento na variavel uni3, e no arquivo faturamento 1
	{
		uni3 += lucro;
		arq3 = fopen("D:\\Arquivos_Pizzaria\\Documents\\Faturamento3.txt", "a+b");
		fwrite(&uni3, sizeof(uni3), 1, arq3);
		fclose(arq3);
	}
	printf("\nRelatorio da Unidade %d", uni);//Mostra qual e a unidade que esta operando no sistema
	printf("\nPedidos Entregues: %d", confirmados);//Mostra quantos pedidos foram entregues
	printf("\nPedidos Cancelados : %d", cancelados);//Mostra quantos pedidos foram cancelados
	printf("\nFaturamento Unidade 1:  %.2f", uni1);//Mostra o faturamento da unidade 1
	printf("\nFaturamento Unidade 2:  %.2f", uni2);//Mostra o faturamento da unidade 2
	printf("\nFaturamento Unidade 3:  %.2f", uni3);//Mostra o faturamento da unidade 3
	if (uni1 > uni2 && uni1 > uni3)//Compara o faturamento das tres unidades e imprime o maior
	{
		printf("\nUnidade com maior faturamento: Unidade 1");
	}
	else if (uni2 > uni1 && uni2 > uni3)
	{
		printf("\nUnidade com maior faturamento: Unidade 2");
	}
	else if (uni3 > uni1 && uni3 > uni2)
	{
		printf("\nUnidade com maior faturamento: Unidade 3");
	}
	printf("\n\nFaturamento total da rede: %.2f", uni1 + uni2 + uni3);//Soma todos os faturamentos e imprime o total da rede
	printf("\n\n");

	maior = MatrizProdutos[0].total_vendido;//Inicializa a variavel de controle
	pos_maior = 0;//Indice da variavel de controle
	for (i = 1; i < sizeof(MatrizProdutos) / sizeof(struct produtos); i++)//Laco para percorrer a matriz
	{
		if (MatrizProdutos[i].total_vendido > maior) {
			maior = MatrizProdutos[i].total_vendido;//Guarda qual e a posicao do numero do produto com maior numero de produtos pedidos
			pos_maior = i;//Guarda o indice da matriz que pertence ao produto mais pedido
		}
	}
	printf("Sabor mais pedido: %s", MatrizProdutos[pos_maior].descricao);//Imprime a descricao do produto mais pedido



	system("pause");



}

