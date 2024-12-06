// Gustavo Figueiredo de Oliveira
// As outras duas integrantes do grupo cancelaram a materia           
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//Struct para armazenar as classes(informacoes) dos livros
struct BCLV {

    int Identificador;
    string Nome;
    string NomeAutor;
    string Editora;
    string Genero;
    int NumeroPaginas;

};

//funcao particiona identificador
int particionaID(BCLV *vetor, int inicio, int final){

    int pivo = vetor[inicio].Identificador;
    int i = (inicio + 1);
    int j = final;

    while(i <= j){
        while(i <= j && vetor[i].Identificador <= pivo){
            i++;
        }
        while(i <= j && vetor[j].Identificador >= pivo){
            j--;
        }
        if(i < j){
            swap(vetor[i], vetor[j]);
        }
    }
    swap(vetor[inicio], vetor[j]);
    return j;
}

//funcao quicksort do identificador do livro
void quickSortID(BCLV *vetor, int inicio, int final){

    if(inicio < final){
        int pivoPos = particionaID(vetor, inicio, final);
        quickSortID(vetor, inicio, pivoPos - 1);
        quickSortID(vetor, pivoPos + 1, final);
    }
}

//funcao particiona nome do livro
int particionaNome(BCLV *vetor, int inicio, int final){
   
    string pivo = vetor[inicio].Nome;
    int i = inicio + 1;
    int j = final;

    while(i <= j){
        while(i <= j && vetor[i].Nome <= pivo){
            i++;
        }
        while(i <= j && vetor[j].Nome >= pivo){
            j--;
        }
        if(i < j){
            swap(vetor[i], vetor[j]);
        }
    }
   
    swap(vetor[inicio], vetor[j]);
    return j;
}

//funcao quicksort do nome do livro
void quickSortNome(BCLV *vetor, int inicio, int final){

    if(inicio < final){
        int pivoPos = particionaNome(vetor, inicio, final);
        quickSortNome(vetor, inicio, pivoPos - 1);
        quickSortNome(vetor, pivoPos + 1, final);
    }
}

//funcao para finalizar a navegacao
void finalizarPrograma(){

    int continuar;
    cout << "Certo! Digite 1 para fechar o programa" << endl;
    cin >> continuar;
    cout << endl;
           
    switch(continuar){

        case 1:
           
            //clear para linux e cls para windows
            system("cls || clear");

            cout << "Programa Finalizado!";
            cout << endl;
            break;
       
        default:
       
            cout << "Opcao invalida. digite 1 para ter exito!" << endl;
            cout << endl;
            break;
    }
}

//Função para escrever uma string como vetor de char
void StringParaChar(ofstream &arqBin, string &str) {

    char vetorChar[50];
    int a = 0;

    while(a < (int)str.size()) {
        vetorChar[a] = str[a];
        a++;
    }
    while(a < 50){
        vetorChar[a] = '|';
        a++;
    }
   
    arqBin.write(reinterpret_cast<const char*>(&vetorChar), sizeof(char[50]));
}

//funcao para transformar o vetor de char em string para imprimir
string CharParaString(ifstream &arqEntradaBin){

    char vetorChar[50];
    string aux = "";
    arqEntradaBin.read(reinterpret_cast<char *>(&vetorChar), sizeof(char[50]));

    for (int j = 0; j < 50; j++){
        if (vetorChar[j] != '|')
        {
            aux += vetorChar[j];
        }
    }
    return aux;
}

//Função para salvar os dados em um arquivo binário
void salvarEmBinario(BCLV *vetor, int tam) {

    ofstream arqBinario("livros.dat", ios::binary);

    if (!arqBinario) {
        cout << "ERRO ao Criar o Arquivo Binário!" << endl;
        return;
    }
    else{
        for (int i = 0; i < tam; i++) {

            if(vetor[i].Identificador != 0){

                // Escreve o Identificador e Número de Páginas como inteiros binários
                arqBinario.write(reinterpret_cast<const char*>(&vetor[i].Identificador), sizeof(vetor[i].Identificador));

                // Escreve os strings como vetores de char
                StringParaChar(arqBinario, vetor[i].Nome);
                StringParaChar(arqBinario, vetor[i].NomeAutor);
                StringParaChar(arqBinario, vetor[i].Editora);
                StringParaChar(arqBinario, vetor[i].Genero);

                // Escreve o Identificador e Número de Páginas como inteiros binários
                arqBinario.write(reinterpret_cast<const char*>(&vetor[i].NumeroPaginas), sizeof(vetor[i].NumeroPaginas));
            }
        }  
    }
    arqBinario.close();
}

