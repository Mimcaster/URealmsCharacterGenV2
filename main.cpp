/* 
 * File:   themainplusplus.cpp
 * Author: administrater1
 *
 * Created on May 28, 2017, 4:30 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include <time.h>
#include <cstdlib>
#include <string.h> //string class for c
#include <string>   //string class for c++

using namespace std;

//using namespace std;

/*  Urealms Season 3 character generator
*  Hooked up to database tables
*  Im guessing that will go under source files?
*  Starting gold is a randomly generated number
*
*/
//variables of the character being generated
    char name;
    int stamina;
    const unsigned char *race;
    const unsigned char *classname;
    const unsigned char *cornerstone;
    const unsigned char *attribute;
    int gold;

int starting_gold();       //the amount of gold the user starts with
int select_race_and_class(sqlite3 *thedatabase);
int select_cornerstone(sqlite3 *thedatabase);
int select_attribute(sqlite3 *thedatabase);
int select_inventory(sqlite3 *thedatabase);
string select_name(sqlite3 *namedatabase);

int main(int argc, char** argv) 
{

    sqlite3_initialize();       //initializes the sql library
    sqlite3 *database;
	
    int diditopen = sqlite3_open_v2("UrealmsS3Cards.db", &database, SQLITE_OPEN_READONLY, NULL);
    
    
    
    int race_index = select_race_and_class(database);
    select_cornerstone(database);
    select_attribute(database);
    
    char *sqlcommand;
    
    int startinggold = starting_gold(); //the amount of gold the user starts with
    

    
    printf("Your starting gold is %d.\n", startinggold);
        
    sqlite3_close(database);       //reallocates resources used by the database
    sqlite3_shutdown();        //reallocates the resources previously used by the sqlite library
    return 0;
    
    
    
    
    return 0;
}

int starting_gold()
{
	int diceroll = 0;
	srand(time(NULL));       //seeds the random number generator with the current time
	diceroll = (rand() % 20) + 1;   //rand is stored in the stdlib.h file. returns a random number between 0 and infinity.
        
        gold += diceroll * 10;
        
        
	if (diceroll == 20)
	{                       //user gets double gold if they get a nat crit
		gold += 200;
	}

	return gold;
}

int select_attribute(sqlite3 *thedatabase)
{
    const char *getattributenumber = "SELECT COUNT(*) FROM Attribute";
    int commandlength = sizeof(char) * strlen(getattributenumber);
    
    sqlite3_open_v2("UrealmsS3Cards.db", &thedatabase, SQLITE_OPEN_READONLY, NULL);
    
    
    sqlite3_stmt *stmt = NULL; 
    sqlite3_prepare_v2(thedatabase, getattributenumber, commandlength, &stmt, NULL);
    
    int resultcode = sqlite3_step(stmt);
    
    int numofattributes = sqlite3_column_int(stmt, 0);
    
    //printf("There are %d different attributes in URealms Season 3\n", numofattributes);
    
    sqlite3_close(thedatabase);       //reallocates resources used by the database
    
    
    int diceroll = 0;
    srand(time(NULL));
    diceroll = (rand() % numofattributes) + 1;
    
    const char *getattributetable = "SELECT * FROM Attribute WHERE AttributeNum=:attributeid;";
    commandlength = sizeof(char) * strlen(getattributetable);
    sqlite3_prepare_v2(thedatabase, getattributetable, commandlength, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, diceroll);
    
    resultcode = sqlite3_step(stmt);
    
    const unsigned char *getattrib = sqlite3_column_text(stmt, 1);
    stamina += sqlite3_column_int(stmt, 2);
    
    attribute = getattrib;
    
    printf("You have rolled the %s attribute\n", getattrib);
    printf("Your stamina is now at %d\n", stamina);
    
    
    return 0;
}

