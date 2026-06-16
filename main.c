#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct UserNode { // Structure to record user data in a linked list
    char username[50];
    char password[25];
    struct UserNode* next;
} UserNode;

typedef struct { // Structure to record football match results
    char home[25];
    char away[25];
    int homeGoals;
    int awayGoals;
    int homePens;
    int awayPens;
} Match;

typedef struct { // Structure to record football team data
    char group[2];
    char teamName[25];
    int wins;
    int draws;
    int losses;
    int goalsFor;
    int goalsAgainst;
    int goalDifference;
    int points;
} Team;

// Initialization of pointers to files located in the program directory
FILE *loginFile; 
FILE *matchesFile;
FILE *teamsFile;

int i, j, flag = 0; // Index variables and flags used throughout the program
int indicesC[4], indexS; // Variables used in sorting functions, declared globally

int loginMenu(); // Menu presented for user authentication
void populateUserList(UserNode**); // Fills the linked list with registered users for login
int authenticateUser(UserNode*, char*, char*); // Checks user credentials against the list
void printUserList(UserNode*); // Displays the registered users list (Admin only)
void loadData(Match*, Team*); // Populates arrays with matches and teams from files
void viewTeams(Team*); // Allows viewing stored team information
void viewMatches(Match*, Team*); // Allows viewing stored match information
int viewGroupStage(Match*, Team*); // Displays group stage standings
int viewKnockoutStage(Match*, Team*); // Displays knockout stage matches
void calculateGroupStats(Match*, Team*, char*); 
void calculateStats(Match*, Team*, int*); 
void viewStatistics(Match*, Team*); 
void compareTeams(Match*, Team*); 

int main() {
    char val[2];
    int choice;
    Match FIXTURES[64];
    Team TEAMS[32];
    
    printf("Enter any character to proceed.\n");
    printf("[0 to close the program]: ");
    scanf("%s", val);
    if(strcmp(val, "0") == 0) exit(1);
    
    printf("\n#### AUTHENTICATION ####");
    if(loginMenu() == 0) exit(10);
    
    loadData(FIXTURES, TEAMS);
    printf("\n#################################### FIFA WORLD CUP - QATAR 2022 ####################################\n");
    
    do {
        printf("\n\n------------------------- Main Menu -------------------------\n"
        "\n 1) --> View imported QATAR 2022 archive\n"
        "\n 2) --> Group Stage Standings\n"
        "\n 3) --> Knockout Stage Matches\n"
        "\n 4) --> Tournament Statistics\n"
        "\n\n Type 0 to close the program.\n"
        "\n----------------------------------------------------------------------\n Choice: ");
        fflush(stdin);
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("\n\n###########------------------------- VIEW TEAMS/MATCHES -------------------------###########\n\n");
                do {
                    flag = 0;
                    printf("\n----------------------------------------------------------------------\n"
                    "\n 1) --> Teams\n"
                    "\n 2) --> Matches\n"
                    "\n\n Type 0 to return to the main menu.\n"
                    "\n----------------------------------------------------------------------\n Choice: ");
                    scanf("%d", &choice);
                    if(choice == 1) viewTeams(TEAMS);
                    else if(choice == 2) viewMatches(FIXTURES, TEAMS);
                    else if(choice == 0) {
                        choice = 3;
                        break;  
                    } else {
                        printf("Invalid choice.");
                        flag = 1;
                    }
                } while(flag == 1);
                printf("\n\n###########------------------------- END -------------------------###########\n\n");
                break;
            case 2:
                printf("\n\n###########------------------------- GROUP STAGE -------------------------###########\n\n");
                do {
                    printf("\nGroups in the FIFA WORLD CUP 2022 Group Stage: [A] [B] [C] [D] [E] [F] [G] [H]");
                } while(viewGroupStage(FIXTURES, TEAMS) == 1);
                printf("\n\n###########------------------------- END -------------------------###########\n\n");
                break;
            case 3:
                printf("\n\n###########------------------------- KNOCKOUT STAGE -------------------------###########\n\n");
                do {
                    printf("\nTeams present in the FIFA WORLD CUP 2022 Knockout Stage:\n");
                } while(viewKnockoutStage(FIXTURES, TEAMS) == 1);
                printf("\n\n###########------------------------- END -------------------------###########\n\n");
                break;
            case 4:
                printf("\n\n###########------------------------- STATISTICS -------------------------###########\n\n");
                viewStatistics(FIXTURES, TEAMS);
                printf("\n\n###########------------------------- END -------------------------###########\n\n");
                break;
            default:
                if(choice != 0) printf("\nChoice does not exist.\n");
                break;
        }
    } while(choice != 0);
    return 0;
}   

