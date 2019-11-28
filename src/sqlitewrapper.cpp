#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>
#include<sstream>
#include"sqlite3.h"
#include"sqlitewrapper.hpp"

#include"factory.hpp"


/**
    * Creates a new instance of the sqlitewrapper class.
    *
    * var database:        the contact database wrapped by this class 
    * var isSlim:          if true, statements will only be prepared when needed
    *                      if false, statements will be prepared immediatly
    *
    */
Sqlitewrapper::Sqlitewrapper(sqlite3** database, bool isSlim){
    database_ = database;
    isSlim_ = isSlim;

    if(isSlim)
        return;
    else{
        //prepare unbound generic statements to INSERT, DELETE, and SELECT a contact fromt he contact table

        const char* pzTail;

        //unbound sql statements
        const char* unbound_insert_stmnt = "INSERT INTO recipes (name, energyRequired) VALUES (@name, @energyRequired);";

        const char* unbound_delete_stmnt = "DELETE FROM @table WHERE @attribute = @value";

        const char* unbound_select_stmnt = "INSERT INTO recipes ( name, energyRequired) VALUES ('Alex', 0);";

        //prepare statements
        int prepareInsertSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_insert_stmnt,
            -1,
            &generic_insert_stmt_,
            &pzTail
        );
        int prepareDeleteSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_delete_stmnt,
            -1,
            &generic_delete_stmt_,
            &pzTail
        );
        int prepareSelectSuccess = sqlite3_prepare_v2(
            *database_,
            unbound_select_stmnt,
            -1,
            &generic_select_stmt_,
            &pzTail
        );
    }
}


/**
    * Tries to open a database connection at the specified path and returns
    * the sqlite3 return code.
    * var path:     the path and path to the database
    *
    * return:       SQLITE_OK if successful
    *               extended errorcode else
    */
int Sqlitewrapper::openDb(const char *path){
    int resultCode = sqlite3_open_v2(path, database_, SQLITE_OPEN_READWRITE, NULL);
    if(resultCode != SQLITE_OK)
        resultCode = sqlite3_extended_errcode(*database_);
    return resultCode;
}

/**
    * Tries to create a sqlite3 database with at the given path.
    *
    * var path:         the path to the new db
    *
    * return:           SQLITE_OK if successful
    *                   errorcode else
    */
