/***********************************************************
 * Author: Yannick
 * Purpose: Lasalle College AEC
 * Course: 420AP1AS - Algorithms and Programming
 * Description: Final Project
 * Date: Dec 20 2021
 * *********************************************************/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/*
    I could have done better if I had had a month instead of a week to do it. In its present state, the db is not normalize and
    it would not be possible to use an ORM (like entity framework - not sure if there is one for c...) to bridge with a db like SQL server without changing the structure
    of this app. Many struct members like types and platforms should be in their own struct with their own primary key (like SKU for games) 
    since they should be different tables in a db. Regardless, this project is still fairly easy to maintain and grow. I added 4 members because many can have more then one
    value like platform (the game can be released on more than one) I needed to keep these numbers in memory to search and display correctly. Metacritic is always a whole number
    and falls in a range. SKU should be 8 digit long but it was a pain to remember for testing. It increments from 1 to make it easier to test and could be set to a bigger number 
    if necessary. For now I added this sequence "21lc12fa" and i add the skuAutoIncrementor at the end. You only need to seach for this number.

    I added => fflush(stdin); some places because the code was sometime skipping line. Adding the fflush where i did solved that issue.
*/

// This region block can be deleted if it causes problems. I think it only works within visual studio. There are 3 of them!
#pragma region Global_space

#define MAX_STRING_SIZE_LARGE 50
#define MAX_STRING_SIZE_MEDIUM 30
#define MAX_STRING_SIZE_SMALL 10
#define MAX_ARRAY_SIZE 5

unsigned short userSelectedValue = 0;
unsigned short skuAutoIncrementor = 0;
unsigned short k = 0;

struct video_game
{
    unsigned short sku; // should be 8 digit but for simplicity, it increments from 1
    char name[MAX_STRING_SIZE_LARGE];
    char studio[MAX_STRING_SIZE_LARGE];
    unsigned short typeNum; // keep how many type selected in memory for each game
    char type[MAX_ARRAY_SIZE][MAX_STRING_SIZE_MEDIUM];
    char rating[MAX_STRING_SIZE_SMALL];
    unsigned short metacriticRating;
    char metacriticRatingMeaning[MAX_STRING_SIZE_MEDIUM]; // display the text depending on the range of metacriticRating
    unsigned short platformNum; // keep how many platform selected in memory for each game
    char platform[MAX_ARRAY_SIZE][MAX_STRING_SIZE_SMALL];
    unsigned short modeNum; // keep how many mode selected in memory for each game
    char modes[MAX_ARRAY_SIZE][MAX_STRING_SIZE_LARGE];
    float sellingPrice;
    bool digital;
} videoGame;

struct video_game videoGames[MAX_STRING_SIZE_LARGE];

#pragma endregion Global_space

// There are a lot of functions, but if you follow the main, it should be quite easy to trace. (F12 in VS while selecting the name of the function will warp you to its implementation)
#pragma region Function_declaration

// in c# methods are uppercase but after some research about standards, c functions are supposed to be lowercase...
void autoPopulateInventory();
void displayIntroApp();
void displayMainMenu();
void displayEditMenu();
void initInvPopProcess();
void populateInventory();
void displaySearchMenu();
void searchForGamesByType(int);
void searchForGamesByStudio(int);
void searchForGamesByPlatform(int);
void searchForGamesByMetacritic(int);
void searchForGamesByMultiCriteria(int);
void editEntry();
void refreshCurrentStructValue(int);
void displayFilteredInventory();
void displayFullInventory();
void displayGameStruct(int);
void wait();
void autoGenSKU();
void gameName();
void studioName();
void displayType();
void typeSelector();
void displayRating();
void ratingSelector();
void metacritic();
void metacriticMeaning(float);
void displayPlatform();
void platformSelector();
void displayMode();
void modeSelector();
void submode(int);
void sellingPrice();
void isDigital();
void recordUserData();
void updateUserData(int);
void confirmDisplay();

#pragma endregion Function_declaration