//redimensionamento do vetor
BCLV *redimensiona(int& tam, BCLV *vetor){

    BCLV *vetorAux = new BCLV[tam + 5];

    copy(vetor, vetor + tam, vetorAux);
   
    delete[] vetor;

    tam += 5;

    return vetorAux;
}          

//funcao para salvar os dados em CSV
void salvarCsv(BCLV *vetor, int &tam){

    ofstream arqSaidaCsv("BancoLivros01.csv");
    int i = 0;

    arqSaidaCsv << "Identificador;Nome;Nome Autor;Editora;Genero;Numero de Paginas\n"; //Escrevendo o cabeçalho do CSV
    arqSaidaCsv << tam << '\n';

    while(i <= tam){
        if(vetor[i].Identificador != 0){ //arquivo vai receber ate chegar nas posicoes vazias do vetor

            arqSaidaCsv << vetor[i].Identificador << ';';
            arqSaidaCsv << vetor[i].Nome << ';';
            arqSaidaCsv << vetor[i].NomeAutor << ';';
            arqSaidaCsv << vetor[i].Editora << ';';
            arqSaidaCsv << vetor[i].Genero << ';';
            arqSaidaCsv << vetor[i].NumeroPaginas << '\n';

        }
        i++;
    }
}

//funcao para armazenar os dados do CSV em um vetor
BCLV *armazenadorCsv(ifstream &arqEntrada,int &tam){

    int capacidade = 40;
    BCLV *vetor = new BCLV[capacidade]; //Vetor armazenadorCsv dos dados
    char lixo;
    tam = 0;

    while(arqEntrada >> vetor[tam].Identificador){ //Enquanto o arquivo receber

        arqEntrada >> lixo;
        getline(arqEntrada, vetor[tam].Nome, ';');
        getline(arqEntrada, vetor[tam].NomeAutor, ';');
        getline(arqEntrada, vetor[tam].Editora, ';');
        getline(arqEntrada, vetor[tam].Genero, ';');
        arqEntrada >> vetor[tam].NumeroPaginas;

        arqEntrada.ignore();
        tam++;
       
        if(tam == capacidade){
            vetor = redimensiona(capacidade, vetor);
        }
    }
    while(tam < capacidade){

        vetor[tam].Identificador = 0;
        vetor[tam].Nome = "--------------";
        vetor[tam].NomeAutor = "--------------";
        vetor[tam].Editora = "--------------";
        vetor[tam].Genero = "--------------";
        vetor[tam].NumeroPaginas = 0;
       
        tam++;
    }
   
    return vetor;
}

//funcao para armazenar os dados do binario em um vetor
BCLV *armazenadorBinario(ifstream &arqEntradaBin, int &tam){

    int i = 0;
    tam = 40;
    BCLV *vetor = new BCLV[tam];
   

    while(arqEntradaBin.read(reinterpret_cast<char *>(&vetor[i].Identificador), sizeof(vetor[i].Identificador))){

        vetor[i].Nome = CharParaString(arqEntradaBin);
        vetor[i].NomeAutor = CharParaString(arqEntradaBin);
        vetor[i].Editora = CharParaString(arqEntradaBin);
        vetor[i].Genero = CharParaString(arqEntradaBin);

        arqEntradaBin.read(reinterpret_cast<char *>(&vetor[i].NumeroPaginas), sizeof(vetor[i].NumeroPaginas));

        i++;
               
        if(i == tam){
        vetor = redimensiona(tam, vetor);
        }
    }
    while(i < tam){

        vetor[i].Identificador = 0;
        vetor[i].Nome = "--------------";
        vetor[i].NomeAutor = "--------------";
        vetor[i].Editora = "--------------";
        vetor[i].Genero = "--------------";
        vetor[i].NumeroPaginas = 0;
       
        i++;
    }

    return vetor;
}

//funcao para adicionar um livro novo livro
void inserir(BCLV *vetor, int &tam){

    BCLV NovoLivro;
    int i = 0;

    cout << "Digite o novo ID: ";
    cin >> NovoLivro.Identificador;
    cout << "Digite o nome do Livro: ";
    cin.ignore();
    getline(cin, NovoLivro.Nome);
    cout << "Digite o nome do Autor: ";
    getline(cin, NovoLivro.NomeAutor);
    cout << "Digite a editora: ";
    getline(cin, NovoLivro.Editora);
    cout << "Digite o genero: ";
    getline(cin, NovoLivro.Genero);
    cout << "Digite o numero de paginas: ";
    cin >> NovoLivro.NumeroPaginas;

    bool existe = false;

    while (i <= tam && !existe) {
        if (i == tam) {
            vetor = redimensiona(tam,vetor);
        }
        if (vetor[i].Nome == NovoLivro.Nome || vetor[i].Identificador == NovoLivro.Identificador) {
            cout << "Livro ja cadastrado." << endl;
            existe = true;

        } else if (vetor[i].Identificador == 0) {

            cout << "Livro adicionado com sucesso!" << endl;
            vetor[i] = NovoLivro;
            existe = true;
        }
        i++;
    }
}

