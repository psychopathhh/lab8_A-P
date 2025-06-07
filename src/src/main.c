#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int coefficient;
    int exponent;
    struct Node* next;
} Polynomial;


Polynomial* create_term(int coeff, int exp) {
    Polynomial* new_term = (Polynomial*)malloc(sizeof(Polynomial));
    if (new_term == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }
    new_term->coefficient = coeff;
    new_term->exponent = exp;
    new_term->next = NULL;
    return new_term;
}

void add_term(Polynomial** poly, int coeff, int exp) {
    if (coeff == 0) return; // не добавляем нулевые коэффициенты

    Polynomial* new_term = create_term(coeff, exp);

    // Если список пуст или новый элемент имеет большую степень, чем первый
    if (*poly == NULL || (*poly)->exponent < exp) {
        new_term->next = *poly;
        *poly = new_term;
    }
    else {
        Polynomial* current = *poly;
        Polynomial* prev = NULL;

        // Ищем место для вставки
        while (current != NULL && current->exponent > exp) {
            prev = current;
            current = current->next;
        }

        // Если нашли элемент с такой же степенью, складываем коэффициенты
        if (current != NULL && current->exponent == exp) {
            current->coefficient += coeff;
            free(new_term);
            // Если сумма коэффициентов стала нулевой, удаляем элемент
            if (current->coefficient == 0) {
                if (prev == NULL) {
                    *poly = current->next;
                }
                else {
                    prev->next = current->next;
                }
                free(current);
            }
        }
        else {
            // Вставляем новый элемент
            new_term->next = current;
            if (prev == NULL) {
                *poly = new_term;
            }
            else {
                prev->next = new_term;
            }
        }
    }
}


Polynomial* create_polynomial() {
    Polynomial* poly = NULL;
    int coeff, exp;
    char choice;
    int input_result;

    printf("Creating a polynomial (enter coefficients and exponents):\n");

    do {
        printf("Enter coefficient and exponent (e.g., 2 3 for 2x^3): ");
        input_result = scanf("%d %d", &coeff, &exp);


        while (input_result != 2) {
            // Очистка буфера ввода
            while (getchar() != '\n');

            printf("Input error! Enter two integers separated by space: ");
            input_result = scanf("%d %d", &coeff, &exp);
        }

        add_term(&poly, coeff, exp);


        printf("Add another term? (y/n): ");
        scanf(" %c", &choice);

        while (choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N') {
            // Очистка буфера ввода
            while (getchar() != '\n');

            printf("Input error! Enter 'y' to continue or 'n' to finish: ");
            scanf(" %c", &choice);
        }

    } while (choice == 'y' || choice == 'Y');

    return poly;
}

void print_polynomial(Polynomial* poly) {
    if (poly == NULL) {
        printf("0");
        return;
    }

    Polynomial* current = poly;
    int first_term = 1;

    while (current != NULL) {
        if (!first_term) {
            if (current->coefficient > 0) {
                printf(" + ");
            }
            else {
                printf(" - ");
            }
        }
        else {
            if (current->coefficient < 0) {
                printf("-");
            }
            first_term = 0;
        }

        int abs_coeff = abs(current->coefficient);
        if (abs_coeff != 1 || current->exponent == 0) {
            printf("%d", abs_coeff);
        }

        // Вывод переменной и степени
        if (current->exponent > 0) {
            printf("x");
            if (current->exponent > 1) {
                printf("^%d", current->exponent);
            }
        }

        current = current->next;
    }
    printf("\n");
}


Polynomial* add_polynomials(Polynomial* poly1, Polynomial* poly2) {
    Polynomial* result = NULL;

    Polynomial* current = poly1;
    while (current != NULL) {
        add_term(&result, current->coefficient, current->exponent);
        current = current->next;
    }


    current = poly2;
    while (current != NULL) {
        add_term(&result, current->coefficient, current->exponent);
        current = current->next;
    }

    return result;
}


void free_polynomial(Polynomial* poly) {
    Polynomial* current = poly;
    while (current != NULL) {
        Polynomial* temp = current;
        current = current->next;
        free(temp);
    }
}

// Часть 2: Работа со строками (двусвязный список)

typedef struct WordNode {
    char* word;
    struct WordNode* prev;
    struct WordNode* next;
} WordNode;


WordNode* create_word_node(const char* word) {
    WordNode* new_node = (WordNode*)malloc(sizeof(WordNode));
    if (new_node == NULL) {
        printf("Memory allocation error\n");
        exit(1);
    }

    new_node->word = strdup(word);
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}


void add_word(WordNode** head, WordNode** tail, const char* word) {
    WordNode* new_node = create_word_node(word);

    if (*head == NULL) {
        *head = *tail = new_node;
    }
    else {
        (*tail)->next = new_node;
        new_node->prev = *tail;
        *tail = new_node;
    }
}


