#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Tree{
	long long int key;
	int height;
	char nimID[100];
	char name[100];
	long long int studentScore;
	Tree *left, *right;
}*root=NULL;

int getMax (int height1, int height2){
	if (height1 > height2){
		return height1;
	}else{
		return height2;
	}
}

int getHeight(Tree *curr){
	if (curr==NULL){
		return 0;
	}else{
		return curr->height;
	}
}

int getBalanceFactor(Tree *curr){
	if (curr==NULL){
		return 0;
	}else{
		return getHeight(curr->left)-getHeight(curr->right);
	}
}

struct Tree *leftRotation(Tree *curr){
	Tree *child = curr->right;
	Tree *grandChild = child->left;
	
	child->left = curr;
	curr->right = grandChild;
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	child->height = getMax(getHeight(child->left), getHeight(child->right))+1;
	
	return child;
}

struct Tree *rightRotation(Tree *curr){
	Tree *child = curr->left;
	Tree *grandChild = child->right;
	
	child->right = curr;
	curr->left = grandChild;
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	child->height = getMax(getHeight(child->left), getHeight(child->right))+1;
	
	return child;
}
struct Tree *insertNode(Tree *curr, char nimID[],long long int value,char studentName[],int studentScore){
	if (curr==NULL){
		curr = (Tree *)malloc(sizeof(struct Tree));
		curr->key =  value;
		curr->height = 1;
		strcpy(curr->nimID,nimID);
		strcpy(curr->name,studentName);
		curr->studentScore = studentScore;
		curr->left = curr->right = NULL;
		return curr;
	}
	else if (value < curr->key){
		curr->left = insertNode(curr->left, nimID, value, studentName, studentScore);
	}
	else if (value > curr->key){
		curr->right = insertNode(curr->right,nimID, value, studentName, studentScore);
	}
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	int balanceFactor = getBalanceFactor(curr);
	
	if (balanceFactor < -1 && value > curr->right->key){// RR case
		return leftRotation(curr);
	}
	
	if (balanceFactor < -1 && value < curr->right->key){//RL Case
		curr->right = rightRotation(curr->right);
		return leftRotation(curr);
	}
	
	if (balanceFactor > 1 && value < curr->left->key){// LL Case
		return rightRotation(curr);
	}
	
	if (balanceFactor > 1 && value > curr->left->key){ // LR Case
		curr->left = leftRotation(curr->left);
		return rightRotation(curr);
	}
	return curr;
}

static int orderPrint = 0;
void inOrder(Tree *curr) 
{
    if(curr!=NULL)
    {
        inOrder(curr->left);
        printf("| %-3d | %-11d | %-20s | %-4d\n",++orderPrint,curr->key,curr->name,curr->studentScore);
        inOrder(curr->right);
    }
}

int checkNIMLength(char nimID[]){
	if(strlen(nimID) < 10  || strlen(nimID) > 10){
		return 0;
	}else{
		return 1;
	}
}

int checkNIMonlyNumber(char nimID[]){
	int tanda = -1;
	for(int i = 0; i < strlen(nimID); i ++){
		if(nimID[i] < 48 || nimID[i] > 57){
			tanda = 0;
			break;
		}else{
			tanda = 1;
		}
	}
	return tanda;
}

struct Tree *searchInsert(Tree **node, int value){
	char nimID[100];
	int hasilCheckNIM = -1;
	int hasilCheckNIM2 = -1;
	long long int nim = value;;
	char name[100];
	int studentScore;
		
	if((*node)!=NULL){
		if(value<(*node)->key){
			searchInsert(&(*node)->left,value);
		}else if(value>(*node)->key){
			searchInsert(&(*node)->right,value);
		}else{
			
			do{
				printf("NIM already exist\n");
				printf("input student nim [10 numbers] : ");
				scanf("%s",&nimID); fflush(stdin);
				hasilCheckNIM = checkNIMLength(nimID);
				hasilCheckNIM2 = checkNIMonlyNumber(nimID);
				nim = atoi(nimID);
			}while((hasilCheckNIM == 0 || hasilCheckNIM2 ==0));
			
			searchInsert(&root,nim);
		}
	}else{
		do{
			printf("input student name [3..20 characters] : ");
			scanf("%s",&name); fflush(stdin);
		}while(strlen(name) < 3 || strlen(name) > 20);
		do{
			printf("input student score [0..100] : ");
			scanf("%d",&studentScore); fflush(stdin);
		}while(studentScore < 0 || studentScore > 100);
		
		root = insertNode(root, nimID, nim, name, studentScore);
		return 0;
	}
}

int addBT(Tree* root)
{
    if (root == NULL)
        return 0;
    return (root->studentScore + addBT(root->left) + addBT(root->right));
}

int hitungNode(Tree *root)
{
    int c =  1;             
    if (root ==NULL)
        return 0;
    else
    {
        c += hitungNode(root->left);
        c += hitungNode(root->right);
        return c;
    }
}

struct Tree *preDecessor(Tree *curr){
	curr= curr->left;
	while(curr->right){
		curr=curr->right;
	}	
	return curr;
}

