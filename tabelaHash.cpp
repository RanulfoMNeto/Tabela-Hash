#include <iostream>

using namespace std;

class noh{
	friend class tabelaHash;
	friend class lista;
	private:
		string escrito;
		int chave;
		noh* proximo;
		noh* anterior;
		
	public:
		noh(string escrito = "", int chave = 0);
};

noh::noh(string escrito, int chave){
	
	this->escrito = escrito;
	this->chave = chave;
	proximo = NULL;
	anterior = NULL;
}

class lista{
	
	friend class tabelaHash;
	
	private:
		noh* primeiro;
		noh* ultimo;
		int tamanho;
		
	public:
		lista();
		~lista();
		void inserir(string escrito, int chave);
		void removerNaPosicao(int posicao);
		int procurar(int chave);
		void imprimir();
		inline bool vazia();
};

lista::lista(){
	
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}

lista::~lista(){
	
	noh* aux = primeiro;
	noh* temp;
	
	while(aux != NULL){
		
		temp = aux;
		aux = aux->proximo;
		delete temp;
	}
	
	tamanho = 0;
	primeiro = NULL;
	ultimo = NULL;
}

inline bool lista::vazia(){
	
	return (tamanho == 0);
}

void lista::imprimir(){
	
	noh* aux = primeiro;
	if (aux != NULL) {
		cout << aux->chave << " " << aux->escrito << " ";
		if (aux->proximo != NULL) {
			cout << endl << "Lista de colisao: ";
			aux = aux->proximo;
			while(aux != NULL){
				cout << aux->chave << " " << aux->escrito;
				if (aux->proximo != NULL)
					 cout << ", ";
				aux = aux->proximo;
			}
		}
		
	}
	cout << endl;
}

void lista::inserir(string escrito, int chave){
	
	noh* novo = new noh(escrito, chave);
	
	if(vazia()){
		
		primeiro = novo;
		ultimo = novo;
	}
	
	else{
		
		ultimo->proximo = novo;
		novo->anterior = ultimo;
		ultimo = novo;
	}
	
	tamanho++;
}

void lista::removerNaPosicao(int posicao){
	
	if(!vazia()){
		
		noh* aux = NULL;
		
		if((posicao < tamanho) and (posicao >= 0)){
			
			if(posicao == 0){
				
				aux = primeiro;
				
				primeiro = primeiro->proximo;
				if(primeiro != NULL)
					primeiro->anterior = NULL;
				delete aux;
			}
			
			else if(posicao == tamanho - 1){
				
				aux = ultimo;
				
				ultimo = ultimo->anterior;
				if(ultimo != NULL)
					ultimo->proximo = NULL;
				delete aux;
			}
			
			else{
				
				aux = primeiro;
				noh* antecessor = NULL;
				int posAux = 0;
				
				while(posAux < posicao){
					
					antecessor = aux;
					aux = aux->proximo;
					posAux++;
				}
				
				antecessor->proximo = aux->proximo;
				aux->proximo->anterior = antecessor;
				delete aux;
			}
			
			tamanho--;
		}
	}
}

int lista::procurar(int chave){
	
	noh* aux = primeiro;
	int posAux = 0;
	
	while((aux != NULL) and (aux->chave != chave)){
		
		posAux++;
		aux = aux->proximo;
	}
	
	if(aux == NULL)
		posAux = -1;
	
	return posAux;
}

class tabelaHash{
	
	private:
		int capacidade;
		int tamanho;
		lista* tabela;
		
	public:
		tabelaHash(int cap);
		~tabelaHash();
		int calcularHash(int chave);
		void inserirElemento(string escrito, int chave);
		string recuperarValor(int chave);
		void removerElemento(int chave);
		void redimensionar(int novaCapacidade);
		void depuracao();
};

tabelaHash::tabelaHash(int cap){
	
	capacidade = cap;
	tamanho = 0;
	tabela = new lista[cap];
}

tabelaHash::~tabelaHash(){
	
	delete[] tabela;
}

int tabelaHash::calcularHash(int chave){
	
	return (chave % capacidade);
}

void tabelaHash::inserirElemento(string escrito, int chave){
		
	int posicao = calcularHash(chave);
	lista *list = &tabela[posicao];
	
	if(list->procurar(chave) == -1){
		list->inserir(escrito, chave);
		
		tamanho++;
	}
	
	if(tamanho/capacidade >= 0.7) {
		redimensionar(capacidade*2);
	}
	
}

string tabelaHash::recuperarValor(int chave){
	
	int posicao = calcularHash(chave);
	int posAux = tabela[posicao].procurar(chave);
	noh* aux;
	
	if(posAux == -1)
		return "";
	
	else{
		
		aux = tabela[posicao].primeiro;
		
		for(int i = 0; i < posAux; i++)
			aux = aux->proximo;
			
		return aux->escrito;
	}
}

void tabelaHash::removerElemento(int chave){
	
	if(tamanho != 0) {
		
		int posicao = calcularHash(chave);
		int posAux = tabela[posicao].procurar(chave);
		lista *list = &tabela[posicao];
		
		if(posAux == -1)
			cout << "ERRO" << endl;
			
		else{
			
			list->removerNaPosicao(posAux);
			
			tamanho--;
		}
	}
}

void tabelaHash::redimensionar(int novaCapacidade){
	

	if(not (novaCapacidade <= capacidade)) {
		
		lista* tabelaAux = new lista[novaCapacidade];
		noh* aux;
		
		int novaPos;
		
		for(int i = 0; i < capacidade; i++){
			
			if(tabela[i].primeiro != NULL){
				
				aux = tabela[i].primeiro;
				
				while(aux != NULL){
					
					novaPos = aux->chave % novaCapacidade;
					
					tabelaAux[novaPos].inserir(aux->escrito, aux->chave);
					
					aux = aux->proximo;
				}
			}
		}
		
		capacidade = novaCapacidade;
		
		delete[] tabela;
		
		tabela = tabelaAux;
	}

}

void tabelaHash::depuracao(){
	
	for(int i = 0; i < capacidade; i++){
		
		cout << "Posicao " << i << ": ";
		tabela[i].imprimir();
	}
	cout << "-----------------------" << endl;
}

int main(){
	
	int capacidade;
	cin >> capacidade;
	
	tabelaHash hash(capacidade);
	int numero;
	string escrito;
	
	char opcao;
	
	do{
		
		cin >> opcao;
		
		switch(opcao){
			
			case 'I':
				cin >> numero >> escrito;
				hash.inserirElemento(escrito, numero);
				break;
				
			case 'R':
				cin >> numero;
				hash.removerElemento(numero);
				break;
				
			case 'B':
				cin >> numero;
				hash.recuperarValor(numero);
				break;
				
			case 'P':
				hash.depuracao();
				break;
		}
		
	} while(opcao != 'S');
	
	return 0;
}