int main()
{
    unsigned short menuIndex, submenuIndex;

    // comment or uncomment the next line if you want (or not) to use the auto populate inventory. It will add 5 entries automatically you can use to test the app!
    autoPopulateInventory();

    displayIntroApp();
    do
    {
        menuIndex = submenuIndex = 0;      
        displayMainMenu();

        while ((scanf("%hu", &menuIndex) != 1 && (menuIndex = getchar()) != EOF && menuIndex != '\n') || (menuIndex < 1 || menuIndex > 5))
        {    
            printf("Invalid input! \n");
            printf("Select the requested input!\n\n");   
        }   
        
        switch (menuIndex)
        {
            case 1:
                // populate
                do
                {                  
                    initInvPopProcess();
                    recordUserData();
                    
                    printf("Do you want to enter another game in the inventory?\n");
                    confirmDisplay();
                } while (userSelectedValue != 2);
                break;
            case 2:           
                // search by criteria (filter)
                do
                {                  
                    printf("By which creteria(s) do you want to search for a game?\n");
                    displaySearchMenu();
                    int searchIndexcheck;

                    while ((scanf("%hu", &submenuIndex) != 1 && (submenuIndex = getchar()) != EOF && submenuIndex != '\n') || (submenuIndex < 1 || submenuIndex > 5))
                    {    
                        printf("Invalid input! \n");
                        printf("Select the requested input!\n\n");   
                    }   

                    fflush(stdin);

                    switch (submenuIndex)
                    {
                        case 1:
                            // search by studio
                            searchForGamesByStudio(searchIndexcheck);
                            break;
                        case 2:
                            // search by type
                            searchForGamesByType(searchIndexcheck);
                            break;
                        case 3:
                            // search by platform
                            searchForGamesByPlatform(searchIndexcheck);
                            break;       
                        case 4:
                            // search by metacritic rating
                            searchForGamesByMetacritic(searchIndexcheck);
                            break;   
                        case 5:
                            // search by multiple criteria (studio, platform, metacritic)
                            searchForGamesByMultiCriteria(searchIndexcheck);
                            break;                        
                        default:
                            break;
                    }
                    
                    printf("Do you want to do another search?\n");
                    confirmDisplay();
                } while (userSelectedValue != 2);
                break;
            case 3:
                // edit an entry using the primary key (SKU)
                do
                {                  
                    editEntry();

                    printf("Do you want to edit another game?\n");
                    confirmDisplay();
                } while (userSelectedValue != 2);             
                break;
            case 4:
                // display
                displayFullInventory();
                break;     
            case 5:
                // leave the app
                printf("Are you sure you want to Exit?\n");
                confirmDisplay();

                if (userSelectedValue == 2)
                {
                    menuIndex = 0;
                }
                
                break;
            default:
                break;
        }      
    } while (menuIndex != 5);
    
    printf("\n\nThank you and goodbye, Captain Michelle!\n");
    printf("------------------------------\n\n");

    return 0;
}

#pragma region Function_implementation

