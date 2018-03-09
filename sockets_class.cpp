#include <stdio.h>
#include <winsock.h>
#include <string.h>
#include <string>
class socketBase{
private:
	WSADATA data;
	struct sockaddr_in config;
	char ip[16];
	
public:
	int sock;
	int port;
	
	socketBase(){}
	
	socketBase(int port){
		WSAStartup(MAKEWORD(1,1),&(this->data));
		this->sock=socket(AF_INET,SOCK_STREAM,0);
		if(this->sock == -1){
			// erro ao criar socket
			this->~socketBase();
		}else{
			this->config.sin_family=AF_INET;
			for(int i=0;i<8;i++){
				this->config.sin_zero[i]='\0';
			}
			this->config.sin_port=htons(port);			
		}
	}
	
	
	int addIp(std::string ip){
	/*
		adiciona o ip na variavel ip, mas antes
		verifica se ele e maior que 16 pois e o
		tamanho maximo considerando o modelo
		xxx.xxx.xxx.xxx
	*/
		if(ip.length()>16){
			//ip invalido
			return 0;
		}
		strcpy(this->ip,ip.c_str());
		this->config.sin_addr.s_addr=inet_addr(this->ip);
		return 1;
	}
	
	
	int addIp(){
	/*
		configura o ip para qualquer interface
		aceitar a conexao antes de usar o bind
	*/
		this->config.sin_addr.s_addr=INADDR_ANY;
		return 1;
	}
	
	
	int sbind(){
	/*
		realiza o bind (vincula um socket a uma porta)
	*/
		if(bind(this->sock,(struct sockaddr*)&this->config,sizeof(this->config))==-1){
			//erro de bind (verifique suas configuracoes)
			return 0;
		}
		return 1;
	}
	
	
	inline int slisten(int nClientes){
	/*
		coloca o socket em mode de escuta para aceitar conexoes
		nClientes especifica quantos clientes podem conectar
	*/
		listen(this->sock, nClientes);
	}
	
	
	int accpt(){
	/*
		retorna um identificador para o socket
		atribua ele para um inteiro ou
		(socketBase.sock)
	*/
		int aux;
		while((aux=accept((this->sock),0,0))==-1){}
		return aux;
	}
	
	
	int conn(){
	/*
		conecta ao endereco especificado nas configuracoes
	*/
		if(connect(this->sock,(struct sockaddr*)&this->config,sizeof(this->config))==-1){
			return 0;
		}
		return 1;
	}
	
	
	int snd(std::string msg){
	/*
		envia o conteudo contido na string msg para
		quem estiver conectado e retorna o numero de
		bytes enviados
	*/
		int lMsg=msg.length()+1;
		char cmsg[lMsg];
		strcpy(cmsg, msg.c_str());
		return send(this->sock,cmsg,lMsg,0);
		
	}
	
	
	int rcv(std::string& buf, int len){
	/*
		recebe a quantidade de bytes especificados
		no parametro len, e salva na string buf
		e ao final retorna a quantidade de bytes
		recebidos
	*/
		int bytes;
		char buff[(len+1)];
		buf="";
		bytes=recv(this->sock,buff,len,0);
		buf=std::string(buff);
		return bytes;
	}
	
	
	int rcvAll(std::string& saida){
	/*
		enquanto tiver bytes para serem recebidos
		ela chama a funcao this->rcv e salva toda
		a saida no paremetro saida
	*/
		std::string aux;
		saida="";
		while(this->rcv(aux, 100)>0){
			saida+=aux;
			aux="";
		}
	}
	
	
	~socketBase(){
	/*
		destrutor da classe fecha o socket e
		limpa o registro WSA
	*/
		closesocket(this->sock);
		WSACleanup();
	}
	
	
};
