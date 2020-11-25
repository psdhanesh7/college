//Author:  Dhruv  Eldho  Peter
//Date:  28/09/2020

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

struct  tree
{
  char  name[20];
  int  type,nc;
  struct  tree  *link[5];
};

//Function  to  create  directories  and  files
void  create(struct  tree  *root,struct  tree  *ptr,int  l,char  *dname)
{
  int  i;
  if(ptr==NULL)
  {
    ptr=(struct  tree  *)malloc(sizeof(struct  tree));
    printf("Enter  name  of  dir/file(under  %s)  :",dname);
    scanf("%s",ptr->name);
    printf("Enter  1  for  Dir/2  for  file  :");
    scanf("%d",&ptr->type);
    root->link[l]=ptr;
    for(i=0;i<5;i++)
    ptr->link[i]=NULL;
    if(ptr->type==1)
    {
      printf("No  of  sub  directories/files(for  %s):",ptr->name);
      scanf("%d",&ptr->nc);
      for(i=0;i<ptr->nc;i++)
      create(ptr,ptr->link[i],i,ptr->name);
    }
    else
    ptr->nc=0;
  }
}

//Function  to  display  directories  and  files
void  display(struct  tree  *root)
{
  int  i;
  if(root!=NULL)
  {
    if(root->type==1)
    {
      printf("%s\t\t",root->name);
      for(i=0;i<root->nc;i++)
      {
        printf("%s",root->link[i]->name);
        if(root->link[i]->type==1)
        printf("  (sd)  \t");
        else
        printf("  (f)  \t");
      }
      printf("\n");
      for(i=0;i<root->nc;i++)
      display(root->link[i]);
    }
  }
}

void  main()
{
  int  i;
  struct  tree  *root;

  root = (struct  tree  *) malloc(sizeof(struct  tree));

  printf("Enter  name  of  dir/file(under  root)  :");
  scanf("%s",root->name);

  printf("Enter  1  for  Dir/2  for  file  :");
  scanf("%d",&root->type);

  for(i = 0; i < 5; i++)
    root->link[i] = NULL;

  if(root->type == 1) {
    printf("No  of  sub  directories/files(for  %s):", root->name);
    scanf("%d", &root->nc);
  }
  else {
    root->nc = 0;
  }

  for(i = 0; i < root->nc; i++) {
    create(root, root->link[i], i, root->name);
  }
  
  printf("Directory\tSubdirectories/Files\n");
  display(root);
}