//funcao para remover um livro ja cadastrado
void remover(BCLV *&vetor, int &tam){

    int i = 0;
    int ID;
    bool encontrada = false;
    int recebeIndice;

    cout << "Digite o ID do livro que deseja remover: ";
    cin >> ID;

   quickSortID(vetor, 0, tam - 1);

    while(i <= tam && !encontrada){

        if(vetor[i].Identificador == ID){
            encontrada = true;
            recebeIndice = i;
        }
        else{
            encontrada = false;
        }
        i++;
    }
    i = recebeIndice;

    if(encontrada == false){

        cout << "Livro nao encontrado" << endl;
        cout << endl;
    }
    else{

        cout << "Livro removido com sucesso!" << endl;
        cout << endl;

        BCLV *vetorAux = new BCLV[tam - 1];
        copy(vetor, vetor + i, vetorAux);
        copy(vetor + (i + 1), vetor + tam, vetorAux + i);
        tam--;

        delete[] vetor;
        vetor = vetorAux;
    }
} 

//funcao para editar um livro ja cadastrado
void editar(BCLV *&vetor, int &tam){

    int i = 0;
    int ID;
    bool encontrada = false;
    int recebeIndice;

    cout << "Digite o ID do livro que deseja editar: ";
    cin >> ID;

   quickSortID(vetor, 0, tam - 1);

    while(i <= tam && !encontrada){

        if(vetor[i].Identificador == ID){
            encontrada = true;
            recebeIndice = i;
        }
        else{
            encontrada = false;
        }
        i++;
    }
    i = recebeIndice;

    if(encontrada == false){

        cout << "Identificador nao encontrado" << endl;
        cout << endl;
        editar(vetor,tam);
    }
    else{

        cout << "Digite o novo Identificador:" ;
        int AuxID;
        cin >> AuxID;

        bool existeID = false;
        for(int j = 0; j < tam; j++){

            if(vetor[j].Identificador == AuxID){
                existeID = true;
            }
        }
        if(existeID){
            cout << "Identificador ja existente. Tente outro." << endl;
        }
        else{
            vetor[i].Identificador = AuxID;
            cout << "Digite o novo Nome do Livro:";
            cin >> vetor[i].Nome;
            cout << "Digite o novo Nome do autor:";
            cin >> vetor[i].NomeAutor;
            cout << "Digite a nova Editora:";
            cin >> vetor[i].Editora;
            cout << "Digite o novo Genero:";
            cin >> vetor[i].Genero;
            cout << "Digite o novo Numero de Paginas:";
            cin >> vetor[i].NumeroPaginas;
            cout << endl;
            cout << "Livro editado com sucesso!" << endl;
            cout << endl;
        }
    }
} 

//funcao para buscar um identificador de forma binaria
void buscaBinID(BCLV *vetor, int tam, int ID){

    int inicio = 0;
    int final = tam - 1;
    bool encontrada = false;

    while(inicio <= final){
        int meio = inicio + (final - inicio) / 2;

        if(vetor[meio].Identificador == ID){

            encontrada = true;

            cout << "Identificador encontrado!" << endl;
            cout << endl;
            cout << "------------------------------------------" << endl;
            cout << "Identificador: " << vetor[meio].Identificador << endl;
            cout << "Nome: " << vetor[meio].Nome << endl;
            cout << "Nome do Autor: " << vetor[meio].NomeAutor << endl;
            cout << "Editora: " << vetor[meio].Editora << endl;
            cout << "Genero: " << vetor[meio].Genero << endl;
            cout << "Numero de Paginas: " << vetor[meio].NumeroPaginas << endl;
            cout << "------------------------------------------" << endl;
           
            inicio = final + 1;
           
            if(encontrada == false){
                cout << "Livro nao Encontrado" << endl;
                cout << endl;
            }
        }
        else if(vetor[meio].Identificador < ID){
            inicio = meio + 1;
        }
        else{
            final = meio - 1;
        }
    }
    if(encontrada == false){
        cout << "Identificador nao encontrado" << endl;
    }
    finalizarPrograma();
}

