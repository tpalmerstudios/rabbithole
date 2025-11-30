/**
 * @file main.c
 * @brief This is the beginning stage of the Rabbit frontend for the database
 */
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct Item
 * @brief Represents a stored record that consists of a name and value pair.
 */
typedef struct
{
        char name[50]; /**< Item name (trailing null-terminated), user supplied. */
        int value;     /**< Integer value associated with the item. */
} Item;
/**
 * @define MAX_ITEMS
 * @brief Limit to 100 while developing to keep memory bounded.
 */
#define MAX_ITEMS 100
/**
 * @define DATA_FILE
 * @brief Default CSV file path for persisting item data between runs.
 */
#define DATA_FILE "items.csv"
/**
 * @var items
 * @brief In-memory array where all items are accumulated before persistence.
 */
Item items[MAX_ITEMS];
/**
 * @var itemCount
 * @brief Tracks the current number of valid entries stored in @ref items.
 */
int itemCount = 0;

int showMenu ();
int addItem ();
int viewItems ();
int loadItemsFromFile (const char *filename);
int saveItemsToFile (const char *filename);
int readIntFromInput (const char *prompt, int *value);
void trimNewline (char *str);
bool isStringEmpty (const char *str);
int readName (char *buffer, size_t bufferSize);

/**
 * @brief Entry point for the Rabbit CLI.
 *
 * Loads persisted items from @ref DATA_FILE, presents the main menu loop, and
 * saves items before exiting. The function keeps prompting until the user
 * chooses to exit.
 *
 * @return int 0 on normal exit, non-zero on unrecoverable errors.
 */
int main ()
{
        int loaded = loadItemsFromFile (DATA_FILE);
        if (loaded < 0)
                {
                        printf ("Warning: Unable to load items from %s. Starting with an empty list.\n",
                                        DATA_FILE);
                }
        else if (loaded > 0)
                {
                        printf ("Loaded %d item(s) from %s.\n", loaded, DATA_FILE);
                }

        int choice;
        while (1)
                {
                        showMenu ();

                        if (readIntFromInput ("Enter your choice: ", &choice) != 0)
                                {
                                        printf ("Invalid input. Please enter a number.\n");
                                        continue;
                                }

                        switch (choice)
                                {
                                case 1:
                                        addItem ();
                                        break;
                                case 2:
                                        viewItems ();
                                        break;
                                case 3:
                                        if (saveItemsToFile (DATA_FILE) != 0)
                                                {
                                                        printf ("Error saving items to %s. Changes may not persist.\n",
                                                                        DATA_FILE);
                                                }
                                        printf ("Exiting...\n");
                                        return 0;
                                default:
                                        printf ("Invalid choice. Please try again.\n");
                                }
                }

        return 0;
}

/**
 * @brief Display the primary application menu.
 *
 * Prints available user actions to standard output.
 *
 * @return int Always returns 0.
 */
int showMenu ()
{
        printf ("\nRabbit Hole--------\n");
        printf ("Data that goes deeper.\n");
        printf ("1.) Add Item\n");
        printf ("2.) View Items\n");
        printf ("3.) Exit\n");
        return 0;
}

/**
 * @brief Prompt for and add a new item to the collection.
 *
 * Validates capacity, ensures the name is non-empty, and reads an integer value
 * from the user. The item is persisted to @ref DATA_FILE immediately after a
 * successful addition.
 *
 * @return int 0 on success, -1 when validation fails or capacity is exceeded.
 */
int addItem ()
{
        if (itemCount >= MAX_ITEMS)
                {
                        printf ("Cannot add more items. Maximum reached.\n");
                        return -1;
                }

        Item newItem;

        if (readName (newItem.name, sizeof (newItem.name)) != 0)
                {
                        printf ("Name cannot be empty.\n");
                        return -1;
                }

        if (readIntFromInput ("Enter item value (integer): ", &newItem.value) != 0)
                {
                        printf ("Invalid integer. Item not added.\n");
                        return -1;
                }

        items[itemCount++] = newItem;
        printf ("Item added successfully!\n");

        if (saveItemsToFile (DATA_FILE) != 0)
                {
                        printf ("Warning: Item added but failed to save to %s.\n", DATA_FILE);
                }
        return 0;
}

/**
 * @brief Print all stored items to the console.
 *
 * Provides a numbered list of each item's name and value.
 *
 * @return int 0 when items were displayed, -1 if no items are available.
 */
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
                        printf ("Item %d: Name: %s, Value: %d\n", i + 1, items[i].name, items[i].value);
                }
        return 0;
}

