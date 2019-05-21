#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char **argv)
{
    int rank, ranksent, size, source, dest, tag, i, j, k;
    MPI_Status status;
    struct rekord
    {
        double identyfikator;
        char plec;
        char imie[6];
        char nazwisko[6];
    };

    struct rekord baza[20], baza1[20];
    MPI_Datatype rekord1_typ, rekord2_typ;
    int tab_dlug_blokow[4] = {1, 1, 6, 6};

    MPI_Datatype tab_typow[4] = {MPI_DOUBLE, MPI_CHAR, MPI_CHAR, MPI_CHAR};
    MPI_Aint lb, zasieg, podstawa, tab_odstepow[4], temp;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Get_address(&baza[0].identyfikator, &tab_odstepow[0]);
    MPI_Get_address(&baza[0].plec, &tab_odstepow[1]);
    MPI_Get_address(&baza[0].imie[0], &tab_odstepow[2]);
    MPI_Get_address(&baza[0].nazwisko[0], &tab_odstepow[3]);

    if (rank == 0)
    { /*
      printf("adres identyfikator \t %ld\n", tab_odstepow[0]);
      printf("adres plec \t %ld\n", tab_odstepow[1]);
      printf("adres imie \t %ld\n", tab_odstepow[2]);
      printf("adres nazwisko \t %ld\n\n", tab_odstepow[3]);
      */
    }

    double start = MPI_Wtime();

    for (k = 0; k < 1; k++)
    {
        podstawa = tab_odstepow[0];
        for (i = 0; i < 4; i++) {
            tab_odstepow[i] -= podstawa;
        }

        MPI_Type_struct(4, tab_dlug_blokow, tab_odstepow, tab_typow, &rekord1_typ);
        MPI_Type_get_extent(rekord1_typ, &lb, &zasieg);

        if (rank == 0)
        {
            //printf("rekord1_typ - lb = %ld, zasieg = %ld\n\n", lb, zasieg);
        }

        MPI_Type_create_resized(rekord1_typ, lb, zasieg, &rekord2_typ);
        MPI_Type_commit(&rekord2_typ);

        if (rank != 0)
        {
            dest = 0;
            tag = 0;

            baza[2 * rank].identyfikator = rank;
            baza[2 * rank].plec = 'M';
            strcpy(baza[2 * rank].imie, "Janek");
            strcpy(baza[2 * rank].nazwisko, "Kowal");

            baza[2 * rank + 1].identyfikator = rank;
            baza[2 * rank + 1].plec = 'K';
            strcpy(baza[2 * rank + 1].imie, "Agata");
            strcpy(baza[2 * rank + 1].nazwisko, "Nowak");

            //printf("sending: %lf, %c, %s, %s\n", baza[2*rank].identyfikator, baza[2*rank].plec, baza[2*rank].imie, baza[2*rank].nazwisko);
            //printf("sending: %lf, %c, %s, %s\n", baza[2*rank+1].identyfikator, baza[2*rank+1].plec, baza[2*rank+1].imie, baza[2*rank+1].nazwisko);

            MPI_Send(&baza[2 * rank], 2, rekord2_typ, dest, tag, MPI_COMM_WORLD);
            MPI_Send(&baza[2 * rank + 1], 2, rekord2_typ, dest, tag, MPI_COMM_WORLD);
        }
        else
        {
            for (i = 1; i < size; i++)
            {
                MPI_Recv(&baza[2 * i], 2, rekord2_typ, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                //printf("Dane od procesu o randze: %d\n", status.MPI_SOURCE );
                //printf("receiving: %lf, %c, %s, %s\n", baza[2*i].identyfikator, baza[2*i].plec, baza[2*i].imie, baza[2*i].nazwisko);
                MPI_Recv(&baza[2 * i + 1], 2, rekord2_typ, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                //printf("receiving: %lf, %c, %s, %s\n", baza[2*i+1].identyfikator, baza[2*i+1].plec, baza[2*i+1].imie, baza[2*i+1].nazwisko);
            }
        }
    }
    double end1 = (MPI_Wtime() - start) / 2;
    start = MPI_Wtime();

    for (k = 0; k < 1; k++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0)
        {
            //printf("\nPrzeslanie typu spakowanego\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
        {
            int rozm, rozm_pakietu, rozmiar_komunikatu, pozycja;
            void *bufor;
            MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &rozm);
            rozm_pakietu = rozm;
            MPI_Pack_size(1, MPI_CHAR, MPI_COMM_WORLD, &rozm);
            rozm_pakietu += rozm;
            MPI_Pack_size(6, MPI_CHAR, MPI_COMM_WORLD, &rozm);
            rozm_pakietu += rozm;
            MPI_Pack_size(6, MPI_CHAR, MPI_COMM_WORLD, &rozm);
            rozm_pakietu += rozm;

            rozmiar_komunikatu = 2 * rozm_pakietu;
            bufor = (void *)malloc(rozmiar_komunikatu);

            if (rank != 0)
            {
                dest = 0;
                tag = 0;

                pozycja = 0;
                for (i = 2 * rank; i <= 2 * rank + 1; i++)
                {
                    MPI_Pack(&baza[i].identyfikator, 1, MPI_DOUBLE, bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
                    MPI_Pack(&baza[i].plec, 1, MPI_CHAR, bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
                    MPI_Pack(&baza[i].imie[0], 6, MPI_CHAR, bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
                    MPI_Pack(&baza[i].nazwisko[0], 6, MPI_CHAR, bufor, rozmiar_komunikatu, &pozycja, MPI_COMM_WORLD);
                    //printf("packing: %lf, %c, %s, %s\n", baza[i].identyfikator, baza[i].plec, baza[i].imie, baza[i].nazwisko);
                }
                //printf("sending, rozmiar %d (%d / rekord)\n", pozycja, pozycja/2);
                MPI_Send(bufor, pozycja, MPI_PACKED, dest, tag, MPI_COMM_WORLD);
            }
            else
            {

                for (j = 1; j < size; j++)
                {
                    MPI_Recv(bufor, rozmiar_komunikatu, MPI_PACKED, j, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    //printf("Dane od procesu o randze: %d\n", status.MPI_SOURCE );

                    pozycja = 0;
                    for (i = 2 * j; i <= 2 * j + 1; i++)
                    {
                        MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja, &baza1[i].identyfikator, 1, MPI_DOUBLE, MPI_COMM_WORLD);
                        MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja, &baza1[i].plec, 1, MPI_CHAR, MPI_COMM_WORLD);
                        MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja, &baza1[i].imie[0], 6, MPI_CHAR, MPI_COMM_WORLD);
                        MPI_Unpack(bufor, rozmiar_komunikatu, &pozycja, &baza1[i].nazwisko[0], 6, MPI_CHAR, MPI_COMM_WORLD);
                        //printf("unpacked: %lf, %c, %s, %s\n", baza1[i].identyfikator, baza1[i].plec, baza1[i].imie, baza1[i].nazwisko);
                    }
                }
            }
        }
    }

    double end2 = (MPI_Wtime() - start) / 2;

    MPI_Finalize();

    if (rank == 0) {
        printf("\
            Czas bez pakowania: %lf\n\
            Czas z pakowaniem: %lf\n", end1, end2);
    }

    return (0);
}