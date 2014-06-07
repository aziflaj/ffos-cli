/**
 * ffos.h header file
 *
 * This header file contains the functions used by ffos-cli
 * about creating a new project, adding source files, and
 * edditing the manifest file
 *
 * @version 0.1
 * @author Aldo Ziflaj
 */

#ifndef FFOS_H
#define FFOS_H

#define FFOS_PRO 1
#define NOT_FFOS_PRO 0
#define FFOS_VERSION "0.1"
#include <stdio.h>

/**
 * This function is used to create a new Firefox OS application
 * @param argc The number of arguments passed (from cli)
 * @param argv The list of arguments passed(from cli)
 */
void create(int argc,char **argv);

/**
 * This function is used to add source files or permisions to the
 * FFOS application
 * @param argc The number of arguments passed (from cli)
 * @param argv The list of arguments passed(from cli)
 */
void add(int argc,char **args);

/**
 * This function is used to edit the manifest file of the application
 * @param argc The number of arguments passed (from cli)
 * @param argv The list of arguments passed(from cli)
 */
void set(int argc,char **args);

/**
 * This function is used to delete a ffos application 
 * @param argc The number of arguments passed (from cli)
 * @param argv The list of arguments passed(from cli)
 */
void delete(int argc,char **args);

/**
 * This function prints the help page
 */
void help();

/**************** ffos helper functions ****************/

/**
 * This helper function checks if a directory is a ffos project
 * @return FFOS_PRO if the project is a ffos project, 
 * otherwise NOT_FFOS_PRO 
 */
int check();
#endif /* ffos.h */
