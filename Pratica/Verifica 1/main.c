#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <string.h>

#include "testing.h"

#include "football.h"

#define MAX_LENGTH 100

void testCreatePlayer();
void testClonePlayer();
void testDestroyPlayer();
void testSerializePlayer();
void testDeserializePlayer();

void testCreateTeam();
void testCloneTeam();
void testDestroyTeam();
void testSerializeTeam();
void testDeserializeTeam();

int main(void) {
  testCreatePlayer();
  testClonePlayer();
  testDestroyPlayer();
  testSerializePlayer();
  //testDeserializePlayer();

  testCreateTeam();
  testCloneTeam();
  testDestroyTeam();
  testSerializeTeam();
  //testDeserializeTeam();

  return 0;
}

void testCreatePlayer() {
  openTestGroup("createPlayer(...)");

  typedef struct {
    char *test_name;
    char *name_param;
    int number_param;
  } createPlayerErrorCase;

  const createPlayerErrorCase test_cases[] = {
      (createPlayerErrorCase){"createPlayer(NULL, -1)", NULL, -1},
      (createPlayerErrorCase){"createPlayer(\"Testing\", -1)", "Testing", -1},
      (createPlayerErrorCase){"createPlayer(\"\", 0)", "", 0},
      (createPlayerErrorCase){"createPlayer(\"Testing\", 0)", "Testing", 0},
  };

  for (int i = 0; i < 4; i++) {
    const createPlayerErrorCase test_case = test_cases[i];
    Player *result = createPlayer(test_case.name_param, test_case.number_param);

    require(test_cases[i].test_name, result == NULL);
  }

  Player *result = createPlayer("Testing", 10);

  require("createPlayer(\"Testing\", 10)", result != NULL);
  require("createPlayer(\"Testing\", 10) - allocation", isAllocated(result));
  require("createPlayer(\"Testing\", 10) - name",
          strcmp(result->name, "Testing") == 0);
  require("createPlayer(\"Testing\", 10) - name (allocation)",
          isAllocated(result->name));
  require("createPlayer(\"Testing\", 10) - number", result->number == 10);

  free(result->name);
  free(result);

  closeTestGroup();
}

void testClonePlayer() {
  openTestGroup("clonePlayer(...)");

  Player *clone = clonePlayer(NULL);
  require("clonePlayer(NULL)", clone == NULL);

  Player player = {"Testing", 10};
  clone = clonePlayer(&player);
  require("clonePlayer(player)", clone != NULL);
  require("clonePlayer(player) - allocation", isAllocated(clone));
  require("clonePlayer(player) - name", strcmp(clone->name, player.name) == 0);
  require("clonePlayer(player) - name (allocation)", isAllocated(clone->name));
  require("clonePlayer(player) - number", clone->number == player.number);

  free(clone->name);
  free(clone);

  closeTestGroup();
}

void testDestroyPlayer() {
  openTestGroup("destroyPlayer(...)");

  int result = destroyPlayer(NULL);
  require("destroyPlayer(NULL)", result == -1);

  Player *player = (Player *)malloc(sizeof(Player));
  char *player_name = (char *)malloc(sizeof(char) * strlen("Testing") + 1);
  strcpy(player_name, "Testing");

  player->name = player_name;
  player->number = 10;

  result = destroyPlayer(player);
  require("destroyPlayer(player)", result == 0);
  require("destroyPlayer(player) - de-allocation",
          !isAllocated(player) && !isAllocated(player_name));

  closeTestGroup();
}

void testSerializePlayer() {
  openTestGroup("serializePlayer(...)");

  Player player = {"Testing", 10};

  FILE *fp = fopen("test-execution-serialize.txt", "w");
  require("(before)Test file must be opened for tests to work", fp != NULL);

  int result = serializePlayer(NULL, NULL);
  require("serializePlayer(NULL, NULL)", result == -1);

  result = serializePlayer(&player, NULL);
  require("serializePlayer(player, NULL)", result == -1);

  result = serializePlayer(NULL, fp);
  require("serializePlayer(NULL, fp)", result == -1);

  result = serializePlayer(&player, fp);
  require("serializePlayer(player, fp)", result == 0);

  fclose(fp);

  // Testing file contents for a correct serialization
  fp = fopen("test-execution-serialize.txt", "r");
  char file_row[MAX_LENGTH];
  fgets(file_row, MAX_LENGTH, fp);
  require("serializePlayer(player, fp) - file contents",
          strcmp(file_row, "Testing 10"));

  closeTestGroup();
}

