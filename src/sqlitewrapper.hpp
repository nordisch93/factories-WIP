#include<iostream>
#include<string>
#include<cstring>
#include"sqlite3.h"

#include"factory.hpp"

class Sqlitewrapper{

private:
    sqlite3** database_;
    bool isSlim_;

    sqlite3_stmt* generic_insert_stmt_ = NULL;
    sqlite3_stmt* generic_delete_stmt_ = NULL;
    sqlite3_stmt* generic_select_stmt_ = NULL;

public:

    /**
     * Creates a new instance of the sqlitewrapper class.
     *
     * var database:        the recipe database wrapped by this class 
     * var isSlim:          if true, statements will only be prepared when needed
     *                      if false, statements will be prepared immediatly
     *
     */
    Sqlitewrapper(sqlite3** database, bool isSlim);

    /**
     * Tries to open a database connection at the specified path and returns
     * the sqlite3 return code.
     * var filename:  the path and filename of the database
     *
     * return:  SQLITE_OK if successful
     *          errorcode else
     */
    int openDb(const char* filename);

    /**
     * Tries to create a sqlite3 database witht he given filename .
     *
     * var filename:    the filename of the new db
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int createDb(const char* filename);

    /**
     * Attempts to close the db currently opened within the wrapper by finalizing all prepared statements then closing the Db itself.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     *
     */
    int closeDb();


    /**
     * Creates a table for recipes in the database.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     */

    int createRecipeTable();

    /**
     * Adds a recipe to the database by using an INSERT INTO statement on the recipe table.
     *
     * var recipe:     the recipe to be added
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int addRecipe(Recipe recipe);

    /**
     * Deletes a recipe from the database by using a DELETE statement on the recipe table.
     *
     * var recipe_Id:  the ID of the recipe to be deleted
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int deleteRecipe(const char* name);

    /**
     * Edits a recipe from the database with updated data by calculating the difference of the entries and updating oudated data.
     *
     * var recipe:     the new recipe information to be written
     * var recipe_Id:  the ID of the recipe to be deleted
     *
     * return:          SQLITE_OK if successful
     *                  errorcode else
     */
    int editRecipe(Recipe recipe, const char* name);

    /**
     * Defragments the database by copying the db to a temporary db ignoring all free spaces and copying it back to the original db overwriting it.
     *
     * Should be called perdiodically and after drastic changes to the db structure.
     * Only successful if there are no open transactions.
     *
     * return:
     *
     */
    int vacuumDb();

};