//funcao para buscar um nome de livro de forma binaria
void buscaBinNome(BCLV *vetor, int tam, string nome){

    int inicio = 0;
    int final = tam - 1;
    bool encontrada = false;

    while(inicio <= final){
        int meio = ((final + inicio) / 2);

        if(vetor[meio].Nome == nome){
           
            encontrada = true;
            cout << endl;
            cout << "-------------Livro encontrado!-------------" << endl;
            cout << endl;
            cout << "Identificador: " << vetor[meio].Identificador << endl;
            cout << "Nome: " << vetor[meio].Nome << endl;
            cout << "Nome do Autor: " << vetor[meio].NomeAutor << endl;
            cout << "Editora: " << vetor[meio].Editora << endl;
            cout << "Genero: " << vetor[meio].Genero << endl;
            cout << "Numero de Paginas: " << vetor[meio].NumeroPaginas << endl;
            cout << "-------------------------------------------" << endl;

            inicio = final + 1;

            if(encontrada == false){
                cout << "Livro nao Encontrado" << endl;
                cout << endl;
            }
        }
        else if(vetor[meio].Nome < nome){
            inicio = meio + 1;
        }
        else{
            final = meio - 1;
        }
    }
    if(encontrada == false){
        cout << "Nome nao encontrado" << endl;
    }    
    finalizarPrograma();
}

//funcao para imprimir os dados de um arquivo CSV
void imprimeCsvInteiro(BCLV *vetor, int tam){

    cout << endl;
    cout << "----------------- LIVROS -----------------" << endl;
    cout << endl;

    for (int i = 0; i < tam; i++){

        if(vetor[i].Identificador != 0){

            cout << "Identificador: " << vetor[i].Identificador << endl;
            cout << "Nome: " << vetor[i].Nome << endl;
            cout << "Nome do Autor: " << vetor[i].NomeAutor << endl;
            cout << "Editora: " << vetor[i].Editora << endl;
            cout << "Genero: " << vetor[i].Genero << endl;
            cout << "Numero de Paginas: " << vetor[i].NumeroPaginas << endl;
            cout << endl;
            cout << "------------------------------------------" << endl;
            cout << endl;
        }
    }
    finalizarPrograma();
}

//funcao para imprimir um trecho do arquivo
void imprimeTrecho(BCLV *vetor){

    cout << endl;
    cout << "Certo! Digite a posicao inicial do codigo que deseja imprimir (ex: 1): ";
    int inicio;
    cin >> inicio;
    cout << "Digite a posicao final do codigo que deseja imprimir (ex: 10): ";
    int fim;
    cin >> fim;

    cout << endl;
    cout << "----------------- LIVROS -----------------" << endl;
    cout << endl;

    for(int i = inicio - 1; i < fim; i++){

        cout << "Identificador: " << vetor[i].Identificador << endl;
        cout << "Nome: " << vetor[i].Nome << endl;
        cout << "Nome do Autor: " << vetor[i].NomeAutor << endl;
        cout << "Editora: " << vetor[i].Editora << endl;
        cout << "Genero: " << vetor[i].Genero << endl;
        cout << "Numero de Paginas: " << vetor[i].NumeroPaginas << endl;
        cout << endl;
        cout << "------------------------------------------" << endl;
        cout << endl;

    }
    finalizarPrograma();
}

