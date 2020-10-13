#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h>

#define INUM 11 //stores number of product items.

#define CT_NUM 3			//stores number of item categories
int flag_landing = 1;		//if flag is 1, login is a fail.
int flag_main = 0;			//if flag is 0, user_init_db() is called.
int pos = 1;				//stores the item pos. at which user is present in search()
int min = 1;				//stores the id of the first product in that particualt category
int max = INUM;				//stores the id of the last product in that particualt category
int cart_view = 0;			//depending on value...'v' in search() allows user to view/unview cart
int first = 0;				//first wil be 0 only the first time search() is accessed.
char *usr_logged = "GUEST"; //tells which user has logged in to landing page.
int cart_count = 0;			//stores number of items in cart
int disp_mode = 0;
int cat_num = 0; //stores number of product categories
struct h_stack	 //for changes user has made to cart
{
	char item[50];
	struct h_stack *next;
} *top = NULL, *node_s, *ptr_s;

struct user //structure that functions as user db.
{
	char usr[50], pass[50];
	struct user *next;
} *start_u = NULL, *node_u, *ptr_u;

struct product_db //product db
{
	char name[25];
	float price;
	int cat, iid, qty; //cid=category tag, iid=item id
} p[INUM] = {{"Samsung Galaxy Note 7", 65000.00, 1, 0, 50},
			 {"Asus Zenfone 3", 19000.00, 1, 0, 100},
			 {"Apple Iphone 7", 68599.00, 1, 0, 10},
			 {"OnePlus 3", 2799.00, 1, 0, 3},
			 {"Nexus 6P", 39998.00, 1, 0, 6},
			 {"Toshiba Satellite C-180X", 35000.00, 2, 0, 10},
			 {"Asus Rog 552JX", 85000.00, 2, 0, 10},
			 {"Jas U8 Smartwatch", 790.00, 3, 0, 5},
			 {"Apple Watch Sport 38mm", 16900.00, 3, 0, 5},
			 {"Samsung Gear S2", 23085.00, 3, 0, 2},
			 {"Moto 360(2nd Gen)", 21999.00, 3, 0, 2}};
struct qty_db //original qty. of each product
{
	int qty;
} q[INUM] = {{0}};

struct category //stores the number of products in each category
{
	int qty;
} c[INUM] = {{0}};
struct product_ll //ciruclar doubly linked list for products
{
	char name[25];
	float price;
	int cat, iid, qty;
	struct product_ll *next, *prev;
} *start_p = NULL, *node_p, *temp_p, *ptr_p;

struct cart_ll //linked list for cart
{
	char name[25];
	float price;
	int iid, cat, qty;
	struct cart_ll *next, *prev;
} *start_c = NULL, *node_c, *temp_c, *ptr_c, *ptr2_c, *temp2_c;

void create_user();
void validate(char *usr, char *pass); //validates the login credentials.
void login();
void landing();		  //menu options for logged in user
void search();		  //browse, add, remove items from/to cart.
void choose_disp();	  //choose whetehr to display entire product list or only a select category.
void create_prodll(); //create a circular doubly linked list from product db.
void cart();
void cart_add();		 //add chosen items to LL for cart
void cart_del();		 //remove chosen items from LL
void cart_options();	 //cart menu
void cart_sort();		 //sort cart according to product id
void history();			 //stores user search(add/delete/clear cart0 activity
void s_pop();			 //pop activity from stack
void s_push(char *item); //push activity onto stack
void list_display();	 //to display the entire/select-part of product list
void pos_handler(int x); //handles how the arrow moves to allow user to select products
void init_structs();	 //initialze the values of deifferent structurs (qty_db var.)
void main()
{
	char ch;

	init_structs();
	if (flag_main == 0)
	{
		node_u = (struct user *)malloc(sizeof(struct user));
		strcpy(node_u->usr, "admin");
		strcpy(node_u->pass, "admin");
		start_u = node_u;
		node_u->next = NULL;
	}
	flag_main = 1;

	while (1)
	{
		system("cls");
		printf(" * *    **  *****                                       \n");
		printf(" *  *   * *    *                                 \n");
		printf(" *   *  **    *                                 \n");
		printf(" *  *   *    *\n");
		printf(" * *    *   *****\n\n");
		printf("REALIZATION OF AN ONLINE SHOPPING APPLICATION WITH DATA STRUCTS.\n~by Siddhant");
		printf("\n\nMENU:-\n1. GUEST LOGIN\n2. USER LOGIN\n3. CREATE USER ACCOUNT\n4. EXIT\n");
		printf("\n\n");

		ch = getch();
		switch (ch)
		{
		case '1':
			usr_logged = "GUEST";
			landing();
			break;
		case '2':
			login();
			break;
		case '3':
			create_user();
			break;
		case '4':
			exit(0);
		}
	}
}