int Sqlitewrapper::createDb(const char* path){
    int resultCode = sqlite3_open_v2(path, database_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if(resultCode != SQLITE_OK)
        resultCode = sqlite3_extended_errcode(*database_);
    return resultCode;
}

/**
     * Attempts to close the db currently opened within the wrapper by finalizing all prepared statements then closing the Db itself.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     *
     */
int Sqlitewrapper::closeDb(){
    if(generic_delete_stmt_ != NULL){
        if(sqlite3_finalize(generic_delete_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic delete stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    if(generic_insert_stmt_ != NULL){
        if(sqlite3_finalize(generic_insert_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic insert stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    if(generic_select_stmt_ != NULL){
        if(sqlite3_finalize(generic_select_stmt_) !=  SQLITE_OK){
            std::cout << "Could not finalize generic select stmt\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    return SQLITE_OK;
}


/**
     * Creates a table for recipes in the database.
     *
     * return:          SQLITE_OK if successful
     *                  extended errorcode else
     */
int Sqlitewrapper::createRecipeTable(){
    int stepReturnValue;
    int finalizeReturnValue;
    int prepareReturnValue;

    const char* add_table_stmnt = "CREATE TABLE IF NOT EXISTS recipes (            name TEXT PRIMARY KEY, energyRequired DOUBLE);";
    sqlite3_stmt* ppStmt;
    const char* pzTail;
    prepareReturnValue = sqlite3_prepare_v2(
        *database_,
        add_table_stmnt,
        -1,
        &ppStmt,
        &pzTail
    );
    if(prepareReturnValue == SQLITE_OK){
        std::cout << "Successfully prepared create table statement.\n";

        //execute & finalize statement
        stepReturnValue = sqlite3_step(ppStmt);
        if(stepReturnValue == SQLITE_DONE){
                std::cout << "Successfully processed create table statement.\n";

                finalizeReturnValue = sqlite3_finalize(ppStmt);
                if(finalizeReturnValue == SQLITE_OK){
                std::cout << "Successfully finalized create table statement.\n";
                return SQLITE_OK;
                }
                else{
                    return sqlite3_extended_errcode(*database_);
                }
        }
        else{
            return sqlite3_extended_errcode(*database_);
        }
    }
    else{
        return sqlite3_extended_errcode(*database_);
    }
}

/**
    * Adds a recipe to the database by using an INSERT INTO statement on the contact table. If a generic insert statement already exists it is rebound and used, otherwise a new insert statement is prepared.
    *
    * var recipe:     the recipe to be added
    *
    * return:          SQLITE_OK if successful
    *                  errorcode else
    */
int Sqlitewrapper::addRecipe(){

//     const char* name = recipe.getName().c_str();
//     double energyRequired = recipe.getEnergyRequired();
// 
//     sqlite3_stmt* insertStmt;
//     if(generic_insert_stmt_ != NULL)
//         insertStmt = generic_insert_stmt_;
//     else{
//         sqlite3_stmt* ppStmt;
//         const char* pzTail;
//         const char *insertStmntText = "INSERT INTO contacts ( name, energyRequired) VALUES (@name, @energyRequired);";
//         int prepareReturnValue = sqlite3_prepare_v2(
//             *database_,
//             insertStmntText,
//             -1,
//             &insertStmt,
//             &pzTail
//         );
//    }

    //bindReturnValue = sqlite3_bind_int(insertStmt, 1, NULL);

    //bind name, energyRequired
//     int bindReturnValue = sqlite3_bind_text(insertStmt, 1, name,-1,SQLITE_STATIC);
//     bindReturnValue = sqlite3_bind_double(insertStmt, 2, energyRequired);
// 
//     if(bindReturnValue == SQLITE_OK){
//         std::cout << "Successfully prepared insert statement.\n";
// 
//         //execute & finalize statement
//         if(sqlite3_step(insertStmt) == SQLITE_DONE){
//             std::cout << "Successfully processed insert statement.\n";
// 
//             //finalize statement if wrapper is in a mode that conserves memory
//             if(isSlim_){
//                 if(sqlite3_finalize(insertStmt) == SQLITE_OK){
//                     std::cout << "Successfully finalized insert statement.\n";
//                     return SQLITE_OK;
//                 }
//                 else{
//                     std::cout << "Statement could not be finalized.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
//                     return sqlite3_extended_errcode(*database_);
//                 }
//             }
//             //reset the statement, but don't finalize
//             else{
//                 if(sqlite3_reset(insertStmt) == SQLITE_OK){
//                     std::cout << "Successfully reset insert statement.\n";
//                     return SQLITE_OK;
//                 }
//                 else{
//                     std::cout << "Statement could not be reset.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
//                     return sqlite3_extended_errcode(*database_);
//                 }
// 
//             }
//         }
//         else{
//             std::cout << "Statement to insert could not be processed.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
//             return sqlite3_extended_errcode(*database_);
//         }
//     }
//     else{
//             std::cout << "Statement to insert could not be prepared.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
//             return sqlite3_extended_errcode(*database_);
//     }
}


/**
    * Deletes a recipe from the database by using a DELETE statement on the recipe table. If a generic delete statement already exists it is rebound and used, otherwise a new delete statement is prepared.
    *
    * var contact_Id:  the ID of the contact to be deleted
    *
    * return:          SQLITE_OK if successful
    *                  errorcode else
    */
int Sqlitewrapper::deleteRecipe(const char* name){

    const char* tableName = "recipes";
    const char* attribute = "name";

    sqlite3_stmt* deleteStmt;
    if(generic_delete_stmt_ != NULL)
        deleteStmt = generic_delete_stmt_;
    else{
        sqlite3_stmt* ppStmt;
        const char* pzTail;
        const char* deleteStmntText = "DELETE FROM @table WHERE @attribute = @value";
        int prepareReturnValue = sqlite3_prepare_v2(
            *database_,
            deleteStmntText,
            -1,
            &deleteStmt,
            &pzTail
        );
    }

    int bindReturnValue = sqlite3_bind_text(deleteStmt, 1, tableName,-1,SQLITE_STATIC);
    bindReturnValue = sqlite3_bind_text(deleteStmt, 2, attribute,-1,SQLITE_STATIC);
    bindReturnValue = sqlite3_bind_text(deleteStmt, 3, name, -1, SQLITE_STATIC);

    if(bindReturnValue == SQLITE_OK){
        std::cout << "Successfully prepared delete statement.\n";

        //execute & finalize statement
        if(sqlite3_step(deleteStmt) == SQLITE_DONE){
            std::cout << "Successfully processed delete statement.\n";

            //finalize statement if wrapper is in a mode that conserves memory
            if(isSlim_){
                if(sqlite3_finalize(deleteStmt) == SQLITE_OK){
                    std::cout << "Successfully finalized delete statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be finalized.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }
            }
            //reset the statement, but don't finalize
            else{
                if(sqlite3_reset(deleteStmt) == SQLITE_OK){
                    std::cout << "Successfully reset delete statement.\n";
                    return SQLITE_OK;
                }
                else{
                    std::cout << "Statement could not be reset.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
                    return sqlite3_extended_errcode(*database_);
                }
            }
        }
        else{
            std::cout << "Statement to delete could not be processed.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
        }
    }
    else{
            std::cout << "Statement to insert could not be prepared.\n Error code " << sqlite3_extended_errcode(*database_) << ".\n";
            return sqlite3_extended_errcode(*database_);
    }
}

/**
    * Edits a contact from the database with updated data by calculating the difference of the entries and updating oudated data. 
    *
    * var contact:     the new contact information to be written
    * var contact_Id:  the ID of the contact to be deleted
    *
    * return:          SQLITE_OK if successful
    *                  errorcode else
    */
// int Sqlitewrapper::editContact(Contact contact, int contactId){
//     return 0;
// }


