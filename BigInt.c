
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"BigInt.h"

struct node{
    int data;
    struct node* next;
};

struct BigInteger{
    struct node* head;
    long int length;
    char sign;
};

struct node* createNode(int val);
struct node* insert(struct node* head, int val);
void insert_tail(struct BigInteger* num, int element);
long int length(struct node* head);
void displayLL(struct node* link);
void display(struct BigInteger b);
struct BigInteger initialize(char* s);
struct node* reverse(struct node* head);
int compare(struct BigInteger num1, struct BigInteger num2);
struct BigInteger add(struct BigInteger num1, struct BigInteger num2);
struct BigInteger sub(struct BigInteger num1, struct BigInteger num2);
struct BigInteger mul(struct BigInteger num1, struct BigInteger num2);
void freeL(struct BigInteger* num);


struct node* createNode(int val) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = val;
    newNode->next = NULL;
    return newNode;
}

struct node* insert(struct node* head, int val){
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    if(temp==NULL){
        printf("Memory Allocation failed!");
        exit(1);
    }
    temp->data = val;
    temp->next = head;
    return temp;
}

void insert_tail(struct BigInteger* num, int element) {
    struct node* temp = (struct node*)malloc(sizeof(struct node));
    if (temp == NULL) {
        printf("memory allocation failed\n");
        return;
    }
    temp->data = element;
    temp->next = NULL;
    num->length = num->length +1;
    if (num->head == NULL) {
        num->head = temp;
        return;
    }
    struct node* ptr = num->head;
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = temp;
}

long int length(struct node* head) {
    long int c = 0;
    while (head) {
        head = head->next;
        c++;
    }
    return c;
}

void displayLL(struct node* link){
    if(link==NULL)
        return;
    displayLL(link->next);
    printf("%d", link->data);
}

void display(struct BigInteger b){
    if(b.head == NULL) {
        printf("0");
        return;
    }
    if(b.sign == '-')
        printf("-");
    displayLL(b.head);
}

struct BigInteger initialize(char* s){
    struct BigInteger b;
    int i;
    b.head = NULL;
    if(s[0] == '-')
        b.sign = '-';
    else
        b.sign = '+';
    if(b.sign == '-')
        i = 1;
    else
        i = 0 ;
    for(i; s[i] != '\0'; i++){
        b.head = insert(b.head, s[i] - '0'); 
    }
    b.length = length(b.head);
    return b;
}

struct BigInteger add(struct BigInteger num1, struct BigInteger num2){
    struct BigInteger num3;
    num3.head = NULL;
    num3.sign = '+';
    num3.length = 0;

    if (num1.sign == '-' && num2.sign == '-') {
        num3.sign = '-';
    } else if (num1.sign == '-') {
        num1.sign = '+';
        num3 = sub(num2, num1);
        num1.sign = '-';
        return num3;
    } else if (num2.sign == '-') {
        num2.sign = '+';
        num3 = sub(num1, num2);
        num2.sign = '-';
        return num3;
    }

    struct node* p1 = num1.head, * p2 = num2.head;
    int sum=0, carry = 0;

    while (p1 != NULL || p2 != NULL || carry != 0) {
        int val1 = (p1 != NULL) ? p1->data : 0;
        int val2 = (p2 != NULL) ? p2->data : 0;

        sum = val1 + val2 + carry;
        carry = sum / 10;
        insert_tail(&num3, sum % 10);

        if (p1 != NULL)
{ p1 = p1->next;
}
        if (p2 != NULL) {
p2 = p2->next;
}
    }

    return num3;
}



struct node* reverse(struct node* head) {
    struct node* prev = NULL, * next= NULL, * current = head;
    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    return prev;
}

