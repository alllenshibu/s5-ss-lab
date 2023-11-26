
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int locctr, prevaddr, stdadr;
char label[20], opcode[20], operand[20];
FILE *optab, *symtab, *intermediate, *object;

int main()
{

    optab = fopen("./../common/optab.txt", "r");

    if (optab == NULL)
    {
        printf("Error: Optab not found\n");
        exit(0);
    }

    symtab = fopen("./../common/symtab.txt", "r");

    if (symtab == NULL)
    {
        printf("Error: Symtab not found\n");
        exit(0);
    }

    intermediate = fopen("./../common/intermediate.txt", "r");

    if (intermediate == NULL)
    {
        printf("Error: Intermediate not found\n");
        exit(0);
    }

    object = fopen("./../common/object.txt", "w");

    if (object == NULL)
    {
        printf("Error: Cannot create object file\n");
        exit(0);
    }

    fscanf(intermediate, "%s %s %d", label, opcode, &stdadr);

    if (strcmp(opcode, "START") != 0)
    {
        printf("Error: No start found\n");
        exit(0);
    }

    printf("\t%s\t%s\t%d\n", label, opcode, stdadr);
    fprintf(object, "H %s %06d %06d\n", label, stdadr, 0);
    fscanf(intermediate, "%d %s %s %s", &locctr, label, opcode, operand);
    prevaddr = locctr;
    char text_record[100];
    strcpy(text_record, "");

    int txt_rec_len = 0;
    while (strcmp(opcode, "END") != 0)
    {
        printf("%d\t%s\t%s\t%s\t", locctr, label, opcode, operand);
        if (txt_rec_len >= 30)
        {
            char overflow[100];
            for (int i = 30; i < txt_rec_len; i++)
            {
                overflow[i - 30] = text_record[i];
            }
            overflow[txt_rec_len - 30] = '\0';
            text_record[30] = '\0';
            txt_rec_len -= 30;
            fprintf(object, "T %06d %02d %s\n", prevaddr, txt_rec_len, text_record);
            strcpy(text_record, overflow);
            txt_rec_len = strlen(overflow);
        }

        if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0)
        {
            // do nothing
        }
        else if (strcmp(opcode, "BYTE") == 0)
        {
            printf("######");
            strcat(text_record, "######");
            txt_rec_len += 6;
        }
        else if (strcmp(opcode, "WORD") == 0)
        {
            // printf("%06s", "operand");
            // while (strlen(operand) < 6)
            // {
            //     strcat(text_record, "0");
            // }
            // strcat(text_record, operand);
            printf("000000");
            strcat(text_record, "000000");
            txt_rec_len += 6;
        }

        else
        {
            bool opcode_found = false;

            char temp[20], machinecode[20];

            rewind(optab);

            while (fscanf(optab, "%s %s", temp, machinecode) != EOF)
            {
                if (strcmp(temp, opcode) == 0)
                {
                    printf("%s", machinecode);
                    strcat(text_record, machinecode);
                    txt_rec_len += 3;
                    opcode_found = true;
                    break;
                }
            }

            if (!opcode_found)
            {
                printf("Error: Invalid opcode %s\n", opcode);
                fclose(symtab);
                fclose(optab);
                exit(0);
            }

            if (strcmp(operand, "-") != 0)
            {
                rewind(symtab);
                char symaddr[20];
                while (fscanf(symtab, "%s %s", temp, &symaddr) != EOF)
                {
                    if (strcmp(temp, operand) == 0)
                    {
                        break;
                    }
                }

                printf("%s", symaddr);
                strcat(text_record, symaddr);
                txt_rec_len += 4;
            }
        }

        printf("\n");
        prevaddr = locctr;
        fscanf(intermediate, "%d %s %s %s", &locctr, label, opcode, operand);
    }

    fprintf(object, "T %06d %02d %s\n", prevaddr, txt_rec_len, text_record);

    fprintf(object, "E %06d", stdadr);

    fclose(symtab);
    fclose(intermediate);
    fclose(object);

    return 0;
}