/*
*
*lab5
*
*Created by Indra Bhurtel on 11/27/18.
*
*
*Name:Indra Bhurtel
*I am senior at UTA. My major is software Engineering.
*UTA ID NO:1001542825
*
*lab 5 assignment
*
*
*This program determine strongly connected components for a directed graph.
*
*Professor Bob Weems
*
*Code compilation in Omega as follows:
*Make sure you make a.dat /b.dat / c.dat sample run from the sample run given by the professor.
*The input will be read as a shell redirect (<) from a file.
*    gcc lab5.c -lm
*   ./a.out <a.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2

int n; ///number of nodes
int e; ///number of edges

char **char_index;
char *string;


struct edge
{
    int tail,head,type;
};
typedef struct edge edgeType;
edgeType *edgeTab;
int *firstEdge; ///Table indicating first in range of edges with a common tail

int *vertexStatus,*secondDFSrestarts;

int tailThenHead(const void* xin, const void* yin)///Used in calls to qsort() and bsearch() for read_input_file()
{
    int result;
    edgeType *x,*y;

    x=(edgeType*) xin;
    y=(edgeType*) yin;
    result=x->tail - y->tail;
    if (result!=0)
        return result;
    else
        return x->head - y->head;
}

int small_prime(int n)
{
    int p;
    for (p = n; p < 2 * n; p++)
    {
        int q = (int)(sqrt(p) + 1);

        int j = 0;
        int i;
        for (i = 2; i < q; i++)
        {
            if (p % i == 0)
                break;
            else if (i == q - 1)
                return p;
        }
    }
}

void read_input_file() ///For reading input file
{
    int a,b,i,j;

    char tail[50];
    char head[50];

    edgeType work;
    edgeType *ptr;

    scanf("%d %d",&n,&e);
    edgeTab=(edgeType*) malloc(e*sizeof(edgeType));
    if (!edgeTab)
    {
        printf("edgeTab malloc failed %d\n",__LINE__);
        exit(0);
    }

    char_index = malloc(n * sizeof(char*));

    int prime = small_prime(n);
    printf("Double hash table size is: %d\n", prime);

    int v;
    for (v=0; v< n; v++)
    {
        char_index[v] = (char *)malloc(50 * sizeof(char));
    }

    char new_line;
    scanf("%c", &new_line);
    bool is_tail_in_char_index;
    bool is_head_in_char_index;
    int count = 0;

    for (i=0; i<e; i++)
    {
        scanf("%s", tail);
        scanf(" ");
        scanf("%s\n", head);

        is_tail_in_char_index = false;
        is_head_in_char_index = false;

        for (j = 0; j < n; j++)
        {
            if (strcmp(tail, char_index[j]) == 0)
            {
                is_tail_in_char_index = true;

            }
            if (strcmp(head, char_index[j]) == 0)
            {
                is_head_in_char_index = true;
            }
        }

        if (is_tail_in_char_index == false)
        {
            strcpy(char_index[count], tail);
            count++;
        }

        if (is_head_in_char_index == false)
        {
            strcpy(char_index[count], head);
            count++;
        }

        int u = 0;
        for (u = 0; u < n; u++)
        {
            if (strcmp(tail, char_index[u]) == 0)
            {
                a = u;
            }
            if (strcmp(head, char_index[u]) == 0)
            {
                b = u;
            }
        }

        if (a<0 || a>=n || b<0 || b>=n)
        {
            printf("Invalid input %d %d at %d\n",a,b,__LINE__);
            exit(0);
        }
        edgeTab[i].tail=a;
        edgeTab[i].head=b;
    }

    qsort(edgeTab,e,sizeof(edgeType),tailThenHead);///sort edges
    j=0;
    for (i=1; i<e; i++)
    {

        if (edgeTab[j].tail==edgeTab[i].tail
                && edgeTab[j].head==edgeTab[i].head)
            ;
        else
        {
            j++;
            edgeTab[j].tail=edgeTab[i].tail;
            edgeTab[j].head=edgeTab[i].head;
        }
    }
    e=j+1;

    firstEdge=(int*) malloc((n+1)*sizeof(int));///For each vertex as a tail, determine first in range of edgeTab entries
    if (!firstEdge)
    {
        printf("malloc failed %d\n",__LINE__);
        exit(0);
    }
    j=0;
    for (i=0; i<n; i++)
    {
        firstEdge[i]=j;
        for ( ; j<e && edgeTab[j].tail==i; j++)
            ;
    }
    firstEdge[n]=e;
}

int finishIndex;

void DFSvisit(int u)
{
    int i,v;

    vertexStatus[u]=GRAY;

    for (i=firstEdge[u]; i<firstEdge[u+1]; i++)
    {
        v=edgeTab[i].head;
        if (vertexStatus[v]==WHITE)
            DFSvisit(v);
    }
    vertexStatus[u]=BLACK;
    secondDFSrestarts[--finishIndex]=u;
}

void reverseEdges()
{
    int a,b,i,j;
    edgeType work;
    edgeType *ptr;

    for (i=0; i<e; i++)
    {
        a=edgeTab[i].tail;
        b=edgeTab[i].head;
        edgeTab[i].tail=b;
        edgeTab[i].head=a;
    }

    qsort(edgeTab,e,sizeof(edgeType),tailThenHead);///sort edges

    if (!firstEdge)///for each vertex as a tail, determine first in range of edgeTab entries
    {
        printf("malloc failed %d\n",__LINE__);
        exit(0);
    }
    j=0;
    for (i=0; i<n; i++)
    {
        firstEdge[i]=j;
        for ( ;
                j<e && edgeTab[j].tail==i;
                j++)
            ;
    }
    firstEdge[n]=e;
}

void DFSvisit2(int u)
{
    int i,v;

    printf("%s\n", char_index[u]);///Indicate that u is in SCC for this restart
    vertexStatus[u]=GRAY;

    for (i=firstEdge[u]; i<firstEdge[u+1]; i++)
    {
        v=edgeTab[i].head;
        if (vertexStatus[v]==WHITE)
            DFSvisit2(v);
    }
    vertexStatus[u]=BLACK;
}

int main()
{
    int u,i,j,k,nextDFS;
    int SCCcount=0;

    read_input_file();

    vertexStatus=(int*) malloc(n*sizeof(int));
    secondDFSrestarts=(int*) malloc(n*sizeof(int));
    if (!vertexStatus || !secondDFSrestarts)
    {
        printf("malloc failed\n");
        exit(0);
    }
    for (u=0; u<n; u++)
        vertexStatus[u]=WHITE;
    finishIndex=n;
    for (u=0; u<n; u++)
        if (vertexStatus[u]==WHITE)
            DFSvisit(u);

    reverseEdges();

    for (u=0; u<n; u++)
        vertexStatus[u]=WHITE;
    for (i=0; i<n; i++)
        if (vertexStatus[secondDFSrestarts[i]]==WHITE)
        {
            SCCcount++;
            printf("SCC %d\n",SCCcount);
            DFSvisit2(secondDFSrestarts[i]);
        }

    int y = 0;
    for (y = 0; y < n; y++)
    {
        free(char_index[y]);
    }

    free(char_index);

    free(edgeTab);
    free(firstEdge);
    free(vertexStatus);
    free(secondDFSrestarts);
    return 0;
}