void autoPopulateInventory()
{
    // 5 games added to the inventory. 

    autoGenSKU();
    strcpy(videoGame.name, "Crusader 3"); 
    strcpy(videoGame.studio, "Gestalt"); 
    videoGame.typeNum = 1;
    strcpy(videoGame.type[0], "action"); 
    strcpy(videoGame.rating, "M"); 
    videoGame.metacriticRating = 67;
    metacriticMeaning(videoGame.metacriticRating);
    videoGame.platformNum = 2;
    strcpy(videoGame.platform[0], "PS"); 
    strcpy(videoGame.platform[1], "Xbox"); 
    videoGame.modeNum = 1;
    strcpy(videoGame.modes[0], "Single Player");
    videoGame.sellingPrice = 89.99;
    videoGame.digital = true;
    recordUserData();

    autoGenSKU();
    strcpy(videoGame.name, "Grip Drive"); 
    strcpy(videoGame.studio, "EA"); 
    videoGame.typeNum = 2;
    strcpy(videoGame.type[0], "simulation"); 
    strcpy(videoGame.type[1], "action"); 
    strcpy(videoGame.rating, "E"); 
    videoGame.metacriticRating = 99;
    metacriticMeaning(videoGame.metacriticRating);
    videoGame.platformNum = 2;
    strcpy(videoGame.platform[0], "PC"); 
    strcpy(videoGame.platform[1], "Xbox"); 
    videoGame.modeNum = 2;
    strcpy(videoGame.modes[0], "Single Player");
    strcpy(videoGame.modes[1], "Local multiplayer");
    videoGame.sellingPrice = 49.99;
    videoGame.digital = true;
    recordUserData();

    autoGenSKU();
    strcpy(videoGame.name, "Carnival"); 
    strcpy(videoGame.studio, "Baraka"); 
    videoGame.typeNum = 1;
    strcpy(videoGame.type[0], "action"); 
    strcpy(videoGame.rating, "A"); 
    videoGame.metacriticRating = 67;
    metacriticMeaning(videoGame.metacriticRating);
    videoGame.platformNum = 2;
    strcpy(videoGame.platform[0], "Xbox"); 
    strcpy(videoGame.platform[1], "PC"); 
    videoGame.modeNum = 1;
    strcpy(videoGame.modes[0], "Single Player");
    videoGame.sellingPrice = 69.99;
    videoGame.digital = false;
    recordUserData();

    autoGenSKU();
    strcpy(videoGame.name, "Dorus the Explorus"); 
    strcpy(videoGame.studio, "Minousoft"); 
    videoGame.typeNum = 2;
    strcpy(videoGame.type[0], "idle"); 
    strcpy(videoGame.type[1], "strategy"); 
    strcpy(videoGame.rating, "E"); 
    videoGame.metacriticRating = 22;
    metacriticMeaning(videoGame.metacriticRating);
    videoGame.platformNum = 3;
    strcpy(videoGame.platform[0], "PC"); 
    strcpy(videoGame.platform[1], "PS"); 
    strcpy(videoGame.platform[2], "Xbox"); 
    videoGame.modeNum = 1;
    strcpy(videoGame.modes[0], "Single Player");
    videoGame.sellingPrice = 9.99;
    videoGame.digital = true;
    recordUserData();

    autoGenSKU();
    strcpy(videoGame.name, "Mutant X Hockey"); 
    strcpy(videoGame.studio, "Baraka"); 
    videoGame.typeNum = 1;
    strcpy(videoGame.type[0], "sports"); 
    strcpy(videoGame.rating, "A"); 
    videoGame.metacriticRating = 67;
    metacriticMeaning(videoGame.metacriticRating);
    videoGame.platformNum = 1;
    strcpy(videoGame.platform[0], "PC"); 
    videoGame.modeNum = 3;
    strcpy(videoGame.modes[0], "Single Player");
    strcpy(videoGame.modes[1], "Local multiplayer + Coop");
    strcpy(videoGame.modes[2], "Online multiplayer + Coop");
    videoGame.sellingPrice = 89.99;
    videoGame.digital = false;
    recordUserData();

}

void displayIntroApp()
{
    printf("\n\nWelcome to the Gestalt Library for video games listing.\nYou can use this inventory of games to perform these actions:\n");
    printf("------------------------------\n");
    printf("- Enter a New Game\n");
    printf("- Search for a specific game or a list of games by criteria\n");
    printf("- Edit a game already in the inventory\n");
    printf("- Display all games in the inventory\n");
    printf("------------------------------\n\n");
}

void displayMainMenu()
{
    printf("Please select one of the following action:\n");
    printf("------------------------------\n");
    printf("1: Create\n");
    printf("2: Search by criteria\n");
    printf("3: Edit\n");
    printf("4: Display\n");
    printf("5: Exit\n");
    printf("------------------------------\n\n");
}

void displayEditMenu()
{
    printf("------------------------------\n");
    printf("1: Game Name\n");
    printf("2: Studio's Name\n");
    printf("3: Game Type\n");
    printf("4: ESRB Rating\n");
    printf("5: Metacritic Rating\n");
    printf("6: Platform Release\n");
    printf("7: Game Mode\n");
    printf("8: Selling Price\n");
    printf("9: Is the Game Digital\n");
    printf("------------------------------\n\n");  
}