int loginMenu() {
    int choice, pin;
    char user[50], pass[25];
    UserNode* head = NULL;
    populateUserList(&head);
    
    do {
        printf("\n\n--------------------------------------------------\n"
        "\n 1) --> Login\n"
        "\n 2) --> View password archive (PIN required)\n"
        "\n\n Type 0 to close the program.\n"
        "\n------------------------------------------------------\n Choice: ");
        fflush(stdin);
        scanf("%d", &choice);
        
        switch(choice) {
        case 1:
                printf("\nUsername: ");
                scanf("%s", user);
                printf("Password: ");
                scanf("%s", pass);
                if(authenticateUser(head, user, pass) == 1) flag = 1;
                else {
                    printf("\nIncorrect username and/or password.\n");
                }
            break;
            
        case 2:
            do {
                flag = 0;
                printf("Enter PIN. \n(4 digits): ");
                fflush(stdin);
                scanf("%d", &pin);
                if(pin < 1000 || pin > 9999) {
                    printf("The entered PIN is invalid.\n");
                    flag = 2;
                }
            } while(flag != 0);
            if(pin == 1234) printUserList(head);
            else printf("\nIncorrect PIN.\n");
            break;
            
        default:
                if(choice != 0) printf("\nChoice does not exist.\n");
                break;
        }
    } while(choice != 0 && flag != 1);
    
    if(choice == 0) return 0;
    else return 1;
}

void populateUserList(UserNode** head) {
    if((loginFile = fopen("Login.txt", "r")) == NULL) {
        printf("\nError opening file.\n");
        exit(2);
    }
    while(!feof(loginFile)) {
        UserNode* node = malloc(sizeof(UserNode));
        if(node == NULL) {
            printf("Memory allocation error.");
            exit(0);
        }
        if (fscanf(loginFile,"%s %s\n", node->username, node->password) == 2) {
            node->next = (*head);
            (*head) = node;
        }
    }
    fclose(loginFile);
}

int authenticateUser(UserNode* list, char* u, char* p) {
    while(list != NULL) {
        if(strcmp(list->username, u) == 0 && strcmp(list->password, p) == 0) return 1;
        list = list->next;
    }
    return 0;
}

void printUserList(UserNode* ptr) {
    printf("\n## REGISTERED USERS ARCHIVE ##\n");
    while(ptr != NULL) {
        printf("_________________________________________________________\n");
        printf("\nUsername: %s ||| Password: %s \n", ptr->username, ptr->password);
        ptr = ptr->next;
    }
    printf("_________________________________________________________\n");
    printf("\n##############################\n");
}

void loadData(Match* q, Team* g) {
    if((teamsFile = fopen("Teams.txt", "r")) == NULL) {
        printf("\nError opening file.\n");
        exit(3);
    }
    for(i = 0; i < 32; i++) {
        fflush(stdin);
        fscanf(teamsFile,"%s %[A-Za-z ]\n", ((g+i)->group), ((g+i)->teamName));
    }
    fclose(teamsFile);
    
    if((matchesFile = fopen("Fixtures.txt", "r")) == NULL) {
        printf("\nError opening file.\n");
        exit(4);
    }
    
    for(i = 0; i < 64; i++) {
        fflush(stdin);
        fscanf(matchesFile,"%[A-Za-z ]-%[A-Za-z ]: %d-%d", ((q+i)->home), ((q+i)->away), &((q+i)->homeGoals), &((q+i)->awayGoals));
        if(i < 48 || ((q+i)->homeGoals) != ((q+i)->awayGoals)) {
            fscanf(matchesFile,"\n");
            ((q+i)->homePens) = 0;
            ((q+i)->awayPens) = 0;
        } else {
            fscanf(matchesFile,"%d-%d\n", &((q+i)->homePens), &((q+i)->awayPens));
        }
    }
    fclose(matchesFile);
}