WordNode* create_word_list(const char* input) {
    WordNode* head = NULL;
    WordNode* tail = NULL;

    char buffer[256];
    int buffer_index = 0;

    for (int i = 0; input[i] != '.' && input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            if (buffer_index > 0) {
                buffer[buffer_index] = '\0';
                add_word(&head, &tail, buffer);
                buffer_index = 0;
            }
        }
        else {
            buffer[buffer_index++] = input[i];
        }
    }

    // Добавляем последнее слово, если оно есть
    if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        add_word(&head, &tail, buffer);
    }

    return head;
}

void print_word_list(WordNode* head) {
    WordNode* current = head;
    printf("[");
    while (current != NULL) {
        printf("\"%s\"", current->word);
        if (current->next != NULL) {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}

int is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y';
}


void remove_vowel_words(WordNode** head, WordNode** tail) {
    WordNode* current = *head;

    while (current != NULL) {
        WordNode* next = current->next;

        if (current->word[0] != '\0' && is_vowel(current->word[0])) {
            // Удаляем узел
            if (current->prev != NULL) {
                current->prev->next = current->next;
            }
            else {
                *head = current->next;
            }

            if (current->next != NULL) {
                current->next->prev = current->prev;
            }
            else {
                *tail = current->prev;
            }

            free(current->word);
            free(current);
        }

        current = next;
    }
}


void free_word_list(WordNode* head) {
    WordNode* current = head;
    while (current != NULL) {
        WordNode* temp = current;
        current = current->next;
        free(temp->word);
        free(temp);
    }
}

int main() {
    printf("=== Part 1: Polynomial operations ===\n");

    printf("\nCreating first polynomial:\n");
    Polynomial* poly1 = create_polynomial();
    printf("First polynomial: ");
    print_polynomial(poly1);

    printf("\nCreating second polynomial:\n");
    Polynomial* poly2 = create_polynomial();
    printf("Second polynomial: ");
    print_polynomial(poly2);

    Polynomial* sum = add_polynomials(poly1, poly2);
    printf("\nSum of polynomials: ");
    print_polynomial(sum);

    free_polynomial(poly1);
    free_polynomial(poly2);
    free_polynomial(sum);

    printf("\n=== Part 2: String operations ===\n");

    /*char input[256];
    printf("\nEnter a string of lowercase Latin letters (end with a dot):\n");

    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);

    fgets(input, sizeof(input), stdin);*/

    char input[256];
    int valid_input = 0;
    //ИИшные проверки.....
    while (!valid_input) {
        printf("\nEnter a string of lowercase Latin letters (end with a dot):\n");
        printf("Example: 'hello world example.'\n");
        printf("Rules:\n");
        printf("1. Only lowercase letters a-z and single spaces allowed\n");
        printf("2. Must end with a dot\n");
        printf("3. No multiple spaces between words\n");
        printf("4. Cannot start or end with space before dot\n\n");

        // Очищаем буфер ввода
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Получаем ввод
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input. Please try again.\n");
            continue;
        }

        // Удаляем символ новой строки
        input[strcspn(input, "\n")] = '\0';

        // Проверка 1: Должна заканчиваться точкой
        if (input[strlen(input) - 1] != '.') {
            printf("Error: Input must end with a dot! Your input: '%s'\n", input);
            continue;
        }

        // Проверка 2: Только строчные буквы и пробелы
        int invalid_char = 0;
        for (int i = 0; input[i] != '.' && input[i] != '\0'; i++) {
            if (!(islower(input[i]) || input[i] == ' ')) {
                invalid_char = 1;
                break;
            }
        }
        if (invalid_char) {
            printf("Error: Only lowercase letters and spaces allowed before dot!\n");
            continue;
        }

        // Проверка 3: Нет двойных пробелов
        int double_space = 0;
        for (int i = 1; input[i] != '.' && input[i] != '\0'; i++) {
            if (input[i] == ' ' && input[i - 1] == ' ') {
                double_space = 1;
                break;
            }
        }
        if (double_space) {
            printf("Error: Multiple spaces between words are not allowed!\n");
            continue;
        }

        // Проверка 4: Не начинается с пробела
        if (input[0] == ' ') {
            printf("Error: Input cannot start with space!\n");
            continue;
        }

        // Проверка 5: Точка не после пробела
        if (strlen(input) > 1 && input[strlen(input) - 2] == ' ') {
            printf("Error: Dot cannot follow a space! Remove space before dot.\n");
            continue;
        }

        // Проверка 6: Не пустая строка (только ".")
        if (strlen(input) == 1 && input[0] == '.') {
            printf("Error: Empty string! Please enter some words before dot.\n");
            continue;
        }
        valid_input = 1;
    }

    // Здесь продолжение работы с корректным input
    printf("Valid input received: '%s'\n", input);

    WordNode* word_head = create_word_list(input);
    WordNode* word_tail = word_head;
    while (word_tail != NULL && word_tail->next != NULL) {
        word_tail = word_tail->next;
    }

    printf("\nOriginal word list: ");
    print_word_list(word_head);

    remove_vowel_words(&word_head, &word_tail);
    printf("List after removing words starting with vowel: ");
    print_word_list(word_head);

    free_word_list(word_head);

    return 0;
}