//funcao para imprimir as informacoes de acordo com as preferencias do usuario
void menu(BCLV *&vetor, int tam){

    int p1;
    string nomeBuscado = "";
    int idBuscado;
    int reiniciar;
           
    //clear para linux e cls para windows
    system("cls || clear");
   
    cout << "------------------------------------------------------------------" << endl;
    cout << "   -| ESCOLHA UMA DAS OPCOES ABAIXO PARA COMECAR A NAVEGACAO |-   " << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << " || Digite (1) para Imprimir todos os dados do arquivo na tela || " << endl;
    cout << " ||                                                            || " << endl;
    cout << " || Digite (2) para Imprimir um trecho especifico do arquivo   || " << endl;
    cout << " ||                                                            || " << endl;
    cout << " || Digite (3) para Adicionar um livro ao arquivo              || " << endl;
    cout << " ||                                                            || " << endl;
    cout << " || Digite (4) para Remover um livro do arquivo                || " << endl;
    cout << " ||                                                            || " << endl;   
    cout << " || Digite (5) para Editar um livro ja cadastrado              || " << endl;
    cout << " ||                                                            || " << endl;
    cout << " || Digite (6) para Buscar um livro pelo nome                  || " << endl;
    cout << " ||                                                            || " << endl;
    cout << " || Digite (7) para Buscar um livro pelo identificador         || " << endl;
    cout << " ||                                                            || " << endl;  
    cout << " || Digite (8) para Salvar as alteracoes no arquivo CSV        || " << endl;
    cout << " ||                                                            || " << endl;   
    cout << " || Digite (9) para Salvar as alteracoes no arquivo binario    || " << endl;
    cout << " ||                                                            || " << endl;    
    cout << " || Digite (10) para Fechar                                    || " << endl;
    cout << "------------------------------------------------------------------" << endl;
    cout << endl;

    cin >> p1;

    switch(p1){//switch para escolher a funcao de acordo com a resposta do usuario

        case 1:

            imprimeCsvInteiro(vetor, tam);
            cout << "------------------------------------------------------------" << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;
       
        case 2:
           
            imprimeTrecho(vetor);
            cout << "------------------------------------------------------------" << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;
           
        case 3:
     
            inserir(vetor, tam);
            cout << "------------------------------------------------------------" << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;

        case 4:
       
            remover(vetor, tam);
            cout << "------------------------------------------------------------" << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;

        case 5:

            editar(vetor, tam);
            cout << "------------------------------------------------------------" << endl;
            
            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;  



        case 6:
           
            cout << "Digite o Livro que Deseja Buscar: " << endl;
            cout << endl;
            getline(cin, nomeBuscado);
            quickSortNome(vetor, 0, tam - 1);

            while(getline(cin,nomeBuscado) && nomeBuscado != "parar"){

                buscaBinNome(vetor, tam, nomeBuscado);
                getline(cin, nomeBuscado);
             
                cout << "Caso queira voltar ao menu incial digite 2" << endl;
                cin >> reiniciar;
                if(reiniciar == 2){
                menu(vetor, tam);
            }
            }
            break;

        case 7:

            cout << "Digite o Identificador que Deseja Buscar (Entre 100001 e 100100) " << endl;
            cout << endl;

            cin >> idBuscado;

            quickSortID(vetor, 0, tam - 1);

            buscaBinID(vetor, tam, idBuscado);

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;

        case 8:

            salvarCsv(vetor,tam);
            cout << "------------------------------------------------------------" << endl;
           
            cout << "Arquivo salvo com sucesso em livros.dat" << endl;
            cout << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;

        case 9:

            salvarEmBinario(vetor,tam);
            cout << "------------------------------------------------------------" << endl;

            cout << "Arquivo salvo com sucesso em livros.dat" << endl;
            cout << endl;

            cout << "Caso queira voltar ao menu incial digite 2" << endl;
            cin >> reiniciar;
            if(reiniciar == 2){
                menu(vetor, tam);
            }
            break;
            
        case 10:
        
            finalizarPrograma();
            break;

        default:

            menu(vetor,tam);//Recomeca o questionario caso o valor digitado seja diferente dos disponiveis

    }
}

//funcao para ler o arquivo
void leitura(){     

    string linha = "";
    int qtdeRegistros;
    int arquivoDesejado;
    BCLV *Livros;
               
    //clear para linux e cls para windows
    system("cls || clear");
    cout << "------------------------------------------------------------" << endl;
    cout << "Escolha qual tipo de arquivo voce deseja trabalhar: "<< endl;
    cout << "      (Digite 1 para .csv ou 2 para .dat)" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << endl;
    cin >> arquivoDesejado;
   
    if(arquivoDesejado == 1){
        ifstream arqEntrada("BancoLivros01.csv");

        if (!arqEntrada.is_open()){
            cout << "ERRO ao Abrir o Arquivo!" << endl;
            leitura();
        }
        else {

            cout << "Arquivo Aberto com Sucesso!" << endl;
            cout << endl;
       
            getline(arqEntrada, linha);
            arqEntrada >> qtdeRegistros;
            arqEntrada.ignore();

            Livros = armazenadorCsv(arqEntrada, qtdeRegistros);
            menu(Livros, qtdeRegistros);

        }
        arqEntrada.close();
    }
    else if(arquivoDesejado == 2){

        ifstream arqEntradaBin("livros.dat", ios::binary);

        if (!arqEntradaBin.is_open()){
            cout << "ERRO ao Abrir o Arquivo!" << endl;
            leitura();

        }
        else {

            cout << "Arquivo Aberto com Sucesso!" << endl;
            cout << endl;

            Livros = armazenadorBinario(arqEntradaBin,qtdeRegistros);
            menu(Livros, qtdeRegistros);

        }
    }
}

//funcao main, fiz o possivel para deixa-la vazia
int main() {

    leitura();

    return 0;
}