void viewTeams(Team* g) {
    int choice;
    char targetGroup[2], ynChoice[3];
    
    do {
        printf("\n\n---------------------------- TEAMS -----------------------------------\n"
        "\n 1) --> View all teams\n"
        "\n 2) --> View all teams and their respective groups\n"
        "\n 3) --> View teams of a specific group\n"
        "\n\n Type 0 to return to the main menu.\n"
        "\n----------------------------------------------------------------------\n Choice: ");
        fflush(stdin);
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("\nAll tournament teams: \n\n");
                for(i = 0; i < 32; i++) {
                    if(i != 31) printf("%s, ", ((g+i)->teamName));
                    else printf("%s.", ((g+i)->teamName));
                }
                break;
            case 2:
                printf("\nAll tournament teams with their respective groups: \n\n");
                for(i = 0; i < 32; i++) {
                    printf("\n%-15s-----> Group %s\n", ((g+i)->teamName), ((g+i)->group));
                }
                break;
            case 3:
                do {
                    do {
                        flag = 0;
                        printf("\nGroups in the FIFA WORLD CUP 2022: [A] [B] [C] [D] [E] [F] [G] [H]");
                        printf("\nGroup to view: ");
                        scanf("%s", targetGroup);
                        targetGroup[0] = toupper(targetGroup[0]); // Safe uppercase
                        if(strcmp(targetGroup, "A")!=0 && strcmp(targetGroup, "B")!=0 && strcmp(targetGroup, "C")!=0 && strcmp(targetGroup, "D")!=0 && strcmp(targetGroup, "E")!=0 && strcmp(targetGroup, "F")!=0 && strcmp(targetGroup, "G")!=0 && strcmp(targetGroup, "H")!=0)
                            flag = 1;
                    } while(flag == 1);
                    printf("\n\nAll tournament teams belonging to Group %s: \n", targetGroup);
                    printf("----------------------------------------------------------------\n");
                    for(i = 0; i < 32; i++) {
                        if(strcmp(targetGroup, ((g+i)->group)) == 0) {
                            printf("____________________________________________________________\n");
                            printf("\n%s\n", ((g+i)->teamName));
                        }
                    }
                    printf("____________________________________________________________\n");
                    printf("----------------------------------------------------------------\n");
                    do {
                        printf("\nDo you want to perform another view?\n");
                        printf("[yes/no]: ");
                        scanf("%s", ynChoice);
                    } while(strcmp(ynChoice, "yes")!=0 && strcmp(ynChoice, "no")!=0);
                    if(strcmp(ynChoice, "yes") == 0) flag = 1;
                } while(flag != 0);
                break;
            default:
                if(choice != 0) printf("\nChoice does not exist.\n");
                break;
        }
    } while(choice != 0);
}