int compare(struct BigInteger num1, struct BigInteger num2) {
    if (num2.length > num1.length) {
        return 1;
    } 
    else if (num2.length == num1.length) {
        int store = 0;
        num1.head = reverse(num1.head);
        num2.head = reverse(num2.head);
        struct node* p1 = num1.head, * p2 = num2.head;
        while (p1 != NULL && p2 != NULL) {
            if (p2->data > p1->data) {
                store = 1;
                break;
            } 
            else if (p1->data > p2->data) {
                store= 0;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        num1.head = reverse(num1.head);
        num2.head = reverse(num2.head);
        return store;
    }
    return 0;
}

struct BigInteger sub(struct BigInteger num1, struct BigInteger num2){
    struct BigInteger num3;
    num3.head = NULL;
    num3.sign = '+';
    num3.length = 0;


    if (num1.sign == '-' && num2.sign == '+') {
        num1.sign = '+';
        num3 = add(num1, num2);
        num1.sign = '-';
        num3.sign = '-';
        return num3;
    } else if (num1.sign == '+' && num2.sign == '-') {
        num2.sign = '+';
        num3 = add(num1, num2);
        num2.sign = '-';
        return num3;
    } else if (num1.sign == '-' && num2.sign == '-') {
        num2.sign = '+';
        num1.sign = '+';
        num3 = sub(num2, num1);
        num2.sign = '-';
        num1.sign = '-';
        return num3;
    }
    else if (compare(num1, num2)) {
        struct node* temp = num1.head;
        num1.head = num2.head;
        num2.head = temp;
        num3.sign = '-';
    }

    struct node* p1 = num1.head, * p2 = num2.head;
    int diff, borrow = 0;

    while (p1 != NULL || p2 != NULL) {
        int d1 = (p1 != NULL) ? p1->data : 0;
        int d2 = (p2 != NULL) ? p2->data : 0;

        diff = d1 - d2 - borrow;

        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        insert_tail(&num3, diff);

        if (p1 != NULL) p1 = p1->next;
        if (p2 != NULL) p2 = p2->next;
    }


    while (num3.head != NULL && num3.head->data == 0) {
        struct node* temp = num3.head;
        num3.head = num3.head->next;
        free(temp);
    }
    return num3;
}

struct BigInteger mul(struct BigInteger num1, struct BigInteger num2){
    struct node* n1 = num1.head, * n2 = num2.head;
    struct BigInteger num3 = initialize(""); 
    num3.length = 0;
    if (n1 == NULL || n2 == NULL) {
        return num3;
    }
    struct BigInteger mid, result = initialize(""); 
    int carry = 0, product = 0, i = 0;
    while (n2 != NULL) {
        n1 = num1.head;
        carry = 0;
        mid = initialize("");
        for (int j = 0; j < i; j++) {
            insert_tail(&mid, 0);
        }
        while (n1 != NULL) {
            product = (n1->data) * (n2->data) + carry;
            insert_tail(&mid, product % 10);
            carry = product / 10;
            n1 = n1->next;
        }

        if (carry > 0) {
            insert_tail(&mid, carry);
        }

        result = add(mid, result);
        struct node* current = mid.head;
        freeL(&mid);

        n2 = n2->next;
        i++;
    }
    if (num1.sign != num2.sign) {
        result.sign = '-';
    }
    return result;
}

struct BigInteger div1(struct BigInteger dividend, struct BigInteger divisor) {
    
    struct BigInteger zero = initialize("0");
    if (compare(divisor, zero) == 0) {
        printf("Error: Division by zero.\n");
       
    }

    
    struct BigInteger quotient = initialize("0");
    quotient.sign = (dividend.sign == divisor.sign) ? '+' : '-';

    
    dividend.sign = '+';
    divisor.sign = '+';

   
    struct BigInteger current = initialize("0");
    struct BigInteger one = initialize("1");
    struct BigInteger ten = initialize("10");

    long int i;
    for (i = dividend.length - 1; i >= 0; i--) {
        current = mul(current, ten);
        current = add(current, initialize(&dividend.head[i]));

        int count = 0;
        while (compare(current, divisor) >= 0) {
            current = sub(current, divisor);
            count++;
        }

        insert_tail(&quotient, count);
    }

    
    while (quotient.head->data == 0 && quotient.length > 1) {
        struct node* temp = quotient.head;
        quotient.head = temp->next;
        free(temp);
        quotient.length--;
    }

    
    dividend.sign = quotient.sign;
    divisor.sign = quotient.sign;

    return quotient;
}



void freeL(struct BigInteger* num) {
    struct node* ptr = num->head;
    while (ptr != NULL) {
        struct node* temp = ptr;
        ptr = ptr->next;
        free(temp);
    }
    num->head = NULL;
    num->length = 0;
}


int main() {
    struct BigInteger num0 = initialize("12");
    struct BigInteger num01 = initialize("-5");
    
    struct BigInteger sum0 = add(num0, num01);
    struct BigInteger diff0 = sub(num0, num01);
    struct BigInteger mul0 = mul(num0, num01);

    printf("Case 0:\n");
    printf("First number: ");
    display(num0);
    printf("\n");
    printf("Second number: ");
    display(num01);
    printf("\n");
    printf("Addition: ");
    display(sum0);
    printf("\n");
    printf("Subtraction: ");
    display(diff0);
    printf("\n");
    printf("Multiplication: ");
    display(mul0);
    printf("\n\n");
    struct BigInteger num1 = initialize("432");
    struct BigInteger num2 = initialize("128");
    
    struct BigInteger sum1 = add(num1, num2);
    struct BigInteger diff1 = sub(num1, num2);
    struct BigInteger mul1 = mul(num1, num2);
    struct BigInteger div=div1(num1,num2);

    printf("Case 1:\n");
    printf("First number: ");
    display(num1);
    printf("\n");
    printf("Second number: ");
    display(num2);
    printf("\n");
    printf("Addition: ");
    display(sum1);
    printf("\n");
    printf("Subtraction: ");
    display(diff1);
    printf("\n");
    printf("Multiplication: ");
    display(mul1);
    printf("\n");
    printf("Division: ");
    display(div);
    printf("\n\n");

    struct BigInteger num3 = initialize("1234");
    struct BigInteger num4 = initialize("-5678");
    
    struct BigInteger sum2 = add(num3, num4);
    struct BigInteger diff2 = sub(num3, num4);
    struct BigInteger mul2 = mul(num3, num4);
    
    struct BigInteger div2=div1(num3,num4);

    printf("Case 1:\n");
    printf("First number: ");
    display(num3);
    printf("\n");
    printf("Second number: ");
    display(num4);
    printf("\n");
    printf("Addition: ");
    display(sum2);
    printf("\n");
    printf("Subtraction: ");
    display(diff2);
    printf("\n");
    printf("Multiplication: ");
    display(mul2);
    printf("\n");
    printf("Division: ");
    display(div2);
    printf("\n\n");
    struct BigInteger num5 = initialize("-1234");
    struct BigInteger num6 = initialize("-5678");
    
    struct BigInteger sum3 = add(num5, num6);
    struct BigInteger diff3 = sub(num5, num6);
    struct BigInteger mul3 = mul(num5, num6);
    
    printf("Case 3:\n");
    printf("First number: ");
    display(num5);
    printf("\n");
    printf("Second number: ");
    display(num6);
    printf("\n");
    printf("Addition: ");
    display(sum3);
    printf("\n");
    printf("Subtraction: ");
    display(diff3);
    printf("\n");
    printf("Multiplication: ");
    display(mul3);
    printf("\n\n");

    struct BigInteger num7 = initialize("5678");
    struct BigInteger num8 = initialize("123456");
    
    struct BigInteger sum4 = add(num7, num8);
    struct BigInteger diff4 = sub(num7, num8);
    struct BigInteger mul4 = mul(num7, num8);

    printf("Case 4:\n");
    printf("First number: ");
    display(num7);
    printf("\n");
    printf("Second number: ");
    display(num8);
    printf("\n");
    printf("Addition: ");
    display(sum4);
    printf("\n");
    printf("Subtraction: ");
    display(diff4);
    printf("\n");
    printf("Multiplication: ");
    display(mul4);
    printf("\n\n");
    
    struct BigInteger num9 = initialize("123456");
    struct BigInteger num10 = initialize("5678");
    
    struct BigInteger sum5 = add(num9, num10);
    struct BigInteger diff5 = sub(num9, num10);
    struct BigInteger mul5 = mul(num9, num10);

    printf("Case 5:\n");
    printf("First number: ");
    display(num9);
    printf("\n");
    printf("Second number: ");
    display(num10);
    printf("\n");
    printf("Addition: ");
    display(sum5);
    printf("\n");
    printf("Subtraction: ");
    display(diff5);
    printf("\n");
    printf("Multiplication: ");
    display(mul5);
    printf("\n\n");
    
    struct BigInteger num11 = initialize("2982763498726985692852592635928698263982639462396326286598265988754328475295492598265498126982691845698126598260451");
    struct BigInteger num12 = initialize("-3740594857619846598164958619843598543596745963498591285491459814298519843598149851984359814893");
    
    struct BigInteger sum6 = add(num11, num12);
    struct BigInteger diff6 = sub(num11, num12);
    struct BigInteger mul6 = mul(num11, num12);

    printf("Case 6:\n");
    printf("First number: ");
    display(num11);
    printf("\n");
    printf("Second number: ");
    display(num12);
    printf("\n");
    printf("Addition: ");
    display(sum6);
    printf("\n");
    printf("Subtraction: ");
    display(diff6);
    printf("\n");
    printf("Multiplication: ");
    display(mul6);
    printf("\n\n");

    return 0;
}