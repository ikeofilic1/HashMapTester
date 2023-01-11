#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "./utils/timer/timer.h"
#include "./utils/hash_table/hashtable.h"

typedef struct node
{
    char *word;
    char *antonym;
    struct node *next;
} node;

void test_function(FILE *, int, int (*)(char *, int), int (*)(int *, int, hash_table));
void free_node(void *);

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp)
    {
        printf("Could not open file, try again\n");
        return -1;
    }

    int size = 0;
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        if (c == '\n')
            size++;
    }

    printf("Load Factor: %f, elements: %d, size: %d\n\n", DEFAULT_LF, (int)ceil(size * DEFAULT_LF), size);

    // Tests

    //#1
    struct timer obj;
    init_timer(&obj, WALLTIME);

    rewind(fp);
    printf("89^x hash (prime hashing) with linear probing\n");
    start_timer(&obj);
    test_function(fp, size, prime_hash, linear_probe);
    end_timer(&obj);
    print_duration(NULL, &obj);

    rewind(fp);
    printf("\n\n89^x hash (prime hashing) with quadratic probing:\n");
    start_timer(&obj);
    test_function(fp, size, prime_hash, quad_rehash);
    end_timer(&obj);
    print_duration(NULL, &obj);

    //#2
    rewind(fp);
    printf("\n\nAscii with quad probing:\n");
    start_timer(&obj);
    test_function(fp, size, ascii_hash, quad_rehash);
    end_timer(&obj);
    print_duration(NULL, &obj);

    rewind(fp);
    printf("\n\nAscii with linear:\n");
    start_timer(&obj);
    test_function(fp, size, ascii_hash, linear_probe);
    end_timer(&obj);
    print_duration(NULL, &obj);

    rewind(fp);
    printf("\n\nAscii with +5 rehash:\n");
    start_timer(&obj);
    test_function(fp, size, ascii_hash, plus_5);
    end_timer(&obj);
    print_duration(NULL, &obj);

    fclose(fp);
    return 0;
}

void test_function(FILE *fp, int M, int (*hf)(char *, int), int (*rhf)(int *, int, hash_table))
{
    hash_table table = new_table(free_node, M, DEFAULT_LF);

    char *delim = " \n", *token;
    char *line = malloc(51 * sizeof(char));
    size_t size = sizeof line;
    int max = 0, count = 0, count_re = 0, hwc = 0, LC = 0, coll = 0;

    while (getline(&line, &size, fp) != -1)
    {
        token = strtok(line, delim);
        int index = hf(token, table->capacity);
        assert(index < table->capacity);

        if (table->data[index] != NULL)
        { //&& data->word != NULL
            /*if (!strcmp(table[index].data->word, token)) {
                printf("%s has already been used as a key\n", token);
                continue;
            }*/
            int check = rhf(&index, table->capacity, table);
            if (check == -1)
            {
                printf("Something very terrible happened\n");
                continue;
            }
            coll += check;
            if (check > max)
                max = check;

            if (count - LC > hwc)
                hwc = count - LC;
            LC = count;
            count_re++;
        }

        node *nd = malloc(sizeof(node));
        nd->word = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(nd->word, token);

        token = strtok(NULL, delim);
        assert(token != NULL && "Each line should have at least two words seperated by space");

        nd->antonym = malloc((strlen(token) + 1) * sizeof(char));
        strcpy(nd->antonym, token);

        nd->next = NULL;
        table->data[index] = nd;
        count++;
    }

    free(line);
    int probes = coll + count;

    printf("Total number of probes: %d\n", probes);
    printf("Mean number of probes per insertion: %.5f\n", (double)probes / count);
    printf("Total number of collisions: %d\n", coll);
    printf("%.2f%% of all probes are collisions\n", (double)coll / probes * 100);
    printf("Cells with collisions (counting duplicates): %d\n", count_re);
    printf("Longest streak without collisions: %d\n", hwc);
    printf("Max collisions in any one cell: %d\n", max);
    printf("Mean collisions (only counting collided cells): %.5f\n", (double)coll / count_re);

    free_table(table);
}

void free_node(void *_nd)
{
    node *nd = _nd;
    while (nd != NULL)
    {
        // printf("Word: %s Antonym: %s", nd->word, nd->antonym);
        node *temp = nd->next;
        free(nd->word);
        free(nd->antonym);
        free(nd);
        nd = temp;
    }
}
