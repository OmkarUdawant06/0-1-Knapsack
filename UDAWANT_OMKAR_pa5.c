#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

struct knapsack_struct          //file structure for 0/1knapsack
{
    char item_name[40];
    int profit;
    int weight;
    double fraction;
}item[30];

//global variables
int B[30][100];
int X[30];
int maxprofit;
int num;
int tempweight = 0;
int include[30];
int bestset[30];
int flag[30][100]={0};
int solution,solution_temp;
int tempP1,tempW1,tempW2 = 0;

//required functions
int bruteForce(int n,int W);                    //bruteforce approach
int RDynamicP(int n, int w);                    //dynamic approach
int KWF2(int i, int weight, int profit, int n, int W);
int Promising(int i, int profit, int W, int weight, int n);     //check if node is promising
void knapsack(int i, int profit, int weight, int W,int n);      
void Knapsack(int W, int n);                    //Backtrack approach
void validateArguments(int argc);

int bruteForce(int n,int W)
{
    int temp,tempP,tempW;
    tempP1 = 0;
    tempW1 = 0;
    int maxP = 0;
    for(int num=0;num<pow(2,n);num++) //each num is a solution
    {
        temp = num;
        tempW = 0;
        tempP = 0;
        for(int i=0;i<n;i++) //binary n bit
        {
            if(temp%2==1)
            {
                tempW+=item[i].weight;
                tempP+=item[i].profit;
            }
            temp = temp/2;          //binary
        }
        
        if(tempW<=W)
        {
            if(tempP>maxP)
            {
                maxP=tempP;
                solution = num;
            }
        }
    }
    return maxP;
}

int RDynamicP(int n, int w)
{
    if (n == 0 || w<=0) 
    {
        return 0;
    }
    else
    {
        for (int i = n-1; i>=0; i--) 
        {
            if (item[i].weight > w) 
            {
                flag[n][w] = 0;
                return RDynamicP(n-1, w);
            }
            else
            {
                int tmp1 = RDynamicP(n-1, w);
                int tmp2 = item[i].profit + RDynamicP(n-1, w-item[i].weight);
                
                if (tmp1 > tmp2)                    //Calculate B-Matrix
                {
                    flag[n][w] = 0;
                    B[n][w] = tmp1;
                    return tmp1;
                }
                else
                {
                    flag[n][w] = 1;
                    B[n][w] = tmp2;
                    return tmp2;
                }
            }
        }
    }
    return 0;
}

int KWF2(int i, int weight, int profit, int n, int W)
{
    float bound = profit;
    for (int j = i; j<=n; j++) 
    {
        X[j] = 0;
    }
    float sbx;
    while (weight < W && i<=n)                  //calculate bound for every node
    {
        if (weight + item[i-1].weight <= W) 
        {
            weight = weight + item[i-1].weight;
            bound = bound + item[i-1].profit;
        }
        else
        {
            sbx = (float)(W-weight)/(float)item[i-1].weight;
            weight = W;
            bound = bound + item[i-1].profit * sbx;
        }
        i += 1;
    }
    return bound;
}

int Promising(int i, int profit, int W, int weight, int n)
{
    FILE *fp;
    if (weight >= W) 
    {
        return 0;
    }
    float bound = KWF2(i+1, weight, profit, n, W);
    
    printf("(%d, %d, %f)\n",profit,weight,bound);
    fp=fopen("entries3.txt","a+");                  //write enteries to txt file
    fprintf(fp,"(%d, %d, %f)\n",profit,weight,bound);
    fclose(fp);
    return (bound > maxprofit);                     //return bound > max profit
}

void knapsack(int i, int profit, int weight, int W,int n)
{
    if (weight <= W && profit > maxprofit) 
    {
        maxprofit = profit;
        num = i;
        for (int j = 1; j<=n; j++) 
        {
            bestset[j] = include[j];                //set bestset
        }
    }
    if (Promising(i, profit, W, weight, n)) 
    {
        include[i+1] = 1;
        knapsack(i+1, profit + item[i].profit, weight + item[i].weight, W, n);
        include[i+1] = 0;
        knapsack(i+1, profit, weight, W, n);
    }
}

void Knapsack(int W, int n)
{
    FILE *fp;
    num = 0;
    maxprofit = 0;
    tempweight = 0;
    int tempweight_temp, count=0;
    fp=fopen("output3.txt","w");                    //write output to a file
    knapsack(0, 0, 0, W, n);
    tempweight_temp = tempweight;
    printf("\nThe Final Solution(selected items) is: \n\n");
    printf("Item:    Profit    Weight\n\n");
    
    for (int i = 1; i<=num; i++) 
    {
        if (bestset[i]==1) 
        {
            count += 1;
            tempweight_temp += item[i-1].weight;
        }
    }
    fprintf(fp,"%d %d %d\n",count, maxprofit, tempweight_temp);
    for (int i = 1; i<=num; i++) 
    {
        if (bestset[i]==1) 
        {
            tempweight += item[i-1].weight;
            printf("%s        %d        %d\n",item[i-1].item_name, item[i-1].profit, item[i-1].weight);
            fprintf(fp,"%s %d %d\n",item[i-1].item_name, item[i-1].profit, item[i-1].weight);
        }
    }
    printf("\nThe max profit is : %d\n", maxprofit);        //print max profit
    printf("The max weight is : %d\n\n", tempweight);       //print max weight
    fclose(fp);
}

