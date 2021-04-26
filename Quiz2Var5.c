#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node
{
	int key;
	char nama[100];
	char project[100];
	struct Node *kiri;
	struct Node *kanan;
	int height;
};

int check;
int max(int a, int b);
int height(struct Node *N)
{
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b)
{
	return (a > b)? a : b;
}

struct Node* newNode(int key, char nama[100], char project[100])
{
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	node->key = key;
	strcpy(node->nama, nama);
	strcpy(node->project, project);
	node->kiri = NULL;
	node->kanan = NULL;
	node->height = 1;
	return(node);
}

struct Node *rightRotate(struct Node *y)
{
	struct Node *x = y->kiri;
	struct Node *T2 = x->kanan;

	x->kanan = y;
	y->kiri = T2;

	y->height = max(height(y->kiri), height(y->kanan))+1;
	x->height = max(height(x->kiri), height(x->kanan))+1;

	return x;
}

struct Node *leftRotate(struct Node *x)
{
	struct Node *y = x->kanan;
	struct Node *T2 = y->kiri;

	y->kiri = x;
	x->kanan = T2;

	x->height = max(height(x->kiri), height(x->kanan))+1;
	y->height = max(height(y->kiri), height(y->kanan))+1;

	return y;
}

int getBalance(struct Node *N)
{
	if (N == NULL)
		return 0;
	return height(N->kiri) - height(N->kanan);
}

struct Node* insert(struct Node* node, int key, char nama[100], char project[100])
{
	if (node == NULL){
		return(newNode(key, nama, project));
	}

	if (strcmp(nama, node->nama) < 0){
		node->kiri = insert(node->kiri, key, nama, project);
	}else if (strcmp(nama, node->nama) > 0){
		node->kanan = insert(node->kanan, key, nama, project);
	}else
		return node;

	node->height = 1 + max(height(node->kiri),height(node->kanan));

	int balance = getBalance(node);

	if (balance > 1 && strcmp(nama, node->kiri->nama) < 0){
		return rightRotate(node);
	}

	if (balance < -1 && strcmp(nama, node->kanan->nama) > 0){
		return leftRotate(node);
	}

	if (balance > 1 && strcmp(nama, node->kiri->nama) > 0)
	{
		node->kiri = leftRotate(node->kiri);
		return rightRotate(node);
	}

	if (balance < -1 && strcmp(nama, node->kanan->nama) < 0)
	{
		node->kanan = rightRotate(node->kanan);
		return leftRotate(node);
	}

	return node;
}

struct Node * minValueNode(struct Node* node)
{
    struct Node* current = node;
    while (current->kiri != NULL)
        current = current->kiri;
  
    return current;
}

struct Node* deleteNode(struct Node* root, char nama[100])
{
    if (root == NULL){
        return root;
	}
  
    if (strcmp(nama, root->nama) < 0){
    	root->kiri = deleteNode(root->kiri, nama);
	}else if(strcmp(nama, root->nama) > 0){
        root->kanan = deleteNode(root->kanan, nama);
	}else
    {
    	if( (root->kiri == NULL) || (root->kanan == NULL) )
        {
            struct Node *temp = root->kiri ? root->kiri : root->kanan;
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            	*root = *temp;
            free(temp);
        	check = 1;
		}
        else
        {
            struct Node* temp = minValueNode(root->kanan);
            root->key = temp->key;
            strcpy(root->nama, temp->nama);
            strcpy(root->project,temp->project);
            root->kanan = deleteNode(root->kanan, temp->nama);
        }
    }
  
    if (root == NULL){
		return root;
	}
      
    root->height = 1 + max(height(root->kiri), height(root->kanan));
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->kiri) >= 0){
        return rightRotate(root);
	}
  
    if (balance > 1 && getBalance(root->kiri) < 0)
    {
        root->kiri =  leftRotate(root->kiri);
        return rightRotate(root);
    }
  
    if (balance < -1 && getBalance(root->kanan) <= 0){
        return leftRotate(root);
	}
  
    if (balance < -1 && getBalance(root->kanan) > 0)
    {
        root->kanan = rightRotate(root->kanan);
        return leftRotate(root);
    }
  
    return root;
}

void preOrder(struct Node *root)
{
	if(root != NULL)
	{
		preOrder(root->kiri);
		printf("%s%d | %-20s | %-20s\n", "SKY-KY", root->key, root->nama, root->project);
		preOrder(root->kanan);
	}
}

void changeProject(struct Node *root, char nama[100], char project[100])
{
	if(root != NULL)
	{
		changeProject(root->kiri, nama, project);
		if(strcmp(root->nama, nama) == 0){
			strcpy(root->project, project);
			check = 1;
		}
		changeProject(root->kanan, nama, project);
	}
}

int main()
{
	int id = 1, loop = 1, choice;
	struct Node *root = NULL;
	
	root = insert(root, id, "Nagita Slavina", "Ruby");
	id++;
	root = insert(root, id, "Ayu Ting Ting", "Diamond");
	id++;
	root = insert(root, id, "Sky", "PHP .NET XZ");
	id++;
	
	
	while(loop == 1){
		system("cls");
		printf("=======================================================\n");
		preOrder(root);
		printf("=======================================================\n\n");
		
		printf("Single-Nite Freelancer HQ\n");
		printf("1. New Freelancer\n");
		printf("2. Update Project\n");
		printf("3. Freelancer Resign\n");
		printf("4. Exit\n");
		printf("Select Choice : ");
		scanf(" %d", &choice);
		switch(choice){
			case 1:
				printf("Input New Freelancer Name : ");
				char nama[100];
				scanf(" %[^\n]s", &nama);
				printf("Input Project Name : ");
				char project[100];
				scanf(" %[^\n]s", &project);
				
				root = insert(root, id, nama, project);
				id++;
				
				printf("\nInsert %s Success!\n", nama);
				break;
			case 2:
				check = 0;
				printf("Input New Freelancer Name : ");
				char nama2[100];
				scanf(" %[^\n]s", &nama2);
				printf("Input Project Name : ");
				char project2[100];
				scanf(" %[^\n]s", &project2);
				
				changeProject(root, nama2, project2);
				if(check == 1){
					printf("Project for employee %s has successfully change to %s\n", nama2, project2);
				}else{
					printf("Employee Not found\n");
				}
				system("pause");
				break;
			case 3:
				check = 0;
				printf("Input New Freelancer Name : ");
				char nama3[100];
				scanf(" %[^\n]s", &nama3);
				
				root = deleteNode(root, nama3);
				if(check == 1){
					printf("Good bye %s, will be happy to work with you again later.\n", nama3);
				}else{
					printf("Employee Not found\n");
				}
				system("pause");
				break;
			case 4:
				loop = 0;
				system("exit");
				break;
			default:
				break;
		}
	}
	
	return 0;
}