void validate(char *usr, char *pass)
{
	system("cls");
	flag_landing = 1;
	ptr_u = start_u;
	while (ptr_u != NULL)
	{

		if ((strcmp(usr, ptr_u->usr) == 0) && (strcmp(pass, ptr_u->pass) == 0))
			flag_landing = 0;
		ptr_u = ptr_u->next;
	}
	if (flag_landing == 1)
	{
		printf("INVALID USER CREDENTIALS.\n");
		getch();
		main();
	}
	else
	{
		usr_logged = usr;
	}
	landing();
}

void login()
{
	system("cls");
	char usr[50], pass_temp[50], pass[50], ch;
	int ct = 0, i;
	printf(">>PLEASE DO NOT INPUT ANY SPECICAIL CHARS<<\n");
	printf("INPUT THE FOLLOWING DETAILS:-\n\nUSERNAME: ");
	scanf(" %[^\n]", usr);
	printf("PASSWORD: ");
	do
	{
		ch = getch();
		system("cls");
		printf("INPUT THE FOLLOWING DETAILS:-\n\nUSERNAME: %s\nPASSWORD: ", usr);

		if (isalnum(ch))
		{
			if (ct < 50)
			{
				pass_temp[ct] = ch;
				++ct;
			}
		}
		if (ch == '\b')
		{
			if (ct > 0)
				--ct;
		}
		for (int i = 0; i < ct; ++i)
		{
			printf("*");
		}
	} while (ch != '\r');
	for (i = 0; i < ct; ++i)
	{
		pass[i] = pass_temp[i];
	}
	pass[i] = '\0';
	validate(usr, pass);
}

void landing()
{
	char ch;
	if (first == 0)
		create_prodll();
	first = 1;
	while (1)
	{
		system("cls");

		printf("(LOGGED IN AS %s)\n\n", usr_logged);

		printf("1. SEARCH FOR PRODUCTS\n2. VIEW CART AND CHECKOUT\n3. VIEW HISTORY FOR CURRENT SESSION\n4. LOGOUT\n5. LOGOUT AND EXIT\n");
		ch = getch();
		switch (ch)
		{
		case '1':
			search();
			break;
		case '2':
			system("cls");
			cart();
			cart_options();
			break;
		case '4':
			main();
			break;
		case '3':
			history();
			break;
		case '5':
			exit(0);
		}
	}
}

void search()
{
	system("cls");
	choose_disp();
	cart_view = 0;
	char ch;
	while (1)
	{
		system("cls");
		list_display();
		printf("\nNAVIGATION:-\n\n(1.) to go to main menu.\n(2.) To choose p4oduct category.\n(v) to view/unview cart.\n(w/s) to MOVE UP/DOWN\n(a/d) to ADD/DEL item TO/FROM inventory\n");
		if (cart_view % 2 != 0)
		{
			printf("\n\n**************************************CART**************************************\n");
			if (start_c == NULL)
				printf("\n                                    EMPTY CART\n");

			else
			{
				ptr_c = start_c;

				do
				{

					printf("%s(Rs. %0.2f)[QTY: %d]\n", ptr_c->name, ptr_c->price, ptr_c->qty);
					ptr_c = ptr_c->next;
				} while (ptr_c != start_c);
			}
			printf("********************************************************************************\n\n");
		}
		ch = getch();

		if (ch == 'w')
		{
			pos_handler(-1);
		}
		if (ch == 's')
		{
			pos_handler(1);
		}
		if (ch == 'a')
		{
			cart_add();
		}
		if (ch == 'd')
		{
			cart_del();
		}
		if (ch == '1')
		{
			landing();
		}
		if (ch == 'v')
			++cart_view;
		if (ch == '2')
		{
			system("cls");
			choose_disp();
		}
	}
}
void create_prodll()
{
	system("cls");
	int ctr = 1, ch;

	for (int i = 0; i < INUM; ++i)
	{
		node_p = (struct product_ll *)malloc(sizeof(struct product_ll));
		strcpy(node_p->name, p[i].name);
		node_p->price = p[i].price;
		node_p->iid = p[i].iid;
		node_p->cat = p[i].cat;
		node_p->qty = p[i].qty;
		if (start_p == NULL)
		{
			start_p = node_p;
			node_p->prev = NULL;
			node_p->next = start_p;
		}
		else
		{
			ptr_p = start_p;
			while (ptr_p->next != start_p)
				ptr_p = ptr_p->next;
			ptr_p->next = node_p;
			node_p->prev = ptr_p;
			node_p->next = start_p;
			start_p->prev = node_p;
		}
	}
	temp_p = start_p;
}

