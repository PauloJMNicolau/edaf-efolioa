#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;
//Estrutura de Erros para lista vazia
struct ERRLIST: public exception{
    const char* what() const throw(){
        return "Lista vazia!";
    }

};

//Estrutura de Erros para posições inválidas
struct ERRARG: public exception{
    virtual const char* what() const throw(){
        return "Posicao invalida!";
    }
};

//Classe que representa cada no da lista
template <class T>
class NO_LISTA{
    private:
        int elemento; //Valor da Lista
        NO_LISTA<T> *proximo; //Elemento Seguinte na Lista

    public:
        //Construtor de Nó da Lista
        NO_LISTA<T>(T valor){
            this->elemento=valor; 
            proximo = nullptr;
        }

        //Construtor por Copia de um Nó da Lista
        NO_LISTA<T>(NO_LISTA<T> & no){
            this->elemento=no->obterElemento();
            this->proximo=no->obterProximo();
        }

        //Destroi um o nó
        ~NO_LISTA(){
            this->elemento=0;
            proximo=nullptr;
        }

        //Obter elemento seguinte do nó
        NO_LISTA<T>* obterProximo() const{
            return proximo;
        }

        //Obter Elemento do nó
        const int obterElemento() const{
            return elemento;
        }

        //Atualiza elemento seguinte com o nó fornecido
        void atualizaProximo(NO_LISTA<T> * no){
            proximo=no;
        }

        //Atualiza elemento do nó
        void atualizaElemento(T valor){
            elemento=valor;
        }

        //Comparar nó com outro nó
        bool operator==(const NO_LISTA<T> * no)const{
            return (this == no);
        }
};


//Classe que armazena a lista
//Disponibiliza as funções base da lista (inserir, remover, procurar, inverter, obter elementos)
class LISTA{
    private:
        NO_LISTA<int> * cauda; //Ultimo Elemento da Lista
        int tamanho; //Tamanho da Lista
        int maximo; //Maior Valor na lista

        //Incrementa Tamanho da Lista
        void incrementaLista(){
            tamanho++;
        }

        //Decrementa Tamanho da Lista
        void decrementaLista(){
            if(tamanho>0){
                tamanho--;
                if(tamanho==0){
                    cauda=nullptr;
                    maximo=INT16_MIN; //Atualiza para o minimo inteiro de 16bits
                }
            }
        }

        //Atualiza valor máximo
        void atualizarMaximo(){
            if(tamanho>0){
                NO_LISTA<int> * temp = cauda->obterProximo();
                int aux = temp->obterElemento(); //Coloca a variável com o valor do primeiro elemento da lista
                //Procura elemento maior da lista
                //Percorrendo a lista até ao ultimo elemento
                temp = temp->obterProximo();
                while(temp!=cauda->obterProximo()){
                    if(aux < temp->obterElemento())
                        aux= temp->obterElemento(); //Atualiza variável auxiliar caso o elemento atual seja maior do que o guardado na variável
                    temp = temp->obterProximo();
                }
                maximo= aux; //Atualiza o valor máximo na lista
            }
        }

        //Procura nó anterior à posicao fornecida
        NO_LISTA<int> * procuraAnteriorPosicao(int pos){
            if(pos>=tamanho || pos<0 || cauda == nullptr){
                return nullptr; // Se estiver vazio retorna NULL
            }
            if(pos==0)
                return cauda; //Se for a Posição 0 retorna o nó da cauda
            else{
                //Percorre a lista até à posição anterior à fornecida e retorna esse nó
                NO_LISTA<int> * temp = cauda;
                for(int i =0;i<pos;i++)
                    temp = temp->obterProximo();
                return temp;
            }
        }

        //Remover no topo da Lista e atualiza quantidade de elementos
        bool removerCabeca(){
            try{
                if(tamanho==0){
                    throw ERRLIST(); //Erro Lista Vazia
                }
                NO_LISTA<int> * temp = cauda->obterProximo();
                cauda->atualizaProximo(temp->obterProximo());
                decrementaLista();
                //Verifica se valor removido era o valor máximo
                //Caso seja atualiza o valor máximo
                if(temp->obterElemento()==maximo && tamanho>0){
                    atualizarMaximo();
                }
                delete(temp);
                return true;
            } catch(ERRLIST& e){
                throw e;
            }
        }