/**
 * @brief Load existing items from a CSV file.
 *
 * Each line is expected to be in the format "name,value". Parsing validates
 * non-empty names and integer values and stops when @ref MAX_ITEMS is reached.
 *
 * @param filename Path to the CSV file to read.
 * @return int Number of items loaded on success, -1 if the file cannot be
 * opened.
 */
int loadItemsFromFile (const char *filename)
{
        FILE *file = fopen (filename, "r");
        if (file == NULL)
                {
                        return -1;
                }

        char line[128];
        int loaded = 0;

        while (fgets (line, sizeof (line), file) != NULL && itemCount < MAX_ITEMS)
                {
                        trimNewline (line);
                        char *comma = strchr (line, ',');
                        if (comma == NULL)
                                {
                                        continue;
                                }

                        *comma = '\0';
                        char *name = line;
                        char *valueStr = comma + 1;

                        if (isStringEmpty (name))
                                {
                                        continue;
                                }

                        char *endptr;
                        long value = strtol (valueStr, &endptr, 10);
                        while (isspace ((unsigned char)*endptr))
                                {
                                        endptr++;
                                }
                        if (*endptr != '\0')
                                {
                                        continue;
                                }

                        strncpy (items[itemCount].name, name, sizeof (items[itemCount].name) - 1);
                        items[itemCount].name[sizeof (items[itemCount].name) - 1] = '\0';
                        items[itemCount].value = (int)value;
                        itemCount++;
                        loaded++;
                }

        if (!feof (file))
                {
                        printf ("Warning: Maximum item limit reached while loading. Some items were not loaded.\n");
                }

        fclose (file);
        return loaded;
}

/**
 * @brief Persist all current items to a CSV file.
 *
 * Writes each item from @ref items up to @ref itemCount using the
 * "name,value" format.
 *
 * @param filename Path to the CSV file to write.
 * @return int 0 on success, -1 if the file cannot be opened for writing.
 */
int saveItemsToFile (const char *filename)
{
        FILE *file = fopen (filename, "w");
        if (file == NULL)
                {
                        return -1;
                }

        for (int i = 0; i < itemCount; i++)
                {
                        fprintf (file, "%s,%d\n", items[i].name, items[i].value);
                }

        fclose (file);
        return 0;
}

/**
 * @brief Read an integer from standard input with validation.
 *
 * Prompts the user, parses an integer, and verifies that no trailing
 * non-whitespace characters remain.
 *
 * @param prompt Prompt string displayed before reading.
 * @param value Output pointer populated with the parsed integer on success.
 * @return int 0 when parsing succeeds, -1 for invalid input or EOF.
 */
int readIntFromInput (const char *prompt, int *value)
{
        char buffer[64];
        printf ("%s", prompt);
        if (fgets (buffer, sizeof (buffer), stdin) == NULL)
                {
                        return -1;
                }

        char *endptr;
        long parsed = strtol (buffer, &endptr, 10);

        while (isspace ((unsigned char)*endptr))
                {
                        endptr++;
                }

        if (*endptr != '\0')
                {
                        return -1;
                }

        *value = (int)parsed;
        return 0;
}

/**
 * @brief Remove a trailing newline from a string buffer if present.
 *
 * @param str Mutable string buffer to sanitize.
 */
void trimNewline (char *str)
{
        size_t len = strlen (str);
        if (len > 0 && str[len - 1] == '\n')
                {
                        str[len - 1] = '\0';
                }
}

/**
 * @brief Determine if a string contains only whitespace.
 *
 * @param str Input string to evaluate.
 * @return bool True when the string is empty or whitespace only, false
 * otherwise.
 */
bool isStringEmpty (const char *str)
{
        for (const char *c = str; *c != '\0'; c++)
                {
                        if (!isspace ((unsigned char)*c))
                                {
                                        return false;
                                }
                }
        return true;
}

/**
 * @brief Read a non-empty item name from standard input.
 *
 * Reads into the supplied buffer, trims a trailing newline, and rejects names
 * that are empty or whitespace-only.
 *
 * @param buffer Destination buffer to receive the name.
 * @param bufferSize Size of @p buffer in bytes.
 * @return int 0 on success, -1 if input fails or validation fails.
 */
int readName (char *buffer, size_t bufferSize)
{
        printf ("Enter item name: ");
        if (fgets (buffer, bufferSize, stdin) == NULL)
                {
                        return -1;
                }

        trimNewline (buffer);

        if (isStringEmpty (buffer))
                {
                        return -1;
                }

        return 0;
}