void cart()
{
	ptr_c = start_c;
	printf("\n\n**************************************CART**************************************\n");
	if (start_c == NULL)
		printf("\n                                    EMPTY CART\n");

	else
	{

		do
		{
			if (ptr_c == temp_c)
				printf("->%s(Rs. %0.2f)[QTY: %d]\n", ptr_c->name, ptr_c->price, ptr_c->qty);
			else
				printf("  %s(Rs. %0.2f)[QTY: %d]\n", ptr_c->name, ptr_c->price, ptr_c->qty);
			ptr_c = ptr_c->next;
		} while (ptr_c != start_c);
	}
	printf("********************************************************************************\n\n");
}

void cart_add()
{
	if (p[(temp_p->iid) - 1].qty > 0)
	{
		ptr_p = start_p;
		do
		{
			if (ptr_p->iid == temp_p->iid)
				break;
			ptr_p = ptr_p->next;
		} while (ptr_p != start_p);
		--ptr_p->qty;
		--p[(temp_p->iid) - 1].qty;
		++cart_count;
		int cat = ptr_p->cat, id = ptr_p->iid;
		float price = ptr_p->price;
		char name[25];
		strcpy(name, ptr_p->name);
		printf("\n\nITEM ADDED TO CART.");
		char h[50];
		strcpy(h, ptr_p->name);
		strcat(h, " was added.\n");
		s_push(h);
		int fl = 0; //if fl=0,new node created. Else, qty. in existing node has been incremented.
		if (start_c != NULL)
		{
			ptr_c = start_c;
			do
			{
				if (ptr_c->iid == id)
				{
					ptr_c->qty++;
					fl = 1;
					break;
				}
				ptr_c = ptr_c->next;
			} while (ptr_c != start_c);
		}
		if (!fl)
		{
			node_c = (struct cart_ll *)malloc(sizeof(struct cart_ll));
			node_c->iid = id;
			strcpy(node_c->name, name);
			node_c->price = price;
			node_c->cat = cat;
			node_c->qty = 1;
			if (start_c == NULL)
			{
				start_c = node_c;
				node_c->next = start_c;
				node_c->prev = start_c;
			}
			else
			{
				ptr_c = start_c;
				while (ptr_c->next != start_c)
					ptr_c = ptr_c->next;
				ptr_c->next = node_c;
				node_c->next = start_c;
				node_c->prev = ptr_c;
			}
		}
		Sleep(600);
	}
	else
	{
		printf("\n\nITEM OUT OF STOCK.");
		Sleep(600);
	}
}
void cart_del()
{
	ptr2_c = temp2_c = start_c;
	if (p[(temp_p->iid) - 1].qty < q[(temp_p->iid) - 1].qty)
	{
		ptr_p = start_p;
		do
		{
			if (ptr_p->iid == temp_p->iid)
				break;
			ptr_p = ptr_p->next;
		} while (ptr_p != start_p);
		++ptr_p->qty;
		++p[(temp_p->iid) - 1].qty;
		--cart_count;
		int id = temp_p->iid;
		printf("\n\nINSTANCE OF ITEM REMOVED FROM CART.");

		char h[50];
		strcpy(h, ptr_p->name);
		strcat(h, " was removed.\n");
		s_push(h);
		int fl = 0; //fl=1 if product id to be deleted has been matched
		do
		{
			if (ptr2_c->iid == id)
			{
				fl = 1;
				break;
			}
			ptr2_c = ptr2_c->next;
		} while (ptr2_c != start_c);

		if (fl == 1)
		{
			if (ptr2_c->qty > 1)
				--(ptr2_c->qty);
			else
			{
				if (ptr2_c == start_c)
				{
					if (start_c->next == start_c)
					{
						free(start_c);
						start_c = NULL;
					}
					else
					{
						temp2_c = ptr2_c;
						while (ptr2_c->next != start_c)
							ptr2_c = ptr2_c->next;
						start_c = start_c->next;
						ptr2_c->next = start_c;
						start_c->prev = ptr2_c;
						free(temp2_c);
					}
				}
				else
				{
					ptr2_c->prev->next = ptr2_c->next;
					ptr2_c->next->prev = ptr2_c->prev;
					free(ptr2_c);
				}
			}
		}
		Sleep(600);
	}
}
void cart_options()
{
	printf("\n");
	char ch;
	temp_c = start_c;
	while (1)
	{
		system("cls");
		cart();
		printf("\n\nOPTIONS:-\n\n");
		printf("NAVIGATION:-\n\n(1.) MAIN MENU\n(2.) CONTINUE SHOPPING/MAKE CHANGES TO CART\n(3.) CLEAR CART\n(4.) CHECKOUT(MODULE NOT IMPLEMENTED)\n(5.) LOGOUT\n(6.) LOGOUT AND EXIT\n");
		printf("(w/s) to MOVE UP/DOWN\n(a/d) to ADD/DEL item TO/FROM inventory\n");
		ch = getch();
		switch (ch)
		{
		case '6':
			exit(0);
		case '5':
			main();
			break;
		case '2':
			search();
			break;
		case '1':
			landing();
			break;
		case 's':
			if (start_c)
				temp_c = temp_c->next;
			break;
		case 'w':
			if (start_c)
				temp_c = temp_c->prev;
			break;
		case 'a':
			if (start_c)
			{
				temp_p = start_p;
				while (temp_p->iid != temp_c->iid)
					temp_p = temp_p->next;
				cart_add();
			}
			break;
		case 'd':
			if (start_c)
			{
				temp_p = start_p;
				while (temp_p->iid != temp_c->iid)
					temp_p = temp_p->next;
				cart_del();
				temp_c = start_c;
			}
			break;
		case '3':
			if (start_c)
			{
				s_push("CART WAS CLEARED.\n");

				ptr_c = start_c;
				while (ptr_c->next != start_c)
				{
					ptr_c = ptr_c->next;
				}
				while (ptr_c != start_c)
				{
					temp_c = ptr_c;
					ptr_c = ptr_c->prev;
					free(temp_c);
				}
				free(start_c);
				start_c = NULL;
				ptr_p = start_p;
				for (int i = 0; i < INUM; ++i)
				{
					p[i].qty = q[i].qty;
					ptr_p->qty = q[i].qty;
					ptr_p = ptr_p->next;
				}
			}
			break;
		}
	}
}