        //Remover no fim da Lista e atualiza quantidade de elementos
        bool removerCauda(){
            try{
                if(tamanho==0){
                    throw ERRLIST(); //Erro de lista vazia
                }
                //Obtem elemento anteiro da lista
                NO_LISTA<int> * temp = procuraAnteriorPosicao(tamanho-1);
                NO_LISTA<int> * aux = cauda;
                //Atualiza nó obtido com o primeiro da lista  
                temp->atualizaProximo(cauda->obterProximo());
                cauda = temp; // Atualiza cauda e reduz tamanho da lista
                decrementaLista();
                //Verifica se valor removido era o valor máximo
                //Caso seja atualiza o valor máximo
                if(aux->obterElemento()==maximo){
                    atualizarMaximo();
                }
                delete(aux);
                return true;
            } catch(ERRLIST& e){
                throw e;
            }
        }

         //Inserir elemento no topo da Lista e atualiza quantidade de elementos
        bool inserirCabeca(int valor){
            NO_LISTA<int> * novo = new NO_LISTA<int>(valor); //Cria novo No com o valor recebido
            //Se Lista Vazia
            if(cauda==nullptr){
                novo->atualizaProximo(novo);
                cauda=novo;
                maximo= valor;//Atualiza valor Maximo da lista
                incrementaLista();
                return true;
            }
            //Se não estiver Vazia
            else{
                novo->atualizaProximo(cauda->obterProximo());
                cauda->atualizaProximo(novo);
                if(valor>maximo)
                    maximo=valor;//Atualiza valor Maximo da lista
                incrementaLista();
                return true;
            }
        }

        //Inserir elemento no fundo da Lista e atualiza quantidade de elementos
        bool inserirCauda(int valor){
            NO_LISTA<int> * novo = new NO_LISTA<int>(valor); //Cria novo No com o valor recebido
            //Se Lista Vazia
            if(cauda==nullptr){
                novo->atualizaProximo(novo);
                cauda=novo;
                maximo= valor;//Atualiza valor Maximo da lista
                incrementaLista();
                return true;
            }
            //Se não estiver Vazia
            else{
                novo->atualizaProximo(cauda->obterProximo());
                cauda->atualizaProximo(novo);
                incrementaLista();
                if(valor>maximo)
                    maximo=valor;//Atualiza valor Maximo da lista
                cauda=novo;
                return true;
            }
        }

    public:
        //Construtor de Lista vazia
        LISTA(){
            cauda = nullptr;
            tamanho = 0;
            maximo = INT16_MIN;//Coloca o valor máximo com o valor minimo que o inteiro de 16bit suporta
        }

        //Destroi Lista
        ~LISTA(){
            if(tamanho!=0){
                while(!listaVazia()){
                    removerCauda();
                }
            }
        }

        //Inserir elemento na posição recebida e atualiza quantidade de elementos
        bool inserirPosicao(int valor, int pos){
            NO_LISTA<int> * novo = new NO_LISTA<int>(valor); //Cria novo No com o valor recebido
            //Se Lista Vazia
            if(cauda==nullptr){
                novo->atualizaProximo(novo);
                cauda=novo;
                maximo=valor;//Atualiza valor Maximo da lista
                incrementaLista();
                return true;
            }
            // Verifica se a posição fornecida é a primeira ou a ultima
            //Caso seja executa a função respetiva
            if(pos == 0){
                return inserirCabeca(valor);
            } else if(pos >= tamanho-1){
                return inserirCauda(valor);
            }
            //Se não estiver Vazio
            else{
                NO_LISTA<int> * temp=procuraAnteriorPosicao(pos);
                novo->atualizaProximo(temp->obterProximo());
                temp->atualizaProximo(novo);
                incrementaLista();
                if(valor>maximo)
                    maximo=valor;//Atualiza valor Maximo da lista
                return true;
            }
        }