void initInvPopProcess()
{
    autoGenSKU();
    populateInventory();
}

void populateInventory()
{    
    gameName();
    studioName();
    typeSelector();
    ratingSelector();
    metacritic();
    platformSelector();
    modeSelector();
    sellingPrice();
    isDigital();    
}

void displaySearchMenu()
{
    printf("------------------------------\n");
    printf("1: Search by Studio's Name\n");
    printf("2: Search by type\n");
    printf("3: Search by platform\n");
    printf("4: Search by metacritic rating\n");
    printf("5: Search by Studio's Name, platform and metacritic rating\n");
    printf("------------------------------\n\n");
}

void searchForGamesByStudio(int searchIndexcheck)
{
    char str[MAX_STRING_SIZE_LARGE];
    searchIndexcheck = 0;
    printf("\nEnter the name of a studio to search:\n");
    gets(str); 

    printf("\nInventory Filtered Display\n");
    for (int i = 0; i < k; i++)
    {
        if (stricmp(videoGames[i].studio, str) == 0)
        {
            searchIndexcheck++;
            displayFilteredInventory(i);
        }
    }

    if(searchIndexcheck == 0)
    {
        printf("------------------------------\n");
        printf("\nNo game made by this studio was found in the inventory!\n");
    }

    wait();
}

void searchForGamesByType(int searchIndexcheck)
{
    char str[MAX_STRING_SIZE_LARGE];
    userSelectedValue = searchIndexcheck = 0;

    printf("\nWhat is the type of the games you want to search for:\n");
    displayType();

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }  

    switch (userSelectedValue)
    {
        case 1:
            strcpy(str, "action"); 
            break;
        case 2:
            strcpy(str, "idle"); 
            break;
        case 3:
            strcpy(str, "role-playing"); 
            break;
        case 4:
            strcpy(str, "simulation"); 
            break;
        case 5:
            strcpy(str, "sports"); 
            break;
        case 6:
            strcpy(str, "strategy"); 
            break;
    default:
        break;
    }

    printf("\nInventory Filtered Display\n");
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < MAX_ARRAY_SIZE; j++)
        {
            if (strcmp(videoGames[i].type[j], str) == 0)
            {
                searchIndexcheck++;
                displayFilteredInventory(i);
            }
        }
    }


    if (searchIndexcheck == 0)
    {
        printf("------------------------------\n");
        printf("\nNo game with this type was found in the inventory!\n");
    }
    
    wait();
}

void searchForGamesByPlatform(int searchIndexcheck)
{
    char str[MAX_STRING_SIZE_MEDIUM];
    userSelectedValue = searchIndexcheck = 0;

    printf("\nWhat is the platform of the games you want to search for:\n");
    displayPlatform();

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }  

    switch (userSelectedValue)
    {
        case 1:
            strcpy(str, "PS"); 
            break;
        case 2:
            strcpy(str, "Xbox"); 
            break;
        case 3:
            strcpy(str, "Switch"); 
            break;
        case 4:
            strcpy(str, "PC"); 
            break;
        case 5:
            strcpy(str, "Mobile"); 
            break;
        case 6:
            strcpy(str, "Streaming"); 
            break;
    default:
        break;
    }
    
    printf("\nInventory Filtered Display\n");
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < MAX_ARRAY_SIZE; j++)
        {
            if (strcmp(videoGames[i].platform[j], str) == 0)
            {
                searchIndexcheck++;
                displayFilteredInventory(i);
            }
        }
    }

    if (searchIndexcheck == 0)
    {
        printf("------------------------------\n");
        printf("\nNo game with this platform was found in the inventory!\n");
    }

    wait();
}

