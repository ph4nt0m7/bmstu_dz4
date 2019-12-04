#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void PrintGraph(int amount, int **conn_elements, char **name_elements) {
    printf("--------------------\n"); //related graph / unrelated graph
    _Bool relat_graph = 1;
    for(int i=0; i<amount; i++){
        _Bool temp_relat_graph = 0; // 0 - no conn
        for(int j=0; j<amount; j++){
            if(conn_elements[i][j] == 1)
                temp_relat_graph = 1;
            
            if(conn_elements[j][i] == 1)
                temp_relat_graph = 1;
        }
        if(temp_relat_graph == 0)
            relat_graph=0;
    }
    if(relat_graph == 0){
        printf("unrelated graph\n");
    } else {
        printf("related graph\n");
        
    }
    printf("--------------------\n\t");
    
    for(int i=0;i<amount;i++){
        printf("%c%c%c.\t",name_elements[i][0], name_elements[i][1], name_elements[i][2]);
    }
    printf("\n");
    for(int i=0; i<amount; i++) {
        printf("%s\t", name_elements[i]);
        for (int j=0; j<amount; j++) {
            printf("%d\t", conn_elements[i][j]);
        }
        printf("\n");
    }
}

/// Sorting Graph using ShellSort
/// @param amount amount of vertexes
/// @param names array of names of vertexes
/// @param conns table of connection for all vertexes
/// @param mode "asc" / "ASC" / "Asc" OR "desc" / "DESC" / "Desc" [if no one uses desc]
void SortShellGraph(int amount, char **names, int **conns, char mode[5]) {
    int *vertex_degree;
    vertex_degree = calloc(amount, sizeof(int));
    
    for(int i=0; i<amount; i++){
        for(int j=0; j<amount; j++){
            vertex_degree[i] += conns[i][j];
            vertex_degree[i] += conns[j][i];
        }
    }
    
    int temp_changes[amount];
    for(int i=0; i<amount; i++){
        temp_changes[i] = i;
    }

    for (int gap = amount/2; gap > 0; gap /= 2) {
        for (int i = gap; i < amount; i++) {
            for (int j = i-gap; j >= 0 && vertex_degree[j] > vertex_degree[j+gap]; j-=gap) {
                int temp = vertex_degree[j];
                vertex_degree[j] = vertex_degree[j+gap];
                vertex_degree[j+gap] = temp;

                int temp2 = temp_changes[j];
                temp_changes[j] = temp_changes[j+gap];
                temp_changes[j+gap] = temp2;
            }
        }
    }
    
    if( (strcmp(mode, "asc") || strcmp(mode, "ASC") || strcmp(mode, "Asc")) != 0 ){
        for(int i=0; i<amount/2; i++){
            int temp = vertex_degree[i];
            vertex_degree[i] = vertex_degree[amount-i-1];
            vertex_degree[amount-i-1] = temp;
            
            temp = temp_changes[i];
            temp_changes[i] = temp_changes[amount-i-1];
            temp_changes[amount-i-1] = temp;
        }
    }

    char **temp_names = calloc(amount, sizeof(char*));
    int **temp_conns   = calloc(amount, sizeof(int*));
    for(int i=0; i<amount; i++)
        temp_names[i] = names[i];

    for(int i=0; i<amount; i++){
        int temp_arr[amount];
        for(int j=0; j<amount; j++)
            temp_arr[j] = 0;
            
        for(int j=0; j<amount; j++)
            temp_arr[j] = conns[i][ temp_changes[j] ];
        
        for(int j=0; j<amount; j++)
            conns[i][j] = temp_arr[j];
        
        temp_conns[i] = conns[i];
    }
    
    for(int i=0; i<amount; i++){
        names[i] = temp_names[ temp_changes[i] ];
        conns[i] = temp_conns[ temp_changes[i] ];
    }
    printf("\n");
    free(vertex_degree);
    free(temp_conns);
    free(temp_names);
    
}

char *InfinityArray(void) {
    int array_size=1;
    char *arr,c;
    arr = calloc(array_size+1, sizeof(char));

    int i = 0;
    while((c=getchar()) != ';'){
        if( ('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')){ //[a-zA-Z0-9]
            char *temp_arr;
            temp_arr = calloc(array_size+1, sizeof(char));
            
            for(int cl = 0; cl < array_size; cl++){
                temp_arr[cl] = arr[cl];
            }
            
            arr[array_size-1]=c;
            array_size++;
            i++;
            free(temp_arr);
        }
        else if(c == '\n'){
            return arr;
            
        }
    }
    arr[array_size-1]='\0';
    return arr;
}