void testDeserializePlayer() {
  openTestGroup("deserializePlayer(...)");

  Player *result = deserializePlayer(NULL);
  require("deserializePlayer(NULL)", result == NULL);

  FILE *fp = fopen("test-files/deserializePlayer-1.txt", "r");
  require("(before)test-files/deserializePlayer-1.txt file check", fp != NULL);

  result = deserializePlayer(fp);
  require("deserializePlayer(fp) - Test file 1", result == NULL);

  fclose(fp);

  fp = fopen("test-files/deserializePlayer-2.txt", "r");
  require("(before)test-files/deserializePlayer-2.txt file check", fp != NULL);

  result = deserializePlayer(fp);
  require("deserializePlayer(fp) - Test file 2", result == NULL);

  fclose(fp);

  fp = fopen("test-files/deserializePlayer-3.txt", "r");
  require("(before)test-files/deserializePlayer-3.txt file check", fp != NULL);

  result = deserializePlayer(fp);
  require("deserializePlayer(fp) - Test file 3", result != NULL);
  require("deserializePlayer(fp) - Test file 3 - allocation",
          isAllocated(result));
  require("deserializePlayer(fp) - Test file 3 - name",
          strcmp(result->name, "Szczesny") == 0);
  require("deserializePlayer(fp) - Test file 3 - name - allocation",
          isAllocated(result->name));
  require("deserializePlayer(fp) - Test file 3 - number", result->number == 33);

  fclose(fp);

  closeTestGroup();
}

void testCreateTeam() {
  openTestGroup("createTeam(...)");

  typedef struct {
    char *test_name;
    char *team_name_param;
    Player *members_param;
    int members_count_param;
  } createTeamErrorCase;

  Player members[] = {(Player){"Testing", 10}};

  createTeamErrorCase test_cases[] = {
      (createTeamErrorCase){"createTeam(NULL, NULL, -1)", NULL, NULL, -1},
      (createTeamErrorCase){"createTeam(\"\", NULL, -1)", "", NULL, -1},
      (createTeamErrorCase){"createTeam(\"Testing\", NULL, -1)", "Testing",
                            NULL, -1},
      (createTeamErrorCase){"createTeam(NULL, members, -1)", NULL, members, -1},
      (createTeamErrorCase){"createTeam(NULL, members, 0)", NULL, members, 0},
      (createTeamErrorCase){"createTeam(NULL, members, 1)", NULL, members, 1},
      (createTeamErrorCase){"createTeam(\"\", members, -1)", "", members, 1},
  };

  for (int i = 0; i < 7; i++) {
    createTeamErrorCase test_case = test_cases[i];
    Team *result =
        createTeam(test_case.team_name_param, test_case.members_param,
                   test_case.members_count_param);
    require(test_case.test_name, result == NULL);
  }

  Team *team = createTeam("Testing Team", members, 1);
  require("createTeam(\"Testing Team\", members, 1)", team != NULL);
  require("createTeam(\"Testing Team\", members, 1) - allocation",
          isAllocated(team));
  require("createTeam(\"Testing Team\", members, 1) - name",
          strcmp(team->name, "Testing Team") == 0);
  require("createTeam(\"Testing Team\", members, 1) - name - allocation",
          isAllocated(team->name));
  require("createTeam(\"Testing Team\", members, 1) - members_count",
          team->members_count == 1);
  require("createTeam(\"Testing Team\", members, 1) - members[0] check",
          strcmp(team->members[0].name, "Testing") == 0 &&
              team->members[0].number == 10);

  free(team->members[0].name);
  free(team->name);
  free(team);

  closeTestGroup();
}

void testCloneTeam() {
  openTestGroup("cloneTeam(...)");

  Team *result = cloneTeam(NULL);
  require("cloneTeam(NULL)", result == NULL);

  Player players[] = {(Player){"Testing", 10}};
  Team clone = {"Testing Team", players, 1};

  result = cloneTeam(&clone);
  require("cloneTeam(team)", result != NULL);
  require("cloneTeam(team) - allocation", isAllocated(result));
  require("cloneTeam(team) - name - allocation", isAllocated(result->name));
  require("cloneTeam(team) - members - allocation",
          isAllocated(result->members));
  require("cloneTeam(team) - name check",
          strcmp(result->name, "Testing Team") == 0);
  require("cloneTeam(team) - members_count check", result->members_count == 1);
  require("cloneTeam(team) - members[0] check",
          strcmp(result->members[0].name, "Testing") == 0 &&
              result->members[0].number == 10);

  closeTestGroup();
}