int select_cornerstone(sqlite3 *thedatabase)
{   //Tablename: Cornerstone    Values: CornerstoneID, CornerstoneName, Stamina
    
    sqlite3_open_v2("UrealmsS3Cards.db", &thedatabase, SQLITE_OPEN_READONLY, NULL);
    
    int numcornerstone = 1;
    const char *getcornerstonecommand = "SELECT COUNT(*) FROM Cornerstone;";
    int commandlength = sizeof(char) * strlen(getcornerstonecommand);
    
    sqlite3_stmt *stmt = NULL;
    
    sqlite3_prepare_v2(thedatabase, getcornerstonecommand, commandlength, &stmt, NULL);
    
    int resultcode = sqlite3_step(stmt);
    
    printf("Result code %d\n", resultcode);
    numcornerstone = sqlite3_column_int(stmt, 0);
    
    printf("There are %d cornerstone cards in URealms S3.\n", numcornerstone);
    
    int diceroll = 0;
    srand(time(NULL));       //seeds the random number generator with the current time
    diceroll = (rand() % numcornerstone) + 1;   //rand is stored in the stdlib.h file. returns a random number between 0 and infinity.
   
    
    const char *getrandomcornerstone = "SELECT * FROM Cornerstone WHERE CornerstoneID=:cornerstoneid;";
    commandlength = sizeof(char) * strlen(getrandomcornerstone);
    
    sqlite3_prepare_v2(thedatabase, getrandomcornerstone, commandlength, &stmt, NULL);
    
    sqlite3_bind_int(stmt, 1, diceroll);
    sqlite3_step(stmt);
    
    unsigned const char *cornerstonename = sqlite3_column_text(stmt, 1);
    
    printf("He has the cornerstone %s\n", cornerstonename);
    stamina += sqlite3_column_int(stmt, 2);
   
}

int select_race_and_class(sqlite3 *thedatabase)
{
    const char *getracecommand = "SELECT COUNT(*) FROM Class;";
    int commandlength = sizeof(char) * strlen(getracecommand);

    int numclasses = 1;
    sqlite3_open_v2("UrealmsS3Cards.db", &thedatabase, SQLITE_OPEN_READONLY, NULL);
    
    
    sqlite3_stmt *stmt = NULL;      //sqlite3 statement, gets intializes in the prepare command
    sqlite3_prepare_v2(thedatabase, getracecommand, commandlength, &stmt, NULL);

    sqlite3_step(stmt);

    numclasses = sqlite3_column_int(stmt, 0);
    
    //printf("There are %d classes\n", numclasses);
    
    srand(time(NULL));       //seeds the random number generator with the current time
    int diceroll = (rand() % numclasses) + 1;   //rand is stored in the stdlib.h file. returns a random number between 0 and infinity.
    
    int resultcode;
    
    
    
    
    //creates a statement with a bound parameter
    const char *getclass = "SELECT * FROM Class WHERE ClassNum=:classid;";
    commandlength = sizeof(char) * strlen(getclass);

    sqlite3_prepare_v2(thedatabase, getclass, commandlength, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, diceroll);
    
    sqlite3_extended_result_codes(thedatabase, 1);          //returns extended errorcode    
    resultcode = sqlite3_step(stmt);
    //resultcode = sqlite3_extended_errcode(thedatabase);
    
    //printf("error code %d\n", resultcode);
    stamina += sqlite3_column_int(stmt, 1);
    gold += sqlite3_column_int(stmt, 2);
    classname = sqlite3_column_text(stmt, 0);
    
    //printf("The stamina from class %s is %d, and your gold is %d\n", classname, stamina, gold);
    printf("Your class is %s.\n", classname);

    
    const char *getracenum = "SELECT COUNT(*) FROM Race;";
    commandlength = sizeof(char) * strlen(getracenum);
    sqlite3_prepare_v2(thedatabase, getracenum, commandlength, &stmt, NULL);
    
    sqlite3_step(stmt);
    
    int racenum = sqlite3_column_int(stmt, 0);
    
    //printf("There are %d races\n", racenum);
    
    srand(time(NULL));       //seeds the random number generator with the current time
    diceroll = (rand() % racenum) + 1;   //rand is stored in the stdlib.h file. returns a random number between 0 and infinity.
    
    
    const char *getrace = "SELECT * FROM Race WHERE RaceNum=:raceid;";
    commandlength = sizeof(char) * strlen(getrace);
    
    sqlite3_prepare_v2(thedatabase, getrace, commandlength, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, diceroll);
    
    resultcode = sqlite3_step(stmt);
    
    stamina += sqlite3_column_int(stmt, 1);
    gold += sqlite3_column_int(stmt,2);
    race = sqlite3_column_text(stmt,0);
    const unsigned char *description = sqlite3_column_text(stmt, 3);
    
    printf("%s\n", description);
    //printf("Your race is %s, and you now have %d gold and %d stamina.\n", race, gold, stamina);
    printf("Your race is %s.\n", race, gold, stamina);

    sqlite3_close(thedatabase);       //reallocates resources used by the database
    
    return 0;
}

string generate_name(sqlite3 *namedatabase)
{

    string charname = "kyle";
    return charname;
}