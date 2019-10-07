#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    char buffer[51][500];
    char linker[5];
    int inputMat[51][51];
    int sumOfCols[51];
    int sumOfRows[51];
    int rowcount=0;
    int colcount=0;
    int lastNumCheck=0;
    int link=0;
    if(argc==1){
	printf("put the input filename");
	return -1;
    }             
    FILE *fp = fopen(argv[1], "r");
    //read a input file	
    while(!feof(fp)){ 
        fgets(buffer[rowcount],sizeof(buffer),fp);
        if(buffer[rowcount][0] =='\0'|| buffer[rowcount][0] =='\n' || buffer[rowcount][0] == ' ') break;
        rowcount++;
    }
    fclose(fp);
    //get a sum of rows in each cols
    for(int i=0;i<500;i++){
        if(buffer[0][i] != ' ' && buffer[0][i] !='\n' &&buffer[0][i]!='\0' ){
            linker[link]=buffer[0][i];
            link++;
        }
        else{
            sumOfRows[colcount]=atoi(linker);
            colcount++;
            for(int j=0;j<5;j++){
                linker[j]='\0';
            }
            link=0;
        }
        if(buffer[0][i]=='\n' || buffer[0][i]=='\0') break;
    }
    int rows=rowcount-1;
    int cols=colcount;
    //get sum of cols in each rows and input matrix's value
    for(int i=1;i<rows+1;i++)
        lastNumCheck=0;
        for(int j=0;j<=500;j++){
            if(buffer[i][j] != ' ' && buffer[i][j] !='\n'&&buffer[i][j]!='\0'){
                linker[link]=buffer[i][j];
                link++;
            }
            else{
                if(lastNumCheck ==cols) sumOfCols[i-1]=atoi(linker);//sum of cols in each rows
                else inputMat[i-1][lastNumCheck]=atoi(linker);//put the input matrix's value
                lastNumCheck++;
                for(int k=0;k<5;k++){
                    linker[k]='\0';
                }
                link=0;
            
        }
            if(buffer[i][j]=='\n' || buffer[i][j]=='\0') break;
        }
    }
    //write the formula
    fp = fopen("formula","w");
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            fprintf(fp, "(declare-const p%d/%d Int)\n", i, j);//declaration about sum of cols in each rows(black is 1, white is 0)
    
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            fprintf(fp, "(declare-const q%d/%d Int)\n", i, j);//declaration about sum of rows in each cols(white is 1, black is 0)
    
    fprintf(fp, "(assert (and ");
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
           fprintf(fp, "(or (= p%d/%d 1) (= p%d/%d 0))", i, j, i, j);//p%d/%d could be either 1 or 0
    fprintf(fp, "))\n");

    fprintf(fp, "(assert (and ");
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < cols; j++)
           fprintf(fp, "(or (= q%d/%d 1) (= q%d/%d 0))", i, j, i, j);////q%d/%d could be either 1 or 0
    fprintf(fp, "))\n");
    
//make a formula about sum of rows in each cols
    fprintf(fp,"(assert (and ");
    for (int i=0; i<cols; i++){
        fprintf(fp,"(= (+ ");
        for (int j=0; j<rows; j++){
            fprintf(fp,"(* p%d/%d %d)",j,i,inputMat[j][i]);
        }
        fprintf(fp,") %d)",sumOfRows[i]);
    }
    fprintf(fp,"))\n");
//make a formula about sum of cols in each rows
    fprintf(fp,"(assert (and ");
    for (int i=0; i<rows; i++){
        fprintf(fp,"(= (+ ");
        for (int j=0; j<cols; j++){
            fprintf(fp,"(* q%d/%d %d)",i,j,inputMat[i][j]);
        }
        fprintf(fp,") %d)",sumOfCols[i]);
    }
    fprintf(fp,"))\n");
//if p%d/%d is 0, q%d/%d have to 1.
//if p%d/%d is 1, q%d/%d have to 0.
    fprintf(fp,"(assert (and ");
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            fprintf(fp,"(or (= (- p%d/%d 1) q%d/%d) (= (+ p%d/%d 1) q%d/%d))",i,j,i,j,i,j,i,j);
        }
    }
    fprintf(fp,"))\n");

    fprintf(fp, "(check-sat)\n");
    fprintf(fp, "(get-model)\n");
    fclose(fp);

    int i, j;
    char b[128];
    char s[128];
    char t[128];
    char buf[128];
    int board[51][51]={0};
    FILE *fin = popen("z3 formula","r");
    
    fscanf(fin, "%s", buf);
    
    // no solution 
    if (strcmp(buf, "unsat") == 0){
        printf("No solution\n");
        return -1;
      }
    fscanf(fin, "%s", buf);
   
    while (!feof(fin))
    {
      fscanf(fin, "%s %s %s %s %s", b, s, b, b, t);
      //we need only p%d/%d(black is 1 white is 0)
      if(strchr(s,'p')){
        sscanf(s, "p%d/%d", &i, &j);
        if (strcmp(t, "0)") != 0)
        {
            board[i][j] = 1;
        }
      }
    }
    // print solution 
    for (i = 0; i < rows; i++){
      for (j = 0; j < cols; j++){
        printf("%d ", board[i][j]);
      }
      printf("\n");
    }
    printf("\n");

    pclose(fin);

    return 0;

}