struct Tree *deleteNode(Tree *curr, char nimID[],long long int value,char studentName[],int studentScore){
	if (curr==NULL){
		return curr;
	}
	else if (value < curr->key){
		curr->left = deleteNode(curr->left, nimID, value, studentName, studentScore);
	}
	else if (value > curr->key){
		curr->right = deleteNode(curr->right, nimID, value, studentName, studentScore);
	}
	else{
		Tree *temp = NULL;
		if (curr->left ==NULL || curr->right==NULL){ // 1 CHILD OR NO CHILD
			if (curr->left==NULL){
				temp = curr->right;
			}
			else{
				temp = curr->left;
			}
			
			if (temp==NULL){ // NO CHILD
				temp=curr;
				curr=NULL;
			}else{ // HAVE 1 CHILD
				*curr = *temp;
			}
			free(temp);
		}
		else{ // HAVE 2 CHILD
			temp = preDecessor(curr);
			curr->key = temp->key;
			strcpy(curr->nimID,temp->nimID);
			strcpy(curr->name,temp->name);
			curr->studentScore = temp->studentScore;
			curr->left = deleteNode(curr->left, temp->nimID ,temp->key, temp->name, temp->studentScore);
		}
	}
	
	if (curr==NULL){
		return curr;
	}
	
	curr->height = getMax(getHeight(curr->left), getHeight(curr->right))+1;
	int balanceFactor = getBalanceFactor(curr);
	
	if (balanceFactor >1 && getBalanceFactor(curr->left)<0){
		curr->left = leftRotation(curr->left);
		return rightRotation(curr);
	}
	
	if (balanceFactor > 1 && getBalanceFactor(curr->left)>=0){
		return rightRotation(curr);
	}
	
	if (balanceFactor < -1 && getBalanceFactor(curr->right)<=0){
		return leftRotation(curr);
	}
	
	if (balanceFactor < -1 && getBalanceFactor(curr->right)>0){
		curr->right = rightRotation(curr->right);
		return leftRotation(curr);
	}
	
	return curr;
}

struct Tree *searchDelete(Tree **node, long long int value){
	if((*node)!=NULL){
		if(value<(*node)->key){
			searchDelete(&(*node)->left,value);
		}else if(value>(*node)->key){
			searchDelete(&(*node)->right, value);
		}else{
			root = deleteNode(root , (*node)->name, value, (*node)->name, (*node)->studentScore);
    		printf("Data succesfully deleted");
    		getchar();
		}
	}else{
		printf("data is not found!");
		getchar();
		return 0;
	}
}

struct Tree *searchStudent(Tree **node, long long int value, int ketemu){
	if((*node)!=NULL){
		if(value<(*node)->key){
			searchStudent(&(*node)->left,value, (ketemu + 1));
		}else if(value>(*node)->key){
			searchStudent(&(*node)->right, value, (ketemu + 1));
		}else{
    		printf("Data found after %d searchs\n",ketemu);
    		printf("Name : %s\n",(*node)->name);
    		printf("Score : %lld\n",(*node)->studentScore);
    		getchar();
		}
	}else{
		printf("data is not found!");
		getchar();
		return 0;
	}
}

int main(){
	int choose = 0;
	static int count = 1;
	char nimID[100];
	int hasilCheckNIM = -1;
	int hasilCheckNIM2 = -1;
	long long int nim = 0;
	double jumlahScore = 0;
	double jumlahNode = 0;
	
	do{
		if(count==1)
        {
			root = insertNode(root, "1300030990",1300030990,"Setiabudi",60);
        	root = insertNode(root, "1300030992",1300030992,"Andi Setiawan",50);
			root = insertNode(root, "1300030994",1300030994,"John",70);
			root = insertNode(root, "1300030996",1300030996,"Tony Sutejo",90);
			root = insertNode(root, "1300030998",1300030998,"Karen Wijaya",80);
            count++;
        }else{
			system("cls");
			printf("======================================================\n");
	       	printf("| %-3s | %-11s | %-20s | %-4s   |\n","No.","Nim","Name","Score");
	       	printf("======================================================\n");
	       	orderPrint = 0;
	    	inOrder(root);
	    	printf("======================================================\n");
			jumlahScore = addBT(root);
			jumlahNode = hitungNode(root);
			printf("Average Score : %.2f\n",jumlahScore/jumlahNode);
			printf("======================================================\n");
	    	printf("\n");
	    	
			printf("1. Input student data\n");
			printf("2. Remove student data\n");
			printf("3. Search student\n");
			printf("4. Exit\n");
			printf("input your choice : ");
			scanf("%d",&choose); fflush(stdin);	
			if(choose == 1){
				do{
					printf("input student nim [10 numbers] : ");
					scanf("%s",&nimID); fflush(stdin);
					hasilCheckNIM = checkNIMLength(nimID);
					hasilCheckNIM2 = checkNIMonlyNumber(nimID);
					nim = atoi(nimID);
				}while((hasilCheckNIM == 0 || hasilCheckNIM2==0));
				
				searchInsert(&root,nim);
			}
			else if(choose == 2){
				do{
					printf("input student nim [10 numbers] : ");
					scanf("%s",&nimID); fflush(stdin);
					hasilCheckNIM = checkNIMLength(nimID);
					hasilCheckNIM2 = checkNIMonlyNumber(nimID);
					nim = atoi(nimID);
				}while((hasilCheckNIM == 0 || hasilCheckNIM2==0));
				
				searchDelete(&root, nim);
			}
			else if(choose == 3){
				do{
					printf("input student nim [10 numbers] : ");
					scanf("%s",&nimID); fflush(stdin);
					hasilCheckNIM = checkNIMLength(nimID);
					hasilCheckNIM2 = checkNIMonlyNumber(nimID);
					nim = atoi(nimID);
				}while((hasilCheckNIM == 0 || hasilCheckNIM2==0));
				
				searchStudent(&root, nim, 1);
			}
		}
	}while(choose != 4);
}