void searchForGamesByMetacritic(int searchIndexcheck)
{
    float metacriticSearchValue;
    searchIndexcheck = 0;

    printf("What is the metacritic ratings of the game you are looking for:\n");

    while ((scanf("%f", &metacriticSearchValue) != 1 && (metacriticSearchValue = getchar()) != EOF && metacriticSearchValue != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }   

    printf("\nInventory Filtered Display\n");
    for (int i = 0; i < k; i++)
    {
        if (videoGames[i].metacriticRating == metacriticSearchValue)
        {
            searchIndexcheck++;
            displayFilteredInventory(i);
        }
    }

    if(searchIndexcheck == 0)
    {
        printf("------------------------------\n");
        printf("\nNo game with this metacritic was found in the inventory!\n");
    }

    wait();
}

// with more time I could have refactor all searches so that the user gets to decided which groups of filter to use instead of having one static option
void searchForGamesByMultiCriteria(int searchIndexcheck)
{
    userSelectedValue = searchIndexcheck = 0;
    char strStudio[MAX_STRING_SIZE_LARGE], strPlatform[MAX_STRING_SIZE_LARGE];
    float metacriticSearchValue;

    printf("\nEnter the name of a studio to search:\n");
    gets(strStudio); 

    printf("What is the metacritic ratings of the game you are looking for:\n");         
    while ((scanf("%f", &metacriticSearchValue) != 1 && (metacriticSearchValue = getchar()) != EOF && metacriticSearchValue != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }  

    printf("\nWhat is the platform of the games you want to search for:\n");
    displayPlatform();

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }  

    switch (userSelectedValue)
    {
        case 1:
            strcpy(strPlatform, "PS"); 
            break;
        case 2:
            strcpy(strPlatform, "Xbox"); 
            break;
        case 3:
            strcpy(strPlatform, "Switch"); 
            break;
        case 4:
            strcpy(strPlatform, "PC"); 
            break;
        case 5:
            strcpy(strPlatform, "Mobile"); 
            break;
        case 6:
            strcpy(strPlatform, "Streaming"); 
            break;
    default:
        break;
    }          

    printf("\nInventory Filtered Display\n");
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < MAX_ARRAY_SIZE; j++)
        {
            if (stricmp(videoGames[i].studio, strStudio) == 0 && videoGames[i].metacriticRating == metacriticSearchValue && strcmp(videoGames[i].platform[j], strPlatform) == 0)
            {
                searchIndexcheck++;              
                displayFilteredInventory(i);  
            }
        }   
    }

    if(searchIndexcheck == 0)
    {
        printf("------------------------------\n");
        printf("\nNo games with these criterias were found in the inventory!\n");
    }

    wait();
}

void refreshCurrentStructValue(int i)
{
    videoGame = videoGames[i];
    
    /*
    videoGame.sku = videoGames[i].sku;
    strcpy(videoGame.name, videoGames[i].name); 
    strcpy(videoGame.studio, videoGames[i].studio);

    for (int j = 0; j < videoGames[i].typeNum; j++)
    {
        strcpy(videoGame.type[j], videoGames[i].type[j]);
    }

    strcpy(videoGame.rating, videoGames[i].rating);
    videoGame.metacriticRating = videoGames[i].metacriticRating;
    strcpy(videoGame.metacriticRatingMeaning, videoGames[i].metacriticRatingMeaning);
    videoGame.platformNum = videoGames[i].platformNum;

    for (int j = 0; j < videoGames[i].platformNum; j++)
    {
        strcpy(videoGame.platform[j], videoGames[i].platform[j]);
    }
    
    videoGame.modeNum = videoGames[i].modeNum;

    for (int j = 0; j < videoGames[i].modeNum; j++)
    {
        strcpy(videoGame.modes[j], videoGames[i].modes[j]);
    }

    videoGame.sellingPrice = videoGames[i].sellingPrice;
    videoGame.digital = videoGames[i].digital;
    */
}