void viewMatches(Match* q, Team* g) {
    int choice;
    char targetTeam[25];
    
    do {
        printf("\n\n---------------------------- MATCHES -----------------------------------\n"
        "\n 1) --> View all matches\n"
        "\n 2) --> View Group Stage matches\n"
        "\n 3) --> View Knockout Stage matches\n"
        "\n 4) --> View matches for a specific team\n"
        "\n\n Type 0 to return to the main menu.\n"
        "\n----------------------------------------------------------------------\n Choice: ");
        fflush(stdin);
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                printf("\nAll matches: \n\n");
                for(i = 0; i < 48; i++) {
                    for(j = 0; j < 32; j++) {
                        if(strcmp(((q+i)->home), ((g+j)->teamName)) == 0) {
                            printf("\n Group Stage | Group %s -> ", ((g+j)->group));
                        }
                    }
                    printf("%s v %s %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                }
                for(i = 48; i < 64; i++) {
                    printf("\n Knockout Stage | ");
                    if(i > 47 && i < 56) printf("Round of 16 -> ");
                    else if (i > 55 && i < 60) printf("Quarter-finals -> ");
                    else if (i > 59 && i < 62) printf("Semi-finals -> ");
                    else if (i == 62) printf("Third-place Play-off -> ");
                    else if (i == 63) printf("Final -> ");
                    
                    if(((q+i)->homeGoals) != ((q+i)->awayGoals)) {
                        printf("%s v %s %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                    } else {
                        printf("%s v %s %d - %d / pens %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals), ((q+i)->homePens), ((q+i)->awayPens));
                    }
                }
                break;
            case 2:
                printf("\nAll Group Stage matches: \n\n");
                for(i = 0; i < 48; i++) {
                    for(j = 0; j < 32; j++) {
                        if(strcmp(((q+i)->home), ((g+j)->teamName)) == 0) {
                            printf("\n Group %s -> ", ((g+j)->group));
                        }
                    }
                    printf("%s v %s %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                }
                break;
            case 3:
                printf("\nAll Knockout Stage matches: \n\n");
                for(i = 48; i < 64; i++) {
                    if(i > 47 && i < 56) printf("Round of 16 -> ");
                    else if (i > 55 && i < 60) printf("Quarter-finals -> ");
                    else if (i > 59 && i < 62) printf("Semi-finals -> ");
                    else if (i == 62) printf("Third-place Play-off -> ");
                    else if (i == 63) printf("Final -> ");
                    
                    if(((q+i)->homeGoals) != ((q+i)->awayGoals)) {
                        printf("%s v %s %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                    } else {
                        printf("%s v %s %d - %d / pens %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals), ((q+i)->homePens), ((q+i)->awayPens));
                    }
                }
                break;
            case 4:
                do {
                    printf("\nEnter the team: ");
                    fflush(stdin);
                    scanf(" %[A-Za-z ]", targetTeam);
                    targetTeam[0] = toupper(targetTeam[0]);
                    flag = 0;
                    for(i = 0; i < 32 && flag == 0; i++) {
                        if(strcmp(targetTeam, ((g+i)->teamName)) == 0) {
                            j = i;
                            flag = 1;
                        }
                    }
                    if(flag == 0) printf("The team is not present in the tournament.\n");
                } while(flag == 0);
                
                for(i = 0; i < 64; i++) {
                    if(strcmp(targetTeam, ((q+i)->home)) == 0 || strcmp(targetTeam, ((q+i)->away)) == 0) {
                        if(i >= 0 && i < 48) {
                            printf("\nGroup Stage | Group %s -> ", ((g+j)->group));
                            printf("%s v %s %d - %d\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                        } else {
                            if(i > 47 && i < 56) printf("\nRound of 16 -> ");
                            else if (i > 55 && i < 60) printf("\nQuarter-finals -> ");
                            else if (i > 59 && i < 62) printf("\nSemi-finals -> ");
                            else if (i == 62) printf("\nThird-place Play-off -> ");
                            else if (i == 63) printf("\nFinal -> ");
                            
                            if(((q+i)->homeGoals) != ((q+i)->awayGoals)) {
                                printf("%s v %s %d - %d\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                            } else {
                                printf("%s v %s %d - %d / pens %d - %d\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals), ((q+i)->homePens), ((q+i)->awayPens));
                            }
                        }
                    }
                }   
                break;
            default:
                if(choice != 0) printf("\nChoice does not exist.\n");
                break;
        }
    } while(choice != 0);
}

int viewGroupStage(Match* q, Team* g) {
    char targetGroup[2], ynChoice[4];
    for(i = 0; i < 32; i++) {
        ((g+i)->wins) = 0;
        ((g+i)->draws) = 0;
        ((g+i)->losses) = 0;   
        ((g+i)->goalsFor) = 0;
        ((g+i)->goalsAgainst) = 0;
        ((g+i)->goalDifference) = 0;
        ((g+i)->points) = 0;
    }
    do {
        flag = 0;
        printf("\n Which group to view? \n");
        scanf("%s", targetGroup);
        targetGroup[0] = toupper(targetGroup[0]); // Safe uppercase
        if(strcmp(targetGroup, "A")!=0 && strcmp(targetGroup, "B")!=0 && strcmp(targetGroup, "C")!=0 && strcmp(targetGroup, "D")!=0 && strcmp(targetGroup, "E")!=0 && strcmp(targetGroup, "F")!=0 && strcmp(targetGroup, "G")!=0 && strcmp(targetGroup, "H")!=0)
            flag = 1;
    } while(flag == 1);
    
    calculateGroupStats(q, g, targetGroup);
    
    printf("------------ GROUP %s -------------------------------------------\n", targetGroup);
    printf("TEAM                    W  D  L  GF GA GD Pts.\n");
    j = 1;
    for(i = 0; i < 4; i++) {
        if (((g+indicesC[i])->goalDifference) >= 0) {
            printf("%d. %-15s          %d  %d  %d  %d  %d  +%d  %d\n", j, ((g+indicesC[i])->teamName), ((g+indicesC[i])->wins), ((g+indicesC[i])->draws), ((g+indicesC[i])->losses), ((g+indicesC[i])->goalsFor), ((g+indicesC[i])->goalsAgainst), ((g+indicesC[i])->goalDifference), ((g+indicesC[i])->points));   
        } else {
            printf("%d. %-15s          %d  %d  %d  %d  %d  %d  %d\n", j, ((g+indicesC[i])->teamName), ((g+indicesC[i])->wins), ((g+indicesC[i])->draws), ((g+indicesC[i])->losses), ((g+indicesC[i])->goalsFor), ((g+indicesC[i])->goalsAgainst), ((g+indicesC[i])->goalDifference), ((g+indicesC[i])->points));
        }
        j++;
    }
    printf("----------------------------------------------------------------\n");
    printf("\nLEGEND \nW: WINS \nD: DRAWS \nL: LOSSES \nGF: GOALS FOR \nGA: GOALS AGAINST \nGD: GOAL DIFFERENCE \nPts.: POINTS\n");
    
    do {
        printf("\nDo you want to perform another view?\n");
        printf("[yes/no]: ");
        scanf("%s", ynChoice);
    } while(strcmp(ynChoice, "yes")!=0 && strcmp(ynChoice, "no")!=0);
    if(strcmp(ynChoice, "yes") == 0) return 1;
    else return 0;   
}

int viewKnockoutStage(Match* q, Team* g) {
    char phase[15], ynChoice[4];
    int koIndex[16];
    int k = 0, x, localFlag;
    
    for(i = 48; i < 64; i++) {
        for(j = 0; j < 32; j++) {
            localFlag = 0;
            if(strcmp(((q+i)->home), ((g+j)->teamName)) == 0 || strcmp(((q+i)->away), ((g+j)->teamName)) == 0) {
                if(k != 0) {
                    for(x = 0; x < k; x++) {
                        if(koIndex[x] == j) {
                            localFlag = 1;
                        }
                    }
                }
                if(localFlag != 1) {
                    koIndex[k] = j;
                    k++;
                }
            }
        }
    }
    for(i = 0; i < 16; i++) {
        if (i != 15) printf("%s, ", (g+koIndex[i])->teamName);
        else printf("%s.\n", (g+koIndex[i])->teamName);
    }
    
    do {
        flag = 0;
        printf("\nWhich phase to view? [Round16, Quarters, Semis, Finals]\n");
        scanf("%s", phase);
        
        if(strcmp(phase, "Round16") == 0) {
            i = 48;
            j = 56;
            printf("---------------------------------------------------\n\n");
        }
        else if(strcmp(phase, "Quarters") == 0) {
            i = 56;
            j = 60;
            printf("---------------------------------------------------\n\n");
        }
        else if(strcmp(phase, "Semis") == 0) {
            i = 60;
            j = 62;
            printf("---------------------------------------------------\n\n");
        }
        else if(strcmp(phase, "Finals") == 0) {
            i = 62;
            j = 64;
            printf("---------------------------------------------------\n\n");
            printf("(Third-place Play-off) ");
        }
        else {
            flag = 1;
        }
        if(flag == 0) {
            while(i < j) {
                if(((q+i)->homeGoals) != ((q+i)->awayGoals)) {
                    printf("%s v %s %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals));
                } else {
                    printf("%s v %s %d - %d / pens %d - %d\n\n", ((q+i)->home), ((q+i)->away), ((q+i)->homeGoals), ((q+i)->awayGoals), ((q+i)->homePens), ((q+i)->awayPens));
                }
                i++;
            }
            printf("---------------------------------------------------\n");
        }
    } while(flag == 1);
    
    do {
        printf("\nDo you want to perform another view?\n");
        printf("[yes/no]: ");
        scanf("%s", ynChoice);
    } while(strcmp(ynChoice, "yes")!=0 && strcmp(ynChoice, "no")!=0);
    if(strcmp(ynChoice, "yes") == 0) return 1;
    else return 0;   
}

void calculateGroupStats(Match* q, Team* g, char* groupStr) {
    for(i = 0; i < 48; i++) {
        for(j = 0; j < 32; j++) {
            if(strcmp(((q+i)->home), ((g+j)->teamName)) == 0 && strcmp(((g+j)->group), groupStr) == 0) {
                if(((q+i)->homeGoals) > ((q+i)->awayGoals)) {
                    ((g+j)->wins)++;    
                    ((g+j)->points) += 3;
                }
                else if(((q+i)->homeGoals) < ((q+i)->awayGoals)) {
                    ((g+j)->losses)++;  
                }
                else if(((q+i)->homeGoals) == ((q+i)->awayGoals)) {
                    ((g+j)->draws)++;
                    ((g+j)->points)++;
                }
                
                ((g+j)->goalsFor) += ((q+i)->homeGoals);
                ((g+j)->goalsAgainst) += ((q+i)->awayGoals);
                ((g+j)->goalDifference) += (((q+i)->homeGoals) - ((q+i)->awayGoals));
            }
                    
            else if(strcmp(((q+i)->away), ((g+j)->teamName)) == 0 && strcmp(((g+j)->group), groupStr) == 0) {
                if(((q+i)->awayGoals) > ((q+i)->homeGoals)) {
                    ((g+j)->wins)++;
                    ((g+j)->points) += 3;
                }
                else if(((q+i)->awayGoals) < ((q+i)->homeGoals)) {
                    ((g+j)->losses)++;
                }
                else if(((q+i)->awayGoals) == ((q+i)->homeGoals)) {
                    ((g+j)->draws)++;
                    ((g+j)->points)++;
                }
                
                ((g+j)->goalsFor) += ((q+i)->awayGoals);
                ((g+j)->goalsAgainst) += ((q+i)->homeGoals);
                ((g+j)->goalDifference) += (((q+i)->awayGoals) - ((q+i)->homeGoals));
            }
        }       
    }
    j = 0;
    for(i = 0; i < 32; i++) {
        if(strcmp(((g+i)->group), groupStr) == 0) {
            indicesC[j] = i;
            j++;    
        }
    }
    
    // Bubble sort for standings
    j = 0;
    for(i = 0; j < 8; i++) {
        if(i == 3) i = 0;
        if((((g+indicesC[i])->points) < ((g+indicesC[i+1])->points)) || 
            (((g+indicesC[i])->points) == ((g+indicesC[i+1])->points) && ((g+indicesC[i])->goalDifference) < ((g+indicesC[i+1])->goalDifference)) ||
            (((g+indicesC[i])->points) == ((g+indicesC[i+1])->points) && ((g+indicesC[i])->goalDifference) == ((g+indicesC[i+1])->goalDifference) && ((g+indicesC[i])->goalsFor) < ((g+indicesC[i+1])->goalsFor))
            ){
            indexS = indicesC[i];
            indicesC[i] = indicesC[i+1];
            indicesC[i+1] = indexS;
        }
    j++;    
    }           
}

void calculateStats(Match* q, Team* g, int* phase) {
    int x = 0, y = 64;
    for(i = 0; i < 32; i++) {
        ((g+i)->wins) = 0;
        ((g+i)->draws) = 0;
        ((g+i)->losses) = 0;   
        ((g+i)->goalsFor) = 0;
        ((g+i)->goalsAgainst) = 0;
        ((g+i)->goalDifference) = 0;
        ((g+i)->points) = 0;
    }
    if(*phase == 1) {
        x = 0;
        y = 48;
    }
    else if(*phase == 2) {
        x = 48;
        y = 64;
    }
    else if (*phase == 3) {
        x = 0;
        y = 64;
    }
    for(i = x; i < y; i++) {
        for(j = 0; j < 32; j++) {
            if(strcmp(((q+i)->home), ((g+j)->teamName)) == 0) {
                if(((q+i)->homeGoals) > ((q+i)->awayGoals)) {
                    ((g+j)->wins)++;    
                    if(i >= 0 && i < 48) ((g+j)->points) += 3;
                }
                else if(((q+i)->homeGoals) < ((q+i)->awayGoals)) {
                    ((g+j)->losses)++;  
                }
                else if(((q+i)->homeGoals) == ((q+i)->awayGoals)) {
                    ((g+j)->draws)++;
                    if(i >= 0 && i < 48) ((g+j)->points)++;
                }
                
                ((g+j)->goalsFor) += ((q+i)->homeGoals);
                ((g+j)->goalsAgainst) += ((q+i)->awayGoals);
                if(i >= 0 && i < 48) ((g+j)->goalDifference) += (((q+i)->homeGoals) - ((q+i)->awayGoals));
            }
                    
            else if(strcmp(((q+i)->away), ((g+j)->teamName)) == 0) {
                if(((q+i)->awayGoals) > ((q+i)->homeGoals)) {
                    ((g+j)->wins)++;
                    if(i >= 0 && i < 48) ((g+j)->points) += 3;
                }
                else if(((q+i)->awayGoals) < ((q+i)->homeGoals)) {
                    ((g+j)->losses)++;
                }
                else if(((q+i)->awayGoals) == ((q+i)->homeGoals)) {
                    ((g+j)->draws)++;
                    if(i >= 0 && i < 48) ((g+j)->points)++;
                }
                
                ((g+j)->goalsFor) += ((q+i)->awayGoals);
                ((g+j)->goalsAgainst) += ((q+i)->homeGoals);
                if(i >= 0 && i < 48) ((g+j)->goalDifference) += (((q+i)->awayGoals) - ((q+i)->homeGoals));
            }       
        }
    }
}

void viewStatistics(Match* q, Team* g) {
    int sceltam, vis, phase;
    do {
        printf("\n\n---------------------------- STATISTICS -----------------------------------\n"
        "\n 1) --> View all statistics of all tournament teams\n"
        "\n 2) --> View Group Stage statistics of all teams\n"
        "\n 3) --> View Knockout Stage statistics of all teams\n"
        "\n 4) --> Team comparison statistics\n"
        "\n\n Type 0 to return to the main menu.\n"
        "\n----------------------------------------------------------------------\n Choice: ");
        fflush(stdin);
        scanf("%d", &sceltam);
        switch(sceltam) {
            case 1:
                phase = 3;            
                calculateStats(q, g, &phase);
                
                do {
                    printf("\nNormal or Compact view? (enter number)\n [1 - Normal / 2 - Compact]: ");
                    fflush(stdin);
                    scanf("%d", &vis);
                } while(vis < 1 || vis > 2);
                
                if(vis == 1) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    for(i = 0; i < 32; i++) {
                        if (((g+i)->goalDifference) >= 0) {
                            printf("-----------------------------------------\n");
                            printf("TEAM: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE(group %s): +%d  \nPOINTS(group %s): %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->group), ((g+i)->goalDifference), ((g+i)->group), ((g+i)->points));  
                            printf("-----------------------------------------\n");
                        } else {
                            printf("-----------------------------------------\n");
                            printf("TEAM: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE(group %s): %d  \nPOINTS(group %s): %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->group), ((g+i)->goalDifference), ((g+i)->group), ((g+i)->points));   
                            printf("-----------------------------------------\n");
                        }
                    }
                }
                else if(vis == 2) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    printf("TEAM                     W  D  L  GF GA GD Pts.\n\n");
                    for(i = 0; i < 32; i++) {
                        printf("____________________________________________________________\n");
                        if (((g+i)->goalDifference) >= 0) {
                            printf("%-15s          %d  %d  %d  %d  %d  +%d  %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));  
                        } else {
                            printf("%-15s          %d  %d  %d  %d  %d  %d  %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));                       
                        }
                    }
                    printf("____________________________________________________________\n");
                    printf("\nLEGEND \nW: WINS \nD: DRAWS \nL: LOSSES \nGF: GOALS FOR \nGA: GOALS AGAINST \nGD: GOAL DIFFERENCE \nPts.: POINTS\n");
                }
                printf("----------------------------------------------------------------\n");
                break;
                
            case 2:
                phase = 1;            
                calculateStats(q, g, &phase);
                
                do {
                    printf("\nNormal or Compact view? (enter number)\n [1 - Normal / 2 - Compact]: ");
                    fflush(stdin);
                    scanf("%d", &vis);
                } while(vis < 1 || vis > 2);
                
                if(vis == 1) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    for(i = 0; i < 32; i++) {
                        if (((g+i)->goalDifference) >= 0) {
                            printf("-----------------------------------------\n");
                            printf("TEAM: %s \nGROUP: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE: +%d  \nPOINTS: %d\n", ((g+i)->teamName), ((g+i)->group), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));    
                            printf("-----------------------------------------\n");
                        } else {
                            printf("-----------------------------------------\n");
                            printf("TEAM: %s \nGROUP: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE: %d  \nPOINTS: %d\n", ((g+i)->teamName), ((g+i)->group), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points)); 
                            printf("-----------------------------------------\n");
                        }
                    }
                }
                else if(vis == 2) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    printf("TEAM             GROUP                W  D  L  GF GA GD Pts.\n");
                    for(i = 0; i < 32; i++) {
                        printf("____________________________________________________________\n");
                        if (((g+i)->goalDifference) >= 0) {
                            printf("%-15s     %s                 %d  %d  %d  %d  %d  +%d  %d\n", ((g+i)->teamName), ((g+i)->group), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));   
                        } else {
                            printf("%-15s     %s                 %d  %d  %d  %d  %d  %d  %d\n", ((g+i)->teamName), ((g+i)->group), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));
                        }
                    }
                    printf("____________________________________________________________\n");
                    printf("\nLEGEND \nW: WINS \nD: DRAWS \nL: LOSSES \nGF: GOALS FOR \nGA: GOALS AGAINST \nGD: GOAL DIFFERENCE \nPts.: POINTS\n");
                }
                printf("----------------------------------------------------------------\n");
                break;
                
            case 3:
                phase = 2;            
                calculateStats(q, g, &phase);
                
                do {
                printf("\nNormal or Compact view? (enter number)\n [1 - Normal / 2 - Compact]: ");
                fflush(stdin);
                scanf("%d", &vis);
                } while(vis < 1 || vis > 2);
                
                if(vis == 1) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    for(i = 0; i < 32; i++) {
                        if(((g+i)->wins) > 0 || ((g+i)->draws) > 0 || ((g+i)->losses) > 0) {
                            printf("-----------------------------------------\n");
                            printf("TEAM: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d \n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst));   
                            printf("-----------------------------------------\n");
                        }
                    }
                }
                else if(vis == 2) {
                    printf("------------ TEAM STATISTICS -----------------------------\n");
                    printf("TEAM                     W  D  L  GF GA \n\n");
                    for(i = 0; i < 32; i++) {
                        if(((g+i)->wins) > 0 || ((g+i)->draws) > 0 || ((g+i)->losses) > 0) {
                            printf("____________________________________________________________\n");
                            printf("%-15s          %d  %d  %d  %d  %d \n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst));   
                        }
                    }
                    printf("____________________________________________________________\n");
                    printf("\nLEGEND \nW: WINS \nD: DRAWS \nL: LOSSES \nGF: GOALS FOR \nGA: GOALS AGAINST\n");
                }
                printf("----------------------------------------------------------------\n");
                break;
            
            case 4:
                phase = 3;            
                calculateStats(q, g, &phase);
                compareTeams(q, g);
                break;  
            default:
                if(sceltam != 0) printf("\nChoice does not exist.\n");
                break;
        }
    } while(sceltam != 0);
}

void compareTeams(Match* q, Team* g) {
    int vis, n;
    do {
     printf("How many teams to compare?\n (from 1 to 32): ");
     fflush(stdin);
     scanf("%d", &n);
    } while(n < 1 && n > 32);
    
    char targetTeams[n][25];
    for(j = 0; j < n; j++) {
        do {
            printf("\nEnter team %d: ", j+1);
            fflush(stdin);
            scanf(" %[A-Za-z ]", targetTeams[j]);
            targetTeams[j][0] = toupper(targetTeams[j][0]);
            flag = 0;
            for(i = 0; i < 32 && flag == 0; i++) {
                if(strcmp(targetTeams[j], ((g+i)->teamName)) == 0) {
                    flag = 1;
                }
            }
            if(flag == 0) printf("The team is not present in the tournament.\n");
        } while(flag == 0);
    }
    do {
        printf("\nNormal or Compact view? (enter number)\n [1 - Normal / 2 - Compact]: ");
        fflush(stdin);
        scanf("%d", &vis);
    } while(vis < 1 || vis > 2);
    
    if(vis == 1) {
        printf("------------ TEAM STATISTICS -----------------------------\n");
        for(j = 0; j < n; j++) { 
            for(i = 0; i < 32; i++) {       
                if(strcmp(targetTeams[j], ((g+i)->teamName)) == 0) {
                    if (((g+i)->goalDifference) >= 0) {
                        printf("-----------------------------------------\n");
                        printf("TEAM: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE(group %s): +%d  \nPOINTS(group %s): %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->group), ((g+i)->goalDifference), ((g+i)->group), ((g+i)->points));  
                        printf("-----------------------------------------\n");
                    } else {
                        printf("-----------------------------------------\n");
                        printf("TEAM: %s \nWINS: %d  \nDRAWS: %d  \nLOSSES: %d  \nGOALS FOR: %d  \nGOALS AGAINST: %d  \nGOAL DIFFERENCE(group %s): %d  \nPOINTS(group %s): %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->group), ((g+i)->goalDifference), ((g+i)->group), ((g+i)->points));   
                        printf("-----------------------------------------\n");
                    }
                }
            }
        }
    }
    else if(vis == 2) {
        printf("------------ TEAM STATISTICS -----------------------------\n");
        printf("TEAM                     W  D  L  GF GA GD Pts.\n\n");
        for(j = 0; j < n; j++) { 
            for(i = 0; i < 32; i++) {       
                if(strcmp(targetTeams[j], ((g+i)->teamName)) == 0) {
                    printf("____________________________________________________________\n");
                    if (((g+i)->goalDifference) >= 0) {
                        printf("%-15s          %d  %d  %d  %d  %d  +%d  %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));                      
                    } else {
                        printf("%-15s          %d  %d  %d  %d  %d  %d  %d\n", ((g+i)->teamName), ((g+i)->wins), ((g+i)->draws), ((g+i)->losses), ((g+i)->goalsFor), ((g+i)->goalsAgainst), ((g+i)->goalDifference), ((g+i)->points));
                    }
                }
            }
        }
        printf("____________________________________________________________\n");
        printf("\nLEGEND \nW: WINS \nD: DRAWS \nL: LOSSES \nGF: GOALS FOR \nGA: GOALS AGAINST \nGD: GOAL DIFFERENCE(Group Stage) \nPts.: POINTS(Group Stage)\n");
    }
    printf("----------------------------------------------------------------\n");
}