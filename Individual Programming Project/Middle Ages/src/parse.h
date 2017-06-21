 /** @file
  *  Interface of parser.
  *  Contains all functions needed to parse Command from input.
 */

#ifndef PARSE_H
#define PARSE_H

 /**
  * Struct of a Command.
  * @param name Name of a Command or "ERROR" if unexpected name was parsed.
  * @param data[] Necessary arguments for each Command
  */
typedef struct Command {
    char name[16];
    int data[7];
} Command;


/** Reads a Command.
  Returns Command with data points using 'Command' structure.
  */
Command* parseCommand();

#endif /* PARSE_H */