void create_user()
{

	system("cls");
	char usr[50], pass_temp[50], pass[50], ch;
	int ct = 0, i;
	printf(">>PLEASE DO NOT INPUT ANY SPECIAL CHARS<<\n");
	printf("INPUT THE FOLLOWING DETAILS FOR YOUR NEW ACCOUNT:-\n\nUSERNAME: ");
	scanf(" %[^\n]", usr);
	if (start_u)
	{
		ptr_u = start_u;
		while (ptr_u != NULL)
		{
			if (strcmp(usr, ptr_u->usr) == 0)
			{
				printf(" USER ALREADY EXISTS IN DATABASE!!!!\n");
				getch();
				main();
			}
			ptr_u = ptr_u->next;
		}
	}
	printf("PASSWORD: ");
	do
	{
		ch = getch();
		system("cls");
		printf("INPUT THE FOLLOWING DETAILS:-\n\nUSERNAME: %s\nPASSWORD: ", usr);

		if (isalnum(ch))
		{
			if (ct < 50)
			{
				pass_temp[ct] = ch;
				++ct;
			}
		}
		if (ch == '\b')
		{
			if (ct > 0)
				--ct;
		}
		for (int i = 0; i < ct; ++i)
		{
			printf("*");
		}
	} while (ch != '\r');
	for (i = 0; i < ct; ++i)
	{
		pass[i] = pass_temp[i];
	}
	pass[i] = '\0';
	node_u = (struct user *)malloc(sizeof(struct user));
	strcpy(node_u->usr, usr);
	strcpy(node_u->pass, pass);
	if (start_u == NULL)
	{
		start_u = node_u;
		node_u->next = NULL;
	}
	else
	{
		ptr_u = start_u;
		while (ptr_u->next != NULL)
			ptr_u = ptr_u->next;
		ptr_u->next = node_u;
		node_u->next = NULL;
	}
}
void s_push(char *item)
{
	node_s = (struct h_stack *)malloc(sizeof(struct h_stack));
	strcpy(node_s->item, item);
	if (top == NULL)
	{
		top = node_s;
		node_s->next = NULL;
	}
	else
	{
		node_s->next = top;
		top = node_s;
	}
}
void s_pop()
{
	if (top)
	{
		if (top->next)
		{
			ptr_s = top;
			top = top->next;
			free(ptr_s);
		}
		else
		{
			free(top);
			top = NULL;
		}
	}
}
void history()
{
	char ch;
	while (1)
	{
		ptr_s = top;
		system("cls");
		if (ptr_s == NULL)
		{
			printf("\nNO USER ACTIVITY HISTORY FOUND.\n");
			getch();
			landing();
		}
		else
		{
			while (ptr_s != NULL)
			{
				printf("-> ");
				printf(ptr_s->item);
				printf("\n");
				ptr_s = ptr_s->next;
			}

			printf("\n\nNAVIGATION:-\n\n1. Clear Last Activity\n2. Clear History\n3. MAIN MENU\n");
			switch (ch = getch())
			{
			case '3':
				landing();
				break;
			case '1':
				s_pop();
				break;
			case '2':
				while (top)
					s_pop();
				break;
			}
		}
	}
}
void list_display()
{
	ptr_p = start_p;
	int fl1 = 0;
	int ct = 0; //stores number of products htat have been printed in a particulat category
	int j = 0;	//used to loop thorugh cat_db struct
	printf("\n");
	do
	{

		if (ct == 0 && j == 0 && (disp_mode == 0 || disp_mode == 1))
		{
			printf(" SMARTPHONES:-\n");
			printf("-----------------\n\n");
		}
		if (ct == 0 && j == 1 && (disp_mode == 0 || disp_mode == 2))
		{
			printf("\n LATOPS:-\n");
			printf("-----------------\n\n");
		}
		if (ct == 0 && j == 2 && (disp_mode == 0 || disp_mode == 3))
		{
			printf("\n SMART WATCHES:-\n");
			printf("-----------------\n\n");
		}

		if (ptr_p->cat == disp_mode)
		{

			if (ptr_p->iid == temp_p->iid)
				printf("->%s (Rs. %0.2f)", ptr_p->name, ptr_p->price);
			else
				printf("  %s (Rs. %0.2f)", ptr_p->name, ptr_p->price);
			if (ptr_p->qty > 0)
				printf("[IN STOCK]\n");
			else
				printf("[OUT OF STOCK]\n");
		}
		else if (disp_mode == 0)
		{

			if (ptr_p->iid == temp_p->iid)
				printf("->%s (Rs. %0.2f)", ptr_p->name, ptr_p->price);
			else
				printf("  %s (Rs. %0.2f)", ptr_p->name, ptr_p->price);
			if (ptr_p->qty > 0)
				printf("[IN STOCK]\n");
			else
				printf("[OUT OF STOCK]\n");
		}
		ptr_p = ptr_p->next;
		++ct;
		if (ct == c[j].qty)
		{
			ct = 0;
			j++;
		}
	} while (ptr_p != start_p);
	printf("____________________________________________________________\n");
}
void choose_disp()
{
	char ch;
	printf("PRODUCT CATEGORIES:-\n\n1. MOBILES\n2. LAPTOPS\n3. SMART WATCHES\n4. DISPLAY ALL PRODUCTS\n");
	ch = getch();
	switch (ch)
	{
	case '1':
		disp_mode = 1;
		break;
	case '2':
		disp_mode = 2;
		break;
	case '3':
		disp_mode = 3;
		break;
	case '4':
		disp_mode = 0;
		break;
	}
	if (disp_mode != 0)
	{

		temp_p = start_p;
		while (temp_p->cat != disp_mode)
			temp_p = temp_p->next;
	}
	else
	{
		temp_p = start_p;
	}
	system("cls");
}

