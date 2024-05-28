//
// Created by malinka on 27.05.24.
//

#include "functions.h"

TeaRecipes_array teaRecipes;

void printMenu() {
    printf("\nTea Shop Management System\n");
    printf("1. Add new recipe\n");
    printf("2. Edit existing recipe\n");
    printf("3. Show all recipes\n");
    printf("4. Brew tea\n");
    printf("5. Search recipe by tea type\n");
    printf("6. Search recipe by tea type and ingredient\n");
    printf("7. Search recipe by water amount\n");
    printf("8. Save recipes to file\n");
    printf("0. Exit\n");
}

void menu() {
    loadRecipesFromFile(FILENAME);
    int choice;
    do {
        TeaRecipes_array searchResults = {0};
        printMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addRecipe();
                break;
            case 2:
                editRecipe();
                break;
            case 3:
                displayRecipes();
                break;
            case 4:
                brewTea();
                break;
            case 5:
                searchByTeaType(&searchResults);
                outputSearchResults(searchResults);
                break;
            case 6:
                searchByTeaTypeAndIngredient(&searchResults);
                outputSearchResults(searchResults);
                break;
            case 7:
                searchByWaterAmount(&searchResults);
                outputSearchResults(searchResults);
                break;
            case 8:
                saveRecipesToFile(FILENAME);
                printf("Recipes saved to file.\n");
                break;
            case 0:
                saveRecipesToFile(FILENAME);
                printf("Exiting and saving recipes to file.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

TeaType getTeaTypeFromUser() {
    int type;
    printf("Select tea type (0: Black, 1: Green, 2: Herbal, 3: Fruit): ");
    scanf("%d", &type);
    getchar(); // consume newline

    while (type < 0 || type > 3) {
        printf("Invalid choice. Please enter again: ");
        scanf("%d", &type);
        getchar(); // consume newline
    }

    return (TeaType) type;
}

MeasurementUnit getMeasurementUnitFromUser() {
    int unit;
    printf("Select measurement unit (0: Grams, 1: ML): ");
    scanf("%d", &unit);
    getchar(); // consume newline

    while (unit < 0 || unit > 1) {
        printf("Invalid choice. Please enter again: ");
        scanf("%d", &unit);
        getchar(); // consume newline
    }

    return (MeasurementUnit) unit;
}

IngredientType getIngredientTypeFromUser() {
    int type;
    printf("Select ingredient type (0: Main, 1: Additional): ");
    scanf("%d", &type);
    getchar(); // consume newline

    while (type < 0 || type > 1) {
        printf("Invalid choice. Please enter again: ");
        scanf("%d", &type);
        getchar(); // consume newline
    }

    return (IngredientType) type;
}

void inputIngredient(Ingredient *ingredient) {
    printf("Enter ingredient name: ");
    fgets(ingredient->name, 50, stdin);
    ingredient->name[strcspn(ingredient->name, "\n")] = '\0'; // remove newline

    ingredient->unit = getMeasurementUnitFromUser();
    if (ingredient->unit == GRAMS) {
        printf("Enter quantity in grams: ");
        scanf("%d", &ingredient->quantity.grams);
    } else {
        printf("Enter quantity in ml: ");
        scanf("%d", &ingredient->quantity.ml);
    }
    getchar(); // consume newline
}

void addRecipe() {
    TeaRecipe newRecipe;

    printf("Enter recipe name: ");
    fgets(newRecipe.name, 50, stdin);
    newRecipe.name[strcspn(newRecipe.name, "\n")] = '\0'; // remove newline

    newRecipe.teaType = getTeaTypeFromUser();
    printf("Enter brewing time (minutes): ");
    scanf("%d", &newRecipe.brewingTime);
    while (newRecipe.brewingTime < 0) {
        printf("Invalid input. Please enter again: ");
        scanf("%d", &newRecipe.brewingTime);
        getchar(); // consume newline
    }
    printf("Enter water amount (ml): ");
    scanf("%d", &newRecipe.waterAmount);
    getchar(); // consume newline
    while (newRecipe.waterAmount <= 0) {
        printf("Invalid input. Please enter again: ");
        scanf("%d", &newRecipe.waterAmount);
        getchar(); // consume newline
    }

    printf("Enter main ingredient details:\n");
    inputIngredient(&newRecipe.mainIngredient);
    newRecipe.mainIngredient.type = MAIN;

    printf("Enter number of additional ingredients: ");
    scanf("%d", &newRecipe.numAdditionalIngredients);
    getchar(); // consume newline

    for (int i = 0; i < newRecipe.numAdditionalIngredients; i++) {
        printf("Enter details for additional ingredient %d:\n", i + 1);
        inputIngredient(&newRecipe.additionalIngredients[i]);
        newRecipe.additionalIngredients[i].type = ADDITIONAL;
    }
    newRecipe.index = teaRecipes.size;
    teaRecipes.array[teaRecipes.size++] = newRecipe;
    printf("Recipe added successfully.\n");
    saveRecipesToFile(FILENAME);
}

void editRecipe() {
    int index;
    printf("Enter recipe index to edit (0 to %d): ", teaRecipes.size - 1);
    scanf("%d", &index);
    getchar(); // consume newline

    if (index < 0 || index >= teaRecipes.size) {
        printf("Invalid recipe index.\n");
        return;
    }

    TeaRecipe *recipe = &teaRecipes.array[index];
    int choice;
    do {
        printf("\nEditing recipe: %s\n", recipe->name);
        printf("1. Edit name\n");
        printf("2. Edit tea type\n");
        printf("3. Edit brewing time\n");
        printf("4. Edit water amount\n");
        printf("5. Edit main ingredient\n");
        printf("6. Edit additional ingredients\n");
        printf("0. Finish editing\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter new recipe name: ");
                fgets(recipe->name, 50, stdin);
                recipe->name[strcspn(recipe->name, "\n")] = '\0'; // remove newline
                break;
            case 2:
                recipe->teaType = getTeaTypeFromUser();
                break;
            case 3:
                printf("Enter new brewing time (minutes): ");
                scanf("%d", &recipe->brewingTime);
                getchar(); // consume newline
                while (recipe->brewingTime < 0) {
                    printf("Invalid input. Please enter again: ");
                    scanf("%d", &recipe->brewingTime);
                    getchar(); // consume newline
                }
                break;
            case 4:
                printf("Enter new water amount (ml): ");
                scanf("%d", &recipe->waterAmount);
                getchar(); // consume newline
                while (recipe->waterAmount <= 0) {
                    printf("Invalid input. Please enter again: ");
                    scanf("%d", &recipe->waterAmount);
                    getchar(); // consume newline
                }
                break;
            case 5:
                printf("Enter new main ingredient details:\n");
                inputIngredient(&recipe->mainIngredient);
                break;
            case 6: {
                int subChoice;
                printf("1. Add additional ingredient\n");
                printf("2. Edit existing additional ingredient\n");
                printf("3. Remove additional ingredient\n");
                printf("Enter your choice: ");
                scanf("%d", &subChoice);
                getchar(); // consume newline

                switch (subChoice) {
                    case 1:
                        if (recipe->numAdditionalIngredients < 10) {
                            printf("Enter details for new additional ingredient:\n");
                            inputIngredient(&recipe->additionalIngredients[recipe->numAdditionalIngredients]);
                            recipe->additionalIngredients[recipe->numAdditionalIngredients].type =
                                    getIngredientTypeFromUser();
                            recipe->numAdditionalIngredients++;
                        } else {
                            printf("Cannot add more additional ingredients.\n");
                        }
                        break;
                    case 2:
                        int ingIndex;
                        printf("Enter additional ingredient index to edit (0 to %d): ",
                               recipe->numAdditionalIngredients - 1);
                        scanf("%d", &ingIndex);
                        getchar(); // consume newline
                        if (ingIndex >= 0 && ingIndex < recipe->numAdditionalIngredients) {
                            printf("Enter new details for additional ingredient %d:\n", ingIndex + 1);
                            inputIngredient(&recipe->additionalIngredients[ingIndex]);
                            recipe->additionalIngredients[ingIndex].type = getIngredientTypeFromUser();
                        } else {
                            printf("Invalid ingredient index.\n");
                        }
                        break;
                    case 3:
                        printf("Enter additional ingredient index to remove (0 to %d): ",
                               recipe->numAdditionalIngredients - 1);
                        scanf("%d", &ingIndex);
                        getchar(); // consume newline
                        if (ingIndex >= 0 && ingIndex < recipe->numAdditionalIngredients) {
                            for (int i = ingIndex; i < recipe->numAdditionalIngredients - 1; i++) {
                                recipe->additionalIngredients[i] = recipe->additionalIngredients[i + 1];
                            }
                            recipe->numAdditionalIngredients--;
                        } else {
                            printf("Invalid ingredient index.\n");
                        }
                        break;
                    default:
                        printf("Invalid choice.\n");
                }
                break;
            }
            case 0:
                printf("Finished editing.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
    saveRecipesToFile(FILENAME);
}

void outputTeaType(TeaRecipe recipe) {
    switch (recipe.teaType) {
        case BLACK: printf("Black\n");
            break;
        case GREEN: printf("Green\n");
            break;
        case HERBAL: printf("Herbal\n");
            break;
        case FRUIT: printf("Fruit\n");
            break;
    }
}

void outputMeasurmentUnit(Ingredient ing) {
    switch (ing.unit) {
        case GRAMS:
            printf(" grams");
            break;
        case ML:
            printf(" ml");
            break;
    }
}

void outputQuantity(Ingredient ing) {
    switch (ing.unit) {
        case GRAMS:
            printf("%d", ing.quantity.grams);
            break;
        case ML:
            printf("%d", ing.quantity.ml);
            break;
    }
}

void displayRecipes() {
    for (int i = 0; i < teaRecipes.size; i++) {
        printf("\nRecipe %d:\n", i);
        printf("Name: %s\n", teaRecipes.array[i].name);
        printf("Tea Type: ");
        outputTeaType(teaRecipes.array[i]);
        printf("Brewing Time: %d minutes\n", teaRecipes.array[i].brewingTime);
        printf("Water Amount: %d ml\n", teaRecipes.array[i].waterAmount);
        printf("Main Ingredient: %s (", teaRecipes.array[i].mainIngredient.name);
        outputQuantity(teaRecipes.array[i].mainIngredient);
        outputMeasurmentUnit(teaRecipes.array[i].mainIngredient);
        printf(")\n");
        printf("Additional Ingredients:\n");
        for (int j = 0; j < teaRecipes.array[i].numAdditionalIngredients; j++) {
            printf("  %s (", teaRecipes.array[i].additionalIngredients[j].name);
            outputQuantity(teaRecipes.array[i].additionalIngredients[j]);
            outputMeasurmentUnit(teaRecipes.array[i].additionalIngredients[j]);
            printf(")\n");
        }
    }
}

void brewTea() {
    int index;
    printf("Enter recipe index to brew (0 to %d): ", teaRecipes.size - 1);
    scanf("%d", &index);
    getchar(); // consume newline

    if (index < 0 || index >= teaRecipes.size) {
        printf("Invalid recipe index.\n");
        return;
    }

    printf("Brewing tea...\n");
    printf("Recipe: %s\n", teaRecipes.array[index].name);
    printf("Brewing time: %d minutes\n", teaRecipes.array[index].brewingTime);
    printf("Water amount: %d ml\n", teaRecipes.array[index].waterAmount);
    printf("Main ingredient: %s\n", teaRecipes.array[index].mainIngredient.name);
    printf("Additional ingredients:\n");
    for (int i = 0; i < teaRecipes.array[index].numAdditionalIngredients; i++) {
        printf("  %s\n", teaRecipes.array[index].additionalIngredients[i].name);
    }
    printf("Tea is ready!\n");
}

void searchByTeaType(TeaRecipes_array *result) {
    TeaType teaType = getTeaTypeFromUser();

    for (int i = 0; i < teaRecipes.size; i++) {
        if (teaRecipes.array[i].teaType == teaType) {
            result->array[result->size++] = teaRecipes.array[i];
        }
    }
}

void searchByTeaTypeAndIngredient(TeaRecipes_array *result) {
    TeaType teaType = getTeaTypeFromUser();
    char ingredientName[50];
    printf("Enter ingredient name: ");
    fgets(ingredientName, 50, stdin);
    ingredientName[strcspn(ingredientName, "\n")] = '\0'; // remove newline

    for (int i = 0; i < teaRecipes.size; i++) {
        if (teaRecipes.array[i].teaType == teaType) {
            if (strcmp(teaRecipes.array[i].mainIngredient.name, ingredientName) == 0) {
                result->array[result->size++] = teaRecipes.array[i];
            }
        }
    }
}

void searchByWaterAmount(TeaRecipes_array *result) {
    int minWater, maxWater;
    printf("Enter minimum water amount (ml): ");
    scanf("%d", &minWater);
    printf("Enter maximum water amount (ml): ");
    scanf("%d", &maxWater);
    getchar(); // consume newline

    for (int i = 0; i < teaRecipes.size; i++) {
        if (teaRecipes.array[i].waterAmount >= minWater && teaRecipes.array[i].waterAmount <= maxWater) {
            result->array[result->size++] = teaRecipes.array[i];
        }
    }
}

void saveRecipesToFile(const char *filename) {
    FILE *file;
    if ((file = fopen(filename, "wb")) == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(&teaRecipes.size, sizeof(int), 1, file);
    fwrite(teaRecipes.array, sizeof(TeaRecipe), teaRecipes.size, file);
    fclose(file);
}

void loadRecipesFromFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        puts("New file will be created after first saving to file");
        return;
    }
    fread(&teaRecipes.size, sizeof(int), 1, file);
    fread(teaRecipes.array, sizeof(TeaRecipe), teaRecipes.size, file);
    fclose(file);
}

void outputSearchResults(TeaRecipes_array teaRecipes) {
    for (int i = 0; i < teaRecipes.size; ++i) {
        printf("Index: %d\n", teaRecipes.array[i].index);
        printf("Name: %s\n", teaRecipes.array[i].name);
        printf("Brewing time: %d min\n", teaRecipes.array[i].brewingTime);
        printf("Water for brew: %d ml\n", teaRecipes.array[i].waterAmount);
    }
}
