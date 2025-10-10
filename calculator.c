#include<stdio.h>
#include <ctype.h>

int precedence(char op) {
    if (op == '*' || op == '/') 
    {
        return 2;
    }
    if (op == '+' || op == '-') 
    {
        return 1;
    }
    return 0;
}

int calculate(int a, int b, char op) {
    if (op == '+') 
    {
        return a + b;
    }
    if (op == '-') 
    {
        return a - b;
    }
    if (op == '*') 
    {
        return a * b;
    }
    if (op == '/') 
    {
        if (b == 0) 
        { 
            printf("Cannot divide by 0\n");    
            return 0;
        }
        return a / b;        
    }
    return 0;
}

void process(char input[]){
    int i;
    int nums[100], nTop = -1;  
    char op[100], oTop = -1; 
    for(i = 0; input[i] != '\0';){
        if(input[i] == ' ' ||  input[i] == '\n'){
            i++;
        }

        else if(isdigit(input[i])){
            int num = 0;
            while(isdigit(input[i])){
                num = num * 10 + (input[i] - '0');
                i++;
            }
            nums[++nTop] = num;
        }
        else if(input[i]=='+'||input[i]=='-'||input[i]=='*'||input[i]=='/'){
            while(oTop >= 0 && precedence(op[oTop]) >= precedence(input[i])){
                int second = nums[nTop--];
                int first = nums[nTop--];
                int result = calculate(first,second,op[oTop--]);
                if(result == 0){
                    return;
                }
                nums[++nTop] = result;
            }
            op[++oTop] = input[i];
            i++;
        }
        else{
            printf("Invalid  Input\n");
            return;
        }
    }
    while (oTop >= 0) {
        int second = nums[nTop--];
        int first = nums[nTop--];
        int result = calculate(first, second, op[oTop--]);
        nums[++nTop] = result;
    }    

    printf("\nAnswer is : %d",nums[nTop]);

}

int main(){
    char input[100];
    fgets(input,100,stdin);
    //printf("%s",input);
    process(input);
    return 0;
}