void editEntry()
{
    userSelectedValue = 0;
    printf("\nWhat is the SKU of the game you want to edit?\n");

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    } 

    for (int i = 0; i < k; i++)
    {
        if (videoGames[i].sku == userSelectedValue)
        {
            fflush(stdin);
            printf("\nInventory Filtered Display\n");
            displayFilteredInventory(i);
            refreshCurrentStructValue(i);

            printf("\nDo you want to completely reedit this entry?\n");

            do
            {
                confirmDisplay();
            } while (userSelectedValue < 1 || userSelectedValue > 2);          

            if (userSelectedValue == 1)
            {
                fflush(stdin);
                populateInventory();
                updateUserData(i);
            }
            else
            {            
                printf("\nWhich field do you want to edit?\n");

                userSelectedValue = 0;
                displayEditMenu();

                while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 9))
                {    
                    printf("Invalid input! \n");
                    printf("Select the requested input!\n\n");   
                }  
                
                switch (userSelectedValue)
                {
                    case 1:
                        gameName();
                        break;
                    case 2:
                        studioName();
                        break;
                    case 3:
                        typeSelector();
                        break;
                    case 4:
                        ratingSelector();
                        break;
                    case 5:
                        metacritic();
                        break;
                    case 6:
                        platformSelector();
                        break;
                    case 7:
                        modeSelector();
                        break;
                    case 8:
                        sellingPrice();
                        break;
                    case 9:
                        isDigital(); 
                        break;
                    default:
                        break;
                }
                updateUserData(i);
                break;
            }  
        }     
    }
}

void displayFilteredInventory(int i)
{
    displayGameStruct(i);
}

void displayFullInventory()
{
    printf("\nInventory Full Display\n");

    if (k == 0)
    {
        printf("The inventory is empty!\n");
    }
    else
    {
        for (int i = 0; i < k; i++)
        {
            printf("Game %d\n", i+1);
            displayGameStruct(i);
        }
    }
    wait();
}

void displayGameStruct(int i)
{
    printf("------------------------------\n");
    printf("Game SKU: 21lc12fa%d\n", videoGames[i].sku);
    printf("Game Name: %s\n", videoGames[i].name);
    printf("Game Studio: %s\n", videoGames[i].studio);

    for (int j = 0; j < videoGames[i].typeNum; j++)
    {
        printf("Game Type: %s\n", videoGames[i].type[j]);
    }

    printf("Game ESRB Rating: %s\n", videoGames[i].rating);
    printf("Game Metacritic: %d = %s\n", videoGames[i].metacriticRating, videoGames[i].metacriticRatingMeaning);   

    for (int j = 0; j < videoGames[i].platformNum; j++)
    {
        printf("Game Platforms: %s\n", videoGames[i].platform[j]); 
    }
    
    for (int j = 0; j < videoGames[i].modeNum; j++)
    {
        printf("Game Modes: %s\n", videoGames[i].modes[j]); 
    }

    printf("Game Price: $%.2f\n", videoGames[i].sellingPrice);   

    if (videoGames[i].digital == 0)
    {
        printf("Game Packaging Format: Not Digital\n"); 
    }
    else
    {
        printf("Game Packaging Format: Digital\n"); 
    }  
    printf("------------------------------\n");
}

void wait()
{
    printf("\n\n");  
    system("pause");
    printf("\n");  
}

void autoGenSKU()
{
    videoGame.sku = ++skuAutoIncrementor;
    fflush(stdin);
}

void gameName()
{
    printf("\nWhat is the name of your game:\n");
    gets(videoGame.name);    
}

void studioName()
{
    printf("\nWhat is the name of your studio:\n");
    gets(videoGame.studio);    
}

void displayType()
{
    printf("------------------------------\n");
    printf("1 = action\n");
    printf("2 = idle\n");
    printf("3 = role-playing\n");
    printf("4 = simulation\n");
    printf("5 = sports\n");
    printf("6 = strategy\n");   
    printf("------------------------------\n");
}

