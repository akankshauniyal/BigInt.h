
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"BigInt.h"


struct node *insert(struct node *head, int val) {
    struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (temp==NULL) {
    printf("Memory Not Allocated!\n");
    exit(1);
  }
  temp->data = val;
  temp->next = head;
  return temp;
}

void insert_tail(struct BigInteger *num, int element) {
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  if (temp == NULL) {
    printf("Memory Not Allocated\n");
    return;
  }
  temp->data = element;
  temp->next = NULL;
  num->length += 1;
  if (num->head == NULL) {
    num->head = temp;
    return;
  }
  struct node *ptr = num->head;
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  ptr->next = temp;
}

long int length(struct node *head) {
  long int c = 0;
  while (head) {
    head = head->next;
    c++;
  }
  return c;
}

void displayLL(struct node *link) {
  if (!link)
    return;
  displayLL(link->next);
  printf("%d", link->data);
}

void display(struct BigInteger b) {
  if (b.head == NULL) {
    printf("0");
    return;
  }
  if (b.sign == '-')
    printf("-");
  displayLL(b.head);
}

struct BigInteger initialize(char *s) {
  struct BigInteger b;
  b.head = NULL;
  if (s[0] == '-')
    b.sign = '-';
  else
    b.sign = '+';
  int i;
  if (b.sign == '-')
    i = 1;
  else
    i = 0;
  int flag = 0;
  while (s[i] == '0' && s[i] != '\0')
    i++;
  for (int j = i; s[j] != '\0'; j++) {
    b.head = insert(b.head, s[j] - '0'); 
    flag = 1;
  }
  if (flag == 0) {
    b.head = insert(b.head, 0);
    b.sign = '+'; 
  }
  b.length = length(b.head);
  return b;
}

struct node *reverse(struct node *head) {
  struct node *next = NULL, *prev = NULL, *current = head;
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
    return -1;
  } else if (num2.length == num1.length) {
    int return_val = 0;
    num1.head = reverse(num1.head);
    num2.head = reverse(num2.head);
    struct node *head1 = num1.head, *head2 = num2.head;
    while (head1 != NULL && head2 != NULL) {
      if (head2->data > head1->data) {
        return_val = -1; 
        break;
      } else if (head1->data > head2->data) { 
        return_val = 1; 
        break;
      }
      head1 = head1->next;
      head2 = head2->next;
    }
    num1.head = reverse(num1.head);
    num2.head = reverse(num2.head);
    return return_val;
  }
  return 1;
}

struct BigInteger add(struct BigInteger num1, struct BigInteger num2) {
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

  struct node *l1 = num1.head, *l2 = num2.head;
  int sum, carry = 0;

  while (l1 != NULL || l2 != NULL || carry != 0) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    sum = d1 + d2 + carry;
    carry = sum / 10;
    insert_tail(&num3, sum % 10);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  
  num3.head = reverse(num3.head);
  while (num3.head != NULL && num3.head->data == 0) {
    struct node *temp = num3.head;
    num3.head = num3.head->next;
    free(temp);
  }
  num3.head = reverse(num3.head);
  num3.length = length(num3.head);
  return num3;
}

struct BigInteger sub(struct BigInteger num1, struct BigInteger num2) {
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
  } else if (compare(num1, num2) == -1) {
    num3.sign = '-'; 
    struct BigInteger temp = num1;
    num1 = num2;
    num2 = temp;
  }

  struct node *l1 = num1.head, *l2 = num2.head;
  int diff, borrow = 0;

  while (l1 != NULL || l2 != NULL) {
    int d1 = (l1 != NULL) ? l1->data : 0;
    int d2 = (l2 != NULL) ? l2->data : 0;

    diff = d1 - d2 - borrow;

    if (diff < 0) {
      diff += 10;
      borrow = 1;
    } else {
      borrow = 0;
    }

    insert_tail(&num3, diff);

    if (l1 != NULL)
      l1 = l1->next;
    if (l2 != NULL)
      l2 = l2->next;
  }
  
  num3.head = reverse(num3.head);
  while (num3.head != NULL && num3.head->data == 0) {
    struct node *temp = num3.head;
    num3.head = num3.head->next;
    free(temp);
  }
  num3.head = reverse(num3.head);
  num3.length = length(num3.head);
  return num3;
}

struct BigInteger mul(struct BigInteger num1, struct BigInteger num2) {
  struct node *n1 = num1.head, *n2 = num2.head;
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
    struct node *current = mid.head;
    while (current != NULL) {
      struct node *temp = current;
      current = current->next;
      free(temp);
    }

    n2 = n2->next;
    i++;
  }
  if (num1.sign != num2.sign) {
    result.sign = '-';
  }
  
  result.head = reverse(result.head);
  while (result.head != NULL && result.head->data == 0) {
    struct node *temp = result.head;
    result.head = result.head->next;
    free(temp);
  }
  result.head = reverse(result.head);
  result.length = length(result.head);
  return result;
}

struct BigInteger div1(struct BigInteger num1, struct BigInteger num2) {
  struct BigInteger num3 = initialize("0");
  num3.length = 0;
  num3.sign = '+';
  char sig = '+';
  int flag = 0;
  if (num2.length == 1 && num2.head->data == 0) {
    printf("Error division by zero\n");
    return num3;
  }
  if (num1.sign != num2.sign)
    sig = '-';
  char t1 = num1.sign;
  char t2 = num2.sign;
  num1.sign = '+';
  num2.sign = '+';
  struct BigInteger temp = add(num2, initialize("0"));
  while (compare(num1, temp) >= 0) {
    temp = add(temp, num2);
    num3 = add(num3, initialize("1"));
    flag = 1;
  }
  if (flag)
    num3.sign = sig;
  num1.sign = t1;
  num2.sign = t2;
  return num3;
}

struct BigInteger mod(struct BigInteger num1, struct BigInteger num2) {
  struct BigInteger num3 = initialize("0");
  num3.length = 0;
  num3.sign = '+';
  char sig = '+';
  int flag = 0;
  if (num2.length == 1 && num2.head->data == 0) {
    printf("Error division by zero\n");
    return num3;
  }
  if (num1.sign != num2.sign)
    sig = '-';
  char t1 = num1.sign;
  char t2 = num2.sign;
  num1.sign = '+';
  num2.sign = '+';
  struct BigInteger temp = add(num2, initialize("0"));
  while (compare(num1, temp) >= 0) {
    temp = add(temp, num2);
    num3 = add(num3, initialize("1"));
  }
    struct BigInteger x = sub(temp, num2);
  struct BigInteger modulus = sub(num1, x);
  num1.sign = t1;
  num2.sign = t2;
  return modulus;
}

