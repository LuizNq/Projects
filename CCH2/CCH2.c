#include <stdio.h>
#include <stdlib.h>

typedef struct Departamento{ // Define uma struct para os departamentos.
    int id;
    char nome_dpt[100];
} DPTO;

typedef struct Funcionario{ // Define uma struct para os funcionários.
    int id_func;
    char nome_func[100];
    float salario;
    DPTO dpto_func;
} FUNC;

void cadastro_dpto(FILE *arq_dpto){
    DPTO departamento;

    printf("Digite o ID do departamento: ");
    scanf("%d", &departamento.id);
    printf("Digite o nome do departamento: ");
    scanf("%s", departamento.nome_dpt);

    fwrite(&departamento, sizeof(DPTO), 1, arq_dpto); // Escreve os dados no arquivo de departamentos.
}

void imprimir_dptos(FILE *arq_dpto){
    DPTO departamento;

    rewind(arq_dpto);

    //  Le e imprime os departamentos cadastrados
    while (fread(&departamento, sizeof(DPTO), 1, arq_dpto) == 1) {
        printf("ID: %d, Nome: %s\n", departamento.id, departamento.nome_dpt);
    }
}

void cadastro_func(FILE *arq_func, FILE *arq_dpto){
    FUNC funcionario;
    DPTO departamento;

    printf("Digite o ID do funcionario: ");
    scanf("%d", &funcionario.id_func);
    printf("Digite o nome do funcionario: ");
    scanf("%s", funcionario.nome_func);
    printf("Digite o salario do funcionario: ");
    scanf("%f", &funcionario.salario);

    int id_dpto_existe = 0;

    while(1){
        printf("Digite o departamento do funcionario: ");
        scanf("%d", &funcionario.dpto_func.id);

        // Verifica se o ID do departamento existe
        rewind(arq_dpto);

        id_dpto_existe = 0;

        while(fread(&departamento, sizeof(DPTO), 1, arq_dpto) == 1){
            if(departamento.id == funcionario.dpto_func.id){
                id_dpto_existe = 1;
                break;
            }
        }

        if(id_dpto_existe){
            break;
        }else{
            printf("ID do departamento nao existe. Tente novamente.\n");
        }
    }
    

    fwrite(&funcionario, sizeof(FUNC), 1, arq_func); // Escreve os dados dos funcionários no arquivo.
}

void imprimir_func(FILE *arq_func){
    FUNC funcionario;

    rewind(arq_func);

    int id_dpto;

    printf("Digite o departamento: ");
    scanf("%d", &id_dpto);

    //  Le e imprime os funcionarios cadastrados
    while(fread(&funcionario, sizeof(FUNC), 1, arq_func) == 1) {
        if(funcionario.dpto_func.id == id_dpto){
            printf("ID: %d, Nome: %s, Salario: %.2f, Departamento: %d\n", funcionario.id_func, funcionario.nome_func,
            funcionario.salario, funcionario.dpto_func.id);
        }
    }
}

void buscar_id(FILE *arq_func){
    FUNC funcionario;

    rewind(arq_func);

    int buscar;

    printf("Digite o ID do funcionario: ");
    scanf("%d", &buscar);

    //  Le e imprime os funcionários cadastrados com base no ID
    while (fread(&funcionario, sizeof(FUNC), 1, arq_func) == 1) {
        if(funcionario.id_func == buscar){
            printf("ID: %d, Nome: %s, Salario: %.2f, Departamento: %d\n", funcionario.id_func, funcionario.nome_func,
            funcionario.salario, funcionario.dpto_func.id);
        }
    }
}

void remover_func(FILE *arq_func){
    FUNC funcionario;
    FILE *arq_func_temp = fopen("dados_func_temp.bin", "wb+"); // Cria um arquivo temporário
    if (arq_func_temp == NULL) {
        printf("Erro ao abrir o arquivo temporario!\n");
        exit(1);
    }

    rewind(arq_func);

    int buscar;

    printf("Digite o ID do funcionario a ser removido: ");
    scanf("%d", &buscar);

    while (fread(&funcionario, sizeof(FUNC), 1, arq_func) == 1) {
        if (funcionario.id_func != buscar) {
            fwrite(&funcionario, sizeof(FUNC), 1, arq_func_temp);  // Escreve os dados de todos os funcionários, exceto o que será removido, no arquivo temporário.
        }
    }

    fclose(arq_func);
    fclose(arq_func_temp);

    remove("dados_func.bin");
    rename("dados_func_temp.bin", "dados_func.bin"); // Substitui o arquivo temporário pelo original.

    arq_func = fopen("dados_func.bin", "rb+");
    if (arq_func == NULL) {
        printf("Erro ao reabrir o arquivo!\n");
        exit(1);
    }
}

int main(){
    int opcao;

    FILE *arq_dpto = fopen("dados.bin", "wb+");
    if(arq_dpto == NULL) {
        printf("Erro ao abir o arquivo!\n"); 
        exit(1); // Verifica se o arquivo existe
    }

    FILE *arq_func = fopen("dados_func.bin", "wb+");
    if(arq_func == NULL) {
        printf("Erro ao abir o arquivo!\n"); 
        exit(1); // Verifica se o arquivo existe
    }

    do {
        printf("Menu\n");
        printf("1. Cadastrar Departamentos\n");
        printf("2. Cadastrar Funcionarios\n");
        printf("3. Listar Departamentos\n");
        printf("4. Listar Funcionarios de Um Departamento\n");
        printf("5. Buscar Funcionario por ID\n");
        printf("6. Remover Funcionario\n");
        printf("7. Encerrar Programa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1:
                // Cadastrar Departamentos
                system("cls");
                cadastro_dpto(arq_dpto);
                break;
            case 2:
                // Cadastrar Funcionarios
                system("cls");
                cadastro_func(arq_func, arq_dpto);
                break;
            case 3:
                // Listar Departamentos
                system("cls");
                imprimir_dptos(arq_dpto);
                break;
            case 4:
                // Listar Funcionarios
                system("cls");
                imprimir_func(arq_func);
                break;
            case 5:
                // Buscar por Id
                system("cls");
                buscar_id(arq_func);
                break;
            case 6:
                system("cls");
                remover_func(arq_func);
                break;
            case 7:
                // Encerrar Programa
                system("cls");
                printf("Encerrando Programa...\n");
                break;
            default:
                system("cls");
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
    } while(opcao != 7);

    fclose(arq_dpto);
    fclose(arq_func);

    return 0;
}
