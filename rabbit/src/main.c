/**
 * @file main.c
 * @brief This is the beginning stage of the Rabbit frontend for the database
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @sruct Item
 * @brief This is the sample structure we'll be using for now
 */
typedef struct
{
	char name[50];
	int value;
} Item;
/**
 * @define MAX_ITEMS 100
 * @brief Limit to 100 while developing
 */
#define MAX_ITEMS 100
/**
 * @param items
 * @brief An array of Item structures
 */
Item items[MAX_ITEMS];
/**
 * @param itemCount
 * @brief Track current number of items
 */
int itemCount = 0;

int showMenu ();
int addItem ();
int viewItems ();

int main ()
{
	showMenu ();
	int choice;
	while (1)
		{
			printf ("Enter your choice: ");
			scanf ("%d", &choice);
			getchar ();

			switch (choice)
				{
				case 1:
					addItem ();
					break;
				case 2:
					viewItems ();
					break;
				case 3:
					printf ("Exiting...\n");
					return 0;
				default:
					printf ("Invalid choice. Please try again.\n");
				}
		}
	return 0;
}

int showMenu ()
{
	printf ("\nRabbit Hole--------\n");
	printf ("Data that goes deeper.\n");
	printf ("1.) Add Item\n");
	printf ("2.) View Items\n");
	printf ("3.) Exit\n");
	return 0;
}

int addItem ()
{
	if (itemCount >= MAX_ITEMS)
		{
			return -1;
		}

	Item newItem;
	printf ("Enter item name: ");
	fgets (newItem.name, sizeof (newItem.name), stdin);
	newItem.name[strcspn (newItem.name, "\n")] = '\0';

	printf ("Enter item value (integer): ");
	scanf ("%d", &newItem.value);
	getchar ();

	items[itemCount++] = newItem;
	printf ("Item added successfully!\n");
	return 0;
}

int viewItems ()
{
	if (itemCount == 0)
		{
			printf ("No items to display.\n");
			return -1;
		}

	printf ("\nItem List\n");
	for (int i = 0; i < itemCount; i++)
		{
			printf ("Item %d: Name: %s, Value: %d\n", i + 1, items[i].name,
					items[i].value);
		}
	return 0;
}
