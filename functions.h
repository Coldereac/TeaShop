//
// Created by malinka on 27.05.24.
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "recipes.bin"

#define MAX_RECIPES 100

// Enum for tea type
typedef enum {
    BLACK,
    GREEN,
    HERBAL,
    FRUIT
} TeaType;

// Enum for measurement unit
typedef enum {
    GRAMS,
    ML
} MeasurementUnit;

// Enum for ingredient type
typedef enum {
    MAIN,
    ADDITIONAL
} IngredientType;

// Structure for ingredient
typedef struct {
    char name[50];

    union {
        int grams;
        int ml;
    } quantity;

    MeasurementUnit unit;
    IngredientType type;
} Ingredient;

// Structure for tea recipe
typedef struct {
    char name[50];
    TeaType teaType;
    Ingredient mainIngredient;
    Ingredient additionalIngredients[10];
    int numAdditionalIngredients;
    int brewingTime; // in minutes
    int waterAmount; // in ml
    int index;
} TeaRecipe;

typedef struct {
    TeaRecipe array[MAX_RECIPES];
    int size;
} TeaRecipes_array;

// Function prototypes
void addRecipe();

void editRecipe();

void displayRecipes();

void brewTea();

void searchByTeaType(TeaRecipes_array *result);

void searchByTeaTypeAndIngredient(TeaRecipes_array *result);

void searchByWaterAmount(TeaRecipes_array *result);

void saveRecipesToFile(const char *filename);

void loadRecipesFromFile(const char *filename);

void printMenu();

void menu();

void outputSearchResults(TeaRecipes_array result);

TeaType getTeaTypeFromUser();

MeasurementUnit getMeasurementUnitFromUser();

IngredientType getIngredientTypeFromUser();

void outputTeaType(TeaRecipe recipe);

#endif //FUNCTIONS_H