void validateArguments(int argc)            //check if command line arguments are appropriate
{
    if(argc != 3)
    {
        fprintf(stderr,"\nInvalid number of arguments:\nExpected:\t./<executable_name> -k <knapsack_file_name.txt>\n");
        exit(0);
    }
}

int main(int argc, const char * argv[]) 
{
    validateArguments(argc);
    srand(time(0));
    int check;
    FILE *fp;
    int count=0;
    int profit_temp=0;
    int tempW1_temp=0;
    int tempW2_temp=0;
    char *txt=NULL;
    int return_val;
    size_t txt_size = 0;
    int n=0;
    int W =0;

    if(strcmp(argv[0],"./bruteforce") == 0)     
    {
        fp=fopen(argv[2],"r");                      //read file for input
        if(!fp)
        {
            printf("Error opening specified file!\n");
            exit(0);
        }
        //printf("Test func()\n");
        while(return_val = getline(&txt,&txt_size,fp)!=-1)
        {
            n = atoi(strtok(txt," "));                  //initialize number of items
            //printf("N: %d\n",n);
            W = atoi(strtok(NULL," "));                 //initialize capacity of knapsack
            //printf("W: %d\n",W);
            for(int i = 0; i < n; i++)
            {
                return_val = getline(&txt,&txt_size,fp);
                strcpy(item[i].item_name, strtok(txt," ")); //strtok() is ursed for splitting a string by some delimiter.
                item[i].profit = atoi(strtok(NULL," "));        //store profit
                item[i].weight = atoi(strtok(NULL," "));        //store weight
                //printf("%s        %d        %d\n", item[i].item_name, item[i].weight, item[i].profit);
            }
        }
        fclose(fp);

        printf("====================================================\n\n");
        printf("Brute Force algorithm:\n\n");
        int maxP = bruteForce(n, W);
        solution_temp = solution;
        tempW1_temp = tempW1;
        printf("The final solution(selected items) is: \n\n");
        printf("Item:    Profit    Weight\n\n");
        for(int i=0;i<n;i++) //binary n bit
        {
            if(solution%2==1)
            {
                count += 1;
                tempW1+=item[i].weight;
                printf("%s        %d        %d\n", item[i].item_name, item[i].profit, item[i].weight);
            }
            solution = solution/2;              //binary
        }
    
        printf("\nThe max profit is : %d\n", maxP);
        printf("The max weight is : %d\n\n", tempW1);

        fp=fopen("output1.txt","w");
        fprintf(fp, "%d ", count);
        fprintf(fp, "%d ", maxP);
        fprintf(fp, "%d\n", tempW1);
        for(int i=0;i<n;i++)                    //binary n bit
        {
            if(solution_temp%2 == 1)
            {
                tempW1_temp+=item[i].weight;
                fprintf(fp,"%s ",item[i].item_name);
                fprintf(fp,"%d ",item[i].profit);
                fprintf(fp,"%d\n",item[i].weight);
            }
            solution_temp = solution_temp/2;          //binary
        }
        fclose(fp);
    }
    else if(strcmp(argv[0],"./dynpro") == 0)
    {
        fp=fopen(argv[2],"r");
        if(!fp)
        {
            printf("Error opening specified file!\n");
            exit(0);
        }
        while(return_val = getline(&txt,&txt_size,fp)!=-1)
        {
            n = atoi(strtok(txt," "));
            W = atoi(strtok(NULL," "));
            for(int i = 0; i < n; i++)
            {
                return_val = getline(&txt,&txt_size,fp);
                strcpy(item[i].item_name, strtok(txt," ")); //strtok() is ursed for splitting a string by some delimiter.
                item[i].profit = atoi(strtok(NULL," "));
                item[i].weight = atoi(strtok(NULL," "));
            }
        }
        fclose(fp);

        printf("====================================================\n\n");
        printf("Refinement algorithm of dynamic programming approach:\n\n");
        profit_temp = RDynamicP(n, W);                              //call dynamic func()
        
        printf("\nThe final solution(selected items) is: \n\n");
        printf("Item:    Profit    Weight\n\n");
        int ssb, ssk;
        ssb = n; 
        ssk =W;
        count = 0;
        tempW2_temp = tempW2;
        for (int i = 1; i<=n; i++) 
        {
            if (flag[ssb][ssk]==1) 
            {
                count += 1;
                tempW2 += item[ssb-1].weight;
                printf("%s         %d        %d\n",item[ssb-1].item_name, item[ssb-1].profit, item[ssb-1].weight);
                ssk -= item[ssb-1].weight;
            }
            ssb--;
        }
        printf("The Total profit is : %d\n", profit_temp);
        printf("\nThe Total weight is : %d\n\n", tempW2);
        int sb, sk;
        for(sb = 1; sb <= n; sb++)                          //print row entries
        {
            printf("row%d: ", sb);
            for(sk = 0; sk <= W; sk++)
            {
                if(B[sb][sk]!=0)
                {
                    printf("B(%d,%d)=%d ",sb, sk, B[sb][sk]);
                }
            }
            printf("\n");
        }

        fp=fopen("output2.txt","w");
        fprintf(fp, "%d ", count);
        fprintf(fp, "%d ", profit_temp);
        fprintf(fp, "%d\n", tempW2);
        ssb = n; ssk =W;
        count = 0;
        for (int i = 1; i<=n; i++) 
        {
            if (flag[ssb][ssk]==1) 
            {
                tempW2_temp += item[ssb-1].weight;
                fprintf(fp,"%s ",item[ssb-1].item_name);
                fprintf(fp,"%d ",item[ssb-1].profit);
                fprintf(fp,"%d\n",item[ssb-1].weight);
                ssk -=item[ssb-1].weight;
            }
            ssb--;
        }
        fclose(fp);

        fp=fopen("entries2.txt","w");
        for(sb = 1; sb <= n; sb++)
        {
            fprintf(fp, "row%d: ", sb);
            for(sk = 0; sk <=W; sk++)
            {
                if(B[sb][sk])
                {
                    fprintf(fp,"B(%d,%d)=%d ",sb, sk, B[sb][sk]);
                }
            }
            fprintf(fp,"\n");
        }
        fclose(fp);
    }
    else if(strcmp(argv[0],"./backtrack") == 0)
    {
        fp=fopen(argv[2],"r");
        if(!fp)
        {
            printf("Error opening specified file!\n");
            exit(0);
        }
        while(return_val = getline(&txt,&txt_size,fp)!=-1)
        {
            n = atoi(strtok(txt," "));
            //printf("N: %d\n",n);
            W = atoi(strtok(NULL," "));
            //printf("W: %d\n",W);
            for(int i = 0; i < n; i++)
            {
                return_val = getline(&txt,&txt_size,fp);
                strcpy(item[i].item_name, strtok(txt," ")); //strtok() is ursed for splitting a string by some delimiter.
                item[i].profit = atoi(strtok(NULL," "));
                item[i].weight = atoi(strtok(NULL," "));
                printf("%s        %d        %d\n", item[i].item_name, item[i].profit, item[i].weight);
            }
        }
        fclose(fp);
        printf("====================================================\n\n");
        printf("Backtracking algorithm:\n\n");
        for(int i=0;i<n-1;i++)
        {
            item[i].fraction = (double)item[i].profit / (double)item[i].weight;
            for(int j=i+1;j<n;j++)
            {
                item[j].fraction = (double)item[j].profit / (double)item[j].weight;
                if(item[j].fraction>item[i].fraction)
                {
                    struct knapsack_struct temp;
                    temp=item[i];
                    item[i]=item[j];
                    item[j]=temp;
                }
            }
        }
        printf("Sorted Items (According to weight):\n");
        printf("Item:    Profit    Weight\n\n");
        for (int i = 0; i<n; i++) 
        {
            printf("%s        %d        %d \n", item[i].item_name, item[i].profit, item[i].weight);
        }
        printf("\n");
        fp=fopen("entries3.txt","w");
        fclose(fp);
        printf("(profit, weight, bound) for each node:\n");
        Knapsack(W, n);
    }
    else            //if ./createkn01 execute this part
    {
        n = rand() % (8-4+1)+4;
        printf("Selected N items: %d\n\n",n);
        printf("Item:    Profit    Weight    Fraction(Profit/Weight)\n\n");
        //srand(time(0));
        int totalW = 0;
        int totalP = 0;
        for (int i = 0; i<n; i++) 
        {
            sprintf(item[i].item_name,"Item%d",i+1);
            item[i].profit = rand() % (30-10+1)+10;             //assign random number between 10-30
            item[i].weight = rand() % (20-5+1)+5;               //assign random number between 5-20
            item[i].fraction = (double)item[i].profit / (double)item[i].weight;
            totalW += item[i].weight;                       //calculate totalWeight
            totalP += item[i].profit;                       //calculate totalProfit
        }
        for (int i = 0; i<n; i++) 
        {
            printf("%s         %d        %d         %f\n",item[i].item_name, item[i].profit,item[i].weight, item[i].fraction);
        }
        printf("\n");
        int W = floor(0.6*totalW);                          //initialize knapsack capacity

        fp=fopen(argv[2],"w");                  //write data to file specified in command line argument
        fprintf(fp, "%d ", n);  
        fprintf(fp, "%d\n", W);
        for(int i=0;i<n;i++)
        {
            fprintf(fp,"%s ",item[i].item_name);
            fprintf(fp,"%d ",item[i].profit);
            fprintf(fp,"%d\n",item[i].weight);
        }
        fclose(fp);                         //close file descriptor
    }

    return 0;
}