void typeSelector()
{
    videoGame.typeNum = 0;
    unsigned short selections[6];
    printf("\nSelect how many types your game have:\n");

    while ((scanf("%hu", & videoGame.typeNum) != 1 && (videoGame.typeNum = getchar()) != EOF && videoGame.typeNum != '\n') || (videoGame.typeNum < 1 || videoGame.typeNum > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }    

    for (int i = 0; i < videoGame.typeNum; i++)
    {
        userSelectedValue = 0;

        printf("\nSelect the type %d of your game from the list below:\n", i+1);
        displayType();
    
        while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6)
            || (userSelectedValue == selections[0] || userSelectedValue == selections[1] || userSelectedValue == selections[2] || userSelectedValue == selections[3] || userSelectedValue == selections[4] || userSelectedValue == selections[5]))
        {    
            printf("Invalid input! \n");
            printf("Select the requested input or choose another platform.\n\n");   
        }    

        selections[i] = userSelectedValue; 

        switch (userSelectedValue)
        {
            case 1:
                strcpy(videoGame.type[i], "action"); 
                break;
            case 2:
                strcpy(videoGame.type[i], "idle"); 
                break;
            case 3:
                strcpy(videoGame.type[i], "role-playing"); 
                break;
            case 4:
                strcpy(videoGame.type[i], "simulation"); 
                break;
            case 5:
                strcpy(videoGame.type[i], "sports"); 
                break;
            case 6:
                strcpy(videoGame.type[i], "strategy"); 
                break;
        default:
            break;
        }
    }
}

void displayRating()
{
    printf("------------------------------\n");
    printf("1 = E\n");
    printf("2 = E10+\n");
    printf("3 = T\n");
    printf("4 = M\n");
    printf("5 = A\n");
    printf("6 = RP\n");   
    printf("------------------------------\n");
}

void ratingSelector()
{
    userSelectedValue = 0;

    printf("Select the rating of your game from the ESRB list below:\n");
    displayRating();

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }    

    switch (userSelectedValue)
    {
        case 1:
            strcpy(videoGame.rating, "E"); 
            break;
        case 2:
            strcpy(videoGame.rating, "E10+"); 
            break;
        case 3:
            strcpy(videoGame.rating, "T"); 
            break;
        case 4:
            strcpy(videoGame.rating, "M"); 
            break;
        case 5:
            strcpy(videoGame.rating, "A"); 
            break;
        case 6:
            strcpy(videoGame.rating, "RP"); 
            break;
    default:
        break;
    }
}

void metacritic()
{
    float metaRating;
    printf("What is the metacritic ratings of your game:\n");

    while ((scanf("%f", &metaRating) != 1 && (metaRating = getchar()) != EOF && metaRating != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }      

    videoGame.metacriticRating = metaRating;
    metacriticMeaning(videoGame.metacriticRating);
}

void metacriticMeaning(float metacriticSearchValue)
{
    if (metacriticSearchValue >= 0 && metacriticSearchValue < 20)
    {
        strcpy(videoGame.metacriticRatingMeaning, "Overwhelming Dislike"); 
    }
    else if(metacriticSearchValue >= 20 && metacriticSearchValue < 50)
    {
        strcpy(videoGame.metacriticRatingMeaning, "Generally Unfavorable Reviews"); 
    }
    else if(metacriticSearchValue >= 50 && metacriticSearchValue < 75)
    {
        strcpy(videoGame.metacriticRatingMeaning, "Mixed or Average Reviews");         
    }
    else if(metacriticSearchValue >= 75 && metacriticSearchValue < 90)
    {
        strcpy(videoGame.metacriticRatingMeaning, "Generally Favorable Reviews"); 
    }
    else if(metacriticSearchValue >= 90 && metacriticSearchValue <= 100)
    {
        strcpy(videoGame.metacriticRatingMeaning, "Universal Acclaim"); 
    }    
}

void displayPlatform()
{
    printf("------------------------------\n");
    printf("1 = PS\n");
    printf("2 = Xbox\n");
    printf("3 = Switch\n");
    printf("4 = PC\n");
    printf("5 = Mobile\n");
    printf("6 = Streaming\n");   
    printf("------------------------------\n");
}

void platformSelector()
{
    videoGame.platformNum = 0;
    unsigned short selections[6];
    printf("\nSelect how many platforms your game will be released on::\n");

    while ((scanf("%hu", &videoGame.platformNum) != 1 && (videoGame.platformNum = getchar()) != EOF && videoGame.platformNum != '\n') || (videoGame.platformNum < 1 || videoGame.platformNum > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }    
    
    for (int i = 0; i < videoGame.platformNum; i++)
    {  
        userSelectedValue = 0;

        printf("\nSelect platform number %d your game will be released on:\n", i+1);
        displayPlatform(i);

        while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6)
            || (userSelectedValue == selections[0] || userSelectedValue == selections[1] || userSelectedValue == selections[2] || userSelectedValue == selections[3] || userSelectedValue == selections[4] || userSelectedValue == selections[5]))
        {    
            printf("Invalid input! \n");
            printf("Select the requested input or choose another platform.\n\n");   
        }    
        
        selections[i] = userSelectedValue; 

        switch (userSelectedValue)
        {
            case 1:
                strcpy(videoGame.platform[i], "PS"); 
                break;
            case 2:
                strcpy(videoGame.platform[i], "Xbox"); 
                break;
            case 3:
                strcpy(videoGame.platform[i], "Switch"); 
                break;
            case 4:
                strcpy(videoGame.platform[i], "PC"); 
                break;
            case 5:
                strcpy(videoGame.platform[i], "Mobile"); 
                break;
            case 6:
                strcpy(videoGame.platform[i], "Streaming"); 
                break;
            default:
                break;
        }
    }
}

