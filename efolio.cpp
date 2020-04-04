#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <exception>

using namespace std;

struct ERRLIST: public exception{
    const char* what() const throw(){
        return "Lista vazia!";
    }

};

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
        NO_LISTA<T>(T valor){
            this->elemento=valor;
            proximo = nullptr;
        }

        NO_LISTA<T>(NO_LISTA<T> & no){
            this->elemento=no->obterElemento();
            this->proximo=no->obterProximo();
        }

        ~NO_LISTA(){
            this->elemento=0;
            proximo=nullptr;
        }

        //Obter referencia do seguinte elemento da lista
        NO_LISTA<T>* obterProximo() const{
            return proximo;
        }

        //Obter Elemento
        int obterElemento() const{
            return elemento;
        }

        //Atualiza elemento seguinte
        void atualizaProximo(NO_LISTA<T> * no){
            proximo=no;
        }

        //Atualiza elemento
        void atualizaElemento(T valor){
            elemento=valor;
        }

        bool operator==(const NO_LISTA<T> * no)const{
            return (this == no);
        }
};


//Classe que armazena a lista
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
                int aux = temp->obterElemento();
                temp = temp->obterProximo();
                while(temp!=cauda->obterProximo()){
                    if(aux < temp->obterElemento())
                        aux= temp->obterElemento();
                    temp = temp->obterProximo();
                }
                maximo= aux;
            }
        }

        //Procura no anterior à posicao fornecida
        NO_LISTA<int> * procuraAnteriorPosicao(int pos){
            if(pos>=tamanho || pos<0 || cauda == nullptr){
                return nullptr;
            }
            if(pos==0)
                return cauda;
            else{
                NO_LISTA<int> * temp = cauda;
                for(int i =0;i<pos;i++)
                    temp = temp->obterProximo();
                return temp;
            }
        }

    public:

        LISTA(){
            cauda = nullptr;
            tamanho = 0;
            maximo = INT16_MIN;
        }

        ~LISTA(){
            cauda=nullptr;
            tamanho=0;
            maximo =INT16_MIN;
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

        //Inserir Depois do No recebido e atualiza quantidade de elementos
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

        //Remover no topo da Lista e atualiza quantidade de elementos
        bool removerCabeca(){
            //Verifica se lista está vazia
            if(cauda == nullptr){
                return false;
            }
            //Se tiver membros
            else{
                NO_LISTA<int> * temp = cauda->obterProximo();
                cauda->atualizaProximo(temp->obterProximo());
                decrementaLista();
                if(temp->obterElemento()==maximo && tamanho>0){
                    atualizarMaximo();
                }
                delete(temp);
                return true;
            }
        }

        //Remover no fim da Lista e atualiza quantidade de elementos
        bool removerCauda(){
            //Verifica se lista está vazia
            if(cauda == nullptr){
                return false;
            }
            //Se tiver membros
            else{
                NO_LISTA<int> * temp = cauda->obterProximo();
                NO_LISTA<int> * aux = cauda;
                while(temp->obterProximo()!=cauda)
                    temp = temp->obterProximo();
                temp->atualizaProximo(cauda->obterProximo());
                cauda = temp;
                decrementaLista();
                if(aux->obterElemento()==maximo){
                    atualizarMaximo();
                }
                delete(aux);
                return true;
            }
        }

        //Remover Elementos da Lista e atualiza quantidade de elementos
        bool remover(int pos){
            if(pos>=tamanho || pos <0 || cauda == nullptr){
                return false; //Se posição inválida ou lista vazia retorna false
            }
            if(pos == 0){
                return removerCabeca();
            } else if(pos == tamanho-1){
                return removerCauda();
            }
            //Apagar no na posição recebida
            else{
                NO_LISTA<int> * aux = procuraAnteriorPosicao(pos);
                NO_LISTA<int> * temp= aux->obterProximo();
                aux->atualizaProximo(temp->obterProximo());
                decrementaLista();
                if(temp->obterElemento()==maximo){
                    atualizarMaximo();
                }
                delete(temp);
                return true;
            }
        }

        //Obter o elemento Final da Lista
        NO_LISTA<int> * obterCauda() const{
            return cauda;
        }

        //Obter o elemento inicial da Lista
        NO_LISTA<int> * obterCabeca() const{
            return cauda->obterProximo();
        }

        //Obter Tamanho da Lista
        const int obterTamanho() const{
            return tamanho;
        }

        //Obter Maximo da Lista
        const int obterMaximo() const{
            try{
                if(tamanho==0)
                    throw ERRLIST();
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
        }

        //Retorna Posição do Elemento com o Valor Maximo
        //Se retornar -1 não encontrou elemento
        const int procuraItem(){
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
        }

        //Gerar uma versão textual dos elementos da lista
        const string listaCompleta(){
            string texto;
            if(tamanho>0){
                NO_LISTA<int> * temp = obterCabeca();
                for(int i=0; i<tamanho; i++){
                    texto.append(" ");
                    texto.append(to_string(temp->obterElemento()));
                    temp = temp->obterProximo();
                }
            }
            return texto;
        }

        bool inverter(int pos1, int pos2){
            try{
                if(tamanho==0){
                    throw ERRLIST();
                }
                if(pos1<0 || pos1 >=tamanho){
                    throw ERRARG();
                }
                if(pos2<0 || pos2 >=tamanho){
                    throw ERRARG();
                }
                if(pos1 > pos2){
                    int aux=pos1;
                    pos1=pos2;
                    pos2=aux;
                }
                if(tamanho>0){
                    LISTA * temp = new LISTA();
                    for(int i= pos1; i <= pos2; i++){
                        NO_LISTA<int> * noTemp = procuraAnteriorPosicao(pos1);
                        NO_LISTA<int> * aux= noTemp->obterProximo();
                        temp->inserirCabeca(aux->obterElemento());
                        this->remover(pos1);
                    }
                    for(int i= 0; i <= pos2-pos1; i++){
                        NO_LISTA<int> * noTemp = temp->procuraAnteriorPosicao(0);
                        NO_LISTA<int> * aux= noTemp->obterProximo();
                        this->inserirPosicao(aux->obterElemento(),pos1+i);
                        temp->removerCabeca();
                    }
                    delete(temp);
                    return true;
                }
                return false;
            } catch(ERRARG &e){
                throw e;
            } catch(ERRLIST &e){
                throw e;
            }
        }

};

//Classe Interface de Gestão da Lista
class GESTOR_LISTA{
    private:
        LISTA * inteiros;

    public:

        GESTOR_LISTA(){
            inteiros = new LISTA();
        }

        ~GESTOR_LISTA(){
            apagarLista();
            delete(inteiros);
            inteiros = nullptr;
        }
    
        //Inserir elemento no inicio
        bool inserirNoInicio(int valor){
            return inteiros->inserirCabeca(valor);
        }

        //Inserir elemento no final
        bool inserirNoFinal(int valor){
            return inteiros->inserirCauda(valor);
        }

        //Remover elemento no final
        const bool removerNoFim(){
            try{
                if(inteiros->listaVazia()){
                    throw ERRLIST();
                }
                return inteiros->removerCauda();
            } catch(ERRLIST& e){
                throw e;
            }
        }

        //Remover elemento no inicio
        const bool removerNoInicio(){
            try{
                if(inteiros->listaVazia()){
                    throw ERRLIST();
                }
                return inteiros->removerCabeca();
            } catch(ERRLIST& e){
                throw e;
            }
        }

        //Remover elemento na posicao
        const bool removerNaPosicao(int pos){
            try{
                if(inteiros->listaVazia()){
                    throw ERRLIST();
                }
                int tamanho=inteiros->obterTamanho();
                if(tamanho < pos){
                    throw ERRARG();
                }
                if(pos == 0){
                    return inteiros->removerCabeca();
                } else if(pos == tamanho-1){
                    return inteiros->removerCauda();
                } else if(pos<inteiros->obterTamanho()){
                    return inteiros->remover(pos);
                }
                return false;
            } catch(ERRLIST& e){
                throw e;
            } catch(ERRARG& e){
                throw e;
            }
        }


        //Apaga a lista toda
        bool apagarLista(){
            try{
                if(inteiros->listaVazia()){
                    throw ERRLIST();
                }
                while(!inteiros->listaVazia()){
                    inteiros->removerCauda();
                }
                return true;
            } catch(ERRLIST& e){
                throw e;
            }
        }

        //Obter Tamanho da Lista
        const int obterTamanho() const{
            return inteiros->obterTamanho();
        }

        //Obter Maximo da Lista
        const int obterMaximo() const{
            try{
                return inteiros->obterMaximo();
            } catch(ERRLIST & e){
                throw e;
            }
        }

        //Retorna o Valor do Primeiro Item da Lista
        const int obterElementoCabeca() const{
            try{
                if(!inteiros->listaVazia())
                    return inteiros->obterCabeca()->obterElemento();
                throw ERRLIST();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna o Valor do Ultimo Item da Lista
        const int obterElementoCauda() const{
            try{
                if(!inteiros->listaVazia())
                    return inteiros->obterCauda()->obterElemento();
                throw ERRLIST();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna Primeira Ocorrencia do elemento igual a "valor"
        //Se retornar -1 não encontrou elemento
        const int procuraPrimeiraOcorrencia(int valor){
            try{
                if(inteiros->listaVazia())
                    throw ERRLIST();
                return inteiros->procuraItem(valor);
            }catch(ERRLIST& e){
                throw e;
            }
        }

        //Retorna Primeira Ocorrencia do elemento igual a maximo
        const int procuraElementoMaximo(){
            try{
                if(inteiros->listaVazia())
                    throw ERRLIST();
                return inteiros->procuraItem();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        string obterListaTextual(){
            try{
                if(!inteiros->listaVazia())
                    return inteiros->listaCompleta();
                throw ERRLIST();
            }catch(ERRLIST& e){
                throw e;
            }
        }

        bool inverter(int pos1,int pos2){
            try{
                if(pos1 > pos2){
                    return inteiros->inverter(pos2,pos1);
                }
                return inteiros->inverter(pos1,pos2);
            } catch(ERRARG &e){
                throw e;
            } catch(ERRLIST &e){
                throw e;
            }
        }
};

class INTERFACE{
    private:
        GESTOR_LISTA* lista;
        string comando;
        string argumentos;

        //Executar Comando Inserir no Inicio
        void insert_0(){
            int aux;
            bool res;
            if(argumentos != " "){
                istringstream arg(argumentos);
                while(arg >> aux){
                    res = lista->inserirNoInicio(aux);
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
                    res = lista->inserirNoFinal(aux);
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
            }catch(ERRLIST e){ //Caso a lista esteja vazia
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
            }catch(ERRLIST e){ //Caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando Para imprimir os elementos da Lista
        void print(){
            try{
                int aux;
                string res;
                res = lista->obterListaTextual();
                cout << "Lista=" << res << endl;
            }catch(ERRLIST e){ //Caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para apagar o primeiro elemento da lista
        void delete_0(){
            try{
                int aux;
                bool res;
                res = lista->removerNoInicio();
            }catch(ERRLIST e){ //Caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para apagar o ultimo elemento da lista
        void delete_end(){
            try{
                int aux;
                bool res;
                res = lista->removerNoFim();
            }catch(ERRLIST e){ //Caso a lista esteja vazia
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
                int aux;
                bool res;
                res = lista->apagarLista();
            }catch(ERRLIST e){ //Caso a lista esteja vazia
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
                        res = lista->procuraPrimeiraOcorrencia(aux);
                        if(res==-1){ //Caso não encontre o item
                            cout << "Item " << aux << " nao encontrado!" <<endl;
                        }
                        else{ //Caso seja encontrado o item
                            cout << "Item " << aux << " na posicao " << res <<endl;
                        }
                    }
                }                 
            }catch(ERRLIST e){ //Caso a lista esteja vazia
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

        //Executa comando para procurar o elemento maximo da lista
        void find_max(){
            try{
                int aux;
                int res = lista->procuraElementoMaximo();
                cout << "Max Item " << lista->obterMaximo() << " na posicao " << res <<endl;                
            }catch(ERRLIST e){ //Caso a lista esteja vazia
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
                        res = lista->removerNaPosicao(aux);
                    } 
                } else{
                    cout << "Comando " << comando<< ": Posicao invalida!"<<endl;
                }
            } catch(ERRARG & e){
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            } catch(ERRLIST & e){
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

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
            } catch(ERRARG & e){
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            } catch(ERRLIST & e){
                cout << "Comando " << comando<< ": " << e.what() <<endl;
            }
        }

    public:
        INTERFACE(){
            lista = new GESTOR_LISTA();
            this->comando ="";
            this->argumentos="";
        }

        ~INTERFACE(){
            delete(lista);
            lista=nullptr;
            this->comando ="";
            this->argumentos="";
        }

        void inserirComando(string comando){
            this->comando = comando;
        }

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
                cout << "Comando não suportado!" <<endl;
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

