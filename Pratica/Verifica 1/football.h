#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 100

// Player represents a single football player
typedef struct
{
    // The player name
    char *name;

    // The number of the player
    int number;
} Player;

char *strdup(const char *str)
{
    if(str == NULL)
    {
        return NULL;
    }
    char *t = (char *)malloc(sizeof(char)*strlen(str)+1);
    if(t == NULL)
    {
        return NULL;
    }
    strcpy(t,str);
    return t;
}

/**
 * createPlayer creates a valid Player entity.
 *
 * @param name The name of the player, must not be empty or NULL
 * @param number The number of the player, must be > 0
 *
 * @returns The valid player, or NULL in case of any error
 */
Player *createPlayer(char *name, int number)
{
    // implement function logic here
    if(name == NULL || strlen(name)== 0 ||  number <= 0)
    {
        return NULL;
    }

    Player *player  = (Player *)malloc(sizeof(Player));
    if(player==NULL){
        free(player);
        return  NULL;
    }
    player->name = strdup(name);
    player->number = number;
    return player;
}

/**
 * clonePlayer creates a valid Player entity by copying another one.
 *
 * @param player The player to clone, must not be NULL
 *
 * @returns The cloned player, or NULL in case of any error
 */
Player *clonePlayer(Player *player)
{
    // implement function logic here
    if(player == NULL)
    {
        return NULL;
    }
    return createPlayer(player->name,player->number);
}

/**
 * destroyPlayer destroys a previously created Player entity and deallocates
 * it completely.
 *
 * @param player The player to destroy, must not be NULL
 *
 * @returns -1 in case of any error, 0 otherwise
 */
int destroyPlayer(Player *player)
{
    // implement function logic here
    if(player == NULL)
    {
        return -1;
    }
    free(player->name);
    free(player);
    return 0;
}

/**
 * serializePlayer serializes the player into the specified file.
 *
 * @param player The player to serialize, must not be NULL
 * @param out_file The output file to serialize into, must not be NULL
 *
 * @returns -1 in case of any error, 0 otherwise
 */
int serializePlayer(Player *player, FILE *out_file)
{
    // implement function logic here
    if(player == NULL || out_file == NULL)
    {
        return -1;
    }
    fprintf(out_file,"%s %d\n",player->name,player->number);
    return 0;
}

/**
 * deserializePlayer deserializes the player from the specified file.
 *
 * @param in_file The output file to deserialize from, must not be NULL
 *
 * @returns The deserialized player, or NULL in case of any error
 */
Player *deserializePlayer(FILE *in_file)
{
    // implement function logic here

    return NULL;
}

// Team represents a football team
typedef struct
{
    // The name of the team
    char *name;

    // The array of members of the team
    Player *members;

    // The team members count
    int members_count;
} Team;

/**
 * createTeam creates a valid Team entity.
 *
 * @param name The name of the team, must not be empty or NULL
 * @param members The array of members, must not be NULL
 * @param members_count The number of the members of the team, must be > 0
 *
 * @returns The valid team, or NULL in case of any error
 */
Team *createTeam(char *name, Player *members, int members_count)
{
    // implement function logic here
    if(name == NULL || strlen(name)==0 || members == NULL || members_count<=0)
    {
        return NULL;
    }
    Team *team = (Team *)malloc(sizeof(Team));
    if(team == NULL)
    {
        return NULL;
    }
    team->members_count = members_count;
    team->name = strdup(name);
    if(name == NULL)
    {
        free(team);
        return NULL;
    }
    team->members = (Player *)malloc(sizeof(Player) * members_count);
    for(int i = 0; i < members_count; i++)
    {
        Player *t = createPlayer((members + i)->name,(members +i)->number);
        if(t == NULL)
        {
            free(team->members);
            free(team->name);
            free(team);
        }
        *(team->members +i) = *t;
    }
    return team;
}

/**
 * cloneTeam creates a valid Team entity by copying another one.
 *
 * @param team The team to clone, must not be NULL
 *
 * @returns The cloned team, or NULL in case of any error
 */
Team *cloneTeam(Team *team)
{
    // implement function logic here
    if(team == NULL)
    {
        return NULL;
    }
    return createTeam(team->name,team->members,team->members_count);
}

/**
 * destroyTeam destroys a previously created Team entity and deallocates
 * it completely.
 *
 * @param team The team to destroy, must not be NULL
 *
 * @returns -1 in case of any error, 0 otherwise
 */
int destroyTeam(Team *team)
{
    // implement function logic here
    if(team == NULL)
    {
        return -1;
    }
    for(int i = 0; i<team->members_count; i++)
    {
        destroyPlayer(team->members +i);
    }
    free(team->name);
    free(team);

    return 0;
}

/**
 * serializeTeam serializes the team into the specified file.
 *
 * @param team The team to serialize, must not be NULL
 * @param out_file The output file to serialize into, must not be NULL
 *
 * @returns -1 in case of any error, 0 otherwise
 */
int serializeTeam(Team *team, FILE *out_file)
{
    // implement function logic here
    if(team == NULL || out_file == NULL)
    {
        return -1;
    }
    fprintf(out_file,"%s %d\n",team->name,team->members_count);
    for(int i = 0; i < team->members_count; i++)
    {

        Player *temp = (Player *)malloc(sizeof(Player));
        temp = clonePlayer(team->members + i);
        fprintf(out_file,"%s %d\n",temp->name,temp->number);
    }
    return 0;

}

/**
 * deserializeTeam deserializes the team from the specified file.
 *
 * @param in_file The output file to deserialize from, must not be NULL
 *
 * @returns The deserialized team, or NULL in case of any error
 */
Team *deserializeTeam(FILE *in_file)
{
    // implement function logic here

    return NULL;
}
