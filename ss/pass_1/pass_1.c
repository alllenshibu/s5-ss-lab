#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

FILE *input, *optab, *symtab, *output;
int stdadr, length;
int locctr;
char label[20], opcode[20], operand[20];

int main()
{
    input = fopen("./../common/input.asm", "r");

    if (input == NULL)
    {
        printf("Error: Input file not found\n");
        exit(0);
    }

    symtab = fopen("./../common/symtab.txt", "w");

    if (symtab == NULL)
    {
        printf("Error: Cannot create symtab\n");
        fclose(input);
        exit(0);
    }

    optab = fopen("./../common/optab.txt", "r");

    if (optab == NULL)
    {
        printf("Error: Optab file not found\n");
        fclose(symtab);
        fclose(input);
        exit(0);
    }

    output = fopen("./../common/intermediate.txt", "w");

    if (optab == NULL)
    {
        printf("Error: Cannot create output file\n");
        fclose(optab);
        fclose(symtab);
        fclose(input);
        exit(0);
    }

    fscanf(input, "%s %s %d", label, opcode, &stdadr);

    if (strcmp(opcode, "START") == 0)
    {
        locctr = stdadr;
        fprintf(output, "\t%s\t%s\t%d\n", label, opcode, stdadr);
        printf("\t%s\t%s\t%d\n", label, opcode, stdadr);
    }
    else
    {
        locctr = 0;
    }

    while (fscanf(input, "%s %s %s", label, opcode, operand) != EOF)
    {
        fprintf(output, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        printf("%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

        if (strcmp(label, "-") != 0)
        {
            fprintf(symtab, "%s\t%d\n", label, locctr);
        }

        if (strcmp(opcode, "END") == 0)
        {
            printf("Completed\n");
            break;
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            locctr += 3;
        }
        else if (strcmp(opcode, "RESW") == 0)
        {
            locctr += (3 * atoi(operand));
        }
        else if (strcmp(opcode, "RESB") == 0)
        {
            locctr += atoi(operand);
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            locctr += 1;
        }
        else
        {
            bool opcode_found = false;

            char temp[20];

            rewind(optab);

            while (fscanf(optab, "%s %X", temp, &length) != EOF)
            {
                if (strcmp(temp, opcode) == 0)
                {
                    locctr += 3;
                    opcode_found = true;
                    break;
                }
            }

            if (!opcode_found)
            {
                printf("Error: Invalid opcode %s\n", opcode);
                fclose(symtab);
                fclose(optab);
                fclose(input);
                exit(0);
            }
        }
    }

    fclose(optab);
    fclose(input);
    fclose(symtab);

    printf("Length of program = %d\n", locctr - stdadr);

    return 0;
}
