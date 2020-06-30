#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Tree{
	int key;
	int height;
	char itemID[10000];
	int lastPrice;
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
struct Tree *insertNode(Tree *curr, char itemID[],int value,int lastPrice){
	if (curr==NULL){
		curr = (Tree *)malloc(sizeof(struct Tree));
		curr->key =  value;
		curr->height = 1;
		strcpy(curr->itemID,itemID);
		curr->lastPrice = lastPrice;
		curr->left = curr->right = NULL;
		return curr;
	}
	else if (value < curr->key){
		curr->left = insertNode(curr->left, itemID, value, lastPrice);
	}
	else if (value > curr->key){
		curr->right = insertNode(curr->right,itemID, value, lastPrice);
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

struct Tree *preDecessor(Tree *curr){
	curr= curr->left;
	while(curr->right){
		curr=curr->right;
	}
	
	return curr;
}
struct Tree *deleteNode(Tree *curr, int value, char itemID[],int lastPrice){
	if (curr==NULL){
		return curr;
	}
	else if (value < curr->key){
		curr->left = deleteNode(curr->left, value, itemID,lastPrice);
	}
	else if (value > curr->key){
		curr->right = deleteNode(curr->right, value,itemID,lastPrice);
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
			strcpy(curr->itemID,temp->itemID);
			curr->lastPrice = temp->lastPrice;
			curr->left = deleteNode(curr->left, temp->key,itemID,lastPrice);
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

void inOrder(Tree *curr) 
{
    if(curr!=NULL)
    {
        inOrder(curr->left);
        printf("| %10s | %10c%5d      |\n", curr->itemID,'$',curr->lastPrice);
        inOrder(curr->right);
    }
}

struct Tree *searchInsert(Tree **node, long long value,char itemID[],int lastPrice){
	if((*node)!=NULL){
		if(value<(*node)->key){
			searchInsert(&(*node)->left,value,itemID,lastPrice);
		}else if(value>(*node)->key){
			searchInsert(&(*node)->right,value,itemID,lastPrice);
		}else{
			if(lastPrice > (*node)->lastPrice){
				(*node)->lastPrice = lastPrice;
				printf("Success Update New Price!");
				getchar();
			}
			else{
				printf("Your Bid is Below or Same With Last Price");
				getchar();
			}
		}
	}else{
		root = insertNode(root, itemID,value, lastPrice);
		printf("Success Insert New Bid!");
		getchar();
		return 0;
	}
}

struct Tree *searchDelete(Tree **node, long long value,char itemID[],int lastPrice){
	if((*node)!=NULL){
		if(value<(*node)->key){
			searchDelete(&(*node)->left,value,itemID,lastPrice);
		}else if(value>(*node)->key){
			searchDelete(&(*node)->right,value,itemID,lastPrice);
		}else{
			root = deleteNode(root , value, itemID,lastPrice);
    		printf("Item ID succesfully canceled");
    		getchar();
		}
	}else{
		printf("No Data That Match Item ID!");
		getchar();
		return 0;
	}
}

int main(){
	int choose=0;
	static int count =1;
	int inputNumber;
	char itemID[10000];
	char tampung[10000];
	int lastPrice;
	int itemsID;
	
	 do{
	 	 if(count==1)
        {
			root = insertNode(root, "0004",4,300);
        	root = insertNode(root, "0003",3,100);
			root = insertNode(root, "0007",7,500);
			root = insertNode(root, "0001",1,2000);
			root = insertNode(root, "0005",5,35000);
            count++;
        }else{
       system("cls");
       	printf("STAR Auction\n");
       	printf("=====================================\n");
       	printf("| %15s | %15s |\n","Item ID","Last Price");
       	printf("=====================================\n");
    	inOrder(root);
    	printf("=====================================\n");
    	printf("\n");
    	puts("1. Bid Items");
    	puts("2. Cancel Bid");
    	puts("3. Quit");
    	printf("Enter Your Choice: ");
        scanf("%d",&choose);fflush(stdin);
        
        switch(choose)
        {
            case 1:
                do{
                	printf("Item ID [1-9999]: ");
					scanf("%s", &itemID); fflush(stdin);
					itemsID = atoi(itemID);
				}while(itemsID < 1 || itemsID > 9999);
				
				do{
					printf("Bid [$100 - $100000]: $");
					scanf("%d",&lastPrice);fflush(stdin);
				}while(lastPrice < 100 || lastPrice > 100000);
				
				searchInsert(&root,itemsID,itemID,lastPrice);
				
                break;
            case 2:
    			do{
                	printf("Item ID [1-9999]: ");
					scanf("%s", &itemID); fflush(stdin);
					itemsID = atoi(itemID);
				}while(itemsID < 1 || itemsID > 9999);
				
				searchDelete(&root,itemsID,itemID,lastPrice);
    			
                break;
        }
    }
    }while(choose != 3 || choose < 1 || choose > 3);
	return 0;
}