void displayMode()
{
    printf("------------------------------\n");
    printf("1 = Single Player\n");
    printf("2 = Local multiplayer\n");
    printf("3 = Online multiplayer\n");
    printf("------------------------------\n");
}

void modeSelector()
{
    videoGame.modeNum = 0;
    unsigned short selections[3];
    printf("\nSelect how many modes your game have:\n");

    while ((scanf("%hu", &videoGame.modeNum) != 1 && (videoGame.modeNum = getchar()) != EOF && videoGame.modeNum != '\n') || (videoGame.modeNum < 1 || videoGame.modeNum > 6))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }    
    
    for (int i = 0; i < videoGame.modeNum; i++)
    {  
        userSelectedValue = 0;

        printf("Select the %d gaming mode for your game:\n", i+1);
        displayMode();

        while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n') || (userSelectedValue < 1 || userSelectedValue > 6)
            || (userSelectedValue == selections[0] || userSelectedValue == selections[1] || userSelectedValue == selections[2]))
        {    
            printf("Invalid input! \n");
            printf("Select the requested input or choose another mode.\n\n");   
        }    
        
        selections[i] = userSelectedValue; 

        switch (userSelectedValue)
        {
            case 1:
                strcpy(videoGame.modes[i], "Single Player"); 
                break;
            case 2:
                strcpy(videoGame.modes[i], "Local multiplayer"); 
                submode(i);
                break;
            case 3:
                strcpy(videoGame.modes[i], "Online multiplayer"); 
                submode(i);
                break;
            default:
                break;
        }   
    }    
}

void submode(int i)
{
    printf("\nDoes your game include a cooperative mode?\n");

    do
    {
        confirmDisplay();
    } while (userSelectedValue < 1 || userSelectedValue > 2);

    char coopString[] = " + Coop";
    if (userSelectedValue == 1)
    {           
        strcat(videoGame.modes[i], coopString);
    }
}

void sellingPrice()
{
    float price;
    printf("\nWhat is the price of your game:\n");

    while ((scanf("%f", &price) != 1 && (price = getchar()) != EOF && price != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }  

    videoGame.sellingPrice = price;
}

void isDigital()
{
    printf("\nIs your game digital:\n");

    do
    {
        confirmDisplay();
    } while (userSelectedValue < 1 || userSelectedValue > 2);

    switch (userSelectedValue)
    {
        case 1:
            videoGame.digital = true;
            break;
        case 2:
            videoGame.digital = false;
            break;
        default:
            break;
    }  
}

void recordUserData()
{
    videoGames[k] = videoGame;
    k++;
}

void updateUserData(int i)
{
    videoGames[i] = videoGame;  
}

void confirmDisplay()
{
    userSelectedValue = 0;   
    
    printf("------------------------------\n");
    printf("1: Yes\n");
    printf("2: No\n");
    printf("------------------------------\n\n");   

    while ((scanf("%hu", &userSelectedValue) != 1 && (userSelectedValue = getchar()) != EOF && userSelectedValue != '\n'))
    {    
        printf("Invalid input! \n");
        printf("Select the requested input!\n\n");   
    }    
}

#pragma endregion Function_implementation