void testDestroyTeam() {
  openTestGroup("destroyTeam(...)");

  int result = destroyTeam(NULL);
  require("destroyTeam(NULL)", result == -1);

  char *player_name = malloc(sizeof(char) * strlen("Testing") + 1);
  player_name = strdup("Testing");
  Player *members = (Player *)malloc(sizeof(Player));
  *members = (Player){player_name, 10};

  char *team_name = malloc(sizeof(char) * strlen("Testing Team") + 1);
  team_name = strdup("Testing Team");
  Team *team = (Team *)malloc(sizeof(Team));
  *team = (Team){team_name, members, 1};

  result = destroyTeam(team);
  require("destroyTeam(team)", result == 0);
  require("destroyTeam(team) - de-allocation",
          !isAllocated(team) && !isAllocated(team_name) &&
              !isAllocated(members) && !isAllocated(player_name));

  closeTestGroup();
}

void testSerializeTeam() {
  openTestGroup("serializeTeam(...)");

  FILE *fp = fopen("test-execution-serialize.txt", "w");
  require("(before)Test file must be opened for tests to work", fp != NULL);

  Player players[] = {(Player){"Testing", 10}};
  Team team = {"Testing Team", players, 1};

  int result = serializeTeam(NULL, NULL);
  require("serializeTeam(NULL, NULL)", result == -1);

  result = serializeTeam(&team, NULL);
  require("serializeTeam(team, NULL)", result == -1);

  result = serializeTeam(NULL, fp);
  require("serializeTeam(NULL, fp)", result == -1);

  result = serializeTeam(&team, fp);
  require("serializeTeam(team, fp)", result == 0);

  fclose(fp);

  // Testing file contents for a correct serialization
  fp = fopen("test-execution-serialize.txt", "r");
  char file_row[MAX_LENGTH];
  fgets(file_row, MAX_LENGTH, fp);
  bool test_passed = strcmp(file_row, "Testing Team 1\n") == 0;

  fgets(file_row, MAX_LENGTH, fp);
  test_passed = test_passed || strcmp(file_row, "Testing 10\n") == 0;

  require("serializeTeam(team, fp) - file contents", test_passed);

  closeTestGroup();
}

void testDeserializeTeam() {
  openTestGroup("deserializeTeam(...)");

  Team *result = deserializeTeam(NULL);
  require("deserializeTeam(NULL)", result == NULL);

  FILE *fp = fopen("test-files/deserializeTeam-1.txt", "r");
  require("(before)test-files/deserializeTeam-1.txt file check", fp != NULL);

  result = deserializeTeam(fp);
  require("deserializeTeam(fp) - Test file 1", result == NULL);

  fclose(fp);

  fp = fopen("test-files/deserializeTeam-2.txt", "r");
  require("(before)test-files/deserializeTeam-2.txt file check", fp != NULL);

  result = deserializeTeam(fp);
  require("deserializeTeam(fp) - Test file 2", result == NULL);

  fclose(fp);

  fp = fopen("test-files/deserializeTeam-3.txt", "r");
  require("(before)test-files/deserializeTeam-3.txt file check", fp != NULL);

  result = deserializeTeam(fp);
  require("deserializeTeam(fp) - Test file 3", result == NULL);

  fclose(fp);

  fp = fopen("test-files/deserializeTeam-4.txt", "r");
  require("(before)test-files/deserializeTeam-4.txt file check", fp != NULL);

  Player players[] = {
      (Player){"Schuurs", 3},  (Player){"Buongiorno", 4},
      (Player){"Sanabria", 9}, (Player){"Rodriguez", 13},
      (Player){"Zapata", 91},
  };
  Team expected_result = {"Torino", players, 5};
  result = deserializeTeam(fp);
  require("deserializeTeam(fp) - Test file 4", result != NULL);
  require("deserializeTeam(fp) - Test file 4 - allocation",
          isAllocated(result));
  require("deserializeTeam(fp) - Test file 4 - name - allocation",
          isAllocated(result->name));

  bool test_passed = strcmp(result->name, expected_result.name) == 0;
  for (int i = 0; test_passed && i < 5; i++) {
    bool player_check_passed =
        strcmp(result->members[i].name, expected_result.members[i].name) == 0 &&
        result->members[i].number == expected_result.members[i].number;
    test_passed = test_passed && player_check_passed;
  }

  require("deserializeTeam(fp) - Test file 4 - contents", test_passed);

  fclose(fp);

  closeTestGroup();
}
