#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 309

struct bigInt {
    int size;
    int sign;
    char* val;
};

void print(struct bigInt* data);
void swap(struct bigInt* x, struct bigInt* y);
void init(struct bigInt* num, int n, int size);
int compare(struct bigInt* num1, struct bigInt* num2);
struct bigInt convert(char s[SIZE + 2]);
struct bigInt add(struct bigInt* num1, struct bigInt* num2);
struct bigInt subtract(struct bigInt* num1, struct bigInt* num2);
struct bigInt multiply(struct bigInt* num1, struct bigInt* num2);

void print(struct bigInt* data) {
    if (data->sign == 1)
        printf("-");
    for (int i = data->size - 1; i >= 0; i--)
        printf("%d", data->val[i]);
    printf("\n");
}

void swap(struct bigInt* x, struct bigInt* y) {
    struct bigInt temp = *x;
    *x = *y;
    *y = temp;
}

void init(struct bigInt* num, int n, int size) {
    num->size = size;
    num->val = (char*)malloc(n * sizeof(char));
    num->sign = 0;
}

struct bigInt convert(char s[SIZE + 2]) {
    struct bigInt num;
    init(&num, SIZE, strlen(s));
    int i = 0, j = strlen(s) - 1;
    if (s[0] == '-') {
        num.size--;
        num.sign = 1;
    } else {
        num.sign = 0;
    }
    while (i < num.size) {
        num.val[i] = s[j] - '0';
        i++;
        j--;
    }
    return num;
}

int compare(struct bigInt* num1, struct bigInt* num2) {
    if (num1->size > num2->size)
        return 1;
    else if (num1->size < num2->size)
        return -1;
    for (int i = num1->size - 1; i >= 0; i--) {
        if (num1->val[i] > num2->val[i])
            return 1;
        if (num1->val[i] < num2->val[i])
            return -1;
    }
    return 0;
}

struct bigInt add(struct bigInt* num1, struct bigInt* num2) {
    struct bigInt total;
    init(&total, SIZE, 0);
    if (num1->sign != num2->sign) {
        num2->sign = !num2->sign;
        total = subtract(num1, num2);
        num2->sign = !num2->sign;
        return total;
    }
    int i = 0, carry = 0;
    while (i < num1->size || i < num2->size || carry != 0) {
        int sum = carry;
        if (i < num1->size) sum += num1->val[i];
        if (i < num2->size) sum += num2->val[i];
        total.val[i] = sum % 10;
        carry = sum / 10;
        ++i;
    }
    total.size = i;
    total.sign = num1->sign;
    return total;
}

struct bigInt subtract(struct bigInt* num1, struct bigInt* num2) {
    struct bigInt total;
    init(&total, SIZE, 0);
    if (num1->sign != num2->sign) {
        num2->sign = !num2->sign;
        total = add(num1, num2);
        num2->sign = !num2->sign;
        return total;
    }
    int cmp = compare(num1, num2);
    if (cmp < 0) {
        swap(num1, num2);
        total.sign = 1;
    } else {
        total.sign = 0;
    }
    int i = 0, borrow = 0;
    while (i < num1->size) {
        int diff = num1->val[i] - (i < num2->size ? num2->val[i] : 0) - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        total.val[i] = diff;
        ++i;
    }
    while (i > 1 && total.val[i - 1] == 0)
        --i;
    total.size = i;
    return total;
}

struct bigInt multiply(struct bigInt* num1, struct bigInt* num2) {
    struct bigInt total;
    init(&total, 2 * SIZE, 0);
    for (int i = 0; i < num2->size; i++) {
        int carry = 0;
        for (int j = 0; j < num1->size || carry != 0; j++) {
            int prod = total.val[i + j] +
                       (i < num2->size && j < num1->size ? num2->val[i] * num1->val[j] : 0) +
                       carry;
            total.val[i + j] = prod % 10;
            carry = prod / 10;
        }
    }
    total.size = num1->size + num2->size;
    while (total.size > 1 && total.val[total.size - 1] == 0)
        total.size--;
    total.sign = num1->sign ^ num2->sign;
    return total;
}

int main() {
    char num1Str[SIZE + 2], num2Str[SIZE + 2];
    printf("Enter first number: ");
    scanf("%s", num1Str);
    printf("Enter second number: ");
    scanf("%s", num2Str);
    struct bigInt num1 = convert(num1Str);
    struct bigInt num2 = convert(num2Str);
    printf("First Number: ");
    print(&num1);
    printf("Second Number: ");
    print(&num2);
    struct bigInt sum = add(&num1, &num2);
    printf("Sum: ");
    print(&sum);
    struct bigInt diff = subtract(&num1, &num2);
    printf("Difference: ");
    print(&diff);
    struct bigInt prod = multiply(&num1, &num2);
    printf("Product: ");
    print(&prod);
    return 0;
}