        //Remover Elementos da Lista e atualiza quantidade de elementos
        bool remover(int pos){
             try{
                if(tamanho==0){
                    throw ERRLIST(); //Erro lista vazia
                }
                if(tamanho < pos || pos <0){
                    throw ERRARG(); //Erro de posição inválida
                }
                // Verifica se a posição fornecida é a primeira ou a ultima
                //Caso seja executa a função respetiva
                if(pos == 0){
                    return removerCabeca();
                } else if(pos == tamanho-1){
                    return removerCauda();
                }
                //Apagar no na posição recebida
                NO_LISTA<int> * aux = procuraAnteriorPosicao(pos);
                NO_LISTA<int> * temp= aux->obterProximo();
                aux->atualizaProximo(temp->obterProximo());
                decrementaLista();
                if(temp->obterElemento()==maximo){
                    atualizarMaximo();
                }
                delete(temp);
                return true;

            } catch(ERRLIST& e){
                throw e;
            } catch(ERRARG& e){
                throw e;
            }
        }

        //Obter o elemento Final da Lista
        NO_LISTA<int> * obterCauda() const{
            return cauda;
        }

        //Obter o elemento inicial da Lista
        NO_LISTA<int> * obterCabeca() const{
            try{
                if(tamanho==0)
                    throw ERRLIST();
                return cauda->obterProximo();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Obter Tamanho da Lista
        const int obterTamanho() const{
            return tamanho;
        }

        //Obter Maximo da Lista
        const int obterMaximo() const{
            try{
                if(tamanho==0)
                    throw ERRLIST(); //Lista Vazia
                return maximo;
            } catch(ERRLIST & e){
                throw e;
            }           
        }

        //Verifica se lista está vazia
        const bool listaVazia(){
            return tamanho==0;
        }

        //Retorna a posição do primeiro elemento igual ao valor fornecido
        //Se retornar -1 não encontrou elemento
        const int procuraItem(int valor){
            try{
                if(tamanho==0)
                    throw ERRLIST(); //Erro lista Vazia
                int posicao= -1;
                int i=0;
                NO_LISTA<int> * temp = cauda->obterProximo();
                while(posicao ==-1 && i< tamanho){
                    if(temp->obterElemento()==valor)
                        posicao=i;
                    temp = temp->obterProximo();
                    i++;
                }
                return posicao;
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna Posição do Elemento com o Valor Maximo
        //Se retornar -1 lista está vazia
        const int procuraItem(){
            try{
                if(tamanho==0)
                    throw ERRLIST(); //Erro lista vazia
                int posicao= -1;     
                int i=0;
                NO_LISTA<int> * temp = cauda->obterProximo();
                while(posicao ==-1 && i< tamanho){
                    if(temp->obterElemento()==maximo)
                        posicao=i;
                    temp = temp->obterProximo();
                    i++;
                }
                return posicao;
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Gerar uma versão textual dos elementos da lista
        const string listaCompleta(){
            try{
                if(tamanho==0)
                    throw ERRLIST();
                string texto;
                NO_LISTA<int> * temp = obterCabeca();
                for(int i=0; i<tamanho; i++){
                    texto.append(" ");
                    texto.append(to_string(temp->obterElemento()));
                    temp = temp->obterProximo();
                }
                return texto;
            }catch(ERRLIST& e){
                throw e;
            }            
        }

        //Inverter elementos da lista entre as posições fornecidas
        bool inverter(int pos1, int pos2){
            try{
                if(tamanho==0){
                    throw ERRLIST(); //Lista Vazia
                }
                if(pos1<0 || pos1 >=tamanho){
                    throw ERRARG(); //Posição 1 Inválida
                }
                if(pos2<0 || pos2 >=tamanho){
                    throw ERRARG(); //Posição 2 Inválida
                }
                //Verifica se posições estão trocadas
                //Caso esteja troca
                if(pos1 > pos2){
                    int aux=pos1;
                    pos1=pos2;
                    pos2=aux;
                }
                if(tamanho>0){
                    LISTA * temp = new LISTA();
                    //Cria uma lista temporária com os elementos que se pretende trocar
                    //Procura o elemento das posições pedidas e insere na lista temporária
                    for(int i= pos1; i <= pos2; i++){
                        NO_LISTA<int> * noTemp = procuraAnteriorPosicao(pos1);
                        NO_LISTA<int> * aux= noTemp->obterProximo();
                        //Insere os elementos sempre na cabeça.
                        //Desta forma os elementos já ficam trocados
                        temp->inserirCabeca(aux->obterElemento());  
                        this->remover(pos1); //Remove o elemento da lista principal
                    }
                    //Copia novamente os elementos já invertidos para a lista principal
                    for(int i= 0; i <= pos2-pos1; i++){
                        //Retira sempre da lista temporária o nó da cabeça
                        //Insere o elemento na lista principal na posição1 incrementada com o valor de i
                        this->inserirPosicao(temp->obterCabeca()->obterElemento(),pos1+i);
                        temp->removerCabeca();
                    }
                    delete(temp); //Apaga a lista temporária
                    return true;
                }
                return false;
            } catch(ERRARG &e){
                throw e;
            } catch(ERRLIST &e){
                throw e;
            }
        }

        //Apaga a lista toda
        bool apagarLista(){
            try{
                if(tamanho==0){
                    throw ERRLIST(); //Erro Lista Vazia
                }
                while(!listaVazia()){
                    removerCauda();
                }
                return true;
            } catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna o Valor do Primeiro Item da Lista
        const int obterElementoCabeca() const{
            try{
                if(tamanho==0)
                    throw ERRLIST(); //Erro lista Vazia
                return cauda->obterProximo()->obterElemento();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna o Valor do Ultimo Item da Lista
        const int obterElementoCauda() const{
            try{
                if(tamanho==0)
                    throw ERRLIST();
                return obterCauda()->obterElemento();
            }catch(ERRLIST& e){
                throw e;
            }
        }

};

class INTERFACE{
    private:
        LISTA* lista;
        string comando;
        string argumentos;

        //Executar Comando Inserir no Inicio
        void insert_0(){
            int aux;
            bool res;
            if(argumentos != " "){
                istringstream arg(argumentos);
                while(arg >> aux){
                    res = lista->inserirPosicao(aux,0);
                } 
            } else{
                cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
            }
            
        }

        //Executar Comando Inserir no Inicio
        void insert_end(){
            int aux;
            int res;
            if(argumentos != " "){
                istringstream arg(argumentos);
                while(arg >> aux){
                    res = lista->inserirPosicao(aux, lista->obterTamanho());
                } 
            } else{
                cout << "Comando " << comando<< ": Não existem valores a introduzir!"<<endl;
            }
        }

        //Executa comando Para imprimir Primeiro elemento da Lista
        void print_0(){
            try{
                int aux;
                int res;
                res = lista->obterElementoCabeca();
                cout << "Lista(0)= " << res << endl;
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando Para imprimir Ultimo elemento da Lista
        void print_end(){
            try{
                int aux;
                int res;
                res = lista->obterElementoCauda();
                cout << "Lista(end)= " << res << endl;
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando Para imprimir os elementos da Lista
        void print(){
            try{
                int aux;
                string res;
                res = lista->listaCompleta();
                cout << "Lista=" << res << endl;
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para apagar o primeiro elemento da lista
        void delete_0(){
            try{
                bool res = lista->remover(0);
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para apagar o ultimo elemento da lista
        void delete_end(){
            try{
                bool res = lista->remover(lista->obterTamanho()-1);
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando que mostra tamanho da lista
        void dim(){
            cout << "Lista tem " << lista->obterTamanho() << " itens" <<endl;
        }

        //Executa comando para apagar toda a lista
        void clear(){
            try{
                bool res = lista->apagarLista();
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para procurar o elemento da lista especifico
        void find(){
            try{
                int aux;
                int res;
                if(argumentos != " "){
                    istringstream arg(argumentos);
                    while(arg >> aux){
                        res = lista->procuraItem(aux);
                        if(res==-1){ //Caso não encontre o item
                            cout << "Item " << aux << " nao encontrado!" <<endl;
                        }
                        else{ //Caso seja encontrado o item
                            cout << "Item " << aux << " na posicao " << res <<endl;
                        }
                    }
                }                 
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para procurar o elemento maximo da lista
        void find_max(){
            try{
                int res = lista->procuraItem();
                cout << "Max Item " << lista->obterMaximo() << " na posicao " << res <<endl;                
            }catch(ERRLIST e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando que apaga elemento na posição solicitada
        void delete_pos(){
            try{
                int aux;
                bool res;
                if(argumentos != " "){
                    istringstream arg(argumentos);
                    while(arg >> aux){
                        res = lista->remover(aux);
                    } 
                } else{
                    cout << "Comando " << comando<< ": Posicao invalida!"<<endl;
                }
            } catch(ERRARG & e){ //Trata o erro caso as posições sejam invalidas
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            } catch(ERRLIST & e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa o comando que inverte os elementos entre as posições recebidas
        void invert_range(){
            try{
                int pos1, pos2;
                bool res;
                if(argumentos != " "){
                    istringstream arg(argumentos);
                    arg >> pos1;
                    arg >> pos2;
                    res = lista->inverter(pos1,pos2);
                } else{
                    cout << "Comando " << comando<< ": Posicao invalida!"<<endl;
                }
            } catch(ERRARG & e){ //Trata o erro caso as posições sejam inválidas
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            } catch(ERRLIST & e){ //Trata o erro caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

    public:
        //Constroi a interface
        INTERFACE(){
            lista = new LISTA();
            this->comando ="";
            this->argumentos="";
        }

        //Destroi a interface
        ~INTERFACE(){
            delete(lista);
            lista=nullptr;
            this->comando ="";
            this->argumentos="";
        }

        //Atualiza o valor de comando
        void inserirComando(string comando){
            this->comando = comando;
        }

        //Atualiza o valor de argumentos
        void inserirArgumentos(string argumentos){
            this->argumentos=argumentos;
        }

        //Executa o comando respectivo
        void executaComando(){
            if(comando == "insert_0"){
                insert_0();
            } else if(comando == "insert_end"){
                insert_end();
            } else if(comando == "print_0"){
                print_0();
            } else if(comando == "print_end"){
                print_end();
            } else if(comando == "print"){
                print();
            } else if(comando == "delete_0"){
                delete_0();
            } else if(comando == "delete_end"){
                delete_end();
            } else if(comando == "dim"){
                dim();
            } else if(comando == "clear"){
                clear();
            } else if(comando == "find"){
                find();
            } else if(comando == "find_max"){
                find_max();
            } else if(comando == "delete_pos"){
                delete_pos();
            } else if(comando == "invert_range"){
                invert_range();
            } else{
                cout << "Comando não suportado!" <<endl; // Caso o comando não seja válido
            }
        }
};

int main(){
    /* Enter your code here. Read input from CIN or STDIN. Print output to COUT or STDOUT */


    fstream file;
    string linha;
    file.open("cmd.txt", ios::in);
    INTERFACE *consola = new INTERFACE();

    /*//Código HR

    while(getline(cin,linha)){
        if(linha.substr(0,1)== "#" || linha ==""){
            continue;
        }
        int pos = linha.find_first_of(" ", 0);
        consola->inserirComando(linha.substr(0,pos));
        pos = linha.find_first_not_of(" ",pos);
        if(pos!=-1){
            consola->inserirArgumentos(linha.substr(pos,linha.size()));
        }
        consola->executaComando();
    }

    //Fim Codigo HR */

    
    //Código IDE
    while(getline(file,linha)){
        if(linha.substr(0,1)== "#" || linha ==""){
            continue;
        }
        int pos = linha.find_first_of(" ", 0);
        consola->inserirComando(linha.substr(0,pos));
        pos = linha.find_first_not_of(" ",pos);
        if(pos!=-1){
            consola->inserirArgumentos(linha.substr(pos,linha.size()));
        }
        consola->executaComando();
    }
    file.close();

    //Fim Cóódigo IDE*/
    return 0;
}