void pos_handler(int x)
{
	int cat;
	if (disp_mode == 0)
	{
		if (x == 1)
			temp_p = temp_p->next;
		else
			temp_p = temp_p->prev;
	}
	else
	{
		cat = temp_p->cat;
		if (x == 1)
		{

			if (cat == temp_p->next->cat)
				temp_p = temp_p->next;
			else
			{
				temp_p = temp_p->next;
				while (temp_p->cat != cat)
					temp_p = temp_p->next;
			}
		}
		else
		{
			if (cat == temp_p->prev->cat)
				temp_p = temp_p->prev;
			else
			{
				temp_p = temp_p->prev;
				while (temp_p->cat != cat)
					temp_p = temp_p->prev;
			}
		}
	}
}
void init_structs()
{

	int i = 0, j = 0;
	int temp_cat = 0;
	for (i = 0; i < INUM; ++i) //assigns the qty of profutcs to qty_db that stores the original qty.
	{
		q[i].qty = p[i].qty;
	}
	int qty = 1;
	for (i = 1; i < INUM; ++i)
	{
		if (p[i].cat == p[i - 1].cat)
			++qty;
		else
		{
			c[j++].qty = qty;
			qty = 1;
		}
	}
	c[j].qty = qty;
	cat_num = j;
	for (i = 0; i < INUM; ++i)
	{
		p[i].iid = i + 1;
	}
}