int main() {
    int digraph = -1;
    
    char temp_graph[64] = {0};
    
    while(digraph == -1){
        printf("Use graph or digraph:\n> ");
        scanf("%s", temp_graph);
        if( strcmp(temp_graph, "graph") == 0){
            digraph = 0;
        } else if( strcmp(temp_graph, "digraph") == 0) {
            digraph = 1;
        } else {
            printf("Enter valid parameter\n");
        }
    }
    
    printf("Enter amount of elements:\n> ");
    int amount;
    scanf("%d",&amount);
    if (amount <= 0)
        return 6;
    
    printf("Enter names of elements separated by ';':\n");
    printf("! You could enter characters [a-zA-Z] and numbers [0-9]\n");
    printf("! All another symbols will be ignored)\n");

    
    char *name_elements[amount];
    int *conn_elements[amount];
    for(int i=0; i<amount; i++){
        conn_elements[i] = calloc(amount, sizeof(int));
        for(int j=0; j<amount; j++){
            conn_elements[i][j] = 0;
        }
    }
    _Bool flag_one=1;
    getchar();
    while(flag_one){
        printf("> ");
        int duplicate = 0;
        for(int i=0; i<amount; i++){
            name_elements[i] = InfinityArray();
        }
        
        for(int i=0; i<amount; i++){
            for (int j=0; j<amount; j++){
                if( strcmp(name_elements[i], name_elements[j]) == 0 && i != j ){
                    duplicate++;
                }
            }
        }
        
        if(duplicate != 0){
            printf("[Error] You enter 2 or more identical elements\n Retype it...\n");
        } else {
            flag_one=0;
        }
    }
    if(1==1){
        char c;
        while( (c = getchar()) != '\n' );
    }
    printf("Enter for each one connection separated by ';', for input press enter:\n");
    for(int i=0; i<amount; i++){
        printf("'%s'> ",name_elements[i]);
        _Bool flag=1;
        while(flag){
            char *compare;
            compare = InfinityArray();
            
            for(int j=0; j<amount; j++){
                if( strcmp(compare, name_elements[j]) == 0){
                    conn_elements[i][j]++;
                }
            }

            if(strcmp(compare, "") == 0){
                flag = 0;
            }
            free(compare);
        }
    }
    
    _Bool flag=1;
    while(flag){
        printf("Print Table? (y/n)\n");
        char answer;
        scanf("%c",&answer);getchar();
        if(answer == 'y' || answer == 'Y'){
            PrintGraph(amount, conn_elements, name_elements);
 
            flag=0;
        } else if (answer == 'n' || answer == 'N'){
            flag=0;
        } else {
            printf("Enter valid parameter\n");
        }
    }
    
    _Bool flag2=1;
    while(flag2){
        printf("Sort graph? (y/n)\n");
        char answer;
        scanf("%c",&answer);getchar();
        if(answer == 'y' || answer == 'Y'){
            SortShellGraph(amount, name_elements, conn_elements, "asc");
            
            PrintGraph(amount, conn_elements, name_elements);
            flag2=0;
        } else if (answer == 'n' || answer == 'N'){
            flag2=0;
        } else {
            printf("Enter valid parameter\n");
        }
    }

    
    
    char *string;
    string = calloc(2048, sizeof(char));
    char arrow[3] = "";
    strcat(string, "echo '");
    if(digraph == 1){
        strcat(string, "digraph G {");
        strcat(arrow, "->");
    } else {
        strcat(string, "graph G {");
        strcat(arrow, "--");
    }
    for(int i=0; i<amount; i++){
        strcat(string, name_elements[i]);
        strcat(string, "; ");
    }
    for(int i=0; i<amount; i++){
        for(int j=0; j<amount; j++){
            for(int k=0; k<conn_elements[i][j]; k++){
                strcat(string, name_elements[i]);
                strcat(string, arrow);
                strcat(string, name_elements[j]);
                strcat(string, "; ");
                
            }
        }
    }
    strcat(string, "}' | dot -Tpng > ./graph.png");

    
    system(string);
    
    _Bool flag3=1;
       while(flag3){
           printf("Open File? (y/n)\n");
           char answer;
           scanf("%c",&answer);getchar();
           if(answer == 'y' || answer == 'Y'){
               system("open ./graph.png");
               flag3=0;
           } else if (answer == 'n' || answer == 'N'){
               flag3=0;
           } else {
               printf("Enter valid parameter\n");
           }
       }
    
    free(string);
    for(int i=0; i<amount; i++){
        free(name_elements[i]);
        free(conn_elements[i]);
    }
    
    
    printf("\nBYE...\n");
    return 0;
}
